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

#include <maya/MPxNode.h> 

#include <maya/MGlobal.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnIkJoint.h>
#include <maya/MFnPlugin.h>

#include <maya/MAngle.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h> 
#include <maya/MTransformationMatrix.h> 
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MQuaternion.h>
#include <maya/MVector.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
 
#define ROTATE_ORDER_XYZ	0
#define ROTATE_ORDER_YZX	1
#define ROTATE_ORDER_ZXY	2
#define ROTATE_ORDER_XZY	3
#define ROTATE_ORDER_YXZ	4
#define ROTATE_ORDER_ZYX	5

namespace {
	static const MString JOINT_ORIENT_TYPE_XYZ = "xyz";
	static const MString JOINT_ORIENT_TYPE_YZX = "yzx";
	static const MString JOINT_ORIENT_TYPE_ZXY = "zxy";
	static const MString JOINT_ORIENT_TYPE_XZY = "xzy";
	static const MString JOINT_ORIENT_TYPE_YXZ = "yxz";
	static const MString JOINT_ORIENT_TYPE_ZYX = "zyx";
}

/////////////////////////////////////
// Plugin RebalanceTransform Class //
/////////////////////////////////////

// INTRODUCTION:
//	This class will create a "rebalanceTransform" node. This node is used for
//	demonstrating how to rebalance a world transform, in order to retain the
//	values for various joint attributes.
//
// WHAT THIS PLUG-IN DEMONSTRATES:
//	This plug-in creates a node called "rebalanceTransform". It accepts as
//	inputs:
//		inputWorldMatrix			- world matrix to apply to target joint
//		inputParentInverseMatrix	- parentInverseMatrix of target joint
//		inputJointOrient			- jointOrient of target joint
//		inputJointOrientType		- jointOrientType of target joint
//		inputRotateOrder			- rotateOrder of target joint
//		inputRotateAxis				- rotateAxis of target joint
//		inputSegmentScaleCompensate	- segmentScaleCompensate of target joint
//		inputInverseScale			- inverseScale of target joint
//		
//
// HOW TO USE THIS PLUG-IN:
//	(1) Compile the plug-in
//	(2) Load the compiled plug-in into Maya via the plug-in manager
//	(3) Load the sample scene rebalanceTransform.ma
//  (4) Run the following command to apply the xform command to the joint3
//  node, using the same worldMatrix that is driving the rebalanceTransform
//  node:
//		string $m = floatArrayToString( `getAttr composeMatrix1.outputMatrix`, " ");
//		string $cmd = ("xform -ws -m " + $m + "\"joint3\"");
//		eval( $cmd );
//
//	See also testRebalanceTransform.mel
//
class rebalanceTransform : public MPxNode
{
public:
	rebalanceTransform();
	~rebalanceTransform() override; 

	MStatus		compute( const MPlug& plug, MDataBlock& data ) override;

	static  void*		creator();
	static  MStatus		initialize();

	static	MTypeId		id;				// The IFF type id

	// Inputs
	//
	static	MObject		inputWorldMatrix;
	static	MObject		inputParentInverseMatrix;
	static  MObject 	inputJointOrient;
	static  MObject 	inputJointOrientType;
	static	MObject		inputRotateOrder;
	static	MObject		inputRotateAxis;
	static  MObject 	inputSegmentScaleCompensate;
	static  MObject 	inputInverseScale;
	
	// Outputs
	//
	static  MObject 	outputTranslation;
	static  MObject 	outputRotation;
	static  MObject 	outputScale;
	static  MObject 	output; // Plug of all output compound plugs

};

MObject	rebalanceTransform::inputWorldMatrix;
MObject	rebalanceTransform::inputParentInverseMatrix;
MObject rebalanceTransform::inputJointOrient;
MObject rebalanceTransform::inputJointOrientType;
MObject	rebalanceTransform::inputRotateOrder;
MObject	rebalanceTransform::inputRotateAxis;
MObject rebalanceTransform::inputSegmentScaleCompensate;
MObject rebalanceTransform::inputInverseScale;

MObject rebalanceTransform::output;
MObject rebalanceTransform::outputTranslation;
MObject rebalanceTransform::outputRotation;
MObject rebalanceTransform::outputScale;

