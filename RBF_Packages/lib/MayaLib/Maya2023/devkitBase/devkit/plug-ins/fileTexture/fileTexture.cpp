///////////////////////////////////////////////////////////////////
// DESCRIPTION: A simple example of file texture node.
//
// Inputs:
//
//	FileName: the name of the file to load
//  UV: uv coordinate we're evaluating now.
//
// Output:
//
//  outColor: the result color.
//
// Need to enter the following commands before using:
//
//  shadingNode -asTexture fileTexture;
//  shadingNode -asUtility place2dTexture;
//  connectAttr place2dTexture1.outUV fileTexture1.uvCoord;
///////////////////////////////////////////////////////////////////

#include <maya/MFnPlugin.h>
#include <maya/MPxNode.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>
#include <maya/MRenderUtil.h>
#include <maya/MImage.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MDrawRegistry.h>
#include <maya/MPxShadingNodeOverride.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MFragmentManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MStateManager.h>

// Node Declaration
class FileNode : public MPxNode
{
public:
	static void* creator();
	static MStatus initialize();

	FileNode();
	~FileNode() override;

	MStatus setDependentsDirty(
		const MPlug& plug,
		MPlugArray& plugArray) override;
	MStatus compute(const MPlug&, MDataBlock&) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

	// ID tag for use with binary file format
	static const MTypeId id;

private:
	MImage fImage;
	size_t fWidth;
	size_t fHeight;

	// Attributes
	static MObject aFileName;
	static MObject aCMConfigPath;
	static MObject aCMWorkingSpace;
	static MObject aColorSpace;
	static MObject aCMEnabled;
	static MObject aCMConfigEnabled;
	static MObject aUVCoord;
	static MObject aOutColor;
	static MObject aOutAlpha;

	friend class FileNodeOverride;
};

// Override Declaration
class FileNodeOverride : public MHWRender::MPxShadingNodeOverride
{
public:
	static MHWRender::MPxShadingNodeOverride* creator(const MObject& obj);

	~FileNodeOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	MString fragmentName() const override;
	void getCustomMappings(
		MHWRender::MAttributeParameterMappingList& mappings) override;

	void updateDG() override;
	void updateShader(
		MHWRender::MShaderInstance& shader,
		const MHWRender::MAttributeParameterMappingList& mappings) override;

	bool valueChangeRequiresFragmentRebuild(const MPlug* plug) const override;

private:
	FileNodeOverride(const MObject& obj);

	MString fFragmentName;
	MObject fObject;
	MString fFileName;
	const MHWRender::MSamplerState* fSamplerState;

	mutable MString fResolvedMapName;
	mutable MString fResolvedSamplerName;
};



////////////////////////////////////////////////////////////////////////////
// Node Implementation
////////////////////////////////////////////////////////////////////////////
const MTypeId FileNode::id(0x00081057);

void* FileNode::creator()
{
	return new FileNode();
}

FileNode::FileNode()
: fWidth(0)
, fHeight(0)
{
}

FileNode::~FileNode()
{
}

MStatus FileNode::setDependentsDirty(
	const MPlug& plug,
	MPlugArray& plugArray)
{
	if (plug == aFileName)
	{
		fImage.release();
		fWidth = fHeight = 0;
	}
	return MPxNode::setDependentsDirty(plug, plugArray);
}

