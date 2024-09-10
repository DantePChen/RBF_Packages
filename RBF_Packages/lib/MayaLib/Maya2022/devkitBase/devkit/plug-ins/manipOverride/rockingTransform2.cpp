//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MIOStream.h>

#include "rockingTransform2.h"

#ifndef _USE_MATH_DEFINES
	#define _USE_MATH_DEFINES
#endif

#include <math.h>

//
// Initialize our static class variables
//
MObject rockingTransform2Node::aRockInX;
MTypeId rockingTransform2Node::id(kRockingTransform2NodeID);
MTypeId rockingTransform2Matrix::id(kRockingTransform2MatrixID);

//
// Implementation of our custom transformation matrix
//

//
//	Constructor for matrix
//
rockingTransform2Matrix::rockingTransform2Matrix()
{
	rockXValue = 0.0;
}

//
// Creator for matrix
//
MPxTransformationMatrix *rockingTransform2Matrix::creator()
{
	return new rockingTransform2Matrix();
}

//
//	Utility method for getting the rock
//	motion in the X axis
//
double rockingTransform2Matrix::getRockInX() const
{
	return rockXValue;
}

//
//	Utility method for setting the rcok 
//	motion in the X axis
//
void rockingTransform2Matrix::setRockInX( double rock )
{
	rockXValue = rock;
}

//
// This method will be used to return information to
// Maya.  Use the attributes which are outside of
// the regular transform attributes to build a new
// matrix.  This new matrix will be passed back to
// Maya.
//
MMatrix rockingTransform2Matrix::asMatrix() const
{
	// Get the current transform matrix
	MMatrix m = ParentClass::asMatrix();
	// Initialize the new matrix we will calculate
	MTransformationMatrix tm( m );
	// Find the current rotation as a quaternion
	MQuaternion quat = rotation();
	// Convert the rocking value in degrees to radians
	DegreeRadianConverter conv;
	double newTheta = conv.degreesToRadians( getRockInX() );
	quat.setToXAxis( newTheta );
	// Apply the rocking rotation to the existing rotation
	tm.addRotationQuaternion( quat.x, quat.y, quat.z, quat.w, MSpace::kTransform );
	// Let Maya know what the matrix should be
	return tm.asMatrix();
}

MMatrix rockingTransform2Matrix::asMatrix(double percent) const
{
	MPxTransformationMatrix m(*this);

	//	Apply the percentage to the matrix components
	MVector trans = m.translation();
	trans *= percent;
	m.translateTo( trans );
	MPoint rotatePivotTrans = m.rotatePivot();
	rotatePivotTrans = rotatePivotTrans * percent;
	m.setRotatePivot( rotatePivotTrans );
	MPoint scalePivotTrans = m.scalePivotTranslation();
	scalePivotTrans = scalePivotTrans * percent;
	m.setScalePivotTranslation( scalePivotTrans );

	//	Apply the percentage to the rotate value.  Same
	// as above + the percentage gets applied
	MQuaternion quat = rotation();
	DegreeRadianConverter conv;
	double newTheta = conv.degreesToRadians( getRockInX() );
	quat.setToXAxis( newTheta );
	m.rotateBy( quat );
	MEulerRotation eulRotate = m.eulerRotation();
	m.rotateTo(  eulRotate * percent, MSpace::kTransform);

	//	Apply the percentage to the scale
	MVector s(scale(MSpace::kTransform));
	s.x = 1.0 + (s.x - 1.0)*percent;
	s.y = 1.0 + (s.y - 1.0)*percent;
	s.z = 1.0 + (s.z - 1.0)*percent;
	m.scaleTo(s, MSpace::kTransform);

	return m.asMatrix();
}

MMatrix	rockingTransform2Matrix::asRotateMatrix() const
{
	// To be implemented
	return ParentClass::asRotateMatrix();
}


//
// Implementation of our custom transform
//

//
//	Constructor of the transform node
//
rockingTransform2Node::rockingTransform2Node()
: ParentClass()
{
	rockXValue = 0.0;
}

