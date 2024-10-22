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

// lockEvent.cpp 
// 
// This plug-in demonstrates the API callbacks for node and plug locking. 
// These callbacks allow you to receive notification when the locked 
// status of a plug or node is queried internally. The API programmer has 
// option, upon receipt of the callback, to 'override' the lock state of 
// node or plug.  This is 'override' is controlled via a 'decision' variable 
// passed into the callback function.  The variable can hold two values 
//
//  1) decision = true  --> You want to accept the lock state and do  
//     whatever the internal default behavior is. 
//  2) decision = false --> You want to deny the lock state and do the
//     opposite of what Maya would usually do. 
//
// The flow of execution would be as follows ... 
//
// 1) Received a callback from Maya.  
// 2) What kind of event is this?
// 3) Do I want to allow this event?
// |-- 4) Yes, I do not want to OVERRIDE this event. decision = true.
// |-- 4) No, I want to OVERRIDE this event. decision = false.
// 5) Return from callback.  
//
//
// Example usage: 
//   sphere ; 
//
//   // Watch the translateX plug on the sphere we just created ...
//   lockEvent -a 3 nurbsSphere1.translateX; 
//   // Do not allow any changes to the plug. 
//   lockEvent -o true 
//
//   // Now you can try changes nurbsSphere1.translateX 's value 
//   // but you will not be allowed to do so.  
//   // 
//   setAttr "nurbsSphere1.translateX" 22
//


#include <maya/MIOStream.h>
#include <maya/MString.h> 
#include <maya/MArgList.h> 
#include <maya/MFnPlugin.h> 
#include <maya/MPxCommand.h> 
#include <maya/MSyntax.h> 
#include <maya/MArgDatabase.h> 
#include <maya/MMessage.h> 
#include <maya/MLockMessage.h> 
#include <maya/MObject.h> 
#include <maya/MSelectionList.h> 
#include <maya/MFn.h> 
#include <maya/MItSelectionList.h> 
#include <maya/MFnDependencyNode.h> 
#include <maya/MStringArray.h> 
#include <maya/MString.h>
#include <maya/MGlobal.h> 
#include <maya/MDagPath.h> 
#include <maya/MCallbackIdArray.h>

/////////
// Macro Definitions 
//
#define MEL_COMMAND_NAME  "lockEvent"
#define VENDOR_TAG		  "Autodesk"
#define PLUGIN_VERSION	  "1.0"

#define checkStdError(stat,msg)		\
    if ( MS::kSuccess != stat ) {	\
	        cerr << msg;			\
	        return MS::kFailure;    \
    }

////////
// Flags 
//

// Clear the registered callbacks ... 
//
#define kClearCBLong 	  "-clearCB" 
#define kClearCB          "-ccb" 
#define kClearCBDV		  false

// Toggle the override flag. Dictates if we should 
// obey the lock or override the lock 
// 
#define kOverrideLong     "-override"
#define kOverride         "-o"
#define kOverrideDV       false  

// Attach a new callback to a node. 
// 
#define kAttachLong       "-attach"
#define kAttach           "-a"
#define kAttachDV         0

// Macro used to save a little typing 
//
#define MLM               MLockMessage

/////////
// Class Definition 
//
class lockEvent : public MPxCommand 
{
public: 
    lockEvent( ); 
    ~lockEvent( ) override; 
    
    MStatus        doIt( const MArgList & ) override; 
    MStatus        parseArgs( const MArgList &args ); 
	MCallbackId    installCallback( MItSelectionList & ); 
	static MSyntax newSyntax( );  	
    static void*   creator( ); 

	// Clear callback ids 
	static bool    clearCallbackIds();

private: 
	MSelectionList theList; 
	unsigned int fAttach;
	bool fOverrideFlag, fOverrideVal, fClearCB; 
}; 

static MCallbackIdArray callbackIds; 
static bool overrideMode = false; 

// Callback for DAG locking events. 
// 
void lockDagDecision( MDagPath &path, MDagPath &other, 
					  void *clientData, MLM::LockDAGEvent,
					  bool &decision ); 

// Other, node related events ..  
// 
void lockDecision( MObject &node, MObject &aux, 
				   void *clientData, MLM::LockEvent, 
				   bool &decision ); 

// Events relating to plugs. 
// 
void plugDecision( MPlug &p1, MPlug &p2, 
				   void *clientData, MLM::LockPlugEvent, 
				   bool &decision ); 

// Callback for global watchers (i.e. watch all plugs on a node).
//
void nodePlugDecision( MPlug &p1, MPlug &p2, 
						void *clientData, MLM::LockPlugEvent, 
						bool &decision ); 

