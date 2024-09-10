#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>

///////////////////////////////////////////////////////////////////
// DESCRIPTION: 
//	Produces dependency graph node CompositeNode

// This node is an example of a compositing shading node.
// The inputs for this node are foreground, background, back color, and a mask value.
// The outputs for this node are "outColor" and "outAlpha". 
// To use this shader, create a CompositeNode and connect its output to an input of a surface/shader node such as Color. 
//
///////////////////////////////////////////////////////////////////


class myComp : public MPxNode
{
	public:
	         myComp();
	~myComp() override;

	MStatus compute( const MPlug&, MDataBlock& ) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

	static  void *  creator();
	static  MStatus initialize();

	//  Id tag for use with binary file format
    static  MTypeId id;

	private:

	// Input attributes

	static MObject aForegroundColor;
	static MObject aBackgroundColor;
	static MObject aBackColor;
	static MObject aMask;

	// Output attributes
	static MObject aOutColor;
	static MObject aOutAlpha;
};

// Static data
MTypeId myComp::id( 0x8100c );

// Attributes
MObject         myComp::aForegroundColor;
MObject         myComp::aBackgroundColor;
MObject         myComp::aBackColor;
MObject         myComp::aMask;
 
MObject         myComp::aOutColor;
MObject         myComp::aOutAlpha;

#define MAKE_INPUT(attr)			\
    CHECK_MSTATUS( attr.setKeyable(true)); 		\
	CHECK_MSTATUS( attr.setStorable(true));		\
    CHECK_MSTATUS( attr.setReadable(true)); 	\
	CHECK_MSTATUS( attr.setWritable(true));

#define MAKE_OUTPUT(attr)			\
    CHECK_MSTATUS( attr.setKeyable(false)); 	\
	CHECK_MSTATUS( attr.setStorable(false));	\
    CHECK_MSTATUS( attr.setReadable(true)); 	\
	CHECK_MSTATUS( attr.setWritable(false));

myComp::myComp()
{
}

myComp::~myComp()
{
}

// creates an instance of the node
void * myComp::creator()
{
    return new myComp();
}

// initializes attribute information
MStatus myComp::initialize()
{
    MFnNumericAttribute nAttr; 

    // Create input attributes

    aForegroundColor = nAttr.createColor("foreground", "fg");
    MAKE_INPUT(nAttr);

    aBackgroundColor = nAttr.createColor("background", "bg");
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(1., 1., 1.));

    aBackColor = nAttr.createColor("backColor", "bk");
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(1., 1., 1.));

    aMask = nAttr.create( "mask", "ms", MFnNumericData::kFloat);
    CHECK_MSTATUS( nAttr.setMin(0.f));
    CHECK_MSTATUS( nAttr.setMax(1.f));
    MAKE_INPUT(nAttr);

	// Create output attributes here
    aOutColor = nAttr.createColor("outColor", "oc");
	MAKE_OUTPUT(nAttr);

    aOutAlpha = nAttr.create( "outAlpha", "oa", MFnNumericData::kFloat);
	MAKE_OUTPUT(nAttr);

    // Add the attributes here
    CHECK_MSTATUS( addAttribute(aForegroundColor) );
    CHECK_MSTATUS( addAttribute(aBackgroundColor) );
    CHECK_MSTATUS( addAttribute(aBackColor) );
    CHECK_MSTATUS( addAttribute(aMask) );

    CHECK_MSTATUS( addAttribute(aOutColor) );
    CHECK_MSTATUS( addAttribute(aOutAlpha) );

    // all input affect the output color and alpha
    CHECK_MSTATUS( attributeAffects(aForegroundColor, aOutColor) );
    CHECK_MSTATUS( attributeAffects(aForegroundColor, aOutAlpha) );

    CHECK_MSTATUS( attributeAffects(aBackgroundColor, aOutColor) );
    CHECK_MSTATUS( attributeAffects(aBackgroundColor, aOutAlpha) );

    CHECK_MSTATUS( attributeAffects(aBackColor, aOutColor) );
    CHECK_MSTATUS( attributeAffects(aBackColor, aOutAlpha) );

    CHECK_MSTATUS( attributeAffects(aMask, aOutColor) );
    CHECK_MSTATUS( attributeAffects(aMask, aOutAlpha) );

    return MS::kSuccess;
}


//
// This function gets called by Maya to evaluate the texture.

MStatus myComp::compute(const MPlug& plug, MDataBlock& block)
{
	// outColor or individial R, G, B channel
    if((plug != aOutColor) && (plug.parent() != aOutColor) &&
	   (plug != aOutAlpha))
		return MS::kUnknownParameter;

	MFloatVector resultColor;
	MFloatVector& fore = block.inputValue( aForegroundColor ).asFloatVector();
	MFloatVector& back = block.inputValue( aBackgroundColor ).asFloatVector();
	MFloatVector& bclr = block.inputValue( aBackColor ).asFloatVector();
	float& alpha = block.inputValue( aMask ).asFloat();

	if ( alpha > 0.99999f ) alpha = 1.f;
	else if ( alpha < 0.00001 )  alpha = 0.f;
	
	resultColor = fore + ((bclr - back) * (1.0f - alpha));

	// normalize output color
	if (resultColor[0] < 0.f )	resultColor[0] = 0.f;
	if (resultColor[1] < 0.f )	resultColor[1] = 0.f;
	if (resultColor[2] < 0.f )	resultColor[2] = 0.f;
	
	if (resultColor[0] > 1.f )	resultColor[0] = 1.f;
	if (resultColor[1] > 1.f )	resultColor[1] = 1.f;
	if (resultColor[2] > 1.f )	resultColor[2] = 1.f;

	// set ouput color attribute
	MDataHandle outColorHandle = block.outputValue( aOutColor );
	MFloatVector& outColor = outColorHandle.asFloatVector();
	outColor = resultColor;
	outColorHandle.setClean();
	
	MDataHandle outAlphaHandle = block.outputValue( aOutAlpha );
	float& outAlpha = outAlphaHandle.asFloat();
	outAlpha = ( resultColor.x + resultColor.y + resultColor.z ) / 3.0f;
	outAlphaHandle.setClean();

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	const MString UserClassify( "utility/color" );

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");
	CHECK_MSTATUS( plugin.registerNode( "composite", myComp::id, 
						 &myComp::creator, &myComp::initialize,
						 MPxNode::kDependNode, &UserClassify ) );

	return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	CHECK_MSTATUS( plugin.deregisterNode( myComp::id ) );

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
