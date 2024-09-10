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
// Produces the MEL command "iffPpm".
//
// This command takes as arguments the names of an existing IFF file and the name of a PPM (portable pixmap)
// file that it must create. The IFF image is read and written out in PPM format to the second file.
// For example: "iffPpm sphere.iff sphere.ppm".
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
#include <float.h>
#include "iffreader.h"

#include <iostream>
#include <fstream>

#define IFFCHECKERR(stat, call) \
if (!stat) { \
	MString string = reader.errorString(); \
    string += " in method "; \
	string += #call; \
    displayError (string); \
	return MS::kFailure; \
}

class iffPpm : public MPxCommand
{
public:
                iffPpm();
        ~iffPpm() override;

    MStatus     doIt ( const MArgList& args ) override;
    MStatus     redoIt () override;
    MStatus     undoIt () override;
    bool        isUndoable() const override;

    static      void* creator();

private:
	MString     ppmFile;
	MString     fileName;
	bool        useDepth;
};

iffPpm::iffPpm()
{
}

iffPpm::~iffPpm() {}

void* iffPpm::creator()
{
    return new iffPpm;
}

bool iffPpm::isUndoable() const
{
    return true;
}

MStatus iffPpm::undoIt()
{
    return MS::kSuccess;
}

MString itoa (int n)
{
	char buffer [256];
	sprintf (buffer, "%d", n);
	return MString (buffer);
}

MStatus iffPpm::doIt( const MArgList& args )
{
    MString componentName;
	if (args.length () < 2 || args.length () > 3) {
		displayError ("Syntax: iffPpm ifffile ppmfile [-depth]");
		return MS::kFailure;
	}

	args.get (0, fileName);
	args.get (1, ppmFile);
	if (args.length () == 3)
	{
		MString lastArg;
		args.get (2, lastArg);
		if (lastArg != MString ("-depth")) {
			displayError ("Syntax: iffPpm ifffile ppmfile [-depth]");
			return MS::kFailure;
		}
		useDepth = true;
	}
	else
		useDepth = false;

    return redoIt();
}

MStatus iffPpm::redoIt()
{
    clearResult();

	IFFimageReader reader;
	MStatus stat;

	stat = reader.open (fileName);
	IFFCHECKERR (stat, open);

	int imageWidth,imageHeight,bytesPerChannel;
	stat = reader.getSize (imageWidth,imageHeight);
	IFFCHECKERR (stat, getSize);

	bytesPerChannel = reader.getBytesPerChannel ();

	stat = reader.readImage ();
	IFFCHECKERR (stat, readImage);

    std::ofstream out (ppmFile.asChar ());
	if (!out.good ())
	{
		displayError ("Could not create output file");
		return MS::kFailure;
	}
	out << "P3" << std::endl << imageWidth << " " << imageHeight << std::endl;
	if (useDepth) {
		if (!reader.hasDepthMap ()) {
			displayError ("Image has no depth map");
			return MS::kFailure;
		}

		// Step 1: calculate the range of depth values in the data.
		// We'll normalize against this range.
		float minDepth=FLT_MAX, maxDepth=(-FLT_MAX);

		const float *depthMap = reader.getDepthMap ();
		for(int index=0; index<imageWidth*imageHeight; index++)
		{
			float depth=depthMap[index];
			if (depth!=0.) // 0 values indicate nothing there
			{
				float realDepth= -1.0f/depth;
				if (realDepth<minDepth)
					minDepth=realDepth;
				if (realDepth>maxDepth)
					maxDepth=realDepth;
			}
		}

		// Step 2: output data, normalizing to 0-255

		out << "255" << std::endl;
		float scaleFactor = (float) (255.0 / ((double)maxDepth - (double)minDepth));
		float offset = minDepth * scaleFactor;

		const float *entry = depthMap;

		for(int y=0; y < imageHeight; y++)
			for(int x=0; x < imageWidth; x++, entry++)
			{
				if (*entry == 0.)
					out << "0 0 0" << std::endl;
				else {
					float realDepth = -scaleFactor / *entry - offset;
					out << (int)realDepth << " " << (int)realDepth << " " <<
						(int) realDepth << std::endl;
				}
			}
	} else {
		if (!reader.isRGB () && !reader.isGrayscale ()) {
			displayError ("Image has no RGB data");
			return MS::kFailure;
		}
		if (bytesPerChannel==1)
			out << "255" << std::endl;
		else
			out << "65535" << std::endl;
		const unsigned char *bitmap = reader.getPixelMap ();
		const unsigned char *pixel = bitmap;
		int bytesPerPixel = (bytesPerChannel == 1) ? 4 : 8;
		for(int y=0; y < imageHeight; y++)
			for(int x=0; x < imageWidth; x++, pixel += bytesPerPixel)
			{
				// Note that if the image was greyscale then the ILload
				// function will have expanded the grey into rgb now.
				
				//
				if (bytesPerChannel==1)
				{
#if defined(_WIN32) || defined(__linux__)
					// The actual internal storage on NT is BGRA
					out << (int)pixel[2] << " " <<
						(int)pixel[1] << " " <<
						(int)pixel[0] << std::endl;
#else
					// The actual internal storage on IRIX is ABGR
					out << (int)pixel[3] << " " <<
						(int)pixel[2] << " " <<
                        (int)pixel[1] << std::endl;
#endif
				}
				else // 16 bit
				{
#if defined(_WIN32) || defined(__linux__)
					out	<< ((int)pixel[4]<<8)+(int)pixel[5] << " " 
						<< ((int)pixel[2]<<8)+(int)pixel[3] << " " 
						<< ((int)pixel[0]<<8)+(int)pixel[1] << std::endl;
#else
					out	<< ((int)pixel[6]<<8)+(int)pixel[7] << " " 
						<< ((int)pixel[4]<<8)+(int)pixel[5] << " " 
						<< ((int)pixel[2]<<8)+(int)pixel[3] << std::endl;
#endif
				}
			}
	}

	IFFCHECKERR (stat, getPixel);

	stat = reader.close ();
	IFFCHECKERR (stat, close);

    return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");
    status = plugin.registerCommand( "iffPpm", iffPpm::creator );

	if (!status)
		status.perror("registerCommand");

    return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus status;
    MFnPlugin plugin( obj );
    status = plugin.deregisterCommand( "iffPpm" );

	if (!status)
		status.perror("deregisterCommand");

    return status;
}
