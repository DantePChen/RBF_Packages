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

//
// dagMessageCmd.cpp
//
// Description:
//     Sample plug-in that demonstrates how to register/de-register
//     a callback with the MDagMessage class.
//
//     This plug-in will register a new command in maya called
//     "dagMessage" which adds a callback for the all nodes on
//     the active selection list. A message is printed to stdout 
//     whenever a connection is made or broken for those nodes. If
//		nothing is selected, the callback will be for all nodes.
//
//	   dagMessage -help will list the options.
//
#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MArgDatabase.h>
#include <maya/MIntArray.h>
#include <maya/MSelectionList.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MSyntax.h>
#include <maya/MDagMessage.h>
#include <maya/MDGMessage.h>
#include <maya/MModelMessage.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MObjectArray.h>

#define kCmdName					"dagMessage"

#define kAllDagFlag 				"-ad"
#define kAllDagFlagLong				"-allDag"
#define kParentAddedFlag			"-pa"
#define kParentAddedFlagLong		"-parentAdded"
#define kParentRemovedFlag			"-pr"
#define kParentRemovedFlagLong		"-parentRemoved"
#define kChildAddedFlag				"-ca"
#define kChildAddedFlagLong			"-childAdded"
#define kChildRemovedFlag			"-cr"
#define kChildRemovedFlagLong		"-childRemoved"
#define kChildReorderedFlag			"-cro"
#define kChildReorderedFlagLong		"-childReordered"

#define kHelpFlag					"-h"
#define kHelpFlagLong				"-help"

#define CheckErrorContinue(stat, msg)	\
	if (MS::kSuccess != stat) {			\
		displayError(msg);				\
		continue;						\
	}							

#define CheckErrorReturn(stat, msg)		\
	if (MS::kSuccess != stat) {			\
		displayError(msg);				\
		return;							\
	}

// This table will keep track of the registered callbacks
// so they can be removed when the plug-ins is unloaded.
//
MCallbackIdArray callbackIds;

// Node added to model callback.
static void userNodeRemovedCB(MObject& node,void *clientData)
{
	if (! node.isNull()) {
		bool doDisplay = true;

		MStatus status;
		MFnDagNode dagNode(node,&status);
		if ( status.error() ) {
			doDisplay = false;
			MGlobal::displayInfo("Error: failed to get dag node.");
		}

		if ( doDisplay ) {
			MString s = dagNode.name();
			MString info("DAG Model -  Node removed: ");
			info+= s;

			MGlobal::displayInfo(info);	
		}
	}

	// remove the callback
	MCallbackId id = MMessage::currentCallbackId();
	MMessage::removeCallback(id);
}

// Node added to model callback.
static void userNodeAddedCB(MObject& node,void *clientData)
{
	MStatus status;

	if (! node.isNull()) {
		bool doDisplay = true;

		MDagPath path;
		status = MDagPath::getAPathTo(node,path);
		if ( status.error() ) {
			doDisplay = false;
			MGlobal::displayInfo("Error: failed to get dag path to node.");
		}

		if ( doDisplay ) {
			MString s = path.fullPathName();
			MString info("DAG Model -  Node added: ");
			info+= s;

			path.transform(&status);
			if (MS::kInvalidParameter == status) {
				info += "(WORLD)";
			}

			MGlobal::displayInfo(info);	
		}
	}

	// remove the callback
	MCallbackId id = MMessage::currentCallbackId();
	MMessage::removeCallback(id);

	// listen for removal message
	/* MCallbackId id = */ MModelMessage::addNodeRemovedFromModelCallback( node, userNodeRemovedCB, 0, &status );
	if ( status.error() ) {
		MGlobal::displayError("Failed to install node removed from model callback.\n");
		return;
	}
}

