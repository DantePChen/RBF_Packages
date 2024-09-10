//-
// Copyright 2016 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+


#include <maya/MDrawContext.h>
#include <maya/MDrawRegistry.h>
#include <maya/MFnPlugin.h>
#include <maya/MFragmentManager.h>
#include <maya/MGlobal.h>
#include <maya/MPxHardwareShader.h>
#include <maya/MPxShaderOverride.h>
#include <maya/MRenderUtilities.h>
#include <maya/MShaderManager.h>
#include <maya/MStateManager.h>
#include <maya/MString.h>


////////////////////////////////////////////////////////////////////////////////
//
// This plug-in implementation shows the usage of MPxShaderOverride for a Maya
// shader node which can be assigned to particle sprites for customized shading.
//
// The purpose is to demonstrate how to define, connect and debug shader fragments
// using C++ API or XML schema for shading fragments in plug-ins, and in particular
// how to define and connect a geometry shader fragment. In this case the plug-in
// will create a shader instance from a fragment graph whose connections, properties,
// values and outputs are defined in XML and connect it with a geometry shader
// fragment using MShaderInstance::addInputFragment().
//
// Because the shader instance is generated from shader fragments, the lighting
// fragments can be connected to the shader instance in order to allow the shader
// to work with maya lights. For more details please see the documentation for
// MPxSurfaceShadingNodeOverride.
//
// An implementation of color management on the input texture is also built into
// customFileTextureOutputColor.xml, which is to show how maya converts texture
// color from sRGB space to linear space. The implementation depends on input
// color space and may vary according to needs, e.g. plug-ins can also choose
// GPU linearization instead of a shader method.
//
// Transparency notification is handled by setting MPxShaderOverride::isTransparent()
// virtual method return value appropriately as needed. The support for advanced
// transparency algorithms, such as depth peeling and weighted average, are also
// handled by returning true for MPxShaderOverride::supportsAdvancedTransparency()
// and checking specific pass semantics in MPxShaderOverride::handlesDraw().
//
// The code also demonstrates handling of non-textured drawing by providing another
// MShaderInstance to be returned from MPxShaderOverride::nonTexturedShaderInstance().
// It will also handle transparency by appropriately setting the shader instance
// transparency hint using MShaderInstance::setIsTransparent().
//
// (De)registration is shown as part of plug-in (de)initialization, noting that
// the MPxShaderOverride association to the MPxNode node is achieved by supplying
// the same "drawdb/shader" classification for both registration interfaces.
//
// For debugging shader fragments, the final effect source of the shader instance
// can be written to disk using MShaderInstance::writeEffectSourceToFile() and
// can be used to create another shader instance for being bound to a draw context.
// In case of failure, shader compilation errors can be either displayed in Script
// Editor during binding or queried from MShaderManager::getLastError() afterwards.
//
// customSpriteShader.mel is provided for easier testing, which creates some
// particle sprites with random scale and twist, and then assign them with
// customSpriteShader.
//
// The plugin assumes that the devkit location follows "Setting up your build
// environment" at Maya Developer Help; otherwise, shaders/textures cannot be
// located. In this case create a mod (module description file) as below in a
// suitable modules folder (getenv "MAYA_MODULE_PATH") and restart Maya.
//
//   + devkit 1.0 <local devkit path>
//
////////////////////////////////////////////////////////////////////////////////


class customSpriteShader : public MPxHardwareShader
{
public:

	~customSpriteShader() override
	{
	}

	static void* creator()
	{
		return new customSpriteShader();
	}

	static MStatus initialize()
	{
		return MS::kSuccess;
	}

	static MTypeId id;

private:

	customSpriteShader()
	{
	}
};

MTypeId customSpriteShader::id(0x00081033);


class customSpriteShaderOverride : public MHWRender::MPxShaderOverride
{
public:

	~customSpriteShaderOverride() override;

	static MHWRender::MPxShaderOverride* creator(const MObject& obj)
	{
		return new customSpriteShaderOverride(obj);
	}

	MHWRender::DrawAPI supportedDrawAPIs() const override
	{
		return MHWRender::kAllDevices;
	}

	MHWRender::MShaderInstance* shaderInstance(MHWRender::MDrawContext& context) const override
	{
		return fShaderInstance;
	}

	MHWRender::MShaderInstance* nonTexturedShaderInstance(bool &monitorNode) const override
	{
		monitorNode = false;
		return fNonTexturedShaderInstance;
	}

	bool isTransparent() override
	{
		return true;
	}
	
	bool supportsAdvancedTransparency() const override
	{
		return true;
	}

