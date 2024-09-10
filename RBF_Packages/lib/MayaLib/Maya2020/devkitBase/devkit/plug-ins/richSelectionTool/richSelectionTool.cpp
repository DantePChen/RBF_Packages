// Copyright 2016 Autodesk, Inc. All rights reserved.
// 
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.

// File: richSelectionTool.cpp
//
#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>
#include <maya/MEvent.h> 
#include <maya/MDagPath.h>
#include <maya/MFn.h>
#include <maya/MGlobal.h>

#include <maya/MUIDrawManager.h>
#include <maya/MFrameContext.h>

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContextCommand.h>

class richSelectionContext : public MPxSelectionContext
{
public:

	// constructor
	richSelectionContext();

	// destructor
	virtual ~richSelectionContext() {} ;

	virtual void	toolOnSetup( MEvent & event );
	virtual void	toolOffCleanup();

	virtual MStatus	doPress ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);
	virtual MStatus	doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context);

	virtual void	getClassName(MString & name) const;
}; 

richSelectionContext::richSelectionContext()
{

}

void richSelectionContext::toolOnSetup( MEvent &event )
{
	MPxSelectionContext::toolOnSetup( event );
	
	setAllowPreSelectHilight();
	setAllowSoftSelect();
	setAllowSymmetry();
	setAllowDoubleClickAction();
}

void richSelectionContext::toolOffCleanup()
{
	MPxSelectionContext::toolOffCleanup();
}

MStatus	richSelectionContext::doPress ( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	MPxSelectionContext::doPress(event, drawMgr, context);

	return MS::kSuccess;
}

MStatus	richSelectionContext::doRelease( MEvent & event, MHWRender::MUIDrawManager& drawMgr, const MHWRender::MFrameContext& context)
{
	MPxSelectionContext::doRelease(event, drawMgr, context);
	
	return MS::kSuccess;
}


void richSelectionContext::getClassName( MString & name ) const
{
	name.set("richSelection");
}


//---------------------------------------------
class richSelectionContextCommand : public MPxContextCommand
{
public:	 
						richSelectionContextCommand();
	virtual MPxContext* makeObj();
	static void*		creator();

protected:
	richSelectionContext*		fRichSelectionContext;
};		    

richSelectionContextCommand::richSelectionContextCommand() {}

MPxContext* richSelectionContextCommand::makeObj()
//
// Description
//    When the context command is executed in maya, this method
//    be used to create a context.
//
{
	fRichSelectionContext = new richSelectionContext();
	return fRichSelectionContext;
}

void* richSelectionContextCommand::creator()
//
// Description
//    This method creates the context command.
//
{
	return new richSelectionContextCommand;
}

// ------------------------------------------------------------------
#define CTX_CREATOR_NAME "richSelectionContext"

MStatus initializePlugin( MObject obj )
	//
	//	Description:
	//		this method is called when the plug-in is loaded into Maya.  It 
	//		registers all of the services that this plug-in provides with 
	//		Maya.
	//
	//	Arguments:
	//		obj - a handle to the plug-in object (use MFnPlugin to access it)
	//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, "", "2017", "Any");

	// Add plug-in feature registration here
	//
	status = plugin.registerContextCommand(CTX_CREATOR_NAME, richSelectionContextCommand::creator);

	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}

MStatus uninitializePlugin( MObject obj )
	//
	//	Description:
	//		this method is called when the plug-in is unloaded from Maya. It 
	//		deregisters all of the services that it was providing.
	//
	//	Arguments:
	//		obj - a handle to the plug-in object (use MFnPlugin to access it)
	//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	// Add plug-in feature deregistration here
	//
	status = plugin.deregisterContextCommand(CTX_CREATOR_NAME);

	CHECK_MSTATUS_AND_RETURN_IT(status);

	return status;
}


