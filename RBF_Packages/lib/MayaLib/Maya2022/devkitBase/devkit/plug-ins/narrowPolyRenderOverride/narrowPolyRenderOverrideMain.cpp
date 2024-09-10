//-
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include <stdio.h>

#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MViewport2Renderer.h>

#include "narrowPolyRenderOverride.h"

static narrowPolyRenderOverride *narrowPolyRenderOverrideInstance = NULL;

///////////////////////////////////////////////////
//
// Plug-in functions
//
///////////////////////////////////////////////////

/*!
	Register an override and associated command
*/
MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");

	// Create and register an override.
	if (!narrowPolyRenderOverrideInstance)
	{
		// my_narrowPolyRenderOverride is the unique identifier string
		// for this override
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer(false);
		if (renderer)
		{
			narrowPolyRenderOverrideInstance = new narrowPolyRenderOverride( "my_narrowPolyRenderOverride" );
			status = renderer->registerOverride(narrowPolyRenderOverrideInstance);
		}
	}

	if (!status)
	{
		status.perror("registerOverride");
	}

	return status;
}

/*
	When uninitializing the plugin, make sure to deregister the
	override and then delete the instance which is being kept here.

	Also remove the command used to set options on the override
*/
MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	// Degister and delete override
	//
	if (narrowPolyRenderOverrideInstance)
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			status = renderer->deregisterOverride(narrowPolyRenderOverrideInstance);
		}

		delete narrowPolyRenderOverrideInstance;
		narrowPolyRenderOverrideInstance = NULL;
	}

	if (!status)
	{
		status.perror("deregisterOverride");
	}

	return status;
}

