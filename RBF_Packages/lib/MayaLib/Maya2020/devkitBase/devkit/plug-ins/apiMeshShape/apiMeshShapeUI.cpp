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
// apiMeshShapeUI.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include "apiMeshShapeUI.h"

#include <maya/MMaterial.h>
#include <maya/MColor.h>
#include <maya/MDrawData.h>
#include <maya/MMatrix.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MObjectArray.h>
#include <maya/MDagPath.h>
// for user interaction performance example code. (search on 
// "userChangingViewContext")
#include <maya/MGlobal.h>
#include <maya/MDrawInfo.h>
#include <maya/MDrawRequest.h>
#include <maya/MDrawRequestQueue.h>
#include <maya/MSelectInfo.h>

#include <maya/MTextureEditorDrawInfo.h> 

// Object and component color defines
//
#define LEAD_COLOR				18	// green
#define ACTIVE_COLOR			15	// white
#define ACTIVE_AFFECTED_COLOR	8	// purple
#define DORMANT_COLOR			4	// blue
#define HILITE_COLOR			17	// pale blue
#define DORMANT_VERTEX_COLOR	8	// purple
#define ACTIVE_VERTEX_COLOR		16	// yellow

// Vertex point size
//
#define POINT_SIZE				2.0	
#define UV_POINT_SIZE			4.0

////////////////////////////////////////////////////////////////////////////////
//
// UI implementation
//
////////////////////////////////////////////////////////////////////////////////

apiMeshUI::apiMeshUI() {}
apiMeshUI::~apiMeshUI() {}

void* apiMeshUI::creator()
{
	return new apiMeshUI();
}

///////////////////////////////////////////////////////////////////////////////
//
// Overrides
//
///////////////////////////////////////////////////////////////////////////////

/* override */
void apiMeshUI::getDrawRequests( const MDrawInfo & info,
							 bool objectAndActiveOnly,
							 MDrawRequestQueue & queue )
