//-
// ===========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//
// Produces the dependency graph node "basicMorphNode".
//
// This plug-in demonstrates how to create a user-defined deformer which
// supports fan-in GPU evaluation. A deformer is a node which takes 
// input geometries, deforms them, and places the output into the output
// geometry attribute. Fan-in evaluation allows for the deformer to take multiple
// different GPU evaluation chains and use their results to drive further deformation
// without reading their results back into the CPU. This allows uninterrupted 
// GPU evaluation. This example plug-in defines a new deformer node
// that morphs a base mesh to mimic a target mesh.
//
// The basic morph node example below is not meant to be a practical deformer to
// be used: its main function is to explain certain concepts and give examples
// on how to use the API.
//
// The node morphs all verts of the deforming geometry to match the target
// geometry in the local coordinate system. Both geometries must have an
// equal number of vertices and they should be in the same order.
// The degree of morphing is controlled by the envelope attribute.
// An envelope of 1 will match the target exactly, while an envelope of
// 0 will not deform at all. Envelope also supports negative values and 
// values greater than 1.
//
// To use this node:
//	- load the basicMorphNode plugin
//	- create some polygonal object
//	- type: "deformer -type basicMorph" to create a basicMorph node on the object
//	- create a target object with the same number of verts as the first object
//	- connect the target mesh to the basicMorph's targetGeometry attribute
//		This can be done with the node editor or a command such as 
//		"connectAttr target.outMesh basicMorph1.targetGeometry"
//
// Use this script to create a simple example with the basic morph node:
//
//	loadPlugin basicMorphNode;
//
//	polyCube - name target - sx 6 - sy 6 - sz 6;
//	move 3 0 0;
//	polyCube - name base - sx 6 - sy 6 - sz 6;
//	sculpt;
//	select - r base;
//	deformer - type basicMorph - name morph;
//	connectAttr targetShape.outMesh morph.targetGeometry;
//
//	setKeyframe - v 0 - at envelope - t 1 morph;
//	setKeyframe - v 1 - at envelope - t 60 morph;
//	select - cl;
//
////////////////////////////////////////////////////////////////////////



#include <maya/MPxDeformerNode.h>
#include <maya/MItGeometry.h>

#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnData.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnGeometryData.h>
#include <maya/MFnPlugin.h>

#include <maya/MGlobal.h>
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
#include <clew/clew.h>
#include <vector>


////////////////////////////////////////////////////////////////////////
//
// Basic Morph CPU Implementation
//
////////////////////////////////////////////////////////////////////////

class basicMorph : public MPxDeformerNode
{
public:
	basicMorph();
	~basicMorph() override;

	static void* creator();
	static MStatus initialize();

	// deformation function
	MStatus deform(MDataBlock& block,
		MItGeometry& iter,
		const MMatrix& mat,
		unsigned int multiIndex) override;

public:

	// local node attributes
	static MObject targetGeometry;
	static MTypeId id;

	// path from where the plugin was loaded
	static MString pluginPath;

private:
};

// local attributes
MTypeId basicMorph::id(0x0008006E);
MObject basicMorph::targetGeometry;
MString basicMorph::pluginPath;

basicMorph::basicMorph() {}
basicMorph::~basicMorph() {}

void* basicMorph::creator()
{
	return new basicMorph();
}

/*!
	Initializes local attributes.
	\return initialization status
*/
MStatus basicMorph::initialize()
{
	// local attribute initialization
	MFnTypedAttribute meshAttr;
	targetGeometry = meshAttr.create("targetGeometry", "tg", MFnData::kMesh);
	meshAttr.setStorable(false);
	meshAttr.setConnectable(true);

	//  deformation attributes
	addAttribute(targetGeometry);

	attributeAffects(basicMorph::targetGeometry, basicMorph::outputGeom);

	return MStatus::kSuccess;
}

