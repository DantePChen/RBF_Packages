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

#include <maya/MTypeId.h>
#include <maya/MTypes.h>
#include <maya/MPxManipulatorNode.h>
#include <maya/MFnPlugin.h>
#include <maya/MObjectHandle.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MDagModifier.h>

#include <maya/M3dView.h>
#include <maya/MGLFunctionTable.h>

#include "manipulatorMath.h"


//
// Custom manipulator class
//
class lineManip : public MPxManipulatorNode
{
public:
	lineManip();
	~lineManip() override;
	void postConstructor() override;

	// Viewport 2.0 manipulator draw overrides
	void preDrawUI( const M3dView &view ) override;
	void drawUI(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext ) const override;

	// Important virtuals
	MStatus	doPress( M3dView& view ) override;
	MStatus	doDrag( M3dView& view ) override;
	MStatus	doRelease( M3dView& view ) override;

	// Standard API required methods
	static void* creator();
	static MStatus initialize();

	// Utility method
	MStatus updateDragInformation();

	// Node id
	static MTypeId id;

private:
	// GL component name for drawing and picking
	MGLuint lineName;
	// Simple class for plane creation, intersection. Although
	// the manipulator is just a line we want it to move
	// within a plane
	planeMath plane;
	// Modified mouse position used for updating manipulator
	MPoint mousePointGlName;

	// Some data used for viewport 2.0 manip draw
	MPoint fLineStart;
	MPoint fLineEnd;
	short   fLineColorIndex;
	short   fSelectedLineColorIndex;
	bool fDrawManip;
};



