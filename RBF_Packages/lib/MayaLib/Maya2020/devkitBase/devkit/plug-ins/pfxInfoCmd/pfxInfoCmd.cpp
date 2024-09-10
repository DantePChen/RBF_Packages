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
// Produces the command "pfxInfo". 
//
// This plug-in demonstrates how to extract paint effects render line information
// from a Maya scene.
//
// It demonstrates the use of the MFnPfxGeometry class for retrieving
// pfx information.  The paint effects tube data is baked down into
// arrays of line data including position, color, twist, flatness,
// u parameters, color, incandescence, and transparency. This command
// lets one ask for line data, line and width data, or all. However,
// MFnPfxGeometry::getLineData allows one to ask for any combination
// of the total data. The node name passed into the pfxInfo command
// must be a valid pfxGeometry node. Currently, stroke and pfxHair
// nodetypes are instances of the pfxGeometry node class.
//
// Syntax:
//		pfxInfo $pfxNodeName $dataToReturn;
//
// To use the plug-in: 
//		(1) Load the plug-in and a Maya scene that contains paint effects information.
//		(2) Execute the following example:
//  
//		//Get all data
//		pfxInfo strokeShape2; // defaults to all
//		// Or
//		pfxInfo strokeShape2 "all";
//		// Only line and width
// 		pfxInfo pfxHairShape2 "lineAndWidth";
//		// Just line
//		pfxInfo strokeShape1 "line";
//
//		The render line information will be written to the MEL editor.
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MArgList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSelectionList.h>
#include <maya/MPxCommand.h>
#include <maya/MFnDagNode.h>
#include <maya/MRenderLine.h>
#include <maya/MRenderLineArray.h>
#include <maya/MFnPfxGeometry.h>
#include <maya/MFnPlugin.h>


#define mCommandName "pfxInfo"	// Command name

class pfxInfoCmd : public MPxCommand
{
public:
				 pfxInfoCmd();
    		~pfxInfoCmd() override;

    MStatus doIt ( const MArgList& args ) override;

    static void* creator();

private:

	static MStatus nodeFromName(MString name, MObject & obj);

	MStatus parseArgs ( const MArgList& args );

	MStatus printDoubleArray( MDoubleArray &array, const char *title );
	MStatus printVectorArray( MVectorArray &array, const char *title );
	MStatus printRenderLine( MRenderLine &renderLine );
	MStatus printRenderLineArray( MRenderLineArray &lineArray, const char *title );
	MString pfxName;
	MObject pfxNode;
	MString pfxOutput;
};

pfxInfoCmd::pfxInfoCmd()
{
}

pfxInfoCmd::~pfxInfoCmd() 
{
}


//
//
///////////////////////////////////////////////////////////////////////////////

MStatus pfxInfoCmd::nodeFromName(MString name, MObject & obj)
{
	MSelectionList tempList;
    tempList.add( name );
    if ( tempList.length() > 0 ) 
	{
		tempList.getDependNode( 0, obj );
		return MS::kSuccess;
	}
	return MS::kFailure;
}

MStatus pfxInfoCmd::parseArgs( const MArgList& args )
{
	// Parse the arguments.
	MStatus stat = MS::kSuccess;

	// defaults for output if early return
	pfxOutput = "none";

	if( args.length() < 1 )
	{
		MGlobal::displayError( "Missing pfx node name argument(stroke or pfxHair)." );
		return MS::kFailure;
	}
	else if( args.length() > 2 )
	{
		MGlobal::displayError( "Too many arguments." );
		return MS::kFailure;
	}

	pfxName = args.asString( 0, &stat );
	if (stat != MS::kSuccess)
	{
		MGlobal::displayError( "Failed to parse pfx node name argument." );
		return MS::kFailure;
	}

	if(args.length() == 1) {
		// assume that the user wants to print all the voxels
		// they probably won't do this more than once
		pfxOutput = "all";
	} else {
		pfxOutput = args.asString( 1, &stat );
		if (stat != MS::kSuccess)
		{
			MGlobal::displayError( "Failed to parse num voxels to pribt argument." );
			return MS::kFailure;
		}
	}

	nodeFromName( pfxName, pfxNode );

	if( pfxNode.isNull() )
	{
		MGlobal::displayError( "There is no pfx node with the given name." );
		return MS::kFailure;
	}

	if( ! (pfxNode.hasFn( MFn::kPfxGeometry )
	   || pfxNode.hasFn( MFn::kStroke ) 
	   || pfxNode.hasFn( MFn::kPfxHair )) )
	{
		MGlobal::displayError( "The named node is not a stroke or pfxHair node." );
		return MS::kFailure;
	}

	return MS::kSuccess;
}

MStatus pfxInfoCmd::printDoubleArray( MDoubleArray &array, const char *title )
{
	int i;
	char buffer[256];
	int len = array.length();
	sprintf( buffer, "    %s(%d entries):", title, len);
	MGlobal::displayInfo( buffer );

	for( i = 0; i < len; i++ ){
		sprintf( buffer, "      %f", array[i]);
		MGlobal::displayInfo( buffer );
	}
	MGlobal::displayInfo( "  " );
	return MS::kSuccess;
}

