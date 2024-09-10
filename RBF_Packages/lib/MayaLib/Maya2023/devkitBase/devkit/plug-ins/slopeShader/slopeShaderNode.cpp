#include <math.h>
#include "slopeShaderNode.h"
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFloatVector.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnGenericAttribute.h>

#define AWdegreesToRadians 0.0174532925199432957692369076848f

MTypeId slopeShaderNode::id( 0x81005 );

//Input values
// 
MObject slopeShaderNode::aAngle;
MObject slopeShaderNode::aColor1;
MObject slopeShaderNode::aColor2;

//Rendering attributes
//
MObject slopeShaderNode::aTriangleNormalCamera;
MObject slopeShaderNode::aMatrixEyeToWorld;

//Output color
//
MObject slopeShaderNode::aOutColor;

//Dummy plug for forcing
//an evaluation of this node
//
MObject slopeShaderNode::aDirtyShaderAttr;

slopeShaderNode::slopeShaderNode()
//
//	Description:
//		Constructor
//
{
}

slopeShaderNode::~slopeShaderNode()
//
//	Description:
//		Descructor
//
{
}

void* slopeShaderNode::creator()
//
//	Description:
//		Returns a new instance of this class
//
{
    return new slopeShaderNode();
}

MStatus slopeShaderNode::initialize()
//
//	Description:
//		Initializes the attributes for this node.
//
{
     MFnNumericAttribute nAttr; 
	 MFnMatrixAttribute nMAttr;
	 MFnTypedAttribute nTAttr;
	 MFnGenericAttribute nGAttr;


	// Input Attributes
	//
    aAngle = nAttr.create( "angle", "ang", MFnNumericData::kFloat);
    nAttr.setDefault(30.0f);
    nAttr.setMin(0.0f);
    nAttr.setMax(100.0f);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);

    aColor1 = nAttr.createColor( "walkableColor", "w" );
    nAttr.setDefault(0.0f, 1.0f, 0.0f);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setUsedAsColor(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);

    aColor2 = nAttr.createColor( "nonWalkableColor", "nw" );
    nAttr.setDefault(1.0f, 0.0f, 0.0f);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setUsedAsColor(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);


	// Surface Normal supplied by the render sampler
	//
    aTriangleNormalCamera = nAttr.createPoint( "triangleNormalCamera", "n" );
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(true);

	//View matrix from the camera into world space
	//
	aMatrixEyeToWorld = nMAttr.create( "matrixEyeToWorld", "mew",
									   MFnMatrixAttribute::kFloat );
	nAttr.setHidden(true);
	nMAttr.setWritable(true);

	// Output Attributes
	//
    aOutColor  = nAttr.createColor( "outColor", "oc" );
    nAttr.setStorable(false);
    nAttr.setHidden(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);

	//dummy plug for forcing evaluation
	//
	aDirtyShaderAttr = nGAttr.create( "dirtyShaderPlug", "dsp");
	nGAttr.setArray(true);
	nGAttr.setHidden(false);
	nGAttr.setUsesArrayDataBuilder(true);
	nGAttr.setReadable(false);
	nGAttr.setStorable(true);
	nGAttr.setIndexMatters(false);
	nGAttr.addDataAccept(MFnData::kMesh);

	//Add attribues
    addAttribute(aAngle);
    addAttribute(aColor1);
    addAttribute(aColor2);
    addAttribute(aTriangleNormalCamera);
    addAttribute(aOutColor);
	addAttribute(aMatrixEyeToWorld);
	addAttribute(aDirtyShaderAttr);

    attributeAffects (aAngle,  aOutColor);
    attributeAffects (aColor1, aOutColor);
    attributeAffects (aColor2, aOutColor);
    attributeAffects (aTriangleNormalCamera,  aOutColor);
	attributeAffects (aDirtyShaderAttr, aOutColor);

    return MS::kSuccess;
}

MStatus slopeShaderNode::compute(const MPlug & plug, MDataBlock & block )
//
//	Description:
//		Computes a color value  
//	from a surface noraml angle.
//
{ 
	if ((plug != aOutColor) && (plug.parent() != aOutColor))
		return MS::kUnknownParameter;

    MFloatVector resultColor;

    MFloatVector&  walkable = block.inputValue( aColor1 ).asFloatVector();
    MFloatVector&  nonWalkable = block.inputValue( aColor2 ).asFloatVector();
    MFloatVector&  surfaceNormal = block.inputValue( aTriangleNormalCamera ).asFloatVector();
	MFloatMatrix&  viewMatrix = block.inputValue( aMatrixEyeToWorld ).asFloatMatrix();
    float angle = block.inputValue( aAngle ).asFloat();

    // Normalize the view vector
	//
	surfaceNormal.normalize();
	MFloatVector WSVector = surfaceNormal * viewMatrix;
	
    // find dot product
	//
    float scalarNormal = WSVector * MFloatVector(0, 1, 0);

    // take the absolute value
	//
    if (scalarNormal < 0.0) scalarNormal *= -1.0;

	if(cos(angle*AWdegreesToRadians) < scalarNormal)
		resultColor = walkable;
	else
		resultColor = nonWalkable;

    // set ouput color attribute
	//
    MDataHandle outColorHandle = block.outputValue( aOutColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

    return MS::kSuccess;
}

// =====================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// =====================================================================
