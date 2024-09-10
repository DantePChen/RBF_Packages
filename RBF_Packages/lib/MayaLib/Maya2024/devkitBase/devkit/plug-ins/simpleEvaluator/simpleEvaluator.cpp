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

#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>
#include <maya/MString.h>

#include <maya/MPxCustomEvaluator.h>
#include <maya/MEulerRotation.h>
#include <maya/MCustomEvaluatorClusterNode.h>

#include <maya/MGlobal.h>
#include <maya/MTime.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MPlugArray.h>
#include <maya/MPlug.h>
#include <maya/MAnimControl.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MProfiler.h>
#include <maya/MItDependencyNodes.h>
#include <vector>
#include <string>

// to avoid pulling in boost for just hash_combine, lets have our own
template <class T>
inline void hash_combine(std::size_t & seed, const T & v)
{
		  std::hash<T> hasher;
		    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

int _profilerCategory = MProfiler::addCategory("Simple Evaluator", "Events from the EM simple evaluator");

///////////////////////////////////////////////////
//
// Evaluator class declaration
//
///////////////////////////////////////////////////
class simpleEvaluator : public MPxCustomEvaluator
{
public:
				~simpleEvaluator() override;

    bool	markIfSupported		(const MEvaluationNode* node) override;
    
    MCustomEvaluatorClusterNode::SchedulingType schedulingType	(const MCustomEvaluatorClusterNode* cluster) override;
    
    void	preEvaluate			(const MEvaluationGraph* graph) override;
    void	postEvaluate		(const MEvaluationGraph* graph) override;

    bool	clusterInitialize	(const MCustomEvaluatorClusterNode* cluster) override;
    void	clusterEvaluate		(const MCustomEvaluatorClusterNode* cluster) override;
    void	clusterTerminate	(const MCustomEvaluatorClusterNode* cluster) override;

	static MPxCustomEvaluator*	creator();

protected:
    void    buildPlugListWithControllerTag();
    void    buildHashValue();

private:
    std::size_t fOldHashValue;
    std::size_t fCurrentHashValue;

    MPlugArray  fControllerPlugs;
};

///////////////////////////////////////////////////
//
// Evaluator class implementation
//
// This evaluator assumes the existence of controller 
// nodes that control the animation for a scene. It is 
// assumed that the nodes are tagged as controllers (in
// the "Rigging" menu set, Control -> Tag as Controller).
// During the scheduling phase (in clusterInitialize(...), 
// we call buildPlugListWithControllerTag to build a 
// list of plugs for translation, and rotation.
// 
// During preEvaluate(...) which is called per frame
// we calculate a hash value based on the plug values of
// the current frame. This value is compared with the 
// previous frame's hash in clusterEvaluate(...) and
// if the hash is different, we proceed with evaluation
// otherwise we do nothing.
//
// The setup is cleaned up in clusterTerminate(...), which
// is only called when the scene's graph topology is
// invalidated.
///////////////////////////////////////////////////

bool simpleEvaluator::markIfSupported(const MEvaluationNode* node)
{
    return true;
}

void simpleEvaluator::buildPlugListWithControllerTag()
{
    MStatus stat;
    MItDependencyNodes dgIter(MFn::kControllerTag, &stat);

    if (stat == MS::kSuccess)
    {
        const char* values[] = {"translateX", "translateY", "translateZ", "rotateX", "rotateY", "rotateZ"};

        for (; !dgIter.isDone(); dgIter.next()) 
        {
            MFnDependencyNode controllerTagNode(dgIter.thisNode(), &stat);

            if (stat != MS::kSuccess) continue;
            
            MPlug currControllerTagPlug = controllerTagNode.findPlug("controllerObject",  true,  &stat);

            if (stat != MS::kSuccess) continue;

            // found controller tag node, now get its source controller
            MPlugArray source;
            if (currControllerTagPlug.connectedTo(source, true /* asDst */, false /* asSrc */, &stat))
            {
                if (stat != MS::kSuccess) continue;

                // there should only be one source with the controller tag node as destination
                MObject controllerNode = source[0].node(&stat);

                if (stat != MS::kSuccess) continue;

                MFnDependencyNode currControllerNode(controllerNode, &stat);

                for (unsigned int j = 0; j < 6; j++)
                {
                    MPlug currPlug = currControllerNode.findPlug(values[j],  true,  &stat);

                    if (stat == MS::kSuccess)
                    {
                        fControllerPlugs.append(currPlug);
                    }
                    else
                    {
                        std::cerr << "NO PLUG: " << currControllerNode.name().asChar() << "." << values[j] << std::endl;
                    }
                }
            }
        }
    }
}

void simpleEvaluator::buildHashValue()
{
    unsigned int length = fControllerPlugs.length();
    MStatus stat = MS::kSuccess;

    for (unsigned int i = 0; i < length; i++)
    {
        float value = 0;
        stat = fControllerPlugs[i].getValue(value);

        if (stat == MS::kSuccess)
        {
            hash_combine(fCurrentHashValue, value);
        }
        else
        {
            std::cerr << "NO VALUE: " << fControllerPlugs[i].name().asChar() << std::endl;
        }
    }
}

// Is this evaluator capable of evaluating clusters in parallel?
MCustomEvaluatorClusterNode::SchedulingType simpleEvaluator::schedulingType	(const MCustomEvaluatorClusterNode* cluster)
{
    return cluster->schedulingType();
}

void simpleEvaluator::preEvaluate(const MEvaluationGraph* graph)
{
    MProfilingScope profilingScopeForEval(_profilerCategory, MProfiler::kColorD_L1, "buildPoseHash", "building pose hash");
    buildHashValue();
}

void simpleEvaluator::postEvaluate(const MEvaluationGraph* graph)
{
    fOldHashValue = fCurrentHashValue;
    fCurrentHashValue = 0;
}

// called during scheduling
bool simpleEvaluator::clusterInitialize	(const MCustomEvaluatorClusterNode* cluster)
{
    if (fControllerPlugs.length() == 0)
    {
        buildPlugListWithControllerTag();
    }

    return true;
}

void simpleEvaluator::clusterEvaluate(const MCustomEvaluatorClusterNode* cluster)
{
    if (fOldHashValue != fCurrentHashValue)
    {
        // if the poses are different, call the cluster's evaluation
        cluster->evaluate();
    }
}

void simpleEvaluator::clusterTerminate(const MCustomEvaluatorClusterNode* cluster)
{
    if (fControllerPlugs.length() > 0)
    {
        fControllerPlugs.clear();
    }
}

MPxCustomEvaluator* simpleEvaluator::creator()
{
	return new simpleEvaluator();
}

simpleEvaluator::~simpleEvaluator()
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

    status = plugin.registerEvaluator("SimpleEvaluator", 50 /* unused */, simpleEvaluator::creator);

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

	status =  plugin.deregisterEvaluator( "SimpleEvaluator" );
	if (!status)
    {
		status.perror("deRegisterEvaluator");
		return status;
	}

	return status;
}
