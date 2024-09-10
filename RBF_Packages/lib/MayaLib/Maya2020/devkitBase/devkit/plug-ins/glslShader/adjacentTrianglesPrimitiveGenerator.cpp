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

// Example plugin: adjacentTrianglesPrimitiveGenerator.cpp
//
// This plug-in is an example of a custom MPxIndexBufferMutator.
// It provides custom primitives based on shader requirements coming from 
// an MPxShaderOverride.  The name() in the MIndexBufferDescriptor is used 
// to signify a unique identifier for a custom buffer.
//
// Based on public domain code found here: http://prideout.net/blog/?p=54

#include "adjacentTrianglesPrimitiveGenerator.h"
#include <maya/MStatus.h>
#include <maya/MFnMesh.h>
#include <maya/MIntArray.h>
#include <maya/MFloatArray.h>
#include <maya/MPxVertexBufferGenerator.h>
#include <maya/MHWGeometry.h>
#include <maya/MDrawRegistry.h>
#include <maya/MComponentDataIndexing.h>
#include <unordered_map>
#include <vector>
#include <map>

#if defined(_MSC_VER)
#if (_MSC_VER < 1600)
typedef unsigned __int64 uint64_t;
// Other compilers
#else
#include <stdint.h>
#endif
#endif


namespace
{
	struct HalfEdge;
	typedef std::vector<HalfEdge> ThalfEdgeArray;
	typedef std::unordered_map<uint64_t, size_t> TedgeAdjacencyIndex;

	struct HalfEdge
	{
		uint64_t Vert;  // Vertex index at the end of this half-edge
		size_t Twin; // Oppositely oriented adjacent half-edge
		size_t Next; // Next half-edge around the face

		HalfEdge() : Vert(-1), Twin(-1), Next(-1) {};
		bool hasTwin() const { return Twin != -1; };
		size_t twinVert(const ThalfEdgeArray& edgeArray) const {
			size_t next = edgeArray[Twin].Next;
			return edgeArray[next].Vert;
		}
	};

	struct VertexF
	{
		static const float kTolerance;
		VertexF(const float* buffer, unsigned int index)
		{
			unsigned int bufferPos = index * 3;
			x = buffer[bufferPos++];
			y = buffer[bufferPos++];
			z = buffer[bufferPos++];
		}

		bool isEqual(const VertexF &rhs) const
		{
			return (fabs(x - rhs.x) < kTolerance && fabs(y - rhs.y) < kTolerance && fabs(z - rhs.z) < kTolerance);
		}

		float x, y, z;
	};

	const float VertexF::kTolerance = 1e-5f;

	bool operator< (const VertexF& lhs, const VertexF& rhs)
	{
		return ((lhs.x - rhs.x) < -VertexF::kTolerance) || 
			   (fabs(lhs.x - rhs.x) < VertexF::kTolerance && (lhs.y - rhs.y) < -VertexF::kTolerance) || 
			   (fabs(lhs.x - rhs.x) < VertexF::kTolerance && fabs(lhs.y - rhs.y) < VertexF::kTolerance && (lhs.z - rhs.z) < -VertexF::kTolerance);
	}

	struct VertexFMap
	{
		unsigned int getVertexId( const VertexF& v );

		typedef std::map<VertexF, unsigned int> TVtxMap;
		TVtxMap vertexMap;
		void clear() { TVtxMap empty; vertexMap.swap(empty); }
	};

	unsigned int VertexFMap::getVertexId( const VertexF& v )
	{
		VertexFMap::TVtxMap::const_iterator itVtx = vertexMap.find(v);
		if (itVtx != vertexMap.end())
			return itVtx->second;
		unsigned int nextId = (unsigned int)vertexMap.size();
		vertexMap.insert(TVtxMap::value_type(v,nextId));
		return nextId;
	}
}

AdjacentTrianglesPrimitiveGenerator::AdjacentTrianglesPrimitiveGenerator()
{
}

