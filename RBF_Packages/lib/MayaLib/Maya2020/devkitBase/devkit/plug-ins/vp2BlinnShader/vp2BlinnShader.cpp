//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+
#include <maya/MIOStream.h>
#include <math.h>
#include <cstdlib>

//#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnDependencyNode.h>
//#include <maya/MUserData.h>

//#include <maya/MHardwareRenderer.h>

// Includes for swatch rendering
#include <maya/MHWShaderSwatchGenerator.h>
#include <maya/MImage.h>
#include <maya/MRenderUtilities.h>

#include <maya/MMatrix.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxShaderOverride.h>
#include <maya/MDrawContext.h>
#include <maya/MStateManager.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MShaderManager.h>

#undef ENABLE_TRACE_API_CALLS
//#define ENABLE_TRACE_API_CALLS 1
#ifdef ENABLE_TRACE_API_CALLS
#define TRACE_API_CALLS(x) cerr <<(x)<<"\n"
#else
#define TRACE_API_CALLS(x)
#endif

#include "vp2BlinnShader.h"

// Node id
MTypeId vp2BlinnShader::id( 0x00081102 );
// Node attributes
MObject  vp2BlinnShader::aColor;
MObject  vp2BlinnShader::aDiffuse;
MObject  vp2BlinnShader::aTransparency;
MObject  vp2BlinnShader::aSpecularColor;
MObject  vp2BlinnShader::aSpecularRolloff;
MObject  vp2BlinnShader::aEccentricity;
MObject  vp2BlinnShader::aNonTexturedColor;
MObject  vp2BlinnShader::aNonTexturedTransparency;

///////////////////////////////////////////////////////////////////////////////////////////
// This plug-in implementation shows the usage of an MPxShaderOverride for 
// a Maya shader node. 
//
// The purpose is to show as simple a plug-in as possible without worrying
// about the details of trying to write a shader system.
// 
// As such it attempts to (re)use as many internal VP2 API
// intterfaces and constructs as possible. 
//
// In this case the plug-in will use a stock shader instance (MShaderInstance)
// as it's single internal shader. As part of initialization it will reuse
// internal MShaderInstance utility methods to show how a DX11 shader signature
// can be set as as well as how to return the vertex requirements simply.
// As there is only one shader instance the shader key used is also simplistic
// as it can be a constant value.
//
// Transparency notificaiton is handle by setting the MPxShaderOverride::isTransparent() 
// virtual method return value appropriately based on node attribute values.
//
// The code also demonstrates handling of non-textured draw by providing another stock 
// MShaderInstance to be returned from MPxShaderOverride::nonTexturedShaderInstance()
// It will also handle transparency by appropriately updating the shader instance
// transparency hint using MShaderInstance::setIsTransparent().
//
// For update, the node attributes match a few of the MShaderInstance parameters
// to show DG update and device update. Additional attributes are added to
// show how the non-texture mode shader instance parameters can be driven
// by attribute changes.
//
// For drawing, the code reuses the internally provided binding interfaces on
// MShaderInstance as well as the utility geometry drawing method:
// MPxShaderOverride::drawGeometry().
//
// For swatch rendering the basic VP2 supplied utility 
// MRenderUtilities::renderMaterialViewerGeometry() is used.
//
// (De)registration is shown as part of plug-in (de)initialization, noting
// that the MPxShaderOverride association to the MPxNode node is achieved
// by supplying the same "drawdb/shader" classification for both registration
// interfaces.
// 
///////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////
// Node methods
///////////////////////////////////////////////////////////////////////////////////////////
void * vp2BlinnShader::creator()
{
	TRACE_API_CALLS("vp2BlinnShader::creator");
    return new vp2BlinnShader();
}

vp2BlinnShader::vp2BlinnShader()
{
	TRACE_API_CALLS("vp2BlinnShader::vp2BlinnShader");
}

vp2BlinnShader::~vp2BlinnShader()
{
	TRACE_API_CALLS("vp2BlinnShader::~vp2BlinnShader");
}