MStatus FileNode::compute(const MPlug& plug, MDataBlock& block)
{
	// outColor or individial R, G, B channel, or alpha
	if ((plug != aOutColor) &&
		(plug.parent() != aOutColor) &&
		(plug != aOutAlpha))
	{
		return MS::kUnknownParameter;
	}

    MFloatVector resultColor(0.0f, 0.0f, 0.0f);
    float resultAlpha = 1.0f;

    // Read from file if we need to
    if (!fImage.pixels())
    {
	    MString& fileName = block.inputValue(aFileName).asString();
    	MString exactName(fileName);
		
		// This class is derived from MPxNode, therefore it is not a DAG node and does not have a path.
		// Instead you we just get the node's name using the name() method inherited from MPxNode as the context.
		if (MRenderUtil::exactFileTextureName(fileName, false, "", name(), exactName))
		{
			unsigned int width = 0;
			unsigned int height = 0;
			if (fImage.readFromFile(exactName) &&
				fImage.getSize(width, height))
			{
				fWidth = width;
				fHeight = height;
			}
		}
	}

	// Compute outputs from image data
	unsigned char* data = fImage.pixels();
	if (data && fWidth > 0 && fHeight > 0)
	{
		float2& uv = block.inputValue(aUVCoord).asFloat2();
		float u = uv[0]; if (u<0.0f) u=0.0f; if (u>1.0f) u=1.0f;
		float v = uv[1]; if (v<0.0f) v=0.0f; if (v>1.0f) v=1.0f;

		static const size_t pixelSize = 4;
		size_t rowOffset = (size_t)(v*(fHeight-1)) * fWidth;
		size_t colOffset = (size_t)(u*(fWidth-1));
		const unsigned char* pixel = data +
			((rowOffset + colOffset) * pixelSize);

		resultColor[0] = ((float)pixel[0])/255.0f;
		resultColor[1] = ((float)pixel[1])/255.0f;
		resultColor[2] = ((float)pixel[2])/255.0f;
		resultAlpha = ((float)pixel[3])/255.0f;
	}

    // Set ouput color attribute
    MDataHandle outColorHandle = block.outputValue(aOutColor);
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

    // Set ouput alpha attribute
	MDataHandle outAlphaHandle = block.outputValue(aOutAlpha);
	float& outAlpha = outAlphaHandle.asFloat();
	outAlpha = resultAlpha;
	outAlphaHandle.setClean();

	return MS::kSuccess;
}

#define MAKE_INPUT(attr) \
	CHECK_MSTATUS(attr.setKeyable(true)); \
	CHECK_MSTATUS(attr.setStorable(true)); \
	CHECK_MSTATUS(attr.setReadable(true)); \
	CHECK_MSTATUS(attr.setWritable(true));

#define MAKE_OUTPUT(attr) \
	CHECK_MSTATUS(attr.setKeyable(false)); \
	CHECK_MSTATUS(attr.setStorable(false)); \
	CHECK_MSTATUS(attr.setReadable(true)); \
	CHECK_MSTATUS(attr.setWritable(false));

// Attributes
MObject FileNode::aFileName;
MObject FileNode::aCMConfigPath;
MObject FileNode::aCMWorkingSpace;
MObject FileNode::aColorSpace;
MObject FileNode::aCMEnabled;
MObject FileNode::aCMConfigEnabled;
MObject FileNode::aUVCoord;
MObject FileNode::aOutColor;
MObject FileNode::aOutAlpha;

