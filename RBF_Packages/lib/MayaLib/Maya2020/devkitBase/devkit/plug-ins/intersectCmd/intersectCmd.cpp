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
///////////////////////////////////////////////////////////////////////////////////////////////////
// DESCRIPTION://
// Produces a proxy command "intersectCmd".//
// This is an example of finding the intersection points on a mesh from a spotlight using the// intersection acceleration methods in MFnMesh.//
// To use the plug-in:
//	
//	(1) Create a spotlight and a poly plane (mesh).//
//	(2) Orient the light to shine on the plane.//
//	(3) Run the command as such: intersectCmd <spotLightName> <planeName>.
//		If there is an intersection, a small sphere will be created at the intersection point.//
//	Use the following script to automatically display intersections on the mesh (assuming //	a spotlight and a mesh with the names used here exist)://
//	global proc intersectExample()//
//	{//
//		intersectCmd spotLight1 pPlane1;
//		select -r spotLight1;//
//	}//
//	scriptJob -ac "spotLight1.tx" intersectExample;
//	scriptJob -ac "spotLight1.ty" intersectExample;
//	scriptJob -ac "spotLight1.tz" intersectExample;
//	scriptJob -ac "spotLight1.rx" intersectExample;
//	scriptJob -ac "spotLight1.ry" intersectExample;
//	scriptJob -ac "spotLight1.rz" intersectExample;//
//////////////////////////////////////////////////////////////////////////////////////////////////

// MAYA HEADERS
#include <maya/MFnPlugin.h>
#include <maya/MDagPath.h>
#include <maya/MFnMesh.h>
#include <maya/MItSelectionList.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MObject.h>
#include <maya/MSelectionList.h>
#include <maya/MArgList.h>
#include <maya/MFloatPoint.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnSpotLight.h>
#include <maya/MFloatPointArray.h>
#include <maya/MPxCommand.h>
#include <maya/MDGModifier.h>
#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MGlobal.h>

#ifndef INTERSECTCMD
#define INTERSECTCMD

#define MERR_CHK(stat,msg) if ( !stat ) { MGlobal::displayError(msg); } // cerr << msg << endl; }
// MAIN CLASS FOR THE INTERSECTCMD COMMAND:
class intersectCmd : public MPxCommand
{
	public:
		intersectCmd();
		~intersectCmd() override;
		static void* creator();
		bool isUndoable() const override;
		
		MStatus doIt(const MArgList&) override;
		MStatus undoIt() override;
};
#endif

// CONSTRUCTOR:
intersectCmd::intersectCmd()
{
}

// DESTRUCTOR:
intersectCmd::~intersectCmd()
{
}

// FOR CREATING AN INSTANCE OF THIS COMMAND:
void* intersectCmd::creator()
{
   return new intersectCmd;
}

// MAKE THIS COMMAND NOT UNDOABLE:
bool intersectCmd::isUndoable() const
{
   return false;
}


MStatus intersectCmd::doIt(const MArgList& args)

// Description:
// 		Determine if the ray from the spotlight intersects the mesh.
//		If it does, display the intersection points.

