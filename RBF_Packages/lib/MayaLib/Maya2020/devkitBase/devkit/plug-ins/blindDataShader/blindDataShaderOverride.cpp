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
#include "blindDataShaderOverride.h"
#include <maya/MShaderManager.h>
#include <maya/MDrawContext.h>




blindDataShaderOverride::blindDataShaderOverride(const MObject& obj)
    : MHWRender::MPxShaderOverride(obj)
{
	MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	// Get a built-in shader that use vertex position and per vertex color as input.
	fCPVShaderInstance = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dCPVSolidShader); 
}



blindDataShaderOverride::~blindDataShaderOverride()
{
	MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	if(fCPVShaderInstance)
		shaderMgr->releaseShader(fCPVShaderInstance);
}



MString blindDataShaderOverride::initialize(const MInitContext& initContext, MInitFeedback& initFeedback)
{
	//
	// Define the geometry requirement (Vertex shader input layout) that is required by the shader k3dCPVSolidShader.
	//
	MHWRender::MVertexBufferDescriptor positionDesc(MString(), MHWRender::MGeometry::kPosition, MHWRender::MGeometry::kFloat, 3);

	MHWRender::MVertexBufferDescriptor colorDesc(MString(), MHWRender::MGeometry::kColor, MHWRender::MGeometry::kFloat, 4);
	
	colorDesc.setSemanticName("blindDataColorStream"); // Here we set the name of the vertex buffer generator that will generate the color stream. (See initializePlugin())

	addGeometryRequirement(positionDesc);
	addGeometryRequirement(colorDesc);

	return MString("Autodesk Maya blindDataShaderOverride");
}



MHWRender::DrawAPI blindDataShaderOverride::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}



MHWRender::MShaderInstance* blindDataShaderOverride::shaderInstance(MDrawContext& /*context*/) const
{
	return fCPVShaderInstance;
}



void blindDataShaderOverride::activateKey(MDrawContext& context, const MString& key)
{
	if(fCPVShaderInstance)
		fCPVShaderInstance->bind(context);
}



void blindDataShaderOverride::terminateKey(MDrawContext& context, const MString& key)
{
	if(fCPVShaderInstance)
		fCPVShaderInstance->unbind(context);
}



bool blindDataShaderOverride::handlesDraw(MHWRender::MDrawContext& context)
{
	const MHWRender::MPassContext & passCtx = context.getPassContext();
	const MStringArray & passSem = passCtx.passSemantics();

	bool handlePass = false;

	for (unsigned int i = 0; i<passSem.length(); i++)
	{
		if (passSem[i] == MHWRender::MPassContext::kColorPassSemantic)
		{
			bool hasOverrideShader = passCtx.hasShaderOverride();
			if (!hasOverrideShader)
			{
				handlePass = true;
			}
		}
	}

	return handlePass;
}



bool blindDataShaderOverride::draw(MDrawContext& context, const MRenderItemList& renderItemList)const
{
	if(fCPVShaderInstance)
	{
		const unsigned int passCount = fCPVShaderInstance->getPassCount(context);
		for (unsigned int i = 0; i < passCount; i++)
		{
			fCPVShaderInstance->activatePass(context, i);
			MHWRender::MPxShaderOverride::drawGeometry(context);
		}
	}
	return true;
}
