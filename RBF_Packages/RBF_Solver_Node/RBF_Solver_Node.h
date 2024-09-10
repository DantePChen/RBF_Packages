#pragma once
#include<maya/MPxNode.h>

// create a class of RBF_Solver_Node
class RBF_Solver_Node : public MPxNode
{
public:
	// constructor and destructor
	RBF_Solver_Node();
	
	virtual ~RBF_Solver_Node() override;

	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

	// static methods
	static void* Creator();

	static MStatus Initialize();

	static MTypeId GetTypeId();
	static MString GetTypeName();


private:
	// input attributes
	static MObject RBFKernals;
	static MObject currentPositionObj;
	static MObject poseInputObj;
	static MObject poseInputMatrixObj;
	static MObject poseInputValueObj;

	//output attributes
	static MObject outputValueObj;


};

