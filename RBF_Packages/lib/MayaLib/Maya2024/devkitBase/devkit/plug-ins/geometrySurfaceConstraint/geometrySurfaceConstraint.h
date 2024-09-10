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

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxTransform.h>
#include <maya/MPxConstraint.h> 
#include <maya/MPxConstraintCommand.h> 

#include <maya/MArgDatabase.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MGlobal.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnTransform.h>
#include <maya/MVector.h>
#include <maya/MTypes.h>
#include <maya/MFnNumericData.h>
#include <maya/MDGModifier.h>

#include <math.h>
#include <float.h>

//
//	Macros
//


#define kConstrainToLargestWeightFlag "-lw"
#define kConstrainToLargestWeightFlagLong "-largestWeight"
#define kConstrainToSmallestWeightFlag "-sw"
#define kConstrainToSmallestWeightFlagLong "-smallestWeight"

//
// Class definitions
//

class geometrySurfaceConstraintCommand : public MPxConstraintCommand
{
public:

	enum ConstraintType
	{
		kLargestWeight,
		kSmallestWeight,
	};

	geometrySurfaceConstraintCommand();
	~geometrySurfaceConstraintCommand() override;

	MStatus		doIt(const MArgList &argList) override;
	MStatus		appendSyntax() override;

	MTypeId constraintTypeId() const override;
	MPxConstraintCommand::TargetType targetType() const override;

	const MObject& constraintInstancedAttribute() const override;
	const MObject& constraintOutputAttribute() const override;
	const MObject& constraintTargetInstancedAttribute() const override;
	const MObject& constraintTargetAttribute() const override;
	const MObject& constraintTargetWeightAttribute() const override;
	const MObject& objectAttribute() const override;

	MStatus connectTarget(MDagPath& opaqueTarget, int index) override;
	MStatus connectObjectAndConstraint( MDGModifier& modifier ) override;

	void createdConstraint(MPxConstraint *constraint) override;

	static  void* creator();

protected:
		MStatus			parseArgs(const MArgList &argList) override;

	geometrySurfaceConstraintCommand::ConstraintType weightType;
};

class geometrySurfaceConstraint : public MPxConstraint
{
public:
						geometrySurfaceConstraint();
					~geometrySurfaceConstraint() override; 

	void		postConstructor() override;
	MStatus		compute( const MPlug& plug, MDataBlock& data ) override;

	const MObject weightAttribute() const override;
    const MObject targetAttribute() const override;
	void getOutputAttributes(MObjectArray& attributeArray) override;

	static  void*		creator();
	static  MStatus		initialize();

public:
	static MObject		compoundTarget;
	static MObject		targetGeometry;
	static MObject		targetWeight;

	static MObject		constraintParentInverseMatrix;
	static MObject		constraintGeometry;

	static	MTypeId		id;

	geometrySurfaceConstraintCommand::ConstraintType weightType;
};

// Useful inline method
inline bool
equivalent(double a, double b  )
{
	return fabs( a - b ) < .000001 ;
}

