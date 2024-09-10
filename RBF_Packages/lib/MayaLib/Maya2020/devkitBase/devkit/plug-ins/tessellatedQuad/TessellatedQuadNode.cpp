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


////////////////////////////////////////////////////////////////////////////////
//
// This plug-in demonstrates how to define custom selection effect via the
// MShaderInstance interface for render items.
//
// TessellatedQuadNode is a custom locator. It is drawn in Viewport 2.0 with
// a tessellation shader acquired via MShaderManager::getEffectsFileShader()
// and thus requires a companion selection shader that includes the same GPU
// tessellation algorithm in order to achieve WYSIWYG selection. Note that,
// for Viewport 2.0 to identify and create a shader instance that can be used
// for HW selection, the annotation should be declared for the corresponding
// effect technique.
//
//   string keyword = "mayaHWSelection";
//
// Camera-based selection must be turned on to activate HW selection so that
// the custom selection shader takes effect. To use the plugin, execute the
// MEL script:
//
//   loadPlugin tessellatedQuad;
//   createNode tessellatedQuad;
//
// Please refer to "Porting Selection from Viewport 1 to 2" at Maya Developer
// Help for more information.
//
// The plugin assumes that the devkit location follows "Setting up your build
// environment" at Maya Developer Help; otherwise, shaders/textures cannot be
// located. In this case create a mod (module description file) as below in a
// suitable modules folder (getenv "MAYA_MODULE_PATH") and restart Maya.
//
//   + devkit 1.0 <local devkit path>
//
////////////////////////////////////////////////////////////////////////////////


#include "TessellatedQuadNode.h"
#include "TessellatedQuadGeomOverride.h"

#include <maya/MDistance.h>
#include <maya/MDrawRegistry.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnPlugin.h>


const MString kPluginRegistrantId("tessellatedQuadRegistrant");

const MTypeId TessellatedQuadNode::kTypeId(0x8003A);
const MString TessellatedQuadNode::kTypeName( "tessellatedQuad" );
const MString TessellatedQuadNode::kDrawClassification( "drawdb/geometry/tessellatedQuad" );

void* TessellatedQuadNode::creator()
{
	return new TessellatedQuadNode();
}

MStatus TessellatedQuadNode::initialize()
{
	return MStatus::kSuccess;
}

TessellatedQuadNode::TessellatedQuadNode() 
{
}

TessellatedQuadNode::~TessellatedQuadNode() 
{
}

void TessellatedQuadNode::draw(
	M3dView &view,
	const MDagPath &dagPath,
	M3dView::DisplayStyle style,
	M3dView::DisplayStatus displayStatus)
{
	// get the renderer and gl function table to use for VP1 drawing:
	//
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	MHardwareRenderer *pRenderer = MHardwareRenderer::theRenderer();
	if (!pRenderer)
		return;
	MGLFunctionTable *pGLFT = pRenderer->glFunctionTable();
	if (!pGLFT)
		return;
	_OPENMAYA_POP_WARNING
	
	// determine shaded & active/dormant state
	const bool isShaded = (style == M3dView::DisplayStyle::kFlatShaded
							|| style == M3dView::DisplayStyle::kGouraudShaded
							|| view.selectMode());
	const bool isDormant = (displayStatus == M3dView::DisplayStatus::kDormant);

	// figure out the wireframe colour
	MFnDagNode dagNodeFn(dagPath);
	const MColor wireframeColor(isDormant ? dagNodeFn.dormantColor() : dagNodeFn.activeColor());

	// start GL drawing:
	pGLFT->glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_COLOR_BUFFER_BIT);

		pGLFT->glEnable(GL_LINE_SMOOTH);

		if (isShaded)
		{
			// set the face colour (always the same):
			static const MColor faceColor(0.5, 0.5, 0.5, 1.0);
			pGLFT->glColor4fv(&(faceColor.r));

			// draw the quad:
			pGLFT->glBegin(GL_QUADS);
				pGLFT->glVertex3f(1.0, 0.0, 1.0);
				pGLFT->glVertex3f(1.0, 0.0, -1.0);
				pGLFT->glVertex3f(-1.0, 0.0, -1.0);
				pGLFT->glVertex3f(-1.0, 0.0, 1.0);
			pGLFT->glEnd();
		}

		// draw the edge lines
		pGLFT->glColor4f(wireframeColor.r, wireframeColor.g, wireframeColor.b, wireframeColor.a);
		pGLFT->glBegin(GL_LINE_LOOP);
				pGLFT->glVertex3f(1.0, 0.0, 1.0);
				pGLFT->glVertex3f(1.0, 0.0, -1.0);
				pGLFT->glVertex3f(-1.0, 0.0, -1.0);
				pGLFT->glVertex3f(-1.0, 0.0, 1.0);
		pGLFT->glEnd();

	// finished drawing:
	pGLFT->glPopAttrib();
}

bool TessellatedQuadNode::isBounded() const
{
	return true;
}

MBoundingBox TessellatedQuadNode::boundingBox() const
{
	const float oneUnit = (float) MDistance::uiToInternal(1.0);

	MBoundingBox bb;
	bb.expand(MPoint(oneUnit, 5.0f * oneUnit, oneUnit));
	bb.expand(MPoint(-oneUnit, -5.0 * oneUnit, -oneUnit));
	return bb;
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, kPluginRegistrantId.asChar(), "2019", "Any");

	status = plugin.registerNode(TessellatedQuadNode::kTypeName,
		TessellatedQuadNode::kTypeId,
		TessellatedQuadNode::creator,
		TessellatedQuadNode::initialize,
		MPxNode::kLocatorNode,
		&TessellatedQuadNode::kDrawClassification);
	if (!status)
	{
		status.perror("registerNode " + TessellatedQuadNode::kTypeName);
		return status;
	}

	status = MHWRender::MDrawRegistry::registerGeometryOverrideCreator(
		TessellatedQuadNode::kDrawClassification,
		kPluginRegistrantId,
		TessellatedQuadGeomOverride::creator);
	if (!status)
	{
		status.perror("registerGeometryOverrideCreator " + TessellatedQuadNode::kDrawClassification);
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	status = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator(
		TessellatedQuadNode::kDrawClassification,
		kPluginRegistrantId);
	if (!status)
	{
		status.perror("deregisterGeometryOverrideCreator " + TessellatedQuadNode::kDrawClassification);
		return status;
	}

	status = plugin.deregisterNode(TessellatedQuadNode::kTypeId);
	if (!status) {
		status.perror("deregisterNode " + TessellatedQuadNode::kTypeName);
		return status;
	}

	return status;
}
