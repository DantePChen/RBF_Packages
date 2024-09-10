//-
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#include "customTextureShaderOverride.h"
#include "customTextureShader.h"

#include <maya/MString.h>
#include <maya/MDrawContext.h>
#include <maya/MGlobal.h>

customTextureShaderOverride::customTextureShaderOverride(const MObject& obj)
: MHWRender::MPxShaderOverride(obj)
{
	m_beautyShaderInstance = NULL;
    m_multiDrawBeautyShaderInstance = NULL;
	m_shaderBound = false;
	m_inBeautyPass = false;

	// init custom texture data:
	m_customTextureData = NULL;

	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr)
		return;

	// Tell Maya to get our shader
	//
	// The plugin assumes that the devkit location follows "Setting up your build
	// environment" at Maya Developer Help; otherwise, shaders/textures cannot be
	// located. In this case create a mod (module description file) as below in a
	// suitable modules folder (getenv "MAYA_MODULE_PATH") and restart Maya.
	//
	//   + devkit 1.0 <local devkit path>
	//
	MString shaderLocation;
	if( !MGlobal::executeCommand(MString("getModulePath -moduleName \"devkit\""), shaderLocation, false) ) {
		shaderLocation = MString(getenv("MAYA_LOCATION")) + MString("/devkit");
	}
	shaderLocation += MString("/plug-ins/customTextureShader/CustomTextureBlock");
	m_beautyShaderInstance = shaderMgr->getEffectsFileShader(shaderLocation, "Main");

    // Load the MultiDraw alternate version of the shader
    shaderLocation += MString("MultiDraw");
	m_multiDrawBeautyShaderInstance = shaderMgr->getEffectsFileShader(shaderLocation, "Main");
}

MHWRender::MPxShaderOverride* customTextureShaderOverride::Creator(const MObject& obj)
{
	return new customTextureShaderOverride(obj);
}

customTextureShaderOverride::~customTextureShaderOverride()
{
	if (m_beautyShaderInstance || m_multiDrawBeautyShaderInstance)
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
			if (shaderMgr)
			{
                if (m_beautyShaderInstance)
                {
				    shaderMgr->releaseShader(m_beautyShaderInstance);
				    m_beautyShaderInstance = NULL;
                }
                if (m_multiDrawBeautyShaderInstance)
                {
				    shaderMgr->releaseShader(m_multiDrawBeautyShaderInstance);
				    m_multiDrawBeautyShaderInstance = NULL;
                }
			}
		}
	}

	// clean custom texture data:
	delete m_customTextureData;
	m_customTextureData = NULL;
}

MString customTextureShaderOverride::initialize(const MInitContext& initContext, MInitFeedback& initFeedback)
{
	// Set the geometry requirements for drawing.
	MString empty;

	MHWRender::MVertexBufferDescriptor positionDesc(
		empty,
		MHWRender::MGeometry::kPosition,
		MHWRender::MGeometry::kFloat,
		3);

	MHWRender::MVertexBufferDescriptor uvDesc(
		empty,
		MHWRender::MGeometry::kTexture,
		MHWRender::MGeometry::kFloat,
		2);

	addGeometryRequirement(positionDesc);
	addGeometryRequirement(uvDesc);

	if (m_beautyShaderInstance)
	{
		addShaderSignature(*m_beautyShaderInstance);
	}

	if (m_multiDrawBeautyShaderInstance)
	{
		addShaderSignature(*m_multiDrawBeautyShaderInstance);
	}

	return MString("customTextureShaderOverride");
}

MHWRender::MShaderInstance* customTextureShaderOverride::shaderInstance(MHWRender::MDrawContext& context) const
{
	if (m_inBeautyPass)
	{
        if (context.isMultiDraw())
        {
            return m_multiDrawBeautyShaderInstance;
        }
        else
        {
		    return m_beautyShaderInstance;
        }
	}

	return NULL;
}

void customTextureShaderOverride::updateDG(MObject object)
{
}

void customTextureShaderOverride::updateDevice()
{
	// Set parameters:
	updateShaderInstance();

	// build that custom data texture here and bind to shader:
	buildAndUpdateCustomDataTextureViaMaya();
}

void customTextureShaderOverride::endUpdate()
{
}

bool customTextureShaderOverride::handlesDraw(MHWRender::MDrawContext& context)
{
	const MHWRender::MPassContext& passCtx = context.getPassContext();
	const MStringArray& passSem = passCtx.passSemantics();

	m_inBeautyPass = false;

	// For color passes, only handle if there isn't already
	// a global override. This is the same as the default
	// logic for this method in MPxShaderOverride
	bool handlePass = false;
	for (unsigned int i=0; i<passSem.length(); i++)
	{
		if (passSem[i] == MHWRender::MPassContext::kColorPassSemantic)
		{
			bool hasOverrideShader = passCtx.hasShaderOverride();
			if (!hasOverrideShader)
				handlePass = true;

			m_inBeautyPass = true;
		}

		// If these semantics are specified then they override
		// the color pass semantic handling.
		else if (passSem[i] == MHWRender::MPassContext::kDepthPassSemantic ||
				 passSem[i] == MHWRender::MPassContext::kNormalDepthPassSemantic)
		{
			handlePass = false;
		}
	}

	return handlePass;
}