AdjacentTrianglesPrimitiveGenerator::~AdjacentTrianglesPrimitiveGenerator() {}

void AdjacentTrianglesPrimitiveGenerator::mutateIndexBuffer( const MUintArray& originalBufferIndices, 
						const float* positionBufferFloat, 
						MHWRender::MGeometry::DataType indexBufferDataType,
						void* indexData )
{
	unsigned int numTriVerts = originalBufferIndices.length();

	ThalfEdgeArray edgeArray;
	edgeArray.resize(numTriVerts);

	TedgeAdjacencyIndex adjacencyIndex;

	// Iterate all triangles found in the old index buffer:
	unsigned int vertexIndex = 0;
	unsigned int edgeIndex = 0;

	VertexFMap vertexMap;

	while (vertexIndex < numTriVerts)
	{
		unsigned int A = originalBufferIndices[vertexIndex++];
		unsigned int vA = vertexMap.getVertexId(VertexF(positionBufferFloat, A));
		unsigned int B = originalBufferIndices[vertexIndex++];
		unsigned int vB = vertexMap.getVertexId(VertexF(positionBufferFloat, B));
		unsigned int C = originalBufferIndices[vertexIndex++];
		unsigned int vC = vertexMap.getVertexId(VertexF(positionBufferFloat, C));

		// Create the half-edge that goes from C to A:
		adjacencyIndex.insert(TedgeAdjacencyIndex::value_type(vC | (uint64_t(vA) << 32), edgeIndex));
		edgeArray[edgeIndex].Vert = A;
		edgeArray[edgeIndex].Next = 1 + edgeIndex;
		++edgeIndex;

		// Create the half-edge that goes from A to B:
		adjacencyIndex.insert(TedgeAdjacencyIndex::value_type(vA | (uint64_t(vB) << 32), edgeIndex));
		edgeArray[edgeIndex].Vert = B;
		edgeArray[edgeIndex].Next = 1 + edgeIndex;
		++edgeIndex;

		// Create the half-edge that goes from B to C:
		adjacencyIndex.insert(TedgeAdjacencyIndex::value_type(vB | (uint64_t(vC) << 32), edgeIndex));
		edgeArray[edgeIndex].Vert = C;
		edgeArray[edgeIndex].Next = edgeIndex - 2;
		++edgeIndex;
	}
	// Done with vertexMap:
	vertexMap.clear();

	if (adjacencyIndex.size() != numTriVerts)
		// Bad mesh: duplicated edges or inconsistent winding.
		return;

	// Populate the twin pointers by iterating over the index:
	int boundaryCount = 0;
	for (TedgeAdjacencyIndex::const_iterator it = adjacencyIndex.begin(); it != adjacencyIndex.end(); ++it)
	{
		uint64_t edgeKey = it->first;
		size_t edgeIndex = it->second; 
		uint64_t twinKey = ((edgeKey & 0xffffffff) << 32) | (edgeKey >> 32);
		TedgeAdjacencyIndex::const_iterator twinIt = adjacencyIndex.find(twinKey);
		if (twinIt != adjacencyIndex.end())
		{
			edgeArray[twinIt->second].Twin = edgeIndex;
			edgeArray[edgeIndex].Twin = twinIt->second;
		}
		else
		{
			++boundaryCount;
		}
	}

	// Done with adjacencyIndex:
	TedgeAdjacencyIndex emptyIndex;
	adjacencyIndex.swap(emptyIndex);

	// Now that we have a half-edge structure, it's easy to create adjacency info:
	if (boundaryCount > 0)
	{
		size_t destOffset = 0;
		size_t edgeIndex = 0;
		for (size_t faceIndex = 0; faceIndex < numTriVerts/3; ++faceIndex, edgeIndex += 3, destOffset += 6)
		{
			if (indexBufferDataType == MHWRender::MGeometry::kUnsignedInt32) {
				unsigned int* pDest = ((unsigned int*)indexData) + destOffset;
				pDest[0] = (unsigned int)(edgeArray[edgeIndex + 2].Vert);
				pDest[1] = (unsigned int)(edgeArray[edgeIndex + 0].hasTwin() ? edgeArray[edgeIndex + 0].twinVert(edgeArray) : pDest[0]);
				pDest[2] = (unsigned int)(edgeArray[edgeIndex + 0].Vert);
				pDest[3] = (unsigned int)(edgeArray[edgeIndex + 1].hasTwin() ? edgeArray[edgeIndex + 1].twinVert(edgeArray) : pDest[2]);
				pDest[4] = (unsigned int)(edgeArray[edgeIndex + 1].Vert);
				pDest[5] = (unsigned int)(edgeArray[edgeIndex + 2].hasTwin() ? edgeArray[edgeIndex + 2].twinVert(edgeArray) : pDest[4]);
			}
			else if (indexBufferDataType == MHWRender::MGeometry::kUnsignedInt16) {
				unsigned short* pDest = ((unsigned short*)indexData) + destOffset;
				pDest[0] = (unsigned short)(edgeArray[edgeIndex + 2].Vert);
				pDest[1] = (unsigned short)(edgeArray[edgeIndex + 0].hasTwin() ? edgeArray[edgeIndex + 0].twinVert(edgeArray) : pDest[0]);
				pDest[2] = (unsigned short)(edgeArray[edgeIndex + 0].Vert);
				pDest[3] = (unsigned short)(edgeArray[edgeIndex + 1].hasTwin() ? edgeArray[edgeIndex + 1].twinVert(edgeArray) : pDest[2]);
				pDest[4] = (unsigned short)(edgeArray[edgeIndex + 1].Vert);
				pDest[5] = (unsigned short)(edgeArray[edgeIndex + 2].hasTwin() ? edgeArray[edgeIndex + 2].twinVert(edgeArray) : pDest[4]);
			}
			else if (indexBufferDataType == MHWRender::MGeometry::kUnsignedChar) {
				unsigned char* pDest = ((unsigned char*)indexData) + destOffset;
				pDest[0] = (unsigned char)(edgeArray[edgeIndex + 2].Vert);
				pDest[1] = (unsigned char)(edgeArray[edgeIndex + 0].hasTwin() ? edgeArray[edgeIndex + 0].twinVert(edgeArray) : pDest[0]);
				pDest[2] = (unsigned char)(edgeArray[edgeIndex + 0].Vert);
				pDest[3] = (unsigned char)(edgeArray[edgeIndex + 1].hasTwin() ? edgeArray[edgeIndex + 1].twinVert(edgeArray) : pDest[2]);
				pDest[4] = (unsigned char)(edgeArray[edgeIndex + 1].Vert);
				pDest[5] = (unsigned char)(edgeArray[edgeIndex + 2].hasTwin() ? edgeArray[edgeIndex + 2].twinVert(edgeArray) : pDest[4]);
			}
		}
	}
	else
	{
		size_t destOffset = 0;
		size_t edgeIndex = 0;
		for (size_t faceIndex = 0; faceIndex < numTriVerts/3; ++faceIndex, edgeIndex += 3, destOffset += 6)
		{
			if (indexBufferDataType == MHWRender::MGeometry::kUnsignedInt32) {
				unsigned int* pDest = ((unsigned int*)indexData) + destOffset;
				pDest[0] = (unsigned int)(edgeArray[edgeIndex + 2].Vert);
				pDest[1] = (unsigned int)(edgeArray[edgeIndex + 0].twinVert(edgeArray));
				pDest[2] = (unsigned int)(edgeArray[edgeIndex + 0].Vert);
				pDest[3] = (unsigned int)(edgeArray[edgeIndex + 1].twinVert(edgeArray));
				pDest[4] = (unsigned int)(edgeArray[edgeIndex + 1].Vert);
				pDest[5] = (unsigned int)(edgeArray[edgeIndex + 2].twinVert(edgeArray));
			}
			else if (indexBufferDataType == MHWRender::MGeometry::kUnsignedInt16) {
				unsigned short* pDest = ((unsigned short*)indexData) + destOffset;
				pDest[0] = (unsigned short)(edgeArray[edgeIndex + 2].Vert);
				pDest[1] = (unsigned short)(edgeArray[edgeIndex + 0].twinVert(edgeArray));
				pDest[2] = (unsigned short)(edgeArray[edgeIndex + 0].Vert);
				pDest[3] = (unsigned short)(edgeArray[edgeIndex + 1].twinVert(edgeArray));
				pDest[4] = (unsigned short)(edgeArray[edgeIndex + 1].Vert);
				pDest[5] = (unsigned short)(edgeArray[edgeIndex + 2].twinVert(edgeArray));
			}
			else if (indexBufferDataType == MHWRender::MGeometry::kUnsignedChar) {
				unsigned char* pDest = ((unsigned char*)indexData) + destOffset;
				pDest[0] = (unsigned char)(edgeArray[edgeIndex + 2].Vert);
				pDest[1] = (unsigned char)(edgeArray[edgeIndex + 0].twinVert(edgeArray));
				pDest[2] = (unsigned char)(edgeArray[edgeIndex + 0].Vert);
				pDest[3] = (unsigned char)(edgeArray[edgeIndex + 1].twinVert(edgeArray));
				pDest[4] = (unsigned char)(edgeArray[edgeIndex + 1].Vert);
				pDest[5] = (unsigned char)(edgeArray[edgeIndex + 2].twinVert(edgeArray));
			}
		}
	}
}


