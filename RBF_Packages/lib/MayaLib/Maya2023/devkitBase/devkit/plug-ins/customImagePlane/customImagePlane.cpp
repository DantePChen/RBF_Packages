//-
// ==========================================================================
// Copyright 2019 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// Description: 
//   Demonstrates how to create your own custom image plane based on
//   Maya's internal image plane classes. This allows API users to
//   override the default Maya image plane behavior. This class works
//   like typical API nodes in that it can have a compute method and
//   can contain static attributes added by the API user.  This
//   example class overrides the default image plane behavior and
//   allows users to add transparency to an image plane using the
//   transparency attribute on the node. Note, this code also 
//   illustrates how to use MImage to control the floating point
//   depth buffer.  When useDepthMap is set to true, depth is added
//   is added to the image such that half of the image is at the near 
//   clip plane and the remaining half is at the far clip plane. 
//
//   Note, once the image plane node has been created it you must
//   attached it to the camera shape that is displaying the node.
//   You need to use the imagePlane command to attach the image plane 
//   you created to the specified camera.
//
//   This example works only with renderers that use node evaluation
//   as a part of the rendering process, e.g. Maya Software. It does 
//   not work with 3rd party renderers that rely on a scene translation 
//   mechanism.
// 
//   For example, 
//     string $imageP = `createNode customImagePlane`;
//     imagePlane -edit -camera "persp" $imageP
//    

#include <maya/MPxImagePlane.h>
#include <maya/MFnPlugin.h>
#include <maya/MImage.h>
#include <maya/MString.h>
#include <maya/MFnNumericAttribute.h> 
#include <maya/MFnNumericData.h> 
#include <maya/MDataHandle.h> 
#include <maya/MPlug.h> 
#include <maya/MEventMessage.h>
#include <maya/MGlobal.h>


class customImagePlane : public MPxImagePlane
{
public:
						customImagePlane();
	virtual				~customImagePlane();

	MStatus				loadImageMap( const MString &fileName, int frame, MImage &image ) override;

	bool				getInternalValue( const MPlug&, MDataHandle&)		override;
	bool				setInternalValue( const MPlug&, const MDataHandle&) override;

	static  void*		creator();
	static  MStatus		initialize();

	static	MTypeId		id;				// The IFF type id
	static  MObject		aTransparency;

	static void timeChangedCallback( void * );

private: 
	double				fTransparency;
	MCallbackId			fTimeChangedCallbackId;
};

MObject customImagePlane::aTransparency; 

customImagePlane::customImagePlane()
: MPxImagePlane()
, fTransparency( 0.0 )
, fTimeChangedCallbackId( 0 )
{
	// Uncomment the following line if you need a workaround which updates the
	// transparency of the image plane during playback or srubbing with the 
	// Time Slider.
	//
	//fTimeChangedCallbackId = MEventMessage::addEventCallback(
	//	"timeChanged", &timeChangedCallback, this);
}

customImagePlane::~customImagePlane()
{
	if (fTimeChangedCallbackId != 0) {

		MMessage::removeCallback(fTimeChangedCallbackId);
		fTimeChangedCallbackId = 0;
	}
}

bool
customImagePlane::getInternalValue( const MPlug &plug, MDataHandle &handle)
{
	if ( plug == aTransparency ) {
		handle.set( fTransparency );
		return true; 
	}

	return MPxImagePlane::getInternalValue( plug, handle );
}

bool
customImagePlane::setInternalValue( const MPlug &plug, const MDataHandle &handle)
{
	if ( plug == aTransparency ) {
		fTransparency = handle.asDouble();
		setImageDirty();
		return true;
	}

	return MPxImagePlane::setInternalValue( plug, handle );
}

MStatus
customImagePlane::loadImageMap( const MString &fileName, int frame, MImage &image )
{
	image.readFromFile(fileName);

	unsigned int width, height;
	image.getSize(width, height);
	unsigned int size = width * height; 

	unsigned char *pixels = image.pixels(); 
	unsigned int i; 
	for ( i = 0; i < size; i ++, pixels += 4 ) { 
		pixels[3] = (unsigned char)(pixels[3] * (1.0 - fTransparency));
	}

	MPlug depthMap( thisMObject(), useDepthMap ); 
	bool value; 
	depthMap.getValue( value ); 
	
	if ( value ) {
		float *buffer = new float[width*height];
		unsigned int c, j; 
		for ( c = i = 0; i < height; i ++ ) { 
			for ( j = 0; j < width; j ++, c++ ) { 
				if ( i > height/2 ) { 
					buffer[c] = -1.0f;
				} else { 
					buffer[c] = 0.0f;
				}
			}
		}
		image.setDepthMap( buffer, width, height ); 
		delete [] buffer;
	}

	return MStatus::kSuccess;
}

MTypeId customImagePlane::id( 0x1A19 );

void*
customImagePlane::creator()
{
	return new customImagePlane;
}

MStatus
customImagePlane::initialize()
{
	MFnNumericAttribute nAttr; 

	aTransparency = nAttr.create( "transparency", "tp", 
								  MFnNumericData::kDouble, 0 );
	nAttr.setStorable(true); 
	nAttr.setInternal(true); 
	nAttr.setMin(0.0); 
	nAttr.setMax(1.0); 
	nAttr.setDefault(0.0);
	nAttr.setKeyable(true); 

	addAttribute( aTransparency ); 

	return MStatus::kSuccess;
}

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "7.0", "Any");

	MStatus status = plugin.registerNode( "customImagePlane", customImagePlane::id, 
								  customImagePlane::creator,
								  customImagePlane::initialize, 
								  MPxNode::kImagePlaneNode );
	if (!status) {
		status.perror("registerNode");
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	MStatus status = plugin.deregisterNode( customImagePlane::id );
	if (!status) {
		status.perror("deregisterNode");
	}

	return status;
}

void customImagePlane::timeChangedCallback(void *clientData)
{
	customImagePlane* imagePlane = (customImagePlane *) clientData;

	if (imagePlane) {

		MString cmd = "getAttr " + imagePlane->name() + ".tp";
		MGlobal::executeCommand(cmd);
	}
}