// IFF type ID
// Each node requires a unique identifier which is used by
// MFnDependencyNode::create() to identify which node to create, and by
// the Maya file format.
//
// For local testing of nodes you can use any identifier between
// 0x00000000 and 0x0007ffff, but for any node that you plan to use for
// more permanent purposes, you should get a universally unique id from
// Autodesk Support. You will be assigned a unique range that you can manage
// on your own.
//
MTypeId rebalanceTransform::id( 0x080047 );

// This node does not need to perform any special actions on creation or
// destruction
//
rebalanceTransform::rebalanceTransform() {}
rebalanceTransform::~rebalanceTransform() {}

// The compute() method does the actual work of the node using the inputs
// of the node to generate its output.
//
// Compute takes two parameters: plug and data.
// - Plug is the the data value that needs to be recomputed
// - Data provides handles to all of the nodes attributes, only these
//   handles should be used when performing computations.
//
MStatus rebalanceTransform::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus status;

    // Get input attrs
	//

    // World matrix and parentInverseMatrix
	//
    auto worldMatrix = data.inputValue(inputWorldMatrix, &status).asMatrix();
    auto parentMatrixInverse = data.inputValue(inputParentInverseMatrix, &status).asMatrix();

    // Calculate the corresponding *local* matrix
	//
    MTransformationMatrix localMatrix = worldMatrix * parentMatrixInverse;

    // Extract local translation, rotation, and scale
	//
    MVector localTranslation = localMatrix.getTranslation(MSpace::kTransform, &status);
	MVector localScale;
	// Do I need to fetch it so I can write it out?
	auto localRotation = localMatrix.eulerRotation();

	// Get rotateOrder
	//
	MDataHandle orderHandle = data.inputValue( inputRotateOrder );
	short order = orderHandle.asShort();
	MEulerRotation::RotationOrder rotateOrder;
	switch ( order ) {
		case ROTATE_ORDER_XYZ:
			rotateOrder = MEulerRotation::kXYZ; break;
		case ROTATE_ORDER_YZX:
			rotateOrder = MEulerRotation::kYZX; break;
		case ROTATE_ORDER_ZXY:
			rotateOrder = MEulerRotation::kZXY; break;
		case ROTATE_ORDER_XZY:
			rotateOrder = MEulerRotation::kXZY; break;
		case ROTATE_ORDER_YXZ:
			rotateOrder = MEulerRotation::kYXZ; break;
		case ROTATE_ORDER_ZYX:
			rotateOrder = MEulerRotation::kZYX; break;
		default:
			rotateOrder = MEulerRotation::kXYZ; break;
	}

	// Balance transformation
	//
	// clamp away from zero a distance of at least kScaleEpsilon
	auto clamp = [](auto v)
	{
		auto clamp = [](double x)
		{
			const double kScaleEpsilon = 1.0e-12;
			return (((x < kScaleEpsilon) && (x >= 0)) ? kScaleEpsilon : (((x<0) && (x>= -kScaleEpsilon))?(-kScaleEpsilon):x) );
		};
		v[0] = clamp(v[0]);
		v[1] = clamp(v[1]);
		v[2] = clamp(v[2]);
	};

	// Get jointOrientType, and jointOrient
	//
	MString joType = data.inputValue( inputJointOrientType ).asString();
	MEulerRotation::RotationOrder jointOrientType;
	if( joType == JOINT_ORIENT_TYPE_XYZ ){
		jointOrientType = MEulerRotation::kXYZ;
	} else if( joType == JOINT_ORIENT_TYPE_YZX ){
		jointOrientType = MEulerRotation::kYZX;
	} else if( joType == JOINT_ORIENT_TYPE_ZXY ){
		jointOrientType = MEulerRotation::kZXY;
	} else if( joType == JOINT_ORIENT_TYPE_XZY ){
		jointOrientType = MEulerRotation::kXZY;
	} else if( joType == JOINT_ORIENT_TYPE_YXZ ){
		jointOrientType = MEulerRotation::kYXZ;
	} else if( joType == JOINT_ORIENT_TYPE_ZYX ){
		jointOrientType = MEulerRotation::kZYX;
	} else {
		jointOrientType = MEulerRotation::kXYZ;
	}

	// Get jointOrient, convert to MQuaternion
	//
	auto jointOrientInDegrees = data.inputValue( inputJointOrient).asDouble3();
	MAngle jX( jointOrientInDegrees[0], MAngle::kDegrees );
	MAngle jY( jointOrientInDegrees[1], MAngle::kDegrees );
	MAngle jZ( jointOrientInDegrees[2], MAngle::kDegrees );
	MEulerRotation er( jX.asRadians(), jY.asRadians(), jZ.asRadians(), jointOrientType );
	auto qJointOrient = er.asQuaternion();

	// Get rotation axis, convert to MQuaternion
	//
	auto rotateAxisInDegrees = data.inputValue( inputRotateAxis).asDouble3();
	MAngle rX( rotateAxisInDegrees[0], MAngle::kDegrees );
	MAngle rY( rotateAxisInDegrees[1], MAngle::kDegrees );
	MAngle rZ( rotateAxisInDegrees[2], MAngle::kDegrees );
	MEulerRotation eRotateAxis( rX.asRadians(), rY.asRadians(), rZ.asRadians(), rotateOrder );
	auto qRotateAxis = eRotateAxis.asQuaternion();

	// Get compensateForParentScale (aka, segmentScaleCompensate)
	//
	bool segmentScaleCompensate = data.inputValue( inputSegmentScaleCompensate ).asBool();

	// Get 1/jointScale (aka, inverseScale)
	//
	MVector jointScale = data.inputValue( inputInverseScale ).asDouble3();
	clamp(jointScale);
	MFnTransform::balanceTransformation( localTranslation, localRotation, localScale, localMatrix, qRotateAxis, qJointOrient, segmentScaleCompensate, jointScale, rotateOrder );

    if (plug == outputTranslation || plug.partialName() == "ot0" || plug.partialName() == "ot1" || plug.partialName() == "ot2" ) {
        MDataHandle outData = data.outputValue(outputTranslation, &status);
        outData.setMVector(localTranslation);
		outData.setClean();
    } else if (plug == outputRotation || plug.partialName() == "or0" || plug.partialName() == "or1" || plug.partialName() == "or2" ) {
        MDataHandle outData = data.outputValue(outputRotation, &status);
		CHECK_MSTATUS( status );
        outData.set3Double(MAngle(localRotation.x).asDegrees(), MAngle(localRotation.y).asDegrees(), MAngle(localRotation.z).asDegrees());
		outData.setClean();
    } else if (plug == outputScale || plug.partialName() == "os0" || plug.partialName() == "os1" || plug.partialName() == "os2" ) {
        MDataHandle outData = data.outputValue(outputScale, &status);
		CHECK_MSTATUS( status );
		outData.setMVector(localScale);
		outData.setClean();
    } else if (plug == output) {
        MDataHandle outDataT = data.outputValue(outputTranslation, &status);
        outDataT.setMVector(localTranslation);
		outDataT.setClean();

        MDataHandle outDataR = data.outputValue(outputRotation, &status);
        outDataR.set3Double(MAngle(localRotation.x).asDegrees(), MAngle(localRotation.y).asDegrees(), MAngle(localRotation.z).asDegrees());
		outDataR.setClean();

        MDataHandle outDataS = data.outputValue(outputScale, &status);
		outDataS.setMVector(localScale);
		outDataS.setClean();
	} else {
		return MS::kUnknownParameter;
	}
	return( MS::kSuccess );
}

