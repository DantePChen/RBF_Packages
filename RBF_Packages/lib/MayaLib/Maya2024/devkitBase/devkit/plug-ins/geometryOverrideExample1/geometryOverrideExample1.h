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

#ifndef GeometryOverrideExample1_h

#define GeometryOverrideExample1_h

#include <maya/MPxGeometryOverride.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MFloatVector.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MDagMessage.h>
#include <vector>



class GeometryOverrideExample1_shape : public MPxSurfaceShape
{
public:

	typedef std::vector<MFloatVector> PositionList;
	typedef std::vector<MFloatVector> NormalList;
	typedef std::vector<unsigned int> IndexList;

public:
	static  void *  creator()
	{
		return new GeometryOverrideExample1_shape();
	}

	static  MStatus  initialize(){ return MS::kSuccess; }

	~GeometryOverrideExample1_shape() override;

	void            postConstructor() override;
	bool            isBounded() const override { return true; }
	MBoundingBox    boundingBox() const override;
	MSelectionMask	getShapeSelectionMask() const override;

	const PositionList& getPositions() const { return  fPositions; }
	const NormalList&   getNormals() const { return  fNormals; }
	const IndexList&    getShadedIndices() const { return  fShadedIndices; }
	const IndexList&    getWireFrameIndices() const { return  fWireFrameIndices; }

public:

	static	MTypeId		id;             // plugin node id

private:
	GeometryOverrideExample1_shape();

	PositionList fPositions;            // cube vertex positions
	NormalList   fNormals;              // cube vertex normals
	IndexList    fShadedIndices;        // vertex indices used to draw the cube in filled mode
	IndexList    fWireFrameIndices;		// vertex indices used to draw the cube in wireframe mode

	MCallbackId mInstanceAddedCallbackId = 0;
};



class GeometryOverrideExample1 : public MHWRender::MPxGeometryOverride
{
public:

	static MHWRender::MPxGeometryOverride* Creator(const MObject& obj)
	{
		return new GeometryOverrideExample1(obj);
	}

	~GeometryOverrideExample1() override;

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
	GeometryOverrideExample1(const MObject& obj);

	void updateWireframeItems(const char* renderItemName, MGeometry::DrawMode mode, unsigned int depthPriority,
		                      MColor color, bool isEnable, MHWRender::MRenderItemList& renderItemList, 
		                      const MHWRender::MShaderManager& shaderManager);

private:

	static const char* sActiveWireframeRenderItemName;
	static const char* sDormantWireframeRenderItemName;
	static const char* sShadedRenderItemName;
	
	GeometryOverrideExample1_shape* fMesh = nullptr;
};


#endif // GeometryOverrideExample1_h