MStatus vp2BlinnShader::initialize()
{
	// Shader attributes for the node
	// They have been created to match internal parameters of the
	// hardware shader instance
	//
	TRACE_API_CALLS("vp2BlinnShader::initialize");
    MFnNumericAttribute nAttr;

    // Create textured mode input attributes
    aColor = nAttr.createColor( "color", "c");
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.6f, 0.6f, 0.6f);
	nAttr.setAffectsAppearance( true );

	aDiffuse = nAttr.create( "diffuse", "dc", MFnNumericData::kFloat );
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.8);
	nAttr.setMax(1.0f);
	nAttr.setMin(0.0f);
	nAttr.setAffectsAppearance( true );

	aTransparency = nAttr.create( "transparency", "tr", MFnNumericData::kFloat );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setMin(0.0f);
	nAttr.setAffectsAppearance( true );

    aSpecularColor = nAttr.createColor( "specularColor", "sc" );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f, 1.0f, 1.0f);
	nAttr.setAffectsAppearance( true );

	aSpecularRolloff = nAttr.create( "specularRollOff", "sro", MFnNumericData::kFloat );
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.7);
	nAttr.setMax(1.0f);
	nAttr.setMin(0.0f);
	nAttr.setAffectsAppearance( true );

	aEccentricity = nAttr.create( "eccentricity", "ec", MFnNumericData::kFloat );
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.3);
	nAttr.setMax(1.0f);
	nAttr.setMin(0.0f);
	nAttr.setAffectsAppearance( true );

    // Create non-textured mode input attributes
    aNonTexturedColor = nAttr.createColor( "nonTexturedColor", "nc");
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f, 0.0f, 0.0f);
	nAttr.setAffectsAppearance( true );

	aNonTexturedTransparency = nAttr.create( "nonTexturedTransparency", "nt", MFnNumericData::kFloat );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setMin(0.0f);
	nAttr.setAffectsAppearance( true );

	// create output attributes here
	// outColor is the only output attribute and it is inherited
	// so we do not need to create or add it.
	//

	// Add the attributes to the node
    addAttribute(aColor);
	addAttribute(aDiffuse);
	addAttribute(aTransparency);
	addAttribute(aSpecularColor);
	addAttribute(aSpecularRolloff);
	addAttribute(aEccentricity);
    addAttribute(aNonTexturedColor);
	addAttribute(aNonTexturedTransparency);

    attributeAffects (aColor,			outColor);
	attributeAffects (aDiffuse,			outColor);
    attributeAffects (aTransparency,	outColor);
	attributeAffects (aSpecularColor,	outColor);
	attributeAffects (aSpecularRolloff,	outColor);
	attributeAffects (aEccentricity,	outColor);
	attributeAffects (aNonTexturedColor,outColor);
	attributeAffects (aNonTexturedTransparency,outColor);
    return MS::kSuccess;
}

