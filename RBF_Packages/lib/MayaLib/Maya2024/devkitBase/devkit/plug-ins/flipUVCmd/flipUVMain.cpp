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

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// Produces the command "flipUV".
//
// This is a simple plug-in for demonstrating the "MPxPolyTweakUVCommand" class to manipulate UVs.
// The syntax of the command is:
//
//	flipUV [-es on|off] [-fg on|off] [-h on|off]
//
// Flags:
//
//	-es -extendToShell on|off 
//	-fg -flipGlobal on|off 
//	-h -horizontal on|off
// 
// To use this plug-in:
//
//	(1) Select UVs from a mesh.
//	(2) Invoke the "flipUV" command. 
// 
// Depending on the type of texture used, results might not show up in the modelling windows.
// To see the result, use the UV Editor.
//
////////////////////////////////////////////////////////////////////////

#include "flipUVCmd.h"

#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>

#ifdef WIN32
#define EXTERN_DECL __declspec( dllexport )
#else
#define EXTERN_DECL extern
#endif

EXTERN_DECL MStatus initializePlugin( MObject obj );
EXTERN_DECL MStatus uninitializePlugin( MObject obj );

///////////////////////////////////////////////////////////////////////////////
//
// Define the 2 main hooks for a maya plugin.
//
///////////////////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
	MStatus status;

	// Init the vendor string etc...
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");

	status = plugin.registerCommand(flipUVCmd::cmdName,
									flipUVCmd::creator, 
									flipUVCmd::newSyntax);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	
	return MS::kSuccess;
}

//
///////////////////////////////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj)
{
	MStatus status;

	MFnPlugin plugin( obj );

	status = plugin.deregisterCommand(flipUVCmd::cmdName);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MS::kSuccess;
}
