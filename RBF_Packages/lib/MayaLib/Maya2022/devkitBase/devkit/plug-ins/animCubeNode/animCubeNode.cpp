//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc. All rights reserved.
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
// This plug-in produces the dependency graph node "animCube".
// 
// It demonstrates how to take time as an input, and create polygonal geometry for output.
// The compute method of the node constructs a polygonal cube whose size depends on the current frame number.
// The resulting mesh is passed to an internal Maya node, which displays it and allows it to be positioned.
// 
// To use this node, execute the MEL command "animCubeNode.mel" that contains the following commands:
//
// createNode transform -n animCube1;
// createNode mesh -n animCubeShape1 -p animCube1;
// createNode animCube -n animCubeNode1;
// connectAttr time1.outTime animCubeNode1.time;
// connectAttr animCubeNode1.outputMesh animCubeShape1.inMesh;
// 
// This creates a mesh node under a transform node that is hooked into the world for display.
// It then creates an animCube node and connects its input to the time node, and its output to the mesh node.
// Now, a cube appears on the screen.
// 
// If the play button on the time slider is pressed, the displayed cube will grow and shrink as the frame number changes.
// 
////////////////////////////////////////////////////////////////////////

#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnMeshData.h>

#include <maya/MIOStream.h>

MStatus returnStatus;

#define McheckErr(stat,msg)			\
	if ( MS::kSuccess != stat ) {	\
		cerr << msg;				\
		return MS::kFailure;		\
	}

class animCube : public MPxNode
{
public:
					animCube() {};
			~animCube() override {};
	MStatus compute(const MPlug& plug, MDataBlock& data) override;
	static  void*	creator();
	static  MStatus initialize();

	static MObject	time;
	static MObject	outputMesh;
	static MTypeId	id;

protected:
	MObject createMesh(const MTime& time, MObject& outData, MStatus& stat);

	// Helpers
	MObject createQuads(const MFloatPointArray& points, MObject& outData, MStatus& stat);
	MObject createReverseQuads(const MFloatPointArray& points, MObject& outData, MStatus& stat);
	MObject createTris(const MFloatPointArray& points, MObject& outData, MStatus& stat);
	MObject createReverseTris(const MFloatPointArray& points, MObject& outData, MStatus& stat);
};

MObject animCube::time;
MObject animCube::outputMesh;
MTypeId animCube::id( 0x80000 );

void* animCube::creator()
{
	return new animCube;
}

MStatus animCube::initialize()
{
	MFnUnitAttribute unitAttr;
	MFnTypedAttribute typedAttr;

	MStatus returnStatus;

	animCube::time = unitAttr.create( "time", "tm",
										  MFnUnitAttribute::kTime,
										  0.0, &returnStatus );
	McheckErr(returnStatus, "ERROR creating animCube time attribute\n");


	animCube::outputMesh = typedAttr.create( "outputMesh", "out",
												 MFnData::kMesh,
												 MObject::kNullObj,
												 &returnStatus ); 
	McheckErr(returnStatus, "ERROR creating animCube output attribute\n");
	typedAttr.setStorable(false);

	returnStatus = addAttribute(animCube::time);
	McheckErr(returnStatus, "ERROR adding time attribute\n");

	returnStatus = addAttribute(animCube::outputMesh);
	McheckErr(returnStatus, "ERROR adding outputMesh attribute\n");

	returnStatus = attributeAffects(animCube::time,
								    animCube::outputMesh);
	McheckErr(returnStatus, "ERROR in attributeAffects\n");

	return MS::kSuccess;
}

MObject animCube::createMesh(const MTime& time,
							  MObject& outData,
							  MStatus& stat)

{
	// Scale the cube on the frame number, wrap every 10 frames.
	//
	const int frame = (int)time.as( MTime::kFilm );
	const float cubeSize = 0.5f * (float)( frame % 10 + 1 );

	MFloatPointArray points;
	points.append( -cubeSize, -cubeSize, -cubeSize );
	points.append(  cubeSize, -cubeSize, -cubeSize );
	points.append(  cubeSize, -cubeSize,  cubeSize );
	points.append( -cubeSize, -cubeSize,  cubeSize );
	points.append( -cubeSize,  cubeSize, -cubeSize );
	points.append( -cubeSize,  cubeSize,  cubeSize );
	points.append(  cubeSize,  cubeSize,  cubeSize );
	points.append(  cubeSize,  cubeSize, -cubeSize );

	MObject newMesh;

	static const bool sTestVertexIdAndFaceId =
		(getenv("MAYA_TEST_VERTEXID_AND_FACEID") != NULL);
	if (sTestVertexIdAndFaceId)
	{
		// If the env var is set, the topology of the cube will be changed over
		// frame number (looping in every 4 frames). When the shape is assigned
		// with a hwPhongShader, the shader receives vertex ids and face ids,
		// which are generated from polygonConnects passed to MFnMesh::create
		// method in this plugin.
		//
		switch (frame % 4)
		{
		case 1:
			newMesh = createQuads(points, outData, stat);
			break;
		case 2:
			newMesh = createReverseQuads(points, outData, stat);
			break;
		case 3:
			newMesh = createTris(points, outData, stat);
			break;
		case 0:
			newMesh = createReverseTris(points, outData, stat);
			break;
		default:
			newMesh = createQuads(points, outData, stat);
			break;
		}
	}
	else
	{
		newMesh = createQuads(points, outData, stat);
	}

	return newMesh;
}