/*!
	This method performs the morph deformation by moving the points
	on the base mesh toward the points on the target mesh.
	\param[in] block the datablock of the node
	\param[in] iter an iterator for the geometry to be deformed
	\param[in] m matrix to transform the point into world space
	\param[in] multiIndex the index of the geometry that we are deforming
	\return the deformation status
*/
MStatus basicMorph::deform(MDataBlock& block,
	MItGeometry& iter,
	const MMatrix&,
	unsigned int multiIndex)
{
	MStatus returnStatus = MS::kSuccess;

	// Envelope data from the base class.
	// The envelope is simply a scale factor.
	MDataHandle envData = block.inputValue(envelope, &returnStatus);
	CHECK_MSTATUS_AND_RETURN_IT(returnStatus);
	float env = envData.asFloat();

	// Get the base geometry
	MArrayDataHandle inputHandle = block.outputArrayValue(input, &returnStatus);
	CHECK_MSTATUS_AND_RETURN_IT(returnStatus);
	returnStatus = inputHandle.jumpToElement(multiIndex);
	MDataHandle inputElementHandle = inputHandle.outputValue(&returnStatus);
	MObject inputGeometry = inputElementHandle.child(inputGeom).asMesh();
	MFnMesh baseMesh(inputGeometry, &returnStatus);
	CHECK_MSTATUS_AND_RETURN_IT(returnStatus);

	// Get the target geometry
	MDataHandle tgtGeoData = block.inputValue(targetGeometry, &returnStatus);
	CHECK_MSTATUS_AND_RETURN_IT(returnStatus);
	MFnMesh targetMesh(tgtGeoData.asMesh(), &returnStatus);
	CHECK_MSTATUS_AND_RETURN_IT(returnStatus);

	if (targetMesh.numVertices() != baseMesh.numVertices()) {
		// Our geometry must match the target's
		returnStatus = MS::kFailure;
		return returnStatus;
	}

	// iterate through each point in the geometry
	for (; !iter.isDone(); iter.next()) {
		MPoint pt = iter.position();
		unsigned int ptIndex = iter.index();
		MPoint tgt;
		returnStatus = targetMesh.getPoint(ptIndex, tgt);
		CHECK_MSTATUS_AND_RETURN_IT(returnStatus);

		// morph algorithm
		pt += (tgt - pt) * env;

		iter.setPosition(pt);
	}
	return returnStatus;
}


////////////////////////////////////////////////////////////////////////
//
// Basic Morph GPU Implementation
//
////////////////////////////////////////////////////////////////////////

class basicMorphGPUDeformer : public MPxGPUDeformer
{
public:
	// Virtual methods from MPxGPUDeformer
	basicMorphGPUDeformer();
	~basicMorphGPUDeformer() override;

	MPxGPUDeformer::DeformerStatus evaluate(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& outputPlug, const MPlugArray& inputPlugs, const MGPUDeformerData& inputData, MGPUDeformerData& outputData) override;
	void terminate() override;

	static MGPUDeformerRegistrationInfo* getGPUDeformerInfo();
	static bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode&, const MPlug& plug, MStringArray* messages);
	static bool validateNodeValues(MDataBlock& block, const MEvaluationNode&, const MPlug& plug, MStringArray* messages);

private:
	// helper methods
	void extractWeightArray(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug);
	void extractAffectMap();

	bool needsAffectMap() const;
	unsigned int affectCount() const;
	unsigned int fullCount() const;

	cl_int enqueueInitializeOutputPositions(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions);
	cl_int enqueueDeformation(MAutoCLEvent& syncEvent, const MGPUDeformerBuffer& inputPositions, MGPUDeformerBuffer& outputPositions, const MGPUDeformerBuffer& targetPositions);

	// holds the data for which verts are affected
	MIndexMapper fIndexMapper;

	// Storage for data on the GPU
	MAutoCLMem fCLWeights;
	MAutoCLMem fCLAffectMap;

	unsigned int fNumElements;
	unsigned int fAffectMapBufferSize;
	float fEnvelope;

	// Kernel
	MAutoCLKernel fKernel;
};

