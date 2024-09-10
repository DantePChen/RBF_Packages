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
// Produces the dependency graph node "userLoft".
//
// This plug-in demonstrates how to accept geometry as an input and create
// geometry for output. A NURBS curve is input to the node and from it a NURBS
// surface is created. The resulting geometry is passed to an internal Maya
// node which displays it and allows it to be positioned.
//
// To use this node:
//
//	(1) First draw a curve in the X-Y plane.
//	(2) Execute "simpleLoftNode.mel" that contains the following commands:
//
//		createNode transform -n simpleLoft1;
//		createNode nurbsSurface -n simpleLoftShape1 -p simpleLoft1;
//		createNode simpleLoft -n simpleLoftNode1;
//		connectAttr curveShape1.local simpleLoftNode1.inputCurve;
//		connectAttr simpleLoftNode1.outputSurface simpleLoftShape1.create;
//
//	This creates a nurbsSurface node and hooks the result into the world for display.
//	It then creates a simpleLoft node, connects its input to curveShape1
//	(the geometry from the first curve drawn), and connects its output to the NURBS
//	surface node. 
//
//	A surface will now appear on the screen.
//	If the CVs of the original curve are selected and moved, the surface
//	will be reconstructed to match. 
//
////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h>
#include <maya/MPxCommand.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPlugin.h>

#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MFnNurbsSurfaceData.h>

#include <maya/MPointArray.h>
#include <maya/MDoubleArray.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MGlobal.h>
#include <maya/MItCurveCV.h>
#include <maya/MDagPath.h>

class simpleLoft : public MPxNode
{
public:
						simpleLoft() {};
					~simpleLoft() override;

	MStatus		compute( const MPlug& plug, MDataBlock& data ) override;
	MObject				loft( MObject &curve, MObject &surfFn, MStatus &stat );

	static	void*		creator();
	static	MStatus		initialize();

public:
	static	MObject		inputCurve;		// The input curve.
	static	MObject		outputSurface;	// The output curve.
	static	MTypeId		id;				// The IFF type id
};

MTypeId	    simpleLoft::id( 0x80011 );
MObject	    simpleLoft::inputCurve;
MObject	    simpleLoft::outputSurface;

simpleLoft::~simpleLoft() {}

void* simpleLoft::creator()
{
	return new simpleLoft();
}

MStatus simpleLoft::initialize()
{
	MFnTypedAttribute   typedAttr;
	MStatus				stat;

	inputCurve = typedAttr.create( "inputCurve", "in",
			     MFnNurbsCurveData::kNurbsCurve, MObject::kNullObj, &stat );

	if ( !stat ) {
		stat.perror("ERROR creating simpleLoft curve attribute");
		return stat;
	}

	outputSurface = typedAttr.create( "outputSurface", "out",
				MFnNurbsSurfaceData::kNurbsSurface, MObject::kNullObj, &stat );

	if ( !stat ) {
		stat.perror("ERROR creating simpleLoft surface attribute");
		return stat;
	}
	typedAttr.setStorable( false );

	stat = addAttribute( inputCurve );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( outputSurface );
		if (!stat) { stat.perror("addAttribute"); return stat;}

	stat = attributeAffects( inputCurve, outputSurface );
		if (!stat) { stat.perror("attributeAffects"); return stat;}

	return MS::kSuccess;
}

MObject simpleLoft::loft( MObject &curve, MObject &newSurfData, MStatus &stat )
{
	MFnNurbsSurface surfFn;

	MPointArray cvs;
	MDoubleArray ku, kv;
	MFnNurbsCurve curveFn (curve);

	stat = curveFn.getCVs (cvs, MSpace::kWorld);
	if ( stat != MS::kSuccess )
	{
		cerr << "Error in getting CVs: " << stat << endl;
		return MObject::kNullObj;
	}


	int i, j, k = cvs.length();

	// create knot vectors for U and V

	ku.append( 0.0 );
	ku.append( 0.0 );
	ku.append( 0.0 );

	ku.append( 1.0 );
	ku.append( 1.0 );
	ku.append( 1.0 );

	kv.append( 0.0 );
	kv.append( 0.0 );
	kv.append( 0.0 );

	for ( i = 1; i < k-3; i ++ )
		kv.append( (double) i );

	kv.append( k-3 );
	kv.append( k-3 );
	kv.append( k-3 );

	// create cvs for the surface

	for ( i = 1; i < 4; i++ )
	{
		for ( j = 0; j < k; j++ )
		{
			MPoint point = cvs[j];
			point.z += (double) i;	// loft in Z by 1
			cvs.append( point );
		}
	}


	MObject surf = surfFn.create(
		cvs, ku, kv, 3, 3,
		MFnNurbsSurface::kOpen, MFnNurbsSurface::kOpen,
		false, newSurfData, &stat );

	if ( stat != MS::kSuccess )
	{
		cerr << "Error in creating surface: " << stat << endl;
		return MObject::kNullObj;
	}

	// stat = MGlobal::addToModel( surf );
	return surf;

}

MStatus simpleLoft::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus stat;

	if ( plug == outputSurface )	// loft inputCurve into surface
	{
		MDataHandle inputData = data.inputValue( inputCurve, &stat );
		if( stat != MS::kSuccess )
		{
			cerr << "ERROR getting data: " << stat << endl;
			return stat;
		}

		MObject curve = inputData.asNurbsCurve();
		MFnNurbsCurve curveFn( curve, &stat );
		if( stat != MS::kSuccess )
		{
		  cerr << "ERROR creating curve function set:" << stat << endl;
		  return stat;
		}

		MDataHandle surfHandle = data.outputValue( simpleLoft::outputSurface );
		if( stat != MS::kSuccess )
		{
		  cerr << "Error in getting data handle: " << stat << endl;
		  return stat;
		}
		
		MFnNurbsSurfaceData dataCreator;
		MObject newSurfData = dataCreator.create( &stat );
		if ( stat != MS::kSuccess ) {
		  cerr << "Error creating new nurbs surface data block: "
			   << stat << endl;
		  return stat;
		}
		
		/* MObject newSurf = */ loft( curve, newSurfData,  stat );
		if( stat != MS::kSuccess )
		{
		  cerr << "Error in creating surface: " << stat << endl;
		  return stat;
		}
		
		// newSurf is the new surface object, but it has been packed
		// into the datablock we created for it, and the data block
		// is what we must put onto the plug.
		surfHandle.set( newSurfData );
		
		stat = data.setClean( plug );
		if( stat != MS::kSuccess )
		{
		  cerr << "Error in cleaning outputSurface plug: "
			   << stat << endl;
		  return stat;
		}
	}
	else
	{
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

MStatus initializePlugin( MObject obj )
{
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	status = plugin.registerNode( "simpleLoft", simpleLoft::id,
								  simpleLoft::creator, simpleLoft::initialize );
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

	status = plugin.deregisterNode( simpleLoft::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
