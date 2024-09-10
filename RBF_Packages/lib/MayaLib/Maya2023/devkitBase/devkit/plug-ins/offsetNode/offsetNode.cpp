//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
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
// Produces the dependency graph node "offsetNode".
//
// This plug-in demonstrates how to create a user-defined weighted deformer
// with an associated shape. A deformer is a node which takes any number of
// input geometries, deforms them, and places the output into the output
// geometry attribute. This example plug-in defines a new deformer node
// that offsets vertices according to their CV's weights. The weights are set
// using the set editor or the percent command.
//
// The offset node example below is not meant to be a practical deformer to
// be used: it's only function is to explain certain concepts and give examples
// on how to use the API.
//
// The node moves all verts of the deforming geometry up along the y-axis
// in the local coordinate system of an external transform.
// Furthermore it optionally randomizes this offset slightly by looking up
// a random value in a table of user specified size based on the vertId.
// The size of the table is set by the user by defining the number of chunks.
// Is is important in this example for the GPU implementation that the table
// itself is not animatable, but the effect of it through the chunkEnvelope
// can be.
//
// To use this node:
//	- create a plane or some other object
//	- type: "deformer -type offset"
//	- a locator is created by the command, and you can use this locator
//	  to control the direction of the offset. The object's CV's will be offset
//	  by the value of the weights of the CV's (the default will be the weight * some constant)
//	  in the direction of the y-vector of the locator
//	- you can edit the weights using either the component editor or by using
//	  the percent command (eg. percent -v .5 offset1;)
//
// Use this script to create a simple example with the offset node:
//
//	loadPlugin offsetNode;
//	polyTorus -r 1 -sr 0.5 -tw 0 -sx 50 -sy 50 -ax 0 1 0 -cuv 1 -ch 1;
//	deformer -type "offset";
//	setKeyframe -v 0 -at rotateZ -t 1 transform1;
//	setKeyframe -v 180 -at rotateZ -t 60 transform1;
//	select -cl;
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <maya/MStringArray.h>
#include <math.h>

#include <maya/MPxDeformerNode.h>
#include <maya/MItGeometry.h>
#include <maya/MPxLocatorNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnFloatArrayData.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnDependencyNode.h>

#include <maya/MTypeId.h>
#include <maya/MPlug.h>

#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>

#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>

#include <maya/MDagModifier.h>

#include <maya/MPxGPUStandardDeformer.h>
#include <maya/MGPUDeformerRegistry.h>
#include <maya/MOpenCLBuffer.h>
#include <maya/MOpenCLInfo.h>
#include <maya/MOpenCLUtils.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MFnMesh.h>
#include <clew/clew.h>
#include <vector>

class offset : public MPxDeformerNode
{
public:
						offset();
					~offset() override;

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
	MObject&			accessoryAttribute() const override;

	// create accessory nodes when the node is created
	//
	MStatus				accessoryNodeSetup(MDagModifier& cmd) override;

	// Communicate fixed setup data to the GPU node
	//
	MObject getFixedSetupData(const MString& name) override;
public:

    MFloatArray randomOffsetTable;

	// local node attributes
	static MObject offsetMatrix; // offset center and axis
	static MObject chunkCount;	 // The number of chunks we want.
	static MObject chunkEnvelope; // The influence of the chunks.
	static MTypeId id;

	// path from where the plugin was loaded
	static MString pluginPath;

private:
};

// local attributes
//
MTypeId offset::id( 0x8000c );
MObject offset::offsetMatrix;
MObject offset::chunkEnvelope;
MObject offset::chunkCount;
MString offset::pluginPath;

offset::offset() {}
offset::~offset() {}

void* offset::creator()
{
	return new offset();
}

