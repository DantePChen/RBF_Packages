//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related 
// material (collectively the "Data") in these files contain unpublished 
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
// licensors, which is protected by U.S. and Canadian federal copyright 
// law and by international treaties.
//
// The Data is provided for use exclusively by You. You have the right 
// to use, modify, and incorporate this Data into other products for 
// purposes authorized by the Autodesk software license agreement, 
// without fee.
//
// The copyright notices in the Software and this entire statement, 
// including the above license grant, this restriction and the 
// following disclaimer, must be included in all copies of the 
// Software, in whole or in part, and all derivative works of 
// the Software, unless such copies or derivative works are solely 
// in the form of machine-executable object code generated by a 
// source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
// PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
// TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
// BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
// DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
// AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
// OR PROBABILITY OF SUCH DAMAGES.
//
// ==========================================================================
//+

//
// testCameraSetCmd.cpp
//
// Description:
//     Sample plug-in that demonstrates how to use the MFnCameraSet class.
//
//	   testCameraSet -help will list the options.
//
// Example usages:
// testCameraSet -c;
// testCameraSet -c -cc leftCam rightCam;
// testCameraSet -c -cc leftCam rightCam;
// testCameraSet -c -n myCameraSet;
// testCameraSet -ac persp cameraSet1;
// testCameraSet -ac top cameraSet1;
// testCameraSet -d 0 cameraSet1;
// testCameraSet -q -camera -layer 0 cameraSet1;
// testCameraSet -e -camera side -layer 0 cameraSet1;
// testCameraSet -e -set defaultObjectSet -layer 0 cameraSet1;
// testCameraSet -q -set -layer 0 cameraSet1;
// testCameraSet -e -set "" -layer 0 cameraSet1;
// testCameraSet -e -active true -layer 0 cameraSet1;
// testCameraSet -q -active -layer 0 cameraSet1;
// testCameraSet -e -order 2 -layer 0 cameraSet1;
// testCameraSet -q -order -layer 0 cameraSet1;
// testCameraSet -q -numLayers cameraSet1;
//
//
#include <maya/MPxCommand.h>
#include <maya/MArgList.h>
#include <maya/MCallbackIdArray.h>
#include <maya/MDagMessage.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MDGMessage.h>
#include <maya/MCameraSetMessage.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnCameraSet.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MIntArray.h>
#include <maya/MModelMessage.h>
#include <maya/MObjectArray.h>
#include <maya/MPlug.h>
#include <maya/MSelectionList.h>

#define kCmdName					"testCameraSet"

#define kEditFlag 					"-e"
#define kEditFlagLong				"-edit"
#define kQueryFlag 					"-q"
#define kQueryFlagLong				"-query"
#define kActiveFlag 				"-a"
#define kActiveFlagLong				"-active"
#define kCameraFlag 				"-cam"
#define kCameraFlagLong				"-camera"
#define kCreateFlag 				"-c"
#define kCreateFlagLong				"-create"
#define kCreateWithCamerasFlag 		"-cc"
#define kCreateWithCamerasFlagLong	"-createWithCameras"
#define kAppendCameraFlag 			"-ac"
#define kAppendCameraFlagLong		"-appendCamera"
#define kAppendCameraAndSetFlag 	"-acs"
#define kAppendCameraAndSetFlagLong	"-appendCameraAndSet"
#define kInsertCameraFlag 			"-ic"
#define kInsertCameraFlagLong		"-insertCamera"
#define kInsertCameraAndSetFlag 	"-ics"
#define kInsertCameraAndSetFlagLong	"-insertCameraAndSet"
#define kDeleteAllFlag 				"-da"
#define kDeleteAllFlagLong			"-deleteAll"
#define kDeleteLayerFlag 			"-d"
#define kDeleteLayerFlagLong		"-deleteLayer"
#define kNameFlag 					"-n"
#define kNameFlagLong				"-name"
#define kNumLayersFlag 				"-nl"
#define kNumLayersFlagLong			"-numLayers"
#define kOrderFlag 					"-o"
#define kOrderFlagLong				"-order"
#define kLayerFlag 					"-l"
#define kLayerFlagLong				"-layer"
#define kSetFlag 					"-s"
#define kSetFlagLong				"-set"

#define kHelpFlag					"-h"
#define kHelpFlagLong				"-help"

