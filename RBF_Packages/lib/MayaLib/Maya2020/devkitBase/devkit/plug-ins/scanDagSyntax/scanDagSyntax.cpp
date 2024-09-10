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
// Produces the MEL command "scanDagSyntax". 
//
// This command plug-in provides the same functionality as scanDagCmd except
// that the syntax parsing is performed using syntax objects.
//
// The command accepts several flags:
//
//	-b/-breadthFirst  : Perform breadth first search 
//	-d/-depthFirst    : Perform depth first search 
//	-c/-cameras       : Limit the scan to cameras
//	-l/-lights        : Limit the scan to lights
//	-n/-nurbsSurfaces : Limit the scan to NURBS surfaces
//
////////////////////////////////////////////////////////////////////////

#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MArgList.h>
#include <maya/MFnCamera.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnLight.h>
#include <maya/MColor.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MIOStream.h>

#define kBreadthFlag			"-b"
#define kBreadthFlagLong		"-breadthFirst"
#define kDepthFlag				"-d"
#define kDepthFlagLong			"-depthFirst"
#define kCameraFlag				"-c"
#define kCameraFlagLong			"-cameras"
#define kLightFlag				"-l"
#define kLightFlagLong			"-lights"
#define kNurbsSurfaceFlag		"-n"
#define kNurbsSurfaceFlagLong	"-nurbsSurfaces"
#define kQuietFlag				"-q"
#define kQuietFlagLong			"-quiet"

class scanDagSyntax: public MPxCommand
{
public:
					scanDagSyntax() {};
				~scanDagSyntax() override;
	static void*	creator();
	static MSyntax	newSyntax();
	MStatus	doIt( const MArgList& ) override;

private:
	MStatus			parseArgs( const MArgList& args,
							   MItDag::TraversalType& traversalType,
							   MFn::Type& filter, bool & quiet);
	MStatus			doScan( const MItDag::TraversalType traversalType,
							MFn::Type filter, bool quiet);
	void			printTransformData(const MDagPath& dagPath, bool quiet);
};

scanDagSyntax::~scanDagSyntax() {}

void* scanDagSyntax::creator()
{
	return new scanDagSyntax;
}

MSyntax scanDagSyntax::newSyntax()
{
	MSyntax syntax;

	syntax.addFlag(kBreadthFlag, kBreadthFlagLong);
	syntax.addFlag(kDepthFlag, kDepthFlagLong);
	syntax.addFlag(kCameraFlag, kCameraFlagLong);
	syntax.addFlag(kLightFlag, kLightFlagLong);
	syntax.addFlag(kNurbsSurfaceFlag, kNurbsSurfaceFlagLong);
	syntax.addFlag(kQuietFlag, kQuietFlagLong);

	return syntax;
}


MStatus	scanDagSyntax::doIt( const MArgList& args )
{
	MItDag::TraversalType	traversalType = MItDag::kDepthFirst;
	MFn::Type				filter        = MFn::kInvalid;
	MStatus					status;
	bool					quiet = false;

	status = parseArgs ( args, traversalType, filter, quiet );
	if (!status)
		return status;

	return doScan( traversalType, filter, quiet);
};

MStatus scanDagSyntax::parseArgs( const MArgList& args,
								  MItDag::TraversalType& traversalType,
								  MFn::Type& filter,
								  bool & quiet)
{
	MStatus     	stat;
	MArgDatabase	argData(syntax(), args);

	MString     	arg;

	if (argData.isFlagSet(kBreadthFlag))
		traversalType = MItDag::kBreadthFirst;
	else if (argData.isFlagSet(kDepthFlag))
		traversalType = MItDag::kDepthFirst;

	if (argData.isFlagSet(kCameraFlag))
		filter = MFn::kCamera;
	else if (argData.isFlagSet(kLightFlag))
		filter = MFn::kLight;
	else if (argData.isFlagSet(kNurbsSurfaceFlag))
		filter = MFn::kNurbsSurface;
		
	if (argData.isFlagSet(kQuietFlag))
		quiet = true;
		
	return stat;
}

