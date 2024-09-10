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
// Produces the MEL command "cvPos".
//
// This example demonstrates how to obtain the world or local space position of a NURBS CV or a polygonal vertex.
// 
// The command accepts the flags -l/-local or -w/-world (where world is the default) to indicate whether a local
// or world space location of the CV is required. The command can handle at most 1 CV per invocation because it returns
// the coordinate as 3 element (a MEL float array). If no arguments are provided to the command, it checks the
// active selection list. If exactly one CV or vertex is present in that list, it returns its location.
// If more than one is selected, an error occurs.
//
// Alternatively, a single component can be specified on the command line using the MEL syntax. For example:
// cvPos nurbsSphereShape1.cv[0][0];
// It returns the world space position of the specified CV. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxCommand.h>
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MString.h>
#include <maya/MPoint.h>

#include <maya/MItCurveCV.h>
#include <maya/MItSurfaceCV.h>
#include <maya/MItMeshVertex.h>

#include <maya/MIOStream.h>

class cvPos : public MPxCommand
{
public:
                cvPos();
        ~cvPos() override;

    MStatus     doIt ( const MArgList& args ) override;
    MStatus     redoIt () override;
    MStatus     undoIt () override;
    bool        isUndoable() const override;

    static      void* creator();

private:
    MPoint      point;
};

cvPos::cvPos() :
    point(0.0, 0.0, 0.0)
{
}

cvPos::~cvPos() {}

void* cvPos::creator()
{
    return new cvPos;
}

bool cvPos::isUndoable() const
{
    return true;
}

MStatus cvPos::undoIt()
{
    return MS::kSuccess;
}

MStatus cvPos::doIt( const MArgList& args )
{

    MString componentName;
	MSpace::Space transformSpace = MSpace::kWorld;

	for (unsigned int i = 0; i < args.length (); i++)
	{
		MString argStr;
		args.get (i, argStr);
		if (MString ("-l") == argStr || MString ("-local") == argStr)
			transformSpace = MSpace::kObject;
		else if (MString ("-w") == args.asString (i) ||
				 MString ("-world") == argStr)
			transformSpace = MSpace::kWorld;
		else
			componentName = argStr;
	}

    MObject     component;
    MDagPath    dagPath;

	if (!componentName.length ()) {
		MSelectionList activeList;

		MGlobal::getActiveSelectionList (activeList);

		MItSelectionList iter (activeList, MFn::kComponent);
		if (iter.isDone ())	{
			displayError ("No components selected");
			return MS::kFailure;
		} else {
			iter.getDagPath (dagPath, component);
			iter.next ();
			if (!iter.isDone ()) {
				displayError ("More than one component is selected");
				return MS::kFailure;
			}
		}
	} else {
		MSelectionList list;

		if (! list.add( componentName ) ) {
			componentName += ": no such component";
			displayError(componentName);
			return MS::kFailure; // no such component
		}
		MItSelectionList iter( list );
        iter.getDagPath( dagPath, component );
	}

	if (component.isNull()) {
		displayError("not a component");
		return MS::kFailure;
	}

	switch (component.apiType()) {
	case MFn::kCurveCVComponent:
		{
			MItCurveCV curveCVIter( dagPath, component );
			point = curveCVIter.position(transformSpace );
			curveCVIter.next();
			if (!curveCVIter.isDone()) {
				displayError ("More than one component is selected");
				return MS::kFailure;
			}
			break;
		}

	case MFn::kSurfaceCVComponent:
		{
			MItSurfaceCV surfCVIter( dagPath, component, true );
			point = surfCVIter.position(transformSpace );
			surfCVIter.next();
			if (!surfCVIter.isDone()) {
				displayError ("More than one component is selected");
				return MS::kFailure;
			}
			break;
		}

	case MFn::kMeshVertComponent:
		{
			MItMeshVertex vertexIter( dagPath, component );
			point = vertexIter.position(transformSpace );
			vertexIter.next();
			if (!vertexIter.isDone()) {
				displayError ("More than one component is selected");
				return MS::kFailure;
			}
			break;
		}

	default:
		cerr << "Selected unsupported type: (" << component.apiType()
			 << "): " << component.apiTypeStr() << endl;
	}

    return redoIt();
}

MStatus cvPos::redoIt()
{
    clearResult();
    appendToResult( point.x );
    appendToResult( point.y );
    appendToResult( point.z );

    return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerCommand( "cvPos", cvPos::creator );
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

    status = plugin.deregisterCommand( "cvPos" );
	if (!status) {
		status.perror("deregisterCommand");
		return status;
	}

    return status;
}
