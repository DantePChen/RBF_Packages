//-
// ==========================================================================
// Copyright 2022 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//
// Produces the dependency graph node "testFailure".
//
//
// To use this node:
//	- create a plane or some other object
//	- type: "deformer -type testFailure"

// Use this script to create a simple example with the testFailure node:
//
//	loadPlugin testFailureNode;
//	polyTorus -r 1 -sr 0.5 -tw 0 -sx 50 -sy 50 -ax 0 1 0 -cuv 1 -ch 1;
//	deformer -type "testFailure";
//	select -cl;
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxDeformerNode.h>
#include <maya/MItGeometry.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MTypeId.h>
#include <maya/MPlug.h>

#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MPoint.h>

#include <maya/MPxGPUStandardDeformer.h>
#include <maya/MGPUDeformerRegistry.h>
#include <maya/MOpenCLBuffer.h>
#include <maya/MOpenCLInfo.h>
#include <maya/MOpenCLUtils.h>
#include <clew/clew.h>

class testFailure : public MPxDeformerNode
{
public:
        testFailure();
        ~testFailure() override;

    static  void*		creator();
    static  MStatus		initialize();

    // deformation function
    //
    MStatus	deform(MDataBlock& block,
                   MItGeometry& iter,
                   const MMatrix& mat,
                   unsigned int	multiIndex) override;

public:

    // local node attributes
    static MObject aOffset;
    static MObject aFailureTrigger;
    static MObject aForceDifference;
    static MTypeId id;

    // path from where the plugin was loaded
    static MString pluginPath;

private:
};

// local attributes
//
MTypeId testFailure::id( 0x00000177 );
MObject testFailure::aOffset;
MObject testFailure::aFailureTrigger; // depending on value causes a failure to be generated from the GPU deformer
MObject testFailure::aForceDifference; // causes a difference in the cpu and gpu behavior
MString testFailure::pluginPath;

testFailure::testFailure() {}
testFailure::~testFailure() {}

void* testFailure::creator()
{
    return new testFailure();
}

MStatus testFailure::initialize()
{
    // local attribute initialization

    MFnNumericAttribute nAttr;
    aOffset = nAttr.create( "offset", "of", MFnNumericData::k3Float);
    nAttr.setDefault(0.0f, 0.0f, 0.0f);
    nAttr.setStorable(true);

    // When this flag is on the results for the CPU and GPU evaluation will be different
    aForceDifference = nAttr.create( "forceDifference", "fd", MFnNumericData::kBoolean, false);
    nAttr.setStorable(true);

    // A value equal or less than -1.0 will cause a kDeformerRetryMainThread failure on the GPU
    // A value equal or more than +1.0 will cause a kDeformerFailure failure on the GPU
    aFailureTrigger = nAttr.create( "failureTrigger", "ftr", MFnNumericData::kFloat, 0.0 );
    nAttr.setStorable(true);

    //  deformation attributes
    addAttribute( aOffset );
    addAttribute( aFailureTrigger );
    addAttribute( aForceDifference );

    attributeAffects( testFailure::aOffset, testFailure::outputGeom );
    attributeAffects( testFailure::aFailureTrigger, testFailure::outputGeom );
    attributeAffects( testFailure::aForceDifference, testFailure::outputGeom );

    return MStatus::kSuccess;
}

