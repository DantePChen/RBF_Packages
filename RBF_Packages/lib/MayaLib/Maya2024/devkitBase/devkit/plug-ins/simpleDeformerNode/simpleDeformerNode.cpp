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
// Produces the dependency graph node "simpleDeformerNode".
//
// This plug-in demonstrates how to create a user-defined weighted deformer
// with an associated shape. A deformer is a node which takes any number of
// input geometries, deforms them, and places the output into the output
// geometry attribute. This example plug-in defines a new deformer node
// that moves vertices according to their CV's weights.
//
// The simpleDeformer node example below is not meant to be a practical deformer to
// be used: it's only function is to explain certain concepts and give examples
// on how to use the API.
//
// The node moves all verts of the deforming geometry up based on the coordinate
// system of an external transform.

//
// To use this node:
//	- create a plane or some other object
//	- type: "deformer -type simpleDeformer"
//	- a locator is created by the command, and you can use this locator
//	  to control the direction of the movement. The object's CV's will be moved
//	  by the value of the weights of the CV's
//	- you can edit the weights using either falloffs or the component editor or
//    by using the percent command (eg. percent -v .5 simpleDeformer1;)
//
// Use this script to create a simple example with the simpleDeformer node:
//
//	loadPlugin simpleDeformerNode;
//	polyTorus -r 1 -sr 0.5 -tw 0 -sx 50 -sy 50 -ax 0 1 0 -cuv 1 -ch 1;
//	deformer -type "simpleDeformer";
//	setKeyframe -v 0 -at rotateZ -t 1 transform1;
//	setKeyframe -v 180 -at rotateZ -t 60 transform1;
//	select -cl;
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxDeformerNode.h>
#include <maya/MItGeometry.h>
#include <maya/MPxLocatorNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MTypeId.h>
#include <maya/MPlug.h>

#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>

#include <maya/MDagModifier.h>

#include <maya/MPxGPUStandardDeformer.h>
#include <maya/MGPUDeformerRegistry.h>
#include <maya/MOpenCLBuffer.h>
#include <maya/MOpenCLInfo.h>
#include <maya/MOpenCLUtils.h>
#include <cassert>

class simpleDeformer : public MPxDeformerNode
{
public:
    simpleDeformer();
    ~simpleDeformer() override;

    static  void*		creator();
    static  MStatus		initialize();

    // deformation function
    //
    MStatus	deform(MDataBlock& block,
                    MItGeometry& iter,
                    const MMatrix& mat,
                    unsigned int	multiIndex) override;

    // when the accessory is deleted, this node will clean itself up
    //
    MObject& accessoryAttribute() const override;

    // create accessory nodes when the node is created
    //
    MStatus accessoryNodeSetup(MDagModifier& cmd) override;

public:

    // local node attributes
    static MObject locatorMatrix;
    static MTypeId id;

    // path from where the plugin was loaded
    static MString pluginPath;

private:
};

// local attributes
//
MTypeId simpleDeformer::id( 0x8000f );
MObject simpleDeformer::locatorMatrix;
MString simpleDeformer::pluginPath;

simpleDeformer::simpleDeformer() {}
simpleDeformer::~simpleDeformer() {}

void* simpleDeformer::creator()
{
    return new simpleDeformer();
}

