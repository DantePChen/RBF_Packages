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
#ifndef blindDataMeshVertexBufferGenerator_h
#define blindDataMeshVertexBufferGenerator_h
#include <maya/MPxVertexBufferGenerator.h>



//////////////////////////////////////////////////////////////////////////////////////////////
// 
// Vertex buffer generator use to generate a 4-float color stream from a mesh blind data.
//
//////////////////////////////////////////////////////////////////////////////////////////////
class blindDataMeshVertexBufferGenerator : public MHWRender::MPxVertexBufferGenerator
{
public:
	blindDataMeshVertexBufferGenerator() {}
	~blindDataMeshVertexBufferGenerator()override {}

	bool getSourceIndexing(const MObject& object, MComponentDataIndexing& sourceIndexing) const override;

	bool getSourceStreams(const MObject& object, MStringArray &sourceStreams) const override;

	void createVertexStream(const MObject& object, MVertexBuffer& vertexBuffer, const MComponentDataIndexing& targetIndexing, const MComponentDataIndexing& sharedIndexing, const MVertexBufferArray& sourceStreams) const override;

	static MHWRender::MPxVertexBufferGenerator* Creator()
	{
		return new blindDataMeshVertexBufferGenerator();
	}
};



#endif // blindDataMeshVertexBufferGenerator_h
