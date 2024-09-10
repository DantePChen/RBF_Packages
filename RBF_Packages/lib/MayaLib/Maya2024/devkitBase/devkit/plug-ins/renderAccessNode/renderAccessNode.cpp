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
// Produces the dependency graph node "renderAccessNode".
//
// This example demonstrates how to work with render callbacks.
// The plug-in will register a render callback, a shadow cast
// callback, and a post-process callback. When a render starts, render
// callback will be invoked, providing info related to the render's size.
// Then, if shadow maps exist in the render, shadow cast callback will
// be invoked after shadow maps have been calculated, providing data to
// the shadow maps. Finally, after geometries have been rendered,
// post-render callback will be invoked providing pointers to the rendered pixels.
//
// The plug-in will modify the rendered image in post-process callback
// to demonstrate how to manipulate the pixels. The attribute pointWorld
// will be converted to screen space and back to test MRenderData::worldToScreen()
// and MRenderData::screenToWorld().
//
// Install the plugin and create a renderAccessNode. Set the
// ptWorld attribute to set the point to be converted from world
// space to screen space.
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h> 
#include <stdlib.h> 

#include <maya/MString.h>
#include <maya/MTypeId.h>

#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 

#include <maya/MFnNumericAttribute.h>

#include <maya/MFloatPoint.h>

#include <maya/MRenderData.h>
#include <maya/MRenderShadowData.h>
#include <maya/MRenderUtil.h>
#include <maya/MRenderCallback.h>

class myCallback2 : public MRenderCallback 
{
	public:
	myCallback2() {;}
	~myCallback2() override {;}
	void setObj(MObject &obj) { nodeObj = obj;}

	bool shadowCastCallback( const MRenderShadowData &data ) override;
	bool renderCallback( const MRenderData &data ) override;
	bool postProcessCallback( const MRenderData &data ) override;

	private:
	MObject nodeObj;
};

/////////////////////////////////
// Main Class //
/////////////////////////////////

class renderAccessNode : public MPxNode
{
	myCallback2 callback;

	public: // Methods

	renderAccessNode();
			~renderAccessNode() override;
	void			postConstructor() override;


	MStatus compute( const MPlug&, MDataBlock& ) override;

	static  void *          creator();
	static  MStatus         initialize();

	//  Id tag for use with binary file format
	//
	static MTypeId id;

	public:

	// Input
	static MObject     aSamples;
	static MObject     aDist;
	static MObject     aInColor;
	static MObject     aPointWorld;

	static MObject     aOutput;

};

// To get 3 float values from the node attribute
//
static void getFloat3(MObject node, MObject attr, MFloatPoint & val)
{
	// Get the attr to use
	//
	MObject object;
	MStatus status;
	MPlug	plug(node, attr);

	CHECK_MSTATUS(plug.getValue(object));

	MFnNumericData data(object, &status);
	CHECK_MSTATUS(status);

	CHECK_MSTATUS(data.getData(val[0], val[1], val[2]));
}


bool myCallback2::renderCallback( const MRenderData &data )
{
	MFloatPoint wp;
	getFloat3( nodeObj, renderAccessNode::aPointWorld, wp );

	fprintf( stderr, "\n*** Render Callback\n\n" );
	fprintf( stderr, "Image: %d %d\n", data.resX, data.resY );
	fprintf( stderr, "region rect: %d %d %d %d\n", 
		data.left, 
		data.bottom, 
		data.right, 
		data.top
	);
	return 0;
}

bool myCallback2::shadowCastCallback( const MRenderShadowData &data ) 
{
	MFloatPoint wp;
	getFloat3( nodeObj, renderAccessNode::aPointWorld, wp );

	fprintf( stderr, "\n*** Shadow cast Callback\n\n" );
	fprintf( stderr, "Light Position (%2.2f, %2.2f, %2.2f), size (%dx%d)\n", 
		data.lightPosition.x,
		data.lightPosition.y,
		data.lightPosition.z,
		data.shadowResX,
		data.shadowResY );

	fprintf( stderr, "Light type: %d, perspective: %d\n",
		data.lightType, data.perspective );

	MFloatPoint zp;
	data.worldToZbuffer( wp, zp );

	fprintf( stderr, "World Point( %g, %g, %g) = shadowmap (%g, %g, %g)\n\n",
		wp.x, wp.y, wp.z,
		zp.x, zp.y, zp.z );

	data.zbufferToWorld( zp, wp );

	fprintf( stderr, "shadowmap( %g, %g, %g ) = World Point( %g, %g, %g)\n\n",
		zp.x, zp.y, zp.z,
		wp.x, wp.y, wp.z );

	return 0;
}