//
// Very simplistic software compute for the Maya software renderer
// This code is not the focus of this plug-in example so just
// returns a constant color.
//
MStatus vp2BlinnShader::compute(
const MPlug&      plug,
      MDataBlock& block )
{
	TRACE_API_CALLS("vp2BlinnShader::compute");

    if ((plug != outColor) && (plug.parent() != outColor))
		return MS::kUnknownParameter;

	MFloatVector & color  = block.inputValue( aColor ).asFloatVector();

    // set output color attribute
    MDataHandle outColorHandle = block.outputValue( outColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
	outColor = color;

    outColorHandle.setClean();
    return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////////
// Swatch rendering: 
// Does not matter the mode for the viewport VP1 or VP2
// Uses material viewer utility which uses the VP2 render to draw the swatch.
////////////////////////////////////////////////////////////////////////////////////
MStatus vp2BlinnShader::renderSwatchImage( MImage & outImage )
{	
	if (MHWRender::MRenderer::theRenderer())
	{
		// Use some sample objects for display
		MString meshSphere("meshTeapot");
		MString meshShaderball("meshShaderball");

		unsigned int targetW, targetH;
		outImage.getSize(targetW, targetH);

		return MHWRender::MRenderUtilities::renderMaterialViewerGeometry(targetW > 128 ? meshShaderball : meshSphere, 
																		thisMObject(), 
																		outImage, 
																		MHWRender::MRenderUtilities::kPerspectiveCamera, 
																		MHWRender::MRenderUtilities::kSwatchLight);
	}
	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////////////////////////
// Viewport 2.0 shader override implementation
////////////////////////////////////////////////////////////////////////////////////
class vp2BlinnShaderOverride : public MHWRender::MPxShaderOverride
{
public:
	// Static method to create a new override
	static MHWRender::MPxShaderOverride* Creator(const MObject& obj)
	{
		return new vp2BlinnShaderOverride(obj);
	}

	// Release the textured and non-textured mode shaders.
	~vp2BlinnShaderOverride() override
	{
		MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
		if (theRenderer)
		{
			const MHWRender::MShaderManager* shaderMgr = theRenderer->getShaderManager();
			if (shaderMgr)
			{
				if (fColorShaderInstance)
				{
					shaderMgr->releaseShader(fColorShaderInstance);
				}
				fColorShaderInstance = NULL;

				if (fNonTexturedColorShaderInstance)
				{
					shaderMgr->releaseShader(fNonTexturedColorShaderInstance);
				}
				fColorShaderInstance = NULL;
			}
		}
		
	}

	// 1. Initialize phase
	// For this plug-in we simply set up geometry requirements
	// based on an MShaderInstance
	//
	MString initialize(const MInitContext& initContext,
									 MInitFeedback& initFeedback) override
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::initialize");

		if (fColorShaderInstance)
		{
			// This plugin is using the utility method
			// MPxShaderOverride::drawGeometry(). For DX11 drawing,
			// a shader signature is required. We use
			// the signature from the same MShaderInstance used to
			// set the geometry requirements so that the signature
			// will match the requirements.
			//
			addShaderSignature( *fColorShaderInstance );
	
			// Set the geometry requirements based on the shader instance
			setGeometryRequirements( *fColorShaderInstance );
		}
        return MString("Autodesk Maya vp2 Blinn Shader Override");
    }

	// 2. Update Phase
	// Access the node attributes and cache the values to update
	// during updateDevice()
	//
	void updateDG(MObject object) override
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::updateDG");

		if (object == MObject::kNullObj)
			return;

		// Get the hardware shader node from the MObject.
		vp2BlinnShader *shaderNode = (vp2BlinnShader *) MPxHardwareShader::getHardwareShaderPtr( object );		
		if (!shaderNode)
			return;

		// Cache any data from the node to local data members.
		MStatus status;
		MFnDependencyNode node(object, &status);
		if (status)
		{
			node.findPlug("colorR", true).getValue(fDiffuseColor[0]);
			node.findPlug("colorG", true).getValue(fDiffuseColor[1]);
			node.findPlug("colorB", true).getValue(fDiffuseColor[2]);
			node.findPlug("diffuse", true).getValue(fDiffuse);
			node.findPlug("transparency", true).getValue(fTransparency);
			fDiffuseColor[3] = 1.0f - fTransparency;
					
			node.findPlug("specularColorR", true).getValue(fSpecular[0]);
			node.findPlug("specularColorG", true).getValue(fSpecular[1]);
			node.findPlug("specularColorB", true).getValue(fSpecular[2]);
			node.findPlug("specularRollOff", true).getValue(fSpecularRollOff);
			node.findPlug("eccentricity", true).getValue(fEccentricity);

			node.findPlug("nonTexturedColorR", true).getValue(fNonTextured[0]);
			node.findPlug("nonTexturedColorG", true).getValue(fNonTextured[1]);
			node.findPlug("nonTexturedColorB", true).getValue(fNonTextured[2]);
			node.findPlug("nonTexturedTransparency", true).getValue(fNonTexturedTransparency);			
			fNonTextured[3] = 1.0f - fNonTexturedTransparency;
		}
	}

	// 2. Update Phase
	// Call into utility method to update the shader instance
	// when device level update is allowed.
	void updateDevice() override
	{
		updateShaderInstance();
	}

	// 2. Update Phase
	// Transparency hint
	bool isTransparent() override
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::isTransparent");
		return (fTransparency > 0.0f);
	}

	// 2. Update Phase
	// There is nothing to do at the end of update.
	// Method is included for debug tracing only.
	void endUpdate() override
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::endUpdate");
	}

	// 3. Draw Phase
	// Return the shader instance used for rendering
	MHWRender::MShaderInstance* shaderInstance() const override
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::shaderInstance");
		return fColorShaderInstance;
	}

	// 3. Draw Phase
	// Bind the shader on activateKey() and
	// the termination occur in terminateKey().
	void activateKey(MHWRender::MDrawContext& context, const MString& key) override
	{
		MString out("vp2BlinnShaderOverride::activateKey[");
		out += key;
		out += "]";
		TRACE_API_CALLS(out.asChar());

		fColorShaderInstance->bind( context );
	}

	// 3. Draw Phase
	// Use custom shader instance
	//
	bool draw(MHWRender::MDrawContext& context,
				 const MHWRender::MRenderItemList& renderItemList) const override
	{
		MString out("vp2BlinnShaderOverride::draw[Count=");
		out += renderItemList.length();
		out += "]";
		TRACE_API_CALLS(out);

		// Activate all the shader passes and draw using internal draw methods.
		unsigned int passCount = fColorShaderInstance->getPassCount( context );
		for (unsigned int i=0; i<passCount; i++)
		{
			fColorShaderInstance->activatePass( context, i );
			MHWRender::MPxShaderOverride::drawGeometry(context);
		}

		return true;
	}

	// 3. Draw Phase
	// Unbind / terminate the shader instance here.
    void terminateKey(MHWRender::MDrawContext& context, const MString& key) override
	{
		MString out("vp2BlinnShaderOverride::terminateKey[");
		out += key;
		out += "]";
		TRACE_API_CALLS(out.asChar());

		// Unbind the shader
		fColorShaderInstance->unbind( context );
	}

	
	// We are using an internal resources so we support all draw APIs
	// automatically.
	MHWRender::DrawAPI supportedDrawAPIs() const override
	{
		return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
	}

	// Supply a shader instance to be used when in non-textured mode. This
	// allows for identification of objects using the "non-textured" mode color.
	//
	MHWRender::MShaderInstance* nonTexturedShaderInstance(bool &monitor) const override
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::nonTexturedShaderInstance");
		if (fNonTexturedColorShaderInstance)
		{
			monitor = true;
			// Mark whether the shader is transparent or not
			fNonTexturedColorShaderInstance->setIsTransparent( fNonTexturedTransparency > 0.0f );
			fNonTexturedColorShaderInstance->setParameter("diffuse", 0.8f );
			float spec[3] = { 1.0f, 1.0f, 1.0f };
			fNonTexturedColorShaderInstance->setParameter("specularColor", &spec[0]);
			fNonTexturedColorShaderInstance->setParameter("eccentricity", 0.3f);
			fNonTexturedColorShaderInstance->setParameter("specularRollOff", 0.7f);
			return fNonTexturedColorShaderInstance;
		}
		return NULL;
	}