	MString initialize(const MInitContext& initContext, MSharedPtr<MUserData>& userData) override;
	bool handlesDraw(MHWRender::MDrawContext& context) override;
	void activateKey(MHWRender::MDrawContext& context, const MString& key) override;
	bool draw(MHWRender::MDrawContext& context,const MHWRender::MRenderItemList& renderItemList) const override;
	void terminateKey(MHWRender::MDrawContext& context, const MString& key) override;

	static MStatus registerShadeFragments();
	static MStatus deregisterShadeFragments();
	static bool addFragmentXML(MHWRender::MFragmentManager& fragMgr, const MString& fileName, bool asGraph);

private:

	customSpriteShaderOverride(const MObject& obj);

	void createShaderInstances();

private:

	MHWRender::MShaderInstance *fShaderInstance;
	MHWRender::MShaderInstance *fNonTexturedShaderInstance;

	static MStringArray sFragmentArray;
	static bool sDebugFragment;
};

MStringArray customSpriteShaderOverride::sFragmentArray;
bool customSpriteShaderOverride::sDebugFragment = (getenv("MAYA_CUSTOMSPRITESHADER_DEBUG_FRAGMENTS") != NULL);

customSpriteShaderOverride::customSpriteShaderOverride(const MObject& obj)
	: MHWRender::MPxShaderOverride(obj)
	, fShaderInstance(NULL)
	, fNonTexturedShaderInstance(NULL)
{
	createShaderInstances();
}

customSpriteShaderOverride::~customSpriteShaderOverride()
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (shaderMgr)
		{
			if (fShaderInstance)
			{
				shaderMgr->releaseShader(fShaderInstance);
				fShaderInstance = NULL;
			}

			if (fNonTexturedShaderInstance)
			{
				shaderMgr->releaseShader(fNonTexturedShaderInstance);
				fNonTexturedShaderInstance = NULL;
			}
		}
	}
}

void customSpriteShaderOverride::createShaderInstances()
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (shaderMgr)
		{
			// XML-based fragment graph supports multiple connections between
			// shade fragments, while MShaderInstance::addInputFragment can only
			// support single connection between the shader instance and input
			// fragment. To see how internal fragments are implemented, use the
			// fragmentDumper plugin.
			//
			fShaderInstance = shaderMgr->getFragmentShader(
				"customFileTextureBlinnShader", "outSurfaceFinal", true);

			// Connect the custom geometry shader.
			//
			fShaderInstance->addInputFragment(
				"customPoint2ViewAlignedTexturedQuad", "GPUStage", "GPUStage");

			// Acquire and bind the snow texture.
			//
			// The plugin assumes that the devkit location follows "Setting up your build
			// environment" at Maya Developer Help; otherwise, shaders/textures cannot be
			// located. In this case create a mod (module description file) as below in a
			// suitable modules folder (getenv "MAYA_MODULE_PATH") and restart Maya.
			//
			//   + devkit 1.0 <local devkit path>
			//
			MString path;
			if (!MGlobal::executeCommand(MString("getModulePath -moduleName \"devkit\""), path))
			{
				path = MString(getenv("MAYA_LOCATION")) + MString("/devkit");
			}
			path += MString("/plug-ins/customSpriteShader/");
			MHWRender::MTexture* texture =
				renderer->getTextureManager()->acquireTexture(path + MString("snow.png"), "", 1);
			if (texture)
			{
				MHWRender::MTextureAssignment texResource;
				texResource.texture = texture;
				fShaderInstance->setParameter("map", texResource);
			}
			else
			{
				MString errorMsg = MString("customSpriteShader failed to acquire texture from ") + path + MString("snow.png");
				MGlobal::displayError(errorMsg);
			}

			// Acquire and bind the default texture sampler.
			//
			MHWRender::MSamplerStateDesc samplerDesc;
			const MHWRender::MSamplerState* sampler =
				MHWRender::MStateManager::acquireSamplerState(samplerDesc);
			if (sampler)
			{
				fShaderInstance->setParameter("textureSampler", *sampler);
			}

			// Particle sprites cannot directly be drawn with the default non-
			// textured shader instance which is designed for polygons, so we
			// create a non-texture shader instance as well. Note the geometry
			// shader is slightly different from the textured version.
			//
			fNonTexturedShaderInstance = shaderMgr->getFragmentShader(
				"customSolidColorBlinnShader", "outSurfaceFinal", true);
			fNonTexturedShaderInstance->addInputFragment(
				"customPoint2ViewAlignedSolidQuad", "GPUStage", "GPUStage");

			// Set color and transparency.
			float customColor[3] = {1.0, 0.0, 0.0};
			fNonTexturedShaderInstance->setParameter("customColor", customColor);

			float customTransparency[3] = { 0.0, 0.0, 0.0 };
			fNonTexturedShaderInstance->setParameter("customTransparency", customTransparency);
			fNonTexturedShaderInstance->setIsTransparent(false);

			// Dump final effect source and perform validation, for debugging the
			// custom shader fragments on all device APIs. For validation, plugin
			// can bind a temporary draw context with a shader instance generated
			// from the effect source dump. Make sure it's unbound afterwards.
			if (sDebugFragment)
			{
				MString filePath = path + MString("customSpriteShader.fx");
				fShaderInstance->writeEffectSourceToFile(filePath);

				MHWRender::MDrawContext* dc =
					MHWRender::MRenderUtilities::acquireSwatchDrawContext();
				MHWRender::MShaderInstance* shaderInstance =
					shaderMgr->getEffectsFileShader(filePath, "");

				if (dc && shaderInstance)
				{
					if (!shaderInstance->bind(*dc))
					{
						MString errorMsg = filePath +
							MString(":\n") +
							MHWRender::MShaderManager::getLastError() +
							MHWRender::MShaderManager::getLastErrorSource(true, true, 2);
						MGlobal::displayError(errorMsg);
					}

					shaderInstance->unbind(*dc);
				}

				shaderMgr->releaseShader(shaderInstance);
				MHWRender::MRenderUtilities::releaseDrawContext(dc);
			}
		}
	}
}

