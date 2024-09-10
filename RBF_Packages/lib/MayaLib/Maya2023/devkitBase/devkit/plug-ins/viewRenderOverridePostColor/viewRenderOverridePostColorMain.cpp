#include <maya/MIOStream.h>
#include <cstdlib>

#include <maya/MFnPlugin.h>
#include "viewRenderOverridePostColor.h"
#include "viewRenderOverridePostColorCmd.h"

#undef ENABLE_TRACE_API_CALLS
//#define ENABLE_TRACE_API_CALLS 1
#ifdef ENABLE_TRACE_API_CALLS
#define TRACE_API_CALLS(x) cerr <<(x)<<"\n"
#else
#define TRACE_API_CALLS(x)
#endif

MStatus initializePlugin( MObject obj )
{
	TRACE_API_CALLS("initializePlugin");

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");
	MStatus  status = MStatus::kSuccess;

	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		// We register with a given name
		ColorPostProcessOverride *overridePtr = new ColorPostProcessOverride("ColorPostProcessOverride");
		if (overridePtr)
		{
			renderer->registerOverride(overridePtr);
		}

		plugin.registerCommand( commandName,
										  viewRenderOverridePostColorCmd::creator,
										  viewRenderOverridePostColorCmd::newSyntax);
	}



	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	TRACE_API_CALLS("uninitializePlugin");
	MStatus   status = MStatus::kSuccess;

	MFnPlugin plugin( obj );

	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		// Find override with the given name and deregister
		const MHWRender::MRenderOverride* overridePtr = renderer->findRenderOverride("ColorPostProcessOverride");
		if (overridePtr)
		{
			renderer->deregisterOverride( overridePtr );
			delete overridePtr;
		}
		plugin.deregisterCommand( commandName );
	}

	return status;
}