//
//	Post constructor method.  Have access to *this.  Node setup
//	operations that do not go into the initialize() method should go
//	here.
//
void rockingTransform2Node::postConstructor()
{
	//	Make sure the parent takes care of anything it needs.
	//
	ParentClass::postConstructor();

	MPlug aRockInXPlug(thisMObject(), aRockInX);
}

//
//	Destructor of the rocking transform
//
rockingTransform2Node::~rockingTransform2Node()
{
}

//
//	Method that returns the new transformation matrix
//
MPxTransformationMatrix *rockingTransform2Node::createTransformationMatrix()
{
	return new rockingTransform2Matrix();
}

//
//	Method that returns a new transform node
//
void *rockingTransform2Node::creator()
{
	return new rockingTransform2Node();
}

//
//	Node initialize method.  We configure node
//	attributes here.  Static method so
//	*this is not available.
//
MStatus rockingTransform2Node::initialize()
{
	MFnNumericAttribute numFn;
	aRockInX = numFn.create("RockInX", "rockx", MFnNumericData::kDouble, 0.0);
	numFn.setKeyable(true);
	numFn.setAffectsWorldSpace(true);
	addAttribute(aRockInX);

	//	This is required so that the validateAndSet method is called
	mustCallValidateAndSet(aRockInX);
	return MS::kSuccess;
}

//
//	Debugging method
//
const char* rockingTransform2Node::className() 
{
	return "rockingTransform2Node";
}

//
//	Reset transformation
//
void  rockingTransform2Node::resetTransformation (const MMatrix &matrix)
{
	ParentClass::resetTransformation( matrix );
}

//
//	Reset transformation
//
void  rockingTransform2Node::resetTransformation (MPxTransformationMatrix *resetMatrix )
{
	ParentClass::resetTransformation( resetMatrix );
}

//
// A very simple implementation of validAndSetValue().  No lock
// or limit checking on the rocking attribute is done in this method.
// If you wish to apply locks and limits to the rocking attribute, you
// would follow the approach taken in the rockingTransformCheck example.
// Meaning you would implement methods similar to:
//	* applyRotationLocks();
//	* applyRotationLimits();
//	* checkAndSetRotation();  
// but for the rocking attribute.  The method checkAndSetRotation()
// would be called below rather than updating the rocking attribute
// directly.
//
MStatus rockingTransform2Node::validateAndSetValue(const MPlug& plug, const MDataHandle& handle)
{
	MStatus status = MS::kSuccess;

	//	Make sure that there is something interesting to process.
	//
	if (plug.isNull())
		return MS::kFailure;

	MDataBlock block = forceCache();
	MDataHandle blockHandle = block.outputValue(plug, &status);
	ReturnOnError(status);
	
	if ( plug == aRockInX )
	{
		// Update our new rock in x value
		double rockInX = handle.asDouble();
		blockHandle.set(rockInX);
		rockXValue = rockInX;
		
		// Update the custom transformation matrix to the
		// right rock value.  
		rockingTransform2Matrix *ltm = getRockingTransformMatrix();
		if (ltm)
			ltm->setRockInX(rockXValue);
			
		blockHandle.setClean();
		
		// Mark the matrix as dirty so that DG information
		// will update.
		dirtyMatrix();		
	}
	
	// Allow processing for other attributes
	return ParentClass::validateAndSetValue(plug, handle);
}

//
//	Method for returning the current rocking transformation matrix
//
rockingTransform2Matrix *rockingTransform2Node::getRockingTransformMatrix()
{
	rockingTransform2Matrix *ltm = (rockingTransform2Matrix *) transformationMatrixPtr();
	return ltm;
}

//
// Utility class
//
double DegreeRadianConverter::degreesToRadians( double degrees )
{
	 return degrees * ( M_PI/ 180.0 );
}
double DegreeRadianConverter::radiansToDegrees( double radians )
{
	return radians * (180.0/M_PI);
}	

