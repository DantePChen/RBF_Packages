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
//            	AUTODESK, SO PLEASE USE AND MODIFY AT YOUR OWN RISK.
//
// PLUGIN NAME: closestPointOnCurve v1.0
// FILE: closestPointOnCurveCmd.cpp
// DESCRIPTION: -Defines `closestPointOnCurve` MEL command.
//              -Please see readme.txt for full details.
// AUTHOR: QT
// REFERENCES: -This plugin's concept is based off of the "closestPointOnSurface" node.
//             -The MEL script AEclosestPointOnSurfaceTemplate.mel was referred to for
//              the AE template MEL script that accompanies the closestPointOnCurve node.
// LAST UPDATED: Oct. 13th, 2001.
// COMPILED AND TESTED ON: Maya 4.0 on Windows

// HEADER FILES:
#include "closestPointOnCurveCmd.h"
#include "closestTangentUAndDistance.h"
#include "closestPointOnCurveStrings.h"

// CONSTRUCTOR DEFINITION:
closestPointOnCurveCommand::closestPointOnCurveCommand()
{}

// DESTRUCTOR DEFINITION:
closestPointOnCurveCommand::~closestPointOnCurveCommand()
{}

// METHOD FOR CREATING AN INSTANCE OF THIS COMMAND:
void *closestPointOnCurveCommand::creator()
{
   return new closestPointOnCurveCommand;
}

// CREATES THE SYNTAX OBJECT FOR THE COMMAND:
MSyntax closestPointOnCurveCommand::newSyntax()
{
   MSyntax syntax;

   // INPUT FLAGS:
   syntax.addFlag("-na", "-name", MSyntax::kString);
   syntax.addFlag("-ip", "-inPosition", MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);

   // OUTPUT/QUERYABLE FLAGS:
   syntax.addFlag("-p", "-position");
   syntax.addFlag("-nr", "-normal");
   syntax.addFlag("-t", "-tangent");
   syntax.addFlag("-u", "-paramU");
   syntax.addFlag("-d", "-distance");

   // DEFINE BEHAVIOUR OF COMMAND ARGUMENT THAT SPECIFIES THE CURVE NODE:
   syntax.useSelectionAsDefault(true);
   syntax.setObjectType(MSyntax::kSelectionList, 1, 1);

   // MAKE COMMAND QUERYABLE AND NON-EDITABLE:
   syntax.enableQuery(true);
   syntax.enableEdit(false);
   return syntax;
}

// MAKE THIS COMMAND UNDOABLE:
bool closestPointOnCurveCommand::isUndoable() const
{
   return true;
}

// PARSE THE COMMAND'S FLAGS AND ARGUMENTS AND STORING THEM AS PRIVATE DATA, THEN DO THE WORK BY CALLING redoIt():
MStatus closestPointOnCurveCommand::doIt(const MArgList& args)
{
   // CREATE THE PARSER:
   MArgDatabase argData(syntax(), args);

   // STORE FLAG INDICATORS, STORING WHETHER EACH FLAG HAS BEEN SET OR NOT:
   queryFlagSet = argData.isQuery();
   inPositionFlagSet = argData.isFlagSet("inPosition");
   positionFlagSet = argData.isFlagSet("-position");
   normalFlagSet = argData.isFlagSet("-normal");
   tangentFlagSet = argData.isFlagSet("-tangent");
   paramUFlagSet = argData.isFlagSet("-paramU");
   distanceFlagSet = argData.isFlagSet("-distance");

   // STORE THE NAME OF THE "closestPointOnCurve" NODE IF SPECIFIED, OTHERWISE ASSIGN DEFAULT:
   if (argData.isFlagSet("-name"))
      argData.getFlagArgument("-name", 0, closestPointOnCurveNodeName);
   else
      closestPointOnCurveNodeName = "";

   // STORE THE "inPosition" IF SPECIFIED, OTHERWISE ASSIGN DEFAULT:
   if (inPositionFlagSet)
   {
      argData.getFlagArgument("-inPosition", 0, inPosition.x);
      argData.getFlagArgument("-inPosition", 1, inPosition.y);
      argData.getFlagArgument("-inPosition", 2, inPosition.z);
   } else {
      inPosition.x = 0.0;
      inPosition.y = 0.0;
      inPosition.z = 0.0;
   }

   // STORE THE SPECIFIED OBJECT, INPUTTED FROM EITHER THE COMMAND ARGUMENT OR CURRENT SELECTION:
   argData.getObjects(sList);
   return redoIt();
}