MStatus scanDagSyntax::doScan( const MItDag::TraversalType traversalType,
							   MFn::Type filter,
							   bool quiet)
{   
	MStatus status;

	MItDag dagIterator( traversalType, filter, &status);

	if ( !status) {
		status.perror("MItDag constructor");
		return status;
	}

	//	Scan the entire DAG and output the name and depth of each node

	if (traversalType == MItDag::kBreadthFirst)
	{
		if (!quiet)
			cout << endl << "Starting Breadth First scan of the Dag";
	} else {
		if (!quiet)
			cout << endl << "Starting Depth First scan of the Dag";
	}

	switch (filter) {
		case MFn::kCamera:
			if (!quiet)
				cout << ": Filtering for Cameras\n";
			break;
		case MFn::kLight:
			if (!quiet)
				cout << ": Filtering for Lights\n";
			break;
		case MFn::kNurbsSurface:
			if (!quiet)
				cout << ": Filtering for Nurbs Surfaces\n";
			break;
		default:
			cout << endl;
	}
	
	int objectCount = 0;
	for ( ; !dagIterator.isDone(); dagIterator.next() ) {

		MDagPath dagPath;

		status = dagIterator.getPath(dagPath);
		if ( !status ) {
			status.perror("MItDag::getPath");
			continue;
		}

		MFnDagNode dagNode(dagPath, &status);
		if ( !status ) {
			status.perror("MFnDagNode constructor");
			continue;
		}

		if (!quiet)
			cout << dagNode.name() << ": " << dagNode.typeName() << endl;

		if (!quiet)
			cout << "  dagPath: " << dagPath.fullPathName() << endl;

		objectCount += 1;
		if (dagPath.hasFn(MFn::kCamera)) {
			MFnCamera camera (dagPath, &status);
			if ( !status ) {
				status.perror("MFnCamera constructor");
				continue;
			}

			// Get the translation/rotation/scale data
			printTransformData(dagPath, quiet);

			// Extract some interesting Camera data
			if (!quiet)
			{
				cout << "  eyePoint: "
					 << camera.eyePoint(MSpace::kWorld) << endl;
				cout << "  upDirection: "
					 << camera.upDirection(MSpace::kWorld) << endl;
				cout << "  viewDirection: "
					 << camera.viewDirection(MSpace::kWorld) << endl;
				cout << "  aspectRatio: " << camera.aspectRatio() << endl;
				cout << "  horizontalFilmAperture: "
					 << camera.horizontalFilmAperture() << endl;
				cout << "  verticalFilmAperture: "
					 << camera.verticalFilmAperture() << endl;
			}
		} else if (dagPath.hasFn(MFn::kLight)) {
			MFnLight light (dagPath, &status);
			if ( !status ) {
				status.perror("MFnLight constructor");
				continue;
			}

			// Get the translation/rotation/scale data
			printTransformData(dagPath, quiet);

			// Extract some interesting Light data
			MColor color;

			color = light.color();
			if (!quiet)
			{
				cout << "  color: ["
					 << color.r << ", "
					 << color.g << ", "
					 << color.b << "]\n";
			}
			color = light.shadowColor();
			if (!quiet)
			{
				cout << "  shadowColor: ["
					 << color.r << ", "
					 << color.g << ", "
					 << color.b << "]\n";

				cout << "  intensity: " << light.intensity() << endl;
			}
		} else if (dagPath.hasFn(MFn::kNurbsSurface)) {
			MFnNurbsSurface surface (dagPath, &status);
			if ( !status ) {
				status.perror("MFnNurbsSurface constructor");
				continue;
			}

			// Get the translation/rotation/scale data
			printTransformData(dagPath, quiet);

			// Extract some interesting Surface data
			if (!quiet)
			{
				cout << "  numCVs: "
					 << surface.numCVsInU()
					 << " * "
					 << surface.numCVsInV()
					 << endl;
				cout << "  numKnots: "
					 << surface.numKnotsInU()
					 << " * "
					 << surface.numKnotsInV()
					 << endl;
				cout << "  numSpans: "
					 << surface.numSpansInU()
					 << " * "
					 << surface.numSpansInV()
					 << endl;
			}
		} else {
			// Get the translation/rotation/scale data
			printTransformData(dagPath, quiet);
		}
	}

	if (!quiet)
	{
		cout.flush();
	}
	setResult(objectCount);
	return MS::kSuccess;
}

void scanDagSyntax::printTransformData(const MDagPath& dagPath, bool quiet)
{
	MStatus		status;
	MObject		transformNode = dagPath.transform(&status);
	// This node has no transform - i.e., it's the world node
	if (!status && status.statusCode () == MStatus::kInvalidParameter)
		return;
	MFnDagNode	transform (transformNode, &status);
	if (!status) {
		status.perror("MFnDagNode constructor");
		return;
	}
	MTransformationMatrix	matrix (transform.transformationMatrix());

	if (!quiet)
	{
		cout << "  translation: " << matrix.getTranslation(MSpace::kWorld)
			 << endl;
	}
	double									threeDoubles[3];
	MTransformationMatrix::RotationOrder	rOrder;

	matrix.getRotation (threeDoubles, rOrder);
	if (!quiet)
	{
		cout << "  rotation: ["
			 << threeDoubles[0] << ", "
			 << threeDoubles[1] << ", "
			 << threeDoubles[2] << "]\n";
	}
	matrix.getScale (threeDoubles, MSpace::kWorld);
	if (!quiet)
	{
		cout << "  scale: ["
			 << threeDoubles[0] << ", "
			 << threeDoubles[1] << ", "
			 << threeDoubles[2] << "]\n";
	}
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus status;

	MFnPlugin plugin ( obj, PLUGIN_COMPANY, "3.0", "Any" );
	status = plugin.registerCommand( "scanDagSyntax", 
									 scanDagSyntax::creator,
									 scanDagSyntax::newSyntax ); 
	
	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus status;

	MFnPlugin plugin( obj );
	status = plugin.deregisterCommand( "scanDagSyntax" );

	return status;
}