MStatus offset::initialize()
{
	// local attribute initialization

	MFnMatrixAttribute  mAttr;
	offsetMatrix=mAttr.create( "locateMatrix", "lm");
		mAttr.setStorable(false);
		mAttr.setConnectable(true);

    MFnNumericAttribute nAttr;
    chunkEnvelope = nAttr.create( "chunkEnvelope", "che", MFnNumericData::kFloat, 0.0 );
    nAttr.setStorable(true);

    chunkCount = nAttr.create( "chunkCount", "chc", MFnNumericData::kInt, 0 );
    nAttr.setMin(0);
    nAttr.setMax(20);
    nAttr.setStorable(true);
    // We do not want this keyable since it is a parameter that alters the
    // internal setup that is communicated to the GPU
    nAttr.setKeyable(false);

	//  deformation attributes
	addAttribute( offsetMatrix);
    addAttribute( chunkEnvelope );
    addAttribute( chunkCount );

	attributeAffects( offset::offsetMatrix, offset::outputGeom );
    attributeAffects( offset::chunkEnvelope, offset::outputGeom );
    attributeAffects( offset::chunkCount, offset::outputGeom );

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
offset::deform( MDataBlock& block,
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

    MDataHandle chunkEnvelopeData = block.inputValue( chunkEnvelope, &returnStatus );
    float chunkWeight = chunkEnvelopeData.asFloat();

    MDataHandle chunkCountData = block.inputValue( chunkCount, &returnStatus );
    int numChunks = chunkCountData.asInt();

    if (numChunks != randomOffsetTable.length()) {
        // We are rebuilding the randomOffsetTable. This happens rarely
        // and only because a change to the setup is made, in this case
        // by altering the non-animatable chunkCount attribute
        srand(numChunks*827);
        randomOffsetTable.setLength(numChunks);
        for (size_t i = 0; i < numChunks; ++i) {
            float r = (rand() % 1001)/500.0 - 1.0;
            randomOffsetTable.set(r, i);
        }
    }

	// Get the matrix which is used to define the direction and scale
	// of the offset.
	//
	MDataHandle matData = block.inputValue(offsetMatrix, &returnStatus );
	if (MS::kSuccess != returnStatus) return returnStatus;
	MMatrix omat = matData.asMatrix();
	MMatrix omatinv = omat.inverse();

    bool useRandomOffset = (numChunks > 0 && chunkWeight > 0.0);
	// iterate through each point in the geometry
	//
	for ( ; !iter.isDone(); iter.next()) {
		MPoint pt = iter.position();
		pt *= omatinv;
		float weight = weightValue(block,multiIndex,iter.index());

        if (useRandomOffset)
            weight *= (1.0 + chunkWeight*randomOffsetTable[iter.index()%numChunks]);

		// offset algorithm
		//
		pt.y = pt.y + env*weight;
		//
		// end of offset algorithm

		pt *= omat;
		iter.setPosition(pt);
	}
	return returnStatus;
}


/* override */
MObject&
offset::accessoryAttribute() const
//
//	Description:
//	  This method returns a the attribute to which an accessory
//	  shape is connected. If the accessory shape is deleted, the deformer
//	  node will automatically be deleted.
//
//	This method is optional.
//
{
	return offset::offsetMatrix;
}

/* override */
MStatus
offset::accessoryNodeSetup(MDagModifier& cmd)
//
//	Description:
//		This method is called when the deformer is created by the
//		"deformer" command. You can add to the cmds in the MDagModifier
//		cmd in order to hook up any additional nodes that your node needs
//		to operate.
//
//		In this example, we create a locator and attach its matrix attribute
//		to the matrix input on the offset node. The locator is used to
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

		result = cmd.connect(objLoc,attrMat,this->thisMObject(),offset::offsetMatrix);
	}
	return result;
}

/* override */
MObject
offset::getFixedSetupData(const MString& name)
//
//	Description:
//		This method is called when the GPU deformer wants certain setup data
//		identified by the string name.
//
//		In this example, the node has created an internal randomOffsetTable
//		which we want to pass to the GPU node. This allows the GPU node to remain
//		ignorant of how this table is being calculated and thereby avoids code
//		duplication that would be hard to keep in sync between the CPU and GPU.
//
//	Description:
//		This method is optional.
//
{
    if (name == "randomOffsetTable") {
        MFnFloatArrayData fn;
        return fn.create(randomOffsetTable);
    }

    return MObject::kNullObj;
}

// -----------------------------------------------------------------------------
//
// the GPU override implementation of the offsetNode
//
// -----------------------------------------------------------------------------
class offsetGPUDeformer : public MPxGPUStandardDeformer
{
public:
	// Virtual methods from MPxGPUDeformer
	offsetGPUDeformer();
	~offsetGPUDeformer() override;

