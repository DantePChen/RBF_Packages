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

#include <maya/MPxIndexBufferMutator.h>

class AdjacentTrianglesPrimitiveGenerator : public MHWRender::MPxIndexBufferMutator
{
public:
    AdjacentTrianglesPrimitiveGenerator();
    ~AdjacentTrianglesPrimitiveGenerator() override;

	MHWRender::MGeometry::Primitive mutateIndexing(const MHWRender::MComponentDataIndexingList& sourceIndexBuffers, 
		const MHWRender::MVertexBufferArray& vertexBuffers,
		MHWRender::MIndexBuffer& mutatedBuffer,
		int& primitiveStride) const override;

	// This will allow cleaning up the swatch for PNAEN geometries:
	static void mutateIndexBuffer( const MUintArray& originalBufferIndices, 
						const float* positionBufferFloat, 
						MHWRender::MGeometry::DataType indexBufferDataType,
						void* indexData );

	static MHWRender::MPxIndexBufferMutator* createAdjacentTrianglesPrimitiveGenerator();

private:
};
