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

//

// DISCLAIMER: THIS PLUGIN IS PROVIDED AS IS.  IT IS NOT SUPPORTED BY

//            AUTODESK, SO PLEASE USE AND MODIFY AT YOUR OWN RISK.

//

// PLUGIN NAME: Convert Edges To Contained Faces Command v1.0

// FILE: convertEdgesToFacesCmd.cpp

// DESCRIPTION: o MEL command that converts a selection of edges into a selection of faces that interconnect

//                the original edges (i.e. only faces whose composite edges are contained in the original edge

//                selection).  The command's return value is a string array that contains the names of all of

//                the new contained faces.

//              o This MEL command has no flags, returns a string array, and operates on selected edges.

//              o Example MEL usage:

//                   select -r pCube1.e[1:2] pCube1.e[6:7];

//                   string $convertedFaces[] = `convertEdgesToFaces`;

//                   // Result: pCube1.f[1] //

// AUTHOR: QT

// LAST UPDATED: Oct. 11th, 2001.

// TESTED ON: Maya 4.0 on Windows

// STEPS TO INSTALL: 1) Unzip the file convertEdgesToFaces_v1_0_Maya4_Windows.zip to any temporary directory.

//                   2) Copy the file convertEdgesToFacesCmd.mll to: /AW/Maya4.0/bin/plug-ins/

//                   3) Launch Maya, and load the plugin from Windows->Setting/Preferences->Plugin Manager.





// BEGIN PLUGIN:

// =============





// HEADER FILES:

#include <maya/MFnPlugin.h>

#include <maya/MPxCommand.h>

#include <maya/MGlobal.h>

#include <maya/MSelectionList.h>

#include <maya/MItSelectionList.h>

#include <maya/MObject.h>

#include <maya/MDagPath.h>

#include <maya/MItMeshEdge.h>

#include <maya/MItMeshPolygon.h>





// MAIN CLASS FOR THE MEL COMMAND:

class convertEdgesToContainedFacesCommand : public MPxCommand

{

   public:

      convertEdgesToContainedFacesCommand();

      ~convertEdgesToContainedFacesCommand() override;

      static void* creator();

      bool isUndoable() const override;



      MStatus doIt(const MArgList&) override;

      MStatus redoIt() override;

      MStatus undoIt() override;



   private:

      MSelectionList previousSelectionList;

};





// CONSTRUCTOR:

convertEdgesToContainedFacesCommand::convertEdgesToContainedFacesCommand()

{

}





// DESTRUCTOR CLEARS, AND FREES MEMORY OF THE PREVIOUS SELECTION LIST:

convertEdgesToContainedFacesCommand::~convertEdgesToContainedFacesCommand()

{

   previousSelectionList.clear();

}





// FOR CREATING AN INSTANCE OF THIS COMMAND:

void* convertEdgesToContainedFacesCommand::creator()

{

   return new convertEdgesToContainedFacesCommand;

}





// MAKE THIS COMMAND UNDOABLE:

bool convertEdgesToContainedFacesCommand::isUndoable() const

{

   return true;

}





// SAVE THE PREVIOUS SELECTION, AND THEN CALL redoIt(), WHICH DOES MOST OF THE WORK:

MStatus convertEdgesToContainedFacesCommand::doIt(const MArgList& args)

{

   MGlobal::getActiveSelectionList(previousSelectionList);

   return redoIt();

}





// CONVERTS THE SELECTED EDGES TO "CONTAINED FACES", AND RETURN FROM THE MEL COMMAND, A STRING ARRAY OF THE NEW FACES:

MStatus convertEdgesToContainedFacesCommand::redoIt()

