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
// Produces the dependency graph node "buildRotation". 
//
// This example demonstrates performing a linear algebra calculation based on inputs and outputting the result.
//
// The node takes an up vector and a forward vector as inputs.
// It outputs the rotation that takes the y-axis and z-axis and rotates them to be the up and forward vectors respectively.
// 
// A sample use of this node is to align an object to another surface based on a normal and a tangent vector.
// This example uses the "pointOnSurfaceNode", which is a standard node provided with Maya.
//
// sphere -radius 4;
// cone -ax 0 1 0;
// createNode pointOnSurfaceInfo;
// connectAttr nurbsSphereShape1.worldSpace pointOnSurfaceInfo1.inputSurface;
// connectAttr pointOnSurfaceInfo1.position nurbsCone1.translate;
// setAttr pointOnSurfaceInfo1.u 0.01;
// setAttr pointOnSurfaceInfo1.v 0.01;
//
// At this point, the cone is constrained to a point on the surface of the sphere.
// As the "u" and "v" attributes of the "pointOnSurfaceNode" are changed, the cone will move around the surface.
// The next step is to align the cone so that the tip points in the direction of the normal.
// The "buildRotation" node is used to do this.
//
// createNode buildRotation;
// connectAttr pointOnSurfaceInfo1.normal buildRotation1.up;
// connectAttr pointOnSurfaceInfo1.tangentU buildRotation1.forward;
// connectAttr buildRotation1.rotate nurbsCone1.rotate;
//
// Now, the cone will be constrained to the surface of the sphere and will also be aligned with the normal of the sphere at the point of constraint. 
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h> 

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#define EPSILON				0.001
#define ROTATE_ORDER_XYZ	0
#define ROTATE_ORDER_YZX	1
#define ROTATE_ORDER_ZXY	2
#define ROTATE_ORDER_XZY	3
#define ROTATE_ORDER_YXZ	4
#define ROTATE_ORDER_ZYX	5

#define McheckErr(stat,msg)         \
    if ( MS::kSuccess != stat ) {   \
		cerr << msg;                \
		return MS::kFailure;        \
	}
 
class buildRotation : public MPxNode
{
public:
						buildRotation();
					~buildRotation() override; 

	MStatus		compute( const MPlug& plug, MDataBlock& data ) override;

	static  void*		creator();
	static  MStatus		initialize();

public:
	
	// Inputs
	static  MObject     up;           // Up vector
	static  MObject         upX;
	static  MObject         upY;
	static  MObject         upZ;
    static  MObject     forward;      // Front Vector
    static  MObject         forwardX;
    static  MObject         forwardY;
    static  MObject         forwardZ;
	static  MObject     rotateOrder;  // Desired rotation order

	// Outputs
	static  MObject     rotate;       // Rotation as a 3 Double
	static  MObject         rotateX;
	static  MObject         rotateY;
	static  MObject         rotateZ;
	static  MObject     rotateMatrix; // Rotation as a matrix

	static	MTypeId		id;           
};

MTypeId     buildRotation::id( 0x80004 );
MObject     buildRotation::up; 
MObject     buildRotation::upX;  
MObject     buildRotation::upY;    
MObject     buildRotation::upZ;     
MObject     buildRotation::forward;  
MObject     buildRotation::forwardX;  
MObject     buildRotation::forwardY;  
MObject     buildRotation::forwardZ;     
MObject     buildRotation::rotateOrder;    
MObject     buildRotation::rotate;    
MObject     buildRotation::rotateX;    
MObject     buildRotation::rotateY;   
MObject     buildRotation::rotateZ;  
MObject     buildRotation::rotateMatrix;   

buildRotation::buildRotation() {}
buildRotation::~buildRotation() {}

