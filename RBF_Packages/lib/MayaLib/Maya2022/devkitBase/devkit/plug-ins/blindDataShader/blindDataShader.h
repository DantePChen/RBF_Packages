#ifndef _blindDataShader
#define _blindDataShader

//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+



#include <maya/MPxHwShaderNode.h>

class blindDataShader : public MPxHwShaderNode
{
public:
	// Standard Node functions
	//
    MStatus compute( const MPlug&, MDataBlock& ) override;
    static  void *  creator();
    static  MStatus initialize();
    static  MTypeId id;
};

#endif /* _blindDataShader */