bool myCallback2::postProcessCallback( const MRenderData &data ) 
{
	MFloatPoint wp;
	getFloat3( nodeObj, renderAccessNode::aPointWorld, wp );

	float screenZ;
	MPlug	plug(nodeObj, renderAccessNode::aDist );
	plug.getValue( screenZ );

	fprintf( stderr, "\n*** Post process Callback\n\n" );
	fprintf( stderr, "Image: %d %d\n", data.resX, data.resX );
	fprintf( stderr, "region rect: %d %d %d %d\n", 
		data.left, 
		data.bottom, 
		data.right, 
		data.top
	);
	fprintf( stderr, "Data size: %d x %d (%d)\n", data.xsize, data.ysize, data.bytesPerChannel );

	float r, g, b, a;

	r = 0.0;
	g = 255.0;
	b = 255.0;
	a = 255.0;

	//
	// modify rgba and depth values
	//

	short x, y;
	unsigned char *addr;

	for ( y = 0; y < data.ysize/2; y+= 3 )
	{
		for ( x = 0; x < data.xsize/2; x+= 3 )
		{
			addr = data.rgbaArr + ((data.xsize*y+x)*4*data.bytesPerChannel);
			if ( data.bytesPerChannel == 1 )
			{
				addr[0] = (char)a;	// a
				addr[1] = (char)b;	// b
				addr[2] = (char)g;	// g
				addr[3] = (char)r;	// r
			}
			else
			{
				addr[0] = (char)a;
				addr[1] = (char)(((a - (int)(a)) * 256.0));
				addr[2] = (char)b;
				addr[3] = (char)(((b - (int)(b)) * 256.0));
				addr[4] = (char)g;
				addr[5] = (char)(((g - (int)(g)) * 256.0));
				addr[6] = (char)r;
				addr[7] = (char)(((r - (int)(r)) * 256.0));

			}
			data.depthArr[ data.xsize*y+x ] = -0.25;
		}
	}
	MFloatPoint sp;
	
	data.worldToScreen( wp, sp );

	fprintf( stderr, "\nWorld point: ( %2.2f, %2.2f, %2.2f ) = Screen (%2.2f, %2.2f) depth: %2.2f\n\n",
		wp.x, wp.y, wp.z,
		sp.x, sp.y, sp.z );

	data.screenToWorld( sp, wp );

	fprintf( stderr, "Screen point: ( %2.2f, %2.2f, %2.2f ) = World (%2.2f, %2.2f, %2.2f)\n",
		sp.x, sp.y, sp.z,
		wp.x, wp.y, wp.z );

	fprintf( stderr, "\n" );
	return 0;
}


// attribute information

MTypeId renderAccessNode::id( 0x81018 );

// input

MObject     renderAccessNode::aSamples;
MObject     renderAccessNode::aDist;
MObject     renderAccessNode::aInColor;
MObject     renderAccessNode::aPointWorld;

MObject     renderAccessNode::aOutput;

renderAccessNode::renderAccessNode()
{}

renderAccessNode::~renderAccessNode()
{
	MRenderCallback::removeCallback( &callback );
}


void renderAccessNode::postConstructor()
{
	MObject obj = thisMObject();
	callback.setObj( obj );
	MRenderCallback::addCallback( &callback, 255 );
}

// creates an instance of the node
void * renderAccessNode::creator()
{
	return new renderAccessNode();
}

// initializes attribute information
MStatus renderAccessNode::initialize()
{
	MFnNumericAttribute nAttr; 

	// samples

	aSamples = nAttr.create( "samples", "spl", MFnNumericData::kShort, 0 );
	nAttr.setMin(0);
	CHECK_MSTATUS(addAttribute( aSamples ));

	// distance

	aDist = nAttr.create( "distance", "dis", MFnNumericData::kFloat, 0 );
	CHECK_MSTATUS(addAttribute( aDist ));

	// inColor

	aInColor = nAttr.createColor( "inColor", "ic" );
	nAttr.setStorable(false);
	CHECK_MSTATUS(addAttribute( aInColor ));

	// pointWorld

	aPointWorld = nAttr.createPoint( "worldPoint", "wp" );
	nAttr.setStorable(false);
	nAttr.setWritable(true);
	CHECK_MSTATUS(addAttribute( aPointWorld ));

	// Output

	aOutput = nAttr.create( "output", "out", MFnNumericData::kFloat, 1.0 );
	CHECK_MSTATUS(addAttribute( aOutput ));

	attributeAffects( aPointWorld, aOutput );
	attributeAffects( aInColor, aOutput );

	return MS::kSuccess;
}


MStatus renderAccessNode::compute( const MPlug& plug, MDataBlock& data)
{
	if ( plug == renderAccessNode::aOutput )
	{
		MDataHandle outHandle = data.outputValue( aOutput );
		float& of = outHandle.asFloat();
		of = 1.0f;

		outHandle.setClean();
	}

	return MS::kSuccess;
}


//
//
//

MStatus initializePlugin( MObject obj )
{ 
   const MString UserClassify( "utility/general" );

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");
	plugin.registerNode( "renderAccessNode", renderAccessNode::id,
                         renderAccessNode::creator, renderAccessNode::initialize,
                         MPxNode::kDependNode, &UserClassify );

   return MS::kSuccess;

}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	plugin.deregisterNode( renderAccessNode::id );

	return MS::kSuccess;
}
