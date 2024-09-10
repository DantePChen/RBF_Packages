#include "testMTopologyEvaluator.h"
#include <maya/MDataHandle.h>
#include <maya/MEvaluationNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MGraphNodeIterator.h>

#include <maya/MPlugArray.h>
#include <maya/MProfiler.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>

#include <functional>

/*  This plugin is designed to test different features of topology evaluators.

    Once plugin is loaded it adds a new DG node (TestMTopologyEvaluator_Node) 
    and a new Topology Evaluator (TestMTopologyEvaluator).

    TestMTopologyEvaluator_Node is a simple node with same number of inputs as
    outputs. Each output is a function of input with same index and constant
    offset 5. Each call to compute is counted by internal computeCount.
    Compute count is reset during preEvaluation and should match the number of
    outputs when TestMTopologyEvaluator is disabled or in Evaluation mode.

    TestMTopologyEvaluator is designed to grab every TestMTopologyEvaluator_Node
    and express each input-output pair as a separate evaluation node with
    custom compute method. Compute can happen in two modes controlled by
    attrDoPushCompute:
    - Evaluation with DG mode - in this mode we should get exactly the same
      answer as when evaluator is disabled, i.e. output values with be computed
      by DG compute method and compute count will match the number of outputs.
      What will be different is scheduling, since topology evaluator will
      represent single TestMTopologyEvaluator_Node as a graph. Computation of
      outputs will happen in a separate evaluation node, each output will be computed
      sequentially...but downstream of each output will start earlier, i.e. as soon
      as output is ready. In this mode pushComputeCount should be equal 0.
    - Push mode - in this mode DG compute is not used. Each output is computed
      concurrently by a separate evaluation node. To catch potential errors
      computed value will adding different static offset to value from input
      (i.e. +9). As well, compute count should in this mode be equal 0 and
      push compute count should be equal to total output count.
      In this mode, evaluators are only allowed to read input data and push
      data to outputs. No DG compute is allowed (this is why we expect compute count to be 0).
*/

namespace
{
    // Number of input & output attributes to generate on test plugin
    // More attributes will generate more evaluation nodes in topology evaluator
    // and allow more chances for race condition (if something went wrong)
    const unsigned  _numberOfAttr = 100;

    // Profiler category name for this test
    int             _profilerCategory = MProfiler::addCategory("testMTopologyEvaluator", "Events related to the testMTopologyEvaluator");

    MEvaluationNode getEvaluationNodeConnectedToInput(MObject& attrInput, MEvaluationNode& ownerEvaluationNode)
    {
        MStatus stat = MS::kSuccess;

        MFnDependencyNode testNode(ownerEvaluationNode.dependencyNode(&stat));
        MPlug thisPlug = testNode.findPlug(attrInput, true, &stat);
        if (thisPlug.isNull())
            return ownerEvaluationNode;

        MPlug srcPlug = thisPlug.source();
        if (srcPlug.isNull())
            return ownerEvaluationNode;

        for(int i = 0; i < ownerEvaluationNode.parentCount(); i++)
        {
            MEvaluationNode parentNode = ownerEvaluationNode.parent(i);
            if (srcPlug.node() == parentNode.dependencyNode(&stat))
            {
                return parentNode;
            }
        }

        return ownerEvaluationNode;
    }

    MEvaluationNode getEvaluationNodeConnectedToOutput(MObject& attrOutput, MEvaluationNode& ownerEvaluationNode)
    {
        MStatus stat = MS::kSuccess;

        MFnDependencyNode testNode(ownerEvaluationNode.dependencyNode(&stat));
        MPlug thisPlug = testNode.findPlug(attrOutput, true, &stat);
        if (thisPlug.isNull())
            return ownerEvaluationNode;

        MPlugArray dstPlugs;
        if ( !thisPlug.destinationsWithConversions(dstPlugs) || dstPlugs.length() != 1)
            return ownerEvaluationNode;

        MPlug dstPlug = dstPlugs[0];
        for (int i = 0; i < ownerEvaluationNode.childCount(); i++)
        {
            MEvaluationNode childNode = ownerEvaluationNode.child(i);
            if (dstPlug.node() == childNode.dependencyNode(&stat))
            {
                return childNode;
            }
        }

        return ownerEvaluationNode;
    }
}

MTypeId	TestMTopologyEvaluator_Node::id( 0x80000, 0x7d );

std::vector<MObject> TestMTopologyEvaluator_Node::attrInputs;
std::vector<MObject> TestMTopologyEvaluator_Node::attrOutputs;

