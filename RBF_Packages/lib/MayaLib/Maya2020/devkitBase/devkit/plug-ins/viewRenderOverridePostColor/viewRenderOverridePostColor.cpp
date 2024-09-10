//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include "viewRenderOverridePostColor.h"
#include <maya/MShaderManager.h>

const MString ColorPostProcessOverride::kSwirlPassName = "ColorPostProcessOverride_Swirl";
const MString ColorPostProcessOverride::kFishEyePassName = "ColorPostProcessOverride_FishEye";
const MString ColorPostProcessOverride::kEdgeDetectPassName = "ColorPostProcessOverride_EdgeDetect";

/////////////////////////////////////////////////////////////////////////////////////////
//
// Description: The purpose of this plug-in is to show how a render override
// can be used to perform post scene render color operations.
//
// The basic idea is to render the scene into a custom render target T1.
//
// T1 is then used as input in on the first color (quad) operation. The output
// is routed to a second render target T2. The next color operation will take
// T2 as input and route to T1. If we have series of color operations then
// they would "ping-pong" (alternative) the source and destination targets as needed.
//
// Following the color operations a HUD and presentation operation will use the
// destination target used by the last color operation.
// 
/////////////////////////////////////////////////////////////////////////////////////////

//
// Constructor for override will set up operation lists.
// This override has fairly simple logic to render a scene, perform
// 2d post color operations, and then present the final results
//
ColorPostProcessOverride::ColorPostProcessOverride( const MString & name )
: MRenderOverride( name )
, mUIName("Color Post")
{
    MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
    if (!theRenderer)
        return;

    // Create a new set of operations as required
    MHWRender::MRenderer::theRenderer()->getStandardViewportOperations(mOperations);

    PostQuadRender* swirlOp = new PostQuadRender( kSwirlPassName, "FilterSwirl", "" );
    PostQuadRender* fishEyeOp = new PostQuadRender( kFishEyePassName, "FilterFishEye", "" );
    PostQuadRender* edgeDetectOp = new PostQuadRender( kEdgeDetectPassName, "FilterEdgeDetect", "" );

    swirlOp->setEnabled(false); // swirl is disabled by default

    mOperations.insertAfter(MHWRender::MRenderOperation::kStandardSceneName, swirlOp);
    mOperations.insertAfter(kSwirlPassName, fishEyeOp);
    mOperations.insertAfter(kFishEyePassName, edgeDetectOp);
}


// On destruction all operations are deleted.
//
ColorPostProcessOverride::~ColorPostProcessOverride()
{
}
	
// Drawing uses all internal code so will support all draw APIs
//
MHWRender::DrawAPI ColorPostProcessOverride::supportedDrawAPIs() const
{
	return MHWRender::kAllDevices;
}


// This method is just here as an example.  Simply calls the base class method.
//
MStatus ColorPostProcessOverride::setup( const MString & destination )
{
    return MRenderOverride::setup(destination);
}

// This method is just here as an example.  Simply calls the base class method.
//
MStatus ColorPostProcessOverride::cleanup()
{
	return MRenderOverride::cleanup();
}

//------------------------------------------------------------------------
// Custom quad operation (post color operation)
//
// Instances of this class are used to provide different
// shaders to be applied to a full screen quad.
//
PostQuadRender::PostQuadRender(const MString &name, const MString &id, const MString &technique)
	: MQuadRender( name )
	, mShaderInstance(NULL)
    , mEffectId(id)
    , mEffectIdTechnique(technique)
{
    // Declare the required input targets
    mInputTargetNames.clear();
    // these are the targets we draw into for the post pass
    // newly created offscreen targets.
    mInputTargetNames.append(kAuxiliaryTargetName);
    mInputTargetNames.append(kAuxiliaryDepthTargetName);
    // These are the targets the scene is rendered into.  
    // These are passed into the post pass shader for read.
    mInputTargetNames.append(kColorTargetName);
    mInputTargetNames.append(kDepthTargetName);

    // Declare the produced output targets
    // note these are in the reverse order of the inputs.
    // This is because we want to read from the previously written target
    // and write to a new auxiliary target.  The new target becomes the color target.
    mOutputTargetNames.clear();
    mOutputTargetNames.append(kColorTargetName);
    mOutputTargetNames.append(kDepthTargetName);
    mOutputTargetNames.append(kAuxiliaryTargetName);
    mOutputTargetNames.append(kAuxiliaryDepthTargetName);

}

PostQuadRender::~PostQuadRender()
{
	if (mShaderInstance)
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
			if (shaderMgr)
			{
				shaderMgr->releaseShader(mShaderInstance);
			}
		}
		mShaderInstance = NULL;
	}
}

/*
	Return the appropriate shader instance based on the what
	we want the quad operation to perform
*/
const MHWRender::MShaderInstance *
PostQuadRender::shader()
{
	// Create a new shader instance for this quad render instance
	//
	if (mShaderInstance == NULL)
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
			if (shaderMgr)
			{
				mShaderInstance = shaderMgr->getEffectsFileShader( mEffectId.asChar(), mEffectIdTechnique.asChar() );
			}
		}
	}

	if (mShaderInstance)
	{
		// Set the input texture parameter 'gInputTex' to use
		// a given color target
		MHWRender::MRenderTargetAssignment assignment;
		// Note that we have 2 targets with one being used as the output and
		// here the other as the input.
		assignment.target = getInputTarget(kColorTargetName);
		
		MStatus status = mShaderInstance->setParameter("gInputTex", assignment);
		if (status != MStatus::kSuccess)
		{
			printf("Could not set input render target / texture parameter on post 2d shader\n");
			return NULL;
		}
		const MString edgeDetect("FilterEdgeDetect");
		if (mEffectId == edgeDetect)
		{
			status = mShaderInstance->setParameter("gThickness", 0.5f );
			if (status != MStatus::kSuccess)
			{
				printf("Could not set thickness parameter on edge detect shader\n");
			}
			status = mShaderInstance->setParameter("gThreshold", 0.1f );
			if (status != MStatus::kSuccess)
			{
				printf("Could not set threshold parameter on edge detect shader\n");
			}
		}
	}

	return mShaderInstance;
}

bool PostQuadRender::getInputTargetDescription(const MString& name, MHWRender::MRenderTargetDescription& description) 
{ 
    // We do not provide a target description for the auxiliary target because 
    // we expect that target to be set as an output.
    // We copy the descriptions from the auxiliary targets so the msaa properties and size match.
    if (name == kColorTargetName)
    {
        MHWRender::MRenderTarget* outTarget = getInputTarget(kAuxiliaryTargetName);
        if (outTarget)
            outTarget->targetDescription(description);
        description.setName("_post_target_1");
        return true;
    }
    else if (name == kDepthTargetName)   
    {
        MHWRender::MRenderTarget* outTarget = getInputTarget(kAuxiliaryDepthTargetName);
        if (outTarget)
            outTarget->targetDescription(description);
        description.setName("_post_target_depth");
        return true;
    }

    return false;
}

int PostQuadRender::writableTargets(unsigned int& count)
{
    count = 2;
    return 0;
}

MHWRender::MClearOperation &
PostQuadRender::clearOperation()
{
	mClearOperation.setClearGradient( false );
	mClearOperation.setMask( (unsigned int) MHWRender::MClearOperation::kClearNone );
	return mClearOperation;
}