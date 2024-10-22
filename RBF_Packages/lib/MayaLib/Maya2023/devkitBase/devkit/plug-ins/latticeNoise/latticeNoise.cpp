//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// Produces the dependency graph node "latticeNoise" and MEL command "latticeNoise".
//
// This plug-in is an example of the following:
//
//	- how to have node attributes input and output geometry
//	- how to modify dependency graph connections using the API
//	- how to take Maya objects as arguments to a user defined MEL command
//
// The "latticeNoise" command creates a new lattice deformer around the currently selected geometry
// or around the objects specified on the command line. The command also inserts a "latticeNoise" node
// in between the lattice shape in the DAG and the node that performs the deformation.
//
// The end effect of the "latticeNoise" command is that the objects inside the lattice deform
// with respect to the lattice, but they also wobble about randomly as noise is applied to the lattice points.
// The "latticeNoise" node has attributes for amplitude and frequency that control the amount of noise applied.
//
// An example of using the command is:
//
//	latticeNoise nurbsSphereShape1 nurbsConeShape1;
//
////////////////////////////////////////////////////////////////////////

#include "latticeNoise.h"
#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <stdlib.h>

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	// Register latticeNoise node
	//
	status = plugin.registerNode( "latticeNoise", latticeNoiseNode::id,
										 latticeNoiseNode::creator,
										 latticeNoiseNode::initialize);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	// Register latticeNoise command
	//
	status = plugin.registerCommand( "latticeNoise", latticeNoiseCmd::creator );
	if (!status) {
		plugin.deregisterNode( latticeNoiseNode::id );
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	// Deregister latticeNoise node
	//
	status = plugin.deregisterNode( latticeNoiseNode::id ); 
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	// Register latticeNoise command
	//
	status = plugin.deregisterCommand( "latticeNoise" ); 
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}

//
//  Class: noise
//
//  Description:
//      The noise class is used for generating pseudo-random continuous noise.
//      The noise values generated are always between 0 and 1.
//
//      The technique used is a simple lattice noise algorithm based upon one 
//      by Ken Perlin.  This particular implementation is adapted from 
//      Darwyn Peachey's (Texturing and Modeling: a Procedural Approach, S. Ebert
//      Editor, 1994).
//
#include <stdlib.h>
#include <math.h>

const int kTableMask = TABLE_SIZE - 1;



//////////////////////////////////////////////////////////////
//                       PUBLIC                             //
//////////////////////////////////////////////////////////////

float noise::atValue( float x )
//
//  Description:
//      Get the noise value at the given point in 1-space.
//
//  Arguments:
//      x - the point at which to calculate the noise
//
//  Return Value:
//      the noise value at the point
//
{
	int ix; 
	float fx; 

	if ( !isInitialized ) {
		initTable( 23479015 );
		isInitialized = 1;
	}

	ix = (int)floorf( x );
	fx = x - (float)ix; 
 
	return spline( fx, value( ix - 1 ),
				       value( ix ),
				       value( ix + 1 ),
				       value( ix + 2 ) );
}

float noise::atPoint( float x, float y, float z )
//
//  Description:
//      Get the noise value at the given point in 3-space.
//
//  Arguments:
//      x - x component of point
//      y - x component of point
//      z - x component of point
//
//  Return Value:
//      the noise value at the point
//
{
	int ix, iy, iz;
	int i, j, k;
	float fx, fy, fz;
	float xknots[4], yknots[4], zknots[4];

	if ( !isInitialized ) {
		initTable( 23479015 );
	}

	ix = (int)floorf( x );
	fx = x - (float)ix; 

	iy = (int)floorf( y );
	fy = y - (float)iy;

	iz = (int)floorf( z );
	fz = z - (float)iz;

	for ( k = -1; k <= 2; k++ ) {
		for ( j = -1; j <= 2; j++ ) {
			for ( i = -1; i <= 2 ; i++ ) {
				xknots[i+1] = value( ix + i, iy + j, iz + k );
			}
			yknots[j+1] = spline( fx, xknots[0], xknots[1], xknots[2], xknots[3] );
		}
		zknots[k+1] = spline( fy, yknots[0], yknots[1], yknots[2], yknots[3] );
	}

	float val = spline( fz, zknots[0], zknots[1], zknots[2], zknots[3] ); 

	return val;
}