class basicMorphNodeGPUDeformerInfo : public MGPUDeformerRegistrationInfo
{
public:
	basicMorphNodeGPUDeformerInfo() {}
	~basicMorphNodeGPUDeformerInfo() override {}

	MPxGPUDeformer* createGPUDeformer() override
	{
		return new basicMorphGPUDeformer();
	}

	bool validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
	{
		return basicMorphGPUDeformer::validateNodeInGraph(block, evaluationNode, plug, messages);
	}

	bool validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages) override
	{
		return basicMorphGPUDeformer::validateNodeValues(block, evaluationNode, plug, messages);
	}

	void inputMeshAttributes(MObjectArray& iInputAttributes) override
	{
		// Add the target geometry to the input attributes so its position buffer can be accessed
		iInputAttributes.append(basicMorph::targetGeometry);
	}
};

/*!
	\return The GPU deformer information
*/
MGPUDeformerRegistrationInfo* basicMorphGPUDeformer::getGPUDeformerInfo()
{
	static basicMorphNodeGPUDeformerInfo theOne;
	return &theOne;
}

basicMorphGPUDeformer::basicMorphGPUDeformer()
	: fNumElements(0)
	, fAffectMapBufferSize(0)
{
	// Remember the ctor must be fast.  No heavy work should be done here.
	// Maya may allocate one of these and then never use it.
}

basicMorphGPUDeformer::~basicMorphGPUDeformer()
{
	terminate();
}

/* static */
bool basicMorphGPUDeformer::validateNodeInGraph(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
	// basicMorphGPUDeformer supports everything on the basicMorph node
	return true;
}

/* static */
bool basicMorphGPUDeformer::validateNodeValues(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug, MStringArray* messages)
{
	return true;
}

/*!
	Initialize the output buffer by copying the input buffer into it
	\param[in,out] syncEvent CL event we and downstream GPU operations must sync to
	\param[in] inputPositions the input position buffer
	\param[in] outputPositions the output position buffer
	\return OpenCL error status
*/
cl_int basicMorphGPUDeformer::enqueueInitializeOutputPositions(
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

	err = clEnqueueCopyBuffer(
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

/*!
	Set all kernel parameters and enqueue the deformation kernel to compute 
	the new output positions.
	\param[in,out] syncEvent CL event we and downstream GPU operations must sync to
	\param[in] inputPositions the input position buffer
	\param[in] outputPositions the output position buffer
	\param[in] targetPositions the target position buffer
	\return OpenCL error status
*/
cl_int basicMorphGPUDeformer::enqueueDeformation(
	MAutoCLEvent& syncEvent,
	const MGPUDeformerBuffer& inputPositions,
	MGPUDeformerBuffer& outputPositions,
	const MGPUDeformerBuffer& targetPositions)
{
	cl_int err = CL_SUCCESS;

	unsigned int count = affectCount();

	MAutoCLEvent syncInputEvent = syncEvent;
	syncEvent = MAutoCLEvent();

	MAutoCLEvent syncTargetEvent = targetPositions.bufferReadyEvent();

	// Add all events we need to wait for to the event list
	MGPUEventList eventList;
	eventList.add(syncInputEvent);
	eventList.add(syncTargetEvent);

	// Set all of our kernel parameters.  Input, target and output buffers may be changing every frame
	// so always set them.
	unsigned int parameterId = 0;
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)outputPositions.buffer().getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)inputPositions.buffer().getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)targetPositions.buffer().getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_float), (void*)&fEnvelope);
	MOpenCLInfo::checkCLErrorStatus(err);
	err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)fCLWeights.getReadOnlyRef());
	MOpenCLInfo::checkCLErrorStatus(err);

	if (needsAffectMap())
		err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), (void*)fCLAffectMap.getReadOnlyRef());
	else
		err = clSetKernelArg(fKernel.get(), parameterId++, sizeof(cl_mem), nullptr);
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

