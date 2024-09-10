#include <math.h>
#include <maya/MPxNode.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>

///////////////////////////////////////////////////////////////////
//	DESCRIPTION: 
//	Produces dependency graph node InterpNode 

//	This node is an example of evaluating the surface normal on an object.

//	The inputs for this node are two colors and an input value that modify the interpolation of color.

//	The output attribute for this node is "outColor". It is a 3 float value that represents the 
//	interpolated color as a result of the surface normal. To use this shader, create a InterpNode and connect its 
//	output to an input of a surface/shader node such as Color or Transparency. 
//
///////////////////////////////////////////////////////////////////



class InterpNode : public MPxNode
{
	public:
                  InterpNode();
	      ~InterpNode() override;

	MStatus compute( const MPlug&, MDataBlock& ) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

	static  void *  creator();
	static  MStatus initialize();

	static  MTypeId id;

	private:
	static MObject aInputValue;
	static MObject aColor1;
	static MObject aColor2;
	static MObject aNormalCamera;
	static MObject aPointCamera;
	static MObject aOutColor;
};

MTypeId InterpNode::id( 0x8100e );

MObject InterpNode::aInputValue;
MObject InterpNode::aColor1;
MObject InterpNode::aColor2;

MObject InterpNode::aNormalCamera;
MObject InterpNode::aPointCamera;

MObject InterpNode::aOutColor;

//
// DESCRIPTION:
///////////////////////////////////////////////////////
InterpNode::InterpNode()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
InterpNode::~InterpNode()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void* InterpNode::creator()
{
    return new InterpNode();
}