MStatus pfxInfoCmd::printVectorArray( MVectorArray &array, const char *title )
{
	int i;
	char buffer[256];
	int len = array.length();
	sprintf( buffer, "    %s(%d entries):", title, len);
	MGlobal::displayInfo( buffer );

	for( i = 0; i < len; i++ ){
		MVector vec = array[i];
		sprintf( buffer, "      (%f, %f, %f)", vec.x, vec.y, vec.z);
		MGlobal::displayInfo( buffer );
	}
	MGlobal::displayInfo( "  " );
	return MS::kSuccess;
}

MStatus pfxInfoCmd::printRenderLine( MRenderLine &renderLine )
{
	MVectorArray line = renderLine.getLine();
	MVectorArray twist = renderLine.getTwist();
	MDoubleArray width = renderLine.getWidth();
	MDoubleArray flatness = renderLine.getFlatness();
	MDoubleArray parameter = renderLine.getParameter();
	MVectorArray color = renderLine.getColor();
	MVectorArray incandescence = renderLine.getIncandescence();
	MVectorArray transparency = renderLine.getTransparency();
	if( line.length() > 0 ){
		printVectorArray( line, "line" );
	}
	if( twist.length() > 0 ){
		printVectorArray( twist, "twist" );
	}
	if( width.length() > 0 ){
		printDoubleArray( width, "width" );
	}
	if( flatness.length() > 0 ){
		printDoubleArray( flatness, "flatness" );
	}
	if( parameter.length() > 0 ){
		printDoubleArray( parameter, "parameter" );
	}
	if( color.length() > 0 ){
		printVectorArray( color, "color" );
	}
	if( incandescence.length() > 0 ){
		printVectorArray( incandescence, "incandescence" );
	}
	if( transparency.length() > 0 ){
		printVectorArray( transparency, "transparency" );
	}
	MGlobal::displayInfo( "  " );
	return MS::kSuccess;
}

MStatus pfxInfoCmd::printRenderLineArray( MRenderLineArray &lineArray, const char *title )
{
	char buffer[256];

	int length = lineArray.length();
	sprintf( buffer, "=%s (%d lines) =============================================", title, length);
	MGlobal::displayInfo( buffer );

	MStatus stat = MS::kSuccess;
	for( int i = 0; i < length; i++ ){
		sprintf( buffer, "  LINE %d .......................", (i+1));
		MGlobal::displayInfo( buffer );
		MRenderLine renderLine = lineArray.renderLine( i, &stat );
		// the renderline array is sparce, so some entries may be 
		// blank (or NULL). MS:kSuccess is only true when the entry
		// is not blank, although this is not an error.
		if( stat == MS::kSuccess ){
			printRenderLine( renderLine );
		}
	} 
	MGlobal::displayInfo( "  " );
	return MS::kSuccess;
}

//
// Main routine
///////////////////////////////////////////////////////////////////////////////
MStatus pfxInfoCmd::doIt( const MArgList& args )
{
	MStatus stat = parseArgs( args );
	if( stat != MS::kSuccess ) {
		return stat;
	}
	if( pfxOutput == "none" ){
		return stat;
	}

	MFnPfxGeometry node( pfxNode );

	MRenderLineArray mainLines, leafLines, flowerLines;

	if( pfxOutput == "all" ){
		stat = node.getLineData( mainLines, leafLines, flowerLines,
		true, true, true, true,
		true, true, true, true, true );
	} else if( pfxOutput == "line" ){
		stat = node.getLineData( mainLines, leafLines, flowerLines,
		true, false, false, false,
		false, false, false, false, true );
	} else if( pfxOutput == "lineAndWidth" ){
		stat = node.getLineData( mainLines, leafLines, flowerLines,
		true, false, true, false,
		false, false, false, false, true );
	}	
	if( stat != MS::kSuccess ){
		return stat;
	}

	stat = printRenderLineArray( mainLines, "MainLines" );
	if( stat != MS::kSuccess ){
		return stat;
	}

	stat = printRenderLineArray( leafLines, "LeafLines" );
	if( stat != MS::kSuccess ){
		return stat;
	}

	stat = printRenderLineArray( flowerLines, "FlowerLines" );
	if( stat != MS::kSuccess ){
		return stat;
	}
	// If getLineData was successfully called then at some point
	// the arrays create for it must be deleted. Delete array
	// will remove all memory internally created for the renderLineArray.
	// Be careful: If you have any MRenderLines or arrays that were returned by this class,
	// you should not access them after calling DeleteArray. The internal
	// wrapped data is currently copied by pointer, with no reference counting. 
	// Thus if you assign one MRenderLineArray to another, you should only call
	// the deleteArray once( and not access the other MRenderLineArray after doing so ).
	mainLines.deleteArray();
	leafLines.deleteArray();
	flowerLines.deleteArray();

	return MS::kSuccess;
}


//
//
///////////////////////////////////////////////////////////////////////////////

void * pfxInfoCmd::creator() { return new pfxInfoCmd(); }


MStatus initializePlugin( MObject obj )
{
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "6.5", "Any");

    MStatus status = plugin.registerCommand(mCommandName,
											pfxInfoCmd::creator );
	if (!status) 
		status.perror("registerCommand");

    return status;
}


MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );

    MStatus status = plugin.deregisterCommand(mCommandName);
	if (!status) 
		status.perror("deregisterCommand");

    return status;
}
