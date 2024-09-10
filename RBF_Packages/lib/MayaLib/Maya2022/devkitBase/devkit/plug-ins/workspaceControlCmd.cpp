// ===========================================================================
// Copyright 2016 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
#include <workspaceControlCmd.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <maya/MArgParser.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MObject.h>
#include <maya/MQtUtil.h>
#include <maya/MSyntax.h>

#define	kReloadFlag			"-rl"
#define	kReloadFlagLong		"-reload"

// ==========================================================================
//
//          WorkspaceControlCmd class
//
// ==========================================================================
//  We store a pointer to the workspace control in a static QPointer so that we
//  can close (and thus delete) it if the plugin is unloaded. The QPointer will 
//  automatically set itself to zero if the workspace control is closed on its own.
QPointer<QWidget>   WorkspaceControlCmd::workspaceControl;
const MString       WorkspaceControlCmd::commandName("workspaceControlWindow");

//  Close the workspace control, if it still exists.
void WorkspaceControlCmd::cleanup()
{
	if (!workspaceControl.isNull()) {
		MString closeCommand("workspaceControl -e -close customWorkspaceControl");
		MGlobal::executeCommand(closeCommand);
	}
}

bool WorkspaceControlCmd::hasSyntax()
{
	return true;
}

MSyntax WorkspaceControlCmd::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag( kReloadFlag, kReloadFlagLong );

	return syntax;
}

MStatus WorkspaceControlCmd::doIt(const MArgList& args)
{
    if(!workspaceControl.isNull()) {
		// Call -restore on existing workspace control to make it visible from whatever previous state it was in
		MString restoreCommand("workspaceControl -e -restore customWorkspaceControl");
		MGlobal::executeCommand(restoreCommand);
	} else {
		MArgParser argParser( syntax(), args );
		bool doReload = argParser.isFlagSet( kReloadFlag );

		// If we're reloading a workspace, the workspace control should already be created
		if (!doReload) {
			MGlobal::executeCommand("workspaceControl -label \"Custom Workspace Control\" -retain false -deleteLater false -loadImmediately true -floating true -initialWidth 400 -initialHeight 200 -requiredPlugin \"workspaceControlCmd\" customWorkspaceControl");
		}

		// Create custom UI
		workspaceControl = MQtUtil::getCurrentParent();
		QWidget *uiWidget = new QWidget();
		QVBoxLayout *uiLayout = new QVBoxLayout();
		QLabel *uiLabel = new QLabel("Hello World!");
		uiLabel->setAlignment(Qt::AlignCenter);
		uiLayout->addWidget(uiLabel);
		uiWidget->setLayout(uiLayout);

		// Add UI as a child of the workspace control
		MQtUtil::addWidgetToMayaLayout(uiWidget, workspaceControl);

		// Set the -uiScript (used to rebuild UI when reloading workspace) after creation so that it doesn't get executed
		if (!doReload) {
			MString uiScriptCommand("workspaceControl -e -uiScript \"workspaceControlWindow -reload\" customWorkspaceControl");
			MGlobal::executeCommand(uiScriptCommand);
		}
	}

    return MS::kSuccess;
}
// ==========================================================================
//
//          Plugin load/unload
//
// ==========================================================================
MStatus initializePlugin(MObject plugin)
{
    MStatus     st;
    MFnPlugin   pluginFn(plugin, "Autodesk, Inc.", "1.0", "Any", &st);
    if (!st) {
        MGlobal::displayError(
            MString("workspaceControlCmd - could not initialize plugin: ")
            + st.errorString()
        );
        return st;
    }
    //  Register the command.
    st = pluginFn.registerCommand(WorkspaceControlCmd::commandName, WorkspaceControlCmd::creator, WorkspaceControlCmd::newSyntax);
    if (!st) {
        MGlobal::displayError(
            MString("workspaceControlCmd - could not register '")
            + WorkspaceControlCmd::commandName + "' command: "
            + st.errorString()
        );
        return st;
    }
    return st;
}

MStatus uninitializePlugin(MObject plugin)
{
    MStatus     st;
    MFnPlugin   pluginFn(plugin, "Autodesk, Inc.", "1.0", "Any", &st);
    if (!st) {
        MGlobal::displayError(
            MString("workspaceControlCmd - could not uninitialize plugin: ")
            + st.errorString()
        );
        return st;
    }
    //  Make sure that there is no UI left hanging around.
    WorkspaceControlCmd::cleanup();
    //  Deregister the command.
    st = pluginFn.deregisterCommand(WorkspaceControlCmd::commandName);
    if (!st) {
        MGlobal::displayError(
            MString("workspaceControlCmd - could not deregister '")
            + WorkspaceControlCmd::commandName + "' command: "
            + st.errorString()
        );
        return st;
    }
    return st;
}