MStatus FileNode::initialize()
{
	MFnNumericAttribute nAttr;
	MFnTypedAttribute tAttr;

	// Input attributes
	MFnStringData stringData;
	MObject theString = stringData.create();
	aFileName = tAttr.create("fileName", "f", MFnData::kString, theString);
	MAKE_INPUT(tAttr);
	MFnAttribute attr(aFileName);
	CHECK_MSTATUS(attr.setUsedAsFilename(true));

	MFnStringData stringDataCMConfigPath;
	MObject theStringCMConfigPath = stringDataCMConfigPath.create();
	aCMConfigPath = tAttr.create("colorManagementConfigFilePath", "cmcfp", MFnData::kString, theStringCMConfigPath);

	MFnStringData stringDataCMWorkingSpace;
	MObject theStringCMWorkingSpace = stringDataCMWorkingSpace.create();
	aCMWorkingSpace = tAttr.create("workingSpace", "ws", MFnData::kString, theStringCMWorkingSpace);
	MAKE_INPUT(tAttr);

	MFnStringData stringDataColorSpace;
	MObject theStringColorSpace = stringDataColorSpace.create();
	aColorSpace = tAttr.create("colorSpace", "cs", MFnData::kString, theStringColorSpace);
	MAKE_INPUT(tAttr);

	aCMEnabled = nAttr.create("colorManagementEnabled", "cme", MFnNumericData::kBoolean);
	MAKE_INPUT(nAttr);
	aCMConfigEnabled = nAttr.create("colorManagementConfigFileEnabled", "cmce", MFnNumericData::kBoolean);
	MAKE_INPUT(nAttr);


	MObject child1 = nAttr.create("uCoord", "u", MFnNumericData::kFloat);
	MObject child2 = nAttr.create("vCoord", "v", MFnNumericData::kFloat);
	aUVCoord = nAttr.create("uvCoord","uv", child1, child2);
	MAKE_INPUT(nAttr);
	CHECK_MSTATUS(nAttr.setHidden(true));

	// Output attributes
	aOutColor = nAttr.createColor("outColor", "oc");
	MAKE_OUTPUT(nAttr);

	aOutAlpha = nAttr.create("outAlpha", "oa", MFnNumericData::kFloat);
	MAKE_OUTPUT(nAttr);

	// Add attributes to the node database.
	CHECK_MSTATUS(addAttribute(aFileName));
	CHECK_MSTATUS(addAttribute(aUVCoord));
	CHECK_MSTATUS(addAttribute(aOutColor));
	CHECK_MSTATUS(addAttribute(aOutAlpha));
	CHECK_MSTATUS(addAttribute(aCMEnabled));
	CHECK_MSTATUS(addAttribute(aCMConfigEnabled));
	CHECK_MSTATUS(addAttribute(aCMConfigPath));
	CHECK_MSTATUS(addAttribute(aCMWorkingSpace));
	CHECK_MSTATUS(addAttribute(aColorSpace));

	// All input affect the output color and alpha
	CHECK_MSTATUS(attributeAffects(aFileName, aOutColor));
	CHECK_MSTATUS(attributeAffects(aFileName, aOutAlpha));
	CHECK_MSTATUS(attributeAffects(aUVCoord, aOutColor));
	CHECK_MSTATUS(attributeAffects(aUVCoord, aOutAlpha));
	CHECK_MSTATUS(attributeAffects(aCMEnabled, aOutColor));
	CHECK_MSTATUS(attributeAffects(aCMConfigEnabled, aOutColor));
	CHECK_MSTATUS(attributeAffects(aCMConfigPath, aOutColor));
	CHECK_MSTATUS(attributeAffects(aCMWorkingSpace, aOutColor));
	CHECK_MSTATUS(attributeAffects(aColorSpace, aOutColor));

	return MS::kSuccess;
}



////////////////////////////////////////////////////////////////////////////
// Override Implementation
////////////////////////////////////////////////////////////////////////////
MHWRender::MPxShadingNodeOverride* FileNodeOverride::creator(
	const MObject& obj)
{
	return new FileNodeOverride(obj);
}