pnt noise::atPointAndTime( float x, float y, float z, float t )
//
//  Description:
//      Get three noise values at the given point in 4-space.  This is actually
//      a reasonably expensive operation.  It requires 255 simple spline 
//      interpolations.  However, the noise function is continuous. 
//
//  Arguments:
//      x - x component of point
//      y - x component of point
//      z - x component of point
//      t - t component of point (time component)
//
//  Return Value:
//      the noise value at the point
//
{
	pnt ret;
	
	int ix, iy, iz, it;
	int i, j, k, l;
	float fx, fy, fz, ft;
	float xknots[3][4], yknots[3][4], zknots[3][4], tknots[3][4];

	if ( !isInitialized ) {
		initTable( 23479015 );
	}

	ix = (int)floorf( x );
	fx = x - (float)ix; 

	iy = (int)floorf( y );
	fy = y - (float)iy;

	iz = (int)floorf( z );
	fz = z - (float)iz;

	it = (int)floorf( t);
	ft = t - (float)it;

	for ( l = -1; l <= 2; l++ ) {
		for ( k = -1; k <= 2; k++ ) {
			for ( j = -1; j <= 2; j++ ) {
				for ( i = -1; i <= 2 ; i++ ) {
					xknots[0][i+1] = value( ix + i, iy + j, iz + k, it + l,
											valueTable1 ); 
					xknots[1][i+1] = value( ix + i, iy + j, iz + k, it + l,
											valueTable2 ); 
					xknots[2][i+1] = value( ix + i, iy + j, iz + k, it + l,
											valueTable3 ); 
				}
				yknots[0][j+1] = spline( fx, xknots[0][0], xknots[0][1], 
										 xknots[0][2], xknots[0][3] );
				yknots[1][j+1] = spline( fx, xknots[1][0], xknots[1][1], 
										 xknots[1][2], xknots[1][3] );
				yknots[2][j+1] = spline( fx, xknots[2][0], xknots[2][1], 
										 xknots[2][2], xknots[2][3] );
			}
			zknots[0][k+1] = spline( fy, yknots[0][0], yknots[0][1], 
									 yknots[0][2], yknots[0][3] );
			zknots[1][k+1] = spline( fy, yknots[1][0], yknots[1][1], 
									 yknots[1][2], yknots[1][3] );
			zknots[2][k+1] = spline( fy, yknots[2][0], yknots[2][1], 
									 yknots[2][2], yknots[2][3] );
		}
		tknots[0][l+1] = spline( fz, zknots[0][0], zknots[0][1], 
								 zknots[0][2], zknots[0][3] );
		tknots[1][l+1] = spline( fz, zknots[1][0], zknots[1][1], 
								 zknots[1][2], zknots[1][3] );
		tknots[2][l+1] = spline( fz, zknots[2][0], zknots[2][1], 
								 zknots[2][2], zknots[2][3] );
	} 
	ret.x = spline( ft, tknots[0][0], tknots[0][1], tknots[0][2], tknots[0][3] ); 
	ret.y = spline( ft, tknots[1][0], tknots[1][1], tknots[1][2], tknots[1][3] ); 
	ret.z = spline( ft, tknots[2][0], tknots[2][1], tknots[2][2], tknots[2][3] ); 

	return ret;
}
 

void  noise::initTable( long seed )
//
//  Description:
//      Initialize the table of random values with the given seed. 
//
//  Arguments:
//      seed - the new seed value 
//
{
	srand48( seed );

	for ( int i = 0; i < TABLE_SIZE; i++ ) {
		valueTable1[i] = (float)drand48();
		valueTable2[i] = (float)drand48();
		valueTable3[i] = (float)drand48();
	} 
	isInitialized = 1; 
}



//////////////////////////////////////////////////////////////
//                       PRIVATE                            //
//////////////////////////////////////////////////////////////
 

float noise::spline( float x, float knot0, float knot1, float knot2, float knot3 )
//
//  Description:
//      This is a simple version of a Catmull-Rom spline interpolation.  
//
//  Assumptions:
//      
//      0 < x < 1
// 
//
{
	float c0, c1, c2, c3;

	// Evaluate span of cubic at x using Horner's rule
	//
	c3 = (-0.5F * knot0 ) + ( 1.5F * knot1 ) + (-1.5F * knot2 ) + ( 0.5F * knot3 );
	c2 = ( 1.0F * knot0 ) + (-2.5F * knot1 ) + ( 2.0F * knot2 ) + (-0.5F * knot3 );
	c1 = (-0.5F * knot0 ) + ( 0.0F * knot1 ) + ( 0.5F * knot2 ) + ( 0.0F * knot3 );
	c0 = ( 0.0F * knot0 ) + ( 1.0F * knot1 ) + ( 0.0F * knot2 ) + ( 0.0F * knot3 );
		  
	return ( ( c3 * x + c2 ) * x + c1 ) * x + c0;;
}


int noise::isInitialized = 0;

int noise::permtable[256] = {
    254,    91,     242,    186,    90,     204,    85,     133,    233,
    50,     187,    49,     182,    224,    144,    166,    7,      51,
    20,     179,    36,     203,    114,    156,    195,    40,     24,
    60,     162,    84,     126,    102,    63,     194,    220,    161,
    72,     94,     193,    229,    140,    57,     3,      189,    106,
    54,     164,    198,    199,    44,     245,    235,    100,    87,
    25,     41,     62,     111,    13,     70,     27,     82,     69,
    53,     66,     247,    124,    67,     163,    125,    155,    228,
    122,    19,     113,    143,    121,    9,      1,      241,    171,
    200,    83,     244,    185,    170,    141,    115,    190,    154,
    48,     32,     178,    127,    167,    56,     134,    15,     160,
    238,    64,     6,      11,     196,    232,    26,     89,     0,
    219,    112,    68,     30,     215,    227,    75,     132,    71,
    239,    251,    92,     14,     104,    231,    29,     180,    150,
    226,    191,    47,     73,     37,     183,    88,     105,    42,
    22,     2,      38,     5,      119,    74,     249,    184,    52,
    8,      55,     118,    255,    206,    173,    165,    78,     31,
    123,    98,     212,    80,     139,    61,     138,    77,     177,
    45,     137,    145,    28,     168,    128,    95,     223,    35,
    205,    76,     211,    175,    81,     33,     207,    21,     131,
    58,     152,    16,     240,    18,     96,     210,    109,    214,
    216,    202,    148,    34,     146,    117,    176,    93,     246,
    172,    97,     159,    197,    218,    65,     147,    253,    221,
    217,    79,     101,    142,    23,     149,    99,     39,     12,
    135,    110,    234,    108,    153,    129,    4,      169,    174,
    116,    243,    130,    107,    222,    10,     43,     188,    46,
    213,    252,    86,     157,    192,    236,    158,    120,    17,
    103,    248,    225,    230,    250,    208,    181,    151,    237,
    201,    59,     136,    209 
};

float noise::valueTable1[256];
float noise::valueTable2[256];
float noise::valueTable3[256];