MHWRender::MGeometry::Primitive AdjacentTrianglesPrimitiveGenerator::mutateIndexing(const MHWRender::MComponentDataIndexingList& sourceIndexBuffers, 
		const MHWRender::MVertexBufferArray& vertexBuffers,
		MHWRender::MIndexBuffer& indexBuffer,
		int& primitiveStride) const
{
	MHWRender::MVertexBuffer *positionBuffer = NULL;
	
	for (unsigned int ivb = 0; ivb < vertexBuffers.count() && positionBuffer == NULL; ++ivb)
	{
		MHWRender::MVertexBuffer *currBuffer = vertexBuffers.getBuffer(ivb);

		if (positionBuffer == NULL && currBuffer->descriptor().semantic() == MHWRender::MGeometry::kPosition)
			positionBuffer = currBuffer;
	}

	if (positionBuffer == NULL)
		// We need at least the positions:
		return MHWRender::MGeometry::kInvalidPrimitive;

	float* positionBufferFloat = (float*)positionBuffer->map();

	for (int x = 0; x < sourceIndexBuffers.length(); ++x)
	{
		if (sourceIndexBuffers[x]->componentType() != MHWRender::MComponentDataIndexing::kFaceVertex)
			continue;

		const MUintArray& originalBufferIndices = sourceIndexBuffers[x]->indices();
		unsigned int numTriVerts = originalBufferIndices.length();

		unsigned int numTri = numTriVerts / 3;
		unsigned int triSize = 6;
		unsigned int bufferSize = numTri * triSize;

		void* indexData = indexBuffer.acquire(bufferSize, true /*writeOnly - we don't need the current buffer values*/);
		if (indexData != NULL)
		{
			mutateIndexBuffer( originalBufferIndices, positionBufferFloat,
							   indexBuffer.dataType(), indexData );
		}

		if (positionBuffer) positionBuffer->unmap();
		indexBuffer.commit(indexData);
		primitiveStride = triSize;
		return MHWRender::MGeometry::kAdjacentTriangles;
	}

	if (positionBuffer) positionBuffer->unmap();
	return MHWRender::MGeometry::kInvalidPrimitive;
}

// This is the primitive generator creation function registered with the DrawRegistry.
// Used to initialize a custom primitive generator.
MHWRender::MPxIndexBufferMutator* AdjacentTrianglesPrimitiveGenerator::createAdjacentTrianglesPrimitiveGenerator()
{
	return new AdjacentTrianglesPrimitiveGenerator();
}