protected:
	//
	// Update the shader using the values cached during DG evaluation
	// Called from updateDevice() during update phase.
	//
	void updateShaderInstance()
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::updateShaderInstance");
		
		if (fColorShaderInstance)
		{
			static bool debugShaderParameters = false;
			if (debugShaderParameters)
			{
				MStringArray list;
				fColorShaderInstance->parameterList(list);
				for (unsigned int i=0; i<list.length(); i++)
				{
					MHWRender::MShaderInstance::ParameterType ptype = fColorShaderInstance->parameterType(list[i]);
					fprintf(stderr, "Param[%d] = %s. type=%d\n", i, list[i].asChar(), ptype);
				}
			}

			// Update shader to mark it as drawing with transparency or not.
			fColorShaderInstance->setIsTransparent( isTransparent() );
			float trans[3] = { fTransparency, fTransparency, fTransparency };		
			fColorShaderInstance->setParameter("transparency", &trans[0]);
			fColorShaderInstance->setParameter("diffuse", fDiffuse);
			fColorShaderInstance->setParameter("color", &fDiffuseColor[0]);
			fColorShaderInstance->setParameter("specularColor", &fSpecular[0]);
			fColorShaderInstance->setParameter("eccentricity", fEccentricity);
			fColorShaderInstance->setParameter("specularRollOff", fSpecularRollOff);
		}
		if (fNonTexturedColorShaderInstance)
		{
			fNonTexturedColorShaderInstance->setIsTransparent( fNonTexturedTransparency > 0.0f );
			float trans[3] = { fNonTexturedTransparency, fNonTexturedTransparency, fNonTexturedTransparency };		
			fNonTexturedColorShaderInstance->setParameter("transparency", &trans[0]);
			fNonTexturedColorShaderInstance->setParameter("color", &fNonTextured[0]);
		}
	}

	// Code to create MShaderInstances using a stock internal Blinn shader
	// Create one shader for textured mode and one for non-textured mode.
	// 
	void createShaderInstance()
	{
		TRACE_API_CALLS("vp2BlinnShaderOverride::createShaderInstance");

		MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
		const MHWRender::MShaderManager* shaderMgr = renderer ? renderer->getShaderManager() : NULL;
		if (!shaderMgr)
			return;

		if (!fColorShaderInstance)
		{
			fColorShaderInstance = shaderMgr->getFragmentShader("mayaBlinnSurface", "outSurfaceFinal", true);
		}
		if (!fNonTexturedColorShaderInstance)
		{
			fNonTexturedColorShaderInstance = shaderMgr->getFragmentShader("mayaBlinnSurface", "outSurfaceFinal", true);
			if (fNonTexturedColorShaderInstance)
			{
				fNonTexturedColorShaderInstance->setParameter("color", &fNonTextured[0]);
			}
		}
	}

	//
	// Constructor. Simply initialize shader instances for usage.
	// 
	vp2BlinnShaderOverride(const MObject& obj)
	: MHWRender::MPxShaderOverride(obj)
	, fColorShaderInstance(NULL)
	, fNonTexturedColorShaderInstance(NULL)
	, fNonTexturedTransparency(0.0f)
	, fTransparency(0.0f)
	, fDiffuse(0.8f)
	, fEccentricity(0.3f)
	, fSpecularRollOff(0.7f)
	{		
		fDiffuseColor[0] = fDiffuseColor[1] = fDiffuseColor[2] = fDiffuseColor[3] = 0.0f;
		fSpecular[0] = fSpecular[1] = fSpecular[2] = 0.0f;
		fNonTextured[0] = 1.0; fNonTextured[1] = fNonTextured[2] = fNonTextured[3] = 0.0f;
		fNonTexturedTransparency = 1.0f;

		// Create a shader instance to use for drawing
		//
		createShaderInstance();
	}

	// Cached shader inputs values
	float fTransparency;
	float fDiffuse;
	float fDiffuseColor[4];
	float fSpecular[3];
	float fSpecularRollOff;
	float fEccentricity;
	float fShininess[3];
	float fNonTextured[4];
	float fNonTexturedTransparency;

	// Shader to use to draw with
	MHWRender::MShaderInstance *fColorShaderInstance;
	// Shader to use to draw non-textured with
	MHWRender::MShaderInstance *fNonTexturedColorShaderInstance;
};

