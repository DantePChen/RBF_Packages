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

#ifndef _SLOPESHADERBEHAVIOR_H_
#define _SLOPESHADERBEHAVIOR_H_

#include <maya/MPxDragAndDropBehavior.h>

class slopeShaderBehavior : public MPxDragAndDropBehavior
{

public:

	slopeShaderBehavior();
	~slopeShaderBehavior() override;

	bool shouldBeUsedFor( MObject &sourceNode, MObject &destinationNode,
								  MPlug &sourcePlug, MPlug &destinationPlug) override;

	MStatus connectNodeToNode( MObject &sourceNode, MObject &destinationNode, bool force ) override;

	MStatus connectNodeToAttr( MObject &sourceNode, MPlug &destinationPlug, bool force ) override;

	static void *creator();

protected:

	MObject findShadingEngine(MObject &node);

private:

};

#endif //_SLOPESHADERBEHAVIOR_H_