MObject animCube::createQuads(
	const MFloatPointArray& points,
	MObject& outData,
	MStatus& stat)
{
	// Set up an array containing the number of vertices
	// for each of the 6 cube faces (4 vertices per face)
	//
	const int numFaces = 6;
	int face_counts[numFaces] = { 4, 4, 4, 4, 4, 4 };
	MIntArray faceCounts( face_counts, numFaces );

	// Set up and array to assign vertices from points to each face 
	//
	const int numFaceConnects = 24;
	int	face_connects[ numFaceConnects ] = {0, 1, 2, 3,
											4, 5, 6, 7,
											3, 2, 6, 5,
											0, 3, 5, 4,
											0, 4, 7, 1,
											1, 7, 6, 2};
	MIntArray faceConnects( face_connects, numFaceConnects );
	
	MFnMesh	meshFS;
	return meshFS.create(points.length(), faceCounts.length(),
		points, faceCounts, faceConnects, outData, &stat);
}

MObject animCube::createReverseQuads(
	const MFloatPointArray& points,
	MObject& outData,
	MStatus& stat)
{
	// Set up an array containing the number of vertices
	// for each of the 6 cube faces (4 vertices per face)
	//
	const int numFaces = 6;
	int face_counts[numFaces] = { 4, 4, 4, 4, 4, 4 };
	MIntArray faceCounts( face_counts, numFaces );

	// Set up and array to assign vertices from points to each face
	//
	const int numFaceConnects = 24;
	int	face_connects[ numFaceConnects ] = {0, 3, 2, 1,
											4, 7, 6, 5,
											3, 5, 6, 2,
											0, 4, 5, 3,
											0, 1, 7, 4,
											1, 2, 6, 7};
	MIntArray faceConnects( face_connects, numFaceConnects );

	MFnMesh	meshFS;
	return meshFS.create(points.length(), faceCounts.length(),
		points, faceCounts, faceConnects, outData, &stat);
}

MObject animCube::createTris(
	const MFloatPointArray& points,
	MObject& outData,
	MStatus& stat)
{
	// Set up an array containing the number of vertices
	// for each of the 12 triangles (3 verticies per triangle)
	//
	const int numFaces = 12;
	int face_counts[numFaces] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	MIntArray faceCounts( face_counts, numFaces );

	// Set up and array to assign vertices from points to each face
	//
	const int numFaceConnects = 36;
	int	face_connects[ numFaceConnects ] = {0, 1, 2,
											2, 3, 0,
											4, 5, 6,
											6, 7, 4,
											3, 2, 6,
											6, 5, 3,
											0, 3, 5,
											5, 4, 0,
											0, 4, 7,
											7, 1, 0,
											1, 7, 6,
											6, 2, 1};
	MIntArray faceConnects( face_connects, numFaceConnects );

	MFnMesh	meshFS;
	return meshFS.create(points.length(), faceCounts.length(),
		points, faceCounts, faceConnects, outData, &stat);
}

MObject animCube::createReverseTris(
	const MFloatPointArray& points,
	MObject& outData,
	MStatus& stat)
{
	// Set up an array containing the number of vertices
	// for each of the 12 triangles (3 verticies per triangle)
	//
	const int numFaces = 12;
	int face_counts[numFaces] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
	MIntArray faceCounts( face_counts, numFaces );

	// Set up and array to assign vertices from points to each face
	//
	const int numFaceConnects = 36;
	int	face_connects[ numFaceConnects ] = {0, 2, 1,
											2, 0, 3,
											4, 6, 5,
											6, 4, 7,
											3, 6, 2,
											6, 3, 5,
											0, 5, 3,
											5, 0, 4,
											0, 7, 4,
											7, 0, 1,
											1, 6, 7,
											6, 1, 2};
	MIntArray faceConnects( face_connects, numFaceConnects );

	MFnMesh	meshFS;
	return meshFS.create(points.length(), faceCounts.length(),
		points, faceCounts, faceConnects, outData, &stat);
}

MStatus animCube::compute(const MPlug& plug, MDataBlock& data)

{
	MStatus returnStatus;

	if (plug == outputMesh) {
		/* Get time */
		MDataHandle timeData = data.inputValue( time, &returnStatus ); 
		McheckErr(returnStatus, "Error getting time data handle\n");
		MTime time = timeData.asTime();

		/* Get output object */

		MDataHandle outputHandle = data.outputValue(outputMesh, &returnStatus);
		McheckErr(returnStatus, "ERROR getting polygon data handle\n");

		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		createMesh(time, newOutputData, returnStatus);
		McheckErr(returnStatus, "ERROR creating new Cube");

		outputHandle.set(newOutputData);
		data.setClean( plug );
	} else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "4.0", "Any");

	status = plugin.registerNode("animCube", animCube::id,
						 animCube::creator, animCube::initialize);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus	  status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(animCube::id);
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