//
// Description:
//
//     Add draw requests to the draw queue
//
// Arguments:
//
//     info                 - current drawing state
//     objectsAndActiveOnly - no components if true
//     queue                - queue of draw requests to add to
//
{
	// Get the data necessary to draw the shape
	//
	MDrawData data;
	apiMesh* meshNode = (apiMesh*)surfaceShape();
	apiMeshGeom * geom = meshNode->meshGeomToUse();
	if ( (NULL == geom) || (0 == geom->faceCount) ) {
		//cerr << "NO DrawRequest for apiMesh\n";
		return;
	}

	// This call creates a prototype draw request that we can fill
	// in and then add to the draw queue.
	//
	MDrawRequest request = info.getPrototype( *this );
_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	getDrawData( geom, data );
_OPENMAYA_DEPRECATION_POP_WARNING
	request.setDrawData( data );

	// Decode the draw info and determine what needs to be drawn
	//

	M3dView::DisplayStyle  appearance    = info.displayStyle();
	M3dView::DisplayStatus displayStatus = info.displayStatus();
	
	// Are we displaying meshes?
	if ( ! info.objectDisplayStatus( M3dView::kDisplayMeshes ) )
		return;

	switch ( appearance )
	{
		case M3dView::kWireFrame :
		{
			request.setToken( kDrawWireframe );

			M3dView::ColorTable activeColorTable = M3dView::kActiveColors;
			M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;

			switch ( displayStatus )
			{
				case M3dView::kLead :
					request.setColor( LEAD_COLOR, activeColorTable );
					break;
				case M3dView::kActive :
					request.setColor( ACTIVE_COLOR, activeColorTable );
					break;
				case M3dView::kActiveAffected :
					request.setColor( ACTIVE_AFFECTED_COLOR, activeColorTable );
					break;
				case M3dView::kDormant :
					request.setColor( DORMANT_COLOR, dormantColorTable );
					break;
				case M3dView::kHilite :
					request.setColor( HILITE_COLOR, activeColorTable );
					break;
				default:	
					break;
			}

			queue.add( request );

			break;
		}

		case M3dView::kGouraudShaded :
		{
			// Create the smooth shaded draw request
			//
			request.setToken( kDrawSmoothShaded );

			// Need to get the material info
			//
			MDagPath path = info.multiPath();	// path to your dag object 
			M3dView view = info.view();; 		// view to draw to
			MMaterial material = MPxSurfaceShapeUI::material( path );

			// If the user currently has the default material enabled on the 
			// view then use the default material for shading. 
			// 
			if ( view.usingDefaultMaterial() ) { 
				material = MMaterial::defaultMaterial(); 
			}
			
			// Evaluate the material and if necessary, the texture.
			//
			if ( ! material.evaluateMaterial( view, path ) ) {
				cerr << "Couldnt evaluate\n";
			}
			
			bool drawTexture = true;
			if ( drawTexture && material.materialIsTextured() ) {
				material.evaluateTexture( data );
			}
			
			request.setMaterial( material );
			
			// request.setDisplayStyle( appearance );
			
			bool materialTransparent = false;
			material.getHasTransparency( materialTransparent );
			if ( materialTransparent ) {
				request.setIsTransparent( true );
			}

			queue.add( request );

			// create a draw request for wireframe on shaded if
			// necessary.
			//
			if ( (displayStatus == M3dView::kActive) ||
				 (displayStatus == M3dView::kLead) ||
				 (displayStatus == M3dView::kHilite) )
			{
				MDrawRequest wireRequest = info.getPrototype( *this );
				wireRequest.setDrawData( data );
				wireRequest.setToken( kDrawWireframeOnShaded );
				wireRequest.setDisplayStyle( M3dView::kWireFrame );

				M3dView::ColorTable activeColorTable = M3dView::kActiveColors;

				switch ( displayStatus )
				{
					case M3dView::kLead :
						wireRequest.setColor( LEAD_COLOR, activeColorTable );
						break;
					case M3dView::kActive :
						wireRequest.setColor( ACTIVE_COLOR, activeColorTable );
						break;
					case M3dView::kHilite :
						wireRequest.setColor( HILITE_COLOR, activeColorTable );
						break;
					default:	
						break;

				}

				queue.add( wireRequest );
			}

			break;
		}

		case M3dView::kFlatShaded :
			request.setToken( kDrawFlatShaded );
			queue.add( request );
			break;
		case M3dView::kBoundingBox :
			request.setToken( kDrawBoundingBox );
			queue.add( request );
			break;
		default:	
			break;
	}

	// Add draw requests for components
	//
	if ( !objectAndActiveOnly ) {

		// Inactive components
		//
		if ( (appearance == M3dView::kPoints) ||
			 (displayStatus == M3dView::kHilite) )
		{
			MDrawRequest vertexRequest = info.getPrototype( *this ); 
			vertexRequest.setDrawData( data );
			vertexRequest.setToken( kDrawVertices );
			vertexRequest.setColor( DORMANT_VERTEX_COLOR,
									M3dView::kActiveColors );

			queue.add( vertexRequest );
		}

		// Active components
		//
		if ( surfaceShape()->hasActiveComponents() ) {

			MDrawRequest activeVertexRequest = info.getPrototype( *this ); 
			activeVertexRequest.setDrawData( data );
		    activeVertexRequest.setToken( kDrawVertices );
		    activeVertexRequest.setColor( ACTIVE_VERTEX_COLOR,
										  M3dView::kActiveColors );

		    MObjectArray clist = surfaceShape()->activeComponents();
		    MObject vertexComponent = clist[0]; // Should filter list
		    activeVertexRequest.setComponent( vertexComponent );

			queue.add( activeVertexRequest );
		}
	}
}

/* override */
void apiMeshUI::draw( const MDrawRequest & request, M3dView & view ) const
//
// Description:
//
//     Main (OpenGL) draw routine
//
// Arguments:
//
//     request - request to be drawn
//     view    - view to draw into
//
{ 
	// Get the token from the draw request.
	// The token specifies what needs to be drawn.
	//
	int token = request.token();

	switch( token )
	{
		case kDrawWireframe :
		case kDrawWireframeOnShaded :
			drawWireframe( request, view );
			break;

		case kDrawSmoothShaded :
			drawShaded( request, view );
			break;

		case kDrawFlatShaded : // Not implemented
			break;

		case kDrawVertices :
			drawVertices( request, view );
			break;
			
		case kDrawBoundingBox:
			drawBoundingBox( request, view );
			break;
		// for userChangingViewContext example code 
		//
		case kDrawRedPointAtCenter:
			drawRedPointAtCenter( request, view );
			break;
	}
}