{

   MSelectionList finalFacesSelection;

   MDagPath meshDagPath;

   MObject multiEdgeComponent, singleEdgeComponent;

   int dummyIndex;



   // ITERATE THROUGH EACH "EDGE COMPONENT" THAT IS CURRENTLY SELECTED:

   for (MItSelectionList edgeComponentIter(previousSelectionList, MFn::kMeshEdgeComponent); !edgeComponentIter.isDone(); edgeComponentIter.next())

   {

      // STORE THE DAGPATH, COMPONENT OBJECT AND MESH NAME OF THE CURRENT EDGE COMPONENT:

      edgeComponentIter.getDagPath(meshDagPath, multiEdgeComponent);

      MString meshName = meshDagPath.fullPathName();



      // EDGE COMPONENT HAS TO CONTAIN AT LEAST ONE EDGE:

      if (!multiEdgeComponent.isNull())

      {

         // ITERATE THROUGH EACH "EDGE" IN THE CURRENT EDGE COMPONENT:

         for (MItMeshEdge edgeIter(meshDagPath, multiEdgeComponent); !edgeIter.isDone(); edgeIter.next())

         {

            // FOR STORING THE FACES CONNECTED TO EACH EDGE:

            MIntArray connectedFacesIndices;

            edgeIter.getConnectedFaces(connectedFacesIndices);



            // ITERATE THROUGH EACH FACE CONNECTED TO THE CURRENT EDGE:

            MItMeshPolygon faceIter(meshDagPath);

            for (unsigned i=0; i<connectedFacesIndices.length(); i++)

            {

               // GET THE EDGE INDICES FOR CURRENT FACE:

               MIntArray faceEdgesIndices;

               faceIter.setIndex(connectedFacesIndices[i], dummyIndex);

               faceIter.getEdges(faceEdgesIndices);



               // ITERATE THROUGH EACH FACE EDGE TO SEE IF EACH ONE BELONGS TO THE ORIGINAL EDGE SELECTION:

               bool faceIsContained=1;

               for (unsigned j=0; j<faceEdgesIndices.length(); j++)

               {

                  // CREATE A SELECTION LIST FOR EACH EDGE TO USE IN THE ::hasItem() METHOD:

                  MSelectionList singleEdgeList;

                  singleEdgeList.clear();

                  MString edgeName = meshName;

                  edgeName += ".e[";

                  edgeName += faceEdgesIndices[j];

                  edgeName += "]";

                  singleEdgeList.add(edgeName);

                  singleEdgeList.getDagPath(0, meshDagPath, singleEdgeComponent);



                  // SEE WHETHER EDGE BELONGS TO ORIGINAL SELECTION, AND IF IT DOESN'T, THEN THE WHOLE FACE IS NOT CONTAINED:

                  if (!previousSelectionList.hasItem(meshDagPath, singleEdgeComponent))

                  {

                     faceIsContained = 0;

                     break;

                  }

               }



               // IF FACE IS "CONTAINED", ADD IT TO THE FINAL CONTAINED FACES LIST:

               if (faceIsContained)

               {

                  MString faceName = meshName;

                  faceName += ".f[";

                  faceName += connectedFacesIndices[i];

                  faceName += "]";

                  finalFacesSelection.add(faceName);

               }

            }

         }

      }

   }



   // FINALLY, MAKE THE NEW "CONTAINED FACES", THE CURRENT SELECTION:

   MGlobal::setActiveSelectionList(finalFacesSelection, MGlobal::kReplaceList);



   // RETURN NEW CONTAINED FACES LIST FROM THE MEL COMMAND, AS AN ARRAY OF STRINGS:

   MStringArray containedFacesArray;

   finalFacesSelection.getSelectionStrings(containedFacesArray);

   MPxCommand::setResult(containedFacesArray);



   return MS::kSuccess;

}





// TO UNDO THE COMMAND, SIMPLY RESTORE THE ORIGINAL SELECTION BEFORE THE COMMAND WAS INVOKED:

MStatus convertEdgesToContainedFacesCommand::undoIt()

{

   MGlobal::setActiveSelectionList(previousSelectionList, MGlobal::kReplaceList);

   return MS::kSuccess;

}





// INITIALIZES THE PLUGIN:

MStatus initializePlugin(MObject obj)

{

   MStatus status;

   MFnPlugin plugin(obj, PLUGIN_COMPANY, "4.0", "Any");



   status = plugin.registerCommand("convertEdgesToFaces", convertEdgesToContainedFacesCommand::creator);

   if (!status)

   {

      status.perror("registerCommand");

      return status;

   }



   return status;

}





// UNINITIALIZES THE PLUGIN:

MStatus uninitializePlugin(MObject obj)

{

   MStatus status;

   MFnPlugin plugin(obj);



   status = plugin.deregisterCommand("convertEdgesToFaces");

   if (!status)

   {

      status.perror("deregisterCommand");

      return status;

   }



   return status;

}





// ============================================================================================================== END PLUGIN.

