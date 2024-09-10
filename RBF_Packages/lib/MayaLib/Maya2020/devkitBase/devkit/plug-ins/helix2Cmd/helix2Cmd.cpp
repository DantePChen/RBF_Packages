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
// Produces the MEL command "helix2".
//
// This example takes a selected curve and turns it into a helix.
// Moreover, the plug-in implements undo and redo functions so that the change can be undone and then redone.
// This plug-in is a simple example of implementing a command that supports do, undo, and redo.
//
// To use the plug-in:
//	(1) Create a curve, and then execute "helix2" in the command window.
//		The curve will change into a helix.
//	(2) Select "Undo" from the "Edit" menu and the change will be undone.
//		Select "Redo" and it will be redone.  
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MFnPlugin.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MItCurveCV.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MString.h>
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>

class helix2 : public MPxCommand
{

public:
				helix2();
			~helix2() override;

	MStatus		doIt( const MArgList& ) override;
	MStatus		redoIt() override;
	MStatus		undoIt() override;
	bool		isUndoable() const override;

	static		void* creator();

private:
	MDagPath	fDagPath;
	MObject 	fComponent;
	MPointArray	fCVs;
	double		radius;
	double		pitch;
};

MStatus helix2::doIt( const MArgList& args )
{
	MStatus status;

	// Parse the arguments.
	for ( unsigned i = 0; i < args.length(); i++ ) {
		if ( MString( "-p" ) == args.asString( i, &status )
				&& MS::kSuccess == status)
		{
			double tmp = args.asDouble( ++i, &status );
			if ( MS::kSuccess == status )
				pitch = tmp;
		}
		else if ( MString( "-r" ) == args.asString( i, &status )
				&& MS::kSuccess == status)
		{
			double tmp = args.asDouble( ++i, &status );
			if ( MS::kSuccess == status )
				radius = tmp;
		} else {
			MString msg = "Invalid flag: ";
			msg += args.asString( i );
			displayError( msg );
			return MS::kFailure;
		}
	}

	// Get the first selected curve from the selection list.
	MSelectionList slist;
	MGlobal::getActiveSelectionList( slist );

	MItSelectionList	list( slist, MFn::kNurbsCurve, &status );
	if (MS::kSuccess != status) {
		cerr << "doIt: could not create selection list iterator\n";
		return status;
	}

	if (list.isDone()) {
		cerr << "doIt: no curve has been selected\n";
		return MS::kFailure;
	}

	list.getDagPath( fDagPath, fComponent );

	return redoIt();
}

MStatus helix2::redoIt()
{
	unsigned		i, numCVs;
	MStatus			status;
	MFnNurbsCurve   curveFn( fDagPath );

	numCVs = curveFn.numCVs();
	status = curveFn.getCVs( fCVs );
	if ( MS::kSuccess != status )
	{
		cerr << "redoIt: could not get cvs status: " << status << endl;
		return MS::kFailure;
	}

	MPointArray		points(fCVs);
	for (i = 0; i < numCVs; i++)
		points[i] = MPoint( radius * cos( (double)i ),
							pitch * (double)i,
							radius * sin( (double)i ) );
	status = curveFn.setCVs( points );
	if ( MS::kSuccess != status )
	{
		cerr << "redoIt: could not setCVs status: " << status << endl;
		fCVs.clear();
		return status;
	}

	status = curveFn.updateCurve();
	if ( MS::kSuccess != status )
	{
		cerr << "redoIt: updateCurve() failed status: " << status << endl;
		return status;
	}

	return MS::kSuccess;
}

MStatus helix2::undoIt()
{
	MStatus		status;

	MFnNurbsCurve	curveFn( fDagPath );
	status = curveFn.setCVs( fCVs );
	if ( MS::kSuccess != status)
	{
		cerr << "undoIt: array length is " << fCVs.length()
		     << "bad status: " << status << endl;
		return status;
	}

	status = curveFn.updateCurve();
	if ( MS::kSuccess != status )
	{
		cerr << "undoIt: updateCurve() failed status: " << status << endl;
		return status;
	}

	fCVs.clear();
	return MS::kSuccess;
}

void* helix2::creator()
{
	return new helix2();
}

helix2::helix2()
	: radius( 4.0 ),
	  pitch( 0.5 ),
	  fComponent()
{}

helix2::~helix2()
{
	fCVs.clear();
	// Note that we do nothing with fComponent which is owned by Maya.
}

bool helix2::isUndoable() const
{
	return true;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerCommand( "helix2", helix2::creator );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterCommand( "helix2" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
