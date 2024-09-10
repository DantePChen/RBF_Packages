#ifndef viewOverrideTrackTexture_h_
#define viewOverrideTrackTexture_h_
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>

//
// Simple override class derived from MRenderOverride
//
class viewOverrideTrackTexture : public MHWRender::MRenderOverride
{
public:
	viewOverrideTrackTexture( const MString & name );
	~viewOverrideTrackTexture() override;
	MHWRender::DrawAPI supportedDrawAPIs() const override;

	// Basic setup and cleanup
	MStatus setup( const MString & destination ) override;
	MStatus cleanup() override;

	// Operation iteration methods
	bool startOperationIterator() override;
	MHWRender::MRenderOperation * renderOperation() override;
	bool nextRenderOperation() override;

	// UI name
	MString uiName() const override
	{
		return mUIName;
	}
	
protected:
	// UI name 
	MString mUIName;

	// Operations and operation names
	MHWRender::MRenderOperation* mOperations[2];
	MString mOperationNames[2];

	// Temporary of operation iteration
	int mCurrentOperation;

	MHWRender::MTexture *mTexture;
};

//
// Image blit used to perform the 'scene render'
//
class TextureBlit : public MHWRender::MQuadRender
{
public:
	TextureBlit(const MString &name);
	~TextureBlit() override;

	const MHWRender::MShaderInstance * shader() override;
	MHWRender::MClearOperation & clearOperation() override;

	inline void setColorTexture(MHWRender::MTexture* texture)
	{
		mColorTexture.texture = texture;
		mColorTextureChanged = true;
	}

protected:
	// Shader to use for the quad render
	MHWRender::MShaderInstance *mShaderInstance;
	// Texture used for the quad render. Not owned by operation.
	MHWRender::MTextureAssignment mColorTexture;
	bool mColorTextureChanged;
};

#endif