// Install a node added callback for the node specified
// by dagPath.
static void installNodeAddedCallback( MDagPath& dagPath )
{
	MStatus status;

	MObject dagNode = dagPath.node();
	if ( dagNode.isNull() )
		return;

	/* MCallbackId id = */ MModelMessage::addNodeAddedToModelCallback( dagNode, userNodeAddedCB, 0, &status );
	if ( status.error() ) {
		MGlobal::displayError("Failed to install node added to model callback.\n");
		return;
	}
}

// Decide if the dag is in the model. Dag paths and names
// may not be setup if the dag has not been added to
// the model.
static bool dagNotInModel( MDagPath& dagPath )
{
	MStatus status;
	MFnDagNode dagFn( dagPath, &status );
	if ( status.error() )
		return false;
	bool inModel = dagFn.inModel( &status );
	if ( status.error() )
		return false;
	return ( inModel == false );
}

void userDAGGenericCB(MDagMessage::DagMessage msg, MDagPath &child,
					  MDagPath &parent, void *)
{	
	MString dagStr("DAG Changed - ");
	switch (msg) {
		case MDagMessage::kParentAdded:
			dagStr += "Parent Added: ";
			break;
		case MDagMessage::kParentRemoved:
			dagStr += "Parent Removed: ";
			break;
		case MDagMessage::kChildAdded:
			dagStr += "Child Added: ";
			break;
		case MDagMessage::kChildRemoved:
			dagStr += "Child Removed: ";
			break;
		case MDagMessage::kChildReordered:
			dagStr += "Child Reordered: ";
			break;
		default:
			dagStr += "Unknown Type: ";
			break;
	}

	dagStr += "child = ";
	dagStr += child.fullPathName();
	dagStr += ", parent = ";
	dagStr += parent.fullPathName();

	// Check to see if the parent is the world object.
	//
	MStatus pStat;
	parent.transform(&pStat);
	if (MS::kInvalidParameter == pStat) {
		dagStr += "(WORLD)";
	}

	// Install callbacks if node is not in the model.
	// Callback is for node added to model.
	bool incomplete = false;
	if ( dagNotInModel( child ) ) {
		installNodeAddedCallback( child );
		incomplete = true;
	}
	if ( dagNotInModel( parent ) ) {
		installNodeAddedCallback( parent);
		incomplete = true;
	}

	// Warn user that dag path info may be
	// incomplete
	if (incomplete)
		dagStr += "\t// May be incomplete!";	

	MGlobal::displayInfo(dagStr);
}


//////////////////////////////////////////////////////////////////////////
//
// Command class declaration
//
//////////////////////////////////////////////////////////////////////////

class dagMessageCmd : public MPxCommand
{
public:
					dagMessageCmd() {};
				~dagMessageCmd() override; 
	MStatus			doIt( const MArgList& args ) override;

	static MSyntax	newSyntax();
	static void*	creator();

private:

	MStatus			addGenericCallback(MDagPath *dagPath, 
									   MDagMessage::DagMessage msg, 
									   MString cbName);	
};

//////////////////////////////////////////////////////////////////////////
//
// Command class implementation
//
//////////////////////////////////////////////////////////////////////////

dagMessageCmd::~dagMessageCmd() {}

void* dagMessageCmd::creator()
{
	return new dagMessageCmd();
}

MSyntax dagMessageCmd::newSyntax()
{
	MSyntax syntax;

	syntax.useSelectionAsDefault(true);
	syntax.setObjectType(MSyntax::kSelectionList);
	syntax.setMinObjects(0);

	syntax.addFlag(kAllDagFlag, kAllDagFlagLong);
	syntax.addFlag(kParentAddedFlag, kParentAddedFlagLong);
	syntax.addFlag(kParentRemovedFlag, kParentRemovedFlagLong);
	syntax.addFlag(kChildAddedFlag, kChildAddedFlagLong);
	syntax.addFlag(kChildRemovedFlag, kChildRemovedFlagLong);
	syntax.addFlag(kChildReorderedFlag, kChildReorderedFlagLong);
	syntax.addFlag(kHelpFlag, kHelpFlagLong);
	return syntax;
}