// COMPUTING THE OUTPUT VALUES FOR THE CLOSEST POSITION, NORMAL, TANGENT, PARAMETER-U AND DISTANCE, OR CREATING A "closestPointOnCurve" NODE:
MStatus closestPointOnCurveCommand::redoIt()
{
   // DOUBLE-CHECK TO MAKE SURE THERE'S A SPECIFIED OBJECT TO EVALUATE ON:
   if (sList.length() == 0)
   {
      MStatus stat;
      MString msg = MStringResource::getString(kNoValidObject, stat);
       displayError(msg);
       return MStatus::kFailure;
   }

   // RETRIEVE THE SPECIFIED OBJECT AS A DAGPATH:
   MDagPath curveDagPath;
   sList.getDagPath(0, curveDagPath);

   // CHECK FOR INVALID NODE-TYPE INPUT WHEN SPECIFIED/SELECTED NODE IS *NOT* A "CURVE" NOR "CURVE TRANSFORM":
   if (!curveDagPath.node().hasFn(MFn::kNurbsCurve) && !(curveDagPath.node().hasFn(MFn::kTransform) && curveDagPath.hasFn(MFn::kNurbsCurve)))
   {
      MStatus stat;
      MString msg; 
      
      // Use format to place variable string into message
      MString msgFmt = MStringResource::getString(kInvalidType, stat);
      MStringArray selectionStrings;
      sList.getSelectionStrings(0, selectionStrings);
      msg.format(msgFmt, selectionStrings[0]);
      displayError(msg);
      return MStatus::kFailure;
   }

   // WHEN COMMAND *NOT* IN "QUERY MODE" (I.E. "CREATION MODE"), CREATE AND CONNECT A "closestPointOnCurve" NODE AND RETURN ITS NODE NAME:
   if (!queryFlagSet)
   {
      // CREATE THE NODE:
      MFnDependencyNode depNodeFn;
      if (closestPointOnCurveNodeName == "")
         depNodeFn.create("closestPointOnCurve");
      else
         depNodeFn.create("closestPointOnCurve", closestPointOnCurveNodeName);

      closestPointOnCurveNodeName = depNodeFn.name();

      // SET THE ".inPosition" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:
      if (inPositionFlagSet)
      {
         MPlug inPositionXPlug = depNodeFn.findPlug("inPositionX", true);
         inPositionXPlug.setValue(inPosition.x);

         MPlug inPositionYPlug = depNodeFn.findPlug("inPositionY", true);
         inPositionYPlug.setValue(inPosition.y);

         MPlug inPositionZPlug = depNodeFn.findPlug("inPositionZ", true);
         inPositionZPlug.setValue(inPosition.z);
      }

      // MAKE SOME ADJUSTMENTS WHEN THE SPECIFIED NODE IS A "TRANSFORM" OF A CURVE SHAPE:
      unsigned instanceNumber=0;
      if (curveDagPath.node().hasFn(MFn::kTransform))
      {
         // EXTEND THE DAGPATH TO ITS CURVE "SHAPE" NODE:
         curveDagPath.extendToShape();

         // TRANSFORMS ARE *NOT* NECESSARILY THE "FIRST" INSTANCE TRANSFORM OF A CURVE SHAPE:
         instanceNumber = curveDagPath.instanceNumber();
      }

      // CONNECT THE NODES:
      MPlug worldCurvePlug, inCurvePlug;
      inCurvePlug = depNodeFn.findPlug("inCurve", true);
      depNodeFn.setObject(curveDagPath.node());
      worldCurvePlug = depNodeFn.findPlug("worldSpace", true);
      worldCurvePlug = worldCurvePlug.elementByLogicalIndex(instanceNumber);

      MDGModifier dgModifier;
      dgModifier.connect(worldCurvePlug, inCurvePlug);
      dgModifier.doIt();

      // SET COMMAND RESULT TO BE NEW NODE'S NAME, AND RETURN:
      setResult(closestPointOnCurveNodeName);
      return MStatus::kSuccess;
   }

   // OTHERWISE, WE'RE IN THE COMMAND'S "QUERY MODE":
   else
   {
      // COMPUTE THE CLOSEST POSITION, NORMAL, TANGENT, PARAMETER-U AND DISTANCE, USING THE *FIRST* INSTANCE TRANSFORM WHEN CURVE IS SPECIFIED AS A "SHAPE":
      MPoint position;
      MVector normal, tangent;
      double paramU, distance;
      closestTangentUAndDistance(curveDagPath, inPosition, position, normal, tangent, paramU, distance);

      // WHEN NO QUERYABLE FLAG IS SPECIFIED, INDICATE AN ERROR:
      if (!positionFlagSet && !normalFlagSet && !tangentFlagSet && !paramUFlagSet && !distanceFlagSet)
      {
         MStatus stat;
         MString msg = MStringResource::getString(kNoQueryFlag, stat);
         displayError(msg);
         return MStatus::kFailure;
      }

      // WHEN JUST THE "DISTANCE" IS QUERIED, RETURN A SINGLE "FLOAT" INSTEAD OF AN ENTIRE FLOAT ARRAY FROM THE COMMAND:
      else if (distanceFlagSet && !(positionFlagSet || normalFlagSet || tangentFlagSet || paramUFlagSet))
         setResult(distance);

      // WHEN JUST THE "PARAMETER-U" IS QUERIED, RETURN A SINGLE "FLOAT" INSTEAD OF AN ENTIRE FLOAT ARRAY FROM THE COMMAND:
      else if (paramUFlagSet && !(positionFlagSet || normalFlagSet || tangentFlagSet || distanceFlagSet))
         setResult(paramU);

      // OTHERWISE, SET THE RETURN VALUE OF THE COMMAND'S RESULT TO A "COMPOSITE ARRAY OF FLOATS":
      else
      {
         // HOLDS FLOAT ARRAY RESULT:
         MDoubleArray floatArrayResult;

         // APPEND THE RESULTS OF THE CLOSEST POSITION, NORMAL, TANGENT, PARAMETER-U AND DISTANCE VALUES TO THE FLOAT ARRAY RESULT:
         if (positionFlagSet)
         {
            floatArrayResult.append(position.x);
            floatArrayResult.append(position.y);
            floatArrayResult.append(position.z);
         }

         if (normalFlagSet)
         {
            floatArrayResult.append(normal.x);
            floatArrayResult.append(normal.y);
            floatArrayResult.append(normal.z);
         }

         if (tangentFlagSet)
         {
            floatArrayResult.append(tangent.x);
            floatArrayResult.append(tangent.y);
            floatArrayResult.append(tangent.z);
         }

         if (paramUFlagSet)
            floatArrayResult.append(paramU);

         if (distanceFlagSet)
            floatArrayResult.append(distance);

         // FINALLY, SET THE COMMAND'S RESULT:
         setResult(floatArrayResult);
      }

      return MStatus::kSuccess;
   }
}

// CALLED WHEN USER UNDOES THE COMMAND:
MStatus closestPointOnCurveCommand::undoIt()
{
   // MERELY DELETE THE "closestPointOnCurve" NODE THAT WAS CREATED, IF ONE WAS CREATED:
   if (!queryFlagSet)
   {
      MString deleteCmd = "delete ";
      deleteCmd += closestPointOnCurveNodeName;
      MGlobal::executeCommand(deleteCmd);
   }

   return MStatus::kSuccess;
}
