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

#include <maya/MDagModifier.h>

#include <maya/MPxGPUDeformer.h>
#include <maya/MGPUDeformerRegistry.h>
#include <maya/MOpenCLInfo.h>
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
    size_t numChunks = chunkCountData.asInt();

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


// the GPU override implementation of the offsetNode
//

class offsetGPUDeformer : public MPxGPUDeformer
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
	void extractWeightArray(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug);
	void extractOffsetMatrix(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug);
	void extractRandomOffsetTable(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug);
	void extractAffectMap();

	bool needsAffectMap() const;
	unsigned int affectCount() const;
	unsigned int fullCount() const;

	cl_int enqueueInitializeOutputPositions(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions);
	cl_int enqueueDeformation(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions);

	// holds the data for which verts are affected
	MIndexMapper fIndexMapper;

	// Storage for data on the GPU
	MAutoCLMem fCLWeights;
	MAutoCLMem fCLAffectMap;
	MAutoCLMem fCLOffsetMatrix;
	MAutoCLMem fCLRandomOffsetTable;

	unsigned int fNumElements;
	unsigned int fAffectMapBufferSize;
	unsigned int fRandomOffsetTableSize;
	float fChunkEnvelope;

	// Kernel
	MAutoCLKernel fKernel;
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
	: fNumElements(0)
	, fAffectMapBufferSize(0)
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
	// offsetGPUDeformer supports everything on the offset node except envelope
	// envelope is handled in validateNodeValues because we support some values
	// but not others.
	return true;
}

/* static */
bool offsetGPUDeformer::validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
	// As an example offsetGPUDeformer has conditional support for the envelop
	// attribute.  offsetGPUDeformer supports the underlying depend node if
	// envelope is exactly 1.0f.  Otherwise, the underlying node is not supported.
	// Note that in additional to testing the value of the envelope attribute here
	// we have also registered the envelope attribute as a conditional attribute in
	// offsetNodeGPUDeformerInfo.

	MObject node = plug.node();
	MFnDependencyNode fnNode(node);

	// Now that I know the envelope value is not changing, check to see if it is 1.0f
	MPlug envelopePlug(node, MPxDeformerNode::envelope);
	MDataHandle envData;
	envelopePlug.getValue(envData);
	if (envData.asFloat() != 1.0f)
	{
		MOpenCLInfo::appendMessage(messages, "Offset %s not supported by deformer evaluator because envelope is not exactly 1.0.", fnNode.name().asChar());
		return false;
	}

	return true;
}

