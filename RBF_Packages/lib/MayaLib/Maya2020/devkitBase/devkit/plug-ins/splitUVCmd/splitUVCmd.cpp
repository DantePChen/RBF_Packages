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
// The splitUV command unshares or "splits" the selected UVs of a polygonal mesh.
// It is also a good example of how to write poly operation nodes that properly
// deal with history, tweaks, and so on. 
//
// MEL Command:
//	
//	splitUV
//
////////////////////////////////////////////////////////////////////////

#include "splitUVCmd.h"
#include "splitUVNode.h"

// Function Sets
//
#include <maya/MFnDependencyNode.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSingleIndexedComponent.h>

// Iterators
//
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>

// General Includes
//
#include <maya/MGlobal.h>
#include <maya/MSelectionList.h>
#include <maya/MPlug.h>

#include <maya/MIOStream.h>

// Status Checking Macro - MCheckStatus (Debugging tool)
//
#define MCheckStatus(status,message)	\
	if( MS::kSuccess != status ) {		\
		cerr << message << "\n";		\
		return status;					\
	}


splitUV::splitUV()
//
//	Description:
//		splitUV constructor
//
{}

splitUV::~splitUV()
//
//	Description:
//		splitUV destructor
//
{}

void* splitUV::creator()
//
//	Description:
//		this method exists to give Maya a way to create new objects
//      of this type. 
//
//	Return Value:
//		a new object of this type
//
{
	return new splitUV();
}

bool splitUV::isUndoable() const
//
//	Description:
//		this method tells Maya this command is undoable.  It is added to the 
//		undo queue if it is.
//
//	Return Value:
//		true if this command is undoable.
//
{
	return true;
}

