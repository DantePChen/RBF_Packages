//-
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include <stdio.h>

#include <maya/MString.h>
#include <maya/MSelectionList.h>
#include <maya/M3dView.h>
#include <maya/MDrawContext.h>
#include <maya/MFrameContext.h>

#include <array>
#include <memory>

static const MString kPluginName = "Narrow Poly Renderer Override";
static const MColor kColor(1.0f, 0.5f, 0.5f, 0.5f);
static constexpr double kTolerance = 10;

using MRenderOperationPtr = std::unique_ptr<MHWRender::MRenderOperation>;

//------------------------------------------------------------------------
/*
	Narrow polygon render override.

	Draws narrow triangles (angle between two vertices less than threshold) with a user defined color.

	Is responsible for setting up the render loop operations and
	updating resources for each frame render as well as any
	rendering options.

*/

class narrowPolyRenderOverride : public MHWRender::MRenderOverride
{
public:

	enum 
	{
		kSceneRenderOp,
		kUserOpNumber,
		kPresentOp,
		kNumberOfOps
	};

	narrowPolyRenderOverride(const MString& name)
		: MRenderOverride(name)
		, mCurrentOperation(-1)
	{
	}

	~narrowPolyRenderOverride() override
	{
	}

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	// Virtual overrides to return operations in this override
	bool startOperationIterator() override;
	MHWRender::MRenderOperation* renderOperation() override;
	bool nextRenderOperation() override;

	// Override setup and cleanup for this override
	//
	MStatus setup(const MString& destination) override;
	MStatus cleanup() override;

	const MString& panelName() const { return kPluginName; }
	MString uiName() const override { return kPluginName; }


protected:	
	int mCurrentOperation;
	std::array <MRenderOperationPtr, kNumberOfOps> mRenderOperations;
};

//------------------------------------------------------------------------
/*
	Custom user operation.
*/
class narrowPolyRenderUserOperation : public MHWRender::MUserRenderOperation
{
public:	
	narrowPolyRenderUserOperation(const MString& name) : MUserRenderOperation(name) {}
	~narrowPolyRenderUserOperation() {}

	bool hasUIDrawables() const override { return true; }
	void addUIDrawables(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext) override;
	
	MStatus execute(const MDrawContext& drawContext) override { return MS::kSuccess; }

	const MString& panelName() const { return kPluginName; }
};