FileNodeOverride::FileNodeOverride(const MObject& obj)
: MPxShadingNodeOverride(obj)
, fObject(obj)
, fFragmentName("")
, fFileName("")
, fSamplerState(NULL)
, fResolvedMapName("")
, fResolvedSamplerName("")
{
	// Define fragments and fragment graph needed for VP2 version of shader,
	// these could also be defined in separate XML files.
	//
	static const MString sFragmentOutputName("fileTexturePluginFragmentOutput");
	static const char* sFragmentOutputBody =
		"<fragment uiName=\"fileTexturePluginFragmentOutput\" name=\"fileTexturePluginFragmentOutput\" type=\"structure\" class=\"ShadeFragment\" version=\"1.0\">"
		"	<description><![CDATA[Struct output for simple file texture fragment]]></description>"
		"	<properties>"
        "		<struct name=\"fileTexturePluginFragmentOutput\" struct_name=\"fileTexturePluginFragmentOutput\" />"
		"	</properties>"
		"	<values>"
		"	</values>"
		"	<outputs>"
		"		<alias name=\"fileTexturePluginFragmentOutput\" struct_name=\"fileTexturePluginFragmentOutput\" />"
		"		<float3 name=\"outColor\" semantic=\"mayaCMSemantic\" />"
		"		<float name=\"outAlpha\" />"
		"	</outputs>"
		"	<implementation>"
		"	<implementation render=\"OGSRenderer\" language=\"Cg\" lang_version=\"2.1\">"
		"		<function_name val=\"\" />"
		"		<declaration name=\"fileTexturePluginFragmentOutput\"><![CDATA["
		"struct fileTexturePluginFragmentOutput \n"
		"{ \n"
		"	float3 outColor; \n"
		"	float outAlpha; \n"
		"}; \n]]>"
		"		</declaration>"
		"	</implementation>"
		"	<implementation render=\"OGSRenderer\" language=\"HLSL\" lang_version=\"11.0\">"
		"		<function_name val=\"\" />"
		"		<declaration name=\"fileTexturePluginFragmentOutput\"><![CDATA["
		"struct fileTexturePluginFragmentOutput \n"
		"{ \n"
		"	float3 outColor; \n"
		"	float outAlpha; \n"
		"}; \n]]>"
		"		</declaration>"
		"	</implementation>"
		"	<implementation render=\"OGSRenderer\" language=\"GLSL\" lang_version=\"3.0\">"
		"		<function_name val=\"\" />"
		"		<declaration name=\"fileTexturePluginFragmentOutput\"><![CDATA["
		"struct fileTexturePluginFragmentOutput \n"
		"{ \n"
		"	vec3 outColor; \n"
		"	float outAlpha; \n"
		"}; \n]]>"
		"		</declaration>"
		"	</implementation>"
		"	</implementation>"
		"</fragment>";

	static const MString sFragmentName("fileTexturePluginFragment");
	static const char* sFragmentBody =
		"<fragment uiName=\"fileTexturePluginFragment\" name=\"fileTexturePluginFragment\" type=\"plumbing\" class=\"ShadeFragment\" version=\"1.0\">"
		"	<description><![CDATA[Simple file texture fragment]]></description>"
		"	<properties>"
        "		<float2 name=\"uvCoord\" semantic=\"mayaUvCoordSemantic\" flags=\"varyingInputParam\" />"
        "		<texture2 name=\"map\" />"
        "		<sampler name=\"textureSampler\" />"
		"	</properties>"
		"	<values>"
		"	</values>"
		"	<outputs>"
		"		<struct name=\"output\" struct_name=\"fileTexturePluginFragmentOutput\" />"
		"	</outputs>"
		"	<implementation>"
		"	<implementation render=\"OGSRenderer\" language=\"Cg\" lang_version=\"2.100000\">"
		"		<function_name val=\"fileTexturePluginFragment\" />"
		"		<source><![CDATA["
		"fileTexturePluginFragmentOutput fileTexturePluginFragment(float2 uv, texture2D map, sampler2D mapSampler) \n"
		"{ \n"
		"	fileTexturePluginFragmentOutput result; \n"
		"	uv -= floor(uv); \n"
		"	uv.y = 1.0f - uv.y; \n"
		"	float4 color = tex2D(mapSampler, uv); \n"
		"	result.outColor = color.rgb; \n"
		"	result.outAlpha = color.a; \n"
		"	return result; \n"
		"} \n]]>"
		"		</source>"
		"	</implementation>"
		"	<implementation render=\"OGSRenderer\" language=\"HLSL\" lang_version=\"11.000000\">"
		"		<function_name val=\"fileTexturePluginFragment\" />"
		"		<source><![CDATA["
		"fileTexturePluginFragmentOutput fileTexturePluginFragment(float2 uv, Texture2D map, sampler mapSampler) \n"
		"{ \n"
		"	fileTexturePluginFragmentOutput result; \n"
		"	uv -= floor(uv); \n"
		"	uv.y = 1.0f - uv.y; \n"
		"	float4 color = map.Sample(mapSampler, uv); \n"
		"	result.outColor = color.rgb; \n"
		"	result.outAlpha = color.a; \n"
		"	return result; \n"
		"} \n]]>"
		"		</source>"
		"	</implementation>"
		"	<implementation render=\"OGSRenderer\" language=\"GLSL\" lang_version=\"3.0\">"
		"		<function_name val=\"fileTexturePluginFragment\" />"
		"		<source><![CDATA["
		"fileTexturePluginFragmentOutput fileTexturePluginFragment(vec2 uv, sampler2D mapSampler) \n"
		"{ \n"
		"	fileTexturePluginFragmentOutput result; \n"
		"	uv -= floor(uv); \n"
		"	uv.y = 1.0f - uv.y; \n"
		"	vec4 color = texture(mapSampler, uv); \n"
		"	result.outColor = color.rgb; \n"
		"	result.outAlpha = color.a; \n"
		"	return result; \n"
		"} \n]]>"
		"		</source>"
		"	</implementation>"
		"	</implementation>"
		"</fragment>";

	static const MString sFragmentGraphName("fileTexturePluginGraph");
	static const char* sFragmentGraphBody =
		"<fragment_graph name=\"fileTexturePluginGraph\" ref=\"fileTexturePluginGraph\" class=\"FragmentGraph\" version=\"1.0\">"
		"	<fragments>"
		"			<fragment_ref name=\"fileTexturePluginFragment\" ref=\"fileTexturePluginFragment\" />"
		"			<fragment_ref name=\"fileTexturePluginFragmentOutput\" ref=\"fileTexturePluginFragmentOutput\" />"
		"	</fragments>"
		"	<connections>"
		"		<connect from=\"fileTexturePluginFragment.output\" to=\"fileTexturePluginFragmentOutput.fileTexturePluginFragmentOutput\" />"
		"	</connections>"
		"	<properties>"
        "		<float2 name=\"uvCoord\" ref=\"fileTexturePluginFragment.uvCoord\" semantic=\"mayaUvCoordSemantic\" flags=\"varyingInputParam\" />"
        "		<texture2 name=\"map\" ref=\"fileTexturePluginFragment.map\" />"
        "		<sampler name=\"textureSampler\" ref=\"fileTexturePluginFragment.textureSampler\" />"
		"	</properties>"
		"	<values>"
		"	</values>"
		"	<outputs>"
		"		<struct name=\"output\" ref=\"fileTexturePluginFragmentOutput.fileTexturePluginFragmentOutput\" />"
		"	</outputs>"
		"</fragment_graph>";

	// Register fragments with the manager if needed
	//
	MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
	if (theRenderer)
	{
		MHWRender::MFragmentManager* fragmentMgr =
			theRenderer->getFragmentManager();
		if (fragmentMgr)
		{
			// Add fragments if needed
			bool fragAdded = fragmentMgr->hasFragment(sFragmentName);
			bool structAdded = fragmentMgr->hasFragment(sFragmentOutputName);
			bool graphAdded = fragmentMgr->hasFragment(sFragmentGraphName);
			if (!fragAdded)
			{
				fragAdded = (sFragmentName == fragmentMgr->addShadeFragmentFromBuffer(sFragmentBody, false));
			}
			if (!structAdded)
			{
				structAdded = (sFragmentOutputName == fragmentMgr->addShadeFragmentFromBuffer(sFragmentOutputBody, false));
			}
			if (!graphAdded)
			{
				graphAdded = (sFragmentGraphName == fragmentMgr->addFragmentGraphFromBuffer(sFragmentGraphBody));
			}

			// If we have them all, use the final graph for the override
			if (fragAdded && structAdded && graphAdded)
			{
				fFragmentName = sFragmentGraphName;
			}
		}
	}
}