MString customSpriteShaderOverride::initialize(const MInitContext&, MSharedPtr<MUserData>&)
{
	MString empty;

	MHWRender::MVertexBufferDescriptor positionDesc(
		empty,
		MHWRender::MGeometry::kPosition,
		MHWRender::MGeometry::kFloat,
		3);

	MHWRender::MVertexBufferDescriptor normalDesc(
		empty,
		MHWRender::MGeometry::kNormal,
		MHWRender::MGeometry::kFloat,
		3);

	MString spritePP("spritePP");
	MHWRender::MVertexBufferDescriptor spriteDesc(
		spritePP,
		MHWRender::MGeometry::kTexture,
		spritePP,
		MHWRender::MGeometry::kFloat,
		4);

	MHWRender::MVertexBufferDescriptor uvDesc(
		empty,
		MHWRender::MGeometry::kTexture,
		MHWRender::MGeometry::kFloat,
		2);

	addGeometryRequirement(positionDesc);
	addGeometryRequirement(normalDesc);
	addGeometryRequirement(spriteDesc);
	addGeometryRequirement(uvDesc);

	if (fShaderInstance)
	{
		addShaderSignature(*fShaderInstance);
	}

	return MString("customSpriteShaderOverride");
}

bool customSpriteShaderOverride::handlesDraw(MHWRender::MDrawContext& context)
{
	const MHWRender::MPassContext& passCtx = context.getPassContext();
	const MStringArray& passSem = passCtx.passSemantics();

	bool handlePass = false;
	for (unsigned int i=0; i<passSem.length(); i++)
	{
		const MString& pass = passSem[i];

		// For color passes, only handle if there isn't already a global override.
		// This is the same as the default logic for this method in MPxShaderOverride
		if (pass == MHWRender::MPassContext::kColorPassSemantic)
		{
			if (!passCtx.hasShaderOverride())
			{
				handlePass = true;
			}
		}
		// Advanced transparency algorithms are supported.
		else if (pass == MHWRender::MPassContext::kTransparentPeelSemantic ||
				 pass == MHWRender::MPassContext::kTransparentPeelAndAvgSemantic ||
				 pass == MHWRender::MPassContext::kTransparentWeightedAvgSemantic)
		{
			handlePass = true;
		}
		// If these semantics are specified then they override the color pass
		// semantic handling.
		else if (pass == MHWRender::MPassContext::kDepthPassSemantic ||
				 pass == MHWRender::MPassContext::kNormalDepthPassSemantic)
		{
			handlePass = false;
		}
	}

	return handlePass;
}

void customSpriteShaderOverride::activateKey(MHWRender::MDrawContext& context, const MString& key)
{
	if (fShaderInstance)
	{
		fShaderInstance->bind(context);
	}
}

bool customSpriteShaderOverride::draw(MHWRender::MDrawContext& context, const MHWRender::MRenderItemList& renderItemList) const
{
	if (fShaderInstance)
	{
		unsigned int passCount = fShaderInstance->getPassCount(context);
		if (passCount)
		{
			for (unsigned int i = 0; i < passCount; i++)
			{
				fShaderInstance->activatePass(context, i);
				MHWRender::MPxShaderOverride::drawGeometry(context);
			}
		}
	}

	return true;
}

void customSpriteShaderOverride::terminateKey(MHWRender::MDrawContext& context, const MString& key)
{
	if (fShaderInstance)
	{
		fShaderInstance->unbind(context);
	}
}