//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus InterpNode::initialize()
{
     MFnNumericAttribute nAttr; 

// Inputs and Attributes
//
// User defined attributes require a long-name and short-name that
// are required to be unique within the node.
// (See the compound attribute color1 named "Sides".)
//
// Rendering attributes that your node wants to get from the sampler
// require them to be defined given the pre-defined unique long-name.
// (See the compound attribute aNormalCamera named "normalCamera".)
// 
// User defined Attributes are generally something that you want
// to store in the Maya file. The setStorable(true) method enables 
// an attribute to be stored into the Maya scene file. 
//
// Rendering attributes are primarily data that is generated per sample and
// not something that you want to store in a file. To disable an 
// attribute from being recorded to the Maya scene file use 
// the setStorable(false) method.
//
// Simple attributes that represent a range of values can enable a
// slider on the Attribute Editor by using the methods setMin() and
// setMax(). 
// (See the simple attribute InputValue named "Power".)
//
// Compound attributes that represent a vector of 3 floats can enable
// a color swatch on the Attribute Editor that will launch a color picker 
// tool by using the method setUsedAsColor(true).
// (See the compound attribute color1 name "Sides".)
//
// Both Simple and Compound attributes can be initialized with a 
// default value using the method setDefault().
//
// Attributes by default show up in the Attribute Editor and
// in the Connection Editor unless they are specified as being hidden 
// by using the method setHidden(true). 
//
// Attributes by default have both read/write access in the dependency graph.
// To change an attributes behaviour you can use the methods
// setReadable() and setWritable(). The method setReadable(true) 
// indicates that the attribute can be used as the source in 
// a dependency graph connection. The method setWritable(true) 
// indicates that the attribute can be used as the destination 
// in a dependency graph connection.
// (See the compound attribute aOutColor named "outColor" below.
//  It has been marked as a read-only attribute since it is
//  the computed result of the node, it is not stored in the Maya file since 
//  it is always computed, and it is marked as hidden to prevent it 
//  from being displayed in the user interface.)
//  
//


// User defined input value

    aInputValue = nAttr.create( "power", "pow", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );
    CHECK_MSTATUS ( nAttr.setMin(0.0f) );
    CHECK_MSTATUS ( nAttr.setMax(3.0f) );
    CHECK_MSTATUS ( nAttr.setKeyable(true) );
    CHECK_MSTATUS ( nAttr.setStorable(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(true) );

// User defined color attribute
    aColor1 = nAttr.createColor( "sideColor", "sc");
    CHECK_MSTATUS ( nAttr.setDefault(1.0f, 1.0f, 1.0f) );
    CHECK_MSTATUS ( nAttr.setKeyable(true) );
    CHECK_MSTATUS ( nAttr.setStorable(true) );
    CHECK_MSTATUS ( nAttr.setUsedAsColor(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(true) );


    aColor2 = nAttr.createColor( "facingColor", "fc");
    CHECK_MSTATUS ( nAttr.setDefault(0.0f, 0.0f, 0.0f) );
    CHECK_MSTATUS ( nAttr.setKeyable(true) );
    CHECK_MSTATUS ( nAttr.setStorable(true) );
    CHECK_MSTATUS ( nAttr.setUsedAsColor(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(true) );


	// Surface Normal supplied by the render sampler

    aNormalCamera = nAttr.createPoint( "normalCamera", "n");
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(true) );

	// Point on surface in camera space, will be used to compute view vector

    aPointCamera = nAttr.createPoint( "pointCamera", "p");
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(true) );

// Outputs
//
// Always set your output attributes to be read-only
// You should also mark any internal attributes that your node
// computes to be read-only also, this will prevent any connections.
    aOutColor  = nAttr.createColor( "outColor", "oc" );
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(false) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    CHECK_MSTATUS ( addAttribute(aInputValue) );
    CHECK_MSTATUS ( addAttribute(aColor1) );
    CHECK_MSTATUS ( addAttribute(aColor2) );
    CHECK_MSTATUS ( addAttribute(aNormalCamera) );
    CHECK_MSTATUS ( addAttribute(aPointCamera) );
    CHECK_MSTATUS ( addAttribute(aOutColor) );

    CHECK_MSTATUS ( attributeAffects (aInputValue, aOutColor) );
    CHECK_MSTATUS ( attributeAffects (aColor1,  aOutColor) );
    CHECK_MSTATUS ( attributeAffects (aColor2,  aOutColor) );
    CHECK_MSTATUS ( attributeAffects (aNormalCamera,  aOutColor) );
    CHECK_MSTATUS ( attributeAffects (aPointCamera,  aOutColor) );

    return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus InterpNode::compute(
const MPlug&      plug,
      MDataBlock& block ) 
{ 
    if ((plug != aOutColor) && (plug.parent() != aOutColor))
		return MS::kUnknownParameter;

    MFloatVector resultColor;

    MFloatVector& Side = block.inputValue( aColor1 ).asFloatVector();
    MFloatVector& Face = block.inputValue( aColor2 ).asFloatVector();
    MFloatVector& surfNorm = block.inputValue( aNormalCamera ).asFloatVector();
    MFloatVector& viewVector = block.inputValue(aPointCamera).asFloatVector();
    float power = block.inputValue( aInputValue ).asFloat();

    // Normalize the view vector
    CHECK_MSTATUS ( viewVector.normalize() );
	
    // find dot product
    float scalarNormal = (viewVector.x * surfNorm.x) + 
		(viewVector.y * surfNorm.y) +
		(viewVector.z * surfNorm.z);

    // take the absolute value
    if (scalarNormal < 0.0) scalarNormal = -scalarNormal;

    // Use InputValue to change interpolation 
    //     power == 1.0 linear
    //     power >= 0.0 use gamma function
    //
	float scalar;
    if (power > 0.0) {
        scalar = powf(scalarNormal, 1.0f / power);
    }
    else { scalar = 0.0; }

    // Interpolate the colors
    resultColor = Side + ((Face - Side) * scalar);

    // set ouput color attribute
    MDataHandle outColorHandle = block.outputValue( aOutColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

    return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
   const MString UserClassify( "utility/general" );

   MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");
   CHECK_MSTATUS ( plugin.registerNode( "interpNode", InterpNode::id, 
						InterpNode::creator, InterpNode::initialize,
						MPxNode::kDependNode, &UserClassify ) );

   return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
   MFnPlugin plugin( obj );
   CHECK_MSTATUS ( plugin.deregisterNode( InterpNode::id ) );

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
