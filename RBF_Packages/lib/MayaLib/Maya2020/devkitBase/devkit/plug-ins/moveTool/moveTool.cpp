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
// Produces the MEL commands "moveToolCmd" and "moveToolContext".
// 
// Interactive tool for moving objects and components.
//
// This is an example of a selection-action tool. When nothing is selected,
// this tool behaves in exactly the same way as the selection tool in Maya. 
// Once an object is selected, the tool turns into a translation tool. 
//
// Note that at this time, the plug-in can translate:
//	
//	- transforms 
//	- NURBS curve CVs
//	- NURBS surface CVs 
//	- polygonal vertices 
//
// This plug-in will only perform translation in orthographic views. 
// Undo, redo, and journaling are supported by this tool.
//
// To use this plug-in:
//	(1) Execute the command "source moveTool". 
//		This creates a new entry in the "Shelf1" tab of the tool shelf called "moveTool".
//	(2) Click on the new icon, then select an object and drag it around in an orthographic view.
//		The left mouse button allows movement in two directions, while the middle mouse button
//		constrains the movement to a single direction. 
// 
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>
#include <stdio.h>
#include <stdlib.h>

#include <maya/MPxToolCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MDagPath.h>

#include <maya/MFnTransform.h>
#include <maya/MItCurveCV.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItMeshVertex.h>

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>
#include <maya/M3dView.h>
#include <maya/MFnCamera.h>

#define CHECKRESULT(stat,msg)     \
	if ( MS::kSuccess != stat ) { \
		cerr << msg << endl;      \
	}

#define kVectorEpsilon 1.0e-3

/////////////////////////////////////////////////////////////
//
// The move command
//
// - this is a tool command which can be used in tool
//   contexts or in the MEL command window.
//
/////////////////////////////////////////////////////////////
#define		MOVENAME	"moveToolCmd"
#define		DOIT		0
#define		UNDOIT		1
#define		REDOIT		2

class moveCmd : public MPxToolCommand
{
public:
	moveCmd();
	~moveCmd() override; 

	MStatus     doIt( const MArgList& args ) override;
	MStatus     redoIt() override;
	MStatus     undoIt() override;
	bool        isUndoable() const override;
	MStatus		finalize() override;
	
public:
	static void* creator();

	void		setVector( double x, double y, double z);
private:
	MVector 	delta;	// the delta vectors
	MStatus 	action( int flag );	// do the work here
};

moveCmd::moveCmd( )
{
	setCommandString( MOVENAME );
}

moveCmd::~moveCmd()
{}

void* moveCmd::creator()
{
	return new moveCmd;
}

bool moveCmd::isUndoable() const
//
// Description
//     Set this command to be undoable.
//
{
	return true;
}

void moveCmd::setVector( double x, double y, double z)
{
	delta.x = x;
	delta.y = y;
	delta.z = z;
}

MStatus moveCmd::finalize()
//
// Description
//     Command is finished, construct a string for the command
//     for journalling.
//
{
    MArgList command;
    command.addArg( commandString() );
    command.addArg( delta.x );
    command.addArg( delta.y );
    command.addArg( delta.z );

	// This call adds the command to the undo queue and sets
	// the journal string for the command.
	//
    return MPxToolCommand::doFinalize( command );
}

MStatus moveCmd::doIt( const MArgList& args )
//
// Description
// 		Test MItSelectionList class
//
{
	MStatus stat;
	MVector	vector( 1.0, 0.0, 0.0 );	// default delta
	unsigned i = 0;

	switch ( args.length() )	 // set arguments to vector
	{
		case 1:
			vector.x = args.asDouble( 0, &stat );
			break;
		case 2:
			vector.x = args.asDouble( 0, &stat );
			vector.y = args.asDouble( 1, &stat );
			break;
		case 3:
			vector = args.asVector(i,3);
			break;
		case 0:
		default:
			break;
	}
	delta = vector;

	return action( DOIT );
}

MStatus moveCmd::undoIt( )
//
// Description
// 		Undo last delta translation
//
{
	return action( UNDOIT );
}

MStatus moveCmd::redoIt( )
//
// Description
// 		Redo last delta translation
//
{
	return action( REDOIT );
}

