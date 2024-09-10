#pragma once

#include <maya/MPxNode.h>
class WeightMultiplyNode : public MPxNode
{
public:
	WeightMultiplyNode();
	virtual ~WeightMultiplyNode() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;


	// Static Methods
	static void* Creator();

	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();
private:
	// Input attributes
	static MObject multiAttributeObj;
	static MObject multiWeightObj;
	// Output attribute
	static MObject productObj;
};