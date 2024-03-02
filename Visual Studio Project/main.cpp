#include<maya/MFnPlugin.h>
#include<maya/MGlobal.h>
#include"RBF_Solver_Node.h"

// entrance of plugin, initialize the plugin
MStatus initializePlugin(MObject pluginObj)
{
#ifdef _Debug
	MGlobal::displayInfo("initialize the plugin");
#endif

	//write the vendor and version 
	const char* vendor = "Dante Chen";
	const char* version = "1.0";
	/*create a const tell maya this is a plugin not need specific MayaAPI, this is also a
	requred paramter in MFnPlugin method
	*/
	const char* requiredApiVersion = "Any";

	//create a value to show status
	MStatus status;
	// initialize the plugin
	MFnPlugin pluginfn(pluginObj, vendor, version, requiredApiVersion, &status);

	//check the status of initializing the plugin
	if (!status)
	{
		MGlobal::displayError("Failed to initialize the plugin:" + status.errorString());
		return(status);
	}

	status = pluginfn.registerNode(RBF_Solver_Node::GetTypeName(),
		                           RBF_Solver_Node::GetTypeId(),
		                           RBF_Solver_Node::Creator,
								   RBF_Solver_Node::Initialize,
								   RBF_Solver_Node::kDependNode);

	if (!status)
	{
		MGlobal::displayError("Failed to register the node:" + status.errorString());
		return(status);
	}

	return(status);
}

// exit of plugin, unitialize the plugin
MStatus uninitializePlugin(MObject pluginObj)
{
	// check the status of unintialize the plugin
	MStatus status;

	MFnPlugin pluginfn(pluginObj);

	status = pluginfn.deregisterNode(RBF_Solver_Node::GetTypeId());

	if (!status)
	{
		MGlobal::displayError("Failed to deregister the node:" + status.errorString());
		return(status);
	}

	return(status);
}