cl_int offsetGPUDeformer::enqueueInitializeOutputPositions(
			MAutoCLEvent& syncEvent,
			const MGPUDeformerBuffer& inputPositions,
			MGPUDeformerBuffer& outputPositions)
{
	cl_int err = CL_SUCCESS;

	if (!needsAffectMap()) // Nothing to do here...
		return err;

	// We need to copy the input verts over the output verts since we are
	// not going to compute every single one of them.

	MAutoCLEvent syncInputEvent = syncEvent;
	syncEvent = MAutoCLEvent();

	MGPUEventList eventList;
	eventList.add(syncInputEvent);

	const size_t fullVertBufSize = fNumElements * sizeof(float) * 3;

	err = clEnqueueCopyBuffer (
				MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(),
				inputPositions.buffer().get(),
				outputPositions.buffer().get(),
				0,
				0,
				fullVertBufSize,
				eventList.size(),
				eventList.array(),
				syncEvent.getReferenceForAssignment());

	MOpenCLInfo::checkCLErrorStatus(err);

	return err;
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
  
	// Set all of our kernel parameters.  Input buffer and output buffer may be changing every frame
	// so always set them.
	unsigned int parameterId = 0;
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)outputPositions.buffer().getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)inputPositions.buffer().getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)fCLWeights.getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	if (needsAffectMap())
		err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)fCLAffectMap.getReadOnlyRef());
	else
		err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem),nullptr);
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)fCLOffsetMatrix.getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);

	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)fCLRandomOffsetTable.getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_uint), (void*)&fRandomOffsetTableSize);
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_float), (void*)&fChunkEnvelope);
	MOpenCLInfo::checkCLErrorStatus(err);

	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_uint), (void*)&count);
	MOpenCLInfo::checkCLErrorStatus(err);

	// Figure out a good work group size for our kernel.
	size_t workGroupSize;
	size_t retSize;
	err = clGetKernelWorkGroupInfo(
		fKernel.get(),
		MOpenCLInfo::getOpenCLDeviceId(),
		CL_KERNEL_WORK_GROUP_SIZE,
		sizeof(size_t),
		&workGroupSize,
		&retSize);
	MOpenCLInfo::checkCLErrorStatus(err);

	size_t localWorkSize = 256;
	if (retSize > 0) localWorkSize = workGroupSize;
	size_t globalWorkSize = (localWorkSize - count % localWorkSize) + count; // global work size must be a multiple of localWorkSize

	// run the kernel
	MAutoCLEvent kernelFinishedEvent;
	err = clEnqueueNDRangeKernel(
		MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(),
		fKernel.get(),
		1,
		NULL,
		&globalWorkSize,
		&localWorkSize,
		eventList.size(),
		eventList.array(),
		syncEvent.getReferenceForAssignment());

	MOpenCLInfo::checkCLErrorStatus(err);

	return err;
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
    // offsetGPUDeformer only supports a single input plug
    if (inputPlugs.length() != 1)
        return MPxGPUDeformer::kDeformerFailure;
    const MPlug& inputPlug = inputPlugs[0];

	const MGPUDeformerBuffer inputPositions = inputData.getBuffer(MPxGPUDeformer::sPositionsName(), inputPlug);
	MGPUDeformerBuffer outputPositions = createOutputBuffer(inputPositions);

	if (!inputPositions.isValid() || !outputPositions.isValid())
		return MPxGPUDeformer::kDeformerFailure;

	// evaluate has two main pieces of work.  I need to transfer any data I care about onto the GPU, and I need to run my OpenCL Kernel.
	// First, transfer the data.  offset has two pieces of data I need to transfer to the GPU, the weight array and the offset matrix.
	// I don't need to transfer down the input position buffer, that is already handled by the deformer evaluator, the points are in inputBuffer.
	fNumElements = inputPositions.elementCount();

	MObject node = plug.node();

	extractAffectMap();
	extractWeightArray(block, evaluationNode, plug);
	extractOffsetMatrix(block, evaluationNode, plug);
	extractRandomOffsetTable(block, evaluationNode, plug);

	MStatus status;
	MDataHandle chunkEnvelopeData = block.inputValue(offset::chunkEnvelope, &status );
	fChunkEnvelope = (MS::kSuccess != status) ? 1.0 : chunkEnvelopeData.asFloat();

	// Now that all the data we care about is on the GPU, setup and run the OpenCL Kernel
	if (!fKernel.get())
	{
		MString openCLKernelFile = offset::pluginPath + "/offset.cl";
		MString openCLKernelName("offset");
		fKernel = MOpenCLInfo::getOpenCLKernel(openCLKernelFile, openCLKernelName);

		if (!fKernel) return MPxGPUDeformer::kDeformerFailure;
	}

	MAutoCLEvent syncEvent = inputPositions.bufferReadyEvent();
	cl_int err = CL_SUCCESS;

	err = enqueueInitializeOutputPositions(syncEvent, inputPositions, outputPositions);
	if ( err != CL_SUCCESS )
		return MPxGPUDeformer::kDeformerFailure;

	err = enqueueDeformation(syncEvent, inputPositions, outputPositions);
	if ( err != CL_SUCCESS )
		return MPxGPUDeformer::kDeformerFailure;

	outputPositions.setBufferReadyEvent(syncEvent);
	outputData.setBuffer(outputPositions);

	return MPxGPUDeformer::kDeformerSuccess;
}

void offsetGPUDeformer::terminate()
{
	MHWRender::MRenderer::theRenderer()->releaseGPUMemory(fNumElements*sizeof(float));
	fCLWeights.reset();
	MHWRender::MRenderer::theRenderer()->releaseGPUMemory(fAffectMapBufferSize);
	fCLAffectMap.reset();
	fCLOffsetMatrix.reset();
	fCLRandomOffsetTable.reset();
	MOpenCLInfo::releaseOpenCLKernel(fKernel);
	fKernel.reset();
}

