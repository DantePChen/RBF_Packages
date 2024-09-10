//-
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include <stdio.h>

#include <maya/MString.h>
#include <maya/MColor.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MShaderManager.h>

class viewRenderOverrideTargets : public MHWRender::MRenderOverride
{
public:
	enum
	{
		kMaya3dSceneRender,
		kTargetPreview,
		kPresentOp,
		kOperationCount
	};

	enum {
		kTargetPreviewShader = 0,	// To preview targets
		kShaderCount
	};

	viewRenderOverrideTargets(const MString& name);
	~viewRenderOverrideTargets() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;
	bool startOperationIterator() override;
	MHWRender::MRenderOperation * renderOperation() override;
	bool nextRenderOperation() override;
	MStatus setup(const MString& destination) override;
	MStatus cleanup() override;
	MString uiName() const override
	{
		return mUIName;
	}
	MHWRender::MRenderOperation * getOperation( unsigned int i)
	{
		if (i < kOperationCount)
			return mRenderOperations[i];
		return NULL;
	}

protected:
	MStatus updateRenderOperations();
	MStatus updateShaders(const MHWRender::MShaderManager* shaderMgr);

	MString mUIName;
	MColor mClearColor;

	MHWRender::MRenderOperation * mRenderOperations[kOperationCount];
	MString mRenderOperationNames[kOperationCount];
	bool mRenderOperationEnabled[kOperationCount];
	int mCurrentOperation;

	MHWRender::MShaderInstance * mShaderInstances[kShaderCount];
};

// Scene render to output to targets
class sceneRenderTargets : public MHWRender::MSceneRender
{
public:
    sceneRenderTargets(const MString &name, viewRenderOverrideTargets *override);
    ~sceneRenderTargets() override;

	MHWRender::MClearOperation & clearOperation() override;
	MHWRender::MSceneRender::MSceneFilterOption renderFilterOverride() override;

	void postSceneRender(const MHWRender::MDrawContext & context) override;
	MHWRender::MTexture *tempColourTarget() const { return mTempColourTarget; }
	MHWRender::MTexture *tempDepthTarget() const { return mTempDepthTarget; }
	void releaseTargets();

protected:
	MHWRender::MTexture *mTempColourTarget;
	MHWRender::MTexture *mTempDepthTarget;
	viewRenderOverrideTargets *mOverride;
};

// Target preview render
class quadRenderTargets : public MHWRender::MQuadRender
{
public:
	quadRenderTargets(const MString &name, viewRenderOverrideTargets *theOverride);
	~quadRenderTargets() override;

	const MHWRender::MShaderInstance * shader() override;
	MHWRender::MClearOperation & clearOperation() override;

	void setShader( MHWRender::MShaderInstance *shader)
	{
		mShaderInstance = shader;
	}

	void updateTargets( MHWRender::MTexture *colorTarget, MHWRender::MTexture *depthTarget );

protected:
	// Shader to use for the quad render
	MHWRender::MShaderInstance *mShaderInstance;

	viewRenderOverrideTargets *mOverride;
};

// Present operation to present to screen
class presentTargetTargets : public MHWRender::MPresentTarget
{
public:
	presentTargetTargets(const MString &name);
	~presentTargetTargets() override;
};





