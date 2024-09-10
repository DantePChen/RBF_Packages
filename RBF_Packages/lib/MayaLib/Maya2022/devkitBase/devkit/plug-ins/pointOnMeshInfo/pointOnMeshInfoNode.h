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

// DISCLAIMER: THIS PLUGIN IS PROVIDED AS IS.  IT IS NOT SUPPORTED BY

//            AUTODESK, SO PLEASE USE AND MODIFY AT YOUR OWN RISK.


// PLUGIN NAME: pointOnMeshInfo v1.0

// FILE: pointOnMeshInfoNode.h


// MAYA HEADER FILES:

#include <maya/MPxNode.h>

#include <maya/MTypeId.h>

#include <maya/MDataBlock.h>

#include <maya/MDataHandle.h>

#include <maya/MFnTypedAttribute.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MDagPath.h>

#include <maya/MPlug.h>

#include <maya/MPoint.h>

#include <maya/MVector.h>





// MAIN CLASS DECLARATION FOR THE CUSTOM NODE:

class pointOnMeshInfoNode : public MPxNode

{

   public:

      // CLASS METHOD DECLARATIONS:

      pointOnMeshInfoNode();

      ~pointOnMeshInfoNode() override;

      static void *creator();

      static MStatus initialize();

      MStatus compute(const MPlug &plug, MDataBlock &data) override;



      // CLASS DATA DECLARATIONS:

      static MTypeId id;

      static MObject aInMesh;

      static MObject aFaceIndex;

	  static MObject aRelative;

      static MObject aParameterU;

      static MObject aParameterV;

      static MObject aPosition;

      static MObject aPositionX;

      static MObject aPositionY;

      static MObject aPositionZ;

      static MObject aNormal;

      static MObject aNormalX;

      static MObject aNormalY;

      static MObject aNormalZ;

};