FileNodeOverride::~FileNodeOverride()
{
	MHWRender::MStateManager::releaseSamplerState(fSamplerState);
	fSamplerState = NULL;
}

MHWRender::DrawAPI FileNodeOverride::supportedDrawAPIs() const
{
	return MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile;
}

MString FileNodeOverride::fragmentName() const
{
	// Reset cached parameter names since the effect is being rebuilt
	fResolvedMapName = "";
	fResolvedSamplerName = "";

	return fFragmentName;
}

void FileNodeOverride::getCustomMappings(
	MHWRender::MAttributeParameterMappingList& mappings)
{
	// Set up some mappings for the parameters on the file texture fragment,
	// there is no correspondence to attributes on the node for the texture
	// parameters.
	MHWRender::MAttributeParameterMapping mapMapping(
		"map", "", false, true);
	mappings.append(mapMapping);

	MHWRender::MAttributeParameterMapping textureSamplerMapping(
		"textureSampler", "", false, true);
	mappings.append(textureSamplerMapping);
}

void FileNodeOverride::updateDG()
{
	// Pull the file name from the DG for use in updateShader
	MStatus status;
	MFnDependencyNode node(fObject, &status);
	if (status)
	{
		MString name;
		node.findPlug("fileName", true).getValue(name);
		MRenderUtil::exactFileTextureName(name, false, "", "", fFileName);
	}
}

