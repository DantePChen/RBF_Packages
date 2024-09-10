//-
// Copyright 2016 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include <stdio.h>

#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MViewport2Renderer.h>

#include "viewRenderOverridePointLightShadows.h"

static viewRenderOverridePointLightShadows* viewRenderOverridePointLightShadowsInstance = NULL;

MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		if (!viewRenderOverridePointLightShadowsInstance)
		{
			viewRenderOverridePointLightShadowsInstance = new viewRenderOverridePointLightShadows("my_viewRenderOverridePointLightShadows");
			renderer->registerOverride(viewRenderOverridePointLightShadowsInstance);
		}
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		if (viewRenderOverridePointLightShadowsInstance)
		{
			renderer->deregisterOverride(viewRenderOverridePointLightShadowsInstance);
			delete viewRenderOverridePointLightShadowsInstance;
		}
		viewRenderOverridePointLightShadowsInstance = NULL;
	}

	return status;
}
