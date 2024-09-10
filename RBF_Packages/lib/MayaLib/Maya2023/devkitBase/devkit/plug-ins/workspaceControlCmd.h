#ifndef workspaceControlCmd_h
#define workspaceControlCmd_h
// ===========================================================================
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================

#include <QtCore/QPointer>
#include <QtWidgets/QWidget>
#include <maya/MPxCommand.h>

class WorkspaceControlCmd : public MPxCommand
{
public:
	static void		cleanup();
	static void*	creator()		{ return new WorkspaceControlCmd(); }
	static MSyntax	newSyntax();

	virtual bool	hasSyntax();
	MStatus			doIt(const MArgList& args);

	static QPointer<QWidget>		workspaceControl;
	static const MString			commandName;
};

#endif