MStatus moveCmd::action( int flag )
//
// Description
// 		Do the actual work here to move the objects	by vector
//
{
	MStatus stat;
	MVector vector = delta;

	switch( flag )
	{
		case UNDOIT:	// undo
			vector.x = -vector.x;
			vector.y = -vector.y;
			vector.z = -vector.z;
			break;
		case REDOIT:	// redo
			break;
		case DOIT:		// do command
			break;
		default:
			break;
	}

	// Create a selection list iterator
	//
	MSelectionList slist;
 	MGlobal::getActiveSelectionList( slist );
	MItSelectionList iter( slist, MFn::kInvalid, &stat );

	if ( MS::kSuccess == stat ) {
		MDagPath 	mdagPath;		// Item dag path
		MObject 	mComponent;		// Current component
		MSpace::Space spc = MSpace::kWorld;

		// Translate all selected objects
		//
		for ( ; !iter.isDone(); iter.next() ) 
		{
			// Get path and possibly a component
			//
			iter.getDagPath( mdagPath, mComponent );

			MFnTransform transFn( mdagPath, &stat );
			if ( MS::kSuccess == stat ) {
				stat = transFn.translateBy( vector, spc );
				CHECKRESULT(stat,"Error doing translate on transform");
				continue;
			}

			MItCurveCV cvFn( mdagPath, mComponent, &stat );
			if ( MS::kSuccess == stat ) {
				for ( ; !cvFn.isDone(); cvFn.next() ) {
					stat = cvFn.translateBy( vector, spc );
					CHECKRESULT(stat,"Error setting CV");
				}
				cvFn.updateCurve();
			}

			MItSurfaceCV sCvFn( mdagPath, mComponent, true, &stat );
			if ( MS::kSuccess == stat ) {
				for ( ; !sCvFn.isDone(); sCvFn.nextRow() ) {
					for ( ; !sCvFn.isRowDone(); sCvFn.next() ) {
						stat = sCvFn.translateBy( vector, spc );
						CHECKRESULT(stat,"Error setting CV");
					}
				}
				sCvFn.updateSurface();
			}

			MItMeshVertex vtxFn( mdagPath, mComponent, &stat );
			if ( MS::kSuccess == stat ) {
				for ( ; !vtxFn.isDone(); vtxFn.next() ) {
					stat = vtxFn.translateBy( vector, spc );
					CHECKRESULT(stat,"Error setting Vertex");
				}
				vtxFn.updateSurface();
			}
		} // for
	}
	else {
		cerr << "Error creating selection list iterator" << endl;
	}
	return MS::kSuccess;
}


/////////////////////////////////////////////////////////////
//
// The moveTool Context
//
// - tool contexts are custom event handlers. The selection
//   context class defaults to maya's selection mode and
//   allows you to override press/drag/release events.
//
/////////////////////////////////////////////////////////////
#define     MOVEHELPSTR        "drag to move selected object"
#define     MOVETITLESTR       "moveTool"
#define		TOP			0
#define		FRONT		1
#define		SIDE		2
#define		PERSP		3

class moveContext : public MPxSelectionContext
{
public:
    moveContext();
    void    toolOnSetup( MEvent & event ) override;
    MStatus doPress( MEvent & event ) override;
    MStatus doDrag( MEvent & event ) override;
    MStatus doRelease( MEvent & event ) override;
    MStatus doEnterRegion( MEvent & event ) override;

	/* override */
	MStatus doPress (MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
	/* override */
	MStatus	doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
	/* override */
	MStatus	doDrag ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
	/* override */
	MStatus	doHold ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context) override;
private:
	int currWin;
	MEvent::MouseButtonType downButton;
	M3dView view;
	short startPos_x, endPos_x, start_x, last_x;
	short startPos_y, endPos_y, start_y, last_y;
	moveCmd * cmd;
};


moveContext::moveContext()
{
	MString str( MOVETITLESTR );
    setTitleString( str );

	// Tell the context which XPM to use so the tool can properly
	// be a candidate for the 6th position on the mini-bar.
	setImage("moveTool.xpm", MPxContext::kImage1 );
}

void moveContext::toolOnSetup( MEvent & )
{
	MString str( MOVEHELPSTR );
    setHelpString( str );
}

/* just for test purpose. */
MStatus	moveContext::doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	MStatus stat = doRelease( event ); // call VP1.0 version
	printf("- VP2.0 version doRelease() called.\n");
	return stat;
}
/* just for test purpose. */
MStatus	moveContext::doDrag ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	MStatus stat = doDrag( event ); // call VP1.0 version
	printf("- VP2.0 version doDrag() called.\n");
	return stat;
}
/* just for test purpose. */
MStatus	moveContext::doHold ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	printf("- VP2.0 version doHold() called.\n");
	return MStatus::kSuccess;
}

/* just for test purpose. */
MStatus moveContext::doPress (MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	MStatus stat = doPress( event ); // call VP1.0 version
	printf("- VP2.0 version doPress() called.\n");
	return stat;
}

