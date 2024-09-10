#ifndef _gpuCacheDrawOverride_h_
#define _gpuCacheDrawOverride_h_

//-
//**************************************************************************/
// Copyright 2020 Autodesk, Inc. All rights reserved. 
//
// Use of this software is subject to the terms of the Autodesk 
// license agreement provided at the time of installation or download, 
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
//**************************************************************************/
//+

#include <maya/MPxDrawOverride.h>
#include <maya/MSelectionContext.h>


namespace GPUCache {

/*==============================================================================
 * CLASS gpuCacheDrawOverride
 *============================================================================*/

// Handles the drawing of the cached geometry in the viewport 2.0.

class DrawOverride : public MHWRender::MPxDrawOverride
{

public:

    /*----- static member functions -----*/

    // Used by the MDrawRegistry to create new instances of this
    // class.
	static MPxDrawOverride* creator(const MObject& obj);

    
private:

    /*----- classes -----*/

    // Data used by the draw call back.
    class UserData;
    
    /*----- static member functions -----*/

    // Invoked by the Viewport 2.0 when it is time to draw.
    static void drawCb(const MHWRender::MDrawContext& drawContext,
                       const MUserData* data);

	// draw code for rendering and selection.
	static void draw(const MHWRender::MDrawContext& drawContext,
					 const MHWRender::MSelectionInfo* selectInfo,
					 const MUserData* data);

    /*----- member functions -----*/

	DrawOverride(const MObject& obj);
	~DrawOverride() override;

    // Prohibited and not implemented.
	DrawOverride(const DrawOverride& obj);
	const DrawOverride& operator=(const DrawOverride& obj);

    // Overrides of MPxDrawOverride member funtions. 
	MHWRender::DrawAPI supportedDrawAPIs() const override;

    bool isBounded(
        const MDagPath& objPath,
        const MDagPath& cameraPath) const override;

	MBoundingBox boundingBox(
		const MDagPath& objPath,
		const MDagPath& cameraPath) const override;

	bool disableInternalBoundingBoxDraw() const override;

	MUserData* prepareForDraw(
		const MDagPath& objPath,
		const MDagPath& cameraPath,
		const MHWRender::MFrameContext& frameContext,
		MUserData* oldData) override;

	bool wantUserSelection() const override;

	bool userSelect(
		const MHWRender::MSelectionInfo& selectInfo,
		const MHWRender::MDrawContext& context,
		const MDagPath& objPath,
		const MUserData* data,
		MSelectionList& selectionList,
		MPointArray& worldSpaceHitPts) override;

};

}

#endif
