//-
// Copyright 2019 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include <maya/MFnPlugin.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MApiVersion.h>

#include <memory>

static std::unique_ptr < MHWRender::MRenderOverride> gsRenderOverride;

//
// Class which filters what to render in a scene draw by
// returning the objects in a named set as the object set filter.
//
// Has the option of what to do as the clear operation.
// Usage can be to clear on first set draw, and not clear
// on subsequent draws.
//
class ObjectSetSceneRender : public MHWRender::MSceneRender
{
public:
	ObjectSetSceneRender( const MString& name, const MString setName, unsigned int clearMask ) 
		: MHWRender::MSceneRender( name )
		, mSetName( setName ) 
		, mClearMask( clearMask )
	{}

	// Return filtered list of items to draw
	const MSelectionList* objectSetOverride() override
	{
		// Get members of the specified object set.
		MSelectionList list; 
		list.add( mSetName );

		MObject obj; 
		list.getDependNode( 0, obj );

		MFnSet set( obj ); 
		if (!set.getMembers( mFilterSet, true ))
		{
			mFilterSet.clear();
		}

		return &mFilterSet;
	}

	// Return clear operation to perform
	MHWRender::MClearOperation & clearOperation() override
	{
		mClearOperation.setMask( mClearMask );
		return mClearOperation;
	}

protected:
	MSelectionList mFilterSet;
	MString mSetName;
	unsigned int mClearMask;

};

//
// Render override which draws 2 sets of objects in multiple "passes" (multiple scene renders)
// by using a filtered draw for each pass.
//
class viewObjectSetOverride : public MHWRender::MRenderOverride
{
public:
	viewObjectSetOverride( const MString& name ) 
		: MHWRender::MRenderOverride( name ) 
		, mUIName("Multi-pass filtered object-set renderer")
		, mOperation(0)
	{
		const MString render1Name("Render Set 1");
		const MString render2Name("Render Set 2");
		const MString set1Name("set1");
		const MString set2Name("set2");
		const MString presentName("Present Target");

		// Clear + render set 1
		mRenderSet1 = new ObjectSetSceneRender( render1Name, set1Name,  (unsigned int)MHWRender::MClearOperation::kClearAll );
		// Don't clear and render set 2
		mRenderSet2 = new ObjectSetSceneRender( render2Name, set2Name,  (unsigned int)MHWRender::MClearOperation::kClearNone );
		// Present results
		mPresentTarget = new MHWRender::MPresentTarget( presentName ); 
	}

	~viewObjectSetOverride() override
	{
		delete mRenderSet1; mRenderSet1 = NULL;
		delete mRenderSet2; mRenderSet2 = NULL;
		delete mPresentTarget; mPresentTarget = NULL;
	}

	MHWRender::DrawAPI supportedDrawAPIs() const override
	{
		// this plugin supports both GL and DX
		return (MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile | MHWRender::kDirectX11);
	}

	bool startOperationIterator() override
	{
		mOperation = 0; 
		return true;
	}

	MHWRender::MRenderOperation* renderOperation() override
	{
		switch( mOperation )
		{
		case 0 : return mRenderSet1;
		case 1 : return mRenderSet2;
		case 2 : return mPresentTarget;
		}
		return NULL;
	}

	bool nextRenderOperation() override
	{
		mOperation++; 
		return mOperation < 3 ? true : false;
	}

	// UI name to appear as renderer 
	MString uiName() const override
	{
		return mUIName;
	}

protected:
	ObjectSetSceneRender*		mRenderSet1;
	ObjectSetSceneRender*		mRenderSet2;
	MHWRender::MPresentTarget*	mPresentTarget;
	int							mOperation;
	MString						mUIName;
};


//
// Plugin registration
//
MStatus 
initializePlugin( MObject obj )
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer(); 
	MStatus status = MStatus::kFailure;
	if (renderer)
	{
		gsRenderOverride.reset(new viewObjectSetOverride("viewObjectSetOverride"));
		status = renderer->registerOverride(gsRenderOverride.get());
	}
    return status;
}

//
// Plugin deregistration
//
MStatus 
uninitializePlugin( MObject obj )
{
    MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer(); 
	MStatus status = MStatus::kFailure;
	if (renderer)
	{
		status = renderer->deregisterOverride(gsRenderOverride.get());
		gsRenderOverride = nullptr;
	}
    return status;
}