// Standard API entry and exit points. 
//
MStatus initializePlugin( MObject obj );
MStatus uninitializePlugin( MObject obj );

// -------- 
// METHODS 
// 

lockEvent::lockEvent( ) : 
	fOverrideFlag(kOverrideDV), 
	fOverrideVal(false),
	fAttach(kAttachDV)
{
}

lockEvent::~lockEvent( ) 
{
}

MCallbackId lockEvent::installCallback( MItSelectionList &iter )
// 
// Description: 
//  Uses given iterator and callback type to attach a new callback on
//  a node, dag path, or plug. The selection iterator must contain a
//  valid selection item for the target callback type (fAttach). That is, 
//  if the callback type is three, then the iterator must contain 
//  a dependency node on it the next list item. 
//
{
	MStatus status; 

	MCallbackId id = 0; 
	MObject node, component; 
	MDagPath path; 

	switch (fAttach) { 
	case 1: { 
		status = iter.getDependNode( node ); 
		if ( status ) { 
			// Try to set the callback. Note: we check the status
			// flag at the end of the switch statement. 
			// 
			id = MLM::setNodeLockQueryCallback( node, lockDecision, 
												NULL, &status );
		}
	}	break;
	case 2: { 
		status = iter.getDagPath( path, component );
		if ( status ) { 
			// Try to set the callback. Note: we check the status
			// flag at the end of the switch statement. 
			// 
			id = MLM::setNodeLockDAGQueryCallback( path, lockDagDecision, 
												   NULL, &status ); 
		}
	} 	break;
	case 3: { 
		status = iter.getDependNode( node ); 
		MStringArray plugName; 
		iter.getStrings( plugName ); 
		// Now we have to parse the plug string. 
		// 
		if ( status && plugName.length() > 0 ) { 
			MFnDependencyNode depNode( node ); 
			MStringArray attrName; 
			plugName[0].split( '.', attrName ); 
			
			MPlug plug = depNode.findPlug( attrName[1],  true,  &status ); 
			if ( status ) { 
				// Try to set the callback. Note: we check the status
				// flag at the end of the switch statement. 
				// 
				id = MLM::setPlugLockQueryCallback( plug, plugDecision,	
													NULL,&status );
			}
		} else { 
			status = MS::kFailure; 
		}
	} break;	
	case 4: {
		status = iter.getDependNode( node ); 
		if ( status ) { 
			// Try to set the callback. Note: we check the status
			// flag at the end of the switch statement. 
			// 
			id = MLM::setPlugLockQueryCallback( node, nodePlugDecision, 
												NULL, &status ); 
		}
	} break; 
	default: 
		MGlobal::displayError( "Invalid callback attach type" ); 
		status = MS::kFailure; 
	}; 	

	MFnDependencyNode fnNode( node ); 
	
	// Check the status flag here and report any particular problems 
	// encountered.  It is possible for the callback attach routines 
	// to fail.
	//
	// This typically occurs when a callback has already been attached
	// to the node or plug. 
	//
	if ( !status || !id ) { 
		MString msg; 
		msg = "Unable to add callback for node "; 
		msg += fnNode.name(); 
		MGlobal::displayError( msg );
		status.perror( msg ); 
	} else { 
		// Store the result -- so we can clean up later -- and 
		// echo some useful information. 
		// 
		cerr << "Callback attached to " << fnNode.name(); 	
		cerr << "; attachment type = " << fAttach << endl;  
		callbackIds.append( (int)id ); 
	}
	return id; 
}

MStatus lockEvent::doIt( const MArgList &args ) 
//
// Description: 
//  Entry point 
//
{
	MStatus status; 
	int result = 0;
	
	// First check our arguments 
	//
	if ( !parseArgs( args ) ) { 
		return MS::kFailure; 
	}

	if ( fAttach ) { 
		MItSelectionList iter( theList, MFn::kDependencyNode, &status ); 
		for ( ; status && !iter.isDone(); iter.next() ) { 
			MCallbackId id = installCallback( iter ); 
			if ( id ) { 
				result ++; 
			} else { 
				status = MS::kFailure; 
			}
		}
	} else if ( fOverrideFlag ) { 
		// What to do when callback occurs. 
		overrideMode = fOverrideVal; 
	} else if ( fClearCB ) { 
		clearCallbackIds(); 
		result++; 
	}	 

	clearResult(); 
	// Let the caller know if the operation was successful. 
	// We just use an integer value here.  Anything > 0 is 
	// a success. 
	//
	setResult( result ); 

	return status; 
}