/*!
	Prepare and execute GPU deformer evaluation.
	\param[in] block data block for the overwridden deformer
	\param[in] evaluationNode evaluation node representing the overwridden deformer
	\param[in] outputPlug the output plug of the overwridden deformer
	\param[in] inputPlugs the input plugs which have GPU data in inputData
	\param[in] inputData the input data provided by Maya or other upstream GPU Deformers
	\param[in] outputData the output data to be passed to the rendering system or other downstream GPU Deformers
	\return deformer status
*/
MPxGPUDeformer::DeformerStatus basicMorphGPUDeformer::evaluate(
	MDataBlock& block,
	const MEvaluationNode& evaluationNode,
	const MPlug& outputPlug,
	const MPlugArray& inputPlugs,
	const MGPUDeformerData& inputData,
	MGPUDeformerData& outputData
)
{
	MStatus status;

	// basicMorphGPUDeformer only supports two inputs, a base and a target mesh
	if (inputPlugs.length() != 2)
		return MPxGPUDeformer::kDeformerFailure;

	MDataHandle envelopeData = block.inputValue(basicMorph::envelope, &status);
	fEnvelope = (MS::kSuccess != status) ? 1.0 : envelopeData.asFloat();
	if (fEnvelope == 0.0) {
		// Deformer will have no effect, just pass through without further processing
		return MPxGPUDeformer::kDeformerPassThrough;
	}

	// Find the plugs for both inputs
	MPlug inputPlug;
	MPlug targetPlug;
	for (unsigned int i = 0; i < inputPlugs.length(); i++)
	{
		MPlug plug = inputPlugs[i];
		if (plug.attribute(&status) == basicMorph::inputGeom) // check status
		{
			inputPlug = plug;
			continue;
		}
		else if (plug.attribute(&status) == basicMorph::targetGeometry) // check status
		{
			targetPlug = plug;
			continue;
		}
	}
	if (MS::kSuccess != status)
		return MPxGPUDeformer::kDeformerFailure;

	const MGPUDeformerBuffer inputPositions = inputData.getBuffer(MPxGPUDeformer::sPositionsName(), inputPlug);
	MGPUDeformerBuffer outputPositions = createOutputBuffer(inputPositions);

	if (!inputPositions.isValid() || !outputPositions.isValid())
		return MPxGPUDeformer::kDeformerFailure;

	// evaluate has two main pieces of work. I need to transfer any data I care about onto the GPU, and I need to run my OpenCL Kernel.
	// First, transfer the data. There are two pieces of data I need to buffer on the GPU, the weight array and the affect map.
	// I don't need to transfer down the input or target position buffers, they are already handled by the deformer evaluator.
	// I just need to establish references to their position buffers.
	fNumElements = inputPositions.elementCount();

	// get the target positions
	const MGPUDeformerBuffer targetPositions = inputData.getBuffer(MPxGPUDeformer::sPositionsName(), targetPlug);
	if (!targetPositions.isValid())
		return MPxGPUDeformer::kDeformerFailure;

	// basicMorphGPUDeformer only supports geometries with an equal number of vertices
	if (targetPositions.elementCount() != fNumElements)
		return MPxGPUDeformer::kDeformerFailure;

	extractAffectMap();
	extractWeightArray(block, evaluationNode, outputPlug);

	// Now that all the data we care about is on the GPU, setup and run the OpenCL Kernel
	if (!fKernel.get())
	{
		MString openCLKernelFile = basicMorph::pluginPath + "/basicMorph.cl";
		MString openCLKernelName("basicMorph");
		fKernel = MOpenCLInfo::getOpenCLKernel(openCLKernelFile, openCLKernelName);

		if (!fKernel) return MPxGPUDeformer::kDeformerFailure;
	}

	MAutoCLEvent syncEvent = inputPositions.bufferReadyEvent();
	cl_int err = CL_SUCCESS;

	err = enqueueInitializeOutputPositions(syncEvent, inputPositions, outputPositions);
	if (err != CL_SUCCESS)
		return MPxGPUDeformer::kDeformerFailure;

	err = enqueueDeformation(syncEvent, inputPositions, outputPositions, targetPositions);
	if (err != CL_SUCCESS)
		return MPxGPUDeformer::kDeformerFailure;

	outputPositions.setBufferReadyEvent(syncEvent);
	outputData.setBuffer(outputPositions);

	return MPxGPUDeformer::kDeformerSuccess;
}