/* override */
bool apiMeshUI::select( MSelectInfo &selectInfo, MSelectionList &selectionList,
					MPointArray &worldSpaceSelectPts ) const
//
// Description:
//
//     Main selection routine
//
// Arguments:
//
//     selectInfo           - the selection state information
//     selectionList        - the list of selected items to add to
//     worldSpaceSelectPts  -
//
{
	bool selected = false;
	bool componentSelected = false;
	bool hilited = false;

	hilited = (selectInfo.displayStatus() == M3dView::kHilite);
	if ( hilited ) {
		componentSelected = selectVertices( selectInfo, selectionList,
											worldSpaceSelectPts );
		selected = selected || componentSelected;
	}

	if ( !selected ) {

		apiMesh* meshNode = (apiMesh*)surfaceShape();

		// NOTE: If the geometry has an intersect routine it should
		// be called here with the selection ray to determine if the
		// the object was selected.

		selected = true;
		MSelectionMask priorityMask( MSelectionMask::kSelectNurbsSurfaces );
		MSelectionList item;
		item.add( selectInfo.selectPath() );
		MPoint xformedPt;
		if ( selectInfo.singleSelection() ) {
			MPoint center = meshNode->boundingBox().center();
			xformedPt = center;
			xformedPt *= selectInfo.selectPath().inclusiveMatrix();
		}

		selectInfo.addSelection( item, xformedPt, selectionList,
								 worldSpaceSelectPts, priorityMask, false );
	}

	return selected;
}

///////////////////////////////////////////////////////////////////////////////
//
// Helper routines
//
///////////////////////////////////////////////////////////////////////////////

void apiMeshUI::drawWireframe( const MDrawRequest & request,
							   M3dView & view ) const
//
// Description:
//
//     Wireframe drawing routine
//
// Arguments:
//
//     request - request to be drawn
//     view    - view to draw into
//
{
	MDrawData data = request.drawData();
	apiMeshGeom * geom = (apiMeshGeom*)data.geometry();

	int token = request.token();

	bool wireFrameOnShaded = false;
	if ( kDrawWireframeOnShaded == token ) {
		wireFrameOnShaded = true;
	}

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL(); 
	_OPENMAYA_POP_WARNING

	// Query current state so it can be restored
	//
	bool lightingWasOn = glIsEnabled( GL_LIGHTING ) ? true : false;
	if ( lightingWasOn ) {
		glDisable( GL_LIGHTING );
	}

	if ( wireFrameOnShaded ) {
		glDepthMask( false );
	}

	// Draw the wireframe mesh
	//
	int vid = 0;			
	for ( int i=0; i<geom->faceCount; i++ )
	{
		glBegin( GL_LINE_LOOP );
		for ( int v=0; v<geom->face_counts[i]; v++ )
		{
			MPoint vertex = geom->vertices[ geom->face_connects[vid++] ];
			glVertex3f( (float)vertex[0], (float)vertex[1], (float)vertex[2] );
		}
		glEnd();
	}

	// Restore the state
	//
	if ( lightingWasOn ) {
		glEnable( GL_LIGHTING );
	}
	if ( wireFrameOnShaded ) {
		glDepthMask( true );
	}

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL(); 
	_OPENMAYA_POP_WARNING
}

void apiMeshUI::drawShaded( const MDrawRequest & request,
							M3dView & view ) const
