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
// Produces the MEL command "zoomCamera".
//
// This is a simple plug-in which divides the horizontal field of view for
// the current active camera by 2.0. It is a good example of getting the
// current active view and of modifying the camera.
//
// To use this plug-in:
//	(1) First create a camera by selecting the menu item Create > Cameras > Camera.
//	(2) Position the camera to "look at" an object in the scene.
//	(3) Select the menu item Panels > Perspective > Camera Name to look through the camera.
//	(4) Execute "zoomCamera" in the command window. Your view through the camera will
//		zoom-in by a factor of 2. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>

#include <maya/MFnCamera.h>
#include <maya/MGlobal.h>
#include <maya/M3dView.h>
#include <maya/MDagPath.h>


class zoomCamera : public MPxCommand
{
public:
					zoomCamera() {};
				~zoomCamera() override; 

	MStatus			doIt( const MArgList& args ) override;
	MStatus			redoIt( ) override;
	MStatus			undoIt( ) override;
	bool			isUndoable( ) const override;
	
	static void*	creator();

private:
	// This is data that is necessary to redo/undo the command.
	//
	MDagPath		camera;
};

zoomCamera::~zoomCamera() {}

void* zoomCamera::creator()
{
	return new zoomCamera();
}

MStatus zoomCamera::doIt( const MArgList& )
//
// Description
//     Gets the zoomCamera for the current 3d view and calls
//     the redoIt command to set the focal length.
//
// Note
//     The doit method should collect whatever information is
//     required to do the task, and store it in local class data.
//     It should finally call redoIt to make the command happen.
//
{

	// Get the current zoomCamera
	//
	MStatus stat = M3dView::active3dView().getCamera( camera );	
	
	if ( MS::kSuccess == stat ) {
		redoIt();
	}
	else {
 		cerr << "Error getting camera" << endl;
 	}
	return stat;
}

MStatus zoomCamera::redoIt()
//
// Description
//     Doubles the focal length of current camera
//
// Note
//     The redoIt method should do the actual work, based on the
//     internal data only.
//
{
	MFnCamera fnCamera( camera );
	double fl = fnCamera.focalLength();
	fnCamera.setFocalLength( fl * 2.0 );
	return MS::kSuccess;
}

MStatus zoomCamera::undoIt()
//
// Description
//     the undo routine
//
// Note
//     The undoIt method should undo the actual work, based on the
//     internal data only.
//
{
	MFnCamera fnCamera( camera );
	double fl = fnCamera.focalLength();
	fnCamera.setFocalLength( fl / 2.0 );
	return MS::kSuccess;
}

bool zoomCamera::isUndoable() const
//
// Description
//     Make the command eligable for undo.
//
{ 
	return true;
}


//
// The following routines are used to register/unregister
// the command we are creating within Maya
//
MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerCommand( "zoomCamera", zoomCamera::creator );
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

	status = plugin.deregisterCommand( "zoomCamera" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