	MPxGPUDeformer::DeformerStatus evaluate(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& outputPlug, const MPlugArray& inputPlugs, const MGPUDeformerData& inputData, MGPUDeformerData& outputData) override;
	void terminate() override;

	static MGPUDeformerRegistrationInfo* getGPUDeformerInfo();
	static bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode&, const MPlug& plug, MStringArray* messages);
	static bool validateNodeValues(MDataBlock& block, const MEvaluationNode&, const MPlug& plug, MStringArray* messages);

private:
	// helper methods
	bool extractOffsetMatrix(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug);
	bool extractRandomOffsetTable(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug);

	cl_int enqueueDeformation(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions);

	// Storage for data on the GPU
    MOpenCLBuffer fOffsetMatrixBuffer;
    MOpenCLBuffer fRandomOffsetTableBuffer;

	unsigned int fRandomOffsetTableSize;
	float fChunkEnvelope;

	// Kernel
    MOpenCLKernelInfo fKernelInfo;
};

class offsetNodeGPUDeformerInfo : public MGPUDeformerRegistrationInfo
{
public:
	offsetNodeGPUDeformerInfo() {}
	~offsetNodeGPUDeformerInfo() override{}

	MPxGPUDeformer* createGPUDeformer() override
	{
		return new offsetGPUDeformer();
	}

	bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
	{
		return offsetGPUDeformer::validateNodeInGraph(block, evaluationNode, plug, messages);
	}

	bool validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
	{
		return offsetGPUDeformer::validateNodeValues(block, evaluationNode, plug, messages);
	}
};

MGPUDeformerRegistrationInfo* offsetGPUDeformer::getGPUDeformerInfo()
{
	static offsetNodeGPUDeformerInfo theOne;
	return &theOne;
}

offsetGPUDeformer::offsetGPUDeformer()
	: MPxGPUStandardDeformer()
{
	// Remember the ctor must be fast.  No heavy work should be done here.
	// Maya may allocate one of these and then never use it.
}

offsetGPUDeformer::~offsetGPUDeformer()
{
	terminate();
}

/* static */
bool offsetGPUDeformer::validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
	// offsetGPUDeformer supports everything on the offset node except chunkEnvelope
	// chunkEnvelope is handled in validateNodeValues because we support some values
	// but not others.
	return true;
}

/* static */
bool offsetGPUDeformer::validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
    // As an example offsetGPUDeformer has conditional support for the chunkEnvelope
    // attribute. The offsetGPUDeformer supports the underlying depend node only if
    // chunkEnvelop is 0.0 or larger. Otherwise, the underlying node is not supported.
    // Note that in additional to testing the value of the attribute here we have also
    // registered it as a conditional attribute in offsetNodeGPUDeformerInfo.

	MObject node = plug.node();
	MFnDependencyNode fnNode(node);

    // Check the values of the conditional attributes we added to make sure they are still at
    // values that allow this node to be valid on the GPU. In this case we will check to see
    // if chunkEnvelope is smaller than 0.0 and if so we disqualify this node.
    MPlug chunkEnvelopePlug(node, offset::chunkEnvelope);
    MDataHandle chunkEnvelopeData;
    chunkEnvelopePlug.getValue(chunkEnvelopeData);
    if (chunkEnvelopeData.asFloat() < 0.0)
    {
        MOpenCLInfo::appendMessage(messages, "Offset %s not supported by deformer evaluator because chunkEnvelope is smaller than 0.0", fnNode.name().asChar());
        return false;
    }
	return true;
}

cl_int offsetGPUDeformer::enqueueDeformation(
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
	err = fKernelInfo.setKernelArgBuffer(parameterId++, fOffsetMatrixBuffer, err);
	err = fKernelInfo.setKernelArgBuffer(parameterId++, fRandomOffsetTableBuffer, err);
	err = fKernelInfo.setKernelArg<cl_uint>(parameterId++, fRandomOffsetTableSize, err);
	err = fKernelInfo.setKernelArg<cl_float>(parameterId++, envelope(), err);
	err = fKernelInfo.setKernelArg<cl_float>(parameterId++, fChunkEnvelope, err);
	err = fKernelInfo.setKernelArg<cl_uint>(parameterId++, count, err);

	// run the kernel
    return CL_SUCCESS == err ? fKernelInfo.enqueueKernel(eventList, syncEvent) : err;
}