//
// Description:
//
//     Shaded drawing routine
//
// Arguments:
//
//     request - request to be drawn
//     view    - view to draw into
//
{
	MDrawData data = request.drawData();
	apiMeshGeom * geom = (apiMeshGeom*)data.geometry();

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL(); 
	_OPENMAYA_POP_WARNING

#if		defined(SGI) || defined(MESA)
	glEnable( GL_POLYGON_OFFSET_EXT );
#else
	glEnable( GL_POLYGON_OFFSET_FILL );
#endif

	// Set up the material
	//
	MMaterial material = request.material();
	material.setMaterial( request.multiPath(), request.isTransparent() );

	// Enable texturing ... 
	// 
	// Note, Maya does not enable texturing if useDefaultMaterial is enabled. 
	// However, you can choose to ignore this in your draw routine.  
	//
	bool drawTexture = 
		material.materialIsTextured() && 
		!view.usingDefaultMaterial(); 

	if ( drawTexture ) glEnable(GL_TEXTURE_2D);

	// Apply the texture to the current view
	//
	if ( drawTexture ) {
		material.applyTexture( view, data );
	}

	// Draw the polygons
	//
	int vid = 0;
	int uv_len = geom->uvcoords.uvcount();
	for ( int i=0; i<geom->faceCount; i++ )
	{
		glBegin( GL_POLYGON );
		for ( int v=0; v<geom->face_counts[i]; v++ )
		{
			MPoint vertex = geom->vertices[ geom->face_connects[vid] ];
			MVector normal = geom->normals[ geom->face_connects[vid] ];
			if (uv_len > 0)
			{

				// If we are drawing the texture, make sure the  coord 
				// arrays are in bounds.
				if ( drawTexture ) {
					float u, v; 
					int uvId1 = geom->uvcoords.uvId(vid); 
					if ( uvId1 < uv_len ) { 
						geom->uvcoords.getUV( uvId1, u, v ); 
						glTexCoord2f( (GLfloat)u, (GLfloat)v ); 
					}
				}
			}	

			glNormal3f( (float)normal[0], (float)normal[1], (float)normal[2] );
			glVertex3f( (float)vertex[0], (float)vertex[1], (float)vertex[2] );
			vid++;
		}
		glEnd();
	} 

	// Turn off texture mode
	//
	if ( drawTexture ) glDisable(GL_TEXTURE_2D);

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL(); 
	_OPENMAYA_POP_WARNING
}

void apiMeshUI::drawVertices( const MDrawRequest & request,
							  M3dView & view ) const
//
// Description:
//
//     Component (vertex) drawing routine
//
// Arguments:
//
//     request - request to be drawn
//     view    - view to draw into
//
{
	MDrawData data = request.drawData();
	apiMeshGeom * geom = (apiMeshGeom*)data.geometry();

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL(); 
	_OPENMAYA_POP_WARNING

	// Query current state so it can be restored
	//
	bool lightingWasOn = glIsEnabled( GL_LIGHTING ) ? true : false;
	if ( lightingWasOn ) {
		glDisable( GL_LIGHTING );
	}
	float lastPointSize;
	glGetFloatv( GL_POINT_SIZE, &lastPointSize );


	// If there is a component specified by the draw request
	// then loop over comp (using an MFnComponent class) and draw the
	// active vertices, otherwise draw all vertices.
	//
	MObject comp = request.component();
	if ( ! comp.isNull() ) {
		// Set the point size of the vertices
		//
		glPointSize( POINT_SIZE*2 );

		MFnSingleIndexedComponent fnComponent( comp );
		for ( int i=0; i<fnComponent.elementCount(); i++ )
		{
			int index = fnComponent.element( i );
			glBegin( GL_POINTS );
			MPoint vertex = geom->vertices[ index  ];
			glVertex3f( (float)vertex[0], 
						(float)vertex[1], 
						(float)vertex[2] );
			glEnd();
		}
	}
	else {
		// Set the point size of the vertices
		//
		glPointSize( POINT_SIZE );

		int vid = 0;
		for ( int i=0; i<geom->faceCount; i++ )
		{
			glBegin( GL_POINTS );
			for ( int v=0; v<geom->face_counts[i]; v++ )
			{
				MPoint vertex =
					geom->vertices[ geom->face_connects[vid++] ];
				glVertex3f( (float)vertex[0], 
							(float)vertex[1], 
							(float)vertex[2] );
			}
			glEnd();
		}
	}

	// Restore the state
	//
	if ( lightingWasOn ) {
		glEnable( GL_LIGHTING );
	}
	glPointSize( lastPointSize );

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL(); 
	_OPENMAYA_POP_WARNING
}

