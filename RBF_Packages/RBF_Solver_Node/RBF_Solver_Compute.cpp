#include <maya/MMatrix.h>
#include <maya/MGlobal.h>
#include<maya/MVector.h>
#include"RBF_Solver_Node.h"
#include"RBF_Solver_Compute.h"
#include <Eigen/Dense>

using namespace Eigen;




double RBF_Solver_Compute::distance(MVector vector1, MVector vector2)
{
	return sqrt((vector1.x - vector2.x) * (vector1.x - vector2.x) + (vector1.y - vector2.y) * (vector1.y - vector2.y) + (vector1.z - vector2.z) * (vector1.z - vector2.z));
}

int RBF_Solver_Compute::Max(int input0, int input1)
{
	int output;
	if (input0 <= input1)
		output = input1;
	else
		output = input0;
	
	return output;
}

MatrixXd RBF_Solver_Compute::normalizeMatrix(const MatrixXd& inputMatrix)
{
    // Get the number of rows in the input matrix (number of data points)
    int rows = inputMatrix.rows();

    // Create a temporary matrix of the same size as the input matrix
    MatrixXd normalizedMatrix = inputMatrix;

    // Normalize each row vector
    for (int i = 0; i < rows; ++i) {
        // Calculate the norm of the current row vector
        double norm = inputMatrix.row(i).norm();
        // If the norm is not zero, normalize the current row vector
        if (norm != 0.0) {
            normalizedMatrix.row(i) /= norm;
        }
    }

    return normalizedMatrix; // Return the normalized matrix
}


