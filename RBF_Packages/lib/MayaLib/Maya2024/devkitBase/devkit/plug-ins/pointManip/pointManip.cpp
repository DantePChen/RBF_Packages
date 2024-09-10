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

//
//	pointManip.cpp
//		Creates manipulator node pointManip
//		Creates command pointManipCmd
//
//  This example demonstrates how to use the MPxManipulatorNode	class along with
//  a command to create a user defined manipulator.  The manipulator created is
//  a simple point (tiny circle/sphere) which is an OpenGL pickable component.
//  As you move the pickable component, selected objects will be moved to the
//  first intersection position between the mouse ray and any opaque objects in
//  the scene.
//
//  A corresponding command is used to create and delete the manipulator node.
 
/*

// To show this example using MEL, run the following:
loadPlugin pointManip;
pointManipCmd -create;

// To delete the manipulator using MEL:
pointManipCmd -delete;

*/

#include "pointManip.h"

#include <maya/MMatrix.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MFnCamera.h>
#include <maya/MTemplateCommand.h>

MTypeId pointManip::id( 0x80035 );

pointManip::pointManip()
: fPortWidth(0)
, fPortHeight(0)
, fDepthMap(NULL)
, fIsMouseDragging(false)
, fMousePositionX(0)
, fMousePositionY(0)
, fMainColorIndex(0)
{
}

/*virtual*/
pointManip::~pointManip()
{
	if (fDepthMap != NULL)
	{
		delete[] fDepthMap;
		fDepthMap = NULL;
	}
}

/*virtual*/
void pointManip::postConstructor()
{
	// Get the starting value of the pickable items
	glFirstHandle(fPointHandle);
}

/*static*/
void* pointManip::creator()
{
	return new pointManip();
}

/*static*/
MStatus pointManip::initialize()
{
	return MS::kSuccess;
}

/*virtual*/
MStatus	pointManip::doPress( M3dView& view )
{
	fIsMouseDragging = true;
	return mousePosition(fMousePositionX, fMousePositionY);
}

/*virtual*/
MStatus	pointManip::doDrag( M3dView& view )
{
	return mousePosition(fMousePositionX, fMousePositionY);
}

/*virtual*/
MStatus pointManip::doRelease( M3dView& view )
{
	fIsMouseDragging = false;

	MStatus status = mousePosition(fMousePositionX, fMousePositionY);
	if (status != MS::kSuccess)
	{
		status.perror("Error fetching mouse position");
		return status;
	}

	int w = view.portWidth(&status);
	if (status != MS::kSuccess)
	{
		status.perror("M3dView::portWidth");
		return status;
	}

	int h = view.portHeight(&status);
	if (status != MS::kSuccess)
	{
		status.perror("M3dView::portHeight");
		return status;
	}

	if (fMousePositionX >= w || fMousePositionY >= h)
	{
		status = MS::kFailure;
		status.perror("Mouse out of the port rectangle");
		return status;
	}

	if (w != fPortWidth || h != fPortHeight)
	{
		fPortWidth = w;
		fPortHeight = h;

		if (fDepthMap != NULL)
		{
			delete[] fDepthMap;
			fDepthMap = NULL;
		}
	}

	if (fDepthMap == NULL)
	{
		fDepthMap = new float[fPortWidth * fPortHeight];
	}

	status = view.refresh(false, true);
	if (status != MS::kSuccess)
	{
		status.perror("Error refreshing the active view");
		return status;
	}

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	status = view.readDepthMap(0, 0, fPortWidth, fPortHeight,
		(unsigned char*)fDepthMap, M3dView::kDepth_Float);
	_OPENMAYA_POP_WARNING
	if (status != MS::kSuccess)
	{
		status.perror("Error reading depth map");
		return status;
	}

	double d = fDepthMap[fMousePositionY * fPortWidth + fMousePositionX];

	// Ortho camera has linear depth that maps [0,1] to [Zn,Zf] linearly.
	//
	// Persp camera has non-linear depth:
	//
	//       Zf * (Z - Zn)
	// d = -----------------
	//       Z * (Zf - Zn)
	//
	// To map the non-linear depth from [0,1] back to [Zn,Zf]:
	//
	//          Zf * Zn
	// Z = ----------------------
	//       Zf - d * (Zf - Zn)
	//
	// Then using linear mapping to get back to [0,1] gives the linear depth:
	//
	//   Z - Zn            d * Zn
	// ---------- = ----------------------
	//   Zf - Zn      Zf - d * (Zf - Zn)
	//
	MDagPath dpath;
	view.getCamera(dpath);
	MFnCamera camera(dpath);
	if (!camera.isOrtho())
	{
		double Zn = camera.nearClippingPlane();
		double Zf = camera.farClippingPlane();
		d *= Zn / (Zf - d * (Zf - Zn));
	}

	// Prevent selected objects from being moved afar in case there is no opaque
	// object intersected by the mouse ray.
	if (d < 0.99)
	{
		MPoint nearPw, farPw;
		view.viewToWorld(fMousePositionX, fMousePositionY, nearPw, farPw);
		fWorldPosition = nearPw + d * (farPw - nearPw);

		MSelectionList list;
		MGlobal::getActiveSelectionList(list);
		MVector translation(fWorldPosition);
		for (MItSelectionList iter(list); !iter.isDone(); iter.next())
		{
			MObject node;
			iter.getDependNode(node);

			MFnTransform xform(node, &status);
			if (status == MS::kSuccess)
			{
				xform.setTranslation(translation, MSpace::kTransform);
			}
		}
	}

	return MS::kSuccess;
}

/*virtual*/
void pointManip::preDrawUI(const M3dView &view)
{
	fMainColorIndex = mainColor();
}

/*virtual*/
void pointManip::drawUI(MHWRender::MUIDrawManager& drawManager,
						const MHWRender::MFrameContext& frameContext) const
{
	if (fIsMouseDragging)
	{
		MPoint p;
		p.x = fMousePositionX;
		p.y = fMousePositionY;

		drawManager.beginDrawable();
		drawManager.circle2d(p, 3, true);
		drawManager.endDrawable();
	}
	else
	{
		drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, fPointHandle);
		setHandleColor(drawManager, fPointHandle, fMainColorIndex);
		drawManager.sphere(fWorldPosition, 0.1, true);
		drawManager.endDrawable();

		drawManager.beginDrawable();
		drawManager.setColorIndex(fMainColorIndex);
		drawManager.text(fWorldPosition, MString("point manip"));
		drawManager.endDrawable();
	}
}


char cmdName[] = "pointManipCmd";
char nodeName[] = "pointManip";

class pointManipCmd : public MTemplateCreateNodeCommand<pointManipCmd, cmdName, nodeName>
{
public:
	pointManipCmd() {}
};

static pointManipCmd sPointManipCmd;

MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "2015", "Any");

	MStatus status = plugin.registerNode(nodeName,
										 pointManip::id,
										 pointManip::creator,
										 pointManip::initialize,
										 MPxNode::kManipulatorNode);
	if (!status) 
	{
		status.perror("registerNode");
		return status;
	}

	status = sPointManipCmd.registerCommand( obj );
	if (!status) 
	{
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin(obj);

	MStatus status = plugin.deregisterNode(pointManip::id);
	if (!status) 
	{
		status.perror("deregisterNode");
		return status;
	}

	status = sPointManipCmd.deregisterCommand(obj);
	if (!status) 
	{
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
