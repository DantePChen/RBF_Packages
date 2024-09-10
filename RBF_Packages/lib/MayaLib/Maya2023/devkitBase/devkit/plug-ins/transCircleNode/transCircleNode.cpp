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
// Produces the dependency graph node "transCircle".
//
// This plug-in demonstrates how to use an attribute that contains
// multiple values (a compound attribute). The translate attribute of the
// transform node is used which is composed of the elements: translateX,
// translateY, and translateZ, all of which are communicated over a single
// dependency graph connection. 
//
// To use this node, execute the MEL command "transCircleNode.mel" that contains
// the following: 
//
//	createNode transCircle -n circleNode1;
//	sphere -n sphere1 -r 1;
//	sphere -n sphere2 -r 2;
//	connectAttr sphere2.translate circleNode1.inputTranslate;
//	connectAttr circleNode1.outputTranslate sphere1.translate;
//	connectAttr time1.outTime circleNode1.input;
//
// This creates two spheres and a transCircle node. The translate attribute of sphere1
// is connected to the input of the transCircle node, and its output is connected to
// the translate attribute of sphere2.
//
// If the play button is pressed, the second sphere will circle around the first.
// If the first sphere is moved, the second will also move such that the first sphere
// always remains at the center of the circle.
//
// This plug-in also comes with a sample of an attribute editor template.
// This example suppresses the display of all attributes except scale and frames, and also
// provides an extra quick set control for the scale attribute that uses radio buttons
// to update the attribute value. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxNode.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MFnPlugin.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

class transCircle : public MPxNode
{
public:
						transCircle() {};
					~transCircle() override {}; 

	MStatus		compute( const MPlug& plug, MDataBlock& data ) override;
	static  void*		creator();
	static  MStatus		initialize();

	static  MObject		input;            // The input value.
	static  MObject		inputTranslateX;  // The translate X value.  (input)
	static  MObject		inputTranslateY;  // The translate Y value.  (input)
	static  MObject		inputTranslateZ;  // The translate Z value.  (input)
	static  MObject		inputTranslate;
	static  MObject		outputTranslateX; // The translate X value.  (output)
	static  MObject		outputTranslateY; // The translate Y value.  (output)
	static  MObject		outputTranslateZ; // The translate Z value.  (output)
	static  MObject		outputTranslate;
	static  MObject		frames;           // Number of frames for one circle.
	static  MObject		scale;            // Size of circle.
	static  MTypeId		id;
};

MTypeId	transCircle::id( 0x80013 );
MObject	transCircle::input;        
MObject	transCircle::frames;
MObject	transCircle::scale;
MObject	transCircle::inputTranslate;
MObject	transCircle::outputTranslate;
MObject	transCircle::inputTranslateX;
MObject	transCircle::inputTranslateY;
MObject	transCircle::inputTranslateZ;
MObject	transCircle::outputTranslateX;
MObject	transCircle::outputTranslateY;
MObject	transCircle::outputTranslateZ;

void* transCircle::creator()
{
    return new transCircle;
}