// The creator() method allows Maya to instantiate instances of this node.
// It is called every time a new instance of the node is requested by
// either the createNode command or the MFnDependencyNode::create()
// method.
//
// In this case creator simply returns a new rebalanceTransform object.
//
void* rebalanceTransform::creator()
{
	return( new rebalanceTransform() );
}

// The initialize method is called only once when the node is first
// registered with Maya. In general,
//
MStatus rebalanceTransform::initialize()
{
	MStatus stat;
	MFnCompoundAttribute cAttr;
	MFnEnumAttribute eAttr;
	MFnMatrixAttribute mAttr;
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	// Inputs
	//
	inputWorldMatrix = mAttr.create("inputWorldMatrix", "iwm");
	CHECK_MSTATUS( mAttr.setStorable( false ) );

	inputParentInverseMatrix = mAttr.create("inputParentInverseMatrix", "ipi");
	CHECK_MSTATUS( mAttr.setStorable( false ) );
	
	inputJointOrient = nAttr.create("inputJointOrient", "ijo", MFnNumericData::k3Double);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setArray(false);

	inputJointOrientType = tAttr.create( "inputJointOrientType", "iot", MFnData::kString);
	CHECK_MSTATUS(tAttr.setStorable(false));

	inputRotateOrder = eAttr.create( "inputRotateOrder", "iro", ROTATE_ORDER_XYZ);
		eAttr.addField("xyz", ROTATE_ORDER_XYZ);
		eAttr.addField("yzx", ROTATE_ORDER_YZX);
		eAttr.addField("zxy", ROTATE_ORDER_ZXY);
		eAttr.addField("xzy", ROTATE_ORDER_XZY);
		eAttr.addField("yxz", ROTATE_ORDER_YXZ);
		eAttr.addField("zyx", ROTATE_ORDER_ZYX);
 	    CHECK_MSTATUS(eAttr.setStorable(false));

	inputRotateAxis = nAttr.create("inputRotateAxis", "ira", MFnNumericData::k3Double);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setArray(false);

	inputSegmentScaleCompensate = nAttr.create("inputSegmentScaleCompensate", "isc", MFnNumericData::kBoolean);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setArray(false);

	inputInverseScale = nAttr.create("inputInverseScale", "iis", MFnNumericData::k3Double);
	nAttr.setWritable(true);
	nAttr.setStorable(true);
	nAttr.setReadable(true);
	nAttr.setKeyable(true);
	nAttr.setArray(false);

	// Outputs
	//
	output = cAttr.create("output", "o", &stat);
	cAttr.setKeyable(false);
	cAttr.setStorable(false);
	cAttr.setArray(false);

	outputTranslation = nAttr.create("outputTranslation", "ot", MFnNumericData::k3Double);
	nAttr.setKeyable(false);
	nAttr.setStorable(false);
	nAttr.setArray(false);
	outputRotation = nAttr.create("outputRotation", "or", MFnNumericData::k3Double);
	nAttr.setKeyable(false);
	nAttr.setStorable(false);
	nAttr.setArray(false);

	outputScale = nAttr.create("outputScale", "os", MFnNumericData::k3Double);
	nAttr.setKeyable(false);
	nAttr.setStorable(false);
	nAttr.setArray(false);

	// Set translation/rotation/scale attrs as children of the parent
	// (compound) attribute
	//
	stat = cAttr.addChild(outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	stat = cAttr.addChild(outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);
	stat = cAttr.addChild(outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputJointOrient);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputJointOrientType);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputWorldMatrix);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputParentInverseMatrix);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputRotateOrder);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputRotateAxis);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputSegmentScaleCompensate);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = addAttribute(inputInverseScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	// Add affect between inputs and outputs
	//
	stat = attributeAffects(inputWorldMatrix, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputParentInverseMatrix, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrient, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrientType, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateOrder, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateAxis, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputSegmentScaleCompensate, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputInverseScale, output);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrient, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrientType, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputParentInverseMatrix, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateOrder, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateAxis, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputSegmentScaleCompensate, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputInverseScale, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputWorldMatrix, outputTranslation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrient, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrientType, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputParentInverseMatrix, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateOrder, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateAxis, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputSegmentScaleCompensate, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputInverseScale, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputWorldMatrix, outputRotation);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrient, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputJointOrientType, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputParentInverseMatrix, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateOrder, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputRotateAxis, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputSegmentScaleCompensate, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputInverseScale, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	stat = attributeAffects(inputWorldMatrix, outputScale);
	CHECK_MSTATUS_AND_RETURN_IT(stat);

	return( MS::kSuccess );
}

// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//
MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY , "6.0", "Any");

	status = plugin.registerNode( "rebalanceTransform", rebalanceTransform::id, rebalanceTransform::creator,
								  rebalanceTransform::initialize );
	if (!status) {
		status.perror("registerNode");
		return( status );
	}

	return( status );
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( rebalanceTransform::id );
	if (!status) {
		status.perror("deregisterNode");
		return( status );
	}

	return( status );
}
