//-
// ===========================================================================
// Copyright 2015 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

//
// paintCallback.cpp
//
// Description:
//     Sample plug-in that demonstrates how to register/de-register
//     a callback with the MPaintMessage class.
//
//     This plug-in will register a new command in maya called
//     "paintCallback" which adds a paint callback. 
//	   A message is printed to the script editor window
//     whenever a vertex color paint stamp occurs.
//
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MFnDoubleIndexedComponent.h>
#include <maya/MIntArray.h>
#include <maya/MColorArray.h>
#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MPaintMessage.h>

#include <sstream>

// This will keep track of the registered callback
// so it can be removed when the plug-in is unloaded.
//
MCallbackId callbackId = 0;

//////////////////////////////////////////////////////////////////////////
//
// Callback function
//
//    Prints out stamp information when a vertex paint callback is received.
//
//////////////////////////////////////////////////////////////////////////

void userCB( MDagPath& path, MObject& object, MPlug& plug, MColorArray& colors, void* clientData )
{
	std::ostringstream str;
	str << path.fullPathName() << " ";
	str << plug.info() << std::endl;
	if ( object.hasFn( MFn::kMeshVtxFaceComponent ) ) {
		MFnDoubleIndexedComponent comp( object );
		MIntArray us, vs;
		comp.getElements( us, vs );
		unsigned int count = comp.elementCount();
		str << "[";
		for ( unsigned int i=0; i<count; ++i ) {
			str << "(" << us[i] << "," << vs[i] << ") ";
		}
		str << "]" << std::endl;
	}
	else {
		MFnSingleIndexedComponent comp( object );
		unsigned int count = comp.elementCount();
		str << "[";
		for ( unsigned int i=0; i<count; ++i ) {
			str << comp.element( i ) << " ";
		}
		str << "]" << std::endl;
	}
	str << colors << std::endl;
	MGlobal::displayInfo( str.str().c_str() );
}

//////////////////////////////////////////////////////////////////////////
//
// Command class declaration
//
//////////////////////////////////////////////////////////////////////////

class paintCallbackCmd : public MPxCommand
{
public:
					paintCallbackCmd() {};
				~paintCallbackCmd() override; 
	MStatus			doIt( const MArgList& args ) override;
	static void*	creator();
};

//////////////////////////////////////////////////////////////////////////
//
// Command class implementation
//
//////////////////////////////////////////////////////////////////////////

paintCallbackCmd::~paintCallbackCmd()
{
}

void* paintCallbackCmd::creator()
{
	return new paintCallbackCmd();
}

MStatus paintCallbackCmd::doIt( const MArgList& )
//
// Adds an MPaintMessage callback.
//
{
	if ( callbackId ) {
		return MS::kSuccess;
	}

	MStatus stat;
	MCallbackId id = MPaintMessage::addVertexColorCallback( userCB, NULL, &stat );

	if ( stat ) {
		callbackId = id;
    } else {
	    MGlobal::displayError( "MPaintMessage::addVertexColorCallback failed" );
    }
        
	return stat;
}

//////////////////////////////////////////////////////////////////////////
//
// Plugin registration
//
//////////////////////////////////////////////////////////////////////////

MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	return plugin.registerCommand( "paintCallback", paintCallbackCmd::creator );
}

MStatus uninitializePlugin( MObject obj)
{
	// Remove all callbacks
	//
	if ( callbackId ) {
		MMessage::removeCallback( callbackId );
	}

	MFnPlugin plugin( obj );
	return plugin.deregisterCommand( "paintCallback" );
}
