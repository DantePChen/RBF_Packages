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

// FILE: pointOnMeshCmd.cpp

// DESCRIPTION: - Defines the `pointOnMesh` MEL command.

// AUTHOR: QT

// REFERENCES: - This plug-in is based on the `pointOnSurface` MEL command and "pointOnSurfaceInfo" node.

//             - The pointOnSubdNode.cpp plug-in example from the Maya Developer Kit was also used for reference.

// LAST UPDATED: Oct. 11th, 2001.

// COMPILED AND TESTED ON: Maya 4.0 on Windows





// HEADER FILES:

#include "pointOnMeshCmd.h"

#include "getPointAndNormal.h"





// CONSTRUCTOR DEFINITION:

pointOnMeshCommand::pointOnMeshCommand()

{

}





// DESTRUCTOR DEFINITION:

pointOnMeshCommand::~pointOnMeshCommand()

{

}





// METHOD FOR CREATING AN INSTANCE OF THIS COMMAND:

void* pointOnMeshCommand::creator()

{

   return new pointOnMeshCommand;

}





// MAKE THIS COMMAND UNDOABLE:

bool pointOnMeshCommand::isUndoable() const

{

   return true;

}





// FIRST INVOKED WHEN COMMAND IS CALLED, PARSING THE COMMAND ARGUMENTS, INITIALIZING DEFAULT PARAMETERS, THEN CALLING redoIt():

MStatus pointOnMeshCommand::doIt(const MArgList& args)

{

   // INITIALIZE PRIVATE DATA FOR THE COMMAND:

   nodeCreated = positionSpecified = normalSpecified = faceIndexSpecified = relativeSpecified = parameterUSpecified = parameterVSpecified = false;

   meshNodeName = pointOnMeshInfoName = "";



   // PARSE THE COMMAND'S ARGUMENTS:

   for (unsigned i=0; i<args.length(); i++)

   {

      if ((MString("-name")==args.asString(i)) || (MString("-na")==args.asString(i)))

         pointOnMeshInfoName = args.asString(++i);

      else if ((MString("-position")==args.asString(i)) || (MString("-p")==args.asString(i)))

         positionSpecified = true;

      else if ((MString("-normal")==args.asString(i)) || (MString("-nr")==args.asString(i)))

         normalSpecified = true;

      else if ((MString("-faceIndex")==args.asString(i)) || (MString("-f")==args.asString(i)))

      {

	     faceIndexSpecified = true;

         int temp = args.asInt(++i);

		 if (temp<0)

         {

            displayError("Invalid faceIndex!");

            return MS::kFailure;

         }

		 faceIndex = temp;

      }

      else if ((MString("-relative")==args.asString(i)) || (MString("-r")==args.asString(i)))

      {

         relativeSpecified = true;

         relative = args.asBool(++i);

      }

      else if ((MString("-parameterU")==args.asString(i)) || (MString("-u")==args.asString(i)))

      {

         parameterUSpecified = true;

         double temp = args.asDouble(++i);

		 if ((temp<0) || (temp>1))

         {

            displayError("Invalid parameterU!");

            return MS::kFailure;

         }

         parameterU = temp;

      }

      else if ((MString("-parameterV")==args.asString(i)) || (MString("-v")==args.asString(i)))

      {

         parameterVSpecified = true;

         double temp = args.asDouble(++i);

		 if ((temp<0) || (temp>1))

         {

            displayError("Invalid parameterV!");

            return MS::kFailure;

         }

         parameterV = temp;

      }

      else if (i==(args.length()-1))

         meshNodeName = args.asString(i);

      else

      {

         MString errorMessage = "Invalid flag: ";

         errorMessage += args.asString(i);

         displayError(errorMessage);

         return MS::kFailure;

      }

   }



   // MAKE SURE UNSPECIFIED INPUT PARAMETER FLAGS GET DEFAULT VALUES:

   if (!faceIndexSpecified)

      faceIndex = 0;

   if (!relativeSpecified)

      relative = true;

   if (!parameterUSpecified)

      parameterU = 0.5;

   if (!parameterVSpecified)

      parameterV = 0.5;



   // DO THE WORK:

   return redoIt();

}





