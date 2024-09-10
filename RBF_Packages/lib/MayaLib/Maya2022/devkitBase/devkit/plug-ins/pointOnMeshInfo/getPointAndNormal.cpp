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

//

// PLUGIN NAME: pointOnMeshInfo v1.0

// FILE: getPointAndNormal.cpp

// DESCRIPTION: - Utility function definition that is used by both the command and node.

// AUTHOR: QT

// REFERENCES: - This plug-in is based on the `pointOnSurface` MEL command and "pointOnSurfaceInfo" node.

//             - The pointOnSubdNode.cpp plug-in example from the Maya Developer Kit was also used for reference.

// LAST UPDATED: Oct. 11th, 2001.

// COMPILED AND TESTED ON: Maya 4.0 on Windows


// HEADER FILE:

#include "getPointAndNormal.h"


// FUNCTION THAT FINDS THE POINT AND NORMAL OF A POLY AT A SPECIFIED FACE UV COORD ABOUT A SPECIFIED FACE:

void getPointAndNormal(MDagPath meshDagPath, int faceIndex, bool relative, double parameterU, double parameterV, MPoint &point, MVector &normal, MObject theMesh)

{

   // CREATE FACE ITERATOR, AND SET ITS INDEX TO THAT OF THE SPECIFIED FACE:

   MItMeshPolygon faceIter(meshDagPath);

   if (theMesh!=MObject::kNullObj)

      faceIter.reset(theMesh);

   int dummyIndex;

   faceIter.setIndex(faceIndex, dummyIndex);



   // WHEN "RELATIVE" MODE IS SPECIFIED, CALCULATE THE *ABSOLUTE* UV'S FROM THE SPECIFIED FACE AND "RELATIVE" UV'S:

   float u, v;

   if (relative)

   {

      MFloatArray uArray, vArray;

      faceIter.getUVs(uArray, vArray);

      float minU=999999, minV=999999, maxU=0, maxV=0;

      for (unsigned i=0; i<uArray.length(); i++)

      {

         minU = (uArray[i] < minU) ? uArray[i] : minU;

         minV = (vArray[i] < minV) ? vArray[i] : minV;

         maxU = (uArray[i] > maxU) ? uArray[i] : maxU;

         maxV = (vArray[i] > maxV) ? vArray[i] : maxV;

      }

      u = minU + (float)parameterU * (maxU - minU);

      v = minV + (float)parameterV * (maxV - minV);

   }

   // OTHERWISE, JUST TAKE THE ABSOLUTE UV'S TO BE THE ONES SPECIFIED:

   else

   {

      u = (float)parameterU;

      v = (float)parameterV;

   }



   // FIND THE WORLDSPACE COORDINATE AT THE SPECIFIED UV:

   float2 UV;

   UV[0] = u;

   UV[1] = v;

   faceIter.getPointAtUV(point, UV, MSpace::kWorld);



   // FIND THE NORMAL AT THE SPECIFIED UV:

   MFnMesh meshFn(meshDagPath);

   if (theMesh!=MObject::kNullObj)

      meshFn.setObject(theMesh);

   meshFn.getClosestNormal(point, normal, MSpace::kWorld);

}

