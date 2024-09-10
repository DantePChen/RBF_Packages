#ifndef viewOverrideSimple_h_
#define viewOverrideSimple_h_
//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
#include <maya/MString.h>
#include <maya/MViewport2Renderer.h>

//
// Simple override class derived from MRenderOverride
//
class viewOverrideSimple : public MHWRender::MRenderOverride
{
public:
	viewOverrideSimple( const MString & name );
	~viewOverrideSimple() override;
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
	MHWRender::MRenderOperation* mOperations[3];
	MString mOperationNames[3];

	// Temporary of operation iteration
	int mCurrentOperation;
};

//
// Simple scene operation override to allow for clear color
// tracking.
//
class simpleViewRenderSceneRender : public MHWRender::MSceneRender
{
public:
    simpleViewRenderSceneRender(const MString &name);
    MHWRender::MClearOperation & clearOperation() override;
};

#endif
