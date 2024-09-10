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

#include <maya/MPxDragAndDropBehavior.h>

class hwPhongShaderBehavior : public MPxDragAndDropBehavior
{

	public:

	hwPhongShaderBehavior();
	~hwPhongShaderBehavior() override;

	bool shouldBeUsedFor(MObject &sourceNode, MObject &destinationNode,
								 MPlug &sourcePlug, MPlug &destinationPlug) override;

	MStatus connectNodeToNode(MObject &sourceNode, 
									  MObject &destinationNode, bool force ) override;

	MStatus connectNodeToAttr(MObject &sourceNode,
									  MPlug &destinationPlug, bool force ) override;

	MStatus connectAttrToNode(MPlug &sourceNode, 
									  MObject &destinationNode, bool force ) override;

	MStatus connectAttrToAttr(MPlug &sourceNode,
									  MPlug &destinationPlug, bool force ) override;

	static void *creator();

	protected:

	MObject findShadingEngine(MObject &node);

	private:
};