void FileNodeOverride::updateShader(
	MHWRender::MShaderInstance& shader,
	const MHWRender::MAttributeParameterMappingList& mappings)
{
	// Handle resolved name caching
	if (fResolvedMapName.length() == 0)
	{
		const MHWRender::MAttributeParameterMapping* mapping =
			mappings.findByParameterName("map");
		if (mapping)
		{
			fResolvedMapName = mapping->resolvedParameterName();
		}
	}
	if (fResolvedSamplerName.length() == 0)
	{
		const MHWRender::MAttributeParameterMapping* mapping =
			mappings.findByParameterName("textureSampler");
		if (mapping)
		{
			fResolvedSamplerName = mapping->resolvedParameterName();
		}
	}

	// Set the parameters on the shader
	if (fResolvedMapName.length() > 0 && fResolvedSamplerName.length() > 0)
	{
		// Set sampler to linear-wrap
		if (!fSamplerState)
		{
			MHWRender::MSamplerStateDesc desc;
			desc.filter = MHWRender::MSamplerState::kAnisotropic;
			desc.maxAnisotropy = 16;
			fSamplerState = MHWRender::MStateManager::acquireSamplerState(desc);
		}
		if (fSamplerState)
		{
			shader.setParameter(fResolvedSamplerName, *fSamplerState);
		}

		// Set texture
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			MHWRender::MTextureManager* textureManager =
				renderer->getTextureManager();
			if (textureManager)
			{
				MHWRender::MTexture* texture =
					textureManager->acquireTexture(fFileName, "");
				if (texture)
				{
					MHWRender::MTextureAssignment textureAssignment;
					textureAssignment.texture = texture;
					shader.setParameter(fResolvedMapName, textureAssignment);

					// release our reference now that it is set on the shader
					textureManager->releaseTexture(texture);
				}
			}
		}
	}
}

bool FileNodeOverride::valueChangeRequiresFragmentRebuild(const MPlug* plug) const
{
	if(*plug == FileNode::aColorSpace)
		return true;

	return MHWRender::MPxShadingNodeOverride::valueChangeRequiresFragmentRebuild(plug);
}



////////////////////////////////////////////////////////////////////////////
// Plugin Setup
////////////////////////////////////////////////////////////////////////////
static const MString sRegistrantId("fileTexturePlugin");

MStatus initializePlugin(MObject obj)
{
	const MString UserClassify("texture/2d:drawdb/shader/texture/2d/fileTexture");

	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");
	CHECK_MSTATUS(plugin.registerNode(
		"fileTexture",
		FileNode::id,
		FileNode::creator,
		FileNode::initialize,
		MPxNode::kDependNode,
		&UserClassify));

	CHECK_MSTATUS(
		MHWRender::MDrawRegistry::registerShadingNodeOverrideCreator(
			"drawdb/shader/texture/2d/fileTexture",
			sRegistrantId,
			FileNodeOverride::creator));

	return MS::kSuccess;
}

MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	CHECK_MSTATUS(plugin.deregisterNode(FileNode::id));

	CHECK_MSTATUS(
		MHWRender::MDrawRegistry::deregisterShadingNodeOverrideCreator(
			"drawdb/shader/texture/2d/fileTexture",
			sRegistrantId));


   return MS::kSuccess;
}
// =====================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// =====================================================================
