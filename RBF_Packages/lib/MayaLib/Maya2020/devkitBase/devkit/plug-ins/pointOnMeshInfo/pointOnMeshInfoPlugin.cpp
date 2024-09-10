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
// AUTODESK, SO PLEASE USE AND MODIFY AT YOUR OWN RISK.

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// This plug-in defines both a MEL command and a DG node which computes
// the worldspace position and normal on a poly mesh given a face index,
// a U-parameter, and a V-parameter as input. 
//
// REFERENCES:
//
//	- This plug-in is based on the "pointOnSurface" MEL command and "pointOnSurfaceInfo" node.
//	- The pointOnSubdNode.cpp plug-in example from the Maya Developer Kit was also used for reference.
//
// LAST UPDATED: Oct. 11th, 2001.
//
// COMPILED AND TESTED ON: Maya 4.0 on Windows
//
////////////////////////////////////////////////////////////////////////

// HEADER FILES:

#include "pointOnMeshCmd.h"

#include "pointOnMeshInfoNode.h"

#include <maya/MFnPlugin.h>





// INITIALIZES THE PLUGIN BY REGISTERING THE COMMAND AND NODE:

MStatus initializePlugin(MObject obj)

{

   MStatus status;



   MFnPlugin plugin(obj, PLUGIN_COMPANY, "4.0", "Any");



   status = plugin.registerCommand("pointOnMesh", pointOnMeshCommand::creator);

   if (!status)

   {

      status.perror("registerCommand");

      return status;

   }



   status = plugin.registerNode("pointOnMeshInfo", pointOnMeshInfoNode::id, pointOnMeshInfoNode::creator, pointOnMeshInfoNode::initialize);

   if (!status)

   {

      status.perror("registerNode");

      return status;

   }



   return status;

}





// UNINITIALIZES THE PLUGIN BY DEREGISTERING THE COMMAND AND NODE:

MStatus uninitializePlugin(MObject obj)

{

   MStatus status;



   MFnPlugin plugin(obj);



   status = plugin.deregisterCommand("pointOnMesh");

   if (!status)

   {

      status.perror("deregisterCommand");

      return status;

   }



   status = plugin.deregisterNode(pointOnMeshInfoNode::id);

   if (!status)

   {

      status.perror("deregisterNode");

      return status;

   }



   return status;

}

