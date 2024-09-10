//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include <stdio.h>

#include <maya/MIOStream.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>

#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>

#include <maya/M3dView.h>
#include <maya/MPoint.h>
#include <maya/MImage.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MStateManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MDrawContext.h>

#include "viewRenderOverride.h"

/*
	Code for a custom user operation
*/

//------------------------------------------------------------------------
/*
	Custom user operation
*/
viewRenderUserOperation::viewRenderUserOperation(const MString &name)
: MUserRenderOperation( name )
, fDrawLabel(false)
, fUserCameraOverride(false)
, fDebugLightingInfo(false)
, fDebugDrawContext(false)
, fEnableSRGBWriteFlag(false)
{
	// 100 % of target size
	mViewRectangle[0] = 0.0f;
	mViewRectangle[1] = 0.0f;
	mViewRectangle[2] = 1.0f;
	mViewRectangle[3] = 1.0f;
}

viewRenderUserOperation::~viewRenderUserOperation()
{
}

MStatus viewRenderUserOperation::execute( const MHWRender::MDrawContext & drawContext )
{
	// Sample code to debug pass information
	static const bool debugPassInformation = false;
	if (debugPassInformation)
	{
		const MHWRender::MPassContext & passCtx = drawContext.getPassContext();
		const MString & passId = passCtx.passIdentifier();
		const MStringArray & passSem = passCtx.passSemantics();
		printf("viewRenderUserOperation: drawing in pass[%s], semantic[", passId.asChar());
		for (unsigned int i=0; i<passSem.length(); i++)
			printf(" %s", passSem[i].asChar());
		printf("\n");
	}

	// Example code to find the active override.
	// This is not necessary if the operations just keep a reference
	// to the override, but this demonstrates how this
	// contextual information can be extracted.
	//
	MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
	const MHWRender::MRenderOverride *overridePtr = NULL;
	if (theRenderer)
	{
		const MString & overrideName = theRenderer->activeRenderOverride();
		overridePtr = theRenderer->findRenderOverride( overrideName );
	}

	// Some sample code to debug lighting information in the MDrawContext
	//
	if (fDebugLightingInfo)
	{
		viewRenderOverrideUtilities::printDrawContextLightInfo( drawContext );
	}

	// Some sample code to debug other MDrawContext information
	//
	if (fDebugDrawContext)
	{
		MStatus status;
		MMatrix matrix = drawContext.getMatrix(MHWRender::MFrameContext::kWorldMtx, &status);
		double dest[4][4];
		status = matrix.get(dest);
		printf("World matrix is:\n");
		printf("\t%f, %f, %f, %f\n", dest[0][0], dest[0][1], dest[0][2], dest[0][3]);
		printf("\t%f, %f, %f, %f\n", dest[1][0], dest[1][1], dest[1][2], dest[1][3]);
		printf("\t%f, %f, %f, %f\n", dest[2][0], dest[2][1], dest[2][2], dest[2][3]);
		printf("\t%f, %f, %f, %f\n", dest[3][0], dest[3][1], dest[3][2], dest[3][3]);

		MDoubleArray viewDirection = drawContext.getTuple(MHWRender::MFrameContext::kViewDirection, &status);
		printf("Viewdirection is: %f, %f, %f\n", viewDirection[0], viewDirection[1], viewDirection[2]);

		MBoundingBox box = drawContext.getSceneBox(&status);
		printf("Screen box is:\n");
		printf("\twidth=%f, height=%f, depth=%f\n", box.width(), box.height(), box.depth());
		float center[4];
		box.center().get(center);
		printf("\tcenter=(%f, %f, %f, %f)\n", center[0], center[1], center[2], center[3]);


		int originX, originY, width, height;
		status = drawContext.getViewportDimensions(originX, originY, width, height);
		printf("Viewport dimension: center(%d, %d), width=%d, heigh=%d\n", originX, originY, width, height);
	}

	//  Draw some addition things for scene draw
	//
	M3dView mView;
	if (mPanelName.length() &&
		(M3dView::getM3dViewFromModelPanel(mPanelName, mView) == MStatus::kSuccess))
	{
		// Get the current viewport and scale it relative to that
		//
		int targetW, targetH;
		drawContext.getRenderTargetSize( targetW, targetH );

		if (fDrawLabel)
		{
			MString testString("Drawing with override: ");
			testString += overridePtr->name();
			MPoint pos(0.0,0.0,0.0);
			glColor3f( 1.0f, 1.0f, 1.0f );
			_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
			mView.drawText( testString, pos);
			_OPENMAYA_POP_WARNING
		}
	}

	return MStatus::kSuccess;
}

// Camera override
const MHWRender::MCameraOverride *
	viewRenderUserOperation::cameraOverride()
{
	if (fUserCameraOverride)
	{
		M3dView mView;
		if (mPanelName.length() &&
			(M3dView::getM3dViewFromModelPanel(mPanelName, mView) == MStatus::kSuccess))
		{
			mView.getCamera( mCameraOverride.mCameraPath );
			return &mCameraOverride;
		}
	}
	return NULL;
}

// Target override
MHWRender::MRenderTarget* const*
viewRenderUserOperation::targetOverrideList(unsigned int &listSize)
{
	if (mTargets)
	{
		listSize = 2;
		return &mTargets[kMyColorTarget];
	}
	return NULL;
}

// sRGB write enable
bool viewRenderUserOperation::enableSRGBWrite()
{
	return fEnableSRGBWriteFlag;
}

bool viewRenderUserOperation::hasUIDrawables() const
{
	return true;
}

void viewRenderUserOperation::addUIDrawables(
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext )
{
	drawManager.beginDrawable();

	drawManager.setColor( MColor( 0.95f, 0.5f, 0.1f ) );
	drawManager.text( MPoint( 0, 2, 0 ), MString("UI draw test in user operation") );
	drawManager.line( MPoint( 0, 0, 0), MPoint( 0, 2, 0 ) );
	drawManager.setColor( MColor( 1.0f, 1.0f, 1.0f ) );
	drawManager.sphere(  MPoint( 0, 2, 0 ), 0.8, false );
	drawManager.setColor( MColor( 0.95f, 0.5f, 0.1f, 0.4f ) );
	drawManager.sphere(  MPoint( 0, 2, 0 ), 0.8, true );
	drawManager.endDrawable();
}

