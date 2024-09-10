//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
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
#include <maya/MPxManipContainer.h>
#include <maya/MFnPlugin.h>
#include <maya/MObjectHandle.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MDagModifier.h>
#include <maya/M3dView.h>

#include "manipulatorMath.h"

//
// Manipulator class
//
class lineManip : public MPxManipulatorNode
{
public:
	lineManip();
	~lineManip() override;
	void postConstructor() override;

	// Important virtuals to implement
	MStatus	doPress( M3dView& view ) override;
	MStatus	doDrag( M3dView& view ) override;
	MStatus	doRelease( M3dView& view ) override;

	void preDrawUI( const M3dView &view ) override;
	void drawUI(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext ) const override;

	// Standard required methods
	static void* creator();
	static MStatus initialize();

	// Utility method
	MStatus updateDragInformation();

	// Node id
	static MTypeId id;

	// Manipulator changes behaviour based on the setting
	// of these two booleans
	bool affectScale;
	bool affectTranslate;

private:
	bool fDrawManip;
	MPoint fLineStart, fLineEnd;
	// GL component name used for drawing and picking
	MGLuint lineName;
	// Simple plane math class
	planeMath plane;
	// Modified mouse position
	MPoint mousePointGlName;

private:
	void getManipLinePos(float* top, float* bottom);
};

//
// Manipulator container which will hold two lineManip nodes
//
class lineManipContainer : public MPxManipContainer
{
public:
	lineManipContainer();
	~lineManipContainer() override;

	// Important virtuals to implement
	MStatus createChildren() override;
	MStatus connectToDependNode(const MObject & node) override;

	// Standard required methods
	static void * creator();
	static MStatus initialize();

	// Node id
    static MTypeId id;
};



