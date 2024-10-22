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
//  Class: latticeNoiseNode
//
//  Description:
//      A "latticeNoise" node adds random noise to lattice geometry over time.
//      This makes the geometry being deformed by the lattice look like 
//      wobbly jello.
//
//
//  Node:       latticeNoise
//
//  Attributes: input     - input lattice 
//              amplitude - amplitude of the noise
//              frequency - frequency of the noise
//              time      - the current time
//              output    - the modified lattice
//  
#include "latticeNoise.h"

#include <string.h>
#include <maya/MIOStream.h>
#include <stdio.h>
#include <math.h>

#include <maya/MFnUnitAttribute.h>  
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>  
#include <maya/MFnLattice.h>
#include <maya/MFnLatticeData.h>

#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h> 
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MTime.h>

// CONSTANTS
//
#define DEF_AMPLITUDE 0.2
#define DEF_FREQ      4.0 

#define McheckErr(stat,msg)          \
    if ( MS::kSuccess != stat ) {   \
		cerr << msg;                \
		return MS::kFailure;        \
	}


////////////////////////////////////////////////////////
//             latticeNoiseNode Methods                 //
////////////////////////////////////////////////////////

MTypeId     latticeNoiseNode::id( 0x80010 );
MObject     latticeNoiseNode::input;  
MObject     latticeNoiseNode::amplitude;  
MObject     latticeNoiseNode::frequency;  
MObject     latticeNoiseNode::time;      
MObject     latticeNoiseNode::output;       

MStatus latticeNoiseNode::compute( const MPlug& plug, MDataBlock& data )
{ 
	MStatus returnStatus;

	float noiseAmplitude;
	float noiseFreq;
 
	if( plug == output )
	{
		// Get the lattice data from the input attribute.  First get the 
		// data object, and then use the lattice data function set to extract
		// the actual lattice.
		//

		// Get the data handle
		//
		MDataHandle inputData = data.inputValue( input, &returnStatus );
		McheckErr( returnStatus, "ERROR getting lattice data handle\n" ); 
		// Get the data object
		//
		MObject latticeData = inputData.data(); 
		MFnLatticeData dataFn( latticeData );
		// Get the actual geometry
		// 
		MObject lattice = dataFn.lattice();
		MFnLattice lattFn( lattice, &returnStatus );
		McheckErr( returnStatus, "ERROR getting lattice geometry\n" );  


		// Do the same for the output lattice
		//
		MDataHandle outputData = data.outputValue( output, &returnStatus ); 
		McheckErr( returnStatus, "ERROR getting lattice data handle\n" );
		// Get the data object
		//
		latticeData = outputData.data(); 
		if ( latticeData.isNull() ) { 
			// The data object for this attribute has not been created yet, so
			// we'll create it
			//
			latticeData = dataFn.create();
		} else {
			// Use the data object that is already there
			// 
			dataFn.setObject( latticeData );
		}
		// Get the actual geometry
		// 
		MObject outLattice = dataFn.lattice();
		MFnLattice outLattFn( outLattice, &returnStatus );
		McheckErr( returnStatus, "ERROR getting lattice geometry\n" );  

		// Get the amplitude and frequency
		//
		MDataHandle ampData = data.inputValue( amplitude, &returnStatus );
		McheckErr( returnStatus, "ERROR getting amplitude\n" );
		noiseAmplitude = ampData.asFloat(); 

		MDataHandle freqData = data.inputValue( frequency, &returnStatus );
		McheckErr( returnStatus, "ERROR getting frequency\n" );
		noiseFreq = freqData.asFloat(); 

		// Get the time.  
		//
		MDataHandle timeData = data.inputValue( time, &returnStatus ); 
		McheckErr( returnStatus, "ERROR getting time data handle\n" );
		MTime time = timeData.asTime();
		float seconds = (float)time.as( MTime::kSeconds );

		// Easiest way to modify frequency is by modifying the time
		//
		seconds = seconds * noiseFreq;

		// We have the information we need now.  We'll apply noise to the
		// points upon the lattice
		//
		unsigned s, t, u;
		lattFn.getDivisions( s, t, u );
		// match up the divisions in the lattices
		//
		outLattFn.setDivisions( s, t, u );   

		for ( unsigned i = 0; i < s; i++ ) {
			for ( unsigned j = 0; j < t; j++ ) {
				for ( unsigned k = 0; k < u; k++ ) {
					MPoint & point = lattFn.point( i, j, k );
					MPoint & outPoint = outLattFn.point( i, j, k );
					pnt noisePnt = noise::atPointAndTime( (float)point.x, (float)point.y, 
														  (float)point.z, seconds );
					// Make noise between -1 and 1 instead of 0 and 1
					//
					noisePnt.x =  ( noisePnt.x * 2.0F ) - 1.0F;
					noisePnt.y =  ( noisePnt.y * 2.0F ) - 1.0F;
					noisePnt.z =  ( noisePnt.z * 2.0F ) - 1.0F;

 					outPoint.x = point.x + ( noisePnt.x * noiseAmplitude );
 					outPoint.y = point.y + ( noisePnt.y * noiseAmplitude );
 					outPoint.z = point.z + ( noisePnt.z * noiseAmplitude );  
				}
			}
		} 
		outputData.set( latticeData );
		data.setClean(plug); 
	} else {
		return MS::kUnknownParameter;
	}

	return MS::kSuccess;
}

void* latticeNoiseNode::creator()
{
	return new latticeNoiseNode;
}

MStatus latticeNoiseNode::initialize()
{ 
	MFnUnitAttribute    unitAttr;
	MFnTypedAttribute   typedAttr;
	MFnNumericAttribute numAttr;

	MStatus stat;

	input = typedAttr.create( "input", "in", MFnData::kLattice, MObject::kNullObj, &stat );
	if ( !stat ) {
		stat.perror("ERROR creating latticeNoiseNode input lattice attribute");
		return stat;
	}

	amplitude = numAttr.create( "amplitude", "amp", 
											   MFnNumericData::kFloat,
											   DEF_AMPLITUDE,
											   &stat );
	if ( !stat ) {
		stat.perror("ERROR creating latticeNoiseNode amplitude attribute");
		return stat;
	}

	frequency = numAttr.create( "frequency", "fq", 
											   MFnNumericData::kFloat,
											   DEF_FREQ,
											   &stat );
	if ( !stat ) {
		stat.perror("ERROR creating latticeNoiseNode frequency attribute");
		return stat;
	}

	time = unitAttr.create( "time", "tm", MFnUnitAttribute::kTime,
											0.0, &stat );
	if ( !stat ) {
		stat.perror("ERROR creating latticeNoiseNode time attribute");
		return stat;
	}

	output = typedAttr.create( "output", "out", MFnData::kLattice, MObject::kNullObj, &stat ); 
	if ( !stat ) {
		stat.perror("ERROR creating latticeNoiseNode output attribute");
		return stat;
	}
	typedAttr.setWritable(false);

	stat = addAttribute( input );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( amplitude );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( frequency );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( time );
		if (!stat) { stat.perror("addAttribute"); return stat;}
	stat = addAttribute( output );
		if (!stat) { stat.perror("addAttribute"); return stat;}

	stat = attributeAffects( input, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( amplitude, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( frequency, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}
	stat = attributeAffects( time, output );
		if (!stat) { stat.perror("attributeAffects"); return stat;}


	return MS::kSuccess;
}
