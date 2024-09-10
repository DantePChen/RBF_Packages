//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
//
// How to use:
// This plugin is an example plug-in. Its purpose is to show how to use
// custom blind data in the hardware shader node. To try out the plugin,
// you can run the provided mel script: "blindDataShader.mel" after manually
// loading the plugin into memory.
//
// How it work:
// This plug-in produces two dependency graph nodes: "blindDataShader" and "blindDataMesh".
// 
//
// The "blindDataMesh" node builds a mesh and populates its blind data with color information.
// The "blindDataShader" node is a hardware shader that is associated with a shader override (blindDataShaderOverride).
// The association between the shader node and the shader override is done by the draw classifications string "drawdb/shader/surface/blindDataShaderOverride".
// The blindDataShaderOverride class draw the geometry with a color per vertex shader (k3dCPVSolidShader) and define the custom vertex buffer stream that will be required
// for the color data. The custom stream name is "blindDataColorStream" and a vertex buffer generator (blindDataMeshVertexBufferGenerator) is registried with the same name.
// The blindDataMeshVertexBufferGenerator is automaticly called when required to fill a vertex buffer with color data that are fetched from the mesh blind data.
//
// The results can be viewed by using the shading menu to smooth shade and then turn on hardware texturing. 
//
////////////////////////////////////////////////////////////////////////


#include "blindDataMesh.h"
#include "blindDataShader.h"
#include "blindDataShaderOverride.h"
#include "blindDataMeshVertexBufferGenerator.h"

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>


namespace
{
	const char* gShaderOverrideDrawClassification = "drawdb/shader/surface/blindDataShaderOverride";
	const char* gShaderOverrideRegistrantId       = "blindDataShaderOverride";
}



MStatus initializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is loaded into Maya.  It 
//		registers all of the services that this plug-in provides with 
//		Maya.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
	MStatus   status;
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "5.0", "Any");
	MString   userClassify( "shader/surface/utility:" );
	userClassify += gShaderOverrideDrawClassification;

	status = plugin.registerNode( "blindDataShader",
								  blindDataShader::id, 
			                      blindDataShader::creator,
								  blindDataShader::initialize,
								  MPxNode::kHwShaderNode,
								  &userClassify);
	if (!status) {
		status.perror("registerNode");
		return status;
	}


	// Register a shader override (Only use by VP2.)
	status = MHWRender::MDrawRegistry::registerShaderOverrideCreator(
		        gShaderOverrideDrawClassification,
		        gShaderOverrideRegistrantId,
				blindDataShaderOverride::Creator);
	if (!status) {
		status.perror("registerShaderOverrideCreator failed !");
		return status;
	}

	status = plugin.registerNode( "blindDataMesh",
								  blindDataMesh::id,
								  blindDataMesh::creator,
								  blindDataMesh::initialize );
	if (!status) {
		status.perror("registerNode");
		return status;
	}


	// Register the Vertex Buffer Generator that will fill a buffer stream with vertex colors from the mesh blind data.
	// Only use by VP2.
	status = MDrawRegistry::registerVertexBufferGenerator("blindDataColorStream", blindDataMeshVertexBufferGenerator::Creator);
	if (!status) {
		status.perror("registerVertexBufferGenerator failed !");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
//
//	Description:
//		this method is called when the plug-in is unloaded from Maya. It 
//		deregisters all of the services that it was providing.
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status = plugin.deregisterNode( blindDataMesh::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	status = plugin.deregisterNode( blindDataShader::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::deregisterShaderOverrideCreator(gShaderOverrideDrawClassification, gShaderOverrideRegistrantId);
	if (!status) {
		status.perror("deregisterShaderOverrideCreator failed !");
		return status;
	}

	status = MDrawRegistry::deregisterVertexBufferGenerator("blindDataColorStream");
	if (!status) {
		status.perror("deregisterVertexBufferGenerator failed !");
		return status;
	}

	return status;
}