void apiMeshUI::drawBoundingBox( const MDrawRequest & request,
							  M3dView & view ) const
//
// Description:
//
//     Bounding box drawing routine
//
// Arguments:
//
//     request - request to be drawn
//     view    - view to draw into
//
{
	// Get the surface shape
	MPxSurfaceShape *shape = surfaceShape();
	if ( !shape )
		return;
		
	// Get the bounding box	
	MBoundingBox box = shape->boundingBox();
	float w = (float) box.width();
	float h = (float) box.height();
	float d = (float) box.depth();
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL(); 
	_OPENMAYA_POP_WARNING
	
	// Below we just two sides and then connect
	// the edges together

	MPoint minVertex = box.min();
	
	// Draw first side
	glBegin( GL_LINE_LOOP );
	MPoint vertex = minVertex;
	glVertex3f( (float)vertex[0],   (float)vertex[1],   (float)vertex[2] );
	glVertex3f( (float)vertex[0]+w, (float)vertex[1],   (float)vertex[2] );
	glVertex3f( (float)vertex[0]+w, (float)vertex[1]+h, (float)vertex[2] );
	glVertex3f( (float)vertex[0],   (float)vertex[1]+h, (float)vertex[2] );	
	glVertex3f( (float)vertex[0],   (float)vertex[1],   (float)vertex[2] );
	glEnd();

	// Draw second side
	MPoint sideFactor(0,0,d);
	MPoint vertex2 = minVertex + sideFactor;
	glBegin( GL_LINE_LOOP );
	glVertex3f( (float)vertex2[0],   (float)vertex2[1],   (float)vertex2[2] );	
	glVertex3f( (float)vertex2[0]+w, (float)vertex2[1],   (float)vertex2[2] );
	glVertex3f( (float)vertex2[0]+w, (float)vertex2[1]+h, (float)vertex2[2] );
	glVertex3f( (float)vertex2[0],   (float)vertex2[1]+h, (float)vertex2[2] );	
	glVertex3f( (float)vertex2[0],   (float)vertex2[1],   (float)vertex2[2] );	
	glEnd();

	// Connect the edges together
	glBegin( GL_LINES );
	glVertex3f( (float)vertex2[0],   (float)vertex2[1],   (float)vertex2[2] );	
	glVertex3f( (float)vertex[0],   (float)vertex[1],   (float)vertex[2] );

	glVertex3f( (float)vertex2[0]+w,   (float)vertex2[1],   (float)vertex2[2] );	
	glVertex3f( (float)vertex[0]+w,   (float)vertex[1],   (float)vertex[2] );

	glVertex3f( (float)vertex2[0]+w,   (float)vertex2[1]+h,   (float)vertex2[2] );	
	glVertex3f( (float)vertex[0]+w,   (float)vertex[1]+h,   (float)vertex[2] );

	glVertex3f( (float)vertex2[0],   (float)vertex2[1]+h,   (float)vertex2[2] );	
	glVertex3f( (float)vertex[0],   (float)vertex[1]+h,   (float)vertex[2] );
	glEnd();
	
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL(); 
	_OPENMAYA_POP_WARNING
}

// for userChangingViewContext example code 
//
void apiMeshUI::drawRedPointAtCenter( const MDrawRequest & request,
										M3dView & view ) const
//
// Description:
//
//     Simple very fast draw routine
//
// Arguments:
//
//     request - request to be drawn
//     view    - view to draw into
//
{
	// Draw point
	// 
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL();
	_OPENMAYA_POP_WARNING
		
	// save state
	//
	glPushAttrib( GL_CURRENT_BIT | GL_POINT_BIT );
	glPointSize( 20.0f );
	glBegin( GL_POINTS );
	glColor3f( 1.0f, 0.0f, 0.0f );
	glVertex3f(0.0f, 0.0f, 0.0f );
	glEnd();
							
	// restore state
	//
	glPopAttrib();
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL();
	_OPENMAYA_POP_WARNING

}


bool apiMeshUI::selectVertices( MSelectInfo &selectInfo,
							 MSelectionList &selectionList,
							 MPointArray &worldSpaceSelectPts ) const
