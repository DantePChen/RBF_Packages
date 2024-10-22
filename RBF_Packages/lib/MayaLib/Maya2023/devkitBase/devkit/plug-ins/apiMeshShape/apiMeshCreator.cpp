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
// apiMeshCreator.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <maya/MIOStream.h>

#include "apiMeshCreator.h"           
#include "apiMeshData.h"
#include "api_macros.h"

#include <maya/MFnMesh.h>
#include <maya/MFnPluginData.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2		1.57079632679489661923
#endif

////////////////////////////////////////////////////////////////////////////////
//
// Shape implementation
//
////////////////////////////////////////////////////////////////////////////////

MObject apiMeshCreator::size;
MObject apiMeshCreator::shapeType;
MObject apiMeshCreator::inputMesh;
MObject apiMeshCreator::outputSurface;

MTypeId apiMeshCreator::id( 0x80089 );

apiMeshCreator::apiMeshCreator() {}
apiMeshCreator::~apiMeshCreator() {}

///////////////////////////////////////////////////////////////////////////////
//
// Overrides
//
///////////////////////////////////////////////////////////////////////////////

/* override */
MStatus apiMeshCreator::compute( const MPlug& plug, MDataBlock& datablock )
//
// Description
//
//    When input attributes are dirty this method will be called to
//    recompute the output attributes.
//
{ 
    MStatus stat;
    if ( plug == outputSurface ) {
		// Create some user defined geometry data and access the
		// geometry so we can set it
		//
		MFnPluginData fnDataCreator;
		MTypeId tmpid( apiMeshData::id );
		
		fnDataCreator.create( tmpid, &stat );
		MCHECKERROR( stat, "compute : error creating apiMeshData")

		apiMeshData * newData = (apiMeshData*)fnDataCreator.data( &stat );
		MCHECKERROR( stat, "compute : error gettin at proxy apiMeshData object")

		apiMeshGeom * geomPtr = newData->fGeometry;

		// If there is an input mesh then copy it's values
		// and construct some apiMeshGeom for it.
		//
		bool hasHistory = computeInputMesh( plug, datablock,
											geomPtr->vertices,
											geomPtr->face_counts,
											geomPtr->face_connects,
											geomPtr->normals, 
											geomPtr->uvcoords
			);
											
		// There is no input mesh so check the shapeType attribute
		// and create either a cube or a sphere.
		//
		if ( !hasHistory ) {
			MDataHandle sizeHandle = datablock.inputValue( size );
			double shape_size = sizeHandle.asDouble();
			MDataHandle typeHandle = datablock.inputValue( shapeType );
			short shape_type = typeHandle.asShort();

			switch( shape_type )
			{
				case 0 : // build a cube
					buildCube( shape_size,
							   geomPtr->vertices,
							   geomPtr->face_counts,
							   geomPtr->face_connects,
							   geomPtr->normals, 
							   geomPtr->uvcoords
						);
					break;
			
				case 1 : // buld a sphere
					buildSphere( shape_size,
								 32,
								 geomPtr->vertices,
								 geomPtr->face_counts,
								 geomPtr->face_connects,
								 geomPtr->normals, 
								 geomPtr->uvcoords
						);
					break;
			} // end switch
		}

		geomPtr->faceCount = geomPtr->face_counts.length();

		// Assign the new data to the outputSurface handle
		//
		MDataHandle outHandle = datablock.outputValue( outputSurface );
		outHandle.set( newData );
		datablock.setClean( plug );
		return MS::kSuccess;
    }
    else {
        return MS::kUnknownParameter;
    }
}

///////////////////////////////////////////////////////////////////////////////
//
// Helper routines
//
///////////////////////////////////////////////////////////////////////////////

