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

// FILE: pointOnMeshCmd.h


// HEADER FILES:

#include <maya/MGlobal.h>

#include <maya/MPxCommand.h>

#include <maya/MArgList.h>

#include <maya/MString.h>

#include <maya/MPlug.h>

#include <maya/MSelectionList.h>

#include <maya/MDoubleArray.h>





// MAIN CLASS DECLARATION FOR THE MEL COMMAND:

class pointOnMeshCommand : public MPxCommand

{

   public:

      pointOnMeshCommand();

      ~pointOnMeshCommand() override;

      static void* creator();

      bool isUndoable() const override;

      MStatus doIt(const MArgList&) override;

      MStatus redoIt() override;

      MStatus undoIt() override;



   private:

      bool nodeCreated, positionSpecified, normalSpecified, faceIndexSpecified, relativeSpecified, parameterUSpecified, parameterVSpecified;

      MString meshNodeName, pointOnMeshInfoName;

      int faceIndex;

	  bool relative;

      double parameterU, parameterV;

};

