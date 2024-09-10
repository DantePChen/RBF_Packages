//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
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
//	Geometry of any rotation can be made a child of this transform
//	to demonstrate the effect.
//	The plug-in contains two pieces:
//	1. The custom transform node -- rockingTransformNode
//	2. The custom transformation matrix -- rockingTransformMatrix
//	These classes are used together in order to implement the
//	rocking motion.  Note that the rock attribute is stored outside
//	of the regular transform attributes.
//
// MEL usage:
/*
	// Create a rocking transform and make a rotated plane
	// its child.
	loadPlugin rockingTransform;
	file -f -new;
	polyPlane -w 1 -h 1 -sx 10 -sy 10 -ax 0 1 0 -cuv 2 -ch 1;
	select -r pPlane1 ;
	rotate -r -ws -15 -15 -15 ;
	createNode rockingTransform;
	parent pPlane1 rockingTransform1;
	setAttr rockingTransform1.rockx 55;
*/
//

#include <maya/MPxTransform.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MIOStream.h>
#include <assert.h>

#include "rockingTransform.h"

#ifndef M_PI
#include <math.h>
#endif

//
// Initialize our static class variables
//
MObject rockingTransformNode::aRockInX;
MTypeId rockingTransformNode::id(kRockingTransformNodeID);
MTypeId rockingTransformMatrix::id(kRockingTransformMatrixID);

//
// Implementation of our custom transformation matrix
//

//
//	Constructor for matrix
//
rockingTransformMatrix::rockingTransformMatrix()
{
	rockXValue = 0.0;
}

//
// Creator for matrix
//
MPxTransformationMatrix *rockingTransformMatrix::creator()
{
	return new rockingTransformMatrix();
}

//
//	Utility method for getting the rock
//	motion in the X axis
//
double rockingTransformMatrix::getRockInX() const
{
	return rockXValue;
}

//
//	Utility method for setting the rock
//	motion in the X axis
//
void rockingTransformMatrix::setRockInX( double rock )
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
MMatrix rockingTransformMatrix::asMatrix() const
{
	return ParentClass::asMatrix();
}

MMatrix rockingTransformMatrix::asMatrix(double percent) const
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

MMatrix	rockingTransformMatrix::asRotateMatrix() const
{
	MMatrix Ro = rotateOrientationValue.asMatrix();
	MMatrix R  = rotationValue.asMatrix();
	MMatrix Rr = preRotation().asMatrix();

	MMatrix Rt;
	Rt[3][0] = rotatePivotTranslationValue.x;
	Rt[3][1] = rotatePivotTranslationValue.y;
	Rt[3][2] = rotatePivotTranslationValue.z;

	MMatrix Rp;
	Rp[3][0] = rotatePivotValue.x;
	Rp[3][1] = rotatePivotValue.y;
	Rp[3][2] = rotatePivotValue.z;

	MMatrix RpInv;
	RpInv[3][0] = -rotatePivotValue.x;
	RpInv[3][1] = -rotatePivotValue.y;
	RpInv[3][2] = -rotatePivotValue.z;

	return (RpInv * Ro * R * Rr * Rp * Rt);
}

MStatus rockingTransformMatrix::setRotatePivot(const MPoint &rotatePivot, MSpace::Space space, bool balance)
{
	MPoint newPivot(rotatePivot);
	if (space != MSpace::kTransform) {
		if (space == MSpace::kPostTransform) {
			newPivot *= asMatrixInverse();
		}
		newPivot *= asScaleMatrix();
	}

	if (balance) {
		MMatrix Ro = rotateOrientationValue.asMatrix();
		MMatrix R = rotationValue.asMatrix();
		MMatrix Rr = preRotation().asMatrix();

		MMatrix Rp;
		Rp[3][0] = newPivot.x;
		Rp[3][1] = newPivot.y;
		Rp[3][2] = newPivot.z;

		MMatrix RpInv;
		RpInv[3][0] = -newPivot.x;
		RpInv[3][1] = -newPivot.y;
		RpInv[3][2] = -newPivot.z;

		MMatrix leftMat = RpInv * Ro * R * Rr * Rp;

		MMatrix mat = leftMat.inverse() * asRotateMatrix();

		rotatePivotTranslationValue[0] = mat[3][0];
		rotatePivotTranslationValue[1] = mat[3][1];
		rotatePivotTranslationValue[2] = mat[3][2];
	}

	rotatePivotValue = newPivot;
	return MS::kSuccess;
}

//
// This method returns the local rotation used by rotate manipulator
//
MQuaternion rockingTransformMatrix::preRotation() const
{
	DegreeRadianConverter conv;
	double newTheta = conv.degreesToRadians(getRockInX());
	MQuaternion quat; quat.setToXAxis(newTheta);
	return quat;
}

//
// Implementation of our custom transform
//