MStatus simpleDeformer::initialize()
{
    // local attribute initialization

    MFnMatrixAttribute  mAttr;
    locatorMatrix=mAttr.create( "locatorMatrix", "lm");
    mAttr.setStorable(false);
    mAttr.setConnectable(true);

    //  deformation attributes
    addAttribute( locatorMatrix);

    attributeAffects( simpleDeformer::locatorMatrix, simpleDeformer::outputGeom );

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
simpleDeformer::deform( MDataBlock& block,
                        MItGeometry& iter,
                        const MMatrix& /*m*/,
                        unsigned int multiIndex)
{
    MStatus returnStatus;
    // Envelope data from the base class.
    MDataHandle envData = block.inputValue(envelope, &returnStatus);
    if (MS::kSuccess != returnStatus)
        return returnStatus;
    float deformerEnvelope = envData.asFloat();

    // Get the matrix which is used to define the direction and scale
    // of the deformation.
    //
    MDataHandle matData = block.inputValue(locatorMatrix, &returnStatus );
    if (MS::kSuccess != returnStatus) return returnStatus;
    MMatrix omat = matData.asMatrix();

    // Get the vert weights, either painted, falloffs or a combination thereof...
    unsigned int numWeights;
    const MIndexMapper& mapper = indexMapper(multiIndex);
    const float* vertWeights = envelopeWeights(block, multiIndex, &numWeights);

    // Get all the point that are being deformed.
    MPointArray points;
    iter.allPositions(points);

    // Notes:
    //      When there are vertWeights (nullptr != vertWeights) then numWeights will be
    //      the same as the mapper.affectCount().
    //      When there are no vertWeights (nullptr == vertWeights) then it is assumed
    //      that the per vert weights are 1.0f for all of them.
    //
    //      We are deforming mapper.affectCount() verts of a total of mapper.fullCount() verts.
    //      When mapper.isIdentityMap() == true then we are deforming all the verts in order.
    //
    //      The number of points we retrieved with the iterator will be the same
    //      as the mapper.affectCount()

    assert(nullptr == vertWeights || numWeights == mapper.affectCount());
    assert(points.length() == mapper.affectCount());

    unsigned int affectCount = mapper.affectCount();
    unsigned int aid = 0;
    // Note: the following section could be parallelized
    for (aid = 0; aid < affectCount; ++aid) {
        MPoint& pt = points[aid];
        MPoint newPt = pt * omat;
        float weight = (vertWeights ? vertWeights[aid]*deformerEnvelope : deformerEnvelope);
        pt += weight * (newPt - pt);
    }

    iter.setAllPositions(points);

    return returnStatus;
}


/* override */
MObject&
simpleDeformer::accessoryAttribute() const
//
//	Description:
//	  This method returns a the attribute to which an accessory
//	  shape is connected. If the accessory shape is deleted, the deformer
//	  node will automatically be deleted.
//
//	This method is optional.
//
{
	return simpleDeformer::locatorMatrix;
}

/* override */
MStatus
simpleDeformer::accessoryNodeSetup(MDagModifier& cmd)
//
//	Description:
//		This method is called when the deformer is created by the
//		"deformer" command. You can add to the cmds in the MDagModifier
//		cmd in order to hook up any additional nodes that your node needs
//		to operate.
//
//		In this example, we create a locator and attach its matrix attribute
//		to the matrix input on the simpleDeformer node. The locator is used to
//		set the direction and scale of the random field.
//
//	Description:
//		This method is optional.
//
{
    MStatus result;

    // hook up the accessory node
    //
    MObject objLoc = cmd.createNode(MString("locator"),
                                    MObject::kNullObj,
                                    &result);

    if (MS::kSuccess == result) {
        MFnDependencyNode fnLoc(objLoc);
        MString attrName;
        attrName.set("matrix");
        MObject attrMat = fnLoc.attribute(attrName);

        result = cmd.connect(objLoc,attrMat,this->thisMObject(),simpleDeformer::locatorMatrix);
    }
    return result;
}

// -----------------------------------------------------------------------------
//
// the GPU override implementation of the simpleDeformerNode
//
// -----------------------------------------------------------------------------
class simpleGPUDeformer : public MPxGPUStandardDeformer
{
public:
    // Virtual methods from MPxGPUDeformer
    simpleGPUDeformer();
    ~simpleGPUDeformer() override;

    MPxGPUDeformer::DeformerStatus evaluate(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& outputPlug, const MPlugArray& inputPlugs, const MGPUDeformerData& inputData, MGPUDeformerData& outputData) override;
    void terminate() override;

    static MGPUDeformerRegistrationInfo* getGPUDeformerInfo();
    static bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode&, const MPlug& plug, MStringArray* messages);
    static bool validateNodeValues(MDataBlock& block, const MEvaluationNode&, const MPlug& plug, MStringArray* messages);

private:
    // helper methods
    bool extractLocatorMatrix(MDataBlock& block);

    cl_int enqueueDeformation(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions);

    // Storage for data on the GPU
    cl_float16 fLocatorMatrix;

    // Kernel
    MOpenCLKernelInfo fKernelInfo;
};

class simpleDeformerNodeGPUDeformerInfo : public MGPUDeformerRegistrationInfo
{
public:
    simpleDeformerNodeGPUDeformerInfo() {}
    ~simpleDeformerNodeGPUDeformerInfo() override{}

    MPxGPUDeformer* createGPUDeformer() override
    {
        return new simpleGPUDeformer();
    }

    bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
    {
        return simpleGPUDeformer::validateNodeInGraph(block, evaluationNode, plug, messages);
    }

    bool validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
    {
        return simpleGPUDeformer::validateNodeValues(block, evaluationNode, plug, messages);
    }
};

