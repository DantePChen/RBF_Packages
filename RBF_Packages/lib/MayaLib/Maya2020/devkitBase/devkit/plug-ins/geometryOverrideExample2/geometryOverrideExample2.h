//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#ifndef GeometryOverrideExample2_h

#define GeometryOverrideExample2_h

#include <maya/MPxGeometryOverride.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MDagMessage.h>
#include <vector>



class GeometryOverrideExample2_shape : public MPxSurfaceShape
{
public:
	struct Float2
	{
		Float2() {}
		Float2(float x, float y)
			: x(x), y(y) {}

		float x;
		float y;
	};

	struct Float3
	{
		Float3() {}
		Float3(float x, float y, float z) 
			: x(x), y(y), z(z) {}

		float x;
		float y;
		float z;
	};

	typedef std::vector<Float3>       Float3Array;
	typedef std::vector<Float2>       Float2Array;
	typedef std::vector<unsigned int> IndexList;

public:
	static  void *  creator()
	{
		return new GeometryOverrideExample2_shape();
	}

	static  MStatus  initialize() { return MS::kSuccess; }

	~GeometryOverrideExample2_shape() override;

	void            postConstructor() override;
	bool            isBounded() const override { return true; }
	MBoundingBox    boundingBox() const override;
	MSelectionMask	getShapeSelectionMask() const override;

	const Float3Array&  getPositions() const { return  fPositions; }
	const Float3Array&  getNormals() const { return  fNormals; }
	const Float3Array&  getTangents() const { return  fTangents; }
	const Float3Array&  getBiTangents() const { return  fBiTangents; }
	const Float2Array & getTexCoords() const { return  fTextureCoords; }
	const IndexList&    getShadedIndices() const { return  fShadedIndices; }
	const IndexList&    getWireFrameIndices() const { return  fWireFrameIndices; }

public:

	static	MTypeId		id;             // plugin node id

private:
	GeometryOverrideExample2_shape();

	Float3Array  fPositions;            // cube vertex positions
	Float3Array  fNormals;              // cube vertex normals
	Float3Array  fTangents;             // cube vertex tangents
	Float3Array  fBiTangents;           // cube vertex bitangents
	Float2Array  fTextureCoords;        // cube vertex texture UV
	IndexList    fShadedIndices;        // vertex indices used to draw the cube in filled mode
	IndexList    fWireFrameIndices;		// vertex indices used to draw the cube in wireframe mode

	MCallbackId mInstanceAddedCallbackId = 0;
};



class GeometryOverrideExample2 : public MHWRender::MPxGeometryOverride
{
public:

	static MHWRender::MPxGeometryOverride* Creator(const MObject& obj)
	{
		return new GeometryOverrideExample2(obj);
	}

	~GeometryOverrideExample2() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	void updateDG() override;

	void updateRenderItems(const MDagPath& path, MHWRender::MRenderItemList& list) override;

	void populateGeometry(const MHWRender::MGeometryRequirements& requirements,
		const MHWRender::MRenderItemList& renderItems,
		MHWRender::MGeometry& data) override;

	void cleanUp() override;

	bool requiresGeometryUpdate() const override;
	bool supportsEvaluationManagerParallelUpdate() const override;

private:
	GeometryOverrideExample2(const MObject& obj);

	void updateWireframeItems(const char* renderItemName, MGeometry::DrawMode mode, unsigned int depthPriority,
							  MColor color, bool isEnable, MHWRender::MRenderItemList& renderItemList,
							  const MHWRender::MShaderManager& shaderManager);

private:

	static const char* sActiveWireframeRenderItemName;
	static const char* sDormantWireframeRenderItemName;
	static const char* sShadedRenderItemName;

	GeometryOverrideExample2_shape* fMesh = nullptr;
};


#endif // GeometryOverrideExample2_h
