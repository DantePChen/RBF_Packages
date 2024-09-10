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
// Produces the dependency graph node "shiftNode".
//
// This plug-in demonstrates modifying uvCoord and refPointCamera from within a
// texture plug-in. The uvCoord and refPointCamera are marked as "renderSource" attributes.
// The uvCoord and refPointCamera for the current sample position are requested
// and then subsequently shifted four times. Each time these attributes are modified,
// the inColor attribute is requested, and because the attributes are render sources,
// the request for inColor forces a shading evaluation. Thus, the 2D or 3D texture
// connected to inColor will be evaluated four additional times for every point shaded.
// The inColor values are averaged which produces a blurred result. 
//
////////////////////////////////////////////////////////////////////////

#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>


// Error macro: if not successful, print error message and return
// the MStatus instance containing the error code.
// Assumes that "stat" contains the error value
#define PERRORfail(stat,msg) \
    if (!(stat)) { stat.perror((msg)); return (stat); }


//////////////////////////
// Forward Declarations //
//////////////////////////

class shiftNode;

/////////////////////////////////
// Plugin Lambert Shader Class //
/////////////////////////////////

class shiftNode : public MPxNode
{
	public: // Methods
	shiftNode();
	~shiftNode() override;
	
	MStatus compute( const MPlug&, MDataBlock& ) override;
	
	static  void *          creator();
	static  MStatus         initialize();

	//  Id tag for use with binary file format
	static MTypeId id;

	private:

	float ox, oy, oz;
	int initID, doneID;
	static bool firstTime;

	// Input
	static MObject aShift;
	static MObject aDist;

	static MObject aColor;

	static MObject aRefPointCamera;
	static MObject aUv;

	// Output
	static MObject aOutColor;

};


// attribute information

MTypeId shiftNode::id( 0x8001a );

// input

MObject     shiftNode::aShift;
MObject     shiftNode::aDist;
MObject     shiftNode::aColor;
MObject     shiftNode::aRefPointCamera;
MObject     shiftNode::aUv;

// output

MObject     shiftNode::aOutColor;

shiftNode::shiftNode()
{
}

shiftNode::~shiftNode()
{
}

// creates an instance of the node
void * shiftNode::creator()
{
	return new shiftNode();
}

// initializes attribute information
MStatus shiftNode::initialize()
{
	MStatus stat;

	MFnNumericAttribute nAttr; 

	// shift

	aShift = nAttr.create( "shift", "sh", 
						  MFnNumericData::kBoolean, false );
	stat = addAttribute( aShift );
	PERRORfail( stat, "addAttribute shift" );

	// distance

	aDist = nAttr.create( "distance", "dis", 
						 MFnNumericData::kFloat, 0.0 );
	stat = addAttribute( aDist );
	PERRORfail( stat, "addAttribute dist" );

	// inColor

	aColor = nAttr.createColor( "color", "c" );
	nAttr.setStorable(false);
	stat = addAttribute( aColor );
	PERRORfail( stat, "addAttribute inColor" );
	
	// refPointCamera

	aRefPointCamera = nAttr.createPoint( "refPointCamera", "rpc" );
	nAttr.setStorable(false);
	nAttr.setWritable(true);
	nAttr.setRenderSource(true);
	nAttr.setHidden(true);
	stat = addAttribute( aRefPointCamera );
	PERRORfail( stat, "addAttribute refPointCamera" );

	// uv

	MObject u = nAttr.create( "uCoord", "u", MFnNumericData::kFloat, 0.0 );
	MObject v = nAttr.create( "vCoord", "v", MFnNumericData::kFloat, 0.0 );
	aUv = nAttr.create( "uvCoord", "uv", u, v );
	nAttr.setStorable(false);
	nAttr.setWritable(true);
	nAttr.setRenderSource(true);
	nAttr.setHidden(true);
	stat = addAttribute( aUv );
	PERRORfail( stat, "addAttribute uv" );

	// create output attributes here

    aOutColor = nAttr.createColor( "outColor", "oc" );
	PERRORfail(stat, "initialize create outColor attribute");
	nAttr.setReadable( true );
	nAttr.setWritable( false );
	stat = addAttribute( aOutColor );
	PERRORfail(stat, "addAttribute(outColor)");
	
	// attribute affects

	attributeAffects ( aShift, aOutColor );
	attributeAffects ( aDist, aOutColor );
	attributeAffects ( aColor, aOutColor );
	attributeAffects ( aRefPointCamera, aOutColor );
	attributeAffects ( aUv, aOutColor );

	// we need to do the followings to cause other input attributes
	// to evaluate with the new values we set

	attributeAffects ( aUv, aUv );
	attributeAffects ( aRefPointCamera, aRefPointCamera );

	return MS::kSuccess;
}