/*!
	Deform the point with a squash algorithm
	\param[in] block the datablock of the node
	\param[in] iter an iterator for the geometry to be deformed
	\param[in] m matrix to transform the point into world space
	\param[in] multiIndex the index of the geometry that we are deforming
	\return the deformation status
*/
MStatus
testFailure::deform( MDataBlock& block,
				MItGeometry& iter,
				const MMatrix& /*m*/,
				unsigned int multiIndex)
{
    MStatus returnStatus;
    // Envelope data from the base class.
    // The envelope is simply a scale factor.
    //
    MDataHandle envData = block.inputValue(envelope, &returnStatus);
    if (MS::kSuccess != returnStatus)
        return returnStatus;
    float env = envData.asFloat();

    MDataHandle offsetData = block.inputValue( aOffset, &returnStatus );
    float3& offsetVal = offsetData.asFloat3();

    for ( ; !iter.isDone(); iter.next()) {
        MPoint pt = iter.position();
        float e = env*weightValue(block,multiIndex,iter.index());

        pt.x += offsetVal[0]*e;
        pt.y += offsetVal[1]*e;
        pt.z += offsetVal[2]*e;

        iter.setPosition(pt);
    }
    return returnStatus;
}

// -----------------------------------------------------------------------------
//
// the GPU override implementation of the offsetNode
//
// -----------------------------------------------------------------------------
class testFailureGPUDeformer : public MPxGPUStandardDeformer
{
public:
    // Virtual methods from MPxGPUDeformer
    testFailureGPUDeformer();
    ~testFailureGPUDeformer() override;

    MPxGPUDeformer::DeformerStatus evaluate(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& outputPlug, const MPlugArray& inputPlugs, const MGPUDeformerData& inputData, MGPUDeformerData& outputData) override;
    void terminate() override;

    static MGPUDeformerRegistrationInfo* getGPUDeformerInfo();

private:
    cl_int enqueueDeformation(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions);

    float3 fOffset;
    bool fForceDifference{false};

    // Kernel
    MOpenCLKernelInfo fKernelInfo;
};

class testFailureNodeGPUDeformerInfo : public MGPUDeformerRegistrationInfo
{
public:
    testFailureNodeGPUDeformerInfo() {}
    ~testFailureNodeGPUDeformerInfo() override{}

    MPxGPUDeformer* createGPUDeformer() override
    {
        return new testFailureGPUDeformer();
    }

    bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
    {
        return true;
    }

    bool validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
    {
        return true;
    }
};

MGPUDeformerRegistrationInfo* testFailureGPUDeformer::getGPUDeformerInfo()
{
    static testFailureNodeGPUDeformerInfo theOne;
    return &theOne;
}

testFailureGPUDeformer::testFailureGPUDeformer()
    : MPxGPUStandardDeformer()
{
    // Remember the ctor must be fast.  No heavy work should be done here.
    // Maya may allocate one of these and then never use it.
}

testFailureGPUDeformer::~testFailureGPUDeformer()
{
    terminate();
}


cl_int testFailureGPUDeformer::enqueueDeformation(
        MAutoCLEvent& syncEvent,
        const MGPUDeformerBuffer& inputPositions,
        MGPUDeformerBuffer& outputPositions)
{
    cl_int err = CL_SUCCESS;

    unsigned int count = affectCount();

    MAutoCLEvent syncInputEvent = syncEvent;
    syncEvent = MAutoCLEvent();

    MGPUEventList eventList;
    eventList.add(syncInputEvent);

    cl_float3 offset;
    offset.s[0] = fOffset[0] + (fForceDifference ? 1.0 : 0.0);
    offset.s[1] = fOffset[1];
    offset.s[2] = fOffset[2];

    // Set all of our kernel parameters.  Input buffer and output buffer may be changing every frame so always set them.
    unsigned int parameterId = 0;
    err = fKernelInfo.setKernelArgBuffer(parameterId++, outputPositions.buffer(), err);
    err = fKernelInfo.setKernelArgBuffer(parameterId++, inputPositions.buffer(), err);
    err = fKernelInfo.setKernelArgBuffer(parameterId++, weightsBuffer(), err, hasEnvelopeWeights());
    err = fKernelInfo.setKernelArgBuffer(parameterId++, affectMapBuffer(), err, !isIdentityMap());
    err = fKernelInfo.setKernelArg<cl_float>(parameterId++, envelope(), err);
    err = fKernelInfo.setKernelArg<cl_float3>(parameterId++, offset, err);
    err = fKernelInfo.setKernelArg<cl_uint>(parameterId++, count, err);

    // run the kernel
    return CL_SUCCESS == err ? fKernelInfo.enqueueKernel(eventList, syncEvent) : err;
}

