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
// This plug-in produces the dependency graph node "arcLen".
//
// This node is a simple example of how to take geometry as an input to a dependency node.
// This node takes a NURBS curve as an input and outputs its arc length using the MFnNurbsCurve function to perform the calculation.
//
// The input for this node is a NURBS curve attribute called "inputCurve". 
// NURBS curve shape nodes have two compatible output attributes that you can use as inputs for the arcLen node: "local" and "worldSpace".
// 
// The output attribute of the arcLen node is called "output".
// It is a double value that represents the total length of the curve with an epsilon value of 0.001.
// 
// The following MEL code shows how to hook up the node to a curve:
// createNode -n arcLen1 arcLen;
// connectAttr curveShape1.local arcLen1.inputCurve;
//
// Now, you can connect the "output" attribute to whatever you wish to drive, or read it using the following command:
// getAttr arcLen1.output;
// 
// This node is provided as an example of how to take geometry as an input.
// But, note that there is already a node in Maya that performs the same service called "curveInfo".
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h> 
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 

class arcLen : public MPxNode
{
public:
						arcLen();
					~arcLen() override; 

    MStatus		compute( const MPlug& plug, MDataBlock& data ) override;
	static  void*		creator();
	static  MStatus		initialize();

public:
	static  MObject     inputCurve;  // The input curve.
	static  MObject     output;      // The output value.
	static	MTypeId		id;          // The IFF type id
};

MTypeId     arcLen::id( 0x80001  );
MObject     arcLen::inputCurve;        
MObject     arcLen::output;       

arcLen::arcLen() {}
arcLen::~arcLen() {}

void* arcLen::creator()
{
	return new arcLen();
}

MStatus arcLen::compute( const MPlug& plug, MDataBlock& data )
{
	
	MStatus status;
 
	if( plug == output )
	{
		MDataHandle inputData = data.inputValue( inputCurve, &status );

		if( !status ) {
			status.perror("ERROR getting data");
		} else {
			MObject curve = inputData.asNurbsCurveTransformed();
			MFnNurbsCurve curveFn( curve, &status ); 
			if( !status ) {
				status.perror("ERROR creating curve function set");
			} else {
				double result = curveFn.length();
				MDataHandle outputHandle = data.outputValue( arcLen::output );
				outputHandle.set( result );
				data.setClean(plug);
			}
		}
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

MStatus arcLen::initialize()
{
	MFnNumericAttribute numericAttr;
	MFnTypedAttribute   typedAttr;
	MStatus status;

	inputCurve = typedAttr.create( "inputCurve", "in", 
									MFnData::kNurbsCurve,
									MObject::kNullObj,
									&status );

	if( !status ) {
		status.perror("ERROR creating arcLen curve attribute");
		return status;
	}

	output = numericAttr.create( "output", "out", 
									MFnNumericData::kDouble, 0.0, 
									&status );
	if( !status ) {
		status.perror("ERROR creating arcLen output attribute");
		return status;
	}
	numericAttr.setWritable(false);

	status = addAttribute( inputCurve );
	if( !status ) {
		status.perror("addAttribute(inputCurve)");
		return status;
	}

	status = addAttribute( output );
	if( !status ) {
		status.perror("addAttribute(output)");
		return status;
	}

	status = attributeAffects( inputCurve, output );
	if( !status ) {
		status.perror("attributeAffects(inputCurve, output)");
		return status;
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "arcLen", arcLen::id, arcLen::creator,
											   arcLen::initialize);
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

	status = plugin.deregisterNode( arcLen::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
