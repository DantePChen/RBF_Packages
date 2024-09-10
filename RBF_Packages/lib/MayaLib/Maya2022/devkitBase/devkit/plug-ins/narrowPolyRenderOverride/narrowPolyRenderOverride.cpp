//-
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MUiMessage.h>
#include <maya/MPoint.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MDrawContext.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>

#include <memory>

#include "narrowPolyRenderOverride.h"

static const MString kPresentOpName = "__MyPresentTarget";
static const MString kSceneRenderOpName = "__MySimpleSceneRender";
static const MString kUserOpName = "__MyUserRenderOp";

//------------------------------------------------------------------------
/*
	Narrow Poly Render Override
*/

MHWRender::DrawAPI
narrowPolyRenderOverride::supportedDrawAPIs() const
{
	return ( MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile );
}

bool
narrowPolyRenderOverride::startOperationIterator()
{
	mCurrentOperation = 0;
	return true;
}

MHWRender::MRenderOperation *
narrowPolyRenderOverride::renderOperation()
{
	if (mCurrentOperation >= 0 && mCurrentOperation < kNumberOfOps)
	{
		while(!mRenderOperations[mCurrentOperation])
		{
			mCurrentOperation++;
			if (mCurrentOperation >= kNumberOfOps) return nullptr;
		}

		if (mRenderOperations[mCurrentOperation])
		{
			return mRenderOperations[mCurrentOperation].get();
		}
	}

	return nullptr;
}

bool
narrowPolyRenderOverride::nextRenderOperation()
{
	mCurrentOperation++;
	if (mCurrentOperation < kNumberOfOps)
	{
		return true;
	}
	return false;
}

MStatus
narrowPolyRenderOverride::setup( const MString & destination )
{
	if (mRenderOperations[kPresentOp] == NULL)
	{
		mRenderOperations[kSceneRenderOp] = MRenderOperationPtr(new MSceneRender(kSceneRenderOpName));		
		mRenderOperations[kUserOpNumber] = MRenderOperationPtr(new narrowPolyRenderUserOperation(kUserOpName));
		mRenderOperations[kPresentOp] = MRenderOperationPtr(new MPresentTarget(kPresentOpName));
	}

	mCurrentOperation = -1;

	return MStatus::kSuccess;
}

MStatus
narrowPolyRenderOverride::cleanup()
{
	// Reset current operation
	mCurrentOperation = -1;

	return MStatus::kSuccess;
}

//------------------------------------------------------------------------
/*
	Custom user operation
*/

void narrowPolyRenderUserOperation::addUIDrawables(
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext)
{

	// get the first item in the selection list
	MStatus status;
	MSelectionList selectionList;
	MGlobal::getActiveSelectionList(selectionList);
	MDagPath dagPath;
	status = selectionList.getDagPath(0, dagPath);
	if (status != MS::kSuccess) return;

	MItMeshPolygon itMeshPolygon(dagPath, MObject::kNullObj, &status);
	if (status != MS::kSuccess) return;

	MFloatPointArray positions;

	// Iterate over all faces on the mesh
	for (; !itMeshPolygon.isDone(); itMeshPolygon.next())
	{
		MPointArray points;
		MIntArray polyIndices;
		itMeshPolygon.getPoints(points, MSpace::kWorld, &status);
		if (status != MS::kSuccess) return;

		int length = points.length();

		// Only care about triangles
		if (length == 3)
		{
			status = itMeshPolygon.getVertices(polyIndices);
			if (status != MS::kSuccess)return;

			// Iterate over all points on the triangle face and if the angle
			// between any 2 sides are less than the tolerance, then draw the 
			// triangle with special color
			for (int i = 0; i < length; i++)
			{
				// get the points of the triangle
				MPoint p0 = points[i];
				MPoint p1 = points[(i + 1) % length];
				MPoint p2 = points[(i + 2) % length];

				// create the 2 adjacent vectors to the current point
				MVector v1(p1 - p0);
				MVector v2(p2 - p0);

				// get the angle between the two vectors in degrees
				double angle = v1.angle(v2) * 180 / 3.14159;

				// if angle less than tolerance then draw the triangle with special color
				if ((fabs(angle - kTolerance) < .0001) || angle < kTolerance)
				{
					positions.append(points[0]);
					positions.append(points[1]);
					positions.append(points[2]);
					break;
				}
			}
		}
	}

	if (positions.length() != 0)
	{
		drawManager.beginDrawable(MUIDrawManager::Selectability::kNonSelectable);
		drawManager.setColor(kColor);
		drawManager.mesh(MUIDrawManager::Primitive::kTriangles, positions);
		drawManager.endDrawable();
	}
}