MObject	TestMTopologyEvaluator_Node::attrComputeCount;
MObject	TestMTopologyEvaluator_Node::attrPushComputeCount;
MObject	TestMTopologyEvaluator_Node::attrDoPushCompute;

//----------------------------------------------------------------------

MPxNode::SchedulingType TestMTopologyEvaluator_Node::schedulingType() const
{
	return MPxNode::kParallel;
}

//----------------------------------------------------------------------

MStatus TestMTopologyEvaluator_Node::compute( const MPlug& plug, MDataBlock& data )
{
	
    for (int i = 0; i < attrOutputs.size(); i++)
    {
        auto& outputPlug = attrOutputs[i];
        if (plug != outputPlug)
            continue;

        double inputOutputOffset = 5;	// This number is used by the test; no other reason for it

        MStatus stat;
        MDataHandle inputDouble = data.inputValue(attrInputs[i], &stat);
        MDataHandle outputDouble = data.outputValue(attrOutputs[i], &stat);

        double outputVal = inputDouble.asDouble();
        outputVal += inputOutputOffset;
        outputDouble.setDouble(outputVal);

        stat = data.setClean(attrOutputs[i]);

        computeCount++;

        return stat;
    }

    return MStatus::kUnknownParameter;
}
//----------------------------------------------------------------------

MStatus TestMTopologyEvaluator_Node::setDependentsDirty(const MPlug& plug, MPlugArray& plugArray)
{
    for (int i = 0; i < attrOutputs.size(); i++)
    {
        auto& outputPlug = attrOutputs[i];
        if (plug != outputPlug)
            continue;

        computeCount = 0;
        break;
    }

    return ParentClass::setDependentsDirty(plug, plugArray);
}
//----------------------------------------------------------------------

MStatus	TestMTopologyEvaluator_Node::preEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode)
{
    computeCount = 0;

    return ParentClass::preEvaluation(context, evaluationNode);

}

//----------------------------------------------------------------------

bool TestMTopologyEvaluator_Node::setInternalValue(const MPlug& plug, const MDataHandle& handle)
{
    if (plug == attrDoPushCompute)
    {
        doPushComputeOverride = handle.asBool();
        return true;
    }
    return ParentClass::setInternalValue(plug, handle);
}

bool TestMTopologyEvaluator_Node::getInternalValue(const MPlug& plug, MDataHandle& handle)
{
    if (plug == attrComputeCount)
    {
        handle.setInt(computeCount);
        return true;
    }
    else if (plug == attrPushComputeCount) {
        handle.setInt(pushComputeCount);
        return true;
    }
    else if (plug == attrDoPushCompute) {
        handle.setBool(doPushComputeOverride);
        return true;
    }

    return ParentClass::getInternalValue(plug, handle);
}

//----------------------------------------------------------------------

void* TestMTopologyEvaluator_Node::creator()
{
	return new TestMTopologyEvaluator_Node();
}

//----------------------------------------------------------------------

MStatus TestMTopologyEvaluator_Node::initialize() {
    MStatus stat;
    MString attrLongName;
    MString attrShortName;
    MFnNumericAttribute mAttr;

    attrInputs.reserve(_numberOfAttr);
    for (auto i = 0; i < _numberOfAttr; i++)
    {
        attrLongName = "input";
        attrShortName = "in";
        attrLongName += (i+1);
        attrShortName += (i+1);

        attrInputs.emplace_back(mAttr.create(attrLongName, attrShortName, MFnNumericData::kDouble, 0, &stat));
        mAttr.setKeyable(true);
    }

    attrOutputs.reserve(_numberOfAttr);
    for (auto i = 0; i < _numberOfAttr; i++)
    {
        attrLongName = "output";
        attrShortName = "out";
        attrLongName += (i+1);
        attrShortName += (i+1);

        attrOutputs.emplace_back(mAttr.create(attrLongName, attrShortName, MFnNumericData::kDouble, 0, &stat));
        mAttr.setWritable(false);
        mAttr.setStorable(false);
    }

    attrLongName = "computeCount";
    attrShortName = "cct";
    attrComputeCount = mAttr.create(attrLongName, attrShortName, MFnNumericData::kInt, 0, &stat);
    mAttr.setReadable(true);
    mAttr.setStorable(false);
    mAttr.setInternal(true);
    mAttr.setWritable(false);
    mAttr.setKeyable(false);
    mAttr.setConnectable(false);

    attrLongName = "pushComputeCount";
    attrShortName = "pcc";
    attrPushComputeCount = mAttr.create(attrLongName, attrShortName, MFnNumericData::kInt, 0, &stat);
    mAttr.setReadable(true);
    mAttr.setStorable(false);
    mAttr.setInternal(true);
    mAttr.setWritable(false);
    mAttr.setKeyable(false);
    mAttr.setConnectable(false);

    attrLongName = "doPushCompute";
    attrShortName = "dpc";
    attrDoPushCompute = mAttr.create(attrLongName, attrShortName, MFnNumericData::kBoolean, false, &stat);
    mAttr.setReadable(true);
    mAttr.setStorable(true);
    mAttr.setInternal(true);
    mAttr.setWritable(true);
    mAttr.setKeyable(false);
    mAttr.setConnectable(false);

    for (auto i = 0; i < _numberOfAttr; i++)
    {
        stat = addAttribute(attrInputs[i]);
        stat = addAttribute(attrOutputs[i]);

        stat = attributeAffects(attrInputs[i], attrOutputs[i]);
    }

    stat = addAttribute(attrComputeCount);
    stat = addAttribute(attrPushComputeCount);
    stat = addAttribute(attrDoPushCompute);

	return MS::kSuccess;
}

