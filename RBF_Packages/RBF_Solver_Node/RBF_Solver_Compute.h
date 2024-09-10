#pragma once
#include <maya/MMatrix.h>
#include <maya/MGlobal.h>
#include<maya/MVector.h>
#include<maya/MDataHandle.h>
#include"RBF_Solver_Node.h"
#include <Eigen/Dense>

using namespace Eigen;



class RBF_Solver_Compute : public RBF_Solver_Node
{
public:
	
	static double distance(MVector vector1, MVector vector2);

	static int Max(int input0, int input1);

	static MatrixXd normalizeMatrix(const MatrixXd& inputMatrix);
};