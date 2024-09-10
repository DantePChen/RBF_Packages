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
// 
// DESCRIPTION:
// 
// Produces the MEL command "helix".
//
// This example demonstrates building a simple command that does not have "undo".
// The command accepts two arguments: "-r" to set the radius of the helix and "-p" to set
// the pitch of the helix. So, to create a helix, execute the command "helix [ -r #] [ -p #]"
// in the command window. 
// 
// A MEL script is also provided with this example that can be run by executing the command "source helixCmd".
// 
// This script creates a new "Plug-ins" menu under which the "helix" command can be found.
// Selecting this menu item will bring up a window that allows you to set the radius and pitch for the new helix.
// This is a good example of hooking up a command to the UI. 
//
////////////////////////////////////////////////////////////////////////

#include <math.h>

#include <maya/MIOStream.h>
#include <maya/MSimple.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>

DeclareSimpleCommand( helix, PLUGIN_COMPANY, "3.0");

MStatus helix::doIt( const MArgList& args )
{
	MStatus stat;

	const unsigned	deg 	= 3;			// Curve Degree
	const unsigned	ncvs 	= 20;			// Number of CVs
	const unsigned	spans 	= ncvs - deg;	// Number of spans
	const unsigned	nknots	= spans+2*deg-1;// Number of knots
	double	radius			= 4.0;			// Helix radius
	double	pitch 			= 0.5;			// Helix pitch
	unsigned	i;

	// Parse the arguments.
	for ( i = 0; i < args.length(); i++ )
		if ( MString( "-p" ) == args.asString( i, &stat )
				&& MS::kSuccess == stat)
		{
			double tmp = args.asDouble( ++i, &stat );
			if ( MS::kSuccess == stat )
				pitch = tmp;
		}
		else if ( MString( "-r" ) == args.asString( i, &stat )
				&& MS::kSuccess == stat)
		{
			double tmp = args.asDouble( ++i, &stat );
			if ( MS::kSuccess == stat )
				radius = tmp;
		}

	MPointArray	 controlVertices;
	MDoubleArray knotSequences;

	// Set up cvs and knots for the helix
	//
	for (i = 0; i < ncvs; i++)
		controlVertices.append( MPoint( radius * cos( (double)i ),
			pitch * (double)i, radius * sin( (double)i ) ) );

	for (i = 0; i < nknots; i++)
		knotSequences.append( (double)i );

	// Now create the curve
	//
	MFnNurbsCurve curveFn;

	curveFn.create( controlVertices,
					knotSequences, deg, 
					MFnNurbsCurve::kOpen, 
					false, false, 
					MObject::kNullObj, 
					&stat );

	if ( MS::kSuccess != stat )
		cout<<"Error creating curve."<<endl;

	return stat;
}