MStatus buildRotation::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus returnStatus;
 
	if ((plug == rotate) || (plug.parent() == rotate) || (plug == rotateMatrix)) {
		MDataHandle upData = data.inputValue( up, &returnStatus );
		McheckErr(returnStatus,"ERROR getting up vector data");

		MDataHandle forwardData = data.inputValue( forward, &returnStatus );
		McheckErr(returnStatus,"ERROR getting forward vector data");

		MVector up = upData.asVector();
		MVector forward = forwardData.asVector();

		// Make sure that the up and forward vectors are orthogonal
		//
		if ( fabs( up * forward ) > EPSILON ) {
			// Non-zero dot product
			//
			MVector orthoVec = up ^ forward;
			MVector newForward = orthoVec ^ up;
			if ( forward * newForward < 0.0 ) {
				// Reverse the vector
				//
				newForward *= -1.0;
			}
			forward = newForward;
		}

		// Calculate the rotation required to align the y-axis with the up
		// vector
		//
		MTransformationMatrix firstRot;
		MVector rotAxis = MVector::yAxis ^ up;
		rotAxis.normalize();
		firstRot.setToRotationAxis( rotAxis, MVector::yAxis.angle( up ) );
		
		// Calculate the second rotation required to align the forward vector
		//
		MTransformationMatrix secondRot;
		MVector transformedForward = firstRot.asMatrix() * forward;
		transformedForward.normalize();
		double angle = transformedForward.angle( MVector::zAxis );
		if ( transformedForward.x < 0.0 ) {
			// Compensate for the fact that the angle method returns
			// the absolute value
			//
			angle *= -1.0;
		}
		secondRot.setToRotationAxis( up, angle );

		// Get the requested rotation order
		//
		MDataHandle orderHandle = data.inputValue( rotateOrder );
		short order = orderHandle.asShort();
		MTransformationMatrix::RotationOrder rotOrder;
		switch ( order ) {
			case ROTATE_ORDER_XYZ:
				rotOrder = MTransformationMatrix::kXYZ; break;
			case ROTATE_ORDER_YZX:
				rotOrder = MTransformationMatrix::kYZX; break;
			case ROTATE_ORDER_ZXY:
				rotOrder = MTransformationMatrix::kZXY; break;
			case ROTATE_ORDER_XZY:
				rotOrder = MTransformationMatrix::kXZY; break;
			case ROTATE_ORDER_YXZ:
				rotOrder = MTransformationMatrix::kYXZ; break;
			case ROTATE_ORDER_ZYX:
				rotOrder = MTransformationMatrix::kZYX; break;
			default:
				rotOrder = MTransformationMatrix::kInvalid; break;
		}

		MTransformationMatrix result = firstRot.asMatrix() * secondRot.asMatrix();
		result.reorderRotation( rotOrder );

		double rotation[3];
		result.getRotation( rotation, rotOrder);
		
		MDataHandle outputRot = data.outputValue( rotate );
		outputRot.set( rotation[0], rotation[1], rotation[2] );
		outputRot.setClean();

		MDataHandle outputMatrix = data.outputValue( rotateMatrix );
		outputMatrix.set( result.asMatrix() );
		outputMatrix.setClean();
	} else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}

void* buildRotation::creator()
{
	return new buildRotation();
}

MStatus buildRotation::initialize()
{
	MFnNumericAttribute nAttr;
	MFnMatrixAttribute  mAttr;
	MFnUnitAttribute    uAttr;
	MFnEnumAttribute    eAttr;
	MStatus				stat;

	// Set up inputs
	//
    upX = nAttr.create( "upX", "ux", MFnNumericData::kDouble );
 	    nAttr.setStorable(false);
    upY = nAttr.create( "upY", "uy", MFnNumericData::kDouble );
 	    nAttr.setStorable(false);
    upZ = nAttr.create( "upZ", "uz", MFnNumericData::kDouble );
 	    nAttr.setStorable(false);
	up = nAttr.create( "up", "u", upX, upY, upZ );
	    nAttr.setDefault( 0.0, 1.0, 0.0 );
 	    nAttr.setStorable(false);

    forwardX = nAttr.create( "forwardX", "fx", MFnNumericData::kDouble, 0.0 );
 	    nAttr.setStorable(false);
    forwardY = nAttr.create( "forwardY", "fy", MFnNumericData::kDouble, 0.0 );
 	    nAttr.setStorable(false);
    forwardZ = nAttr.create( "forwardZ", "fz", MFnNumericData::kDouble, 1.0 );
 	    nAttr.setStorable(false);
	forward = nAttr.create( "forward", "f", forwardX, forwardY, forwardZ );
	    nAttr.setDefault( 0.0, 0.0, 1.0 );
 	    nAttr.setStorable(false);

	rotateOrder = eAttr.create( "rotateOrder", "ro", ROTATE_ORDER_XYZ);
		eAttr.addField("xyz", ROTATE_ORDER_XYZ);
		eAttr.addField("yzx", ROTATE_ORDER_YZX);
		eAttr.addField("zxy", ROTATE_ORDER_ZXY);
		eAttr.addField("xzy", ROTATE_ORDER_XZY);
		eAttr.addField("yxz", ROTATE_ORDER_YXZ);
		eAttr.addField("zyx", ROTATE_ORDER_ZYX);
 	    eAttr.setStorable(false);

	// Set up outputs
	// 
    rotateX = uAttr.create( "rotateX", "rx", MFnUnitAttribute::kAngle, 0.0 );
 	    nAttr.setStorable(false);
    rotateY = uAttr.create( "rotateY", "ry", MFnUnitAttribute::kAngle, 0.0 );
 	    nAttr.setStorable(false);
    rotateZ = uAttr.create( "rotateZ", "rz", MFnUnitAttribute::kAngle, 0.0 );
 	    nAttr.setStorable(false);
	rotate = nAttr.create( "rotate", "r", rotateX, rotateY, rotateZ );
 	    nAttr.setStorable(false);
		
	rotateMatrix = mAttr.create( "rotateMatrix", "rm" );
	    nAttr.setStorable(false);
		nAttr.setConnectable(true);

	stat = addAttribute( up );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( forward );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( rotate );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( rotateOrder );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( rotateMatrix );
		if (!stat) { stat.perror("addAttribute"); return stat;}

	stat = attributeAffects( up, rotate );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( up, rotateMatrix );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( forward, rotate );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( forward, rotateMatrix );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( rotateOrder, rotate );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( rotateOrder, rotateMatrix );
		if (!stat) { stat.perror("attributeAffects"); return stat;}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "buildRotation", buildRotation::id, 
						 buildRotation::creator, buildRotation::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus	  status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( buildRotation::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
