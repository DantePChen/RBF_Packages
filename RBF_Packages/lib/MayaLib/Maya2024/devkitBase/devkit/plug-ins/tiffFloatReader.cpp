//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//
// Example floating point tif image file reader.
//
// This example demonstrates how to write a .tif float file reader using the API.
// In image reading menu dialogs of Maya, you can select *.* to see all images
// and then retrieve a tiff extension file item to load the .tif file into Maya.
//
///////////////////////////////////////////////////////////////////

#include <maya/MPxImageFile.h>
#include <maya/MImageFileInfo.h>
#include <maya/MImage.h>
#include <maya/MFnPlugin.h>
#include <maya/MStringArray.h>
#include <maya/MIOStream.h>
#include <maya/MGlobal.h>

// #pragma warning( disable : 4290 )		// Disable STL warnings.

#include "tiff.h"
#include "tiffio.h"

MString kImagePluginName( "TIFF Float Reader");

#define _TIFF_SUCCESS	1

class tiffFloatReader : public MPxImageFile
{
public:
			tiffFloatReader();
			~tiffFloatReader() override;
	static void*	creator();
	MStatus open( MString pathname, MImageFileInfo* info) override;
	MStatus load( MImage& image, unsigned int idx) override;
	MStatus close() override;

protected:
	unsigned int	fWidth;				// Width
	unsigned int	fHeight;			// Height
	unsigned int	fChannels;			// Number of channels

	TIFF			*fInputFile;		// Tif interface
};

//
// DESCRIPTION:
///////////////////////////////////////////////////////
tiffFloatReader::tiffFloatReader()
: fInputFile( NULL), 
  fChannels( 0), 
  fWidth(0),
  fHeight(0)
{

}

//
// DESCRIPTION:
//		Destructor : close the file if opened
///////////////////////////////////////////////////////
tiffFloatReader::~tiffFloatReader()
{
	close();
}

//
// DESCRIPTION:
//		Create a new instance of the reader
///////////////////////////////////////////////////////
void * tiffFloatReader::creator()
{
	return new tiffFloatReader();
}

//
// DESCRIPTION:
//		Open up the file for read. Return "info" if requested.
///////////////////////////////////////////////////////
MStatus tiffFloatReader::open( MString pathname, MImageFileInfo* info)
{
	try
	{
		// Open the tif file for read
		// (TIFF *) cast from integer required on Mac.
		fInputFile = (TIFF *) TIFFOpen( pathname.asChar(), "r" );
	}
	catch( ... )
	{
	}

	if( !fInputFile)
	{
		return MS::kFailure;
	}

	unsigned short num_samps;
	unsigned short bitsPerChannel;
	unsigned short sampleType = 0;
	short config;

	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_IMAGEWIDTH, &fWidth ) || 
		fWidth < 1)
		goto no_support;
	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_IMAGELENGTH, &fHeight ) || 
		fHeight < 1)
		goto no_support;


	// Suport 3 and 4 channel images only
	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_SAMPLESPERPIXEL, &num_samps))
		goto no_support;
	if ((num_samps != 3) && (num_samps!= 4))
		goto no_support;
	fChannels = num_samps;

	// This is more robust than TIFFTAG_SAMPLEFORMAT since it may not be supported
	// properly as it's an extension. 

	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_BITSPERSAMPLE, &bitsPerChannel))
		goto no_support;
	if (bitsPerChannel != 32)
		goto no_support;

	// Check the sample type. We only care about 32 bit 
	// floating point samples for this reader.
	// SAMPLEFORMAT_IEEEFP is specified as IEEE floating point.
	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_SAMPLEFORMAT, &sampleType) ||
		sampleType != SAMPLEFORMAT_IEEEFP)
	{
		goto no_support;
	}

	// See how the data is stored in the scan line. Only support
	// contiguous scan line for now.
	// - PLANARCONFIG_SEPARATE is not supported.
	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_PLANARCONFIG, &config) ||
		(config != PLANARCONFIG_CONTIG))
	{
		goto no_support;
	}

	// Compression not supported yet.. 
#if 0
	unsigned short compression;
	if (_TIFF_SUCCESS != TIFFGetField(fInputFile, TIFFTAG_COMPRESSION, &compression) ||
		(compression != COMPRESSION_NONE))
	{
		goto no_support;
	}
#endif

	//printf("Opened tif file successfully: w=%d,h=%d, ch=%d\n",
	//	fWidth, fHeight, fChannels );

	if( info)
	{
		// Set up the info structure to return
		//
		info->width( fWidth );
		info->height( fHeight );
		info->channels( fChannels );
		info->numberOfImages( 1 );
		info->pixelType( MImage::kFloat);
	}
	return MS::kSuccess;

no_support:
	close();
	return MS::kFailure;
}

//
// DESCRIPTION:
//		Close any open file
///////////////////////////////////////////////////////
/* virtual */
MStatus tiffFloatReader::close()
{
	if (fInputFile)
		TIFFClose( fInputFile );
	fInputFile = NULL;
	return MS::kSuccess;
}

//
// DESCRIPTION:
//		Load the image into system memory (MImage)
///////////////////////////////////////////////////////
MStatus tiffFloatReader::load( MImage& image, unsigned int imageNumber)
{
	MStatus rval = MS::kFailure;

	if (!fInputFile)
		return rval;

	// Configure our Maya image to hold the result
	image.create( fWidth, fHeight, fChannels, MImage::kFloat);
	float* outputBuffer = image.floatPixels();
	if (outputBuffer == NULL)
		return rval;
	
	// Maya expects images upside down
	unsigned int row = 0;
	bool flipVertically = true;
	if (flipVertically)
	{
		outputBuffer += (fHeight-1) * (fWidth * fChannels);
		for (row = 0; row < fHeight; row++)
		{
			TIFFReadScanline (fInputFile, outputBuffer, row);
			outputBuffer -= (fWidth * fChannels);
		}
	}
	else
	{
		for (row = 0; row < fHeight; row++)
		{
			TIFFReadScanline (fInputFile, outputBuffer, row);
			outputBuffer += (fWidth * fChannels);
		}
	}
	rval = MS::kSuccess;

	return rval;
}


//////////////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "2020", "Any" );
	MStringArray extensions;
	extensions.append( "tif");

	// This plugin is registered with a low priority. That means
	// this plugin will only be used if Maya is not able the load a TIF file
	// from a plugin with higher priority or by its internal code.
	CHECK_MSTATUS( plugin.registerImageFile( 
					kImagePluginName,
					tiffFloatReader::creator, 
					extensions,
					MFnPlugin::kImageFilePriorityLow));

	return MS::kSuccess;
}

// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	CHECK_MSTATUS( plugin.deregisterImageFile( kImagePluginName ) );

	return MS::kSuccess;
}

