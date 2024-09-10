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
// This plug-in demonstrates how to use the MProgressWindow class.
// The command "progressWindowPlugin" displays a simple progress window
// which updates every second. The progress window can be terminated by hitting escape. 
//
// To run this plug-in:
//
//	progressWindowCmd;
//	
// A progress window will be displayed. Note that Maya can only display one progress
// window at a time. MEL also supports creating a progress window.
// Program errors may occur if the progress windows of MEL and the API are being called
// at the same time. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxCommand.h>
#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>
#include <maya/MProgressWindow.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>

class progressWindowPlugin : public MPxCommand
{
public:

	MStatus doIt(const MArgList &args) override;

	static void* creator();
};

void* progressWindowPlugin::creator()
{
	return new progressWindowPlugin;
}

MStatus progressWindowPlugin::doIt(const MArgList &args)
{
	MStatus stat = MS::kSuccess;
	MString title = "Doing Nothing";
	MString sleeping = "Sleeping: ";
	
	int amount = 0;
	int maxProgress = 10;
	
	// First reserve the progress window.  If a progress window is already
	// active (eg. through the mel "progressWindow" command), this command
	// fails.
	//
	if (!MProgressWindow::reserve())
	{
		MGlobal::displayError("Progress window already in use.");
		stat = MS::kFailure;
		return stat;
	}

	//
	// Set up and print progress window state
	//

	CHECK_MSTATUS(MProgressWindow::setProgressRange(amount, maxProgress));
	CHECK_MSTATUS(MProgressWindow::setTitle(title));
	CHECK_MSTATUS(MProgressWindow::setInterruptable(true));
	CHECK_MSTATUS(MProgressWindow::setProgress(amount));

	MString progressWindowState = MString("Progress Window Info:") +
		MString("\nMin: ") + MProgressWindow::progressMin() +
		MString("\nMax: ") + MProgressWindow::progressMax() + 
		MString("\nTitle: ") + MProgressWindow::title() + 
		MString("\nInterruptible: ") + MProgressWindow::isInterruptable();

	MGlobal::displayInfo(progressWindowState);
	
	CHECK_MSTATUS(MProgressWindow::startProgress());
	
	// Count 10 seconds
	//
	for (int i = amount; i < maxProgress; i++)
	{
		if (i != 0 && MProgressWindow::isCancelled()) {
			MGlobal::displayInfo("Progress interrupted!");
			break;
		}

		MString statusStr = sleeping;
		statusStr += i;
		CHECK_MSTATUS(MProgressWindow::setProgressStatus(statusStr));
		CHECK_MSTATUS(MProgressWindow::advanceProgress(1));

		MGlobal::displayInfo(MString("Current progress: ") + MProgressWindow::progress());

		MGlobal::executeCommand("pause -sec 1", false, false);
	}
	
	// End the progress, unreserving the progress window so it can be used
	// elsewhere.
	//
	CHECK_MSTATUS(MProgressWindow::endProgress());
	
	return stat;
}

// standard initialize and uninitialize functions

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "6.0", "Any");

    status = plugin.registerCommand( "progressWindowCmd", progressWindowPlugin::creator );
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

    status = plugin.deregisterCommand( "progressWindowCmd" );
	if (!status) {
		status.perror("progressWindowCmd");
	}
    return status;
}
