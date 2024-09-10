//-
// ===========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

/* 

	file : cameraMessageCmd.cpp
	class: cameraMessageCmd
	----------------------
	
	This plugin demonstrates each of the camera manipulation callbacks available
	in the Maya API.  Callbacks are added to nodes by invoking the command:

		cameraMessageCmd <node 1> [<node 2> ...]
	
	This command will register 2 callbacks on the nodes.

	1) MNodeMessage::addBeginManipulationCallback is used to register a camera
	   manipulation beginning callback on the nodes.  This callback is executed
	   once an interactive manipulation of a camera is beginning, such as tumble
	   or zoom.  
	
	1) MNodeMessage::addEndManipulationCallback is used to register a camera
	   manipulation ending callback on the nodes.  This callback is executed
	   once an interactive manipulation of a camera is completed.

	Here is an example of the expected behavior of the callbacks registered on 
	"perspShape", and is then manipulated.

	After loading this plugin, run:
	    select perspShape;
        cameraMessageCmd;
	
	The following messages will be displayed:

        // Adding payload 12345.5 to beginManipulation callback for node perspShape // 
        // Adding payload 54321.5 to endManipulation callback for node perspShape // 
	
	When beginning to tumble a camera, this message will be displayed:
	
        // Beginning-of-Manipulation callback for node: perspShape // 
        // Found Payload: 12345.5 // 
        
	When the tumble of a camera is finished, this message will be displayed:

		// End-of-Manipulation callback for node: perspShape // 
        // Found Payload: 54321.5 // 

*/

#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>
#include <maya/MPxCommand.h>
#include <maya/MMessage.h>
#include <maya/MCameraMessage.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MSyntax.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MCallbackIdArray.h>

// Syntax string definitions

class cameraMessageCmd : public MPxCommand
{
	public:
		MStatus doIt( const MArgList& ) override;

		static void *creator();
		static MSyntax newSyntax();

		static void removeCallbacks();

		// callback functions.
		static void beginManipulationCB(MObject& node, void* clientData);	
		static void endManipulationCB(MObject& node, void* clientData);

	private:
		static MCallbackIdArray callbackIds;
		static bool nodeRemovedCBRegistered;
};

MCallbackIdArray cameraMessageCmd::callbackIds;
bool cameraMessageCmd::nodeRemovedCBRegistered = false;

MStatus cameraMessageCmd::doIt( const MArgList& args )
{
	MStatus status = MS::kSuccess;

	MArgDatabase argData(syntax(), args);
	MSelectionList objects;
	argData.getObjects(objects);

	for (unsigned int i = 0; i < objects.length(); i++) {
		MObject node;
		objects.getDependNode(i, node);

		double* payloadBegin = new double(12345.5);
		MDagPath dagPath;
		status = objects.getDagPath(i, dagPath);
		MGlobal::displayInfo( (MString("Adding payload ") + *payloadBegin) + MString(" to beginManipulation callback for node ") + dagPath.partialPathName());
		callbackIds.append(
			MCameraMessage::addBeginManipulationCallback (node, beginManipulationCB, payloadBegin, &status)
		);
		if (!status) {
			MGlobal::displayWarning("Could not attach beginManipulation callback for node.");
			continue;
		}

		double* payloadEnd = new double(54321.5);
		MGlobal::displayInfo( (MString("Adding payload ") + *payloadEnd) + MString(" to endManipulation callback for node ") + dagPath.partialPathName());
		//  Add some user data to this callback, 
		callbackIds.append(
			MCameraMessage::addEndManipulationCallback (node, endManipulationCB, payloadEnd, &status)
		);
		if (!status) {
			MGlobal::displayWarning("Could not attach endManipulation callback for node.");
			continue;
		}
	}
	
	return status;
}


void* cameraMessageCmd::creator() 
{	
	return new cameraMessageCmd;
}

MSyntax cameraMessageCmd::newSyntax() {
	MSyntax syntax;
	syntax.setObjectType(MSyntax::kSelectionList);
	syntax.setMinObjects(1);
	syntax.useSelectionAsDefault(true);
	return syntax;
}

void cameraMessageCmd::beginManipulationCB(MObject& node, void* clientData) {
	MFnDependencyNode nodeFn(node);
	MGlobal::displayInfo(MString("Beginning-of-Manipulation callback for node: ") + nodeFn.name());

	double* payloadA = static_cast<double*>(clientData);
	if(payloadA)
	{
		MGlobal::displayInfo(MString("Found Payload: ") + (*payloadA));
	}
}

void cameraMessageCmd::endManipulationCB(MObject& node, void* clientData) {
	MFnDependencyNode nodeFn(node);
	MGlobal::displayInfo(MString("End-of-Manipulation callback for node: ") + nodeFn.name());

	double* payloadB = static_cast<double*>(clientData);
	if(payloadB)
	{
		MGlobal::displayInfo(MString("Found Payload: ") + (*payloadB));
	}
}

void cameraMessageCmd::removeCallbacks() {
	MMessage::removeCallbacks(callbackIds);
}

// standard initialize and uninitialize functions

MStatus initializePlugin(MObject obj)
{
	MFnPlugin pluginFn(obj, PLUGIN_COMPANY, "6.0");

	MStatus status;
	status = pluginFn.registerCommand("cameraMessageCmd", cameraMessageCmd::creator, cameraMessageCmd::newSyntax);

	if( !status)
		status.perror("register Command failed");

	return status;
}


MStatus uninitializePlugin ( MObject obj )
{
	MFnPlugin pluginFn(obj);
	MStatus status = MS::kSuccess;

	cameraMessageCmd::removeCallbacks();

	status = pluginFn.deregisterCommand("cameraMessageCmd");

	return status;
}

