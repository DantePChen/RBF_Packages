//-
// ===========================================================================
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
//
// ===========================================================================
//+

/*
# This custom evaluator is used to replace evaluation clusters consisting of 
# point and orient constraints. Since cycles are introduced between those 
# constraints, evaluation inefficiency can arise. By replacing those clusters 
# with a custom evaluator we are able to evaluate the position and orientation
# of the given object faster.
#
# The code sample below creates a cone that is point and orient constrained
# to an animated locator. The evaluator detects this setup and claims 
# the transform for evaluation.
#
# During evaluation, the evaluator does the requisite math that the 
# constraints would normally do, thus saving the cost of computation
# of the constraints 

# NOTE: This is a limited example and is not intended for a more complex
# setup. The setup is, while common, deliberately simple. The evaluator
# checks only if a source transform is basically constrained by a 
# point and orient constraint. It doesn't check additions to this scenario
# (offsets, constraint axes, etc). 

# setup the scene 
import maya.cmds as cmds
import math

def getConstraintSetup():
    constraintPair = {} # 0-src, 1-dst
    constraintPair[0] = cmds.spaceLocator()[0]
    constraintPair[1] = cmds.polyCone()[0]

    cmds.select(constraintPair.values(), r=True)
    cmds.pointConstraint(offset=[0,0,0], weight=1)
    cmds.orientConstraint(offset=[0,0,0], weight=1)
    cmds.select(clear=True)
    
    return constraintPair

def animate(transform, step, ampT, ampR):
    cmds.setKeyframe( transform, attribute='translateX', t='0sec', v=0 )
    cmds.setKeyframe( transform, attribute='translateZ', t='0sec', v=0 )
    cmds.setKeyframe( transform, attribute='translateX', t='2sec', v=(math.cos(step) * ampT) )
    cmds.setKeyframe( transform, attribute='translateZ', t='2sec', v=(math.sin(step) * ampT) )
    
    cmds.setKeyframe( transform, attribute='rotateX', t='0sec', v=0 )
    cmds.setKeyframe( transform, attribute='rotateZ', t='0sec', v=0 )
    cmds.setKeyframe( transform, attribute='rotateX', t='2sec', v=ampR )
    cmds.setKeyframe( transform, attribute='rotateZ', t='2sec', v=ampR )

def createPointOrientSetup(numberOfInstances):
    increment = 360.0/numberOfInstances
    step = 0
    for i in range(numberOfInstances):
        constraintPair = getConstraintSetup()
        animate(constraintPair[0], math.radians(step), 15, 180)
        step += increment
        
# Create 100 constrainted objects with point and orient constraints
createPointOrientSetup(100)

# load and register the evaluator
cmds.loadPlugin("constraintEvaluator.mll")
cmds.evaluator(enable=True, name="ConstraintEvaluator")
cmds.evaluator(query=True, name="ConstraintEvaluator", clusters=True)

# clear the scene, de-register and unload the evaluator
cmds.file(force=True, new=True)
cmds.evaluator(enable=False, name="ConstraintEvaluator")
cmds.unloadPlugin("constraintEvaluator")
*/

#include <maya/MFnPlugin.h>
#include <maya/MString.h>

#include <maya/MPxCustomEvaluator.h>
#include <maya/MCustomEvaluatorClusterNode.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>
#include <maya/MProfiler.h>
#include <maya/MGraphNodeIterator.h>
#include <maya/MFnMatrixData.h>
#include <maya/MPlugArray.h>
#include <maya/MPxTransform.h>
#include <maya/MObjectHandle.h>

#include <map>

namespace
{
    int _profilerCategory = MProfiler::addCategory("Constraint Evaluator", "Events from the EM constraint evaluator");
}

///////////////////////////////////////////////////
//
// Evaluator class declaration
//
///////////////////////////////////////////////////
class constraintEvaluator : public MPxCustomEvaluator
{
public:
    typedef std::map<unsigned int, MObject> TargetMapHash;

	virtual ~constraintEvaluator();

    bool markIfSupported(const MEvaluationNode* node) override;
    
    MCustomEvaluatorClusterNode::SchedulingType schedulingType(const MCustomEvaluatorClusterNode* cluster) override;
    
    void clusterEvaluate(const MCustomEvaluatorClusterNode* cluster) override;
    void clusterTerminate(const MCustomEvaluatorClusterNode* cluster) override;

	static MPxCustomEvaluator* creator();

    void reset();

    TargetMapHash fTransformTargetHash;
};

// reset any data we're storing
void constraintEvaluator::reset()
{
    fTransformTargetHash.clear();
}

