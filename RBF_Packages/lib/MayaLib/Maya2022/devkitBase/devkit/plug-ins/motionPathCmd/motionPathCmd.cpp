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
// Produces the MEL command "motionPath".
//
// This plug-in assigns a curve as a motion path to an object.
//
// To use this plug-in:
//	(1) Create an object and draw a curve.
//	(2) Clear all selected items and then pick the object followed by the curve (order is important).
//	(3) Execute "motionPath" in the command window after both are selected.
//	(4) Click the play button. The object will move along the curve. 
// 
// This is a simple example of how to use the motion path function set. 
// 
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MFnMotionPath.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MTime.h>
#include <maya/MPxCommand.h>

class motionPath : public MPxCommand
{
public:
					motionPath() {};
				~motionPath() override; 

	MStatus     	doIt( const MArgList& args ) override;
	static void*	creator();
};

void* motionPath::creator()
{
	return new motionPath();
}

motionPath::~motionPath() {}

MStatus motionPath::doIt( const MArgList& )
{
	MObject 		mdepNode;
	MObject 		component;
	MDagPath		motionCurve;
	MDagPath		objectToAnimate;
	MStatus 		stat = MS::kSuccess;
	MTime			startTime( 0.0 );
	MTime			endTime( 48.0 );

	MSelectionList slist;
	MGlobal::getActiveSelectionList( slist );
	MItSelectionList itr( slist );

	for (; !itr.isDone(); itr.next() )
	{

		if ( !objectToAnimate.isValid() ) {
			itr.getDagPath( objectToAnimate, component );
		}
		else if ( !motionCurve.isValid() ) {
			itr.getDagPath( motionCurve, component );
		}
		else {
			break;
		}
	}

	if ( (objectToAnimate.isValid()) && (motionCurve.isValid()) ) {

		MFnMotionPath fnMotionPath;
		mdepNode = fnMotionPath.create( motionCurve, objectToAnimate, 
									startTime, endTime );
	
// 		MDagPath dagPath = fnMotionPath.pathObject();
// 		MObject pathObj = dagPath.transform();
	}


	return stat;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerCommand( "motionPath", motionPath::creator );
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

	status = plugin.deregisterCommand( "motionPath" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
