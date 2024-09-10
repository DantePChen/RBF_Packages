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

#ifndef __TESSELLATED_QUAD_NODE_H__
#define __TESSELLATED_QUAD_NODE_H__

#include <maya/MPxLocatorNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MTypeId.h> 


class TessellatedQuadNode : public MPxLocatorNode
{
public:

	static const MTypeId	kTypeId;
	static const MString	kTypeName;
	static const MString	kDrawClassification;

	static  void*			creator();
	static  MStatus			initialize();
	
							TessellatedQuadNode();
	virtual					~TessellatedQuadNode();

	virtual bool			isBounded() const override;
	virtual MBoundingBox	boundingBox() const override;

	virtual void			draw(
		M3dView &view,
		const MDagPath &dagPath,
		M3dView::DisplayStyle style,
		M3dView::DisplayStatus displayStatus) override;

};

#endif // __TESSELLATED_QUAD_NODE_H__
