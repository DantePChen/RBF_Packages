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

#ifndef __CRACK_FREE_PRIMITIVE_GENERATOR_H__
#define __CRACK_FREE_PRIMITIVE_GENERATOR_H__

#include <maya/MPxIndexBufferMutator.h>
#include <maya/MGeometry.h>
#include <maya/MComponentDataIndexing.h>


class CrackFreePrimitiveGenerator : public MHWRender::MPxIndexBufferMutator
{
public:
    CrackFreePrimitiveGenerator(bool addAdjacentEdges, bool addDominantEdges, bool addDominantPosition);
    virtual ~CrackFreePrimitiveGenerator();

	virtual MHWRender::MGeometry::Primitive mutateIndexing(const MHWRender::MComponentDataIndexingList& sourceIndexBuffers, 
		const MHWRender::MVertexBufferArray& vertexBuffers,
		MHWRender::MIndexBuffer& mutatedBuffer,
		int& primitiveStride) const;

	// This will allow cleaning up the swatch for PNAEN geometries:
	static void mutateIndexBuffer( const MUintArray& originalBufferIndices, 
						const float* positionBufferFloat, 
						const float* uvBufferFloat, 
						bool bAddAdjacentEdges,
						bool bAddDominantEdges,
						bool bAddDominantPosition,
						MHWRender::MGeometry::DataType indexBufferDataType,
						void* indexData );

	static unsigned int computeTriangleSize( bool bAddAdjacentEdges,
							bool bAddDominantEdges,
							bool bAddDominantPosition);

	static MHWRender::MPxIndexBufferMutator* createCrackFreePrimitiveGenerator18();
	static MHWRender::MPxIndexBufferMutator* createCrackFreePrimitiveGenerator9();

private:
	bool fAddAdjacentEdges;
	bool fAddDominantEdges;
	bool fAddDominantPosition;
};

#endif // __CRACK_FREE_PRIMITIVE_GENERATOR_H__
