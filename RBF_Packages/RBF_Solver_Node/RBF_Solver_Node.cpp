#include<maya/MFnNumericAttribute.h>
#include<maya/MFnMatrixAttribute.h>
#include<maya/MFnCompoundAttribute.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MPlug.h>
#include<maya/MGlobal.h>
#include<maya/MMatrix.h>
#include<maya/MVector.h>
#include<maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MDataBlock.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDGModifier.h>
#include"RBF_Solver_Compute.h"
#include"RBF_Solver_Node.h"
#include"Eigen/Dense"

using namespace Eigen;


//constants
static const MTypeId TypeID = MTypeId(0x573600);
static const MString TypeName = "RBF_Solver";

MObject RBF_Solver_Node::currentPositionObj;
MObject RBF_Solver_Node::poseInputObj;
MObject RBF_Solver_Node::poseInputMatrixObj;
MObject RBF_Solver_Node::poseInputValueObj;
MObject RBF_Solver_Node::outputValueObj;
 

//public methods
//basic constructor and destrucor funtion
RBF_Solver_Node::RBF_Solver_Node() :
	MPxNode()
{
}

RBF_Solver_Node::~RBF_Solver_Node()
{
}

//compute method
MStatus RBF_Solver_Node::compute(const MPlug& plug, MDataBlock& data)
{
	MStatus status;
	if (plug == outputValueObj) {
		MArrayDataHandle poseInputArray = data.inputValue(poseInputObj, &status);
		MDataHandle currentPositionHandle = data.inputValue(currentPositionObj, &status);

		//MVector currentPositionVector = RBF_Solver_Compute::extractPositionVectorFromMatrix(currentPositionHandle);
		unsigned count = poseInputArray.elementCount();
		//initialize the matrix for next steps
		MatrixXd distanceMatrix = MatrixXd::Zero(count, count);
		MatrixXd featureMatrix = MatrixXd::Zero(count, count);
		MatrixXd inverseFeatureMatrix = MatrixXd::Zero(count, count);
		MatrixXd inputValueMatrix = MatrixXd::Zero(count, count);
		MatrixXd currentDisMatrix = MatrixXd::Zero(1, count);
		MatrixXd currentNormDisMatrix = MatrixXd::Zero(1, count);
		MatrixXd outputWeightMatrix = MatrixXd::Zero(1, count);


		// get the distanceMatrix
		for (unsigned int i = 0; i < count; i++) {
			for (unsigned int j = 0; j < count; j++)
			{
				status = poseInputArray.jumpToArrayElement(i);
				MMatrix tempM = poseInputArray.inputValue(&status).child(poseInputMatrixObj).asMatrix();
				MVector tempV0 = { tempM(3,0),tempM(3,1),tempM(3,2) };

				status = poseInputArray.jumpToArrayElement(j);
				tempM = poseInputArray.inputValue(&status).child(poseInputMatrixObj).asMatrix();
				MVector tempV1 = { tempM(3,0),tempM(3,1),tempM(3,2) };

				distanceMatrix(i, j) = RBF_Solver_Compute::distance(tempV0, tempV1);
			}
		}
		
		
		// normallize the distanceMatrix to get the featureMatrix
		featureMatrix = RBF_Solver_Compute::normalizeMatrix(distanceMatrix);

		// get the inverse featureMatrix
		inverseFeatureMatrix = featureMatrix.inverse();

		//get the inputValueMatrix
		for (unsigned int i = 0; i < count; i++) {
			for (unsigned int j = 0; j < count; j++) {
				if (i == j) {
					status = poseInputArray.jumpToArrayElement(i);
					double tempValue = poseInputArray.inputValue(&status).child(poseInputValueObj).asDouble();
					inputValueMatrix(i, j) = tempValue;
				}
				else {
					inputValueMatrix(i, j) = 0;
				}
			}

		}
		
		// compute the distance matrix
		MMatrix currentM = currentPositionHandle.asMatrix();
		MVector currentV = { currentM(3,0),currentM(3,1),currentM(3,2) };
		for (unsigned int i = 0; i < count; i++) {
			status = poseInputArray.jumpToArrayElement(i);
			MMatrix tempM = poseInputArray.inputValue(&status).child(poseInputMatrixObj).asMatrix();
			MVector tempV0 = { tempM(3,0),tempM(3,1),tempM(3,2) };

			currentDisMatrix(0, i) = RBF_Solver_Compute::distance(tempV0, currentV);
		}

		// normalize the currentDisMatrix
		currentNormDisMatrix = RBF_Solver_Compute::normalizeMatrix(currentDisMatrix);

		outputWeightMatrix = currentNormDisMatrix* (inverseFeatureMatrix * inputValueMatrix);
		
		// output the result
		MArrayDataHandle productArray = data.outputValue(outputValueObj);
		for (unsigned int i = 0; i < count; i++) {
			productArray.jumpToArrayElement(i);
			MDataHandle val = productArray.outputValue();
			val.setDouble(outputWeightMatrix(0, i));
		}

		data.setClean(plug);
	}
	

	

	return(MS::kSuccess);
}


//static methods
void* RBF_Solver_Node::Creator()
{
	return(new RBF_Solver_Node());
}

MStatus RBF_Solver_Node::Initialize()
{   
	MStatus status;
	//create inputs

	MFnMatrixAttribute matrixAttr;
	poseInputMatrixObj = matrixAttr.create("worldMatrix", "WM", MFnMatrixAttribute::kDouble, &status);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);
	matrixAttr.setDisconnectBehavior(MFnAttribute::kDelete);

	MFnNumericAttribute numericAttr;
    poseInputValueObj = numericAttr.create("value", "value", MFnNumericData::kDouble,1, &status);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);
	numericAttr.setDisconnectBehavior(MFnAttribute::kDelete);


	currentPositionObj = matrixAttr.create("currentPosition", "currentPos", MFnMatrixAttribute::kDouble, &status);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);

	MFnCompoundAttribute compoundAttr;
	poseInputObj = compoundAttr.create("poseInput", "poseInput", &status);
	compoundAttr.setKeyable(true);
	compoundAttr.setReadable(false);
	compoundAttr.setArray(true);
	compoundAttr.setUsesArrayDataBuilder(true);
	compoundAttr.setDisconnectBehavior(MFnAttribute::kDelete);

	//add child attributes
	compoundAttr.addChild(poseInputMatrixObj);
	compoundAttr.addChild(poseInputValueObj);


	//create outputs
	MFnNumericAttribute numericAttrOut;
	outputValueObj = numericAttrOut.create("outputValue", "OV", MFnNumericData::kDouble);
	numericAttrOut.setReadable(true);
	numericAttrOut.setWritable(false);
	numericAttrOut.setArray(true);
	numericAttrOut.setUsesArrayDataBuilder(true);
	numericAttrOut.setDisconnectBehavior(MFnAttribute::kDelete);


	//add attributes and affect relationships
	addAttribute(currentPositionObj);
	addAttribute(poseInputObj);
	addAttribute(outputValueObj);

	attributeAffects(currentPositionObj, outputValueObj);
	attributeAffects(poseInputObj, outputValueObj);
	attributeAffects(poseInputMatrixObj, outputValueObj);
	attributeAffects(poseInputValueObj, outputValueObj);
	
	return(MS::kSuccess);
}




MTypeId RBF_Solver_Node::GetTypeId()
{
	return TypeID;
}

MString RBF_Solver_Node::GetTypeName()
{
	return TypeName;
}





