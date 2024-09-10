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
// Example custom transform:
//	This plug-in implements an example custom transform that
//	can be used to perform a rocking motion around the X axis.
//	The checkAndSetRotation() method is implemented in order to apply 
//	locks and rotation limits to user input. 
//	Geometry of any rotation can be made a child of this transform
//	to demonstrate the effect.
//	The plug-in contains two pieces:
//	1. The custom transform node -- rockingTransformCheckNode
//	2. The custom transformation matrix -- rockingTransformCheckMatrix
//	These classes are used together in order to implement the
//	rocking motion.  Note that the rock attribute is stored outside
//	of the regular transform attributes.
//
// MEL usage:
/*
	// Create a rocking transform and make a rotated plane
	// its child.
	loadPlugin rockingTransformCheck;
	file -f -new;
	polyPlane -w 1 -h 1 -sx 10 -sy 10 -ax 0 1 0 -cuv 2 -ch 1;
	select -r pPlane1 ;
	rotate -r -ws -15 -15 -15 ;
	createNode rockingTransformCheck;
	parent pPlane1 rockingTransformCheck1;
	setAttr rockingTransformCheck1.rockx 10;
	//This rotation will be clamped if
	//CHECK_ROTATION_LIMITS_USING_ATTRIBUTES is off
	select -r rockingTransformCheck1 ;
	rotate -r -ws 80 0 0 ;
*/
// NOTE: Classes used in this example derive from classes defined in the rockingTransform example. 
//

#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MIOStream.h>

#include "rockingTransformCheck.h"

//
// Initialize our static class variables
//
MTypeId rockingTransformCheckNode::idCheck(kRockingTransformCheckNodeID);
MTypeId rockingTransformCheckMatrix::idCheck(kRockingTransformCheckMatrixID);

//
// Implementation of our custom transformation matrix
//

//
//	Constructor for matrix
//
rockingTransformCheckMatrix::rockingTransformCheckMatrix()
{
}

//
// Creator for matrix
//
MPxTransformationMatrix *rockingTransformCheckMatrix::creator()
{
	return new rockingTransformCheckMatrix();
}


//
// Implementation of our custom transform
//

//
//	Constructor of the transform node
//
rockingTransformCheckNode::rockingTransformCheckNode()
:	ParentClass()
{
}

//
//	Destructor of the rocking transform
//
rockingTransformCheckNode::~rockingTransformCheckNode()
{
}

//
//	Method that returns the new transformation matrix
//
MPxTransformationMatrix *rockingTransformCheckNode::createTransformationMatrix()
{
	return new rockingTransformCheckMatrix();
}

//
//	Method that returns a new transform node
//
void *rockingTransformCheckNode::creator()
{
	return new rockingTransformCheckNode();
}

//
//	Debugging method
//
const char* rockingTransformCheckNode::className() 
{
	return "rockingTransformCheckNode";
}

//
//	This method allows the custom transform to apply its own locking
//	mechanism to rotation. Standard dependency graph attribute locking
//	happens automatically and cannot be modified by custom nodes.
//	If the plug should not be changed, then the value from the passed savedR
//	argument should be returned to be used in the transformation matrix.
//
MEulerRotation rockingTransformCheckNode::applyRotationLocks(const MEulerRotation &toTest,
									const MEulerRotation &savedRotation,
									MStatus *ReturnStatus )
{
	//
	// Implement a simple lock by checking for an existing attribute
	// Use the following MEL to add the attribute:
	//	addAttr -ln "rotateLockPlug"
	//
	MStatus status;
	MObject object = thisMObject();
	MFnDependencyNode depNode( object );
	MObject rotateLockPlug = depNode.findPlug( "rotateLockPlug",  true,  &status );

	// If the lock does not exist that we return the updated value that has
	// been passed in
	if ( rotateLockPlug.isNull() )
		return toTest;
	
	// We have a lock.  Returned the original saved value of the
	// rotation.
	return savedRotation;
}

MEulerRotation rockingTransformCheckNode::applyRotationLimits(const MEulerRotation &unlimitedRotation,
									  MDataBlock & /*block*/,
									  MStatus *ReturnStatus )
{
#if CHECK_ROTATION_LIMITS_USING_ATTRIBUTES
	// A more complete plug-in would take this approach

	MEulerRotation newRotation = unlimitedRotation;

	updateMatrixAttrs(minRotLimitEnable);
	updateMatrixAttrs(maxRotLimitEnable);

	double3 &minLimit = block.inputValue(minRotLimit).asDouble3();
	double3 &maxLimit = block.inputValue(maxRotLimit).asDouble3();

	unsigned ii = 0, jj = 0;
	for (jj = MFnTransform::kRotateMinX, ii = 0; ii < 3; ++ii, ++jj) {
		if (isLimited((MFnTransform::LimitType)jj) && 
			newRotation[ii] < minLimit[ii]) {
			newRotation[ii] = minLimit[ii];
		}

		if (isLimited((MFnTransform::LimitType)(++jj)) &&
			newRotation[ii] > maxLimit[ii]) {
			newRotation[ii] = maxLimit[ii];
		}
	}

	if ( ReturnStatus )
		*ReturnStatus = MS::kSuccess;

	return newRotation;
#else
	//
	// For demonstration purposes we limit the rotation to 60
	// degrees and bypass the rotation limit attributes
	// 
	DegreeRadianConverter conv;
	double degrees = conv.radiansToDegrees( unlimitedRotation.x );
	if ( degrees < 60 )
		return unlimitedRotation;
	MEulerRotation euler;
	euler.x = conv.degreesToRadians( 60.0 );
	if ( ReturnStatus )
		*ReturnStatus = MS::kSuccess;
	return euler;
#endif
}

//
//	Method for returning the current rocking transformation matrix
//
rockingTransformCheckMatrix *rockingTransformCheckNode::getRockingTransformCheckMatrix()
{
	rockingTransformCheckMatrix *ltm = (rockingTransformCheckMatrix *) transformationMatrixPtr();
	return ltm;
}

