//
//  Copyright 2012 Autodesk, Inc.  All rights reserved.
//
//  Use of this software is subject to the terms of the Autodesk license 
//  agreement provided at the time of installation or download, or which 
//  otherwise accompanies this software in either electronic or hard copy form.   
//

//-----------------------------------------------------------------------------
#pragma once

#include <maya/MPxCommand.h>

#define kmoduleLogicCmdName _T("moduleDetectionLogic")

//-----------------------------------------------------------------------------
class moduleLogicCmd : public MPxCommand {

public:
	moduleLogicCmd () : MPxCommand () {}
	~moduleLogicCmd () override {}

	MStatus doIt (const MArgList &args) override ;

	//- This method tells Maya this command is undoable. It is added to the undo 
	//- queue if it is. Return false if you don't want the command to be undoable.
	bool isUndoable () const override { return (false) ; }

	static void *creator() { return new moduleLogicCmd () ; }

} ;
