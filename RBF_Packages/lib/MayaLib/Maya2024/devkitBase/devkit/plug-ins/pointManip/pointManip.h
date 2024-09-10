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

#include <maya/MPxManipulatorNode.h>
#include <maya/MTypeId.h>

class pointManip : public MPxManipulatorNode
{
public:
	pointManip();
	~pointManip() override;
	void postConstructor() override;

	static void* creator();
	static MStatus initialize();

	MStatus doPress( M3dView& view ) override;
	MStatus doDrag( M3dView& view ) override;
	MStatus doRelease( M3dView& view ) override;

	void preDrawUI( const M3dView &view ) override;
	void drawUI( MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext ) const override;

	static MTypeId id;

private:
	// GL component name for drawing and picking
	MGLuint fPointHandle;

	int		fPortWidth;
	int		fPortHeight;
	float*	fDepthMap;

	bool	fIsMouseDragging;
	short	fMousePositionX;
	short	fMousePositionY;
	MPoint	fWorldPosition;

	short	fMainColorIndex;
};
