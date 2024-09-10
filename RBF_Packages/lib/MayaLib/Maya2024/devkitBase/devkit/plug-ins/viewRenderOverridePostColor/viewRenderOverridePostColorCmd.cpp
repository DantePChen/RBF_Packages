#include <maya/MSyntax.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MArgDatabase.h>
#include <maya/MGlobal.h>
#include <maya/M3dView.h>

#include "viewRenderOverridePostColorCmd.h"
#include "viewRenderOverridePostColor.h"

viewRenderOverridePostColorCmd::viewRenderOverridePostColorCmd(): fishEye(true), swirl(false), edgeDetect(true)
{
}

viewRenderOverridePostColorCmd::~viewRenderOverridePostColorCmd()
{
}

void* viewRenderOverridePostColorCmd::creator()
{
	return (void *)(new viewRenderOverridePostColorCmd);
}

MSyntax viewRenderOverridePostColorCmd::newSyntax()
{
	MSyntax syntax;
	syntax.addFlag(kSwirlFlag, kSwirlFlagLong, MSyntax::kBoolean);
	syntax.addFlag(kFishEyeFlag, kFishEyeFlagLong, MSyntax::kBoolean);
	syntax.addFlag(kEdgeDetectFlag, kEdgeDetectFlagLong, MSyntax::kBoolean);

	syntax.enableQuery(true);

	return syntax;
}


MStatus	viewRenderOverridePostColorCmd::doIt( const MArgList& args )
{
	MStatus status = MStatus::kFailure;

	MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
	{
		MGlobal::displayError( "VP2 renderer not initialized." );
		return status;
	}

	ColorPostProcessOverride* postColorOverride = NULL;
	postColorOverride =	(ColorPostProcessOverride*)renderer->findRenderOverride("ColorPostProcessOverride");

	if (postColorOverride == NULL)
	{
		MGlobal::displayError( "ColorPostProcessOverride is not registered." );
		return status;
	}

	MArgDatabase argData(syntax(), args, &status);
	if (!status) 
		return status;

	bool isQuery = argData.isQuery();

	if (argData.isFlagSet(kSwirlFlag))
	{
        int index = postColorOverride->mOperations.indexOf(ColorPostProcessOverride::kSwirlPassName);
        if(isQuery)
		{
			MPxCommand::setResult(postColorOverride->mOperations[index]->enabled());
		}
		else
		{
			argData.getFlagArgument(kSwirlFlag, 0, swirl);
			postColorOverride->mOperations[index]->setEnabled(swirl);
		}
	}
	if (argData.isFlagSet(kFishEyeFlag))
	{
        int index = postColorOverride->mOperations.indexOf(ColorPostProcessOverride::kFishEyePassName);
		if(isQuery)
			MPxCommand::setResult(postColorOverride->mOperations[index]->enabled());
		else
		{
			argData.getFlagArgument(kFishEyeFlag, 0, fishEye);
			postColorOverride->mOperations[index]->setEnabled(fishEye);
		}
	}
	if (argData.isFlagSet(kEdgeDetectFlag))
	{
        int index = postColorOverride->mOperations.indexOf(ColorPostProcessOverride::kEdgeDetectPassName);
		if(isQuery)
			MPxCommand::setResult(postColorOverride->mOperations[index]->enabled());
		else
		{
			argData.getFlagArgument(kEdgeDetectFlag, 0, edgeDetect);
            postColorOverride->mOperations[index]->setEnabled(edgeDetect);
		}
	}

	M3dView view = M3dView::active3dView(&status);
	if ( !status )
	{
		MGlobal::displayWarning( "Failed to find an active 3d view." );
		return status;
	}

	view.refresh(false, true);

	return MStatus::kSuccess;

}