#define CheckErrorContinue(stat, msg)	\
	if (MS::kSuccess != stat) {			\
		displayError(msg);				\
		continue;						\
	}							

#define CheckErrorReturn(stat, msg)		\
	if (MS::kSuccess != stat) {			\
		displayError(msg);				\
		return;							\
	}

MCallbackIdArray idArray;

//////////////////////////////////////////////////////////////////////////
//
// Command class declaration
//
//////////////////////////////////////////////////////////////////////////

class testCameraSetCmd : public MPxCommand
{
public:
					testCameraSetCmd() {};
				~testCameraSetCmd() override; 
	MStatus			doIt( const MArgList& args ) override;

	static void*	creator();
private:
	MStatus			parseArgs(const MArgList &args);
	void			getCamPath(const MString &camName, MDagPath &camPath);

	bool 			createUsed;
	bool 			createWithCamerasUsed;
	bool 			editUsed;
	bool 			queryUsed;
	bool 			activeUsed;
	bool 			appendCameraUsed;
	bool 			appendCameraAndSetUsed;
	bool 			insertCameraUsed;
	bool 			insertCameraAndSetUsed;
	bool 			cameraUsed;
	bool 			deleteAllUsed;
	bool 			deleteLayerUsed;
	bool 			numLayersUsed;
	bool 			orderUsed;
	bool 			layerUsed;
	bool 			setUsed;
	bool 			helpUsed;

	unsigned int	cameraLayer;
	bool			activeVal;
	int				orderVal;
	MString			createName;
	MString			camName;
	MStringArray	camNameList;
	MString			setName;
	MSelectionList	list;

};

void cameraLayerCB(MObject& cameraSetNode, unsigned int multiIndex, bool added, void *clientData)
{
	MFnDependencyNode nodeFn(cameraSetNode);
	MString str;
	if (added)
	{
		str = MString("Added camera layer with index ");
		str += multiIndex;
		str += " to ";
		str += nodeFn.name();
		MGlobal::displayInfo(str);
	}
	else
	{
		str = MString("Removed camera layer with index ");
		str += multiIndex;
		str += " from ";
		str += nodeFn.name();
		MGlobal::displayInfo(str);
	}
}

void cameraChangedCB(MObject& cameraSetNode, unsigned int multiIndex, MObject &oldCamera, MObject &newCamera, void *clientData)
{
	MFnDependencyNode nodeFn(cameraSetNode);
	MFnDependencyNode oldCamFn(oldCamera);
	MFnDependencyNode newCamFn(newCamera);
	MString str("Camera at index ");
	str += multiIndex;
	str += " changed from ";
	str += oldCamFn.name();
	str += " to ";
	str += newCamFn.name();
	MGlobal::displayInfo(str);
}

//////////////////////////////////////////////////////////////////////////
//
// Command class implementation
//
//////////////////////////////////////////////////////////////////////////

testCameraSetCmd::~testCameraSetCmd()
{
	createName = "";
}

void* testCameraSetCmd::creator()
{
	return new testCameraSetCmd();
}

