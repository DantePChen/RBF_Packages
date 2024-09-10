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
#include <maya/MFnPlugin.h>
#include <maya/MObjectHandle.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MDagModifier.h>
#include <maya/MVector.h>

#include <maya/M3dView.h>
#include <maya/MGLFunctionTable.h>

#include "manipulatorMath.h"

//
// Custom manipulator class
//
class squareScaleManipulator : public MPxManipulatorNode
{
public:
	squareScaleManipulator();
	~squareScaleManipulator() override;
	void postConstructor() override;

	// The important virtuals to implement
	void preDrawUI(const M3dView &view) override;
	void drawUI(MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext) const override;
	MStatus connectToDependNode(const MObject &node) override;

	MStatus	doPress(M3dView& view) override;
	MStatus	doDrag(M3dView& view) override;
	MStatus	doRelease(M3dView& view) override;

	// Utility methods
	MStatus updateDragInformation();

	// Standard API required methods
	static void* creator();
	static MStatus initialize();

	// Node id
	static MTypeId id;
	static MString classification;
	static MString registrantId;

	// Base points for manip
	static const MPoint topLeft;
	static const MPoint topRight;
	static const MPoint bottomLeft;
	static const MPoint bottomRight;

private:
	bool shouldDraw(const MDagPath& camera) const;

	// GL component names for drawing and picking
	MGLuint fActiveName, fTopName, fRightName, fBottomName, fLeftName;

	// Final draw points
	float fTopLeft[4], fTopRight[4], fBottomRight[4], fBottomLeft[4];

	// Simple class for plane creation, intersection
	planeMath fPlane;

	// Modified mouse position used for updating manipulator
	MPoint fMousePoint;

	bool fDrawManip;
};
