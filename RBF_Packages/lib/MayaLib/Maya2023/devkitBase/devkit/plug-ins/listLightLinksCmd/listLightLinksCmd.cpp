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
// Produces the MEL command "listLightLinks". 
// 
// This example demonstrates how to use the MLightLinks class to query
// the light linking information in Maya. The command takes no arguments.
// If the currently selected object is a light, then the command will
// select all objects illuminated by that light. If the currently
// selected object is a piece of geometry, then the command will select
// all the lights that illuminate that geometry. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MSimple.h>
#include <maya/MLightLinks.h>
#include <maya/M3dView.h>
#include <maya/MDagPathArray.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MStringArray.h>
#include <maya/MItSelectionList.h>
#include <math.h>

DeclareSimpleCommand( listLightLinks, PLUGIN_COMPANY, "4.5");

MStatus listLightLinks::doIt( const MArgList& args )
//
//  Description:
//      Implements the MEL listLightLinks command.  After parsing the 
//      information stored in Maya's light linker nodes, it examines
//      the first item on the selection list.  If the item is an object,
//      then the command selects all lights that are linked to that object.
//      If the item is a light, then it will select all of the objects 
//      that are linked to that light.
//
//  Arguments:
//      args - The argument list that was passed to the command from MEL.
//             This command takes no arguments.
//
//  Return Value:
//      MS::kSuccess - command succeeded
//      MS::kFailure - command failed (returning this value will cause the 
//                     MEL script that is being run to terminate unless the
//                     error is caught using a "catch" statement).
//
{
	MStatus stat = MS::kSuccess;
	clearResult();

	// Parse the links on the current scene's light linker node(s).
	//
	MLightLinks lightLink;
	bool parseStatus;
	parseStatus = lightLink.parseLinks(MObject::kNullObj);

	if( !parseStatus )
	{
		setResult( "Error parsing light links\n" );
		return MS::kFailure;
	}

	// Get the first object (or component) on the selection list.
	//
	MSelectionList selectList;
    MDagPath dagPath;
    MObject component;
	MGlobal::getActiveSelectionList( selectList );
	selectList.getDagPath( 0, dagPath, component );
	dagPath.extendToShape();

	// Selection list to store entities linked to the selected light or
	// object.
	//
    MSelectionList newSelection;
    newSelection.clear();

	// Stores the command result.
	//
	char resultString[512];

	// If the object is a surface, we'll select all the lights linked to it.
	// If the object is a light, we'll select all the objects linked to it.
	//
	if( dagPath.hasFn( MFn::kLight ) )
	{
		// Select objects linked to this light.
		//
		MSelectionList objects;
		objects.clear();
		lightLink.getLinkedObjects( dagPath, objects );

		newSelection.merge( objects );

		sprintf( resultString, "Selecting objects linked to light %s", 
				 dagPath.fullPathName().asChar() );

	}
	else
	{
		// Select lights linked to this object.
		//
		MDagPathArray lights;
		lights.clear();
		lightLink.getLinkedLights( dagPath, component, lights );

		for( unsigned int j = 0; j < lights.length(); j++ )
		{
			const MDagPath& path = lights[j];
			newSelection.add( path );
		}

		sprintf( resultString, "Selecting lights linked to object %s", 
				 dagPath.fullPathName().asChar() );

	} 

	// Select the linked entities.
	//
	MGlobal::setActiveSelectionList( newSelection );

	setResult( resultString );
	return stat;
}