bool customSpriteShaderOverride::addFragmentXML(MHWRender::MFragmentManager& fragMgr,
												const MString& fileName,
												bool asGraph)
{
	MString fragName = asGraph ? fragMgr.addFragmentGraphFromFile( fileName ) :
								 fragMgr.addShadeFragmentFromFile( fileName, false );

	if (fragName.length() == 0)
	{
		MString errorMsg = MString("customSpriteShader failed to add fragment from ") + fileName;
		MGlobal::displayError(errorMsg);
		return false;
	}

	sFragmentArray.append(fragName);
	return true;
}

MStatus customSpriteShaderOverride::registerShadeFragments()
{
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		MHWRender::MFragmentManager* fragMgr = renderer->getFragmentManager();
		if (fragMgr)
		{
			// Add search path (once only)
			//
			// The plugin assumes that the devkit location follows "Setting up your build
			// environment" at Maya Developer Help; otherwise, shaders/textures cannot be
			// located. In this case create a mod (module description file) as below in a
			// suitable modules folder (getenv "MAYA_MODULE_PATH") and restart Maya.
			//
			//   + devkit 1.0 <local devkit path>
			//
			MString path;
			if (!MGlobal::executeCommand(MString("getModulePath -moduleName \"devkit\""), path, false))
			{
				path = MString(getenv("MAYA_LOCATION")) + MString("/devkit");
			}
			path += "/plug-ins/customSpriteShader";
			fragMgr->addFragmentPath(path);

			MString info = MString("customSpriteShader added a fragment search path: ") + path;
			MGlobal::displayInfo(info);

			// Fragment graphs are registered after shader fragments because of dependency.
			if (addFragmentXML(*fragMgr, "customFileTextureOutputColor.xml", false) &&
				addFragmentXML(*fragMgr, "customFileTextureOutputTransparency.xml", false) &&
				addFragmentXML(*fragMgr, "customPoint2ViewAlignedSolidQuad.xml", false) &&
				addFragmentXML(*fragMgr, "customPoint2ViewAlignedTexturedQuad.xml", false) &&
				addFragmentXML(*fragMgr, "customFileTextureBlinnShader.xml", true) &&
				addFragmentXML(*fragMgr, "customSolidColorBlinnShader.xml", true))
			{
				return MS::kSuccess;
			}
		}
	}

	return MS::kFailure;
}

MStatus customSpriteShaderOverride::deregisterShadeFragments()
{
	bool success = false;

	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (renderer)
	{
		MHWRender::MFragmentManager* fragMgr = renderer->getFragmentManager();
		if (fragMgr)
		{
			success = true;

			for (unsigned int i = 0; i < sFragmentArray.length(); i++)
			{
				MString fragName = sFragmentArray[i];
				if (fragMgr->hasFragment(fragName) && !fragMgr->removeFragment(fragName))
				{
					success = false;

					MString errorMsg = MString("customSpriteShader failed to remove fragment ") + sFragmentArray[i];
					MGlobal::displayError(errorMsg);
				}
			}
		}
	}

	sFragmentArray.clear();
	return success ? MS::kSuccess : MS::kFailure;
}


static MString sCustomSpriteShaderRegistrantId("customSpriteShaderRegistrantId");
static MString sCustomSpriteShaderDrawdbClassification("drawdb/shader/surface/customSpriteShader");

MStatus initializePlugin( MObject obj )
{
	MStatus status = customSpriteShaderOverride::registerShadeFragments();
	if (status != MS::kSuccess)
	{
		status.perror("registerShadeFragments");
		return status;
	}

	MFnPlugin plugin(obj, "Autodesk", "1.0", "Any");

	MString UserClassification("shader/surface/:");
	UserClassification += sCustomSpriteShaderDrawdbClassification;

	status = plugin.registerNode(
		"customSpriteShader",
		customSpriteShader::id,
		customSpriteShader::creator,
		customSpriteShader::initialize,
		MPxNode::kHardwareShader,
		&UserClassification);
	if (status != MS::kSuccess)
	{
		status.perror("registerNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::registerShaderOverrideCreator(
		sCustomSpriteShaderDrawdbClassification,
		sCustomSpriteShaderRegistrantId,
		customSpriteShaderOverride::creator);
	if (status != MS::kSuccess)
	{
		status.perror("registerShaderOverrideCreator");
		return status;
	}

	return status;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin(obj);

	MStatus status = plugin.deregisterNode(customSpriteShader::id);
	if (status != MS::kSuccess)
	{
		status.perror("deregisterNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::deregisterShaderOverrideCreator(
		sCustomSpriteShaderDrawdbClassification,
		sCustomSpriteShaderRegistrantId);
	if (status != MS::kSuccess)
	{
		status.perror("deregisterShaderOverrideCreator");
		return status;
	}

	status = customSpriteShaderOverride::deregisterShadeFragments();
	if (status != MS::kSuccess)
	{
		status.perror("deregisterShadeFragments");
		return status;
	}

	return status;
}
