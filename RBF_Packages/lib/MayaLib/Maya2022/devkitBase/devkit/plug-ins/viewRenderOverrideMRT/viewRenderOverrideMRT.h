//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
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

class viewRenderOverrideMRT : public MHWRender::MRenderOverride
{
public:
	enum
	{
		kTargetClear,
		kMaya3dSceneRender,
		kUserOp,
		kTargetPreview,
		kPresentOp,
		kOperationCount
	};

	enum {
		kTempColor1 = 0,	// Intermediate target 1
		kTempColor2,		// Intermediate target 2
		kTargetCount
	};

	enum {
		kTargetClearShader,		// To clear targets to specific color
		kSceneAttributeShader,	// To render geometry attributes
		kTargetPreviewShader,	// To preview targets
		kShaderCount
	};

	viewRenderOverrideMRT(const MString& name);
	~viewRenderOverrideMRT() override;

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

protected:
	MStatus updateRenderOperations();
	MStatus updateRenderTargets(MHWRender::MRenderer *theRenderer,
								const MHWRender::MRenderTargetManager *targetManager);
	MStatus updateShaders(const MHWRender::MShaderManager* shaderMgr);

	MString mUIName;
	MColor mClearColor;

	MHWRender::MRenderOperation * mRenderOperations[kOperationCount];
	MString mRenderOperationNames[kOperationCount];
	bool mRenderOperationEnabled[kOperationCount];
	int mCurrentOperation;

	MString mTargetOverrideNames[kTargetCount];
	MHWRender::MRenderTargetDescription* mTargetDescriptions[kTargetCount];
	MHWRender::MRenderTarget* mTargets[kTargetCount];

	MHWRender::MShaderInstance * mShaderInstances[kShaderCount];
};

// Scene render to output to targets
class sceneRenderMRT : public MHWRender::MSceneRender
{
public:
    sceneRenderMRT(const MString &name);
    ~sceneRenderMRT() override;

	MHWRender::MRenderTarget* const* targetOverrideList(unsigned int &listSize) override;
	MHWRender::MClearOperation & clearOperation() override;
	const MHWRender::MShaderInstance* shaderOverride() override;
	MHWRender::MSceneRender::MSceneFilterOption renderFilterOverride() override;

	void setRenderTargets(MHWRender::MRenderTarget **targets);
	void setShader( MHWRender::MShaderInstance *shader)
	{
		mShaderInstance = shader;
	}
	void useViewportRect(bool val)
	{
		mUseViewportRect = val;
	}
	const MFloatPoint * viewportRectangleOverride() override;

protected:
	MHWRender::MRenderTarget **mTargets;
	MHWRender::MShaderInstance *mShaderInstance;
	MFloatPoint mViewRectangle;
	bool mUseViewportRect;
};

// Present operation to present to screen
class presentTargetMRT : public MHWRender::MPresentTarget
{
public:
	presentTargetMRT(const MString &name);
	~presentTargetMRT() override;

	MHWRender::MRenderTarget* const* targetOverrideList(unsigned int &listSize) override;

	void setRenderTargets(MHWRender::MRenderTarget **targets);
protected:
	MHWRender::MRenderTarget **mTargets;
};

// Quad render
class quadRenderMRT : public MHWRender::MQuadRender
{
public:
	quadRenderMRT(const MString &name);
	~quadRenderMRT() override;

	const MHWRender::MShaderInstance * shader() override;
	MHWRender::MRenderTarget* const* targetOverrideList(unsigned int &listSize) override;
	MHWRender::MClearOperation & clearOperation() override;

	void setRenderTargets(MHWRender::MRenderTarget **targets, unsigned int count)
	{
		mOutputTargets = targets;
		mOutputTargetCount = count;
	}
	void setShader( MHWRender::MShaderInstance *shader)
	{
		mShaderInstance = shader;
	}
	void useViewportRect(bool val)
	{
		mUseViewportRect = val;
	}
	const MFloatPoint * viewportRectangleOverride() override;

protected:
	// Shader to use for the quad render
	MHWRender::MShaderInstance *mShaderInstance;
	// Output targets
	MHWRender::MRenderTarget** mOutputTargets;
	unsigned int mOutputTargetCount;
	MFloatPoint mViewRectangle;
	bool mUseViewportRect;
};

// User operation
class userOperationMRT : public MHWRender::MUserRenderOperation
{
public:
	userOperationMRT(const MString &name);
	~userOperationMRT() override;

	MStatus execute( const MHWRender::MDrawContext & drawContext ) override;
	MHWRender::MRenderTarget* const* targetOverrideList(unsigned int &listSize) override;

	void setRenderTargets(MHWRender::MRenderTarget **targets)
	{
		mTargets = targets;
	}

protected:
	// Output targets
	MHWRender::MRenderTarget** mTargets;
};