MPxGPUDeformer::DeformerStatus testFailureGPUDeformer::evaluate(
    MDataBlock& block,							// data block for "this" node
    const MEvaluationNode& evaluationNode,		// evaluation node representing "this" node
    const MPlug& plug,							// the multi index we're working on.  There will be a separate instance created per multi index
    const MPlugArray & inputPlugs,              // the input plugs which have GPU data in inputData
    const MGPUDeformerData& inputData,			// the input data provided by Maya or other upstream GPU Deformers
    MGPUDeformerData& outputData				// the output data to be passed to the rendering system or other downstream GPU Deformers
)
{
    MAutoCLEvent syncEvent;
    MPxGPUDeformer::DeformerStatus ds = prepareEvaluation(block, evaluationNode, plug, inputPlugs, inputData, outputData, syncEvent);
    if (ds != MPxGPUDeformer::kDeformerSuccess)
        return ds;

    prepareAffectMapBuffer();
    prepareWeightsBuffer(evaluationNode);

    MStatus status;
    MDataHandle offsetData = block.inputValue(testFailure::aOffset, &status );
    const float3& val = offsetData.asFloat3();
    fOffset[0] = val[0];
    fOffset[1] = val[1];
    fOffset[2] = val[2];

    MDataHandle failureTriggerData = block.inputValue(testFailure::aFailureTrigger, &status );
    float failureTrigger = (MS::kSuccess != status) ? 0.0 : failureTriggerData.asFloat();

    fForceDifference = block.inputValue(testFailure::aForceDifference, &status ).asBool();

    if (failureTrigger <= -1.0) {
        return MPxGPUDeformer::kDeformerRetryMainThread;
    } else if (failureTrigger >= 1.0) {
        return MPxGPUDeformer::kDeformerFailure;
    }

    // Now that all the data we care about is on the GPU, setup and run the OpenCL Kernel
    MString openCLKernelFile = testFailure::pluginPath + "/testFailure.cl";
    MString openCLKernelName("testFailure");
    if (!fKernelInfo.loadOrUpdate(openCLKernelFile, openCLKernelName, affectCount()))
        return MPxGPUDeformer::kDeformerFailure;

    cl_int err = CL_SUCCESS;
    err = initializeOutputPositions(syncEvent);
    if ( err != CL_SUCCESS )
        return MPxGPUDeformer::kDeformerFailure;

    err = enqueueDeformation(syncEvent, inputPositions(), outputPositions());
    if ( err != CL_SUCCESS )
        return MPxGPUDeformer::kDeformerFailure;

    return finishEvaluation(syncEvent, outputData);
}

void testFailureGPUDeformer::terminate()
{
    fKernelInfo.reset();
    MPxGPUStandardDeformer::terminate();
}

// standard initialization procedures
//

MStatus initializePlugin( MObject obj )
{
    MStatus result;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");
    result = plugin.registerNode( "testFailure", testFailure::id, testFailure::creator,
                                    testFailure::initialize, MPxNode::kDeformerNode );

    MString nodeClassName("testFailure");
    MString registrantId("mayaPluginExample");
    MGPUDeformerRegistry::registerGPUDeformerCreator(
        nodeClassName,
        registrantId,
        testFailureGPUDeformer::getGPUDeformerInfo());

    testFailure::pluginPath = plugin.loadPath();
    return result;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus result;

    MString nodeClassName("testFailure");
    MString registrantId("mayaPluginExample");
    MGPUDeformerRegistry::deregisterGPUDeformerCreator(
        nodeClassName,
        registrantId);

    MFnPlugin plugin(obj);
    result = plugin.deregisterNode(testFailure::id);

    return result;
}
