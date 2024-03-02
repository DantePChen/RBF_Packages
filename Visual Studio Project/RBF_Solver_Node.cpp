#include<maya/MFnNumericAttribute.h>
#include<maya/MFnMatrixAttribute.h>
#include<maya/MFnDependencyNode.h>
#include<maya/MPlug.h>
#include<maya/MGlobal.h>
#include<maya/MMatrix.h>
#include<maya/MVector.h>
#include<maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MDataBlock.h>
#include <maya/MTransformationMatrix.h>
#include"RBF_Solver_Compute.h"
#include"RBF_Solver_Node.h"
#include"Eigen/Dense"

using namespace Eigen;


//constants
static const MTypeId TypeID = MTypeId(0x573600);
static const MString TypeName = "RBF_Solver";

//variables
 MObject RBF_Solver_Node::currentPositionObj;
 MObject RBF_Solver_Node::outputValueObj;
 MObject RBF_Solver_Node::poseInputMatrixObj;
 MObject RBF_Solver_Node::poseInputValueObj;

 

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
		MArrayDataHandle poseInputMArray = data.inputValue(poseInputMatrixObj, &status);
		MArrayDataHandle poseInputVArray = data.inputValue(poseInputValueObj, &status);
		MDataHandle currentPositionHandle = data.inputValue(currentPositionObj, &status);

		//MVector currentPositionVector = RBF_Solver_Compute::extractPositionVectorFromMatrix(currentPositionHandle);

		//get the max count of two inputs to protect the next steps,especilly when create the Matrix
		unsigned int count = RBF_Solver_Compute::Max(poseInputMArray.elementCount(), poseInputVArray.elementCount());
		unsigned int Mcount = poseInputMArray.elementCount();
		unsigned int Vcount = poseInputVArray.elementCount();
		//initialize the matrix for next steps
		MatrixXd distanceMatrix = MatrixXd::Zero(count, count);
		MatrixXd featureMatrix = MatrixXd::Zero(count, count);
		MatrixXd inverseFeatureMatrix = MatrixXd::Zero(count, count);
		MatrixXd inputValueMatrix = MatrixXd::Zero(count, count);
		MatrixXd currentDisMatrix = MatrixXd::Zero(1, count);
		MatrixXd currentNormDisMatrix = MatrixXd::Zero(1, count);
		MatrixXd outputWeightMatrix = MatrixXd::Zero(1, count);


		// get the distanceMatrix
		if (Mcount ==count ) {
			for (unsigned int i = 0; i < count; i++) {
				for (unsigned int j = 0; j < count; j++)
				{
					status = poseInputMArray.jumpToArrayElement(i);
					MMatrix tempM = poseInputMArray.inputValue(&status).asMatrix();
					MVector tempV0 = { tempM(3,0),tempM(3,1),tempM(3,2) };

					status = poseInputMArray.jumpToArrayElement(j);
					tempM = poseInputMArray.inputValue(&status).asMatrix();
					MVector tempV1 = { tempM(3,0),tempM(3,1),tempM(3,2) };

					distanceMatrix(i, j) = RBF_Solver_Compute::distance(tempV0, tempV1);
				}
			}
		}
		else {
			/*The protection mechanism ensures that this section of computation occurs only
			when 'Mcount' equals 'count', thereby preventing crashes due to reading non-existent data.*/
		}
		
		
		// normallize the distanceMatrix to get the featureMatrix
		featureMatrix = RBF_Solver_Compute::normalizeMatrix(distanceMatrix);

		// get the inverse featureMatrix
		inverseFeatureMatrix = featureMatrix.inverse();

		//get the inputValueMatrix
		if (Vcount == count) {
			for (unsigned int i = 0; i < count; i++) {
				for (unsigned int j = 0; j < count; j++) {
					if (i == j) {
						status = poseInputVArray.jumpToArrayElement(i);
						double tempValue = poseInputVArray.inputValue(&status).asDouble();
						inputValueMatrix(i, j) = tempValue;
					}
					else {
						inputValueMatrix(i, j) = 0;
					}
				}

			}
		}
		else {
			//same as before, The protection mechanism
		}
		
		// compute the distance matrix
		if (Mcount == count) {
			MMatrix currentM = currentPositionHandle.asMatrix();
			MVector currentV = { currentM(3,0),currentM(3,1),currentM(3,2) };
			for (unsigned int i = 0; i < count; i++) {
				status = poseInputMArray.jumpToArrayElement(i);
				MMatrix tempM = poseInputMArray.inputValue(&status).asMatrix();
				MVector tempV0 = { tempM(3,0),tempM(3,1),tempM(3,2) };

				currentDisMatrix(0, i) = RBF_Solver_Compute::distance(tempV0, currentV);
			}
		}
		else {

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
	//create inputs
	MFnMatrixAttribute matrixAttr;
	poseInputMatrixObj = matrixAttr.create("WorldMatrix", "WM", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);
	matrixAttr.setArray(true);
	matrixAttr.setUsesArrayDataBuilder(true);

	MFnNumericAttribute numericAttr;
    poseInputValueObj = numericAttr.create("Value", "Value", MFnNumericData::kDouble,1);
	numericAttr.setKeyable(true);
	numericAttr.setReadable(false);
	numericAttr.setArray(true);
	numericAttr.setUsesArrayDataBuilder(true);


	currentPositionObj = matrixAttr.create("currentPosition", "currentPos", MFnMatrixAttribute::kDouble);
	matrixAttr.setKeyable(true);
	matrixAttr.setReadable(false);


	//create outputs
	outputValueObj = numericAttr.create("outputValue", "OV", MFnNumericData::kDouble);
	numericAttr.setWritable(false);
	numericAttr.setArray(true);
	numericAttr.setUsesArrayDataBuilder(true);

	//add attributes and affect relationships
	addAttribute(currentPositionObj);
	addAttribute(poseInputMatrixObj);
	addAttribute(poseInputValueObj);
	addAttribute(outputValueObj);

	attributeAffects(currentPositionObj, outputValueObj);
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





