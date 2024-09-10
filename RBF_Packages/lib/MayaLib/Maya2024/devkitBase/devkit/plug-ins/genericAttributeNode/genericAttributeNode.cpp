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
// 
// DESCRIPTION:
// 
// Produces the dependency node "genericAttributeNode". 
//
// This plug-in demonstrates how to create a MPxNode that contains generic attributes.
// Several generic attributes are added to the node and the compute() method sets its result
// based on a controlling type attribute. 
// 
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h> 

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MGlobal.h>
 
//
// Node definition
//
class genericAttributeNode : public MPxNode
{
public:
						genericAttributeNode();
					~genericAttributeNode() override; 

	MStatus		compute( const MPlug& plug, MDataBlock& data ) override;

	static  void*		creator();
	static  MStatus		initialize();

	static MStatus	addComplexFloatGenericAttribute( MObject& attrObject, const char *longName, const char *shortName );
	
public:
	// Numeric attribute
	static MObject		gInputInt;
	// Generic attribute that can be a float, 2float, 3float
	static MObject		gOutputFloat_2Float_3Float;
	// Type id
	static	MTypeId		id;
};

// Static initialization
MTypeId genericAttributeNode::id( 0x81031 );
MObject genericAttributeNode::gInputInt;
MObject genericAttributeNode::gOutputFloat_2Float_3Float;

// No-op constructor and destructor
genericAttributeNode::genericAttributeNode() {}
genericAttributeNode::~genericAttributeNode() {}

//
// Compute that will update the generic attribute
// when requested.
//
MStatus genericAttributeNode::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus returnStatus;
 
	if ( plug == gOutputFloat_2Float_3Float )
	{
		// attribute affecting generic attribute case.  Based on the
		// input attribute, we modify the output generic attribute
		MDataHandle inputDataInt = data.inputValue( gInputInt );
		int inputInt = inputDataInt.asInt();
		
		// Get the output handle
		MDataHandle outputData = data.outputValue( plug );	
		bool isGenericNumeric = false;
		bool isGenericNull = false;
		
		// Is the output handle generic data
		if ( outputData.isGeneric( isGenericNumeric, isGenericNull ) )
		{
			// Based on the inputHandle, update the generic
			// output handle
			if ( inputInt == 1 )
				outputData.setGenericBool( false, true );
			else if ( inputInt == 2 )
				outputData.setGenericBool( true, true );
			else if ( inputInt == 3 )
				outputData.setGenericChar( 127, true );
			else if ( inputInt == 4 )
				outputData.setGenericDouble( 3.145, true );
			else if ( inputInt == 5 )
				outputData.setGenericFloat( (float)9.98, true );	
			else if ( inputInt == 6 )
				outputData.setGenericShort( 3245, true );
			else if ( inputInt == 7 )
				outputData.setGenericInt( 32768, true );
			else if ( inputInt == 8 )
			{
				MFnNumericData numericData;
				MObject obj = numericData.create( MFnNumericData::k2Float, &returnStatus );
				CHECK_MSTATUS( returnStatus );
				
				returnStatus = numericData.setData( (float)1.5, (float)6.7 );
				CHECK_MSTATUS( returnStatus );

				outputData.set( obj );
			}
			else if ( inputInt == 9 )
			{
				MFnNumericData numericData;
				MObject obj = numericData.create( MFnNumericData::k3Float, &returnStatus );
				CHECK_MSTATUS( returnStatus );

				returnStatus = numericData.setData( (float)2.5, (float)8.7, (float)2.3345 );
				CHECK_MSTATUS( returnStatus );

				outputData.set( obj );
			}
			else if ( inputInt == 10 )
			{
				outputData.setGenericInt( 909, true );
			}							

			// Mark the data clean
			outputData.setClean();
			data.setClean( gOutputFloat_2Float_3Float );
		}
	} 
	else 
	{
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

// Creator
void* genericAttributeNode::creator()
{
	return new genericAttributeNode();
}

// Adds a generic attribute that accepts a float, float2, float3
MStatus	genericAttributeNode::addComplexFloatGenericAttribute( 
	MObject& attrObject,
	const char *longName, const char *shortName )
{
	// Create the generic attribute and set the 3 accepts types
	MFnGenericAttribute gAttr; 
	attrObject = gAttr.create( longName, shortName ); 
    MStatus status = gAttr.addNumericDataAccept(MFnNumericData::kFloat);
	CHECK_MSTATUS( status );

    status = gAttr.addNumericDataAccept(MFnNumericData::k2Float);
	CHECK_MSTATUS( status );

    status = gAttr.addNumericDataAccept(MFnNumericData::k3Float);
	CHECK_MSTATUS( status );

	gAttr.setWritable(false);
	gAttr.setStorable( false );
	
	// Add the attribute to the node
	status = addAttribute( attrObject );
	CHECK_MSTATUS( status );

	return MS::kSuccess;
}

// Create and add attributes
MStatus genericAttributeNode::initialize()
{
	MFnNumericAttribute nAttr;
	MStatus status;

	// single float attribute affecting a generic attribute
	gInputInt = nAttr.create( "gInputInt", "gii",
            	   MFnNumericData::kInt, 0, &status );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	CHECK_MSTATUS( status );

	status = addAttribute( gInputInt );
	CHECK_MSTATUS( status );

	if ( !addComplexFloatGenericAttribute( gOutputFloat_2Float_3Float,
					 "gOutputFloat_2Float_3Float", "gof2f3f" ) )
		return MS::kFailure;

	status = attributeAffects( gInputInt, gOutputFloat_2Float_3Float );
	CHECK_MSTATUS( status );
	
	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "7.0", "Any");

	status = plugin.registerNode( "genericAttributeNode", 
									genericAttributeNode::id, genericAttributeNode::creator,
								  	genericAttributeNode::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( genericAttributeNode::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