// Check the provided node (and its local network) to see
// if it's something we can support. In this case, we want to claim
// the transform that's constrained to a target by two constraints. Once
// we've ascertained that the provided node is the one we want, we'll store
// it (via a hashcode) and its target node.
bool constraintEvaluator::markIfSupported(const MEvaluationNode* node)
{
    MStatus stat;
    MObject thisNode = node->dependencyNode(&stat);

    if (stat == MS::kSuccess)
    {
        MFnDependencyNode depNodeFn(thisNode, &stat);
        MString nodeName = depNodeFn.name();

        if (depNodeFn.typeName() == "transform")
        {
            // check the source of any connections to the translate
            MPlug translateXPlug = depNodeFn.findPlug("translateX",  true,  &stat);

            if (stat != MS::kSuccess) return false;

            MPlugArray sources;
            translateXPlug.connectedTo(sources, true /* asDst */, false /* asSrc */, &stat);

            if (stat != MS::kSuccess) return false;
            if (sources.length() != 1) return false;
            if (sources[0].node().hasFn(MFn::kPointConstraint) != true) return false; // is this a point constraint?

            // store the point constraint
            MFnDependencyNode pointConstraintNode(sources[0].node());

            sources.clear();
            MPlug rotateXPlug = depNodeFn.findPlug("rotateX",  true,  &stat);

            if (stat != MS::kSuccess) return false;

            rotateXPlug.connectedTo(sources, true /* asDst */, false /* asSrc */, &stat);

            if (stat != MS::kSuccess) return false;
            if (sources.length() > 1) return false;
            if (sources[0].node().hasFn(MFn::kOrientConstraint) != true) return false; // is this an orient constraint?

            // store the orient constraint
            MFnDependencyNode orientConstraintNode(sources[0].node());

            // check if the point and orient constraint have the same target
            MPlug pointConstraintTargetTranslatePlug = pointConstraintNode.findPlug("targetTranslate", true, &stat);

            if (stat != MS::kSuccess) return false;

            MFnDependencyNode sourceNode(pointConstraintTargetTranslatePlug.source().node(), &stat);

            if (stat != MS::kSuccess) return false;

            MString targetNodeName = sourceNode.name(); // we'll need this target's name to see if the other constraint uses this

            MPlug orientConstraintTargetRotatePlug = orientConstraintNode.findPlug("targetRotate", true, &stat);

            if (stat != MS::kSuccess) return false;

            stat = sourceNode.setObject(orientConstraintTargetRotatePlug.source().node());

            if (stat != MS::kSuccess) return false;
            if (targetNodeName != sourceNode.name()) return false;

            fTransformTargetHash[MObjectHandle::objectHashCode(thisNode)] = orientConstraintTargetRotatePlug.source().node();

            // this is a transform constrained by a point and an orient constraint, claim it
            return true;
        }
        else if ((depNodeFn.typeName() == "pointConstraint") || (depNodeFn.typeName() == "orientConstraint"))
        {
            // Marking all point and orient constraints as supported since they will 
            // always be in a cycle with destination node, and we will be able to 
            // override the entire cycle only if we mark all nodes forming it. When 
            // processing destination we will test connectivity and decide if we support 
            // this cycle cluster
            return true;
        }
    }  

    return false;
}

// return the scheduling type of the provided cluster
MCustomEvaluatorClusterNode::SchedulingType constraintEvaluator::schedulingType(const MCustomEvaluatorClusterNode* cluster)
{
    return cluster->schedulingType();
}

