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
// Produces the MEL command "cvExpand".
//
// This example demonstrates how to handle selection lists and return the contents
// in a string form that the scripting language will understand. The "cvExpand"
// command goes through the current selection list and splits the range of CVs
// into individual strings for each CV. So, if the selection list looks like this:
// ls -selection;
// // Result: curveShape1.cv[1:3] //
// Then, the "cvExpand" command will return the following:
// cvExpand;
// // Result: curveShape1.cv[1] curveShape1.cv[2] curveShape1.cv[3] //
//
////////////////////////////////////////////////////////////////////////

#include <maya/MSimple.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>

#include <maya/MIOStream.h>

DeclareSimpleCommand( cvExpand, PLUGIN_COMPANY, "3.0");

MStatus cvExpand::doIt( const MArgList& args )
{

	MSelectionList list;
	MSelectionList newList;

	// Get the geometry list from what is currently selected in the 
	// model
	//
	MGlobal::getActiveSelectionList( list );

	MDagPath path;
	MObject  component;
 
	// Make expanded Selection List
	//
	for ( MItSelectionList iter( list ); !iter.isDone(); iter.next() ) {
		iter.getDagPath( path, component );
		
		if ( path.hasFn( MFn::kNurbsSurfaceGeom ) && 
			 !component.isNull() ) {
			for ( MItSurfaceCV cvIter( path, component ); 
				  !cvIter.isDone(); cvIter.next() ) {
				newList.add( path, cvIter.currentItem() );
			}
		} else {
			newList.add( path, component );
		}
	}

	// Return expanded selection list as an array of strings
	//
	MStringArray returnArray;
	newList.getSelectionStrings( returnArray );

	MPxCommand::setResult( returnArray );

	return MS::kSuccess;
}
