//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+
#include "blindDataMeshVertexBufferGenerator.h"
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>
#include <maya/MItMeshVertex.h>


bool blindDataMeshVertexBufferGenerator::getSourceIndexing(const MObject& object, MComponentDataIndexing& sourceIndexing) const
{
	// get the mesh from the current path
	// if it is not a mesh we do nothing.
	MStatus status;
	MFnMesh mesh(object, &status);
	if (!status) 
		return false;

	MIntArray vertexCount, vertexList;
	mesh.getVertices(vertexCount, vertexList);
	unsigned int vertCount = vertexList.length();

	MUintArray& vertices = sourceIndexing.indices();
	vertices.setLength(vertCount);

	for (unsigned int i = 0; i < vertCount; ++i)
		vertices[i] = (unsigned int)vertexList[i];

	// assign the source indexing
	sourceIndexing.setComponentType(MComponentDataIndexing::kFaceVertex);

	return true;
}



bool blindDataMeshVertexBufferGenerator::getSourceStreams(const MObject& object, MStringArray &sourceStreams) const
{
	return false;
}



void blindDataMeshVertexBufferGenerator::createVertexStream(const MObject& object, MVertexBuffer& vertexBuffer, const MComponentDataIndexing& targetIndexing, const MComponentDataIndexing& sharedIndexing, const MVertexBufferArray& sourceStreams) const
{
	const MVertexBufferDescriptor& descriptor = vertexBuffer.descriptor();
		
	// We are expected 4 floats with the color semantic 
	if ( (descriptor.dataType() != MGeometry::kFloat) || 
		 (descriptor.dimension() != 4) || 
		 (descriptor.semantic() != MGeometry::kColor))
		 return;

	// object should be a mesh
	MStatus status;
	MFnMesh mesh(object, &status);
	if (!status) 
		return;

	const MUintArray& indices = targetIndexing.indices();

	const unsigned int vertexCount = indices.length();
	if (vertexCount <= 0)
		return;

	const unsigned vertexBufferSize = vertexCount * 4 * sizeof(float);
	float* customBuffer = (float*)vertexBuffer.acquire(vertexBufferSize, true /*writeOnly*/);

	if (customBuffer)
	{	
		//
		// Extract the mesh blind data and copy it into the vertex buffer.
		//

		// Constant use to extract the blind data. See blindDataMesh::setMeshBlindData()
		const int blindDataID = 60;

		MItMeshVertex itVertex((MObject&)object);
		for (; !itVertex.isDone(); itVertex.next())
		{
			double red, green, blue = 0;

			int vertexIndex = itVertex.index();

			mesh.getDoubleBlindData(vertexIndex, MFn::kMeshVertComponent, blindDataID, "red", red);
			mesh.getDoubleBlindData(vertexIndex, MFn::kMeshVertComponent, blindDataID, "green", green);
			mesh.getDoubleBlindData(vertexIndex, MFn::kMeshVertComponent, blindDataID, "blue", blue);

			customBuffer[vertexIndex * 4    ] = static_cast<float>(red);
			customBuffer[vertexIndex * 4 + 1] = static_cast<float>(green);
			customBuffer[vertexIndex * 4 + 2] = static_cast<float>(blue);
			customBuffer[vertexIndex * 4 + 3] = 1.0f;
		}
		
		vertexBuffer.commit(customBuffer);
	}
}