// During evaluation, we first make sure we're called on our cone's transform
// and grab the information we need from its target node, as well as some of
// its attributes. We then make the appropriate calculations and evaluate only
// the transform node (thus avoiding evaluating the constraints).
void constraintEvaluator::clusterEvaluate(const MCustomEvaluatorClusterNode* cluster)
{
    MProfilingScope profilingScope(_profilerCategory, MProfiler::kColorD_L1, "Evaluate constraint cluster");
	
	MStatus stat = MS::kSuccess;
    MGraphNodeIterator iterator(cluster, &stat);

    if (stat == MS::kSuccess)
    {
        while (!iterator.isDone())
        {
            iterator.next(&stat);

            MEvaluationNode currEvalNode = iterator.currentEvaluationNode(&stat);
            MObject currObject = currEvalNode.dependencyNode(&stat);

			if ( currObject.hasFn(MFn::kTransform) )
            {
				MMatrix piMatrix, wMatrix;

                // We're forcing a call to pre-evaluate on the current node.
                // The idea here is to ask an attribute on the given transform
                // for its dirty status to force a call to that node's preEvaluate(...)
                // call. This is typically not required, however in the case of
                // transform nodes, in order for them to *not* re-dirty and re-evaluate
                // anything, we force a pre-evaluation before setting anything in
                // the plug and/or datablock
				MPlug worldMatrixForPreEval(currObject, MPxTransform::worldMatrix);
				currEvalNode.datablock(&stat).isClean(worldMatrixForPreEval);

				{
					MProfilingScope profilingScopeParent(_profilerCategory, MProfiler::kColorD_L1, "Get parent inverse matrix");

					// Read parent inverse matrix. Use inputValue call to get the most up to 
                    // date value and allow preEvaluation to happen before we write anything 
                    // back to data block.
					MPlug parentInverseMatrixPlug(currObject, MPxTransform::parentInverseMatrix);
					MArrayDataHandle parentInverseMatrixArrayHandle = currEvalNode.datablock(&stat).inputArrayValue(parentInverseMatrixPlug.attribute());
					if ((stat != MS::kSuccess) || (parentInverseMatrixArrayHandle.elementCount() == 0)) continue;

					// We don't support instancing for now, so grab only first instance
					stat = parentInverseMatrixArrayHandle.jumpToElement(0); 
					if (stat != MS::kSuccess) continue;

					MDataHandle elementHandle = parentInverseMatrixArrayHandle.inputValue(&stat);
					if (stat != MS::kSuccess) continue;

					MObject parentInverseMatrixData = elementHandle.data();
					MFnMatrixData fnMatrixData(parentInverseMatrixData, &stat);

					if (stat != MS::kSuccess) continue;
				
					piMatrix = fnMatrixData.matrix(&stat);
				}

				{
					// Get target world matrix
					MProfilingScope profilingScopeTarget(_profilerCategory, MProfiler::kColorD_L1, "Get target matrix");
					
                    MObject targetObject = fTransformTargetHash[MObjectHandle::objectHashCode(currObject)];

					if (stat != MS::kSuccess) continue;

					MPlug worldMatrixPlug(targetObject, MPxTransform::worldMatrix);
					MPlug worldMatrixPlugElement = worldMatrixPlug.elementByLogicalIndex(0, &stat);
					
                    if (stat != MS::kSuccess) continue;

					MDataHandle targetWorldMatrixHandle = worldMatrixPlugElement.asMDataHandle();
					MObject targetWorldMatrixData = targetWorldMatrixHandle.data();
					MFnMatrixData fnTargetWorldMatrixData(targetWorldMatrixData, &stat);
					worldMatrixPlugElement.destructHandle(targetWorldMatrixHandle);

					if (stat != MS::kSuccess) continue;

					wMatrix = fnTargetWorldMatrixData.matrix(&stat);
				}
				
				{
					// We don't support in this simple example offsets
					MProfilingScope profilingScopeDestination(_profilerCategory, MProfiler::kColorD_L1, "Compute and write TR");

                    MMatrix currLocalMatrix = wMatrix * piMatrix;
					MTransformationMatrix transformer(currLocalMatrix);
                    MVector translate = transformer.getTranslation(MSpace::kTransform, &stat);

					if (stat != MS::kSuccess) continue;

					double rotateVals[3];
					MTransformationMatrix::RotationOrder ro = MTransformationMatrix::kXYZ;
					stat = transformer.getRotation(rotateVals, ro);
					if (stat != MS::kSuccess) continue;

					// Put the computed data back to data block. We'll set the 
                    // translate and rotate plugs directly since these will affect
                    // the world space matrix (which we can't set directly).
					MDataBlock currDataBlock = currEvalNode.datablock(&stat);
					if (stat != MS::kSuccess) continue;

					MPlug tPlug(currObject, MPxTransform::translate);
					MDataHandle currTranslateHandle = currDataBlock.outputValue(tPlug);
					currTranslateHandle.set3Double(translate.x, translate.y, translate.z);
					currTranslateHandle.setClean();

					MPlug rPlug(currObject, MPxTransform::rotate);
					MDataHandle currRotateHandle = currDataBlock.outputValue(rPlug);
					currRotateHandle.set3Double(rotateVals[0], rotateVals[1], rotateVals[2]);
					currRotateHandle.setClean();
                   
                    // We can't set the world matrix directly, so put
                    // the computed data back onto the data block by
                    // manually setting the translate and rotate plugs.
                    // We do this to make sure the transform knows the values
                    // have been computed and updated properly, and so won't
                    // have to trigger an evaluation with the constraints
                    tPlug.setValue(currTranslateHandle);
                    rPlug.setValue(currRotateHandle);
				}

				{
					// Call evaluation on the current node to compute what remains to be 
                    // computed and notify renderer. In this particular case we have written 
                    // TR values to transform and will let default code recompute and cache 
                    // world matrix. Many non-drawable nodes will not require this call, 
                    // e.g. the two constraints that we completely ignored.
					MProfilingScope profilingScopeUpdate(_profilerCategory, MProfiler::kColorD_L1, "Finalize evaluation and render update");
					cluster->evaluateNode(currEvalNode, &stat);
				}
			}
        }
    }

	// We failed to evaluate, use native compute
	if(stat != MS::kSuccess)
	{
		MProfilingScope profilingScopeFallback(_profilerCategory, MProfiler::kColorD_L2, "Fall back to native");
		cluster->evaluate(&stat);
	}
}

void constraintEvaluator::clusterTerminate(const MCustomEvaluatorClusterNode* cluster)
{
	// Clear this map when topology gets invalidated
	reset();
}

MPxCustomEvaluator* constraintEvaluator::creator()
{
    constraintEvaluator* newEval = new constraintEvaluator();
    newEval->reset();
    return newEval;
}

constraintEvaluator::~constraintEvaluator()
{
}

/////////////////////////////////////////////////////////////////
//
// The following routines are used to register/unregister
// the evaluator we are creating within Maya
//
/////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerEvaluator("ConstraintEvaluator", 42, constraintEvaluator::creator);

	if (!status) 
    {
		status.perror("registerEvaluator");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status =  plugin.deregisterEvaluator( "ConstraintEvaluator" );
	if (!status)
    {
		status.perror("deRegisterEvaluator");
		return status;
	}

	return status;
}