void *lockEvent::creator( )
// 
// Description: 
//  Create a new instance of this command. 
// 
{
	return new lockEvent; 
}

MStatus lockEvent::parseArgs( const MArgList &args )
{
	MStatus status; 
	MArgDatabase argData( syntax(), args ); 
	
	fAttach = kAttachDV; 
	fOverrideFlag = kOverrideDV; 
	fClearCB = kClearCBDV; 

	// begin-parse-args 

	if ( argData.isFlagSet( kClearCB ) ) { 
		fClearCB = !kClearCBDV; 
 	} 	

	if ( argData.isFlagSet( kOverride ) ) { 
		bool tmp;
		status = argData.getFlagArgument( kOverride, 0, tmp ); 
		if ( !status ) { 
			MGlobal::displayError( "override flag parsing failed" ); 
			return status;
		}
		fOverrideFlag = !kOverrideDV; 
		fOverrideVal = tmp; 
	}

	if ( argData.isFlagSet( kAttach ) ) { 
		unsigned int tmp;
		status = argData.getFlagArgument( kAttach, 0, tmp ); 
		if ( !status ) { 
			MGlobal::displayError( "attach flag parsing failed" ); 
			return status;
		}
		fAttach = tmp; 
	} 

	if ( fAttach ) { 
		status = argData.getObjects( theList ); 
		if ( theList.length() == 0 ) { 
			MString msg = "You must specify a node/plug to attach to!"; 
			MGlobal::displayError(msg); 
			status = MS::kFailure; 
		}
	}

	// Ensure that the caller did not specify too many arguments! 
	//
	if ( status && fAttach && fOverrideFlag ) { 
		MString msg = "You specified too many flags!" ;
		MGlobal::displayError(msg);  
		status = MS::kFailure; 
	} 
	
	// end-parse-args 
	
	return status; 
}

MSyntax lockEvent::newSyntax( )
{
	MSyntax syntax; 
	
	// begin-syntax
	syntax.addFlag( kClearCB, kClearCBLong ); 
	syntax.addFlag( kOverride, kOverrideLong, MSyntax::kBoolean ); 
	syntax.addFlag( kAttach, kAttachLong, MSyntax::kUnsigned ); 

	syntax.useSelectionAsDefault( true ); 
	syntax.setObjectType( MSyntax::kSelectionList, 0 ); 
	// end-syntax 

	return syntax; 
}

void nodePlugDecision( MPlug &p1, MPlug &p2, 
					   void *clientData, MLM::LockPlugEvent event, 
					   bool &decision )
//
// Description:  
//  The watcher for plug callbacks on entire nodes. This callback
//  is invoked whenever lock query occurs on any plug in a node. 
//
{ 
	MString msg, eventString;
	msg = "nodePlugDecision called"; 

	// Echo the received event type. 
	//
	
	switch (event) { 
	case MLM::kPlugLockAttr: 
		eventString = "kPlugLockAttr"; 
		break; 
	case MLM::kPlugUnlockAttr: 
		eventString = "kPlugUnlockAttr";
		break; 
	case MLM::kPlugAttrValChange:
		eventString = "kPlugAttrValChange"; 
		break;
	case MLM::kPlugRemoveAttr: 
		eventString = "kPlugRemoveAttr"; 
		break; 
	case MLM::kPlugRenameAttr:
		eventString = "kPlugRenameAttr";
		break;
	case MLM::kPlugConnect: 
		eventString = "kPlugConnect"; 
		break;
	case MLM::kPlugDisconnect: 
		eventString = "kPlugDisconnect"; 
		break; 
	default:
		eventString = "kLastPlug"; 
		break;
	}; 
			
	cerr << msg << "; event = " << eventString;	
	cerr << "; override = " << overrideMode << endl; 

	decision = !overrideMode; 
}	

void plugDecision( MPlug &p1, MPlug &p2, 
				   void *clientData, MLM::LockPlugEvent event, 
				   bool &decision )
