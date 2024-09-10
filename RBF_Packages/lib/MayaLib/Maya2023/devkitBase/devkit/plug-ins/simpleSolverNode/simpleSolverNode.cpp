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

//////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// Registers the IK solver "simpleSolverNode".
// This is a single-bone, single-plane IK solver.
//
// This plugin demonstrates how to create and register an IK solver.
// Due to the complex nature of IK solvers, this plug-in only 
// works with 2-joint skeletons (1 bone) in the x-y plane.
//
// To use the solver:
//	(1) Create a single IK bone with 2 joints using the Joint tool.
//	(2) Enter the following command in the command window to create
//		an IK handle which uses the new solver:
//
//		ikHandle -sol simpleSolverNode -sj joint1 -ee joint2
//
// This creates a handle that can be dragged around in the x-y
// plane. Moving the handle in the x-y plane will rotate the joint.
//
// The following command can be used to determine which solver a handle is using: 
//
//	ikHandle -q -sol handleName 
//
//////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include <maya/MPxIkSolverNode.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>
#include <maya/MIkHandleGroup.h>
#include <maya/MFnIkHandle.h>
#include <maya/MDagPath.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MDoubleArray.h>


#define MAX_ITERATIONS 1
#define kSolverType "simpleSolverNode"

//////////////////////////////////////////////////////////////////
//
// Class declaration
//
//////////////////////////////////////////////////////////////////
class simpleSolverNode : public MPxIkSolverNode {

public:
					simpleSolverNode();
    			~simpleSolverNode() override;

	MStatus	doSolve() override;
	MString	solverTypeName() const override;

	static	void*	creator();
	static  MStatus         initialize();

	static	MTypeId	id;

private:
	MStatus			doSimpleSolver();

};

MTypeId simpleSolverNode::id( 0x80100 );


//////////////////////////////////////////////////////////////////
//
// Implementation
//
//////////////////////////////////////////////////////////////////
simpleSolverNode::simpleSolverNode()
	: MPxIkSolverNode()
{
	// setMaxIterations( MAX_ITERATIONS );
}

simpleSolverNode::~simpleSolverNode() {}

void* simpleSolverNode::creator()
{
	return new simpleSolverNode;
}

MStatus simpleSolverNode::initialize()
{ 
	return MS::kSuccess;
}

MString simpleSolverNode::solverTypeName() const
//
// This method returns the type name used to identify this solver.
//
{
	return MString( kSolverType );
}

MStatus simpleSolverNode::doSolve()
//
// This is the core solver.
//
{
	doSimpleSolver();
	return MS::kSuccess;
}

MStatus simpleSolverNode::doSimpleSolver()
//
// Solve single joint in the x-y plane
//
// - first it calculates the angle between the handle and the end-effector.
// - then it determines which way to rotate the joint.
//
{
	MStatus stat;

	// Get the handle and create a function set for it
	//	
	MIkHandleGroup * handle_group = handleGroup();
	if (NULL == handle_group) {
		return MS::kFailure;
	}
	MObject handle = handle_group->handle( 0 );
	MDagPath handlePath;
	MDagPath::getAPathTo( handle, handlePath);
	MFnIkHandle fnHandle(handlePath, &stat);


	// Get the position of the end_effector
	//
	MDagPath end_effector;
	fnHandle.getEffector(end_effector);
	MFnTransform tran( end_effector );
	MPoint effector_position = tran.rotatePivot( MSpace::kWorld );


	// Get the position of the handle
	//
	MPoint handle_position = fnHandle.rotatePivot( MSpace::kWorld );


	// Get the start joint position
	//
	MDagPath start_joint;
	fnHandle.getStartJoint( start_joint );
	MFnTransform start_transform( start_joint );
	MPoint start_position = start_transform.rotatePivot( MSpace::kWorld );


	// Calculate the rotation angle
	//
	MVector v1 = start_position - effector_position;
	MVector v2 = start_position - handle_position;
	double angle = v1.angle( v2 );



	// -------- Figure out which way to rotate --------
	//
	//  define two vectors U and V as follows
	//  U   =   EndEffector(E) - StartJoint(S)
	//  N   =   Normal to U passing through EndEffector
	//
	//  Clip handle_position to half-plane U to determine the region it
	//  lies in. Use the region to determine  the rotation direction.
	//
	//             U
	//             ^              Region      Rotation
	//             |  B           
	//            (E)---N            A          C-C-W
	//         A   |                 B           C-W
	//             |  B
	//             |
	//            (S)
	//
	#define CW		 rot = -1.0 * angle;	// clockwise
	#define CCW		 rot = angle;			// counter-clockwise

	double rot = 0.0;	// Rotation about Z-axis


	// U and N define a half-plane to clip the handle against
	//
	MVector U = effector_position - start_position;
	U.normalize();
	

	// Get a normal to U
	//
	MVector zAxis( 0.0, 0.0, 1.0 );
	MVector N = U ^ zAxis;	// Cross product
	N.normalize();


	// P is the handle position vector
	//
	MVector P = handle_position - effector_position;


	// Determine the rotation direction
	//
	double PdotN = P[0]*N[0] + P[1]*N[1];
	if ( PdotN < 0 ) {
		CCW;
	} else {
		CW;
	}


	// get and set the Joint Angles 
	//
	MDoubleArray jointAngles;
	if ( getJointAngles( jointAngles ) ) {
		jointAngles.set( jointAngles[0] + rot, 0 );
		setJointAngles( jointAngles );
	}
	return MS::kSuccess;
}


/////////////////////////////////////////////////////////
//
// Register the solver
//
/////////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
	MStatus			status;
	MFnPlugin	plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode("simpleSolverNode", 
								 simpleSolverNode::id,
								 &simpleSolverNode::creator,
								 &simpleSolverNode::initialize,
								 MPxNode::kIkSolverNode);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MStatus   		status;
	MFnPlugin	plugin( obj );

	status = plugin.deregisterNode(simpleSolverNode::id);
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