/*!
	Release any reserved GPU resources.
*/
void basicMorphGPUDeformer::terminate()
{
	MHWRender::MRenderer::theRenderer()->releaseGPUMemory(fNumElements * sizeof(float));
	fCLWeights.reset();
	MHWRender::MRenderer::theRenderer()->releaseGPUMemory(fAffectMapBufferSize);
	fCLAffectMap.reset();
	MOpenCLInfo::releaseOpenCLKernel(fKernel);
	fKernel.reset();
}

/*!
	Obtain the affect map which identifies the vertices this deformer affects
	and buffer it on the GPU.
*/
void basicMorphGPUDeformer::extractAffectMap()
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
		unsigned int bufferSize = temp.size() * sizeof(unsigned int);

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

/*!
	\return whether an affect map is necessary
*/
bool basicMorphGPUDeformer::needsAffectMap() const
{
	return (affectCount() < fullCount());
}

/*!
	\return count of vertices affected by this deformer
*/
unsigned int basicMorphGPUDeformer::affectCount() const
{
	return fIndexMapper.affectCount();
}

/*!
	\return input vertex count
*/
unsigned int basicMorphGPUDeformer::fullCount() const
{
	return fIndexMapper.fullCount();
}

/*!
	Extract per-vertex deformer weights and copy them into a GPU buffer
	\param[in] block data block for the overwridden deformer
	\param[in] evaluationNode evaluation node representing the overwridden deformer
	\param[in] plug the output plug of the overwridden deformer
*/
void basicMorphGPUDeformer::extractWeightArray(MDataBlock& block, const MEvaluationNode& evaluationNode, const MPlug& plug)
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
		for (unsigned int i = 0; i < fNumElements; i++)
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
		for (unsigned int i = 0; i < numWeights; i++, weights.next())
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
		MHWRender::MRenderer::theRenderer()->holdGPUMemory(fNumElements * sizeof(float));
		fCLWeights.attach(clCreateBuffer(MOpenCLInfo::getOpenCLContext(), CL_MEM_COPY_HOST_PTR | CL_MEM_READ_ONLY, fNumElements * sizeof(float), (void*)&temp[0], &err));
	}
	else
	{
		// I use a blocking write here, non-blocking could be faster...  need to manage the lifetime of temp, and have the kernel wait until the write finishes before running
		// I'm also assuming that the weight buffer is not growing.
		err = clEnqueueWriteBuffer(MOpenCLInfo::getMayaDefaultOpenCLCommandQueue(), fCLWeights.get(), CL_TRUE, 0, fNumElements * sizeof(float), (void*)&temp[0], 0, NULL, NULL);
	}
}


////////////////////////////////////////////////////////////////////////
//
// Standard Initialization Procedures
//
////////////////////////////////////////////////////////////////////////

MStatus initializePlugin(MObject obj)
{
	MStatus result;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");
	result = plugin.registerNode("basicMorph", basicMorph::id, basicMorph::creator,
		basicMorph::initialize, MPxNode::kDeformerNode);

	MString nodeClassName("basicMorph");
	MString registrantId("mayaPluginExample");
	MGPUDeformerRegistry::registerGPUDeformerCreator(
		nodeClassName,
		registrantId,
		basicMorphGPUDeformer::getGPUDeformerInfo());

	MGPUDeformerRegistry::addConditionalAttribute(
		nodeClassName,
		registrantId,
		MPxDeformerNode::envelope);

	basicMorph::pluginPath = plugin.loadPath();
	return result;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus result;

	MString nodeClassName("basicMorph");
	MString registrantId("mayaPluginExample");
	MGPUDeformerRegistry::deregisterGPUDeformerCreator(nodeClassName, registrantId);

	MFnPlugin plugin(obj);
	result = plugin.deregisterNode(basicMorph::id);

	return result;
}