void apiMeshCreator::buildCube( 
	double cube_size, 
	MPointArray& pa,
	MIntArray& faceCounts, 
	MIntArray& faceConnects,
	MVectorArray& normals, 
	apiMeshGeomUV& uvs
)
//
// Description
//
//    Constructs a cube
//
{
	const int num_faces			= 6;
	const int num_face_connects	= 24;
	const double normal_value   = 0.5775;
	const int uv_count			= 14; 
	
	pa.clear(); faceCounts.clear(); faceConnects.clear();
	uvs.reset(); 

	pa.append( MPoint( -cube_size, -cube_size, -cube_size ) );
	pa.append( MPoint(  cube_size, -cube_size, -cube_size ) );
	pa.append( MPoint(  cube_size, -cube_size,  cube_size ) );
	pa.append( MPoint( -cube_size, -cube_size,  cube_size ) );
	pa.append( MPoint( -cube_size,  cube_size, -cube_size ) );
	pa.append( MPoint( -cube_size,  cube_size,  cube_size ) );
	pa.append( MPoint(  cube_size,  cube_size,  cube_size ) );
	pa.append( MPoint(  cube_size,  cube_size, -cube_size ) );

	normals.append( MVector( -normal_value, -normal_value, -normal_value ) );
	normals.append( MVector(  normal_value, -normal_value, -normal_value ) );
	normals.append( MVector(  normal_value, -normal_value,  normal_value ) );
	normals.append( MVector( -normal_value, -normal_value,  normal_value ) );
	normals.append( MVector( -normal_value,  normal_value, -normal_value ) );
	normals.append( MVector( -normal_value,  normal_value,  normal_value ) );
	normals.append( MVector(  normal_value,  normal_value,  normal_value ) );
	normals.append( MVector(  normal_value,  normal_value, -normal_value ) );

	// Define the UVs for the cube. 
	//
	float uv_pts[uv_count * 2] = { 0.375, 0.0, 
								   0.625, 0.0,
								   0.625, 0.25, 
								   0.375, 0.25,
								   0.625, 0.5,
								   0.375, 0.5,
								   0.625, 0.75,
								   0.375, 0.75,
								   0.625, 1.0,
								   0.375, 1.0,
								   0.875, 0.0,
								   0.875, 0.25,
								   0.125, 0.0,
								   0.125, 0.25 
	}; 
	
	// UV Face Vertex Id. 
	//
	int uv_fvid [ num_face_connects ] = {  0, 1, 2, 3, 
										   3, 2, 4, 5, 
										   5, 4, 6, 7, 
										   7, 6, 8, 9, 
										   1, 10, 11, 2, 
										   12, 0, 3, 13 };

	int i;
	for ( i = 0; i < uv_count; i ++ ) {
		uvs.append_uv( uv_pts[i*2], uv_pts[i*2 + 1] ); 
	}
	
	for ( i = 0; i < num_face_connects; i ++ ) { 
		uvs.faceVertexIndex.append( uv_fvid[i] ); 
	}

	// Set up an array containing the number of vertices
	// for each of the 6 cube faces (4 verticies per face)
	//
	int face_counts[num_faces] = { 4, 4, 4, 4, 4, 4 };

	for ( i=0; i<num_faces; i++ )
	{
		faceCounts.append( face_counts[i] );
	}

	// Set up and array to assign vertices from pa to each face 
	//
	int face_connects[ num_face_connects ] = {	0, 1, 2, 3,
												4, 5, 6, 7,
												3, 2, 6, 5,
												0, 3, 5, 4,
												0, 4, 7, 1,
												1, 7, 6, 2	};
	for ( i=0; i<num_face_connects; i++ )
	{
		faceConnects.append( face_connects[i] );
	}
}

void apiMeshCreator::buildSphere( 
	double				rad, 
	int					div, 
	MPointArray &		vertices,
	MIntArray &			counts, 
	MIntArray &			connects,
	MVectorArray &		normals, 
	apiMeshGeomUV &		uvs
)
//
// Description
//
//    Create circles of vertices starting with 
//    the top pole ending with the botton pole
//
{
	double u = -M_PI_2;
	double v = -M_PI;
	double u_delta = M_PI / ((double)div); 
	double v_delta = 2 * M_PI / ((double)div); 

	MPoint topPole( 0.0, rad, 0.0 );
	MPoint botPole( 0.0, -rad, 0.0 );

	// Build the vertex and normal table
	//
	vertices.append( botPole );
	normals.append( botPole-MPoint::origin );
	int i;
	for ( i=0; i<(div-1); i++ )
	{
		u += u_delta;
		v = -M_PI;

		for ( int j=0; j<div; j++ )
		{
			double x = rad * cos(u) * cos(v);
			double y = rad * sin(u);
			double z = rad * cos(u) * sin(v) ;
			MPoint pnt( x, y, z );
			vertices.append( pnt );
			normals.append( pnt-MPoint::origin );
			v += v_delta;
		}
	}
	vertices.append( topPole );
	normals.append( topPole-MPoint::origin );

	// Create the connectivity lists
	//
	int vid = 1;
	int numV = 0;
	for ( i=0; i<div; i++ )
	{
		for ( int j=0; j<div; j++ )
		{
			if ( i==0 ) {
				counts.append( 3 );
				connects.append( 0 );
				connects.append( j+vid );
				connects.append( (j==(div-1)) ? vid : j+vid+1 );
			}
			else if ( i==(div-1) ) {
				counts.append( 3 );
				connects.append( j+vid+1-div );
				connects.append( vid+1 );
				connects.append( j==(div-1) ? vid+1-div : j+vid+2-div );
			}
			else {
				counts.append( 4 );
				connects.append( j + vid+1-div );
				connects.append( j + vid+1 );
				connects.append( j == (div-1) ? vid+1 : j+vid+2 );
				connects.append( j == (div-1) ? vid+1-div : j+vid+2-div );
			}
			numV++;
		}
		vid = numV;
	}

	// TODO: Define UVs for sphere ...
	//
}

