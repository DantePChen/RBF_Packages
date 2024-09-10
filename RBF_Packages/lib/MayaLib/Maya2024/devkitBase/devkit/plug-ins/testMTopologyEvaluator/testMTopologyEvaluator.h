#ifndef __testMTopologyEvaluator_H__
#define __testMTopologyEvaluator_H__

#include <maya/MPxNode.h>
#include <maya/MStatus.h>
#include <maya/MTypeId.h> 
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MPxTopologyEvaluator.h>

#include <vector>
#include <atomic>

//--------------------------------------------------------------------------------------------------------
class TestMTopologyEvaluator_Node : public MPxNode
{
    typedef MPxNode ParentClass;

public:
    TestMTopologyEvaluator_Node() : computeCount(0), pushComputeCount(0), doPushComputeOverride(false) {};
    ~TestMTopologyEvaluator_Node() override {};
   
    MPxNode::SchedulingType schedulingType()            const	override;
    MStatus	compute(const MPlug&, MDataBlock&)		            override;

    MStatus	setDependentsDirty(const MPlug& plug, MPlugArray& plugArray)                        override;
    MStatus	preEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode)    override;

    bool    setInternalValue(const MPlug&, const MDataHandle&)	override;
    bool    getInternalValue(const MPlug&, MDataHandle&)	    override;

    void	getCacheSetup(const MEvaluationNode&, MNodeCacheDisablingInfo&, MNodeCacheSetupInfo&, MObjectArray&) const override;
    //------------------------------------------------------
    static  void*	creator		();
    static  MStatus	initialize	();
    //------------------------------------------------------
   
public:
    std::atomic<unsigned> computeCount;
    std::atomic<unsigned> pushComputeCount;
    bool                  doPushComputeOverride;

    static std::vector<MObject> attrInputs;
    static std::vector<MObject> attrOutputs;

    static  MObject	attrComputeCount;
    static  MObject	attrPushComputeCount;
    static  MObject	attrDoPushCompute;

    static  MTypeId	id;
};

//--------------------------------------------------------------------------------------------------------
class TestMTopologyEvaluator_Evaluator : public MPxTopologyEvaluator
{
public:
    			    ~TestMTopologyEvaluator_Evaluator() override;

    bool	    markIfSupported(const MEvaluationNode* node) override;
    bool	    makeTopology(MTopologyClusterNode* cluster) override;

    static MPxTopologyEvaluator*	    creator();
};


// =====================================================================
// Copyright 2019 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// =====================================================================

#endif // __testMTopologyEvaluator_H__