//----------------------------------------------------------------------
MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "Autodesk", "3.0", "Any");
	status = plugin.registerNode( "TestMTopologyEvaluator_Node", TestMTopologyEvaluator_Node::id, TestMTopologyEvaluator_Node::creator, TestMTopologyEvaluator_Node::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}
    
    status = plugin.registerTopologyEvaluator("TestMTopologyEvaluator", 70 /* unused */, TestMTopologyEvaluator_Evaluator::creator);
    if (!status)
    {
        status.perror("registerEvaluator");
        return status;
    }

	return status;
}

//----------------------------------------------------------------------
MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );
	
    status = plugin.deregisterTopologyEvaluator("TestMTopologyEvaluator");
    if (!status)
    {
        status.perror("deregisterEvaluator");
        return status;
    }
    
    status = plugin.deregisterNode(TestMTopologyEvaluator_Node::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}
	return status;
}

// =====================================================================

TestMTopologyEvaluator_Evaluator::~TestMTopologyEvaluator_Evaluator()
{

}

//----------------------------------------------------------------------
bool TestMTopologyEvaluator_Evaluator::markIfSupported(const MEvaluationNode* node)
{
    MStatus stat = MS::kSuccess;
    MObject owner = node->dependencyNode(&stat);
    if ((stat == MS::kSuccess) && owner.hasFn(MFn::kDependencyNode))
    {
        MFnDependencyNode depNode(owner);
        return (depNode.typeId() == TestMTopologyEvaluator_Node::id);
    }

    return false;
}

//----------------------------------------------------------------------
MPxTopologyEvaluator* TestMTopologyEvaluator_Evaluator::creator()
{
    return new TestMTopologyEvaluator_Evaluator();
}

