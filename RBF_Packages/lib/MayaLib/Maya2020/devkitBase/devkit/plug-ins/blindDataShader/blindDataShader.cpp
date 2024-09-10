//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include <maya/MFloatVector.h>
#include <maya/MPlug.h>
#include "blindDataShader.h"


/****************************************************************************
 * Blind Data Shader class
 ***************************************************************************/

// Static class member variables
//
// Unique Node TypeId
MTypeId blindDataShader::id( 0x00086000 );

void* blindDataShader::creator()
//
// Description: Static member function that returns a new
// dynamically-allocated instance of the class.
//
{
    return new blindDataShader();
}

MStatus blindDataShader::initialize()
//
// Description: Static member function that initializes the static
// member variables of the class. In this case, it does nothing, since
// "id" is already set to the default value.
//
{
	return MS::kSuccess;
}

MStatus blindDataShader::compute(
const MPlug&      plug,
      MDataBlock& block ) 
{ 
    bool k = false;
    k |= (plug==outColor);
    k |= (plug==outColorR);
    k |= (plug==outColorG);
    k |= (plug==outColorB);
    if( !k ) return MS::kUnknownParameter;

	// Always return black for now.
    MFloatVector resultColor(0.0,0.0,0.0);

    // set ouput color attribute
    MDataHandle outColorHandle = block.outputValue( outColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

    return MS::kSuccess;
}