MStatus moveContext::doPress( MEvent & event )
{
	MStatus stat = MPxSelectionContext::doPress( event );
	MSpace::Space spc = MSpace::kWorld;

	// If we are not in selecting mode (i.e. an object has been selected)
	// then set up for the translation.
	//
	if ( !isSelecting() ) {
		event.getPosition( startPos_x, startPos_y );
		view = M3dView::active3dView();

		MDagPath camera;
		stat = view.getCamera( camera );
		if ( stat != MS::kSuccess ) {
			cerr << "Error: M3dView::getCamera" << endl;
			return stat;
		}
		MFnCamera fnCamera( camera );
		MVector upDir = fnCamera.upDirection( spc );
		MVector rightDir = fnCamera.rightDirection( spc );

		// Determine the camera used in the current view
		//
		if ( fnCamera.isOrtho() ) {
			if ( upDir.isEquivalent(MVector::zNegAxis,kVectorEpsilon) ) {
				currWin = TOP;
			} else if ( rightDir.isEquivalent(MVector::xAxis,kVectorEpsilon) ) {
				currWin = FRONT;
			} else  {
				currWin = SIDE;
			}
		}
		else {
			currWin = PERSP;
		}

		// Create an instance of the move tool command.
		//
		cmd = (moveCmd*)newToolCommand();

		cmd->setVector( 0.0, 0.0, 0.0 );
	}
	return stat;
}

MStatus moveContext::doDrag( MEvent & event )
{
	MStatus stat;
	stat = MPxSelectionContext::doDrag( event );

	// If we are not in selecting mode (i.e. an object has been selected)
	// then do the translation.
	//
	if ( !isSelecting() ) {
		event.getPosition( endPos_x, endPos_y );
		MPoint endW, startW;
		MVector vec;
		view.viewToWorld( startPos_x, startPos_y, startW, vec );
		view.viewToWorld( endPos_x, endPos_y, endW, vec );
		downButton = event.mouseButton();

		// We reset the the move vector each time a drag event occurs 
		// and then recalculate it based on the start position. 
		//
		cmd->undoIt();

		switch( currWin )
		{
			case TOP:
				switch ( downButton )
				{
					case MEvent::kMiddleMouse :
						cmd->setVector( endW.x - startW.x, 0.0, 0.0 );
						break;
					case MEvent::kLeftMouse :
					default:
						cmd->setVector( endW.x - startW.x, 0.0,
											   endW.z - startW.z );
						break;
				}
				break;	

			case FRONT:
				switch ( downButton )
				{
					case MEvent::kMiddleMouse :
						cmd->setVector( endW.x - startW.x, 0.0, 0.0 );
						break;
					case MEvent::kLeftMouse :
					default:
						cmd->setVector( endW.x - startW.x,
											   endW.y - startW.y, 0.0 );
						break;
				}
				break;	

			case SIDE:
				switch ( downButton )
				{
					case MEvent::kMiddleMouse :
						cmd->setVector( 0.0, 0.0, endW.z - startW.z );
						break;
					case MEvent::kLeftMouse :
					default:
						cmd->setVector( 0.0, endW.y - startW.y,
											   endW.z - startW.z );
						break;
				}
				break;	

			case PERSP:
				break;
		}

		stat = cmd->redoIt();
		view.refresh( true );
	}
	return stat;
}

MStatus moveContext::doRelease( MEvent & event )
{
	MStatus stat = MPxSelectionContext::doRelease( event );
	if ( !isSelecting() ) {
		event.getPosition( endPos_x, endPos_y );

		// Delete the move command if we have moved less then 2 pixels
		// otherwise call finalize to set up the journal and add the
		// command to the undo queue.
		//
		if ( abs(startPos_x - endPos_x) < 2 && abs(startPos_y - endPos_y) < 2 ) {
			delete cmd;
			view.refresh( true );
		}
		else {
			stat = cmd->finalize();
			view.refresh( true );
		}
	}
	return stat;
}

MStatus moveContext::doEnterRegion( MEvent & /*event*/ )
//
// Print the tool description in the help line.
//
{
	MString str( MOVEHELPSTR );
    return setHelpString( str );
}


/////////////////////////////////////////////////////////////
//
// Context creation command
//
//  This is the command that will be used to create instances
//  of our context.
//
/////////////////////////////////////////////////////////////
#define     CREATE_CTX_NAME	"moveToolContext"

class moveContextCommand : public MPxContextCommand
{
public:
    moveContextCommand() {};
    MPxContext * makeObj() override;

public:
    static void* creator();
};

MPxContext * moveContextCommand::makeObj()
{
    return new moveContext();
}

void * moveContextCommand::creator()
{
    return new moveContextCommand;
}


///////////////////////////////////////////////////////////////////////
//
// The following routines are used to register/unregister
// the commands we are creating within Maya
//
///////////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
	MStatus		status;
	MFnPlugin	plugin( obj, PLUGIN_COMPANY, "3.0", "Any" );

	status = plugin.registerContextCommand( CREATE_CTX_NAME,
									&moveContextCommand::creator,
									MOVENAME, &moveCmd::creator );
	if (!status) {
		status.perror("registerContextCommand");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus		status;
	MFnPlugin	plugin( obj );

	status = plugin.deregisterContextCommand( CREATE_CTX_NAME, MOVENAME );
	if (!status) {
		status.perror("deregisterContextCommand");
		return status;
	}

	return status;
}
