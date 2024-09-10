#include"WeightMultiplyNode.h"

#include<maya/MFnNumericAttribute.h>
#include <maya/MArrayDataHandle.h>

static const MTypeId Type_ID = MTypeId(0x5737A7F8);
static const MString Type_Name = "WeightMultiply";


MObject WeightMultiplyNode::multiAttributeObj;
MObject WeightMultiplyNode::multiWeightObj;
MObject WeightMultiplyNode::productObj;

WeightMultiplyNode::WeightMultiplyNode() :
	MPxNode()
{

}

WeightMultiplyNode::~WeightMultiplyNode()
{

}

MStatus WeightMultiplyNode::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus status;
	if (plug == productObj) {
		MArrayDataHandle multiWeightArray = data.inputValue(multiWeightObj,&status);
		MArrayDataHandle multiAttrArray = data.inputValue(multiAttributeObj, &status);
		unsigned int countW = multiWeightArray.elementCount();
		unsigned int countA = multiAttrArray.elementCount();

		double outX = 0;
		double outY = 0;
		double outZ = 0;

		if (countW == countA) {
			for (int i = 0; i < countW; i++) {
				status = multiWeightArray.jumpToArrayElement(i);
				double weight = multiWeightArray.inputValue(&status).asDouble();
				status = multiAttrArray.jumpToArrayElement(i);
				double3& multiAttr = multiAttrArray.inputValue(&status).asDouble3();
				
				double AttrX = multiAttr[0];
				double AttrY = multiAttr[1];
				double AttrZ = multiAttr[2];
				outX += AttrX * weight;
				outY += AttrY * weight;
				outZ += AttrZ * weight;


			}
		}
		
		
		MDataHandle out = data.outputValue(productObj, &status);
		out.set3Double(outX,outY,outZ);

		data.setClean(plug);

	}
	return(MS::kSuccess);
}


void* WeightMultiplyNode::Creator()
{
	return(new WeightMultiplyNode());
}

MStatus WeightMultiplyNode::Initialize()
{
	MFnNumericAttribute numericAttr;

	multiAttributeObj = numericAttr.create("multiplyAttribute", "multiAttr", MFnNumericData::k3Double);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);
	numericAttr.setArray(true);
	numericAttr.setUsesArrayDataBuilder(true);
	numericAttr.setDisconnectBehavior(MFnAttribute::kDelete);

	multiWeightObj = numericAttr.create("multiplyWeight", "multiWei", MFnNumericData::kDouble, 0.0);
	numericAttr.setKeyable(true);
	numericAttr.setArray(true);
	numericAttr.setReadable(false);
	numericAttr.setUsesArrayDataBuilder(true);
	numericAttr.setDisconnectBehavior(MFnAttribute::kDelete);

	productObj = numericAttr.create("product", "prod", MFnNumericData::k3Double);
	numericAttr.setWritable(false);
	//numericAttr.setUsesArrayDataBuilder(true);
	

	addAttribute(multiAttributeObj);
	addAttribute(multiWeightObj);
	addAttribute(productObj);

	attributeAffects(multiAttributeObj, productObj);
	attributeAffects(multiWeightObj, productObj);

	return (MS::kSuccess);
}

MTypeId WeightMultiplyNode::GetTypeId()
{
	return(Type_ID);
}

MString WeightMultiplyNode::GetTypeName()
{
	return(Type_Name);
}