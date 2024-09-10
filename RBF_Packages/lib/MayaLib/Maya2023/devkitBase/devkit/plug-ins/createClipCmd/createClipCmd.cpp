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
// Produces the MEL command "createClip".
// 
// This example demonstrates the steps for creating a nonlinear animation clip
// using the API. The clip is created either on the selected items or on a character
// set node that is supplied using the -c/-char flag. The plug-in creates the animation
// curves that make up the clip, places them in a source clip, and then instances the
// source clip twice.
//
// To use this plug-in:
//	(1) Create a sphere and select it.
//	(2) Execute the command:
//		createClip
//	(3) Open the Maya Trax Editor to see the clips in place on the timeline. 
//
////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <maya/MPxCommand.h>
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MObjectArray.h>
#include <maya/MPlugArray.h>
#include <maya/MFnCharacter.h>
#include <maya/MFnClip.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MDGModifier.h>

#include <maya/MIOStream.h>

#define CheckError(stat,msg)		\
	if ( MS::kSuccess != stat ) {	\
		displayError(msg);			\
		continue;					\
	}


class createClip : public MPxCommand
{
public:
                createClip();
        ~createClip() override;

	MStatus		parseArgs( const MArgList& args );
    MStatus     doIt ( const MArgList& args ) override;
    MStatus     redoIt () override;
    MStatus     undoIt () override;
    bool        isUndoable() const override;

    static      void* creator();

private:
	MObject		fCharacter;
	MDGModifier fMod;
};

createClip::createClip()
{
}

createClip::~createClip() {}

void* createClip::creator()
{
    return new createClip;
}

bool createClip::isUndoable() const
{
    return true;
}

MStatus createClip::undoIt()
{
	MStatus status = fMod.undoIt();
    return status;
}

MStatus createClip::parseArgs( const MArgList& args )
//
// No arguments to parse.
//
{
	MStatus     	stat = MS::kSuccess;
	MString     	arg;
	MSelectionList	list;
	bool			charNameUsed = 0;
	MString			charName;
	const MString	charFlag			("-c");
	const MString	charFlagLong		("-char");

	// Parse the arguments.
	for ( unsigned int i = 0; i < args.length(); i++ ) {
		arg = args.asString( i, &stat );
		if (!stat)              
			continue;
				
		if ( arg == charFlag || arg == charFlagLong ) {
			// get the char name
			//
			if (i == args.length()-1) {
				arg += ": must specify a character name";
				displayError(arg);
				return MS::kFailure;
			}
			i++;
			args.get(i, charName);
			list.add(charName);
			charNameUsed = 1;
		}
		else {
			arg += ": unknown argument";
			displayError(arg);
			return MS::kFailure;
		}
	}

	if (charNameUsed) {
		// get the character corresponding to the node name
		//
		MItSelectionList iter (list);
		for ( /* nothing */ ; !iter.isDone(); iter.next() ) {
			MObject node;
			iter.getDependNode(node);
			if (node.apiType() == MFn::kCharacter) {
				fCharacter = node;
				break;
			}
		}
		if (fCharacter.isNull()) {
			MString errMsg("Character flag must specify a character node.");
			displayError(errMsg);
			return MS::kFailure;
		}
	}
	
	return stat;
}


MStatus createClip::doIt( const MArgList& args )
{
	// parse the command arguments
	//
	MStatus stat = parseArgs(args);
	if (stat != MS::kSuccess) {
		return stat;
	}
	
	unsigned int count = 0;

	// if the character flag was used, create the clip on the specified
	// character, otherwise, create a character
	//
	MFnCharacter fnCharacter;
	if (fCharacter.isNull()) {
		MSelectionList activeList;
		MGlobal::getActiveSelectionList (activeList);
		if (0 == activeList.length()) {
			MString errMsg("No character was specified, and no objects were selected.");
			displayError(errMsg);
			return MS::kFailure;
		}
		// create a character using the selection list
		//
		fCharacter = fnCharacter.create(activeList,MFnSet::kNone,&stat);
		if (stat != MS::kSuccess) {
			MString errMsg("Failed to create character using the selection.");
			displayError(errMsg);
			return MS::kFailure;
		}
	} else {
		fnCharacter.setObject(fCharacter);
	}

	// Get the array of members of the character. We will create a clip
	// for them.
	//
	MPlugArray plugs;
	fnCharacter.getMemberPlugs(plugs);

	// Now create a animCurves to use as a clip for the character.
	// The curves will be set up between frames 0 and 10;
	//
	MTime start(0.0);
	MTime duration(10.0);
	MObjectArray clipCurves;
	
	for (count = 0; count < plugs.length(); ++count) {
		// Now create a bunch of animCurves to use as a clip for the
		// character
		//
		MFnAnimCurve fnCurve;
//		AnimCurveType plugType = fnCurve.timedAnimCurveTypeForPlug(plugs[count]);
		MObject curve = fnCurve.create(MFnAnimCurve::kAnimCurveTL); // plugType);
		fnCurve.addKeyframe(start,5.0);
		fnCurve.addKeyframe(duration,15.0);
		clipCurves.append(curve);
	}
	
	// Create a source clip node and add the animation to it
	//
	MFnClip fnClipCreate;
	MObject sourceClip = fnClipCreate.createSourceClip(start,duration,fMod,&stat);
	fnCharacter.attachSourceToCharacter(sourceClip,fMod);	
	for (count = 0; count < plugs.length(); ++count) {
		MPlug animPlug = plugs[count];
		fnCharacter.addCurveToClip(clipCurves[count],sourceClip,animPlug,fMod);
	}

	// instance the clip
	//
	MTime schedStart(15.0);
	MObject instancedClip = fnClipCreate.createInstancedClip(sourceClip,
															 schedStart,
															 fMod,
															 &stat);
	fnCharacter.attachInstanceToCharacter(instancedClip,fMod);

	// instance the clip a second time, at time 30
	//
	schedStart.setValue(30.0);
	MObject instancedClip2 = fnClipCreate.createInstancedClip(sourceClip,
															  schedStart,
															  fMod,
															  &stat);
	fnCharacter.attachInstanceToCharacter(instancedClip2,fMod);

	return stat;
}

MStatus createClip::redoIt()
{
    MStatus status = fMod.doIt();
    return status;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.0", "Any");

    status = plugin.registerCommand( "createClip", createClip::creator );
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

    status = plugin.deregisterCommand( "createClip" );
	if (!status) {
		status.perror("deregisterCommand");
	}
    return status;
}