MGPUDeformerRegistrationInfo* simpleGPUDeformer::getGPUDeformerInfo()
{
    static simpleDeformerNodeGPUDeformerInfo theOne;
    return &theOne;
}

simpleGPUDeformer::simpleGPUDeformer()
	: MPxGPUStandardDeformer()
{
    // Remember the ctor must be fast.  No heavy work should be done here.
    // Maya may allocate one of these and then never use it.
}

simpleGPUDeformer::~simpleGPUDeformer()
{
	terminate();
}

/* static */
bool simpleGPUDeformer::validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
    // simpleGPUDeformer supports everything on the simpleDeformer node
    return true;
}

/* static */
bool simpleGPUDeformer::validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
    // simpleGPUDeformer supports everything on the simpleDeformer node
    return true;
}

bool simpleGPUDeformer::extractLocatorMatrix(MDataBlock& block)
{
    MStatus status;
    MDataHandle matData = block.inputValue(simpleDeformer::locatorMatrix, &status );
    if (MS::kSuccess != status)
        return false;

    MMatrix omat = matData.asMatrix();
    float matrixData[4][4];
    if( !omat.get( matrixData ) )
        return false;
    memcpy( fLocatorMatrix.s, matrixData, sizeof(matrixData));
    return true;
}

cl_int simpleGPUDeformer::enqueueDeformation(
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

    // Set all of our kernel parameters.  Input buffer and output buffer may be changing every frame so always set them.
    unsigned int parameterId = 0;
    err = fKernelInfo.setKernelArgBuffer(parameterId++, outputPositions.buffer(), err);
    err = fKernelInfo.setKernelArgBuffer(parameterId++, inputPositions.buffer(), err);
    err = fKernelInfo.setKernelArgBuffer(parameterId++, weightsBuffer(), err, hasEnvelopeWeights());
    err = fKernelInfo.setKernelArgBuffer(parameterId++, affectMapBuffer(), err, !isIdentityMap());
    err = fKernelInfo.setKernelArg<cl_float16>(parameterId++, fLocatorMatrix, err);
    err = fKernelInfo.setKernelArg<cl_float>(parameterId++, envelope(), err);
    err = fKernelInfo.setKernelArg<cl_uint>(parameterId++, count, err);

    // run the kernel
    return CL_SUCCESS == err ? fKernelInfo.enqueueKernel(eventList, syncEvent) : err;
}

MPxGPUDeformer::DeformerStatus simpleGPUDeformer::evaluate(
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

    /*bool subsetChanged =*/ prepareAffectMapBuffer();
    /*bool weightsChanged =*/ prepareWeightsBuffer(evaluationNode);

    // In case the implementation needs to respond to changes in the subset or
    // topology the return value of prepareAffectMapBuffer can be used to
    // trigger the appropriate actions.

    // In case the implementation needs to respond to changes in envelope weights
    // the return value of prepareWeightsBuffer can be used to trigger the
    // appropriate actions.

    // This node is deforming affectCount() of fullCount() verts.
    // isIdentityMap() tells us whether we are fully deforming the geometry or not.

    // The envelope for the deformer is envelope() and hasEnvelopeWeights() tells
    // us whether we have per vertex weight.

    if (!extractLocatorMatrix(block))
        return MPxGPUDeformer::kDeformerFailure;

    // Now that all the data we care about is on the GPU, setup and run the OpenCL Kernel
    MString openCLKernelFile = simpleDeformer::pluginPath + "/simpleDeformer.cl";
    MString openCLKernelName("simpleDeformer");
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

void simpleGPUDeformer::terminate()
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
    result = plugin.registerNode( "simpleDeformer", simpleDeformer::id, simpleDeformer::creator,
                                    simpleDeformer::initialize, MPxNode::kDeformerNode );

    MString nodeClassName("simpleDeformer");
    MString registrantId("mayaPluginExample");
    MGPUDeformerRegistry::registerGPUDeformerCreator(
        nodeClassName,
        registrantId,
        simpleGPUDeformer::getGPUDeformerInfo());

    simpleDeformer::pluginPath = plugin.loadPath();
    return result;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus result;

    MString nodeClassName("simpleDeformer");
    MString registrantId("mayaPluginExample");
    MGPUDeformerRegistry::deregisterGPUDeformerCreator(
        nodeClassName,
        registrantId);

    MFnPlugin plugin(obj);
    result = plugin.deregisterNode(simpleDeformer::id);

    return result;
}