//
// Description:
//
//     Vertex selection.
//
// Arguments:
//
//     selectInfo           - the selection state information
//     selectionList        - the list of selected items to add to
//     worldSpaceSelectPts  -
//
{
	bool selected = false;
	M3dView view = selectInfo.view();

	MPoint 		xformedPoint;
	MPoint 		selectionPoint;
	double		z,previousZ = 0.0;
 	int			closestPointVertexIndex = -1;

	const MDagPath & path = selectInfo.multiPath();

	// Create a component that will store the selected vertices
	//
	MFnSingleIndexedComponent fnComponent;
	MObject surfaceComponent = fnComponent.create( MFn::kMeshVertComponent );
	int vertexIndex;

	// if the user did a single mouse click and we find > 1 selection
	// we will use the alignmentMatrix to find out which is the closest
	//
	MMatrix	alignmentMatrix;
	MPoint singlePoint; 
	bool singleSelection = selectInfo.singleSelection();
	if( singleSelection ) {
		alignmentMatrix = selectInfo.getAlignmentMatrix();
	}

	// Get the geometry information
	//
	apiMesh* meshNode = (apiMesh*)surfaceShape();
	apiMeshGeom * geom = meshNode->meshGeomToUse();

	// Loop through all vertices of the mesh and
	// see if they lie withing the selection area
	//
	int numVertices = geom->vertices.length();
	for ( vertexIndex=0; vertexIndex<numVertices; vertexIndex++ )
	{
		MPoint currentPoint = geom->vertices[ vertexIndex ];

		// Sets OpenGL's render mode to select and stores
		// selected items in a pick buffer
		//
		view.beginSelect();

		glBegin( GL_POINTS );
		glVertex3f( (float)currentPoint[0], 
					(float)currentPoint[1], 
					(float)currentPoint[2] );
		glEnd();

		if ( view.endSelect() > 0 )	// Hit count > 0
		{
			selected = true;

			if ( singleSelection ) {
				xformedPoint = currentPoint;
				xformedPoint.homogenize();
				xformedPoint*= alignmentMatrix;
				z = xformedPoint.z;
				if ( closestPointVertexIndex < 0 || z > previousZ ) {
					closestPointVertexIndex = vertexIndex;
					singlePoint = currentPoint;
					previousZ = z;
				}
			} else {
				// multiple selection, store all elements
				//
				fnComponent.addElement( vertexIndex );
			}
		}
	}

	// If single selection, insert the closest point into the array
	//
	if ( selected && selectInfo.singleSelection() ) {
		fnComponent.addElement(closestPointVertexIndex);

		// need to get world space position for this vertex
		//
		selectionPoint = singlePoint;
		selectionPoint *= path.inclusiveMatrix();
	}

	// Add the selected component to the selection list
	//
	if ( selected ) {
		MSelectionList selectionItem;
		selectionItem.add( path, surfaceComponent );

		MSelectionMask mask( MSelectionMask::kSelectComponentsMask );
		selectInfo.addSelection(
			selectionItem, selectionPoint,
			selectionList, worldSpaceSelectPts,
			mask, true );
	}

	return selected;
}

bool apiMeshUI::canDrawUV() const
//
// Description: 
//  Tells Maya that this surface shape supports uv drawing. 
//
{
	apiMesh* meshNode = (apiMesh*)surfaceShape();
	apiMeshGeom * geom = meshNode->meshGeomToUse();
	return geom->uvcoords.uvcount() > 0; 
}

