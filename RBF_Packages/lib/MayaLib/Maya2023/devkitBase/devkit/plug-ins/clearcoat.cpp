#include <maya/MStatus.h>
#include <maya/MIOStream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>

#include <math.h>

#define kPi					3.14159265358979323846264338327950
#define kFloatEpsilon		1.0e-5F


/////////////////////////////////
// Plugin clearcoat Shader Class //
/////////////////////////////////

class clearcoat : public MPxNode
{
public:
                      clearcoat();
             ~clearcoat() override;

    MStatus   compute( const MPlug&, MDataBlock& ) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

    static  void *    creator();
    static  MStatus   initialize();
    static  MTypeId   id;

protected:

   static MObject aIndex;
   static MObject aScale;
   static MObject aBias;

   static MObject  aNormalCameraX;
   static MObject  aNormalCameraY;
   static MObject  aNormalCameraZ;
   static MObject  aNormalCamera;

   static MObject  aRayDirectionX;
   static MObject  aRayDirectionY;
   static MObject  aRayDirectionZ;
   static MObject  aRayDirection;

   static MObject  aOutValue;
};

//
// DESCRIPTION: node classification
///////////////////////////////////////////////////////

MTypeId clearcoat::id( 0x58000400 );

//
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////

MObject  clearcoat::aIndex;
MObject  clearcoat::aScale;
MObject  clearcoat::aBias;

MObject  clearcoat::aNormalCamera;
MObject  clearcoat::aNormalCameraX;
MObject  clearcoat::aNormalCameraY;
MObject  clearcoat::aNormalCameraZ;

MObject  clearcoat::aRayDirection;
MObject  clearcoat::aRayDirectionX;
MObject  clearcoat::aRayDirectionY;
MObject  clearcoat::aRayDirectionZ;

MObject  clearcoat::aOutValue;


//
// DESCRIPTION:
///////////////////////////////////////////////////////
clearcoat::clearcoat()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
clearcoat::~clearcoat()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void* clearcoat::creator()
{
    return new clearcoat();
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus clearcoat::initialize()
{
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute   cAttr;

    aIndex = nAttr.create( "index", "ix", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(true) );
    CHECK_MSTATUS ( nAttr.setDefault(1.8f) );
    CHECK_MSTATUS ( nAttr.setSoftMin(1.0f) );
    CHECK_MSTATUS ( nAttr.setSoftMax(5.0f) );

    aScale = nAttr.create( "scale", "s", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(true) );
    CHECK_MSTATUS ( nAttr.setDefault(1.55f) );
    CHECK_MSTATUS ( nAttr.setSoftMin(0.0f) );
    CHECK_MSTATUS ( nAttr.setSoftMax(5.0f) );

    aBias = nAttr.create( "bias", "b", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(true) );
    CHECK_MSTATUS ( nAttr.setDefault(-0.1f) );
    CHECK_MSTATUS ( nAttr.setSoftMin(-1.0f) );
    CHECK_MSTATUS ( nAttr.setSoftMax( 1.0f) );



    aNormalCameraX = nAttr.create( "normalCameraX", "nx", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );

    aNormalCameraY = nAttr.create( "normalCameraY", "ny", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false));
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );

    aNormalCameraZ = nAttr.create( "normalCameraZ", "nz", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );


    aNormalCamera = nAttr.create( "normalCamera","n",
                                  aNormalCameraX, aNormalCameraY, aNormalCameraZ);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setDefault(1.0f, 1.0f, 1.0f));
    CHECK_MSTATUS ( nAttr.setHidden(true) );




    aRayDirectionX = nAttr.create( "rayDirectionX", "rx", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );

    aRayDirectionY = nAttr.create( "rayDirectionY", "ry", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false));
    CHECK_MSTATUS ( nAttr.setDefault(1.0f));

    aRayDirectionZ = nAttr.create( "rayDirectionZ", "rz", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );

    aRayDirection = nAttr.create( "rayDirection","r",
                                  aRayDirectionX, aRayDirectionY, aRayDirectionZ);
    CHECK_MSTATUS ( nAttr.setStorable(false) );
    CHECK_MSTATUS ( nAttr.setDefault(1.0f, 1.0f, 1.0f) );
    CHECK_MSTATUS ( nAttr.setHidden(true) );



