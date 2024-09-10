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
// Produces the dependency graph node "jitter".
//
// This plug-in is an example of a user-defined procedural dependency graph node.
// It is primarily oriented toward animation, but can be used to add noise in any
// connection between two float attributes. It takes a float value as input, 
// adds a pseudo-random value to the input, and outputs the noisy float value.
// For example, if the output of a parameter curve node is connected to the input
// of the jitter node and the output of the jitter node is connected to
// the translateX attribute of a surface, the motion of the surface will "jitter"
// parallel to the X axis.
//
// The node has one other input, "time". The output of the time slider node, "time1.outTime",
// must be connected to the time attribute on the jitter node if the "jittered" animation
// is to be repeatable. The attribute "scale" can be used to increase or decrease the
// magnitude of the random offset applied to the input of the jitter node.
// 	
// Once the plug-in is loaded, the following MEL command creates the jitter node "jitter1",
// attaches the attribute "someNode1.outFloat" to "jitter1.input", and attaches 
// "jitter1.output" to "someNode2.inFloat":
//
//	jitter "jitter1" "someNode1.outFloat" "someNode2.inFloat";
// 
// It also attaches the time slider output "time1.outTime" to "jitter1.time" and "jitter2.time".
// Additionally, it creates two windows with sliders for adjusting the scale.
//
// The jitter node can be easily demonstrated in conjunction with the circleNode plug-in.
// Load the circleNode and jitterNode plug-ins. Then, execute the following commands:
//
//	source circleNode
//	source jitterNode
//	createSphereAndAttachCircleNode;
//	jitter "jitter1" "circleNode1.sineOutput" "sphere1.translateX";
//	jitter "jitter2" "circleNode1.cosineOutput" "sphere1.translateZ";
//
// Clicking the "play" icon on the time slider will cause the sphere to move along a "jittered" circle.
// The amount of jitter can be varied by adjusting the scale sliders in the 
// "jitter1 Scale Editor" and "jitter2 Scale Editor" windows.
//
// Attributes ( < input, > output ):
// 
//	< input: the input float value
//	< scale: the scalar for the random value (0 - 1.0)
//	< time: the frame number
// 
//	> output: the randomized float value
// 
// Usage:
// 
//	Use "jitterNode.mel" to insert this DG node between
//	a float value connection in your DG.
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <stdio.h>
#include <maya/MIOStream.h>

#include <maya/MPxNode.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MVector.h>

#include <maya/MFnPlugin.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

static unsigned int xRand, yRand, zRand;     /* seed */

float randomd()
{
    float result;
    unsigned int a = xRand/177, b = xRand%177;
    unsigned int c = yRand/176, d = yRand%176;
    unsigned int e = zRand/178, f = zRand%178;

    xRand = (171 * b - 2 * a) % 30269;
    yRand = (172 * d - 35 * c) % 30307;
    zRand = (170 * f - 63 * e) % 30323;

    result = (float) xRand/30269.0f + yRand/30307.0f + zRand/30323.0f;
    return result - ((int) result);
}

void seedd(unsigned char nx, unsigned char ny, unsigned char nz)
{
    xRand = nx;
    yRand = ny;
    zRand = nz;
    randomd();
    randomd();
    randomd();
    randomd();
}

class jitter : public MPxNode
{
public:
	jitter();
	~jitter() override; 

		MStatus		compute( const MPlug& plug, MDataBlock& data ) override;

	static	void*		creator();
	static	MStatus		initialize();
 
	static	MObject		time;		// The time value.
	static	MObject		scale;		// Scale of jitter.

	static	MObject		input;		// The input value.
	static	MObject		output;		// The jittered-output value.

	static	MTypeId		id;
};

MTypeId     jitter::id( 0x80009 );
MObject		jitter::time;
MObject		jitter::scale;
MObject     jitter::input;
MObject     jitter::output; 

void* jitter::creator()
{
	return new jitter();
}

MStatus jitter::initialize()
{
	MFnNumericAttribute nAttr;
	MStatus				stat;

	time = nAttr.create( "time", "tm", MFnNumericData::kFloat, 0.0 );
 	nAttr.setStorable(true);

	scale = nAttr.create( "scale", "sc", MFnNumericData::kFloat, 1.0 );
	nAttr.setStorable(true);

	input = nAttr.create( "input", "in", MFnNumericData::kFloat, 0.0 );
	nAttr.setStorable(true);

	output = nAttr.create( "output", "out", MFnNumericData::kFloat, 0.0 );
	nAttr.setWritable(false);
	nAttr.setStorable(false);

	stat = addAttribute( time );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( scale );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( input );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}

    stat = attributeAffects( time, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
    stat = attributeAffects( scale, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
    stat = attributeAffects( input, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}

	return MS::kSuccess;
} 

jitter::jitter() {}

jitter::~jitter() {}

// Compute the offset and add it to input
// as the output from this node.
MStatus jitter::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus returnStatus;
 
	if( plug == output )
	{
		MDataHandle timeData = data.inputValue( time, &returnStatus );
		MDataHandle scaleData = data.inputValue( scale, &returnStatus );
		MDataHandle inputData = data.inputValue( input, &returnStatus );

		if( returnStatus != MS::kSuccess )
			cerr << "ERROR getting data\n";
		else
		{
			float currentFrame = timeData.asFloat();
			float scaleFactor  = scaleData.asFloat();
			float inValue = inputData.asFloat();

			// This is where we jitter the value

			unsigned char seed = (unsigned char)currentFrame;
			seedd( seed, seed * 17, seed * 23 );

			float tmp = randomd();
			float result = ( tmp - 0.5f ) * scaleFactor + inValue;

			MDataHandle outHandle = data.outputValue( jitter::output );

			outHandle.set( result );

			data.setClean(plug);
		}
	}
	else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "jitter", jitter::id, 
						 jitter::creator, jitter::initialize  );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus	  status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( jitter::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