MPxGPUDeformer::DeformerStatus offsetGPUDeformer::evaluate(
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

	extractOffsetMatrix(block, evaluationNode, plug);
	extractRandomOffsetTable(block, evaluationNode, plug);

	MStatus status;
	MDataHandle chunkEnvelopeData = block.inputValue(offset::chunkEnvelope, &status );
	fChunkEnvelope = (MS::kSuccess != status) ? 1.0 : chunkEnvelopeData.asFloat();

	// Now that all the data we care about is on the GPU, setup and run the OpenCL Kernel
    MString openCLKernelFile = offset::pluginPath + "/offset.cl";
    MString openCLKernelName("offset");
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

void offsetGPUDeformer::terminate()
{
    fOffsetMatrixBuffer.reset();
    fRandomOffsetTableBuffer.reset();
    fKernelInfo.reset();

    MPxGPUStandardDeformer::terminate();
}

bool offsetGPUDeformer::extractOffsetMatrix(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug)
{
    // I pass the offset matrix to OpenCL using a buffer as well.  I also send down the inverse matrix to avoid calculating it many times on the GPU
    if (!isBufferUpdateNeeded(fOffsetMatrixBuffer, evaluationNode, offset::offsetMatrix))
    {
        return true; // we are up to date
    }

	MStatus status;
	MDataHandle matData = block.inputValue(offset::offsetMatrix, &status );
	if (MS::kSuccess != status)
        return false;

	MMatrix omat = matData.asMatrix();
	// Convert the matrix from Maya format to the format the OpenCL kernel expects
    MMatrixArray array(2);
    array.set(omat.transpose(), 0);
    array.set(omat.inverse().transpose(), 1);

    cl_int err = MOpenCLUtils::uploadToGPU(array, fOffsetMatrixBuffer, MOpenCLUtils::kBlocking);
    return (err == CL_SUCCESS);
}

bool offsetGPUDeformer::extractRandomOffsetTable(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug)
{
    if (!isBufferUpdateNeeded(fRandomOffsetTableBuffer, evaluationNode, offset::chunkCount))
    {
        return true; // we are up to date
    }

    // The randomOffsetTable on the CPU node has been modified so now go ask the CPU for its data.
    // Then upload it to a GPU buffer we can use in our kernel.

    // Deeper dive explanation:
    //
    // We could of course instead replicate the code that generates the randomOffsetTable based on the node parameters
    // here. But that would add a lot of maintenance costs to make sure we do the identical work to avoid mismatches
    // between the CPU and GPU implementation. The getFixedSetupData call below allows us to tunnel through to get
    // the exact data we want. This will move all logic to the CPU and the only responsibility of the GPU node is to
    // upload it and use it in the kernel.
    // This should of course only be done for data that does not depend in any way on any animated attributes. The
    // data should already be present on the CPU node and it should not cause any computation to be done in the
    // graph on the CPU.
    //

    cl_int err = CL_SUCCESS;
    err = uploadFixedSetupData("randomOffsetTable", fRandomOffsetTableBuffer, err, &fRandomOffsetTableSize);
    return (err == CL_SUCCESS);
}

// standard initialization procedures
//

MStatus initializePlugin( MObject obj )
{
	MStatus result;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");
	result = plugin.registerNode( "offset", offset::id, offset::creator,
								  offset::initialize, MPxNode::kDeformerNode );

	MString nodeClassName("offset");
	MString registrantId("mayaPluginExample");
	MGPUDeformerRegistry::registerGPUDeformerCreator(
		nodeClassName,
		registrantId,
		offsetGPUDeformer::getGPUDeformerInfo());

    // Specify the attributes which can determine the validaty of the GPU
    // deformer based on their value.
	MGPUDeformerRegistry::addConditionalAttribute(
			nodeClassName,
			registrantId,
			offset::chunkEnvelope);

	offset::pluginPath = plugin.loadPath();
	return result;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus result;

	MString nodeClassName("offset");
	MString registrantId("mayaPluginExample");
	MGPUDeformerRegistry::deregisterGPUDeformerCreator(
		nodeClassName,
		registrantId);

	MFnPlugin plugin(obj);
	result = plugin.deregisterNode(offset::id);

	return result;
}