MStatus dagMessageCmd::addGenericCallback(MDagPath *dagPath, 
										  MDagMessage::DagMessage msg, 
										  MString cbName)
{
	MStatus status = MS::kFailure;

	if (NULL == dagPath) {
		MCallbackId id = MDagMessage::addDagCallback(	msg,
														userDAGGenericCB, 
														NULL, 
														&status);
		if (MS::kSuccess == status) {
			MString info("Adding a callback for");
			info += cbName;
			info += "on all nodes";
			MGlobal::displayInfo(info);			
			callbackIds.append( id );
		} else {
			MString err("Could not add callback to");
			err += dagPath->fullPathName();
			MGlobal::displayError(err);
		}
	} else {
		MCallbackId id = MDagMessage::addDagDagPathCallback(*dagPath,
													msg,
													userDAGGenericCB, 
													NULL, 
													&status);
		if (MS::kSuccess == status) {
			MString info("Adding a callback for");
			info += cbName;
			info += "on ";
			info += dagPath->fullPathName();
			MGlobal::displayInfo(info);			
			callbackIds.append( id );
		} else {
			MString err("Could not add callback to");
			err += dagPath->fullPathName();
			MGlobal::displayError(err);
		}
	}

	return status;
}

MStatus dagMessageCmd::doIt( const MArgList& args)
//
// Takes the  nodes that are on the active selection list and adds an
// attriubte changed callback to each one.
//
{
	MStatus 		status;
	MSelectionList 	list;
    MArgDatabase argData(syntax(), args);

	status = argData.getObjects(list);
	if (MS::kSuccess != status) {
		MGlobal::displayError("Error getting objects");
		return status;
	}

	//	Get the flags
	//
	bool allDagUsed = argData.isFlagSet(kAllDagFlag);
	bool parentAddedUsed = argData.isFlagSet(kParentAddedFlag);
	bool parentRemovedUsed = argData.isFlagSet(kParentRemovedFlag);
	bool childAddedUsed = argData.isFlagSet(kChildAddedFlag);
	bool childRemovedUsed = argData.isFlagSet(kChildRemovedFlag);
	bool childReorderedUsed = argData.isFlagSet(kChildReorderedFlag);
	bool helpUsed = argData.isFlagSet(kHelpFlag);

	bool nothingSet = (	!allDagUsed && !parentAddedUsed && 
						!parentRemovedUsed && !childAddedUsed && 
						!childRemovedUsed && !childReorderedUsed && 
						!helpUsed);

	if (nothingSet) {
		MGlobal::displayError("A flag must be used. dagMessage -help for availible flags.");
		return MS::kFailure;
	}

	if (argData.isFlagSet(kHelpFlag)) {
		MGlobal::displayInfo("dagMessage -help");
		MGlobal::displayInfo("\tdagMessage adds a callback to the selected nodes,");
		MGlobal::displayInfo("\tor if no nodes are selected, to all nodes. The callback");
		MGlobal::displayInfo("\tprints a message when called. When the plug-in is unloaded");
		MGlobal::displayInfo("\tthe callbacks are removed.");
		MGlobal::displayInfo("");
		MGlobal::displayInfo("\t-h -help : This message is printed");
		MGlobal::displayInfo("\t-ad -allDag : parent changes and child reorders");
		MGlobal::displayInfo("\t-pa -parentAdded : A parent is added");
		MGlobal::displayInfo("\t-pr -parentRemoved : A parent is removed");
		MGlobal::displayInfo("\t-ca -childAdded : A child is added (only for individual nodes)");
		MGlobal::displayInfo("\t-cr -childRemoved : A child is removed (only for individual nodes)");
		MGlobal::displayInfo("\t-cro -childReordered : A child is reordered");
		MGlobal::displayInfo("");
	}

	unsigned nObjs = list.length();
	if (nObjs == 0) {
		//	Add the callback for all changes of the specified type.
		//
		if (allDagUsed) {
			MCallbackId id = MDagMessage::addAllDagChangesCallback(userDAGGenericCB, NULL, &status);
			if (status) {
				callbackIds.append( id );
				MGlobal::displayInfo("Added a callback for all Dag changes on all nodes.\n");
			} else {
				MGlobal::displayError("Could not add a -allDag callback");
				return status;
			}
		}

		if (parentAddedUsed) {
			status = addGenericCallback(NULL, 
										MDagMessage::kParentAdded,
										MString(" parent added "));
			if (MS::kSuccess != status) {
				return status;
			}
		}

		if (parentRemovedUsed) {
			status = addGenericCallback(NULL, 
										MDagMessage::kParentRemoved,
										MString(" parent removed "));
			if (MS::kSuccess != status) {
				return status;
			}
		}		

		if (childAddedUsed) {
			MGlobal::displayError("-childAdded can only be used when a node is selected");
			status = MS::kFailure;
			return status;
		}

		if (childRemovedUsed) {
			MGlobal::displayError("-childRemoved can only be used when a node is selected");
			status = MS::kFailure;
			return status;
		}	

		if (childReorderedUsed) {
			status = addGenericCallback(NULL, 
										MDagMessage::kChildReordered,
										MString(" child reordered "));
			if (MS::kSuccess != status) {
				return status;
			}
		}	
	} else {
		for (unsigned int i=0; i< nObjs; i++) {
			MDagPath dagPath;
			list.getDagPath(i, dagPath);

			if (!dagPath.isValid()) {
				continue;
			}

			//	Add the callback for all changes of the specified type.
			//
			if (allDagUsed) {
				MCallbackId id = MDagMessage::addAllDagChangesDagPathCallback(dagPath, userDAGGenericCB, NULL, &status);
				if (status) {
					callbackIds.append( id );
					MString infoStr("Added a callback for all Dag changes on ");
					infoStr += dagPath.fullPathName();
					MGlobal::displayInfo(infoStr);
				} else {
					MGlobal::displayError("Could not add a -allDag callback");
					return status;
				}
			}

			if (parentAddedUsed) {
				status = addGenericCallback(&dagPath, 
											MDagMessage::kParentAdded,
											MString(" parent added "));
				if (MS::kSuccess != status) {
					return status;
				}
			}

			if (parentRemovedUsed) {
				status = addGenericCallback(&dagPath, 
											MDagMessage::kParentRemoved,
											MString(" parent removed "));
				if (MS::kSuccess != status) {
					return status;
				}
			}		

			if (childAddedUsed) {
				status = addGenericCallback(&dagPath, 
											MDagMessage::kChildAdded,
											MString(" child added "));
				if (MS::kSuccess != status) {
					return status;
				}
			}

			if (childRemovedUsed) {
				status = addGenericCallback(&dagPath, 
											MDagMessage::kChildRemoved,
											MString(" child removed "));
				if (MS::kSuccess != status) {
					return status;
				}
			}	

			if (childReorderedUsed) {
				status = addGenericCallback(&dagPath, 
											MDagMessage::kChildReordered,
											MString(" child reordered "));
				if (MS::kSuccess != status) {
					return status;
				}
			}	
		}
	}
	
	return status;
}

//////////////////////////////////////////////////////////////////////////
//
// Plugin registration
//
//////////////////////////////////////////////////////////////////////////

MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	return plugin.registerCommand( kCmdName,
									dagMessageCmd::creator,
									dagMessageCmd::newSyntax);
}

MStatus uninitializePlugin( MObject obj)
{
	// Remove callbacks
	//
	for (unsigned int i=0; i<callbackIds.length(); i++ ) {
		MMessage::removeCallback( callbackIds[i] );
	}

	MFnPlugin plugin( obj );
	return plugin.deregisterCommand( kCmdName );
}


