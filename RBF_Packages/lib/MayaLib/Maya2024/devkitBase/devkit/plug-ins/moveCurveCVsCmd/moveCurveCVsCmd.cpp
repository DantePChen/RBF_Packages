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
// Produces the MEL command "moveCurveCVs".
//
// This is a simple little plug-in that takes the selected CVs of a NURBS curves
// and moves them to the origin. It is not a very practical plug-in, but
// it demonstrates retrieving CVs from a selection list and the use of the Curve CV iterator.
//
// To use the plug-in:
//	(1) Draw a curve.
//	(2) Switch Maya from Object selection mode to Component selection mode.
//	(3) Pick some or all the CVs of the curve.
//	(4) Type the command "moveCurveCVs" in the command window to move the CVs.  
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MDagPath.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MItCurveCV.h>

class moveCurveCVs : public MPxCommand
{
public:
					moveCurveCVs() {};
				~moveCurveCVs() override; 

	MStatus			doIt( const MArgList& args ) override;
	static void*	creator();
};

moveCurveCVs::~moveCurveCVs() {}

void* moveCurveCVs::creator()
{
	return new moveCurveCVs();
}

MStatus moveCurveCVs::doIt( const MArgList& )
{
	MStatus stat;

	// Create a selection list iterator
	// set the filter moveCurveCVsComponents
	//
	MSelectionList list;
	MGlobal::getActiveSelectionList( list );
	MItSelectionList * iter = new MItSelectionList( list,
													MFn::kCurveCVComponent,
													&stat );

	if ( MS::kSuccess == stat ) {
		MDagPath 	mdagPath;		// Item dag path
		MObject  	mComponent;		// Current component
		double 		coord[4] = {0.0,0.0,0.0,0.0};
		MPoint 		point( coord );

		// Iterate over all selected curves
		//
		for ( ; !iter->isDone(); iter->next() ) 
		{
			// Create a function set to operate on selected curves
			//
			iter->getDagPath( mdagPath, mComponent );
			MItCurveCV fnmoveCurveCVs( mdagPath, mComponent, &stat );

			if ( MS::kSuccess == stat ) {
				for ( ; !fnmoveCurveCVs.isDone(); fnmoveCurveCVs.next() )
				{
					fnmoveCurveCVs.setPosition( point, MSpace::kWorld );
				}
				fnmoveCurveCVs.updateCurve();
			}
			else {
				cerr << "Function set error\n";
			}
		}
	}
	else {
		cerr << "Error creating selection list iterator\n";
	}
	return stat;
}


MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerCommand( "moveCurveCVs", moveCurveCVs::creator );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterCommand( "moveCurveCVs" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