// DOES MOST OF THE WORK IN COMPUTING THE POSITION, NORMAL, OR CREATING A "pointOnMeshInfo" NODE:

MStatus pointOnMeshCommand::redoIt()

{

   // WHEN NO MESH IS SPECIFIED IN THE COMMAND, GET THE FIRST SELECTED MESH FROM THE SELECTION LIST:

   MSelectionList sList;

   if (meshNodeName=="")

   {

      MGlobal::getActiveSelectionList(sList);

      if (sList.length()==0)

      {

         displayError("No mesh or mesh transform specified!");

         return MS::kFailure; 

      }

   }

   // OTHERWISE, USE THE NODE NAME SPECIFIED IN THE LAST ARGUMENT OF THE COMMAND:

   else if (sList.add(meshNodeName) == MS::kInvalidParameter)

   {

      displayError("Specified mesh does not exist!");

      return MS::kFailure; 

   }



   // RETRIEVE THE FIRST ITEM FROM THE SELECTION LIST:

   MDagPath meshDagPath;

   sList.getDagPath(0, meshDagPath);



   // CREATE AND CONNECT A "pointOnMeshInfo" NODE, OR GET THE POINT AND NORMAL ACCORDING TO

   // WHETHER THE "-position/-p" AND/OR "-normal/-nr" FLAGS WERE SPECIFIED, AND WHETHER THE MESH

   // "SHAPE" OR ITS "TRANSFORM" WAS SPECIFIED/SELECTED:

   MPoint point;

   MVector normal;

   // WHEN THE SPECIFIED NODE IS THE MESH "SHAPE":

   if (meshDagPath.node().hasFn(MFn::kMesh))

   {

      // WHEN NEITHER "-position/-p" NOR "-normal/-nr" ARE SPECIFIED, CREATE AND CONNECT A "pointOnMeshInfo" NODE AND RETURN ITS NODE NAME:

      if (!positionSpecified && !normalSpecified)

      {

         // CREATE THE NODE:

         nodeCreated = true;

         MFnDependencyNode depNodeFn;

         if (pointOnMeshInfoName == "")

            depNodeFn.create("pointOnMeshInfo");

         else

            depNodeFn.create("pointOnMeshInfo", pointOnMeshInfoName);

         pointOnMeshInfoName = depNodeFn.name();



         // SET THE ".faceIndex" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

         if (faceIndexSpecified)

         {

            MPlug faceIndexPlug = depNodeFn.findPlug("faceIndex", true);

            faceIndexPlug.setValue(faceIndex);

         }



         // SET THE ".relative" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

         if (relativeSpecified)

         {

            MPlug relativePlug = depNodeFn.findPlug("relative", true);

            relativePlug.setValue(relative);

         }



         // SET THE ".parameterU" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

		 if (parameterUSpecified)

         {

            MPlug parameterUPlug = depNodeFn.findPlug("parameterU", true);

			parameterUPlug.setValue(parameterU);

         }



         // SET THE ".parameterV" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

		 if (parameterVSpecified)

         {

            MPlug parameterVPlug = depNodeFn.findPlug("parameterV", true);

			parameterVPlug.setValue(parameterV);

         }



         // CONNECT THE NODES:

         MPlug worldMeshPlug, inMeshPlug;

         inMeshPlug = depNodeFn.findPlug("inMesh", true);

         depNodeFn.setObject(meshDagPath.node());

         worldMeshPlug = depNodeFn.findPlug("worldMesh", true);

         worldMeshPlug = worldMeshPlug.elementByLogicalIndex(0);  // ASSUME THE *FIRST* INSTANCE OF THE MESH IS REQUESTED FOR MESH SHAPES.

         MDGModifier dgModifier;

         dgModifier.connect(worldMeshPlug, inMeshPlug);

         dgModifier.doIt();



         // SET COMMAND RESULT AND RETURN:

         setResult(pointOnMeshInfoName);

         return MStatus::kSuccess;

      }

      // OTHERWISE, COMPUTE THE POINT-POSITION AND NORMAL, USING THE *FIRST* INSTANCE'S TRANSFORM:

      else

         getPointAndNormal(meshDagPath, faceIndex, relative, parameterU, parameterV, point, normal);

   }

   // WHEN THE SPECIFIED NODE IS A "TRANSFORM" OF A MESH SHAPE:

   else if (meshDagPath.node().hasFn(MFn::kTransform) && meshDagPath.hasFn(MFn::kMesh))

   {

      // WHEN NEITHER "-position/-p" NOR "-normal/-nr" ARE SPECIFIED, CREATE AND CONNECT A "pointOnMeshInfo" NODE AND RETURN ITS NODE NAME:

      if (!positionSpecified && !normalSpecified)

      {

         // CREATE THE NODE:

         nodeCreated = true;

         meshDagPath.extendToShape();

         MFnDependencyNode depNodeFn;

         if (pointOnMeshInfoName == "")

            depNodeFn.create("pointOnMeshInfo");

         else

            depNodeFn.create("pointOnMeshInfo", pointOnMeshInfoName);

         pointOnMeshInfoName = depNodeFn.name();



         // SET THE ".faceIndex" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

         if (faceIndexSpecified)

         {

            MPlug faceIndexPlug = depNodeFn.findPlug("faceIndex", true);

            faceIndexPlug.setValue(faceIndex);

         }



         // SET THE ".relative" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

         if (relativeSpecified)

         {

            MPlug relativePlug = depNodeFn.findPlug("relative", true);

            relativePlug.setValue(relative);

         }



         // SET THE ".parameterU" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

		 if (parameterUSpecified)

         {

            MPlug parameterUPlug = depNodeFn.findPlug("parameterU", true);

            parameterUPlug.setValue(parameterU);

         }



         // SET THE ".parameterV" ATTRIBUTE, IF SPECIFIED IN THE COMMAND:

         if (parameterVSpecified)

         {

            MPlug parameterVPlug = depNodeFn.findPlug("parameterV", true);

			parameterVPlug.setValue(parameterV);

         }



         // CONNECT THE NODES:

         MPlug worldMeshPlug, inMeshPlug;

         inMeshPlug = depNodeFn.findPlug("inMesh", true);

         depNodeFn.setObject(meshDagPath.node());

         worldMeshPlug = depNodeFn.findPlug("worldMesh", true);

         worldMeshPlug = worldMeshPlug.elementByLogicalIndex(meshDagPath.instanceNumber());

         MDGModifier dgModifier;

         dgModifier.connect(worldMeshPlug, inMeshPlug);

         dgModifier.doIt();



         // SET COMMAND RESULT AND RETURN:

         setResult(pointOnMeshInfoName);

         return MStatus::kSuccess;         

      }

      // OTHERWISE, COMPUTE THE POINT-POSITION AND NORMAL:

      else

         getPointAndNormal(meshDagPath, faceIndex, relative, parameterU, parameterV, point, normal);

   }

   // INVALID INPUT WHEN SPECIFIED/SELECTED NODE IS NOT A MESH NOR TRANSFORM:

   else

   {

      displayError("Invalid type!  Only a mesh or its transform can be specified!");

      return MStatus::kFailure; 

   }



   // SET THE RETURN VALUES OF THE COMMAND'S RESULT TO BE AN ARRAY OF FLOATS OUTPUTTING THE POSITION AND/OR NORMAL:

   MDoubleArray result;

   if (positionSpecified)

   {

      result.append(point.x);

      result.append(point.y);

      result.append(point.z);

   }

   if (normalSpecified)

   {

      result.append(normal.x);

      result.append(normal.y);

      result.append(normal.z);

   }

   setResult(result);



   return MStatus::kSuccess;

}





// CALLED WHEN USER UNDOES THE COMMAND:

MStatus pointOnMeshCommand::undoIt()

{

   // MERELY DELETE THE "pointOnMeshInfo" NODE THAT WAS CREATED, IF ONE WAS CREATED:

   if (nodeCreated)

   {

      MString deleteCmd = "delete ";

      deleteCmd += pointOnMeshInfoName;

      MGlobal::executeCommand(deleteCmd);

   }



   return MStatus::kSuccess;

}