MStatus testCameraSetCmd::doIt( const MArgList& args)
//
// Parses the given command line arguments and executes them.
//
{
	MStatus status = parseArgs(args);
	if (status != MS::kSuccess)
	{
		MGlobal::displayError("Error parsing argument list");
		return status;
	}

	bool nothingSet = (	!createUsed && !appendCameraUsed && !appendCameraAndSetUsed &&
 						!deleteLayerUsed && !cameraUsed && !insertCameraUsed &&
						!insertCameraAndSetUsed && !orderUsed && !layerUsed && !helpUsed &&
						!setUsed && !numLayersUsed && !deleteAllUsed && !createWithCamerasUsed);

	if (nothingSet)
	{
		MGlobal::displayError("A flag must be used. testCameraSet -help for availible flags.");
		return MS::kFailure;
	}

	if (helpUsed) {
		MGlobal::displayInfo("testCameraSet -help");
		MGlobal::displayInfo("\ttestCameraSet tests the functionality of the MFnCameraSet function set.");
		MGlobal::displayInfo("");
		MGlobal::displayInfo("\t-h -help : This message is printed");
		MGlobal::displayInfo("\t-a -active [true/false]: Set/get whether a particular layer is active");
		MGlobal::displayInfo("\t-ac -appendCamera <cameraName>: Append a new camera layer to the cameraSet using the specified camera");
		MGlobal::displayInfo("\t-acs -appendCameraAndSet <cameraName> <setName>: Append a new camera layer to the cameraSet using the specified camera and set");
		MGlobal::displayInfo("\t-ic -insertCamera <layerIndex> <cameraName>: Insert a new camera layer to the cameraSet at the given layer index using the specified camera");
		MGlobal::displayInfo("\t-ics -insertCameraAndSet <layerIndex> <cameraName> <setName>: Insert a new camera layer to the cameraSet at the given layer using the specified camera and set");
		MGlobal::displayInfo("\t-cam -camera [<cameraName>]: Set/get the camera for a particular layer");
		MGlobal::displayInfo("\t-c -create : Create a new cameraSet node");
		MGlobal::displayInfo("\t-cc -createWithCameras <cameraName> ...: Create a new cameraSet node with a set of camera layers governed by a list of cameras");
		MGlobal::displayInfo("\t-d -deleteLayer <layerIndex>: Delete the layer at the given index");
		MGlobal::displayInfo("\t-da -deleteAll: Delete all camera layers");
		MGlobal::displayInfo("\t-n  -name: Specifies a name for a newly created cameraSet.  Only used with the -create flag");
		MGlobal::displayInfo("\t-np -numLayers: Returns the number of layers defined in the specified cameraSet");
		MGlobal::displayInfo("\t-o -order [<order number>]: Set the order in which a particular layer is processed");
		MGlobal::displayInfo("\t-p -layer <layerIndex>: Specifies the layer index to be used when accessing layer information");
		MGlobal::displayInfo("\t-s -set [<setName>]: Set/get the set for a particular layer");
		MGlobal::displayInfo("\t-e -edit : Perform an edit operation");
		MGlobal::displayInfo("\t-q -query : Perform a query operation");
		MGlobal::displayInfo("");
		return status;
	}

	if (createUsed)
	{
		// Create a new cameraSet node.
		MFnCameraSet dirFn;
		MObject dirObj = MObject::kNullObj;
		if (createName.length() > 0)
			dirObj = dirFn.create(createName, &status);
		else
			dirObj = dirFn.create(&status);
		if (!status)
		{
			MGlobal::displayError("Could not create a cameraSet node");
			return status;
		}
		else if (createWithCamerasUsed)
		{
			// Create a list of camera paths given the list of camera names.
			MDagPathArray camPaths;
			unsigned int c;
			for (c = 0; c < camNameList.length(); c++)
			{
				MDagPath camPath;
				getCamPath(camNameList[c], camPath);
				if (!camPath.isValid())
				{
					MGlobal::displayError("-createWithCameras must have a list of valid camera nodes specified");
					status = MS::kFailure;
					return status;
				}
				camPaths.append(camPath);
			}

			// Create a new cameraSet node with the list of camera layers.
			MFnCameraSet dirFn;
			MObject dirObj = dirFn.create(camPaths, &status);
			if (!status)
			{
				MGlobal::displayError("Could not create a cameraSet node");
				return status;
			}
			MGlobal::select(dirObj, MGlobal::kReplaceList);
			return status;
		}
		MGlobal::select(dirObj, MGlobal::kReplaceList);
		return status;
	}

	unsigned nObjs = list.length();
	if (nObjs == 0)
	{
		if (appendCameraUsed || appendCameraAndSetUsed || deleteLayerUsed || editUsed ||
				cameraUsed || insertCameraUsed || insertCameraAndSetUsed || 
				setUsed || activeUsed || numLayersUsed || deleteAllUsed) {
			MGlobal::displayError("Must specify a cameraSet node");
			status = MS::kFailure;
			return status;
		}
	}
	else
	{
		if (createUsed || createWithCamerasUsed)
		{
			MGlobal::displayError("-create cannot have any object specifed");
			status = MS::kFailure;
			return status;
		}

		if (appendCameraUsed)
		{
			if (nObjs != 1)
			{
				MGlobal::displayError("-appendCamera must have a single cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get the specified cameraSet node.
			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-appendCamera must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get a dag path to the specified camera.
			MDagPath camPath;
			getCamPath(camName, camPath);
			if (!camPath.isValid())
			{
				MGlobal::displayError("-appendCamera must have a valid camera node specified");
				status = MS::kFailure;
				return status;
			}

			// Call the MFnCameraSet method to append the layer.
			MFnCameraSet dirFn(dirNode);
			status = dirFn.appendLayer(camPath);

			return status;
		}


		if (appendCameraAndSetUsed)
		{
			if (nObjs != 1)
			{
				MGlobal::displayError("-appendCameraAndSet must have a single cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get the specified cameraSet node.
			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-appendCameraAndSet must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get a dag path to the specified camera.
			MDagPath camPath;
			getCamPath(camName, camPath);
			if (!camPath.isValid())
			{
				MGlobal::displayError("-appendCameraAndSet must have a valid camera node specified");
				status = MS::kFailure;
				return status;
			}

			// Get the specified set node.
			MSelectionList setList;
			setList.add(setName);
			MObject setObj = MObject::kNullObj;
			setList.getDependNode(0, setObj);
			if (setObj == MObject::kNullObj)
			{
				MGlobal::displayError("-appendCameraAndSet must have a valid set node specified");
				status = MS::kFailure;
				return status;
			}

			// Call the MFnCameraSet method to append the layer.
			MFnCameraSet dirFn(dirNode);
			status = dirFn.appendLayer(camPath, setObj);

			return status;
		}


		if (insertCameraUsed)
		{
			if (nObjs != 1)
			{
				MGlobal::displayError("-insertCamera must have a single cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get the specified cameraSet node.
			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-insertCamera must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get a dag path to the specified camera.
			MDagPath camPath;
			getCamPath(camName, camPath);
			if (!camPath.isValid())
			{
				MGlobal::displayError("-insertCamera must have a valid camera node specified");
				status = MS::kFailure;
				return status;
			}

			// Call the MFnCameraSet method to insert the layer.
			MFnCameraSet dirFn(dirNode);
			status = dirFn.insertLayer(cameraLayer, camPath);

			return status;
		}

		if (insertCameraAndSetUsed)
		{
			if (nObjs != 1)
			{
				MGlobal::displayError(
						"-insertCameraAndSet must have a single cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get the specified cameraSet node.
			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError(
						"-insertCameraAndSet must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Get a dag path to the specified camera.
			MDagPath camPath;
			getCamPath(camName, camPath);
			if (!camPath.isValid())
			{
				MGlobal::displayError(
						"-insertCameraAndSet must have a valid camera node specified");
				status = MS::kFailure;
				return status;
			}

			// Get the specified set node.
			MSelectionList setList;
			setList.add(setName);
			MObject setObj = MObject::kNullObj;
			setList.getDependNode(0, setObj);
			if (setObj == MObject::kNullObj)
			{
				MGlobal::displayError(
						"-insertCameraAndSet must have a valid set node specified");
				status = MS::kFailure;
				return status;
			}

			// Call the MFnCameraSet method to insert the layer.
			MFnCameraSet dirFn(dirNode);
			status = dirFn.insertLayer(cameraLayer, camPath, setObj);

			return status;
		}

		if (deleteLayerUsed)
		{
			if (nObjs != 1)
			{
				MGlobal::displayError("-deleteLayer must have a single cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-deleteLayer must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Call the MFnCameraSet method to delete the layer.
			MFnCameraSet dirFn(dirNode);
			status = dirFn.deleteLayer(cameraLayer);
	
			return status;
		}

		if (deleteAllUsed)
		{
			if (nObjs != 1)
			{
				MGlobal::displayError("-deleteAll must have a single cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-deleteAll must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			// Call the MFnCameraSet method to delete the layers.
			MFnCameraSet dirFn(dirNode);
			status = dirFn.clear();
	
			return status;
		}

		if (numLayersUsed)
		{
			if (queryUsed)
			{
				MObject dirNode = MObject::kNullObj;
				list.getDependNode(0, dirNode);
				if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
				{
					MGlobal::displayError("-numLayers must have a valid cameraSet node specified");
					status = MS::kFailure;
					return status;
				}

				// Call the MFnCameraSet method to get the number of layers.
				MFnCameraSet dirFn(dirNode);
				unsigned int numLayers = dirFn.getNumLayers(&status);
				if (status != MS::kSuccess)	
					MGlobal::displayError("Error getting the number of layers");
				setResult((int)numLayers);
			}
			else
			{
				MGlobal::displayError("-numLayers requires the query flag to be used");
				status = MS::kFailure;
			}
			return status;
		}

		if (cameraUsed)
		{
			if ((nObjs != 1) || (!layerUsed))
			{
				MGlobal::displayError("-camera must have a cameraSet node and layer specified");
				status = MS::kFailure;
				return status;
			}

			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-camera must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-camera must have a valid camera layer index specified");
				status = MS::kFailure;
				return status;
			}

			if (editUsed)
			{
				// Get a dag path to the specified camera.
				MDagPath camPath;
				getCamPath(camName, camPath);
				if (!camPath.isValid())
				{
					MGlobal::displayError("-camera must have a valid camera node specified");
					status = MS::kFailure;
					return status;
				}

				// Call the MFnCameraSet method to set the camera.
				MFnCameraSet dirFn(dirNode);
				dirFn.setLayerCamera((unsigned int)cameraLayer, camPath);
			}
			else if (queryUsed)
			{
				// Call the MFnCameraSet method to get the camera.
				MDagPath camPath;
				MFnCameraSet dirFn(dirNode);
				status = dirFn.getLayerCamera((unsigned int)cameraLayer, camPath);
				if (status == MS::kSuccess)
				{
					MObject camNode = camPath.node();
					MFnDependencyNode nodeFn(camNode);
					setResult(nodeFn.name());
				}
				else
					setResult("");
			}
		}

		if (setUsed)
		{
			if ((nObjs != 1) || (!layerUsed))
			{
				MGlobal::displayError("-set must have a cameraSet node and layer specified");
				status = MS::kFailure;
				return status;
			}

			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-set must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-set must have a valid camera layer index specified");
				status = MS::kFailure;
				return status;
			}

			if (editUsed)
			{
				// Get the specified set node.
				MObject setObj = MObject::kNullObj;
				if (setName != "")
				{
					MSelectionList setList;
					setList.add(setName);
					setList.getDependNode(0, setObj);
					if (setObj == MObject::kNullObj)
					{
						MGlobal::displayError("-set must have a valid set node specified");
						status = MS::kFailure;
						return status;
					}
				}

				// Call the MFnCameraSet method to set the set node.
				MFnCameraSet dirFn(dirNode);
				dirFn.setLayerSceneData((unsigned int)cameraLayer, setObj);
			}
			else if (queryUsed)
			{
				// Call the MFnCameraSet method to get the set node.
				MObject setObj;
				MFnCameraSet dirFn(dirNode);
				status = dirFn.getLayerSceneData((unsigned int)cameraLayer, setObj);
				if (status == MS::kSuccess)
				{
					MFnDependencyNode nodeFn(setObj);
					setResult(nodeFn.name());
				}
				else
					setResult("");
			}
		}

		if (activeUsed)
		{
			if ((nObjs != 1) || (!layerUsed))
			{
				MGlobal::displayError("-active must have a cameraSet node and layer specified");
				status = MS::kFailure;
				return status;
			}

			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-active must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-active must have a valid camera layer index specified");
				status = MS::kFailure;
				return status;
			}

			if (editUsed)
			{
				// Call the MFnCameraSet method to set the set node.
				MFnCameraSet dirFn(dirNode);
				dirFn.setLayerActive((unsigned int)cameraLayer, activeVal);
			}
			else if (queryUsed)
			{
				// Call the MFnCameraSet method to get the active value.
				MFnCameraSet dirFn(dirNode);
				activeVal = dirFn.isLayerActive((unsigned int)cameraLayer, &status);
				if (status == MS::kSuccess)
					setResult(activeVal);
				else
					MGlobal::displayError("Could not get active value");
			}
		}

		if (orderUsed)
		{
			if ((nObjs != 1) || (!orderUsed))
			{
				MGlobal::displayError("-order must have a cameraSet node and layer specified");
				status = MS::kFailure;
				return status;
			}

			MObject dirNode = MObject::kNullObj;
			list.getDependNode(0, dirNode);
			if ((dirNode == MObject::kNullObj) || (!dirNode.hasFn(MFn::kCameraSet)))
			{
				MGlobal::displayError("-order must have a valid cameraSet node specified");
				status = MS::kFailure;
				return status;
			}

			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-order must have a valid camera layer index specified");
				status = MS::kFailure;
				return status;
			}

			if (editUsed)
			{
				// Call the MFnCameraSet method to set the layer order.
				MFnCameraSet dirFn(dirNode);
				dirFn.setLayerOrder((unsigned int)cameraLayer, orderVal);
			}
			else if (queryUsed)
			{
				// Call the MFnCameraSet method to get the layer order.
				MFnCameraSet dirFn(dirNode);
				status = dirFn.getLayerOrder((unsigned int)cameraLayer, orderVal);
				if (status == MS::kSuccess)
					setResult(orderVal);
				else
					MGlobal::displayError("Could not get layer order");
			}
		}
	}
	
	return status;
}

MStatus testCameraSetCmd::parseArgs( const MArgList& args)
//
// Parses the command line arguments.
//
{
	MStatus status;

	//	Get the flags.
	editUsed = (args.flagIndex(kEditFlag, kEditFlagLong) != MArgList::kInvalidArgIndex);
	queryUsed = (args.flagIndex(kQueryFlag, kQueryFlagLong) != MArgList::kInvalidArgIndex);
	helpUsed = (args.flagIndex(kHelpFlag, kHelpFlagLong) != MArgList::kInvalidArgIndex);
	numLayersUsed = (args.flagIndex(kNumLayersFlag, kNumLayersFlagLong) != MArgList::kInvalidArgIndex);

	// If flags are used which require no other information, return now.
	if (helpUsed)
		return MS::kSuccess;

	unsigned int maxArg = args.length() - 1;
	unsigned int createWithCamerasIndex = args.flagIndex(kCreateWithCamerasFlag, kCreateWithCamerasFlagLong);
	unsigned int createIndex = args.flagIndex(kCreateFlag, kCreateFlagLong);
	unsigned int activeIndex = args.flagIndex(kActiveFlag, kActiveFlagLong);
	unsigned int appendCameraIndex = args.flagIndex(kAppendCameraFlag, kAppendCameraFlagLong);
	unsigned int appendCameraAndSetIndex = args.flagIndex(kAppendCameraAndSetFlag,
			kAppendCameraAndSetFlagLong);
	unsigned int insertCameraIndex = args.flagIndex(kInsertCameraFlag, kInsertCameraFlagLong);
	unsigned int insertCameraAndSetIndex = args.flagIndex(kInsertCameraAndSetFlag,
			kInsertCameraAndSetFlagLong);
	unsigned int deleteAllIndex = args.flagIndex(kDeleteAllFlag, kDeleteAllFlagLong);
	unsigned int cameraIndex = args.flagIndex(kCameraFlag, kCameraFlagLong);
	unsigned int deleteLayerIndex = args.flagIndex(kDeleteLayerFlag, kDeleteLayerFlagLong);
	unsigned int orderIndex = args.flagIndex(kOrderFlag, kOrderFlagLong);
	unsigned int layerIndex = args.flagIndex(kLayerFlag, kLayerFlagLong);
	unsigned int setIndex = args.flagIndex(kSetFlag, kSetFlagLong);
	createWithCamerasUsed = (createWithCamerasIndex != MArgList::kInvalidArgIndex);
	createUsed = (createIndex != MArgList::kInvalidArgIndex);
	activeUsed = (activeIndex != MArgList::kInvalidArgIndex);
	appendCameraUsed = (appendCameraIndex != MArgList::kInvalidArgIndex);
	appendCameraAndSetUsed = (appendCameraAndSetIndex != MArgList::kInvalidArgIndex);
	insertCameraUsed = (insertCameraIndex != MArgList::kInvalidArgIndex);
	insertCameraAndSetUsed = (insertCameraAndSetIndex != MArgList::kInvalidArgIndex);
	cameraUsed = (cameraIndex != MArgList::kInvalidArgIndex);
	deleteAllUsed = (deleteAllIndex != MArgList::kInvalidArgIndex);
	deleteLayerUsed = (deleteLayerIndex != MArgList::kInvalidArgIndex);
	orderUsed = (orderIndex != MArgList::kInvalidArgIndex);
	layerUsed = (layerIndex != MArgList::kInvalidArgIndex);
	setUsed = (setIndex != MArgList::kInvalidArgIndex);

	// Process each flag.
	bool maxArgUsed = false;
	if (createWithCamerasUsed)
	{
		// Use the rest of the arguments as camera names.
		for (unsigned int c = createWithCamerasIndex+1; c <= maxArg; c++)
		{
			MString camName = args.asString(c, &status);
			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-createWithCameras must have list of valid camera nodes specified");
				return status;
			}
			camNameList.append(camName);
		}
		maxArgUsed = true;
	}

	if (createUsed)
	{
		if (createIndex == maxArg)
			maxArgUsed = true;
	}
	if (activeUsed)
	{
		if (editUsed)
		{
			activeVal = args.asBool((activeIndex+1), &status);
			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-active must be either true or false");
				return status;
			}
			if ((activeIndex+1) == maxArg)
				maxArgUsed = true;
		}
	}
	if (appendCameraUsed)
	{
		camName = args.asString((appendCameraIndex+1), &status);
		if (status != MS::kSuccess)
		{
			MGlobal::displayError("-appendCamera must have a valid camera node specified");
			return status;
		}

		if ((appendCameraIndex+1) == maxArg)
			maxArgUsed = true;
	}
	if (appendCameraAndSetUsed)
	{
		camName = args.asString((appendCameraAndSetIndex+1));
		setName = args.asString((appendCameraAndSetIndex+2));
		if ((appendCameraAndSetIndex+2) == maxArg)
			maxArgUsed = true;
	}
	if (insertCameraUsed)
	{
		cameraLayer = args.asInt(insertCameraIndex+1, &status);
		if (status != MS::kSuccess)
		{
			MGlobal::displayError("-insertCamera must have a valid camera layer specified");
			return status;
		}
		camName = args.asString((insertCameraIndex+2), &status);
		if (status != MS::kSuccess)
		{
			MGlobal::displayError("-insertCamera must have a valid camera node specified");
			return status;
		}

		if ((insertCameraIndex+2) == maxArg)
			maxArgUsed = true;
	}
	if (insertCameraAndSetUsed)
	{
		cameraLayer = args.asInt(insertCameraAndSetIndex+1, &status);
		camName = args.asString((insertCameraAndSetIndex+2));
		setName = args.asString((insertCameraAndSetIndex+3));
		if ((insertCameraAndSetIndex+3) == maxArg)
			maxArgUsed = true;
	}
	if (cameraUsed)
	{
		if (editUsed)
		{
			camName = args.asString(cameraIndex+1);
			if ((cameraIndex+1) == maxArg)
				maxArgUsed = true;
		}
	}
	if (deleteLayerUsed)
	{
		cameraLayer = args.asInt(deleteLayerIndex+1);
		if ((deleteLayerIndex+1) == maxArg)
			maxArgUsed = true;
	}
	if (orderUsed)
	{
		if (editUsed)
		{
			orderVal = args.asInt((orderIndex+1), &status);
			if (status != MS::kSuccess)
			{
				MGlobal::displayError("-order must be either true or false");
				return status;
			}
			if ((orderIndex+1) == maxArg)
				maxArgUsed = true;
		}
	}
	if (layerUsed)
	{
		cameraLayer = args.asInt(layerIndex+1);
		if ((layerIndex+1) == maxArg)
			maxArgUsed = true;
	}
	if (setUsed)
	{
		if (editUsed)
		{
			setName = args.asString(setIndex+1);
			if ((setIndex+1) == maxArg)
				maxArgUsed = true;
		}
	}

	// If all of the arguments have been used, get the cameraSet node from the selection list.
	// Otherwise, get it from the last argument.
	if (maxArgUsed)
		MGlobal::getActiveSelectionList(list);
	else
	{
		if (createUsed)
			createName = args.asString(maxArg);
		else
			list.add(args.asString(maxArg));
	}

	return MS::kSuccess;
}

void testCameraSetCmd::getCamPath(const MString &camName, MDagPath &camPath)
//
// Set the camera path to the camera by the given name.
{
	MSelectionList camList;
	camList.add(camName);
	camList.getDagPath(0, camPath);
}


//////////////////////////////////////////////////////////////////////////
//
// Plugin registration
//
//////////////////////////////////////////////////////////////////////////

MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	MStatus stat = plugin.registerCommand( kCmdName, testCameraSetCmd::creator);

	if (stat == MS::kSuccess)
	{
		// Register a callback to be called whenever a camera layer is added or removed from the cameraSet.
		MCallbackId id = MCameraSetMessage::addCameraLayerCallback(cameraLayerCB);
		if (id != 0)
			idArray.append(id);
		id = MCameraSetMessage::addCameraChangedCallback(cameraChangedCB);
		if (id != 0)
			idArray.append(id);
	}

	return stat;
}

MStatus uninitializePlugin( MObject obj)
{
	MFnPlugin plugin( obj );
	MMessage::removeCallbacks(idArray);
	return plugin.deregisterCommand( kCmdName );
}


