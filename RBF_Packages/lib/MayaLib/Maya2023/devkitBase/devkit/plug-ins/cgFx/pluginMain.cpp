//
// Copyright (C) 2002-2004 NVIDIA
//
// File: pluginMain.cpp
//
// Author: Jim Atkinson
//
// Changes:
//  10/2003  Kurt Harriman - www.octopusgraphics.com +1-415-893-1023
//           - "-pluginPath/pp" flag of cgfxShader command returns the
//             full path of the "cgfxShader" subdirectory beneath the
//             directory from which the plug-in binary was loaded.
//             Supporting files such as MEL scripts can be loaded from
//             this directory to avoid inadvertently picking up wrong
//             versions from random directories on the search path.
//           - The plug-in executes the cgfxShader_initUI.mel script
//             from this directory at the time the plug-in is loaded.
//           - The MEL command `pluginInfo -q -version cgfxShader`
//             returns the plug-in version and cgfxShaderNode.cpp
//             compile date.
//  11/2003  Kurt Harriman - www.octopusgraphics.com +1-415-893-1023
//           - To load or reload a CgFX file, use the cgfxShader command
//             "-fx/fxFile <filename>" flag.  Setting the cgfxShader
//             node's "s/shader" attribute no longer loads the file.
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

////////////////////////////////////////////////////////////////////////
// PLUG-IN DESCRIPTION:
// 
// CgFx Shader (Windows-only)
//
// The plug-in is named "cgfxShader.mll". It defines one node named "cgfxShader" and
// one command, which is also named "cgfxShader". The command is used to manipulate the node.
// This is very similar to the expression command and node.
//
// The "cgfxShader" node is a hardware shader (derived from MPxHwShaderNode).
// 
// By default, the shader or "s" attribute is the only thing visible on a "cgfxShader" node.
// You can set this attribute to the name of a .fx file and many interesting effects can occur.
// A .fx file holds CgFX effect definitions. This effect completely controls how the drawing is done.
// Therefore, if you load a different .fx file, you get a completely different effect on the screen.
// In theory, the "cgfxShader" node can do anything that any other hardware shader node can do.
//
// Along with the effect, the CgFX file provides a set of parameters that can be modified to change
// the effect in controlled ways. For example, a glow effect might allow you to set the color, intensity,
// and size of the glow. A bumpy, shiny effect might allow you to change the bump texture map.
// All of these parameters are exposed as dynamic attributes on the shader node.
// If the effect has a "glowColor" parameter, the "cgfxShader" node will have a dynamic color attribute
// named "glowColor". Changes to the attribute affect the parameter.
//
// You can specify the .fx file using the Attribute Editor or "cgfxShader" command. All the dynamic attributes
// that no longer apply are removed and all the attributes that are needed by the new effect are added.
// You can see all the changes in the Attribute Editor.
//
// The syntax of the command is:
// cgfxShader [-e] [-fx fileName] [-n name] [nodeName]
//
//	-e
//	Edit an existing shader. If -e is not specified, a new "cgfxShader" node is created.
// 
//	-fx filename
//	Set the .fx file for the "cgfxShader" node.
//
//	-n name 
//	Sets the name of the shader node to create (create only).
//
//	nodeName
//	The name of the node to edit. You do not need to specify this if the shader node is currently selected. 
// 
// The easiest way to use the shaders is through the Hypershade window. Load the plug-in before opening the
// HyperShade window and then select Create > Material > Cgfx Shader.
// You can then simply drag the material onto a shape to assign it.
//
// Please note that the legacy viewport implementation is provided for backwards compatibility only. It uses
// deprecated Maya API methods and should not be used as an example for implementing custom new
// MPxHwShaderNode plugins.
//
////////////////////////////////////////////////////////////////////////

#include "cgfxShaderCommon.h"

#include "cgfxShaderNode.h"
#include "cgfxProfile.h"
#include "cgfxShaderCmd.h"
#include "cgfxTextureCache.h"
#include "cgfxVector.h"

#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MSceneMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MGlobal.h>
#include <maya/MFileIO.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MDGMessage.h>

#if defined(_SWATCH_RENDERING_SUPPORTED_)
	#include <maya/MHWShaderSwatchGenerator.h>
	#include <maya/MHardwareRenderer.h>
	#include <maya/MViewport2Renderer.h>
#endif


// viewport 2.0
#include <maya/MDrawRegistry.h>


// callbackIds is an array of callback identifiers which need to be
// cancelled when the plug-in is unloaded.
//
static MCallbackIdArray callbackIdArray;

static void cgfxShaderFileSaveCB(void* clientData );

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
	MString   sVer = cgfxShaderNode::getPluginVersion();