{
	MStatus stat = MStatus::kSuccess;

	if (args.length() != 2) 
	{
		MGlobal::displayError("Need 2 items!");
		return MStatus::kFailure;
	}

	MSelectionList activeList;
	int i;
	
	for ( i = 0; i < 2; i++)
	{
		MString strCurrSelection;
		stat = args.get(i, strCurrSelection);
		if (MStatus::kSuccess == stat) activeList.add(strCurrSelection);
	}

	MItSelectionList iter(activeList);
	MFnSpotLight fnLight;  
	MFnMesh fnMesh;
	MFnDagNode dagNod;
	MFnDependencyNode fnDN;

	float fX = 0;
	float fY = 0;
	float fZ = 0;

	for ( ; !iter.isDone(); iter.next() )
	{
		MObject tempObjectParent, tempObjectChild;
		iter.getDependNode(tempObjectParent);

		if (tempObjectParent.apiType() == MFn::kTransform)
		{
			dagNod.setObject(tempObjectParent);
			tempObjectChild = dagNod.child(0, &stat);
		}

		// check what type of object is selected
		if (tempObjectChild.apiType() == MFn::kSpotLight)
		{
			MDagPath pathToLight;
			MERR_CHK(MDagPath::getAPathTo(tempObjectParent, pathToLight), "Couldn't get a path to the spotlight");
			MERR_CHK(fnLight.setObject(pathToLight), "Failure on assigning light");
			
			stat = fnDN.setObject(tempObjectParent);

			MPlug pTempPlug = fnDN.findPlug("translateX", true, &stat);
			if (MStatus::kSuccess == stat)
			{
				pTempPlug.getValue(fX);
			}

			pTempPlug = fnDN.findPlug("translateY", true, &stat);
			if (MStatus::kSuccess == stat)
			{
				pTempPlug.getValue(fY);
			}

			pTempPlug = fnDN.findPlug("translateZ", true, &stat);
			if (MStatus::kSuccess == stat)
			{
				pTempPlug.getValue(fZ);
			}	
		}
		else if (tempObjectChild.apiType() == MFn::kMesh)
		{
			MDagPath pathToMesh;
			MERR_CHK(MDagPath::getAPathTo(tempObjectChild, pathToMesh), "Couldn't get a path to the spotlight");
			MERR_CHK(fnMesh.setObject(pathToMesh), "Failure on assigning light");	
		}
		else
		{
			MGlobal::displayError("Need a spotlight and a mesh");
			return MStatus::kFailure;
		}
	}

	MFloatPoint fpSource(fX, fY, fZ);
	MFloatVector fvRayDir = fnLight.lightDirection(0, MSpace::kWorld, &stat);
	MFloatPoint hitPoint;
	
	MMeshIsectAccelParams mmAccelParams = fnMesh.autoUniformGridParams();
	
	float fHitRayParam, fHitBary1, fHitBary2;
	int nHitFace, nHitTriangle;

	// a large positive number is used here for the maxParam parameter
	bool bAnyIntersection = fnMesh.anyIntersection(fpSource, fvRayDir, NULL, NULL, false,				MSpace::kWorld, (float)9999, false, &mmAccelParams, hitPoint, &fHitRayParam, &nHitFace, &nHitTriangle, 		&fHitBary1, &fHitBary2, (float)1e-6, &stat);
	
	if (! bAnyIntersection) 
	{
		MGlobal::displayInfo("There were no intersection points detected");
		return stat;
	}

	MFloatPointArray hitPoints;
	MFloatArray faHitRayParams;
	MIntArray iaHitFaces;
	MIntArray iaHitTriangles;
	MFloatArray faHitBary1;
	MFloatArray faHitBary2;

	bool bAllIntersections = fnMesh.allIntersections(fpSource, fvRayDir, NULL, NULL, false, MSpace::kWorld, 9999, false, NULL, false, hitPoints, &faHitRayParams, &iaHitFaces, &iaHitTriangles, &faHitBary1, &faHitBary2, 0.000001f, &stat);
	
	if (! bAllIntersections)
	{
		MGlobal::displayInfo("Error getting all intersections");
		return stat;
	}
	
	// check how many intersections are found
	unsigned int nNumberHitPoints = hitPoints.length();

	if (! nNumberHitPoints)
	{
		MGlobal::displayInfo("No hit points detected");
		return MStatus::kSuccess;
	}

	// Intersection exists; display intersections as spheres
	MString strCommandString = "string $strBall[] = `polySphere -r 0.5`;";
	strCommandString += "$strBallName = $strBall[0];";

	float x = 0;
	float y = 0;
	float z = 0;

	for (i = 0; i < (int)nNumberHitPoints; i++)
	{
		// get the points
		x = hitPoints[i][0];
		y = hitPoints[i][1];
		z = hitPoints[i][2];

		// execute some MEL to create a small sphere
		strCommandString += "setAttr ($strBallName + \".tx\") ";
		strCommandString += x;
		strCommandString += ";";

		strCommandString += "setAttr ($strBallName + \".ty\") ";
		strCommandString += y;
		strCommandString += ";";

		strCommandString += "setAttr ($strBallName + \".tz\") ";
		strCommandString += z;
		strCommandString += ";";

		MGlobal::executeCommand(strCommandString);
	}

	return stat;
}

// UNDO THE COMMAND
MStatus intersectCmd::undoIt()
{
	MStatus status;
	// undo not implemented
	return status;
}

// INITIALIZE THE PLUGIN:
MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "6.5", "Any");

	status = plugin.registerCommand("intersectCmd", intersectCmd::creator);

	return status;
}

// UNINITIALIZE THE PLUGIN:
MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);

	plugin.deregisterCommand("intersectCmd");

	return status;
}
