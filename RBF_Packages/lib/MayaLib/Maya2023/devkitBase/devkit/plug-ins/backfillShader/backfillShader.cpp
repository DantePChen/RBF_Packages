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

#include <math.h>

#include <maya/MPxNode.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>

//////////////////////////////////////////////////////////////////
// 
// DESCRIPTION: 
// Produces dependency graph node BackFillShader

// This node is an example of a surface shader that fills non-diffuse illuminated areas with color.

// The inputs for this node are can be found in the Maya UI on the Attribute Editor for the node.

// The output attribute of the node is called "outColor". It is a 3 float value that represents the resulting color produced by the shader. 
// To use this shader, create a BackFillShader with Shading Group or connect its output to a Shading Group's "SurfaceShader" attribute. 
//
////////////////////////////////////////////////////////////////////

class BackNode : public MPxNode
{
	public:
                      BackNode();
              ~BackNode() override;

    MStatus   compute( const MPlug&, MDataBlock& ) override;
	void      postConstructor() override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

    static void *     creator();
    static MStatus    initialize();

	// Id tag for use with binary file format
    static MTypeId    id;

	private:
    static MObject  aTranslucenceCoeff;
    static MObject  aDiffuseReflectivity;
	static MObject  aColor;
	static MObject  aBackColor;
    static MObject  aBoost;
	static MObject  aIncandescence;

	static MObject  aOutColor;
	static MObject  aPointCamera;
	static MObject  aNormalCamera;
	static MObject  aLightDirection;
	static MObject  aLightIntensity;
    static MObject  aPower;
    static MObject  aSpecularity;
    static MObject  aThreshold;				// for backfill percentage

    static MObject  aLightAmbient;
    static MObject  aLightDiffuse;
    static MObject  aLightSpecular;
    static MObject  aLightShadowFraction;
    static MObject  aPreShadowIntensity;
    static MObject  aLightBlindData;
    static MObject  aLightData;
};

MTypeId BackNode::id( 0x81003 );

MObject  BackNode::aTranslucenceCoeff;
MObject  BackNode::aBackColor;
MObject  BackNode::aBoost;
MObject  BackNode::aDiffuseReflectivity;
MObject  BackNode::aColor;
MObject  BackNode::aIncandescence;
MObject  BackNode::aOutColor;
MObject  BackNode::aPointCamera;
MObject  BackNode::aNormalCamera;
MObject  BackNode::aLightData;
MObject  BackNode::aLightDirection;
MObject  BackNode::aLightIntensity; 
MObject  BackNode::aLightAmbient;
MObject  BackNode::aLightDiffuse;
MObject  BackNode::aLightSpecular;
MObject  BackNode::aThreshold;
MObject  BackNode::aLightShadowFraction;
MObject  BackNode::aPreShadowIntensity;
MObject  BackNode::aLightBlindData;
MObject  BackNode::aPower;
MObject  BackNode::aSpecularity;

#define MAKE_INPUT(attr)	\
    CHECK_MSTATUS( attr.setKeyable(true) ) ;	\
    CHECK_MSTATUS( attr.setStorable(true) );	\
    CHECK_MSTATUS( attr.setReadable(true) );	\
    CHECK_MSTATUS( attr.setWritable(true) );

#define MAKE_OUTPUT(attr)	\
    CHECK_MSTATUS( attr.setKeyable(false) );	\
    CHECK_MSTATUS( attr.setStorable(false) );	\
    CHECK_MSTATUS( attr.setReadable(true) );	\
    CHECK_MSTATUS( attr.setWritable(false) );