#if defined(_SWATCH_RENDERING_SUPPORTED_)
    	// Allow an environment variable to override usage of swatch rendering.
	// Set the environment variable to a value other than 0 for it to take effect.
	const char *cgfxEnvVar = getenv("CGFX_SWATCH_RENDERING");

	MString UserClassify = MString( "shader/surface/utility:drawdb/shader/surface/cgfxShader" );

	// Don't initialize swatches in batch mode
	if (MGlobal::mayaState() != MGlobal::kBatch)
	{
		const MString& swatchName =	MHWShaderSwatchGenerator::initialize();

#ifdef _WIN32
	    if (!cgfxEnvVar)
			UserClassify = MString( "shader/surface/utility/:drawdb/shader/surface/cgfxShader:swatch/"+swatchName );
	    else
	    {
	        if ( 0 != strcmp(cgfxEnvVar,"0") )
				UserClassify = MString( "shader/surface/utility/:drawdb/shader/surface/cgfxShader:swatch/"+swatchName );
	    }
#else
	    if ( cgfxEnvVar && ( 0 != strcmp(cgfxEnvVar,"0") ) )
	    {
			UserClassify = MString( "shader/surface/utility/:drawdb/shader/surface/cgfxShader:swatch/"+swatchName );
	    }
#endif
	}

#else
	const MString UserClassify( "shader/surface/utility:drawdb/shader/surface/cgfxShader" );
#endif

	MFnPlugin plugin( obj, "NVIDIA", sVer.asChar(), MApiVersion );

	// Register/initialize localized string resources
	CHECK_MSTATUS( plugin.registerUIStrings(NULL,
				   "cgfxShaderPluginInitStrings" ));

#if defined(_SWATCH_RENDERING_SUPPORTED_)
	// The following code is only used on Maya versions 7.0 and
    // later.
	MStatus status = MStatus::kFailure;
	if (MGlobal::mayaState() == MGlobal::kInteractive)
	{
			// Deprecated Methods: MHardwareRenderer::theRenderer(), pRenderer->backEndString()
			_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
			MHardwareRenderer *pRenderer = MHardwareRenderer::theRenderer();

			if (pRenderer) {
				const MString & backEndStr = pRenderer->backEndString();
				unsigned int width = 64, height = 64;
				status = pRenderer->makeSwatchContextCurrent( backEndStr,
					width,
					height );
			}
			if (status != MStatus::kSuccess) {
				MGlobal::displayError(MString("Unqualified video card : Offscreen contexts not supported. CgFx plugin will not be loaded."));
				return MStatus::kFailure;
			}
			_OPENMAYA_POP_WARNING
	}
#else
#error "CgFx requires the Maya version 7 or greater"
#endif

    cgfxTextureCache::initialize();
    
	// Create Cg Context & register the Cg error callback
	cgfxShaderNode::sCgContext = cgCreateContext();
	cgSetErrorCallback(cgfxShaderNode::cgErrorCallBack);
	cgSetErrorHandler(cgfxShaderNode::cgErrorHandler, 0);
	cgGLRegisterStates(cgfxShaderNode::sCgContext);
	cgGLSetManageTextureParameters(cgfxShaderNode::sCgContext, CG_TRUE);

    cgSetAutoCompile(cgfxShaderNode::sCgContext, CG_COMPILE_LAZY);      
    cgSetLockingPolicy(CG_NO_LOCKS_POLICY);
    cgGLSetDebugMode(CG_FALSE);

    cgfxProfile::initialize();
    if (cgfxProfile::getBestProfile() == NULL) {
        MGlobal::displayError(
            MString("No supported Cg profiles were found. CgFx plugin will not be loaded.")
        );
        return MStatus::kFailure;
    }

    {
      // display banner
      MString s;
      s += sVer;
      MGlobal::displayInfo(s);
    }
    
    CHECK_MSTATUS( plugin.registerNode("cgfxShader",
                                       cgfxShaderNode::sId,
                                       cgfxShaderNode::creator,
                                       cgfxShaderNode::initialize,
                                       MPxNode::kHwShaderNode,
                                       &UserClassify));

	CHECK_MSTATUS( plugin.registerNode("cgfxVector",
		cgfxVector::sId,
		cgfxVector::creator,
		cgfxVector::initialize));

	CHECK_MSTATUS( plugin.registerCommand("cgfxShader",
		cgfxShaderCmd::creator,
		cgfxShaderCmd::newSyntax));

	// Register a shader override for this node
	CHECK_MSTATUS(MHWRender::MDrawRegistry::registerShaderOverrideCreator(
		cgfxShaderOverride::drawDbClassification,
		cgfxShaderOverride::drawRegistrantId,
		cgfxShaderOverride::Creator));

	// Where are my MEL scripts?
	cgfxShaderCmd::sPluginPath = plugin.loadPath();

	// Run MEL script for user interface initialization.
	if (MGlobal::mayaState() == MGlobal::kInteractive)
	{
		MString sCmd = "evalDeferred \"source \\\"cgfxShader_initUI.mel\\\"\"";
		MGlobal::executeCommand( sCmd );
	}

	// Skip the status checking on the addCallback calls since the only
	// way that they can really fail is if Maya is out of memory and then
	// everything is going to fall apart anyway.
	//
	// call backs for "source directory" removal...
	MCallbackId id = MSceneMessage::addCallback(MSceneMessage::kBeforeSave,
		cgfxShaderFileSaveCB, NULL, &status);
	CHECK_MSTATUS(status);
	callbackIdArray.append(id);

	id = MSceneMessage::addCallback(MSceneMessage::kBeforeExport,
		cgfxShaderFileSaveCB, NULL, &status);
	CHECK_MSTATUS(status);
	callbackIdArray.append(id);

	return MStatus::kSuccess;
}

