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

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// Produces the MEL command "exportJointClusterData".
//
// This example demonstrates how to find all joint cluster nodes and uses the MFnWeightGeometryFilter function
// set and MItGeometry iterator to export weights per CV for each geometry deformed by each joint cluster.
//
// To use this plug-in, build a skeleton and bind geometry using the "Rigid Bind" feature. Then type the command:
//
//	exportJointClusterData -f <fileName>;
//
// For example:
//
//	exportJointClusterData -f "C:/temp/skinData"
//
// The output format used is:
// 
//	jointName <skinCount>
//		skin_1 <weightCount>
//		<skin_1_component_index1> <skin_1_wt1>
//		<skin_1_component_index2> <skin_1_wt2>
//		<skin_1_component_index3> <skin_1_wt3>
//		...
//		skin_2 <weightCount>
//		<skin_2_component_index1> <skin_2_wt1>
//		<skin_2_component_index2> <skin_2_wt2>
//		<skin_2_component_index3> <skin_2_wt3>
//		...
//   
////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <maya/MPxCommand.h>
#include <maya/MStatus.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MFloatArray.h>
#include <maya/MObjectArray.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItGeometry.h>
#include <maya/MFnWeightGeometryFilter.h>
#include <maya/MFnGeometryFilter.h>
#include <maya/MItGeometry.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MFnSet.h>


#include <maya/MIOStream.h>

#define CheckError(stat,msg)		\
	if ( MS::kSuccess != stat ) {	\
		displayError(msg);			\
		continue;					\
	}


class exportJointClusterData : public MPxCommand
{
public:
                exportJointClusterData();
        ~exportJointClusterData() override;

	MStatus		parseArgs( const MArgList& args );
    MStatus     doIt ( const MArgList& args ) override;
    MStatus     redoIt () override;
    MStatus     undoIt () override;
    bool        isUndoable() const override;
	MObject     jointForCluster(MObject& jointCluster) const;

    static      void* creator();

private:
	FILE*		file;
};

exportJointClusterData::exportJointClusterData():
file(NULL)
{
}

exportJointClusterData::~exportJointClusterData() {}

void* exportJointClusterData::creator()
{
    return new exportJointClusterData;
}

bool exportJointClusterData::isUndoable() const
{
    return false;
}

MStatus exportJointClusterData::undoIt()
{
    return MS::kSuccess;
}

MStatus exportJointClusterData::parseArgs( const MArgList& args )
//
// There is one mandatory flag: -f/-file <filename>
//
{
	MStatus     	stat;
	MString     	arg;
	MString			fileName;
	const MString	fileFlag			("-f");
	const MString	fileFlagLong		("-file");

	// Parse the arguments.
	for ( unsigned int i = 0; i < args.length(); i++ ) {
		arg = args.asString( i, &stat );
		if (!stat)              
			continue;
				
		if ( arg == fileFlag || arg == fileFlagLong ) {
			// get the file name
			//
			if (i == args.length()-1) {
				arg += ": must specify a file name";
				displayError(arg);
				return MS::kFailure;
			}
			i++;
			args.get(i, fileName);
		}
		else {
			arg += ": unknown argument";
			displayError(arg);
			return MS::kFailure;
		}
	}

	file = fopen(fileName.asChar(),"wb");
	if (!file) {
		MString openError("Could not open: ");
		openError += fileName;
		displayError(openError);
		stat = MS::kFailure;
	}
	
	return stat;
}

MObject exportJointClusterData::jointForCluster(MObject& jointCluster) const
//
// Given a jointCluster, return the joint that drives it
//
{
	MObject result;
	MFnDependencyNode fnNode(jointCluster);
	MObject attrJoint = fnNode.attribute("matrix");
	MPlug pJointPlug(jointCluster,attrJoint);
	MPlugArray conns;
	if (pJointPlug.connectedTo(conns,true,false)) {
		result = conns[0].node();
	}
	return result;
}

MStatus exportJointClusterData::doIt( const MArgList& args )
//
// Process the command	
// 1. parse the args
// 2. find the jointClusters in the scene
// 3. iterate over their members, writing their weight data out to file	
//
{
	// parse args to get the file name from the command-line
	//
	MStatus stat = parseArgs(args);
	if (stat != MS::kSuccess) {
		return stat;
	}

	// count the processed jointClusters
	//
	unsigned int jcCount = 0;

	// Iterate through graph and search for jointCluster nodes
	//
	MItDependencyNodes iter( MFn::kJointCluster);
	for ( ; !iter.isDone(); iter.next() ) {
		MObject object = iter.thisNode();
		MFnWeightGeometryFilter jointCluster(object);

		// get the joint driving this cluster
		//
		MObject joint = jointForCluster(object);
		if (joint.isNull()) {
			displayError("Joint is not attached to cluster.");
			continue;
		}

		MObject deformSet = jointCluster.deformerSet(&stat);
		CheckError(stat,"Error getting deformer set.");
			
		MFnSet setFn(deformSet, &stat);	//need the fn to get the members
		CheckError(stat,"Error getting deformer set fn.");
		
		MSelectionList clusterSetList;

		//get all the members
		//
		stat = setFn.getMembers(clusterSetList, true);
		CheckError(stat,"Could not make member list with getMembers.");

		// print out the name of joint and the number of associated skins
		//
		MFnDependencyNode fnJoint(joint);
		fprintf(file,"%s %u\n",fnJoint.name().asChar(),
				clusterSetList.length());
		
		for (unsigned int kk = 0; kk < clusterSetList.length(); ++kk) {
			MDagPath skinpath;
			MObject components;
			MFloatArray weights;

			clusterSetList.getDagPath(kk,skinpath,components);
			jointCluster.getWeights(skinpath,components,weights);

			// print out the path name of the skin & the weight count
			//
			fprintf(file,
					"%s %u\n",skinpath.partialPathName().asChar(),
					weights.length());

			// loop through the components and print their index and weight
			//
			unsigned counter =0;
			MItGeometry gIter(skinpath,components);
			for (/* nothing */ ; !gIter.isDone() &&
								   counter < weights.length(); gIter.next()) {
				fprintf(file,"%d %f\n",gIter.index(),weights[counter]);
				counter++;
			}
		}
		jcCount++;
	}

	fclose(file);

	if (0 == jcCount) {
		displayError("No jointClusters found in this scene.");
		return MS::kFailure;
	}

	return MS::kSuccess;
}

MStatus exportJointClusterData::redoIt()
{
    clearResult();
	setResult( (int) 1);
    return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

    status = plugin.registerCommand( "exportJointClusterData", exportJointClusterData::creator );
	if (!status) {
		status.perror("registerCommand");
		return status;
	}

    return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterCommand( "exportJointClusterData" );
	if (!status) {
		status.perror("deregisterCommand");
	}
    return status;
}