//
//	Constructor of the transform node
//
rockingTransformNode::rockingTransformNode()
: ParentClass()
{
}

//
//	Post constructor method.  Have access to *this.  Node setup
//	operations that do not go into the initialize() method should go
//	here.
//
void rockingTransformNode::postConstructor()
{
	//	Make sure the parent takes care of anything it needs.
	//
	ParentClass::postConstructor();

	MPlug aRockInXPlug(thisMObject(), aRockInX);
}

//
//  This method computes the transformation matrix for a passed data block
//  and places the output into a passed transformation matrix.
//
MStatus
rockingTransformNode::computeLocalTransformation(MPxTransformationMatrix *xform,
                                                 MDataBlock &block)
{
    //  Get the value from the aRockInX attribute
    MStatus status = MS::kSuccess;
    MDataHandle rockInXHandle = block.inputValue (aRockInX, &status);
    ReturnOnError(status);
    
    // Store it in the transformation matrix so that when asked for it through
    // asMatrix() it can construct the desired matrix
    rockingTransformMatrix* ltm = dynamic_cast<rockingTransformMatrix*>(xform);
    assert(ltm);
    ltm->setRockInX(rockInXHandle.asDouble());

    return ParentClass::computeLocalTransformation(xform, block);
}

//
//  The transform's compute method
//
MStatus rockingTransformNode::compute( const MPlug& plug, MDataBlock& block )
{ 
	if( (plug.attribute() == MPxTransform::matrix)
	||	(plug.attribute() == MPxTransform::inverseMatrix)
	||	(plug.attribute() == MPxTransform::worldMatrix)
	||	(plug.attribute() == MPxTransform::worldInverseMatrix)
	||	(plug.attribute() == MPxTransform::parentMatrix)
	||	(plug.attribute() == MPxTransform::parentInverseMatrix) )
	{
        rockingTransformMatrix *ltm = getRockingTransformMatrix();
        if (ltm) {
            computeLocalTransformation(ltm, block);
        } else {
            MGlobal::displayError("Failed to get rock transform matrix");
        }
	}

	return MPxTransform::compute(plug, block);
}
//
//	Destructor of the rocking transform
//
rockingTransformNode::~rockingTransformNode()
{
}

//
//	Method that returns the new transformation matrix
//
MPxTransformationMatrix *rockingTransformNode::createTransformationMatrix()
{
	return new rockingTransformMatrix();
}

//
//	Method that returns a new transform node
//
void *rockingTransformNode::creator()
{
	return new rockingTransformNode();
}

//
//	Node initialize method.  We configure node
//	attributes here.  Static method so
//	*this is not available.
//
MStatus rockingTransformNode::initialize()
{
	MFnNumericAttribute numFn;
	aRockInX = numFn.create("RockInX", "rockx", MFnNumericData::kDouble, 0.0);
	numFn.setKeyable(true);
	numFn.setAffectsWorldSpace(true);
	addAttribute(aRockInX);

	// Need to update matrix when attribute aRockInX is modified
	attributeAffects(aRockInX, matrix);

	//	This is required so that the validateAndSet method is called
	mustCallValidateAndSet(aRockInX);
	return MS::kSuccess;
}

//
//	Debugging method
//
const char* rockingTransformNode::className()
{
	return "rockingTransformNode";
}

//
//	Reset transformation
//
void  rockingTransformNode::resetTransformation (const MMatrix &matrix)
{
	ParentClass::resetTransformation( matrix );
}

//
//	Reset transformation
//
void  rockingTransformNode::resetTransformation (MPxTransformationMatrix *resetMatrix )
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
MStatus rockingTransformNode::validateAndSetValue(const MPlug& plug, const MDataHandle& handle)
{
	//	Make sure that there is something interesting to process.
	//
	if (plug.isNull())
		return MS::kFailure;

	if ( plug == aRockInX )
	{
		MStatus status = MS::kSuccess;

		MDataBlock block = forceCache();
		MDataHandle blockHandle = block.outputValue(plug, &status);
		ReturnOnError(status);

		// Update our new rock in x value
		double rockInX = handle.asDouble();
		blockHandle.set(rockInX);

		// Update the custom transformation matrix to the
		// right rock value.
		rockingTransformMatrix *ltm = getRockingTransformMatrix();
		if (ltm)
			ltm->setRockInX(rockInX);
		else
			MGlobal::displayError("Failed to get rock transform matrix");

		blockHandle.setClean();

		// Mark the matrix as dirty so that DG information
		// will update.
		dirtyMatrix();

		return status;
	}

	// Allow processing for other attributes
	return ParentClass::validateAndSetValue(plug, handle);
}

//
//	Method for returning the current rocking transformation matrix
//
rockingTransformMatrix *rockingTransformNode::getRockingTransformMatrix()
{
	rockingTransformMatrix *ltm = (rockingTransformMatrix *) transformationMatrixPtr();
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