void offsetGPUDeformer::extractAffectMap()
{
	if (getIndexMapper(fIndexMapper)) {

		if (!needsAffectMap()) {
			// No need for it, so release what we had...
			MHWRender::MRenderer::theRenderer()->releaseGPUMemory(fAffectMapBufferSize);
			fCLAffectMap.reset();
			fAffectMapBufferSize = 0;
			return;
		}

		// Now upload the affect map
		// Two possibilities, we could be updating an existing OpenCL buffer or allocating a new one.
		cl_int err = CL_SUCCESS;

		std::vector<unsigned int> temp;
		temp.resize(affectCount());
		unsigned int bufferSize = temp.size()*sizeof(unsigned int);

		MIntArray affectMap = fIndexMapper.affectMap();
		for (size_t i = 0; i < affectMap.length(); ++i)
			temp[i] = affectMap[i];

		if (fAffectMapBufferSize < bufferSize) { // We need to grow the size of the buffer
			MHWRender::MRenderer::theRenderer()->releaseGPUMemory(fAffectMapBufferSize);
			fCLAffectMap.reset(); // Release what we had...
			fAffectMapBufferSize = 0;
		}

		if (!fCLAffectMap.get())
		{
			fAffectMapBufferSize = bufferSize;
			MHWRender::MRenderer::theRenderer()->holdGPUMemory(fAffectMapBufferSize);
			fCLAffectMap.attach(clCreateBuffer(MOpenCLInfo::getOpenCLContext(), CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, bufferSize, (void*)&temp[0], &err));
		}
		else
		{
			// Using a blocking write here, non-blocking could be faster...  need to manage the lifetime of temp, and have the kernel wait until the write finishes before running
			err = clEnqueueWriteBuffer(MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(), fCLAffectMap.get(), CL_TRUE, 0, bufferSize, (void*)&temp[0], 0, NULL, NULL);
		}
	}

}

bool offsetGPUDeformer::needsAffectMap() const
{
	return (affectCount() < fullCount());
}

unsigned int offsetGPUDeformer::affectCount() const
{
	return fIndexMapper.affectCount();
}

unsigned int offsetGPUDeformer::fullCount() const
{
	return fIndexMapper.fullCount();
}

void offsetGPUDeformer::extractWeightArray(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug)
{
	// if we've already got a weight array and it is not changing then don't bother copying it
	// to the GPU again

	// Note that right now hasAttributeBeenModified takes an attribute, so if any element in the multi is changing we think it is dirty...
	// To avoid false dirty issues here you'd need to only use one element of the MPxDeformerNode::input multi attribute for each
	// offset node.
	if (fCLWeights.get() && !MPxGPUDeformer::hasAttributeBeenModified(evaluationNode, MPxDeformerNode::weightList))
	{
		return;
	}

	// Maya might do some tricky stuff like not store the weight array at all for certain weight
	// values so we can't count on an array existing in the weightList.  For the OpenCL Kernel
	// we want an array with one weight in it per vertex, we need to build it carefully here.
	std::vector<float> temp;
	temp.reserve(fNumElements);

	// Two possibilities: we could have a sparse array in weightList[multiIndex] or there could be nothing in weightList[multiIndex].
	// if nothing is there then all the weights at 1.0f.

	// Get a handle to the weight array we want.
	MStatus status;
	MArrayDataHandle weightList = block.outputArrayValue(MPxDeformerNode::weightList, &status);
	if (!status) return; // we should always be able to get a weightList
	status = weightList.jumpToElement(plug.logicalIndex());
	// it is possible that the jumpToElement fails.  In that case all weights are 1.
	if (!status)
	{
		for(unsigned int i=0; i<fNumElements; i++)
			temp.push_back(1.0f);
	}
	else
	{
		MDataHandle weightsStructure = weightList.inputValue(&status);
		if (!status) return;
		MArrayDataHandle weights = weightsStructure.child(MPxDeformerNode::weights);
		if (!status) return;

		// number of non-zero weights
		unsigned int numWeights = weights.elementCount(&status);
		if (!status) return;

		// we're building a list with a weight per vertex, even if the weight is zero
		unsigned int weightIndex = 0;
		for(unsigned int i=0; i<numWeights; i++, weights.next())
		{
			unsigned int weightsElementIndex = weights.elementIndex(&status);
			while (weightIndex < weightsElementIndex)
			{
				temp.push_back(1.0f); // weights could be sparse, fill in default weight of 1 if no data
				weightIndex++;
			}
			MDataHandle value = weights.inputValue(&status);
			temp.push_back(value.asFloat());
			weightIndex++;
		}
		// now we have written the last non-zero weight into temp, but the last non-zero weight
		// doesn't have to be for the last vertex in the buffer.  Add more zero values if necessary.
		while (weightIndex < fNumElements)
		{
			temp.push_back(1.0f); // weights could be sparse, fill in default weight of 1 if no data
			weightIndex++;
		}
	}

	// Two possibilities, we could be updating an existing OpenCL buffer or allocating a new one.
	cl_int err = CL_SUCCESS;
	if (!fCLWeights.get())
	{
		MHWRender::MRenderer::theRenderer()->holdGPUMemory(fNumElements*sizeof(float));
		fCLWeights.attach(clCreateBuffer(MOpenCLInfo::getOpenCLContext(), CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, fNumElements * sizeof(float), (void*)&temp[0], &err));
	}
	else
	{
		// I use a blocking write here, non-blocking could be faster...  need to manage the lifetime of temp, and have the kernel wait until the write finishes before running
		// I'm also assuming that the weight buffer is not growing.
		err = clEnqueueWriteBuffer(MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(), fCLWeights.get(), CL_TRUE, 0, fNumElements * sizeof(float), (void*)&temp[0], 0, NULL, NULL);
	}
}