void BackNode::postConstructor( )
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
BackNode::BackNode()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
BackNode::~BackNode()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void * BackNode::creator()
{
    return new BackNode();
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus BackNode::initialize()
{
    MFnNumericAttribute nAttr; 
    MFnLightDataAttribute lAttr;

    aColor = nAttr.createColor( "color", "c" );
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(0.0f, 0.58824f, 0.644f) );

    aBackColor = nAttr.createColor( "backColor", "bc" );
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(1.0f, 1.0f, 1.0f) );

    aThreshold = nAttr.create( "threshold", "thr", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(0.2f) );
    CHECK_MSTATUS( nAttr.setMin(0.0f) );
    CHECK_MSTATUS( nAttr.setMax(1.0f) );

    aBoost = nAttr.create( "boost", "boo", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(2.0f) );
    CHECK_MSTATUS( nAttr.setMin(0.0f) );
    CHECK_MSTATUS( nAttr.setMax(5.0f) );

    aIncandescence = nAttr.createColor( "incandescence","ic" );
    MAKE_INPUT(nAttr);

    aTranslucenceCoeff = nAttr.create("translucenceCoeff", "tc", 
									  MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setMin(0.0f) );
    CHECK_MSTATUS( nAttr.setMax(1.0f) );

    aDiffuseReflectivity = nAttr.create("diffuseReflectivity", "drfl",
										MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    
    CHECK_MSTATUS( nAttr.setMin(0.0f) );
    CHECK_MSTATUS( nAttr.setMax(1.0f) );
    CHECK_MSTATUS( nAttr.setDefault(0.8f) );

    aOutColor = nAttr.createColor( "outColor", "oc" );
    MAKE_OUTPUT(nAttr);

    aPointCamera = nAttr.createPoint( "pointCamera", "pc" );
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setDefault(1.0f, 1.0f, 1.0f) );
    CHECK_MSTATUS( nAttr.setHidden(true) );

    aPower = nAttr.create( "power", "pow", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setMin(0.0f) );
    CHECK_MSTATUS( nAttr.setMax(200.0f) );
    CHECK_MSTATUS( nAttr.setDefault(10.0f) );

    aSpecularity = nAttr.create( "specularity", "spc", MFnNumericData::kFloat);
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setMin(0.0f) );
    CHECK_MSTATUS( nAttr.setMax(1.0f) );
    CHECK_MSTATUS( nAttr.setDefault(0.5f) );

    aNormalCamera = nAttr.createColor( "normalCamera", "n" );
    MAKE_INPUT(nAttr);
    CHECK_MSTATUS( nAttr.setHidden(true) );

    aLightDirection = nAttr.createPoint( "lightDirection", "ld" );
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aLightIntensity = nAttr.createColor( "lightIntensity", "li" );
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );
    CHECK_MSTATUS( nAttr.setDefault(1.0f, 1.0f, 1.0f) );

    aLightAmbient = nAttr.create( "lightAmbient", "la",
								  MFnNumericData::kBoolean);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aLightDiffuse = nAttr.create( "lightDiffuse", "ldf",
								  MFnNumericData::kBoolean);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aLightSpecular = nAttr.create( "lightSpecular", "ls", 
								   MFnNumericData::kBoolean);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aLightShadowFraction = nAttr.create("lightShadowFraction", "lsf",
										MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aPreShadowIntensity = nAttr.create("preShadowIntensity", "psi",
									   MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aLightBlindData = nAttr.createAddr("lightBlindData", "lbld");
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    aLightData = lAttr.create( "lightDataArray", "ltd", 
                               aLightDirection, aLightIntensity, aLightAmbient, 
                               aLightDiffuse, aLightSpecular, aLightShadowFraction,
                               aPreShadowIntensity, aLightBlindData);
    CHECK_MSTATUS( lAttr.setArray(true) );
    CHECK_MSTATUS( lAttr.setStorable(false) );
    CHECK_MSTATUS( lAttr.setHidden(true) );
    CHECK_MSTATUS( lAttr.setDefault(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, true, true, false, 0.0f, 1.0f, NULL) );

    CHECK_MSTATUS( addAttribute(aTranslucenceCoeff) );
    CHECK_MSTATUS( addAttribute(aDiffuseReflectivity) );
    CHECK_MSTATUS( addAttribute(aColor) );
    CHECK_MSTATUS( addAttribute(aIncandescence) );
    CHECK_MSTATUS( addAttribute(aPointCamera) );
    CHECK_MSTATUS( addAttribute(aNormalCamera) );
	
	// Only add the parent of the compound
    CHECK_MSTATUS( addAttribute(aLightData) );
    
    CHECK_MSTATUS( addAttribute(aPower) );
    CHECK_MSTATUS( addAttribute(aSpecularity) );
    CHECK_MSTATUS( addAttribute(aOutColor) );

    CHECK_MSTATUS( addAttribute(aBackColor) );
    CHECK_MSTATUS( addAttribute(aThreshold) );
    CHECK_MSTATUS( addAttribute(aBoost) );

    CHECK_MSTATUS( attributeAffects (aTranslucenceCoeff, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aDiffuseReflectivity, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightIntensity, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aIncandescence, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aPointCamera, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aNormalCamera, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightData, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightAmbient, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightSpecular, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightDiffuse, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightDirection, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightShadowFraction, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aPreShadowIntensity, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aLightBlindData, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aPower, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aSpecularity, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aColor, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aBackColor, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aThreshold, aOutColor) );
    CHECK_MSTATUS( attributeAffects (aBoost, aOutColor) );

    return MS::kSuccess;
}


//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus BackNode::compute(const MPlug& plug, MDataBlock& block) 
{
	// outColor or individual R, G, B channel, or alpha
    if ((plug != aOutColor) && (plug.parent() != aOutColor))
       return MS::kUnknownParameter;

    MFloatVector resultColor(0.0,0.0,0.0);

    // get sample surface shading parameters
    MFloatVector& surfaceNormal = block.inputValue( aNormalCamera ).asFloatVector();
    MFloatVector& cameraPosition = block.inputValue( aPointCamera ).asFloatVector();

    MFloatVector& surfaceColor  = block.inputValue( aColor ).asFloatVector();
    MFloatVector& incandescence = block.inputValue( aIncandescence ).asFloatVector();
    float diffuseReflectivity = block.inputValue( aDiffuseReflectivity ).asFloat();
    // float translucenceCoeff   = block.inputValue( aTranslucenceCoeff ).asFloat();
    float threshold   = block.inputValue( aThreshold ).asFloat();
    MFloatVector& BackLightColor  = block.inputValue( aBackColor ).asFloatVector();
    float Boost  = block.inputValue( aBoost ).asFloat();
  

    // Phong shading attributes
    float power = block.inputValue( aPower ).asFloat();
    float spec = block.inputValue( aSpecularity ).asFloat();

    float specularR, specularG, specularB;
    float diffuseR, diffuseG, diffuseB;
    float BackR, BackG, BackB;
    diffuseR = diffuseG = diffuseB = specularR = specularG = specularB = 0.0;
    BackR = BackG = BackB = 0.0;

    // get light list
    MArrayDataHandle lightData = block.inputArrayValue( aLightData );
    int numLights = lightData.elementCount();

    // iterate through light list and get ambient/diffuse values
    for( int count=1; count <= numLights; count++ )
    {
        MDataHandle currentLight = lightData.inputValue();
        MFloatVector& lightIntensity = currentLight.child(aLightIntensity).asFloatVector();
        // float lightShadowF = currentLight.child(aLightShadowFraction).asFloat();

        // find ambient component
        if( currentLight.child(aLightAmbient).asBool() ) {
            diffuseR += lightIntensity[0];
            diffuseG += lightIntensity[1];
            diffuseB += lightIntensity[2];
        }

        // find diffuse and specular component
        if( currentLight.child(aLightDiffuse).asBool() )
        {
            MFloatVector& lightDirection = currentLight.child(aLightDirection).asFloatVector();

            float cosln = lightDirection * surfaceNormal;
    
            if( cosln > 0.0f )  // calculate only if facing light
            {
                 diffuseR += lightIntensity[0] * ( cosln * diffuseReflectivity );
                 diffuseG += lightIntensity[1] * ( cosln * diffuseReflectivity );
                 diffuseB += lightIntensity[2] * ( cosln * diffuseReflectivity );
            }
            if (((cosln > 0.0) && (cosln <= threshold)) ||
                 (cosln <= 0.0))
            {
                float interp;
                if (threshold != 0.0) {
                    interp = ((1.0f / (-1 - threshold)) * cosln) +
						(threshold / (threshold + 1));
                }
                else
                {
                    interp = fabsf(cosln);
                }

                BackR += (BackLightColor[0] * interp);
                BackG += (BackLightColor[1] * interp);
                BackB += (BackLightColor[2] * interp);
            }

            cameraPosition.normalize();

            if( cosln > 0.0f ) // calculate only if facing light
            {
                float RV = ( ( (2*surfaceNormal) * cosln ) - lightDirection ) * cameraPosition;
                if( RV < 0.0 ) RV = -RV;

                if ( power < 0 ) power = -power;

                float s = spec * powf( RV, power );

                specularR += lightIntensity[0] * s; 
                specularG += lightIntensity[1] * s; 
                specularB += lightIntensity[2] * s; 
            }
       }
       if( !lightData.next() ) break;
    }

    // factor incident light with surface color and add incandescence
    resultColor[0] = ( diffuseR * surfaceColor[0] ) + specularR + incandescence[0] + (BackR * Boost);
    resultColor[1] = ( diffuseG * surfaceColor[1] ) + specularG + incandescence[1] + (BackG * Boost);
    resultColor[2] = ( diffuseB * surfaceColor[2] ) + specularB + incandescence[2] + (BackB * Boost);

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
   const MString UserClassify( "shader/surface" );

   MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");
   CHECK_MSTATUS( plugin.registerNode( "backFillShader", BackNode::id, 
                         BackNode::creator, BackNode::initialize,
                         MPxNode::kDependNode, &UserClassify ) );

   return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
   MFnPlugin plugin( obj );
   CHECK_MSTATUS( plugin.deregisterNode( BackNode::id ) );

   return MS::kSuccess;
}