void customTextureShaderOverride::activateKey(MHWRender::MDrawContext& context, const MString& /*key*/)
{
    m_inMultiDrawMode = false;

	if (m_inBeautyPass)
	{
        m_inMultiDrawMode = context.isMultiDraw();

        if (m_inMultiDrawMode && m_multiDrawBeautyShaderInstance)
        {
		    m_multiDrawBeautyShaderInstance->bind(context);	// bind the MultiDraw version of the shader
        }
        else if (!m_inMultiDrawMode && m_beautyShaderInstance)
        {
		    m_beautyShaderInstance->bind(context);	// make sure our shader is the active shader
        }
		m_shaderBound = true;
	}
}

bool customTextureShaderOverride::draw(MHWRender::MDrawContext& context, const MHWRender::MRenderItemList& renderItemList) const
{
	if (!m_shaderBound)
		return false;

	// Draw for color pass
	if (m_inBeautyPass)
	{

        MHWRender::MShaderInstance * shaderInstance = NULL;
        if (m_inMultiDrawMode)
        {
            shaderInstance = m_multiDrawBeautyShaderInstance;
        }
        else
        {
            shaderInstance = m_beautyShaderInstance;
        }

        if (shaderInstance)
        {
		    unsigned int passCount = shaderInstance->getPassCount(context);
		    if (passCount)
		    {
			    for (unsigned int i=0; i<passCount; i++)
			    {
				    shaderInstance->activatePass(context, i);
				    MHWRender::MPxShaderOverride::drawGeometry(context);
			    }
		    }
        }

		return true;
	}

	return false;
}

void customTextureShaderOverride::terminateKey(MHWRender::MDrawContext& context, const MString& /*key*/)
{
	if (m_shaderBound && m_inBeautyPass)
	{
		if (m_inMultiDrawMode && m_multiDrawBeautyShaderInstance)
		{
			m_multiDrawBeautyShaderInstance->unbind(context);
		}
        else if (!m_inMultiDrawMode && m_beautyShaderInstance)
        {
			m_beautyShaderInstance->unbind(context);
        }
	}

    m_inMultiDrawMode = false;
	m_shaderBound = false;
}

MHWRender::DrawAPI customTextureShaderOverride::supportedDrawAPIs() const
{
	return ( MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile | MHWRender::kDirectX11 );
}

bool customTextureShaderOverride::isTransparent()
{
	return false;
}

bool customTextureShaderOverride::overridesDrawState()
{
	return false;
}

bool customTextureShaderOverride::supportsMultiDraw() const
{
    return m_multiDrawBeautyShaderInstance != NULL;
}

void customTextureShaderOverride::updateShaderInstance()
{
	MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	// this could be optimized by checking if parameters actually changed
	// see hwPhongShader for a more complete example in this regards
	if (m_beautyShaderInstance)
	{
		m_beautyShaderInstance->setParameter("colVal", 1.0f);
	}
    if (m_multiDrawBeautyShaderInstance)
	{
		m_multiDrawBeautyShaderInstance->setParameter("colVal", 1.0f);
	}
}

void customTextureShaderOverride::buildAndUpdateCustomDataTextureViaMaya()
{
	MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer)
		return;

	MHWRender::MTextureManager* mt = renderer->getTextureManager();

	generateCustomTexture();

	MString newTextureName = MString("MyCustomTexture");
	MHWRender::MTexture* myTex = mt->acquireTexture(newTextureName, m_desc, m_customTextureData, false);

	if (myTex)
	{
		MHWRender::MTextureAssignment texResource;
		texResource.texture = myTex;
        if (m_beautyShaderInstance)
        {
		    m_beautyShaderInstance->setParameter("MyTexture", texResource);
        }
        
        if (m_multiDrawBeautyShaderInstance)
        {
		    m_multiDrawBeautyShaderInstance->setParameter("MyTexture", texResource);
        }
	}
}

void customTextureShaderOverride::generateCustomTexture()
{
	int textureSize = 256;
	int numSlice = 1; // Number of array slices. e.g. 6 would be required for a cube-map

	// First time, we create a contiguous block of data for the texture.
	if (!m_customTextureData)
	{
		m_customTextureData = new unsigned char[4*textureSize*textureSize*numSlice];

		// init values
		// do whatever you would do with your custom texture data. E.g. generate procedural texture.
		int index = 0;
		for (int face=0 ; face<numSlice ; face++)
		{
			for (int j=0 ; j<textureSize ; j++)
			{
				for (int i=0 ; i<textureSize ; i++)
				{
					m_customTextureData[index++] = (unsigned char)(i);
					m_customTextureData[index++] = (unsigned char)(j);
					m_customTextureData[index++] = (unsigned char)(255*face/numSlice);
					m_customTextureData[index++] = 255;
				}
			}
		}

		// we still have to setup a texture description for Maya, even when using custom data:
		m_desc.setToDefault2DTexture();
		m_desc.fWidth = textureSize;
		m_desc.fHeight = textureSize;
		m_desc.fDepth = 1;
		m_desc.fBytesPerRow = 4*textureSize;
		m_desc.fBytesPerSlice = 4*textureSize*textureSize;
		m_desc.fMipmaps = 1;
		m_desc.fArraySlices = numSlice;
		m_desc.fFormat = MHWRender::kR8G8B8A8_UNORM;
		m_desc.fTextureType = MHWRender::kImage2D;
	}
}