/////////////////////////////////////////////////////////////////////////////////////////
// Plug-in handling
/////////////////////////////////////////////////////////////////////////////////////////
static const MString svp2BlinnShaderRegistrantId("vp2BlinnShaderRegistrantId");

// Note that we use the same drawdb classification for both registerNode()
// and registerShaderOverrideCreator() to associate the override with the Maya node.
//
MStatus initializePlugin( MObject obj )
{
	TRACE_API_CALLS("initializePlugin");
	MStatus   status;

	const MString& swatchName =	MHWShaderSwatchGenerator::initialize();
	const MString UserClassify( "shader/surface/utility/:drawdb/shader/surface/vp2BlinnShader:swatch/"+swatchName);

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");
	status = plugin.registerNode( "vp2BlinnShader", vp2BlinnShader::id,
			                      vp2BlinnShader::creator, vp2BlinnShader::initialize,
								  MPxNode::kHardwareShader, &UserClassify );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	// Register a shader override for this node
	MHWRender::MDrawRegistry::registerShaderOverrideCreator(
		"drawdb/shader/surface/vp2BlinnShader",
		svp2BlinnShaderRegistrantId,
		vp2BlinnShaderOverride::Creator);
	if (status != MS::kSuccess) return status;

	return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj )
{
	TRACE_API_CALLS("uninitializePlugin");
	MStatus   status;

	MFnPlugin plugin( obj );

	// Unregister the shader node
	plugin.deregisterNode( vp2BlinnShader::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	// Deregister the shader override
	status = MHWRender::MDrawRegistry::deregisterShaderOverrideCreator(
		"drawdb/shader/surface/vp2BlinnShader", svp2BlinnShaderRegistrantId);
	if (status != MS::kSuccess) return status;

	return MS::kSuccess;
}



