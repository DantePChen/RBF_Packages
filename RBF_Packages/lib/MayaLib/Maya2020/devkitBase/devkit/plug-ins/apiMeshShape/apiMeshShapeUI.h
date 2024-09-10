//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// apiMeshShapeUI.h
//
// Encapsulates the UI portion of a user defined shape. All of the
// drawing and selection code goes here.
//
////////////////////////////////////////////////////////////////////////////////

#include <maya/MPxSurfaceShapeUI.h> 
#include "apiMeshShape.h" 

class apiMeshUI : public MPxSurfaceShapeUI
{
public:
	apiMeshUI();
	~apiMeshUI() override; 

	/////////////////////////////////////////////////////////////////////
	//
	// Overrides
	//
	/////////////////////////////////////////////////////////////////////

	// Puts draw request on the draw queue
	//
	void	getDrawRequests( const MDrawInfo & info,
									 bool objectAndActiveOnly,
									 MDrawRequestQueue & requests ) override;

	// Main draw routine. Gets called by maya with draw requests.
	//
	void	draw( const MDrawRequest & request,
						  M3dView & view ) const override;

	// Main draw routine for UV editor. This is called by maya when the 
	// shape is selected and the UV texture window is visible. 
	// 
	void	drawUV( M3dView &view, const MTextureEditorDrawInfo & ) const override; 
	bool	canDrawUV() const override; 

	// Main selection routine
	//
	bool	select( MSelectInfo &selectInfo,
							MSelectionList &selectionList,
							MPointArray &worldSpaceSelectPts ) const override;

	/////////////////////////////////////////////////////////////////////
	//
	// Helper routines
	//
	/////////////////////////////////////////////////////////////////////

	void	drawWireframe( const MDrawRequest & request, M3dView & view ) const;
	void	drawShaded( const MDrawRequest & request, M3dView & view ) const;
	void	drawVertices( const MDrawRequest & request, M3dView & view ) const;
	void	drawBoundingBox( const MDrawRequest & request, M3dView & view ) const;
	// for userInteraction example code
	//
	void	drawRedPointAtCenter( const MDrawRequest & request, M3dView & view ) const;

	bool 			selectVertices( MSelectInfo &selectInfo,
									MSelectionList &selectionList,
									MPointArray &worldSpaceSelectPts ) const;

	static  void *      creator();

private:
	void drawUVWireframe( apiMeshGeom *, M3dView &, 
						  const MTextureEditorDrawInfo &info ) const;
	void drawUVMapCoord( M3dView &, int uv, float u, float v, bool ) const;
	void drawUVMapCoordNum( apiMeshGeom *, M3dView &, 
							const MTextureEditorDrawInfo &info, bool ) const;
		
	// Draw Tokens
	//
	enum {
		kDrawVertices, // component token
		kDrawWireframe,
		kDrawWireframeOnShaded,
		kDrawSmoothShaded,
		kDrawFlatShaded,
		kDrawBoundingBox,
		kDrawRedPointAtCenter,  // for userInteraction example code
		kLastToken
	};
};