MStatus uninitializePlugin( MObject obj)
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

	cgDestroyContext(cgfxShaderNode::sCgContext);
    cgfxProfile::uninitialize();
    cgfxTextureCache::uninitialize();
    
	// Remove all the callbacks that we registered.
	//
	MMessage::removeCallbacks(callbackIdArray);

	// Deregister our node types.
	//
	CHECK_MSTATUS(plugin.deregisterNode( cgfxShaderNode::sId ));
	CHECK_MSTATUS(plugin.deregisterNode( cgfxVector::sId ));

	// Deregister our commands.
	//
	CHECK_MSTATUS(plugin.deregisterCommand( "cgfxShader" ));

	// Deregister the override
	CHECK_MSTATUS(MHWRender::MDrawRegistry::deregisterShaderOverrideCreator(
		cgfxShaderOverride::drawDbClassification, cgfxShaderOverride::drawRegistrantId));

	return MStatus::kSuccess;
}

static void cgfxShaderFileSaveCB(void* clientData )
{
	// Look through the scene for cgfxShader nodes whose effect is NULL
	// but whose shader attribute is not empty.
	//
	MStatus status;

	MString workspace;
	status = MGlobal::executeCommand(MString("workspace -q -rd;"),
		workspace);
	if (!status)
	{
		workspace.clear();
	}

	MItDependencyNodes nodeIt;

	for (nodeIt.reset(MFn::kPluginHwShaderNode);
		!nodeIt.isDone();
		nodeIt.next())
	{
		MObject oNode = nodeIt.thisNode();

		MFnDependencyNode fnNode(oNode);
		if (fnNode.typeId() == cgfxShaderNode::sId)
		{
			// We've got a winner.
			//
			cgfxShaderNode* pNode = (cgfxShaderNode*)fnNode.userNode();
			MString ShaderFxFile = pNode->shaderFxFile();
			if (strncmp(ShaderFxFile.asChar(),workspace.asChar(),workspace.length()) == 0)
			{
				ShaderFxFile = ShaderFxFile.substring(workspace.length(),ShaderFxFile.length() - 1);
				MPlug plShader = fnNode.findPlug( pNode->sShader, true );
				plShader.setValue( ShaderFxFile );
				OutputDebugString("CGFX shader pathname saved as: ");
				OutputDebugString(ShaderFxFile.asChar());
				OutputDebugString("\n");
			}

			if( pNode->getTexturesByName())
			{
				cgfxAttrDefList::iterator it(pNode->attrDefList());

				while (it)
				{
					cgfxAttrDef* aDef = (*it);

					MObject oNode = pNode->thisMObject();

					switch (aDef->fType)
					{
					case cgfxAttrDef::kAttrTypeColor1DTexture:
					case cgfxAttrDef::kAttrTypeColor2DTexture:
					case cgfxAttrDef::kAttrTypeColor3DTexture:
					case cgfxAttrDef::kAttrTypeColor2DRectTexture:
					case cgfxAttrDef::kAttrTypeNormalTexture:
					case cgfxAttrDef::kAttrTypeBumpTexture:
					case cgfxAttrDef::kAttrTypeCubeTexture:
					case cgfxAttrDef::kAttrTypeEnvTexture:
					case cgfxAttrDef::kAttrTypeNormalizationTexture:
						{
							MString pathname;
							aDef->getValue(oNode, pathname);
							if (strncmp(pathname.asChar(),workspace.asChar(),workspace.length()) == 0)
							{
								pathname = pathname.substring(workspace.length(),pathname.length());
								aDef->setValue(oNode, pathname);
								OutputDebugString("CGFX texture pathname saved as: ");
								OutputDebugString(pathname.asChar());
								OutputDebugString("\n");
							}
						}
						break;
					default:
						break;
					}
					++it;
				}
			}
		}
	}
}