// 
// Description: 
//   Callback function for plug locking events.  This callback 
//   is only invoked when the plug, p1, has its lock status 
//   queried. This callback shares the same event types 
//   as the nodePlugDecision callback. 
//
{ 
	MString msg, eventString;
	msg = "plugDecision called";

	switch (event) { 
	case MLM::kPlugLockAttr: 
		eventString = "kPlugLockAttr"; 
		break; 
	case MLM::kPlugUnlockAttr: 
		eventString = "kPlugUnlockAttr";
		break; 
	case MLM::kPlugAttrValChange:
		eventString = "kPlugAttrValChange"; 
		break;
	case MLM::kPlugRemoveAttr: 
		eventString = "kPlugRemoveAttr"; 
		break; 
	case MLM::kPlugRenameAttr:
		eventString = "kPlugRenameAttr";
		break;
	case MLM::kPlugConnect: 
		eventString = "kPlugConnect"; 
		break;
	case MLM::kPlugDisconnect: 
		eventString = "kPlugDisconnect"; 
		break; 
	default:
		eventString = "kInvalidPlug"; 
		break;
	}; 
			
	cerr << msg << "; event = " << eventString;	
	cerr << "; override = " << overrideMode << endl; 
	
	decision = !overrideMode;  
}	
	
void lockDagDecision( MDagPath &path, MDagPath &other, 
					  void *clientData, MLM::LockDAGEvent event, 
					  bool &decision )
// 
// Description: 
//  Callback that is invoked whenever a DAG element is involved
//  in a locking event. 
//  
{
	MString eventString; 
	cerr << "lockDagDecision called ";

	switch (event) { 
	case MLM::kGroup: 
		eventString = "kGroup"; 
		break; 
	case MLM::kUnGroup:
		eventString = "kUnGroup";
		break;
	case MLM::kReparent: 
		eventString = "kReparent"; 
		break;
	case MLM::kChildReorder: 
		eventString = "kChildReorder"; 
		break;
	case MLM::kCreateNodeInstance: 
		eventString = "kCreateNodeInstance"; 
		break;
	case MLM::kCreateChildInstance:
		eventString = "kCreateChildInstance"; 
		break;
	case MLM::kCreateParentInstance:
		eventString = "kCreateParentInstance"; 
		break;
	case MLM::kInvalidDAG: 
	default: 
		eventString = "kInvalid"; 	
	}; 
		
	cerr << "on " << eventString << " event"; 
	cerr << "; overrideMode = " << overrideMode << endl; 

	decision = !overrideMode; 
}

void lockDecision( MObject &node, MObject &attr, 
				   void *clientData, MLM::LockEvent event,
				   bool &decision )
// 
// Description: 
//  All other lock callback events are convered in this routine. 
//  This includes everything that is not DAG related.  
//
{
	MString eventString; 
	cerr << "lockDecision called "; 
	
	switch ( event ) { 
	case MLM::kDelete: 
		eventString = "kDelete"; 
		break;
	case MLM::kRename: 
		eventString = "kRename"; 
		break;
	case MLM::kLockNode: 
		eventString = "kLockNode"; 
		break; 
	case MLM::kUnlockNode: 
		eventString = "kUnlockNode"; 
		break; 
	case MLM::kAddAttr: 
		eventString = "kAddAttr"; 
		break;
	case MLM::kRemoveAttr: 
		eventString = "kRemoveAttr"; 
		break;
	case MLM::kRenameAttr: 
		eventString = "kRemoveAttr"; 
		break; 
	case MLM::kUnlockAttr: 
		eventString = "kUnlockAttr"; 
		break;
	case MLM::kLockAttr: 
		eventString = "kLockAttr"; 
		break;
	case MLM::kInvalid: 
	default: 
		eventString = "kInvalid"; 
	}; 

	cerr << "on " << eventString << " event";
	cerr << "; overrideMode = " << overrideMode << endl; 

	decision = !overrideMode; 
}

bool lockEvent::clearCallbackIds( ) 
// 
// Description: 
//  Removes all currently attached callbacks.
//
{
	unsigned int idCount = callbackIds.length(); 
	for ( unsigned int i = 0; i < idCount; i ++ ) { 
		cerr << "callback #" << i << "; id = " << (unsigned)callbackIds[i] << endl; 
		MMessage::removeCallback( (MCallbackId) callbackIds[i] );
	} 
	callbackIds.clear(); 
	return true; 
}

MStatus initializePlugin( MObject obj )
// 
// Load the plugin ...
//
{
	MStatus status;
	
	MFnPlugin plugin( obj, VENDOR_TAG, PLUGIN_VERSION, "Any" ); 

	status = plugin.registerCommand( MEL_COMMAND_NAME, 
									 lockEvent::creator, 
									 lockEvent::newSyntax ); 

	callbackIds.clear(); 
	return status; 
}

MStatus uninitializePlugin( MObject obj )
// 
// Unload the plugin ...
//
{
	MFnPlugin plugin( obj ); 
	
	MStatus status; 

	status = plugin.deregisterCommand( MEL_COMMAND_NAME ); 
	if ( status ) { 
		lockEvent::clearCallbackIds(); 
	}

	return status; 
}
