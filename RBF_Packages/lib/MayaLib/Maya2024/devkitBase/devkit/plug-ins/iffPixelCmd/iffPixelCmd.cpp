//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
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
// Produces the MEL command "iffPixel".
// 
// This command takes as arguments the name of an IFF file and the x and y co-ordinates of a pixel in the image.
// It returns the r/g/b/a values at that pixel. For example: "iffPixel sphere.iff 100 210". 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxCommand.h>
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include "iffreader.h"


#ifndef __APPLE__
#include <maya/MIOStream.h>
#endif

#define IFFCHECKERR(stat, call) \
if (!stat) { \
	MString string = reader.errorString(); \
    string += " in method "; \
	string += #call; \
    displayError (string); \
	return MS::kFailure; \
}

class iffPixel : public MPxCommand
{
public:
                iffPixel();
        ~iffPixel() override;

    MStatus     doIt ( const MArgList& args ) override;
    MStatus     redoIt () override;
    MStatus     undoIt () override;
    bool        isUndoable() const override;

    static      void* creator();

private:
	int         r,g,b,a;
	float       d;
	bool        useDepth;
};

iffPixel::iffPixel()
{
}

iffPixel::~iffPixel() {}

void* iffPixel::creator()
{
    return new iffPixel;
}

bool iffPixel::isUndoable() const
{
    return true;
}

MStatus iffPixel::undoIt()
{
    return MS::kSuccess;
}

MString itoa (int n)
{
	char buffer [256];
	sprintf (buffer, "%d", n);
	return MString (buffer);
}

MStatus iffPixel::doIt( const MArgList& args )
{
    MString componentName;
	if (args.length () < 3 || args.length () > 4) {
		displayError ("Syntax: iffPixel file x y [-depth]");
		return MS::kFailure;
	}

	int x,y;
	MString fileName;

	args.get (0, fileName);
	args.get (1, x);
	args.get (2, y);
	if (args.length () == 4)
	{
		MString lastArg;
		args.get (3, lastArg);
		if (lastArg != MString ("-depth")) {
			displayError ("Syntax: iffPixel file x y [-depth]");
			return MS::kFailure;
		}
		useDepth = true;
	}
	else
		useDepth = false;

	IFFimageReader reader;
	MStatus stat;

	stat = reader.open (fileName);
	IFFCHECKERR (stat, open);

	int w,h;
	stat = reader.getSize (w,h);
	IFFCHECKERR (stat, getSize);
	if (x < 0 || x > w || y < 0 || y > h) {
		MString message ("Co-ordinates out of range. Size of image is ");
		message += itoa (w);
		message += "+";
		message += itoa (h);
		displayError (message);
		return MS::kFailure;
	}

	stat = reader.readImage ();
	IFFCHECKERR (stat, readImage);
	if (useDepth)
	{
		if (!reader.hasDepthMap ()) {
			displayError ("Image has no depth map");
			return MS::kFailure;
		}
		stat = reader.getDepth (x,y,&d);
	}
	else
	{
		if (!reader.isRGB () && !reader.isGrayscale ()) {
			displayError ("Image has no RGB data");
			return MS::kFailure;
		}
		stat = reader.getPixel (x,y,&r,&g,&b,&a);
	}
	IFFCHECKERR (stat, getPixel);

	stat = reader.close ();
	IFFCHECKERR (stat, close);

    return redoIt();
}

MStatus iffPixel::redoIt()
{
    clearResult();
	if (useDepth) {
		appendToResult (d);
	} else {
		appendToResult( r );
		appendToResult( g );
		appendToResult( b );
		appendToResult( a );
	}

    return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");
    status = plugin.registerCommand( "iffPixel", iffPixel::creator );

	if (!status)
		status.perror("registerCommand");

    return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus status;
    MFnPlugin plugin( obj );
    status = plugin.deregisterCommand( "iffPixel" );

	if (!status)
		status.perror("deregisterCommand");

    return status;
}