MStatus transCircle::initialize()
{
	MFnNumericAttribute nAttr;
	MStatus				stat;

	input = nAttr.create( "input", "in", MFnNumericData::kDouble, 0.0 );
	nAttr.setStorable(true);

	inputTranslateX = nAttr.create( "inputTranslateX", "itX", 
									MFnNumericData::kDouble, 0.0 );
	nAttr.setStorable(true);

	inputTranslateY = nAttr.create( "inputTranslateY", "itY", 
									MFnNumericData::kDouble, 0.0 );
	nAttr.setStorable(true);

	inputTranslateZ = nAttr.create( "inputTranslateZ", "itZ", 
									MFnNumericData::kDouble, 0.0 );
	nAttr.setStorable(true);

	inputTranslate = nAttr.create( "inputTranslate", "it", 
									inputTranslateX,
									inputTranslateY,
									inputTranslateZ );
	nAttr.setStorable(true);
	nAttr.setDefault(0.0,0.0,0.0);

	outputTranslateX = nAttr.create( "outputTranslateX", "otX",
									MFnNumericData::kDouble, 0.0 );
	nAttr.setWritable(false);
	nAttr.setStorable(true);

	outputTranslateY = nAttr.create( "outputTranslateY", "otY",
									MFnNumericData::kDouble, 0.0 );
	nAttr.setWritable(false);
	nAttr.setStorable(true);

	outputTranslateZ = nAttr.create( "outputTranslateZ", "otZ",
									MFnNumericData::kDouble, 0.0 );
	nAttr.setWritable(false);
	nAttr.setStorable(true);

	outputTranslate = nAttr.create( "outputTranslate", "ot", 
									outputTranslateX,
									outputTranslateY,
									outputTranslateZ );
	nAttr.setWritable(false);
	nAttr.setStorable(true);
	nAttr.setDefault(0.0,0.0,0.0);

	scale = nAttr.create( "scale", "sc",
									MFnNumericData::kDouble, 10.0 );
	nAttr.setStorable(true);

	frames = nAttr.create( "frames", "fr",
									MFnNumericData::kDouble, 48.0 );
	nAttr.setStorable(true);

	stat = addAttribute( inputTranslate );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( input );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( scale );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( frames );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( outputTranslate );
		if (!stat) { stat.perror("addAttribute"); return stat;}

	stat = attributeAffects( inputTranslateX, outputTranslateX );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( inputTranslateY, outputTranslateY );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( inputTranslateZ, outputTranslateZ );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( inputTranslate, outputTranslateX );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( inputTranslate, outputTranslateY );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( inputTranslate, outputTranslateZ );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( inputTranslate, outputTranslate );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( input, outputTranslateX );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( input, outputTranslateY );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( scale, outputTranslateX );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( scale, outputTranslateY );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( frames, outputTranslateX );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( frames, outputTranslateY );
		if (!stat) { stat.perror("attributeAffects"); return stat;}

	return MS::kSuccess;
}

MStatus transCircle::compute( const MPlug& plug, MDataBlock& data )
{
    MStatus stat;
 
	bool k = ( plug == outputTranslateX ) |
	         ( plug == outputTranslateY ) |
			 ( plug == outputTranslateZ ) |
			 ( plug == outputTranslate );
	if (!k) return MS::kUnknownParameter;

	MDataHandle inputData = data.inputValue( input, &stat );
	if (!stat) { stat.perror("inputValue: input"); return stat;}

	MDataHandle scaleData = data.inputValue( scale, &stat );
	if (!stat) { stat.perror("inputValue: scale"); return stat;}

	MDataHandle framesData = data.inputValue( frames, &stat );
	if (!stat) { stat.perror("inputValue: frames"); return stat;}

	MDataHandle transData  = data.inputValue( inputTranslate, &stat ); 
	if (!stat) { stat.perror("inputValue: inputTranslate"); return stat;}

	double3& iTranslate = transData.asDouble3();
	double currentFrame = inputData.asDouble();
	double scaleFactor  = scaleData.asDouble();
	double framesPerCircle = framesData.asDouble();
	double angle = 6.2831853 * ( currentFrame/framesPerCircle );
	double3 oTranslate;
	
	oTranslate[0] = iTranslate[0] + (sin( angle ) * scaleFactor);
	oTranslate[1] = iTranslate[1] + 1.0;
	oTranslate[2] = iTranslate[2] + (cos( angle ) * scaleFactor);

	MDataHandle otHandle = data.outputValue( outputTranslate ); 
	otHandle.set( oTranslate[0], oTranslate[1], oTranslate[2] );
	data.setClean(plug);

    return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerNode( "transCircle", transCircle::id,
						 transCircle::creator, transCircle::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterNode( transCircle::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

    return status;
}
