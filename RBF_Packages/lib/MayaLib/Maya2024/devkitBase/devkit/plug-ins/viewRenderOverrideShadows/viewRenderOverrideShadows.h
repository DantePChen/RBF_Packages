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
#include <maya/MSelectionList.h>

class viewRenderOverrideShadows : public MHWRender::MRenderOverride
{
public:
	enum
	{
		kShadowPrePass,
		kMaya3dSceneRender,
		kPresentOp,
		kOperationCount
	};

	viewRenderOverrideShadows(const MString& name);
	~viewRenderOverrideShadows() override;

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
	MStatus updateShaders(const MHWRender::MShaderManager* shaderMgr);
	MStatus updateLightList();

	MString mUIName;
	MColor mClearColor;

	MHWRender::MRenderOperation * mRenderOperations[kOperationCount];
	MString mRenderOperationNames[kOperationCount];
	bool mRenderOperationEnabled[kOperationCount];
	int mCurrentOperation;

	MHWRender::MShaderInstance * mLightShader;
	MSelectionList mLightList;
};

// Scene render to output to targets
class sceneRender : public MHWRender::MSceneRender
{
public:
    sceneRender(const MString &name);
    ~sceneRender() override;

	const MHWRender::MShaderInstance* shaderOverride() override;
	void preSceneRender(const MHWRender::MDrawContext & context) override;

	void setShader( MHWRender::MShaderInstance *shader)
	{
		mLightShader = shader;
	}
	void setLightList( MSelectionList *val )
	{
		mLightList = val;
	}

protected:
	MHWRender::MShaderInstance *mLightShader;
	MSelectionList *mLightList;
};

// Shadow prepass operation
class shadowPrepass : public MHWRender::MUserRenderOperation
{
public:
	shadowPrepass(const MString &name);
	~shadowPrepass() override;

	MStatus execute( const MHWRender::MDrawContext & drawContext ) override;
	bool requiresLightData() const override
	{
		return true;
	}
	void setLightList( MSelectionList *val )
	{
		mLightList = val;
	}

	MSelectionList *mLightList;
};