MStatus splitUV::doIt( const MArgList& )
//
//	Description:
//		implements the MEL splitUV command.
//
//	Arguments:
//		args - the argument list that was passes to the command from MEL
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - command failed (returning this value will cause the 
//                     MEL script that is being run to terminate unless the
//                     error is caught using a "catch" statement.
//
{
	MStatus status;

	// Parse the selection list for objects with selected UV components.
	// To simplify things, we only take the first object that we find with
	// selected UVs and operate on that object alone.
	//
	// All other objects are ignored and return warning messages indicating
	// this limitation.
	//
	MSelectionList selList;
	MGlobal::getActiveSelectionList( selList );
	MItSelectionList selListIter( selList );
	selListIter.setFilter( MFn::kMesh );

	// The splitUV node only accepts a component list input, so we build
	// a component list using MFnComponentListData.
	//
	// MIntArrays could also be passed into the node to represent the uvIds,
	// but are less storage efficient than component lists, since consecutive 
	// components are bundled into a single entry in component lists.
	//
	MFnComponentListData compListFn;
	compListFn.create();
	bool found = false;
	bool foundMultiple = false;

	for( ; !selListIter.isDone(); selListIter.next() )
	{
		MDagPath dagPath;
		MObject component;
		selListIter.getDagPath( dagPath, component );

		// Check for selected UV components
		//
		if( component.apiType() == MFn::kMeshMapComponent )
		{
			if( !found )
			{
				// The variable 'component' holds all selected components on the selected
				// object, thus only a single call to MFnComponentListData::add() is needed
				// to store the selected components for a given object.
				//
				compListFn.add( component );

				// Copy the component list created by MFnComponentListData into our local
				// component list MObject member.
				//
				fComponentList = compListFn.object();

				// Locally store the actual uvIds of the selected UVs so that this command
				// can directly modify the mesh in the case when there is no history and
				// history is turned off.
				//
				MFnSingleIndexedComponent compFn( component );
				compFn.getElements( fSelUVs );

				// Ensure that this DAG path will point to the shape of our object.
				// Set the DAG path for the polyModifierCmd.
				//
				dagPath.extendToShape();
				setMeshNode( dagPath );
				found = true;
			}
			else
			{
				// Break once we have found a multiple object holding selected UVs, since
				// we are not interested in how many multiple objects there are, only
				// the fact that there are multiple objects.
				//
				foundMultiple = true;
				break;
			}
		}
	}
	if( foundMultiple )
	{
		displayWarning("Found more than one object with selected UVs - Only operating on first found object.");
	}

	// Initialize the polyModifierCmd node type - mesh node already set
	//
	setModifierNodeType( splitUVNode::id );

	if( found )
	{
		if( validateUVs() )
		{
			// Now, pass control over to the polyModifierCmd::doModifyPoly() method
			// to handle the operation.
			//
			status = doModifyPoly();
			
			if( status == MS::kSuccess )
			{
				setResult( "splitUV command succeeded!" );
			}
			else
			{
				displayError( "splitUV command failed!" );
			}
		}
		else
		{
			displayError( "splitUV command failed: Selected UVs are not splittable" );
			status = MS::kFailure;
		}
	}
	else
	{
		displayError( "splitUV command failed: Unable to find selected UVs" );
		status = MS::kFailure;
	}
	
	return status;
}

MStatus splitUV::redoIt()
//
//	Description:
//		Implements redo for the MEL splitUV command. 
//
//		This method is called when the user has undone a command of this type
//		and then redoes it.  No arguments are passed in as all of the necessary
//		information is cached by the doIt method.
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
//
{
	MStatus status;

	// Process the polyModifierCmd
	//
	status = redoModifyPoly();

	if( status == MS::kSuccess )
	{
		setResult( "splitUV command succeeded!" );
	}
	else
	{
		displayError( "splitUV command failed!" );
	}

	return status;
}

MStatus splitUV::undoIt()
//
//	Description:
//		implements undo for the MEL splitUV command.  
//
//		This method is called to undo a previous command of this type.  The 
//		system should be returned to the exact state that it was it previous 
//		to this command being executed.  That includes the selection state.
//
//	Return Value:
//		MS::kSuccess - command succeeded
//		MS::kFailure - redoIt failed.  this is a serious problem that will
//                     likely cause the undo queue to be purged
//
{
	MStatus status;

	status = undoModifyPoly();

	if( status == MS::kSuccess )
	{
		setResult( "splitUV undo succeeded!" );
	}
	else
	{
		setResult( "splitUV undo failed!" );
	}
    
	return status;
}

MStatus splitUV::initModifierNode( MObject modifierNode )
{
	MStatus status;

	// We need to tell the splitUV node which UVs to operate on. By overriding
	// the polyModifierCmd::initModifierNode() method, we can insert our own
	// modifierNode initialization code.
	//
	MFnDependencyNode depNodeFn( modifierNode );
	MObject uvListAttr;
	uvListAttr = depNodeFn.attribute( "inputComponents" );

	// Pass the component list down to the splitUV node
	//
	MPlug uvListPlug( modifierNode, uvListAttr );
	status = uvListPlug.setValue( fComponentList );

	return status;
}

MStatus splitUV::directModifier( MObject mesh )
{
	MStatus status;

	fSplitUVFactory.setMesh( mesh );
	fSplitUVFactory.setUVIds( fSelUVs );

	// Now, perform the splitUV
	//
	status = fSplitUVFactory.doIt();

	return status;
}

// Private Methods
//
bool splitUV::validateUVs()
//
// Description:
//
//		Validate the UVs for the splitUV operation. UVs are valid only if they are shared
//		by more than one face. While the splitUVNode is smart enough to not process the
//		split if a UV is not splittable, a splitUV node is still created by the polyModifierCmd.
//		So call this method to validate the UVs before calling doModifyPoly().
//
//		validateUVs() will return true so long as there is at least one valid UV. It will
//		also prune out any invalid UVs from both the component list and UVId array.
//
{
	// Get the mesh that we are operating on
	//
	MDagPath		dagPath = getMeshNode();
	MObject			mesh = dagPath.node();

	// Get the number of faces sharing the selected UVs
	//
	MFnMesh			meshFn( mesh );
	MItMeshPolygon	polyIter( mesh );
	MIntArray		selUVFaceCountArray;

	int i;
	int j;
	int count = 0;
	int selUVsCount = fSelUVs.length();

	for( i = 0; i < selUVsCount; i++ )
	{
		for( ; !polyIter.isDone(); polyIter.next() )
		{
			if( polyIter.hasUVs() )
			{
				int polyVertCount = polyIter.polygonVertexCount();

				for( j = 0; j < polyVertCount; j++ )
				{
					int UVIndex = 0;
					polyIter.getUVIndex(j, UVIndex);

					if( UVIndex == fSelUVs[i] )
					{
						count++;
						break;
					}
				}
			}
		}

		selUVFaceCountArray.append(count);
	}

	// Now, check to make sure that at least one UV is being shared by more than one
	// face. So long as we have one UV that we can operate on, we should proceed and let
	// the splitUVNode ignore the UVs which are only shared by one face.
	//
	bool isValid = false;
	MIntArray validUVIndices;

	for( i = 0; i < selUVsCount; i++ )
	{
		if( selUVFaceCountArray[i] > 1 )
		{
			isValid = true;
			validUVIndices.append(i);
		}
	}

	if( isValid )
	{
		pruneUVs( validUVIndices );
	}

	return isValid;
}

MStatus splitUV::pruneUVs( MIntArray& validUVIndices )
//
// Description:
//
//		This method will remove any invalid UVIds from the component list and UVId array.
//		The benefit of this is to reduce the amount of extra processing that the node would
//		have to perform. It will result in less iterations through the mesh as there are
//		less UVs to search for.
//
{
	MStatus status;

	unsigned i;
	MIntArray validUVIds;

	for( i = 0; i < validUVIndices.length(); i++ )
	{
		int uvIndex = validUVIndices[i];
		validUVIds.append( fSelUVs[uvIndex] );
	}

	// Replace the local int array of UVIds
	//
	fSelUVs.clear();
	fSelUVs = validUVIds;

	// Build the list of valid components
	//
	MFnSingleIndexedComponent compFn;
	compFn.create( MFn::kMeshMapComponent, &status );
	MCheckStatus( status, "compFn.create( MFn::kMeshMapComponent )" );
	status = compFn.addElements( validUVIds );
	MCheckStatus( status, "compFn.addElements( validUVIds )" );
	MObject component = compFn.object();

	// Replace the component list
	//
	MFnComponentListData compListFn;
	compListFn.create();
	status = compListFn.add( component );
	MCheckStatus( status, "compListFn.add( component )" );

	fComponentList = compListFn.object();

	return status;
}