// Outputs

    aOutValue = nAttr.create( "outValue", "ov", MFnNumericData::kFloat);
    CHECK_MSTATUS ( nAttr.setHidden(false) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );



    CHECK_MSTATUS ( addAttribute(aIndex));
    CHECK_MSTATUS ( addAttribute(aScale) );
    CHECK_MSTATUS ( addAttribute(aBias) );

	// compound attribute - only need to add parent
    CHECK_MSTATUS ( addAttribute(aNormalCamera) );

	// compound attribute - only need to add parent
    CHECK_MSTATUS ( addAttribute(aRayDirection) );

    CHECK_MSTATUS ( addAttribute(aOutValue) );


    CHECK_MSTATUS ( attributeAffects (aIndex, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aScale, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aBias, aOutValue) );

    CHECK_MSTATUS ( attributeAffects (aNormalCameraX, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aNormalCameraY, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aNormalCameraZ, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aNormalCamera, aOutValue) );

    CHECK_MSTATUS ( attributeAffects (aRayDirectionX, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aRayDirectionY, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aRayDirectionZ, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aRayDirection, aOutValue) );

    return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus clearcoat::compute(
const MPlug&      plug,
      MDataBlock& block )
{
  if( plug == aOutValue )
  {


    MFloatVector& surfaceNormal = block.inputValue( aNormalCamera ).asFloatVector();
    MFloatVector& rayDirection  = block.inputValue( aRayDirection ).asFloatVector();

	float index = block.inputValue(aIndex).asFloat();
	float scale = block.inputValue(aScale).asFloat();
	float bias  = block.inputValue(aBias).asFloat();

	// This is Jim Craighead's code.  Ripped straight from Studio...

	/* This basically computes a fresnel reflection coeficient.  */
	/* It could probably use some optimization, but the trig     */
	/* identities are left as an exercise for the masochistic.   */
	/* Also note that this is reasonably accurate for refractive */
	/* indices greater than 1, but a complete hack for smaller   */
	/* values.  There are still problems with values below 0.6   */
	/* such as Gold and Silver.                                  */

	float origCosne	 = - (rayDirection * surfaceNormal);
	float ninety        = (float) kPi * 0.5f;
	float I             = (float) acos( origCosne );
	float transSin      = (float) sin(I) / (float)(index);
	float ccFresnel     = 1.0;
	float ccBlend       = 0.0;
	float sum           = 0.0;
	float difference    = 0.0;
	if( transSin > 1.0 ) {
		float limit   = (float) asin( (float)(index));
		sum           = limit + ninety;
		difference    = limit - ninety;
	} else {
		float T       = (float) asin( transSin );
		sum           = I + T;
		difference    = I - T;
	}

	if( ! (fabs(difference) < kFloatEpsilon ) ) {
		float fudgedScale = (float)(scale) * 2.0f;
		float fudgedBias  = (float)(bias) * 1.0f;
		if( sum < ninety ) {
			float parallel      = (float) (tan(difference) / tan(sum));
			float perpendicular = (float) (sin(difference) / sin(sum));
			ccFresnel = 0.5f * ( perpendicular * perpendicular
								+ parallel * parallel );
		} else {
			float perpendicular = (float) sin(difference);
			ccFresnel = 0.5f * ( perpendicular * perpendicular );
		}
		ccBlend   = ccFresnel * fudgedScale + fudgedBias;
		if(      ccBlend > 1.0 ) ccBlend = 1.0;
		else if( ccBlend < 0.0 ) ccBlend = 0.0;
	}

    // set ouput color attribute
    MDataHandle outHandle = block.outputValue( aOutValue );
	float& outV = outHandle.asFloat();

	outV = ccBlend;
    outHandle.setClean();

  } else
	  return MS::kUnknownParameter;

  return MS::kSuccess;
}


//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
   const MString UserClassify( "utility/general:drawdb/shader/operation/clearCoat" );

   MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.0", "Any");

   CHECK_MSTATUS ( plugin.registerUIStrings( NULL, "clearcoatInitStrings") );

   CHECK_MSTATUS ( plugin.registerNode( "clearCoat", clearcoat::id,
                         clearcoat::creator, clearcoat::initialize,
                         MPxNode::kDependNode, &UserClassify ) );

   return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
   MFnPlugin plugin( obj );
   CHECK_MSTATUS ( plugin.deregisterNode( clearcoat::id ) );

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