void apiMeshUI::drawUVWireframe( 
	apiMeshGeom *geom, 
	M3dView &view, 
	const MTextureEditorDrawInfo &info 
) const 
//
// Description: 
//  Draws the UV layout in wireframe mode. 
// 
{
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL(); 
	_OPENMAYA_POP_WARNING
	
	// Draw the polygons
	//
	int vid = 0;
	int vid_start = vid;
	for ( int i=0; i<geom->faceCount; i++ )
	{
		glBegin( GL_LINES );
		int v;

		vid_start = vid; 
		for ( v=0; v<geom->face_counts[i]-1; v++ )
		{
			float du1, dv1, du2, dv2; 
			int uvId1 = geom->uvcoords.uvId(vid); 
			int uvId2 = geom->uvcoords.uvId(vid+1); 
			geom->uvcoords.getUV( uvId1, du1, dv1 ); 
			geom->uvcoords.getUV( uvId2, du2, dv2 );
			glVertex3f( (GLfloat)du1, (GLfloat)dv1, 0.0f ); 
			glVertex3f( (GLfloat)du2, (GLfloat)dv2, 0.0f ); 
			vid++;
		}
		
		float du1, dv1, du2, dv2; 
		int uvId1 = geom->uvcoords.uvId(vid); 
		int uvId2 = geom->uvcoords.uvId(vid_start); 
		geom->uvcoords.getUV( uvId1, du1, dv1 ); 
		geom->uvcoords.getUV( uvId2, du2, dv2 );
		glVertex3f( (GLfloat)du1, (GLfloat)dv1, 0.0f );
		glVertex3f( (GLfloat)du2, (GLfloat)dv2, 0.0f ); 
		vid ++ ; 
		glEnd();
	} 
	
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL();
	_OPENMAYA_POP_WARNING
	
}

void apiMeshUI::drawUVMapCoord( 
	M3dView &view, 
	int uv, 
	float u, float v, 
	bool drawNum
) const 
//
// Description: 
//  Draw the specified uv value into the port view. If drawNum is true 
//  It will also draw the UV id for the the UV.  
//
{
	if ( drawNum ) 
	{ 
		char s[32];
		sprintf( s, "%d", uv );
		_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
		view.drawText( s, MPoint( u, v, 0 ), M3dView::kCenter );
		_OPENMAYA_POP_WARNING
	}  
	glVertex3f( (GLfloat)u, (GLfloat)v, 0.0f ); 
}

void apiMeshUI::drawUVMapCoordNum( 
	apiMeshGeom *geom, 
	M3dView &view, 
	const MTextureEditorDrawInfo &info, 
	bool drawNumbers 
) const 
//
// Description: 
//  Draw the UV points for all uvs on this surface shape. 
//
{
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.beginGL(); 
	_OPENMAYA_POP_WARNING

	GLfloat ptSize; 
	glGetFloatv( GL_POINT_SIZE, &ptSize ); 
	glPointSize( UV_POINT_SIZE );

	int uv; 
	int uv_len = geom->uvcoords.uvcount();
	for ( uv = 0; uv < uv_len; uv ++ ) { 
		float du, dv; 
		geom->uvcoords.getUV( uv, du, dv ); 
		drawUVMapCoord( view, uv, du, dv, drawNumbers );
	}

	glPointSize( ptSize ); 

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	view.endGL(); 
	_OPENMAYA_POP_WARNING
}

void apiMeshUI::drawUV( M3dView &view, const MTextureEditorDrawInfo &info ) const
// 
// Description: 
//   Main entry point for UV drawing. This method is called by the UV 
//   texture editor when the shape is 'active'. 
// 
// Input: 
//   A 3dView. 
//
{
	apiMesh* meshNode = (apiMesh*)surfaceShape();
	apiMeshGeom * geom = meshNode->meshGeomToUse();

	int uv_len = geom->uvcoords.uvcount(); 
	if (uv_len > 0)
	{
		_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
		view.setDrawColor( MColor( 1.0f, 0.0f, 0.0f ) ); 
		_OPENMAYA_POP_WARNING
	
		switch( info.drawingFunction() ) { 
		case MTextureEditorDrawInfo::kDrawWireframe: 
			drawUVWireframe( geom, view, info ); 
			break; 
		case MTextureEditorDrawInfo::kDrawEverything: 
		case MTextureEditorDrawInfo::kDrawUVForSelect: 
			drawUVWireframe( geom, view, info ); 
			drawUVMapCoordNum( geom, view, info, false ); 
			break; 
		case MTextureEditorDrawInfo::kDrawVertexForSelect: 
		case MTextureEditorDrawInfo::kDrawEdgeForSelect: 
		case MTextureEditorDrawInfo::kDrawFacetForSelect: 
		default: 
			drawUVWireframe( geom, view, info ); 
		};
	}
}