/*

This function gets called by Maya to evaluate the texture.

*/
MStatus shiftNode::compute( const MPlug& plug, MDataBlock& data ) 
{
	MStatus stat;

	if ((plug != aOutColor) && (plug.parent() != aOutColor))
		return MS::kUnknownParameter;

	MDataHandle colorH;
	MFloatVector color;

	MDataHandle shiftH = data.inputValue( aShift, &stat);
	PERRORfail(stat, "compute getting shift attr");
	bool shiftIt = shiftH.asBool();

	MDataHandle distH = data.inputValue( aDist, &stat);
	PERRORfail(stat, "compute getting distance attr");
	float distance = distH.asFloat();

	MFloatVector clr;

	if ( shiftIt && distance != 0.0 )
	{
		// first evaluate color at default sample posiiton

		clr = data.inputValue( aColor ).asFloatVector();

		// uv is used by 2d textures
		// refPointCamera is used by 3d textures

		MDataHandle refPointCamH = data.inputValue( aRefPointCamera, &stat);
		PERRORfail(stat, "compute getting refPointCamera attr");
		MFloatVector refPC = refPointCamH.asFloatVector();

		// get current UV

		const float2 & oldUV = data.inputValue(aUv).asFloat2();

		// shift and set the uv/refPointCamera values so
		// we can sample around the current uv/refPointCamera

		MDataHandle outUV = data.outputValue( aUv );
		MDataHandle outPC = data.outputValue( aRefPointCamera );

		outUV.set( oldUV[0]-distance, oldUV[1] );
		outPC.set( refPC.x + distance, refPC.y + distance, refPC.z + distance);
		colorH = data.inputValue( aColor, &stat);	// evaluate at new pos
		color = colorH.asFloatVector();
		clr += color;

		outUV.set( oldUV[0]+distance, oldUV[1] );
		outPC.set( refPC.x - distance, refPC.y + distance, refPC.z + distance);
		colorH = data.inputValue( aColor, &stat);	// evaluate at new pos
		color = colorH.asFloatVector();
		clr += color;

		outUV.set( oldUV[0], oldUV[1]-distance );
		outPC.set( refPC.x + distance, refPC.y - distance, refPC.z + distance);
		colorH = data.inputValue( aColor, &stat);	// evaluate at new pos
		color = colorH.asFloatVector();
		clr += color;

		outUV.set( oldUV[0], oldUV[1]+distance );
		outPC.set( refPC.x - distance, refPC.y - distance, refPC.z + distance);
		colorH = data.inputValue( aColor, &stat);	// evaluate at new pos
		color = colorH.asFloatVector();
		clr += color;

		clr /= 5.0;	// average the colors from all locations

		// set sample data back to original values

		outUV.set( oldUV[0], oldUV[1] );
		outPC.set( refPC.x, refPC.y, refPC.z ); 
	}
	else
	{
		colorH = data.inputValue( aColor, &stat);
		clr = colorH.asFloatVector();
	}

	MDataHandle outColorHandle = data.outputValue( aOutColor );
	MFloatVector& oclr = outColorHandle.asFloatVector();
	oclr = clr;
	outColorHandle.setClean();

	return MS::kSuccess;
}


//
//
//

MStatus initializePlugin( MObject obj )
{
   const MString UserClassify( "utility/general" );

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");
	plugin.registerNode( "shiftNode", shiftNode::id,
                         shiftNode::creator, shiftNode::initialize,
                         MPxNode::kDependNode, &UserClassify );

   return MS::kSuccess;

}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );
	plugin.deregisterNode( shiftNode::id );

	return MS::kSuccess;
}
