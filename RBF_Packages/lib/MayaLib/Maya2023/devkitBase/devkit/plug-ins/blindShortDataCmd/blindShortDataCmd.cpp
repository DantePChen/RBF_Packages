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
// This plug-in produces the MEL command "blindShortData".
// 
// This example adds a dynamic attribute to the dependency nodes of each of the currently selected items.
// The attribute is a short and its default value is set to "99".
// 
// To use this plug-in:
//	Select an object, bring up the Attribute Editor (select Window > Attribute Editor), and then click on the "Extras" tab.
//	The attribute editor should display no extra attributes.
//	Now, execute "blindShortData" in the command window. An attribute appears in the editor set to the value of 99.
//  The value can be modified in the editor, or with the MEL commands "setAttr" and "getAttr".
//  When the scene is saved, the new attribute and value for each selected item are also saved.

// This example demonstrates how simple blind data can be attached to an object. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include <maya/MArgList.h>

#include <maya/MPxCommand.h>

#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>

#include <maya/MFnDependencyNode.h>
#include <maya/MFnNumericAttribute.h>

class blindShortData : public MPxCommand
{
public:
					blindShortData() {};
				~blindShortData() override; 

	MStatus			doIt( const MArgList& args ) override;
	MStatus	redoIt() override;
	static void*	creator();
};

blindShortData::~blindShortData() {}

void* blindShortData::creator()
{
	return new blindShortData();
}

MStatus blindShortData::doIt( const MArgList& )
{
	MStatus stat = redoIt();
	return stat;
}

MStatus blindShortData::redoIt()
//     This method performs the action of the command.
//     
//     Iterate over all selected items and for each attach a new
//		dynamic attribute. 
{
	MStatus stat;			// Status code
	MObject dependNode;		// Selected dependency node

	// Create a selection list iterator
	//
	MSelectionList slist;
	MGlobal::getActiveSelectionList( slist );
	MItSelectionList iter( slist, MFn::kInvalid, &stat );

	// Iterate over all selected dependency nodes
	//
	for ( ; !iter.isDone(); iter.next() ) 
	{
		// Get the selected dependency node and create
		// a function set for it
		//
		if ( MS::kSuccess != iter.getDependNode( dependNode ) ) {
			cerr << "Error getting the dependency node" << endl;
			continue;
		}
		MFnDependencyNode fnDN( dependNode, &stat );
		if ( MS::kSuccess != stat ) {
			cerr << "Error creating MFnDependencyNode" << endl;
			continue;
		}

		// Create a new attribute to use as out blind data
		//
		MFnNumericAttribute fnAttr;
		const MString fullName( "blindData" );
		const MString briefName( "bd" );
		double attrDefault = 99;
		MObject newAttr = fnAttr.create( 	fullName, briefName, 
											MFnNumericData::kShort, 
											attrDefault, &stat );
		if ( MS::kSuccess != stat ) {
			cerr << "Error creating new attribute" << endl;
			continue;
		}

		// Now add the new attribute to this dependency node
		//
		stat = fnDN.addAttribute(newAttr);
		if ( MS::kSuccess != stat ) {
			cerr << "Error adding dynamic attribute" << endl;
		}

	}

	return MS::kSuccess;
}


//////////////////////////////////////////////////////////////////////////////
//
// The following routines are used to register/unregister
// the command we are creating within Maya
//
//////////////////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerCommand( "blindShortData",
									 blindShortData::creator );
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

	status = plugin.deregisterCommand( "blindShortData" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

	return status;
}