//----------------------------------------------------------------------
bool TestMTopologyEvaluator_Evaluator::makeTopology(MTopologyClusterNode* cluster)
{
    MStatus stat = MS::kSuccess;
    MGraphNodeIterator iterator(cluster, &stat);
    for (; (stat == MS::kSuccess) && !iterator.isDone(); iterator.next(&stat))
    {
        MEvaluationNode currEvalNode = iterator.currentEvaluationNode(&stat);
        if (stat == MS::kSuccess)
        {
            MFnDependencyNode testNodeFn(currEvalNode.dependencyNode(&stat));
            TestMTopologyEvaluator_Node* testNode = static_cast<TestMTopologyEvaluator_Node*>(testNodeFn.userNode());
            if (!testNode)
                continue;

            std::function<void()> initExecFn = [testNode, currEvalNode]()
            {
                MProfilingScope profilingScopeForInit(_profilerCategory, MProfiler::kColorD_L1, "Reset counters");
                testNode->computeCount = 0;
                testNode->pushComputeCount = 0;
            };
            MEvaluationNode initExec = cluster->createNode("InitExec", initExecFn, MPxNode::kParallel, nullptr, false);
            MEvaluationNode endExec = cluster->createNode("EndExec", currEvalNode, MPxNode::kParallel);

            if (testNode->doPushComputeOverride)
            {
                for (int i = 0; i < TestMTopologyEvaluator_Node::attrOutputs.size(); i++)
                {
                    MObject inputAttr = TestMTopologyEvaluator_Node::attrInputs[i];
                    MObject outputAttr = TestMTopologyEvaluator_Node::attrOutputs[i];

                    MString nodeName = "PushCompute_";
                    nodeName += (i + 1);
                    std::function<void()> pushComputeFn = [currEvalNode, inputAttr, outputAttr, testNode, nodeName]()
                    {
                        MProfilingScope profilingScopeForCompute(_profilerCategory, MProfiler::kColorD_L1, nodeName.asChar());

                        MStatus computeStat = MS::kSuccess;
                        MDataBlock data = currEvalNode.datablock(&computeStat);
                        MDataHandle inputDouble = data.inputValue(inputAttr, &computeStat);
                        MDataHandle outputDouble = data.outputValue(outputAttr, &computeStat);

                        double inputOutputOffsetForPush = 9;	// This number is used by the test; no other reason for it

                        double outputVal = inputDouble.asDouble();
                        outputVal += inputOutputOffsetForPush;
                        outputDouble.setDouble(outputVal);

                        computeStat = data.setClean(outputAttr);

                        testNode->pushComputeCount++;
                    };

                    MEvaluationNode parentNode = getEvaluationNodeConnectedToInput(inputAttr, currEvalNode);
                    MEvaluationNode childNode = getEvaluationNodeConnectedToOutput(outputAttr, currEvalNode);
                    if (parentNode.dependencyNode() != currEvalNode.dependencyNode() && currEvalNode.dependencyNode() != childNode.dependencyNode())
                    {
                        MEvaluationNode inConnection = cluster->connectionPort(parentNode, currEvalNode);
                        MEvaluationNode outConnection = cluster->connectionPort(childNode, currEvalNode);

                        MEvaluationNode pushComputeExec = cluster->createNode(nodeName.asChar(), pushComputeFn, MPxNode::kParallel, &currEvalNode, false);
                        initExec.connect(pushComputeExec);      //init will always execute before any other node in this cluster
                        inConnection.connect(pushComputeExec);  //make push compute dependent on upstream dependency of inputAttr
                        pushComputeExec.connect(outConnection); //make outputAttr downstream dependent on push computation 
                        pushComputeExec.connect(endExec);       //make end execution dependent on every push computation
                    }
                }
            }
            else
            {
                MEvaluationNode lastCompute = initExec;
                for (int i = 0; i < TestMTopologyEvaluator_Node::attrOutputs.size(); i++)
                {
                    MObject inputAttr = TestMTopologyEvaluator_Node::attrInputs[i];
                    MObject outputAttr = TestMTopologyEvaluator_Node::attrOutputs[i];

                    MString nodeName = "FineGrainCompute_";
                    nodeName += (i+1);
                    std::function<void()> computeFn = [currEvalNode, outputAttr, nodeName]()
                    {
                        MProfilingScope profilingScopeForCompute(_profilerCategory, MProfiler::kColorD_L1, nodeName.asChar());

                        MStatus computeStat = MS::kSuccess;
                        MDataBlock data = currEvalNode.datablock(&computeStat);
                        data.inputValue(outputAttr, &computeStat);  // pull output to cause DG compute
                    };

                    MEvaluationNode parentNode = getEvaluationNodeConnectedToInput(inputAttr, currEvalNode);
                    MEvaluationNode childNode = getEvaluationNodeConnectedToOutput(outputAttr, currEvalNode);
                    if (parentNode.dependencyNode() != currEvalNode.dependencyNode() && currEvalNode.dependencyNode() != childNode.dependencyNode())
                    {
                        MEvaluationNode inConnection = cluster->connectionPort(parentNode, currEvalNode);
                        MEvaluationNode outConnection = cluster->connectionPort(childNode, currEvalNode);

                        MEvaluationNode computeExec = cluster->createNode(nodeName.asChar(), computeFn, MPxNode::kParallel, &currEvalNode, true);
                        lastCompute.connect(computeExec);       //computation with DG node at fine-grained level need to be serialized
                        inConnection.connect(computeExec);      //make compute dependent on upstream dependency of inputAttr
                        computeExec.connect(outConnection);     //make outputAttr downstream dependent on computation 
                        computeExec.connect(endExec);           //make end execution dependent on every computation

                        lastCompute = computeExec;
                    }
                }
            }

            MEvaluationGraph evalGraph = MEvaluationGraph::ownerGraph(currEvalNode);
            MEvaluationNode rootConnection = cluster->connectionPort(evalGraph.rootNode(), evalGraph.rootNode());
            rootConnection.connect(initExec);           //make init only dependent on ROOT (will start as soon as execution starts)
        }
    }

    return true;
}

// =====================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// =====================================================================
