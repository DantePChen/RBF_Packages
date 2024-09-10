//-
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
//
// ===========================================================================
//+

#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>

#include <maya/MPxCustomEvaluator.h>
#include <maya/MCustomEvaluatorClusterNode.h>

/*
# The customer evaluator is used to skip the EM evaluation of the nodes 
# inside the set named "PruneSet". The nodes depended on the skipped 
# node will be skipped automatically.

# Animate a sphere along x axis
import maya.cmds as cmds
nodes = cmds.polySphere()
cmds.setKeyframe(nodes[0] + ".tx")
cmds.currentTime(120)
cmds.move(120, 0, 0, relative = True)
cmds.setKeyframe(nodes[0] + ".tx")
cmds.select("pSphere1_translateX")
cmds.sets(name = "PruneSet")

# Playback to confirm the animation
cmds.currentTime(0)
cmds.playbackOptions(minTime = 0.00, maxTime = 60.0)
cmds.play(forward = True)

# Load and enable evaluationPrunningEvaluator
cmds.loadPlugin("evaluationPruningEvaluator")
cmds.evaluator(enable=True, name="evaluationPruningEvaluator")

# Playback to see the stationary sphere because
# the evaluation for the nodes in "PruneSet" is pruned by
# the custom evaluator. EM skips downstream nodes if the 
# upstream node is not evaluated and that's why skipping the
# anim curve node alone is sufficient to stop the animation
cmds.currentTime(0)
cmds.play(forward = True)
*/

///////////////////////////////////////////////////
//
// Evaluator class declaration
//
///////////////////////////////////////////////////
class evaluationPruningEvaluator : public MPxCustomEvaluator
{
public:
	virtual	~evaluationPruningEvaluator();

    bool	markIfSupported		(const MEvaluationNode* node) override;
    
	bool    wantPruneExecution() const override;
	bool    pruneExecution(const MCustomEvaluatorClusterNode* cluster) override;

	void	clusterEvaluate(const MCustomEvaluatorClusterNode* cluster) override;

	static MPxCustomEvaluator*	creator();

};

bool evaluationPruningEvaluator::markIfSupported(const MEvaluationNode* node)
{
	MStatus stat;
	MSelectionList slist;
	stat = slist.add("PruneSet");
	if (stat != MS::kSuccess)
	{
		// Nothing to prune if PruneSet doesn't exist
		return false;
	}

	// Support only the nodes in the set "PruneSet"
	MObject object;
	stat = slist.getDependNode(0, object);
	if (stat == MS::kSuccess && object.hasFn(MFn::kSet))
	{
		MFnSet setFn(object);
		if (setFn.isMember(node->dependencyNode()))
		{
			MFnDependencyNode nodeFn(node->dependencyNode());
			MString msg = "markIfSupported " + nodeFn.name();
			MGlobal::displayInfo(msg);
			return true;
		}
	}

	return false;
}

// Should MPxCustomPruningEvaluator::pruneExecution be called on the supported nodes?
bool evaluationPruningEvaluator::wantPruneExecution() const
{
	return true;
}

// Should the evaluation of the input cluster node be pruned?
bool evaluationPruningEvaluator::pruneExecution(const MCustomEvaluatorClusterNode* cluster)
{
	return true; 
}

// To evlauate the cluster node. The method shouldn't be called if the evaluation is pruned.
void evaluationPruningEvaluator::clusterEvaluate(const MCustomEvaluatorClusterNode* cluster)
{
	MGlobal::displayInfo("evaluationPruningEvaluator::clusterEvaluate()");
	cluster->evaluate();
}

MPxCustomEvaluator* evaluationPruningEvaluator::creator()
{
	return new evaluationPruningEvaluator();
}

evaluationPruningEvaluator::~evaluationPruningEvaluator()
{
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	// For evaluationPruningEvaluator to claim a node, it must have a higher priority than the competing evlauator(s).
	// Use 12000 to make sure it takes precedence over all other evaluators.
    status = plugin.registerEvaluator("evaluationPruningEvaluator", 12000 , evaluationPruningEvaluator::creator);

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

	status =  plugin.deregisterEvaluator( "evaluationPruningEvaluator" );
	if (!status)
    {
		status.perror("deRegisterEvaluator");
		return status;
	}

	return status;
}