void offsetGPUDeformer::extractOffsetMatrix(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug)
{
	// I pass the offset matrix to OpenCL using a buffer as well.  I also send down the inverse matrix to avoid calculating it many times on the GPU
	if (fCLOffsetMatrix.get() && !MPxGPUDeformer::hasAttributeBeenModified(evaluationNode, offset::offsetMatrix))
	{
		return;
	}

	MStatus status;
	MDataHandle matData = block.inputValue(offset::offsetMatrix, &status );
	if (MS::kSuccess != status) return;
	MMatrix omat = matData.asMatrix();
	MMatrix omatinv = omat.inverse();

	// Convert the matrix from Maya format to the format the OpenCL kernel expects
	omat = omat.transpose();
	omatinv = omatinv.transpose();

	// MMatrix stores double values, but I want floating point values on the GPU so convert them here.
	unsigned int numFloat = 32;
	float* temp = new float[numFloat];
	unsigned int curr = 0;

	for(unsigned int row = 0; row<4; row++)
	{
		for(unsigned int column = 0; column<4; column++)
		{
			temp[curr++] = (float)omat(row, column);
		}
	}

	for(unsigned int row = 0; row<4; row++)
	{
		for(unsigned int column = 0; column<4; column++)
		{
			temp[curr++] = (float)omatinv(row, column);
		}
	}

	// Two possibilities, we could be updating an existing OpenCL buffer or allocating a new one.
	cl_int err = CL_SUCCESS;
	if (!fCLOffsetMatrix.get())
	{
		fCLOffsetMatrix.attach(clCreateBuffer(MOpenCLInfo::getOpenCLContext(), CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, numFloat * sizeof(float), (void*) temp, &err));
	}
	else
	{
		// I use a blocking write here, non-blocking could be faster...  need to manage the lifetime of temp, and have the kernel wait until the write finishes before running
		err = clEnqueueWriteBuffer(MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(), fCLOffsetMatrix.get(), CL_TRUE, 0, numFloat * sizeof(float), (void*)temp, 0, NULL, NULL);
	}

	delete [] temp;
}

void offsetGPUDeformer::extractRandomOffsetTable(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug)
{
	if (fCLRandomOffsetTable.get() && !MPxGPUDeformer::hasAttributeBeenModified(evaluationNode,  offset::chunkCount))
	{
		return;
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

    MObject arrOb = getFixedSetupData("randomOffsetTable");
    if (!arrOb.isNull())
    {
        // upload it to the gpu buffer
        if (arrOb.hasFn(MFn::kFloatArrayData)) {
            MFnFloatArrayData fn(arrOb);
            MFloatArray array;
            array.copy(fn.array());
            fRandomOffsetTableSize = array.length();
            if (fRandomOffsetTableSize <= 0) return;
            float& f = array[0];
            cl_int err = CL_SUCCESS;
            if (!fCLRandomOffsetTable.get())
            {
                fCLRandomOffsetTable.attach(clCreateBuffer(MOpenCLInfo::getOpenCLContext(), CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, fRandomOffsetTableSize * sizeof(float), (void*)&f, &err));
            }
            else
            {
                // I use a blocking write here, non-blocking could be faster...  need to manage the lifetime of temp, and have the kernel wait until the write finishes before running
                err = clEnqueueWriteBuffer(MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(), fCLRandomOffsetTable.get(), CL_TRUE, 0, fRandomOffsetTableSize * sizeof(float), (void*)&f, 0, NULL, NULL);
            }
            return;
        }
    }

  	fCLRandomOffsetTable.reset();
    fRandomOffsetTableSize = 0;
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

	MGPUDeformerRegistry::addConditionalAttribute(
			nodeClassName,
			registrantId,
			MPxDeformerNode::envelope);

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
