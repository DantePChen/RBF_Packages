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

#ifndef __TESSELLATED_QUAD_GEOM_OVERRIDE_H__
#define __TESSELLATED_QUAD_GEOM_OVERRIDE_H__

#include <maya/MPxGeometryOverride.h>
#include <maya/MHWGeometry.h>
#include <maya/MDagPath.h>

class TessellatedQuadGeomOverride : public MHWRender::MPxGeometryOverride
{
public:

	static MHWRender::MPxGeometryOverride *	creator(const MObject& obj)
	{
		return new TessellatedQuadGeomOverride(obj);
	}

	~TessellatedQuadGeomOverride() override {}

	MHWRender::DrawAPI supportedDrawAPIs() const override
	{
		return (MHWRender::DrawAPI)(MHWRender::kDirectX11|MHWRender::kOpenGLCoreProfile);
	}

	void updateDG() override {}

	void updateRenderItems(
		const MDagPath &path,
		MHWRender::MRenderItemList& list) override;

	void populateGeometry(
		const MHWRender::MGeometryRequirements &requirements,
		const MHWRender::MRenderItemList &renderItems,
		MHWRender::MGeometry &data) override;

	void cleanUp() override {}

private:

	TessellatedQuadGeomOverride(const MObject &obj) : MHWRender::MPxGeometryOverride(obj)
	{
	}

};

#endif // __TESSELLATED_QUAD_GEOM_OVERRIDE_H__
