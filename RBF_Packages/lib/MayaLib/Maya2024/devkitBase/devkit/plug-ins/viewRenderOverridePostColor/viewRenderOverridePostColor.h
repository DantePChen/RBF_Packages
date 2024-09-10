#ifndef ColorPostProcess_h_
#define ColorPostProcess_h_
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>

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

/* 
     Class to handle color post process override filters
*/
class ColorPostProcessOverride : public MHWRender::MRenderOverride
{
public:
    // operation names
    static const MString kSwirlPassName;
    static const MString kFishEyePassName;
    static const MString kEdgeDetectPassName;

	ColorPostProcessOverride( const MString & name );
	~ColorPostProcessOverride() override;
	MHWRender::DrawAPI supportedDrawAPIs() const override;

	// Basic setup and cleanup
	MStatus setup( const MString & destination ) override;
	MStatus cleanup() override;

	// UI name
	MString uiName() const override
	{
		return mUIName;
	}
	
protected:

	// UI name 
	MString mUIName;

	friend class viewRenderOverridePostColorCmd;
};

//
// 2d Post scene render quad operation
//
class PostQuadRender : public MHWRender::MQuadRender
{
public:
	PostQuadRender(const MString &name, const MString &id, const MString &technique);
	~PostQuadRender() override;

	const MHWRender::MShaderInstance * shader() override;
    MHWRender::MClearOperation & clearOperation() override;

    int writableTargets(unsigned int& count) override;
    bool getInputTargetDescription(const MString& name, MHWRender::MRenderTargetDescription& description) override;

protected:

	MHWRender::MShaderInstance *mShaderInstance;
	MString mEffectId;
	MString mEffectIdTechnique;
};

#endif
