//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include "viewOverrideTrackTexture.h"
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MObject.h>
#include <maya/MTextureManager.h>
// For override creation we return a UI name so that it shows up in as a
// renderer in the 3d viewport menus.
// 
viewOverrideTrackTexture::viewOverrideTrackTexture( const MString & name )
: MRenderOverride( name )
, mUIName("Track Texture VP2 Override")
, mCurrentOperation(-1)
{
	mOperations[0] = mOperations[1] = NULL;
	mOperationNames[0] = "viewOverrideTrackTexture_Blit";
	mOperationNames[1] = "viewOverrideTrackTexture_Present";
	mTexture = NULL;
}

// On destruction all operations are deleted.
//
viewOverrideTrackTexture::~viewOverrideTrackTexture()
{
	if (mTexture)
	{
		MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
		MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();
		textureManager->releaseTexture(mTexture);
	}

	for (unsigned int i=0; i<2; i++)
	{
		if (mOperations[i])
		{
			delete mOperations[i];
			mOperations[i] = NULL;
		}
	}
}
	
// Drawing uses all internal code so will support all draw APIs
//
MHWRender::DrawAPI viewOverrideTrackTexture::supportedDrawAPIs() const
{
	return MHWRender::kAllDevices;
}

// Basic iterator methods which returns a list of operations in order
// The operations are not executed at this time only queued for execution
//
// - startOperationIterator() : to start iterating
// - renderOperation() : will be called to return the current operation
// - nextRenderOperation() : when this returns false we've returned all operations
//
bool viewOverrideTrackTexture::startOperationIterator()
{
	mCurrentOperation = 0;
	return true;
}

MHWRender::MRenderOperation*
viewOverrideTrackTexture::renderOperation()
{
	if (mCurrentOperation >= 0 && mCurrentOperation < 2)
	{
		if (mOperations[mCurrentOperation])
		{
			return mOperations[mCurrentOperation];
		}
	}
	return NULL;
}

bool 
viewOverrideTrackTexture::nextRenderOperation()
{
	mCurrentOperation++;
	if (mCurrentOperation < 2)
	{
		return true;
	}
	return false;
}

//
// On setup we make sure that we have created the appropriate operations
// These will be returned via the iteration code above.
//
// Here we create:
//
//	- One quad operation to draw the texture for the first file node found
//   in the scene.
//	- One "stock" presentation operation to be able to see the results in the viewport
//
MStatus viewOverrideTrackTexture::setup( const MString & destination )
{
	MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
	if (!theRenderer)
		return MStatus::kFailure;

	MHWRender::MTextureManager* textureManager = theRenderer->getTextureManager();

	// Create a new set of operations as required
	if (!mOperations[0])
	{
		mOperations[0] = (MHWRender::MRenderOperation *) new TextureBlit( mOperationNames[0] );
		mOperations[1] = (MHWRender::MRenderOperation *) new MHWRender::MPresentTarget( mOperationNames[1] );
	}
	if (!mOperations[0] || 
		!mOperations[1])
	{
		return MStatus::kFailure;
	}

	TextureBlit* textureBlit = (TextureBlit* )mOperations[0];

	// Scan for the first file texture node in the scene 
	// to draw it's texture.
	//
	MItDependencyNodes iter(MFn::kFileTexture);
	for ( ; !iter.isDone(); iter.next())
	{
		MObject obj = iter.thisNode();
		MFnDependencyNode dependFn( obj );
		bool haveTexture = false;
		if (!mTexture)
		{
			// First time we acquire the texture
			mTexture = textureManager->acquireTexture(obj, false);
			printf("*** Acquired texture %s = %d\n", dependFn.name().asChar(),
				mTexture != NULL);
			if (mTexture)
				haveTexture = true;
		}
		else
		{
			// Afterwards we update the texture
			MStatus status = mTexture->update(obj);
			printf("*** Updated texture %s = %d\n", dependFn.name().asChar(), 
				status == MStatus::kSuccess);
			if (status == MStatus::kSuccess)
				haveTexture = true;
		}
		if (haveTexture)
		{
			if (textureBlit)
			{
				// Set the quad blit texture to show in the viewport.
				textureBlit->setColorTexture(mTexture);
			}
			else
			{
				// Sample code to dump the texture to disk.
				MString saveName("c:/temp/");
				static int sIndex = 1;
				saveName += (sIndex++);
				saveName += ".iff";
				textureManager->saveTexture(mTexture, saveName);
			}
			break;
		}
		else
		{
			textureBlit->setColorTexture(NULL);
		}
	}

	return MStatus::kSuccess;
}

// On cleanup we just return for returning the list of operations for
// the next render
//
MStatus viewOverrideTrackTexture::cleanup()
{
	mCurrentOperation = -1;
	return MStatus::kSuccess;
}
	
//
// Simple quad blit of a texture
//
TextureBlit::TextureBlit(const MString &name)
: MQuadRender( name )
, mShaderInstance(NULL)
, mColorTextureChanged(false)
{
	mColorTexture.texture = NULL;
}

TextureBlit::~TextureBlit()
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	// Release any shader used
	if (mShaderInstance)
	{
		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (shaderMgr)
		{
			shaderMgr->releaseShader(mShaderInstance);
		}
		mShaderInstance = NULL;
	}

	mColorTexture.texture = NULL;
}

const MHWRender::MShaderInstance * TextureBlit::shader()
{
	if (!mShaderInstance)
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		const MHWRender::MShaderManager* shaderMgr = renderer ? renderer->getShaderManager() : NULL;
		if (shaderMgr)
		{
			// Just need a simple color blit
			mShaderInstance = shaderMgr->getEffectsFileShader( "Copy", "" );
		}
	}

	MStatus status = MStatus::kFailure;
	if (mShaderInstance)
	{
		// If texture changed then bind new texture to the shader
		// 
		status = MStatus::kSuccess;
		if (mColorTextureChanged)
		{
			status = mShaderInstance->setParameter("gInputTex", mColorTexture);
			mColorTextureChanged = false;
		}
	}
	if (status != MStatus::kSuccess)
	{
		return NULL;
	}
	return mShaderInstance;
}

MHWRender::MClearOperation & TextureBlit::clearOperation()
{
	mClearOperation.setClearGradient( false );
	mClearOperation.setMask( (unsigned int) MHWRender::MClearOperation::kClearAll );
	return mClearOperation;
}
