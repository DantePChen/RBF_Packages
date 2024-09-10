//
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

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// Produces a dependency graph node "exampleRampAttrNode".
// 
// This plug-in demonstrates how to create curve and color ramps on a custom node.
// Additionally, the coding for the AE template is included in the example.
//
// MEL usage:
//
//	loadPlugin exampleRampAttribute;
//	createNode exampleRampAttrNode;
//
////////////////////////////////////////////////////////////////////////

#include <maya/MIOStream.h>

#include <maya/MPxNode.h> 
#include <maya/MFnPlugin.h>
#include <maya/MString.h> 
#include <maya/MTypeId.h> 
#include <maya/MPlug.h>
#include <maya/MRampAttribute.h>
#include <maya/MCurveAttribute.h>


class exampleRampAttrNode : public MPxNode
{
public:
	exampleRampAttrNode() {};
	       ~exampleRampAttrNode() override {};
	static  void   *creator();
	static  MStatus initialize();
	
	void	postConstructor() override;
	MStatus compute(const MPlug &plug, MDataBlock  &dataBlock) override;
	MStatus	shouldSave(const MPlug &, bool &) override;

	static MObject input1;
	static MObject input2;
	static MObject input3;
	static MTypeId  id;

};

MTypeId exampleRampAttrNode::id( 0x81027 );

MObject exampleRampAttrNode::input1;
MObject exampleRampAttrNode::input2;
MObject exampleRampAttrNode::input3;

void *exampleRampAttrNode::creator()
{
   return((void *) new exampleRampAttrNode);
}

void exampleRampAttrNode::postConstructor()
{
	MPlug curvePlug( thisMObject(), exampleRampAttrNode::input3 ); 
	MCurveAttribute curveAttr( curvePlug );

	MFloatArray pos;
	pos.append(0.0f);
	pos.append(0.25f);
	pos.append(0.5f);
	pos.append(1.0f);

	MFloatArray val;
	val.append(0.0f);
	val.append(0.25f);
	val.append(0.50f);
	val.append(1.0f);

	MStatus stat = curveAttr.setCurve(pos, val);
	if(!stat)
	{
		cout << "ERROR setting curve points!\n";
	}
}

MStatus exampleRampAttrNode::initialize()
{
	MStatus stat;

	MString curveRamp("curveRamp");
	MString cvr("cvr");
	MString colorRamp("colorRamp");
	MString clr("clr");
	MString curveAttrRamp("curveAttrRamp");
	MString car("car");

	input1 = MRampAttribute::createCurveRamp(curveRamp, cvr);
	input2 = MRampAttribute::createColorRamp(colorRamp, clr);
	input3 = MCurveAttribute::createCurveAttr(curveAttrRamp, car);

	stat = addAttribute(input1);
	if(!stat)
	{
		cout << "ERROR in adding curveRamp Attribute!\n";
	}
	stat = addAttribute(input2);
	if(!stat)
	{
		cout << "ERROR in adding colorRamp Attribute!\n";
	}
	stat = addAttribute(input3);
	if(!stat)
	{
		cout << "ERROR in adding curveAttrRamp Attribute!\n";
	}

	return stat;
}

MStatus exampleRampAttrNode::compute( const MPlug &plug, MDataBlock  &dataBlock ) 
{
	return(MStatus::kSuccess);
}

MStatus exampleRampAttrNode::shouldSave(const MPlug &plug, bool &result)
{
	if (plug == input3 || plug.parent() == input3) {
		result = true; // required to ensure curve attribute saved correctly
		return MS::kSuccess;
    }
	return MPxNode::shouldSave(plug, result);
}

MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "7.0", "Any");

	status = plugin.registerNode("exampleRampAttrNode", exampleRampAttrNode::id,
		   	exampleRampAttrNode::creator, exampleRampAttrNode::initialize);
	if (!status) 
	{
		status.perror("registerNode");
		return status;
	}

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	status = plugin.deregisterNode(exampleRampAttrNode::id);
	if (!status) 
	{
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