MStatus apiMeshCreator::computeInputMesh( 
	const MPlug&		plug,
	MDataBlock&			datablock,
	MPointArray&		vertices,
	MIntArray&			counts,
	MIntArray&			connects,
	MVectorArray&		normals, 
	apiMeshGeomUV&		uvs
)
//
// Description
//
//     This function takes an input surface of type kMeshData and converts
//     the geometry into this nodes attributes.
//     Returns kFailure if nothing is connected.
//
{
	MStatus stat;

    // Get the input subdiv
    //        
    MDataHandle inputData = datablock.inputValue( inputMesh, &stat );
    MCHECKERROR( stat, "compute get inputMesh")
    MObject surf = inputData.asMesh();

	// Check if anything is connected
	//
	MObject thisObj = thisMObject();
	MPlug surfPlug( thisObj, inputMesh );
	if ( !surfPlug.isConnected() ) {
    	stat = datablock.setClean( plug );
	    MCHECKERROR( stat, "compute setClean" )
		return MS::kFailure;
	}

	// Extract the mesh data
	//
   	MFnMesh surfFn (surf, &stat);
	MCHECKERROR( stat, "compute - MFnMesh error" );
	stat = surfFn.getPoints( vertices, MSpace::kObject );
	MCHECKERROR( stat, "compute getPoints"); 

	// Check to see if we have UVs to copy. 
	//
	bool hasUVs = surfFn.numUVs() > 0; 	
	surfFn.getUVs( uvs.ucoord, uvs.vcoord ); 

	for ( int i=0; i<surfFn.numPolygons(); i++ )
	{
		MIntArray polyVerts;
		surfFn.getPolygonVertices( i, polyVerts );
		int pvc = polyVerts.length();
		counts.append( pvc );
		int uvId; 
		for ( int v=0; v<pvc; v++ ) {
			if ( hasUVs ) {
				surfFn.getPolygonUVid( i, v, uvId );
				uvs.faceVertexIndex.append( uvId );
			}
			connects.append( polyVerts[v] );
		}
	}

	for ( int n=0; n<(int)vertices.length(); n++ )
	{
		MVector normal;
		surfFn.getVertexNormal( n, true, normal );
		normals.append( normal );
	}

	return MS::kSuccess;
}

void* apiMeshCreator::creator()
//
// Description
//    Called internally to create a new instance of the users MPx node.
//
{
	return new apiMeshCreator();
}

MStatus apiMeshCreator::initialize()
//
// Description
//
//    Attribute (static) initialization. See api_macros.h.
//
{ 
	MStatus				stat;
    MFnTypedAttribute	typedAttr;
	MFnEnumAttribute	enumAttr;

	// ----------------------- INPUTS -------------------------

	MAKE_NUMERIC_ATTR(	size, "size", "sz",
						MFnNumericData::kDouble, 1,
						false, false, true );

	shapeType = enumAttr.create( "shapeType", "st", 0, &stat );
	MCHECKERROR( stat, "create shapeType attribute" );
	stat = enumAttr.addField( "cube", 0 );
	MCHECKERROR( stat, "add enum type cube" );
	stat = enumAttr.addField( "sphere", 1 );
	MCHECKERROR( stat, "add enum type sphere" );
    CHECK_MSTATUS( enumAttr.setHidden( false ) );
    CHECK_MSTATUS( enumAttr.setKeyable( true ) );
	ADD_ATTRIBUTE( shapeType );
	
	MAKE_TYPED_ATTR( inputMesh, "inputMesh", "im", MFnData::kMesh, MObject::kNullObj );

	// ----------------------- OUTPUTS -------------------------
    outputSurface = typedAttr.create( "outputSurface", "os",
									  apiMeshData::id,
									  MObject::kNullObj, &stat );
    MCHECKERROR( stat, "create outputSurface attribute" )
	typedAttr.setWritable( false );
    ADD_ATTRIBUTE( outputSurface );

	// ---------- Specify what inputs affect the outputs ----------
	//
    ATTRIBUTE_AFFECTS( inputMesh, outputSurface );
    ATTRIBUTE_AFFECTS( size, outputSurface );
    ATTRIBUTE_AFFECTS( shapeType, outputSurface );

	return MS::kSuccess;
}
