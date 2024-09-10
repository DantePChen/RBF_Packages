///////////////////////////////////////////////////////////////////
//
//
// DESCRIPTION:
//		This example uses a cube-environment map
//		to perform per pixel Phong shading.
//
//		The light direction is currently fixed at the eye
//		position. This could be changed to track an actual
//		light but has not been coded for this example.
//
//		If multiple lights are to be supported, than the environment
//		map would need to be looked up for each light either
//		using multitexturing or multipass.
//
//		This example also supports animated attributes.
//
///////////////////////////////////////////////////////////////////

#ifdef WIN32
#pragma warning( disable : 4786 )		// Disable STL warnings.
#endif

#include <maya/MIOStream.h>
#include <math.h>
#include <cstdlib>

#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MSceneMessage.h>
#include <maya/MUserData.h>
#include <maya/MUintArray.h>

#include <maya/MHWShaderSwatchGenerator.h>
#include <maya/MRenderUtilities.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MGeometryData.h>
#include <maya/MImage.h>

#include <maya/MMatrix.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxShaderOverride.h>
#include <maya/MDrawContext.h>
#include <maya/MStateManager.h>
#include <maya/MViewport2Renderer.h>
#include <maya/MTextureManager.h>
#include <maya/MShaderManager.h>

#include <maya/MGLdefinitions.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MGL.h>

#undef ENABLE_TRACE_API_CALLS
#ifdef ENABLE_TRACE_API_CALLS
#define TRACE_API_CALLS(x) cerr << "hwPhongShader: "<<(x)<<"\n"
#else
#define TRACE_API_CALLS(x)
#endif

#ifndef GL_EXT_texture_cube_map
# define GL_NORMAL_MAP_EXT                   0x8511
# define GL_REFLECTION_MAP_EXT               0x8512
# define GL_TEXTURE_CUBE_MAP_EXT             0x8513
# define GL_TEXTURE_BINDING_CUBE_MAP_EXT     0x8514
# define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT  0x8515
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT  0x8516
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT  0x8517
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT  0x8518
# define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT  0x8519
# define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT  0x851A
# define GL_PROXY_TEXTURE_CUBE_MAP_EXT       0x851B
# define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT    0x851C
#endif

#include "hwPhongShader.h"
#include "hwPhongShaderBehavior.h"

// Set this env var to print vertex id and face id for both VP1 and VP2.
static const bool sTestVertexIdAndFaceId = (getenv("MAYA_HWPHONG_TEST_VERTEXID_AND_FACEID") != NULL);
// Set this env var to debug trace consolidation
static const bool sDebugConsolidation = (getenv("MAYA_HWPHONG_TEST_DEBUG_CONSOLIDATION") != NULL);
static const bool sTestLocalUVCoord = (getenv("MAYA_HWPHONG_TEST_LOCALUVCOORD") != NULL);
static const bool sTestIndexing = (getenv("MAYA_HWPHONG_TEST_INDEXING") != NULL);

MTypeId hwPhongShader::id( 0x00105449 );

MObject  hwPhongShader::aColor;
MObject  hwPhongShader::aTransparency;
MObject  hwPhongShader::aDiffuseColor;
MObject  hwPhongShader::aSpecularColor;
MObject  hwPhongShader::aShininessX;
MObject  hwPhongShader::aShininessY;
MObject  hwPhongShader::aShininessZ;
MObject  hwPhongShader::aShininess;
MObject  hwPhongShader::aGeometryShape;

//
// DESCRIPTION:
///////////////////////////////////////////////////////

void hwPhongShader::setTransparency(const float fTransparency)
{
	mTransparency = fTransparency; 
	mAttributesChanged = true;
	mAttributesChangedVP2 = true;
}

void hwPhongShader::setAmbient(const float3 &fambient) 
{ 
	mAmbientColor[0] = fambient[0];
	mAmbientColor[1] = fambient[1];
	mAmbientColor[2] = fambient[2];
	mAttributesChanged = true;
	mAttributesChangedVP2 = true;
}

void hwPhongShader::setDiffuse(const float3 &fDiffuse)
{
	mDiffuseColor[0] = fDiffuse[0];
	mDiffuseColor[1] = fDiffuse[1];
	mDiffuseColor[2] = fDiffuse[2];
	mAttributesChanged = true;
	mAttributesChangedVP2 = true;
}

void hwPhongShader::setSpecular(const float3 &fSpecular)
{
	mSpecularColor[0] = fSpecular[0];
	mSpecularColor[1] = fSpecular[1];
	mSpecularColor[2] = fSpecular[2];
	mAttributesChanged = true;
	mAttributesChangedVP2 = true;
}

void hwPhongShader::setShininess(const float3 &fShininess)
{
	mShininess[0] = fShininess[0];
	mShininess[1] = fShininess[1];
	mShininess[2] = fShininess[2];
	mAttributesChanged = true;
	mAttributesChangedVP2 = true;
}

hwPhongShader::hwPhongShader()
{
	TRACE_API_CALLS("hwPhongShader");

	mAmbientColor[0] = mAmbientColor[1] = mAmbientColor[2] = 0.1f;
	mDiffuseColor[0] = mDiffuseColor[1] = mDiffuseColor[2] = 0.5f;
	mSpecularColor[0] = mSpecularColor[1] = mSpecularColor[2] = 0.5f;
	mShininess[0] = mShininess[1] = mShininess[2] = 100.0f;
	mAttributesChanged = false;
	markAttributesChangedVP2();

	mGeometryShape = 0;
	mTransparency = 0.0f;
}

hwPhongShader::~hwPhongShader()
{
	TRACE_API_CALLS("~hwPhongShader");
}

static const MString sHWPhongShaderRegistrantId("HWPhongShaderRegistrantId");

// Custom data for using Viewport 2.0 implementation of shader
class hwPhongShaderData : public MUserData
{
public:
	hwPhongShaderData() : MUserData(true) {}
	~hwPhongShaderData() override {}

	MString fPath;
};

//////////////////////////////////////////////////////////////////////////////////////////////
// Viewport 2.0 implementation for the shader
//
// Options:
//
//	- The shader has the option either use a built in shader or using a fixed function texture lookup
//    as is used in the OpenGL based VP1 implementation. (fDrawUsingShader member)
//
//  - The shader has the option to use the built in geometry drawing method drawGeometry() or a
//    custom drawing method called customDraw(). (fUseCustomDraw member). From within that draw, information about
//    consolidation, geometry streams and index streams are extracted to perform a custom OpenGL draw.
// 
//  - The shader has the option to debug various information. (fDebugConsolidation) can be used
//	  to debug consolidation information.
//	  
//	- The shader has the option to disable handling of consolidated geometry explicitly (fHandlesConsolidateGeometry)
//
//  - The shader has the option to set custom data which will keep track of a dag path string. (fUseCustomData)
//    See the notes for this code as this will implicitly disable consolidation.
//
//////////////////////////////////////////////////////////////////////////////////////////////
class hwPhongShaderOverride : public MHWRender::MPxShaderOverride
{
public:
	static MHWRender::MPxShaderOverride* Creator(const MObject& obj)
	{
		return new hwPhongShaderOverride(obj);
	}

	~hwPhongShaderOverride() override
	{
		// It is possible for the MPxShaderOverride object to be deleted when
		// the fShaderNode is not.  If this happens we need to be sure that the
		// next hwPhongShaderOverride created sets the parameters on the shader
		// instance at least once, or we'll render with unititialized parameters.
		if (fShaderNode) fShaderNode->markAttributesChangedVP2();
		fShaderNode = NULL;

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
				if (fShadowShaderInstance)
				{
					shaderMgr->releaseShader(fShadowShaderInstance);
				}
				if (fPointLightShadowShaderInstance)
				{
					shaderMgr->releaseShader(fPointLightShadowShaderInstance);
				}
				if (fNTColorShaderInstance)
				{
					shaderMgr->releaseShader(fNTColorShaderInstance);
				}
			}
		}

		fColorShaderInstance = NULL;
		fShadowShaderInstance = NULL;
		fPointLightShadowShaderInstance = NULL;
		fNTColorShaderInstance = NULL;
	}

	// Turn this on to indicate that render items using this shader
	// should not be considered for consolidation.
	bool handlesConsolidatedGeometry() const override
	{
		// Return true to allow consolidation if possible.
		return fHandlesConsolidateGeometry;
	}

	// When vertex id, face id or local uv coord are required, we need to expand the geometric
	// streams so that face id is the unshared version that can be used.
	bool requiresUnsharedGeometricStreams() const override
	{
		return sTestVertexIdAndFaceId || sTestLocalUVCoord || sTestIndexing;
	}

	// Initialize phase
	MString initialize(
		const MInitContext& initContext,
		MInitFeedback& initFeedback) override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::initialize");

		bool setRequirementsFromShader = false;
		if (fColorShaderInstance)
		{
			// Use the requirements for the color shader which is a superset
			// of the requirements for the color and shadow shadowers.
			//
			// - Generally this is the easiest way to set geometry requirements since they
			// are taken directly from the shader. To use the custom primitive types
			// branch this has been disabled so that explicit requirments can be set.
			// - The type and format should be made to exactly match the shader.
			if (setRequirementsFromShader)
				setGeometryRequirements( *fColorShaderInstance );

			// This plugin is using the utility method
			// MPxShaderOverride::drawGeometry(). For DX11 drawing,
			// a shader signature is required. We use
			// the signature from the same MShaderInstance used to
			// set the geometry requirements so that the signature
			// will match the requirements.
			//
			addShaderSignature( *fColorShaderInstance );
		}

		if (!setRequirementsFromShader)
		{
			// Set the geometry requirements for drawing. Only need
			// position and normals.
			MString empty;

			// custom primitive types can be used by shader overrides.
			// This code is a simple example to show the mechanics of how that works.
			// Here we declare a custom custom indexing requirements.
			// The name "customPrimitiveTest" will be used to look up a registered
			// MPxPrimitiveGenerator that will handle the generation of the index buffer.
			// The example primitive generator is registered at startup by this plugin.
			//
			// As part of this example the plugin customPrimitiveGenerator should
			// be loaded. Note that this plugin will only handle polygonal meshes
			// where "Smooth Mesh Preview" has not be turned on. That is it will only
			// rebuild indexing apropriately for the original non-smoothed mesh.
			//
			static bool useCustomPrimitiveGenerator = (getenv("MAYA_USE_CUSTOMPRIMITIVEGENERATOR") != NULL);
			if(useCustomPrimitiveGenerator)
			{
				MString customPrimitiveName("customPrimitiveTest");
				MHWRender::MIndexBufferDescriptor indexingRequirement(
					MHWRender::MIndexBufferDescriptor::kCustom,
					customPrimitiveName,
					MHWRender::MGeometry::kTriangles);

				addIndexingRequirement(indexingRequirement);

				MHWRender::MVertexBufferDescriptor positionDesc(
					empty,
					MHWRender::MGeometry::kPosition,
					MHWRender::MGeometry::kFloat,
					3);
				positionDesc.setSemanticName("customPositionStream");

				MHWRender::MVertexBufferDescriptor normalDesc(
					empty,
					MHWRender::MGeometry::kNormal,
					MHWRender::MGeometry::kFloat,
					3);
				normalDesc.setSemanticName("customNormalStream");

				addGeometryRequirement(positionDesc);
				addGeometryRequirement(normalDesc);
			}
			else
			{
				MHWRender::MVertexBufferDescriptor positionDesc(
					empty,
					MHWRender::MGeometry::kPosition,
					MHWRender::MGeometry::kFloat,
					3);
				// Use the custom semantic name "swizzlePosition"
				// When the vertexBufferMutator plugin is loaded,
				// this will swap the x,y and z values of the vertex buffer.
				positionDesc.setSemanticName("swizzlePosition");

				MHWRender::MVertexBufferDescriptor normalDesc(
					empty,
					MHWRender::MGeometry::kNormal,
					MHWRender::MGeometry::kFloat,
					3);

				addGeometryRequirement(positionDesc);
				addGeometryRequirement(normalDesc);

				if (sTestVertexIdAndFaceId || sTestIndexing)
				{
					MHWRender::MVertexBufferDescriptor vertexIdDesc(
						empty,
						MHWRender::MGeometry::kTexture,
						MHWRender::MGeometry::kFloat,
						1);
					vertexIdDesc.setSemanticName("vertexid");
					addGeometryRequirement(vertexIdDesc);

					MHWRender::MVertexBufferDescriptor faceIdDesc(
						empty,
						MHWRender::MGeometry::kTexture,
						MHWRender::MGeometry::kFloat,
						1);
					faceIdDesc.setSemanticName("faceid");
					addGeometryRequirement(faceIdDesc);
				}

				if (sTestLocalUVCoord || sTestIndexing)
				{
					MHWRender::MVertexBufferDescriptor localUVCoordDesc(
						empty,
						MHWRender::MGeometry::kTexture,
						MHWRender::MGeometry::kFloat,
						2);
					localUVCoordDesc.setSemanticName("localuvcoord");
					addGeometryRequirement(localUVCoordDesc);
				}
			}
		}

		// Store path name as string to show Maya source on draw
		// NOTE 1: We cannot use the path to access the DAG object during the
		//         draw callback since that could trigger DG evaluation, we
		//         just use the string for debugging info.
		// NOTE 2: Adding custom data here makes consolidation of objects
		//         sharing the same hwPhongShader impossible. Performance will
		//         suffer in that case. If we set handlesConsolidatedGeometry()
		//		   to be true then we don't want to add in the user data.
		if (!handlesConsolidatedGeometry() && fUseCustomData)
		{
			hwPhongShaderData* data = new hwPhongShaderData();
			data->fPath = initContext.dagPath.fullPathName();
			initFeedback.customData = data;
		}
		else
		{
			initFeedback.customData = NULL;
		}
        return MString("Autodesk Maya hwPhongShaderOverride");
    }

	// Update phase
	void updateDG(MObject object) override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::updateDG");

		if (object != MObject::kNullObj)
		{
			// Get the hardware shader node from the MObject.
			fShaderNode = (hwPhongShader *) MPxHwShaderNode::getHwShaderNodePtr( object );
			
			if (fShaderNode)
			{
				MStatus status;
			    MFnDependencyNode node(object, &status);

				if (status)
				{
					node.findPlug("transparency", true).getValue(fTransparency);
					fShaderNode->setTransparency(fTransparency);

					node.findPlug("colorR", true).getValue(fAmbient[0]);
					node.findPlug("colorG", true).getValue(fAmbient[1]);
					node.findPlug("colorB", true).getValue(fAmbient[2]);
					fShaderNode->setAmbient(fAmbient);

					node.findPlug("diffuseColorR", true).getValue(fDiffuse[0]);
					node.findPlug("diffuseColorG", true).getValue(fDiffuse[1]);
					node.findPlug("diffuseColorB", true).getValue(fDiffuse[2]);
					float3 fDiffuse_RGB = {fDiffuse[0], fDiffuse[1], fDiffuse[2]};
					fShaderNode->setDiffuse(fDiffuse_RGB);
					fDiffuse[3] = 1.0f - fTransparency;
					
					node.findPlug("specularColorR", true).getValue(fSpecular[0]);
					node.findPlug("specularColorG", true).getValue(fSpecular[1]);
					node.findPlug("specularColorB", true).getValue(fSpecular[2]);
					fShaderNode->setSpecular(fSpecular);

					node.findPlug("shininessX", true).getValue(fShininess[0]);
					node.findPlug("shininessY", true).getValue(fShininess[1]);
					node.findPlug("shininessZ", true).getValue(fShininess[2]);
					fShaderNode->setShininess(fShininess);
				}
			}
			else
			{
				fTransparency = 0.0f;
			}	
		}
	}

	void updateDevice() override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::updateDevice");

		updateShaderInstance();
	}

	void endUpdate() override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::endUpdate");
	}

	// Draw phase
	bool handlesDraw(MHWRender::MDrawContext& context) override
	{
		const MHWRender::MPassContext & passCtx = context.getPassContext();
		const MString & passId = passCtx.passIdentifier();
		const MStringArray & passSem = passCtx.passSemantics();

		fInShadowPass = false;
		fInPointLightShadowPass = false;
		fInColorPass = false;

		// Enable to debug what is occuring in handlesDraw()
		//
		bool debugHandlesDraw = false;
		if (debugHandlesDraw)
			printf("In hwPhong shader handlesDraw(). Pass Identifier = %s\n", passId.asChar());
		bool handlePass = false;
		for (unsigned int i=0; i<passSem.length(); i++)
		{
			if (passSem[i] == MHWRender::MPassContext::kColorPassSemantic)
			{
				bool hasOverrideShader = passCtx.hasShaderOverride();
				if (!hasOverrideShader)
				{
					if (debugHandlesDraw)
						printf("-> handle semantic[%d][%s]\n", i, passSem[i].asChar());
					handlePass = true;
					fInColorPass = true;
				}
			}
			else if (passSem[i] == MHWRender::MPassContext::kShadowPassSemantic)
			{
				// Only if we can load in a shadow shader will we handle
				// the shadow pass.
				//
				if (fShadowShaderInstance)
					handlePass = true;
				if (debugHandlesDraw)
					printf("-> handle semantic[%d][%s] = %d\n", i, passSem[i].asChar(), handlePass);

				// Remember that we are currently drawing in a shadow pass
				fInShadowPass = true;
			}
			else if (passSem[i] == MHWRender::MPassContext::kPointLightShadowPassSemantic)
			{
				// Only if we can load in a point light shadow shader will we handle
				// the point light shadow pass.
				//
				if (fPointLightShadowShaderInstance)
					handlePass = true;
				if (debugHandlesDraw)
					printf("-> handle semantic[%d][%s] = %d\n", i, passSem[i].asChar(), handlePass);

				// Remember that we are currently drawing in a point light shadow pass
				fInPointLightShadowPass = true;
			}
			else if (passSem[i] == MHWRender::MPassContext::kDepthPassSemantic)
			{
				if (debugHandlesDraw)
					printf("-> don't handle semantic[%d][%s]\n", i, passSem[i].asChar());
				handlePass = false;
			}
			else if (passSem[i] == MHWRender::MPassContext::kNormalDepthPassSemantic)
			{
				if (debugHandlesDraw)
					printf("-> don't handle semantic[%d][%s]\n", i, passSem[i].asChar());
				handlePass = false;
			}
			else
			{
				if (debugHandlesDraw)
					printf("-> additional semantic[%d][%s]\n", i, passSem[i].asChar());
			}
		}

		// Any other passes, don't override drawing
		//
		return handlePass;
	}

	MHWRender::MShaderInstance* shaderInstance() const override
	{
		if (fInColorPass)
		{
			// If color pass
			if (fColorShaderInstance)
				return fColorShaderInstance;
		}
		else if (fInShadowPass)
		{
			// If shadow pass
			if (fShadowShaderInstance)
				return fShadowShaderInstance;
		}
		else if (fInPointLightShadowPass)
		{
			// If point light shadow pass
			if (fPointLightShadowShaderInstance)
				return fPointLightShadowShaderInstance;
		}

		return NULL;
	}

	// Example of using an MShaderInstance for draw.
	// We make the activation occur in activateKey() and
	// the termination occur in terminateKey().
    void activateKey(MHWRender::MDrawContext& context, const MString& key) override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::activateKey");

		fShaderBound = false;
		if (fInColorPass && fColorShaderInstance)
		{
			fColorShaderInstance->bind( context );
			fShaderBound = true;
		}
		else if (fInShadowPass && fShadowShaderInstance)
		{
			// Update the parameters on the shadow shader
			MMatrix viewProj = context.getMatrix(MHWRender::MFrameContext::kViewProjMtx);
			fShadowShaderInstance->setParameter("shadowViewProj", viewProj );
			fShadowShaderInstance->bind( context );
			fShaderBound = true;
		}
		else if (fInPointLightShadowPass && fPointLightShadowShaderInstance)
		{
			// Update the parameters on the point light shadow shader
			fPointLightShadowShaderInstance->bind( context );
			fShaderBound = true;
		}
	}

	// Example of using MShaderInstace to draw. Terminate
	// the shader instance here.
    void terminateKey(MHWRender::MDrawContext& context, const MString& key) override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::terminateKey");

		if (fShaderBound)
		{
			if (fInColorPass && fColorShaderInstance)
			{
				fColorShaderInstance->unbind( context );
			}
			else if (fInShadowPass && fShadowShaderInstance)
			{
				fShadowShaderInstance->unbind( context );
			}
			else if (fInPointLightShadowPass && fPointLightShadowShaderInstance)
			{
				fPointLightShadowShaderInstance->unbind( context );
			}
		}
		fShaderBound = false;
	}

	// Dump out indexing information
	//
	static void dumpIndexing(const MHWRender::MRenderItem* renderItem)
	{
		if (!renderItem)
			return;

		MUintArray indexRange;
		if (renderItem->isConsolidated())
		{
			MHWRender::MGeometryIndexMapping geometryIndexMapping;
			renderItem->sourceIndexMapping(geometryIndexMapping);

			// Debug the ranges if geometry was consolidated
			for (int i=0; i<geometryIndexMapping.geometryCount(); i++)
			{
				MDagPath path = geometryIndexMapping.dagPath(i);
				MObject comp = geometryIndexMapping.component(i);
				int indexStart = geometryIndexMapping.indexStart(i);
				int indexLength = geometryIndexMapping.indexLength(i);

				// Keep track of end of range
				if (indexRange.length() == 0)
					indexRange.append(indexStart);
				indexRange.append(indexStart+indexLength);

				fprintf(stderr, "Have consolidated geometry[%d]. Path=%s, index start=%d, index length=%d\n",
					i, path.fullPathName().asChar(), indexStart, indexLength);
			}
		}
		else
		{
			MDagPath sourceDagPath = renderItem->sourceDagPath();
			const MHWRender::MGeometry* geometry = renderItem->geometry();
			const MHWRender::MIndexBuffer* buffer = geometry ? geometry->indexBuffer(0) : NULL;
			unsigned int indexCount = buffer ? buffer->size() : 0;
			indexRange.append(0);
			indexRange.append(indexCount);
			fprintf(stderr, "Have unconsolidated geometry. Path=%s. index size=%d\n",
						sourceDagPath.fullPathName().asChar(), indexCount);
		}


		// Dump out indexing information
		//
		// Note that we can have both
		// 32 or 16-bit indexing. The latter will occur when the indexing
		// references a consolidated batch of geometry.
		//
		const MHWRender::MGeometry* geometry = renderItem->geometry();
		if (!geometry) 
			return;

		if (sTestIndexing)
		{
			const MHWRender::MIndexBuffer* buffer = geometry->indexBuffer(0);
			unsigned int indexBufferCount = 0;
			GLuint *indexBufferId = NULL;
			MHWRender::MGeometry::Primitive indexPrimType = renderItem->primitive();
			void *indexHandle = buffer->resourceHandle();
			if (indexHandle)
			{
				indexBufferId = (GLuint *)(indexHandle);
				indexBufferCount = buffer->size();
				{
					fprintf(stderr, "IndexingPrimType(%s), IndexType(%s), IndexCount(%d), Handle(%d)\n",
						MHWRender::MGeometry::primitiveString(indexPrimType).asChar(),
						MHWRender::MGeometry::dataTypeString(buffer->dataType()).asChar(),
						indexBufferCount,
						*indexBufferId);
				}
			}

			MHWRender::MIndexBuffer* nonConstIB = const_cast<MHWRender::MIndexBuffer*>(buffer);
			if (buffer->dataType() == MHWRender::MGeometry::kUnsignedInt32)
			{
				const unsigned int *ptr = (const unsigned int*)nonConstIB->map();
				for (unsigned int r=0; r<indexRange.length()-1; r++)
				{
					unsigned start = indexRange[r];
					unsigned end = indexRange[r+1];
					for (unsigned int i=start; i<end; i++)
					{
						const unsigned int index = ptr[i];
						fprintf(stderr, "object[%d], triangle[%d], index[%d] = %d\n", r, (i-start)/3, i, index);
					}
				}
				nonConstIB->unmap();
			}
			else
			{
				const unsigned short *ptr = (const unsigned short*)nonConstIB->map();
				for (unsigned int r=0; r<indexRange.length()-1; r++)
				{
					unsigned start = indexRange[r];
					unsigned end = indexRange[r+1];
					for (unsigned int i=start; i<end; i++)
					{
						const unsigned short index = ptr[i];
						fprintf(stderr, "object[%d], triangle[%d], index[%d] = %d\n", r, (i-start)/3, i, index);
					}
				}
				nonConstIB->unmap();
			}

		}
	}

	// Dump out extra attribute information for vertex ids, face ids,
	// and local uv coordinates
	static void dumpAttributeInformation(const MHWRender::MRenderItem* renderItem)
	{
		const MHWRender::MGeometry* geometry = renderItem->geometry();
		if (!geometry) 
			return;

		// Dump vertexid and faceid.
		if (sTestVertexIdAndFaceId)
		{
			for (int vbIdx=0; vbIdx<geometry->vertexBufferCount(); vbIdx++)
			{
				const MHWRender::MVertexBuffer* vb = geometry->vertexBuffer(vbIdx);
				if (!vb) continue;

				const MHWRender::MVertexBufferDescriptor& desc = vb->descriptor();
				if (desc.dimension() != 1) continue;

				MString semanticName = desc.semanticName();
				if (semanticName != "vertexid" && semanticName != "faceid") continue;

				// Cancel constness to map buffer and dump data.
				MHWRender::MVertexBuffer* nonConstVB = const_cast<MHWRender::MVertexBuffer*>(vb);
				const float *ptr = (const float*)nonConstVB->map();
				for (unsigned int k=0; k<vb->vertexCount(); k++)
				{
					fprintf(stderr, "%s[%d] = %f\n", semanticName.asChar(), k, ptr[k] );
				}
				nonConstVB->unmap();
			}
		}

		// Dump localUVCoord.
		if (sTestLocalUVCoord)
		{
			for (int vbIdx = 0; vbIdx < geometry->vertexBufferCount(); vbIdx++)
			{
				const MHWRender::MVertexBuffer* vb = geometry->vertexBuffer(vbIdx);
				if (!vb) continue;

				const MHWRender::MVertexBufferDescriptor& desc = vb->descriptor();
				if (desc.dimension() != 2) continue;

				MString semanticName = desc.semanticName();
				if (semanticName != "localuvcoord") continue;

				// Cancel constness to map buffer and dump data.
				MHWRender::MVertexBuffer* nonConstVB = const_cast<MHWRender::MVertexBuffer*>(vb);
				const float *ptr = (const float*)nonConstVB->map();
				for (unsigned int k = 0; k < vb->vertexCount(); k++)
				{
					fprintf(stderr, "%s[%d] = (%g, %g)\n", semanticName.asChar(), k, ptr[2*k], ptr[2*k + 1]);
				}
				nonConstVB->unmap();
			}
		}
	}

	bool draw(
		MHWRender::MDrawContext& context,
		const MHWRender::MRenderItemList& renderItemList) const override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::draw");

		// Check if we are drawing consolidate geometry or not
		//
		if ( fDebugConsolidation || sTestVertexIdAndFaceId || sTestLocalUVCoord || sTestIndexing )
		{
			int numRenderItems = renderItemList.length();
			for (int renderItemIdx=0; renderItemIdx<numRenderItems; renderItemIdx++)
			{
				const MHWRender::MRenderItem* renderItem = renderItemList.itemAt(renderItemIdx);
				if (!renderItem) continue;

				if ( fDebugConsolidation || sTestIndexing )
				{
					hwPhongShaderOverride::dumpIndexing(renderItem);
				}
				if (sTestVertexIdAndFaceId || sTestLocalUVCoord || sTestIndexing)
				{
					hwPhongShaderOverride::dumpAttributeInformation(renderItem);
				}
			}
		}

		MHWRender::MStateManager* stateMgr = context.getStateManager();

		//initialize hwPhongShader blend state once
		if(sBlendState == NULL)
		{
			//initilize blend state
			MHWRender::MBlendStateDesc blendStateDesc;

			for(int i = 0; i < (blendStateDesc.independentBlendEnable ? MHWRender::MBlendState::kMaxTargets : 1); ++i)
			{
				blendStateDesc.targetBlends[i].blendEnable = true;
	   			blendStateDesc.targetBlends[i].sourceBlend = MHWRender::MBlendState::kSourceAlpha;
				blendStateDesc.targetBlends[i].destinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
				blendStateDesc.targetBlends[i].blendOperation = MHWRender::MBlendState::kAdd;
	   			blendStateDesc.targetBlends[i].alphaSourceBlend = MHWRender::MBlendState::kOne;
				blendStateDesc.targetBlends[i].alphaDestinationBlend = MHWRender::MBlendState::kInvSourceAlpha;
				blendStateDesc.targetBlends[i].alphaBlendOperation = MHWRender::MBlendState::kAdd;
			}

			blendStateDesc.blendFactor[0] = 1.0f;
			blendStateDesc.blendFactor[1] = 1.0f;
			blendStateDesc.blendFactor[2] = 1.0f;
			blendStateDesc.blendFactor[3] = 1.0f;
			sBlendState = stateMgr->acquireBlendState(blendStateDesc);
		}

		bool debugDrawContext = false;
		bool exists;
		int val = MGlobal::optionVarIntValue("hwPhong_debugDrawContext", &exists);
		if (exists)
		{
			debugDrawContext = (val > 0);
		}
		if (debugDrawContext)
			printContextInformation(context);

		const MHWRender::MPassContext & passCtx = context.getPassContext();
		const MStringArray & passSem = passCtx.passSemantics();
		bool debugPassInformation = false;
		if (debugPassInformation)
		{
			const MString & passId = passCtx.passIdentifier();
			printf("hwPhong node drawing in pass[%s], semantic[", passId.asChar());
			for (unsigned int i=0; i<passSem.length(); i++)
				printf(" %s", passSem[i].asChar());
			printf(" ]\n");
		}

		// save old render state
		const MHWRender::MBlendState* pOldBlendState = stateMgr->getBlendState();

		// Have a MShaderInstance bound then just need to draw the geometry
		//
		if (fShaderBound)
		{
			// Draw for color pass
			if (fInColorPass)
			{
				bool needBlending = false;
				if (fTransparency > 0.0f)
				{
					needBlending = true;
					stateMgr->setBlendState(sBlendState);
				}
				unsigned int passCount = fColorShaderInstance->getPassCount( context );
				if (passCount)
				{
					for (unsigned int i=0; i<passCount; i++)
					{
						fColorShaderInstance->activatePass( context, i );
						
						// Draw the geometry through the internal interface instead
						// of drawing via the plugin.
						MHWRender::MPxShaderOverride::drawGeometry(context);
					}
				}
				if (needBlending)
				{
					stateMgr->setBlendState(pOldBlendState);
				}
			}

			// Draw for shadow pass
			else if (fInShadowPass)
			{
				unsigned int passCount = fShadowShaderInstance->getPassCount( context );
				if (passCount)
				{
					for (unsigned int i=0; i<passCount; i++)
					{
						fShadowShaderInstance->activatePass( context, i );
						MHWRender::MPxShaderOverride::drawGeometry(context);
					}
				}
			}

			// Draw for point light shadow pass
			else if (fInPointLightShadowPass)
			{
				unsigned int passCount = fPointLightShadowShaderInstance->getPassCount( context );
				if (passCount)
				{
					for (unsigned int i=0; i<passCount; i++)
					{
						fPointLightShadowShaderInstance->activatePass( context, i );
						MHWRender::MPxShaderOverride::drawGeometry(context);
					}
				}
			}

		}

		return true;
	}

	MHWRender::DrawAPI supportedDrawAPIs() const override
	{
		return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
	}

	bool isTransparent() override
	{
		TRACE_API_CALLS("hwPhongShaderOverride::isTransparent");
		if (fShaderNode)
		{
			return (fTransparency > 0.0f);
		}
		return false;
	}

	// Set a non-texture mode shader which is fixed.
	MHWRender::MShaderInstance* nonTexturedShaderInstance(bool &monitor) const override
	{
		if (fNTColorShaderInstance)
		{
			monitor = false;
			return fNTColorShaderInstance;
		}
		return NULL;
	}

	bool overridesDrawState() override
	{
		return true;
	}

	void debugShaderParameters(const MHWRender::MShaderInstance *shaderInstance)
	{
		MStringArray params;
		shaderInstance->parameterList(params);

		unsigned int numParams = params.length();
		printf("DEBUGGING SHADER, BEGIN PARAM LIST OF LENGTH %d\n", numParams);
		for (unsigned int i=0; i<numParams; i++)
		{
			printf("ParamName='%s', ParamType=", params[i].asChar());
			switch (shaderInstance->parameterType(params[i]))
			{
			case MHWRender::MShaderInstance::kInvalid:
				printf("'Invalid', ");
				break;
			case MHWRender::MShaderInstance::kBoolean:
				printf("'Boolean', ");
				break;
			case MHWRender::MShaderInstance::kInteger:
				printf("'Integer', ");
				break;
			case MHWRender::MShaderInstance::kFloat:
				printf("'Float', ");
				break;
			case MHWRender::MShaderInstance::kFloat2:
				printf("'Float2', ");
				break;
			case MHWRender::MShaderInstance::kFloat3:
				printf("'Float3', ");
				break;
			case MHWRender::MShaderInstance::kFloat4:
				printf("'Float4', ");
				break;
			case MHWRender::MShaderInstance::kFloat4x4Row:
				printf("'Float4x4Row', ");
				break;
			case MHWRender::MShaderInstance::kFloat4x4Col:
				printf("'Float4x4Col', ");
				break;
			case MHWRender::MShaderInstance::kTexture1:
				printf("'1D Texture', ");
				break;
			case MHWRender::MShaderInstance::kTexture2:
				printf("'2D Texture', ");
				break;
			case MHWRender::MShaderInstance::kTexture3:
				printf("'3D Texture', ");
				break;
			case MHWRender::MShaderInstance::kTextureCube:
				printf("'Cube Texture', ");
				break;
			case MHWRender::MShaderInstance::kSampler:
				printf("'Sampler', ");
				break;
			default:
				printf("'Unknown', ");
				break;
			}
			printf("IsArrayParameter='%s'\n", shaderInstance->isArrayParameter(params[i]) ? "YES" : "NO");
		}
		printf("END PARAM LIST\n");
	}

	// Code to create a cached MShaderInstance
	void createShaderInstance()
	{
		TRACE_API_CALLS("hwPhongShaderOverride::createShaderInstance");

		MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
		if (!renderer)
			return;

		const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
		if (!shaderMgr)
			return;

		bool debugShader = false;

		// If no shadow shader instance created yet acquire one. Use
		// the stock shadow shader provided.
		//
		if (!fShadowShaderInstance)
		{
			fShadowShaderInstance = shaderMgr->getStockShader( MHWRender::MShaderManager::k3dShadowerShader );

			// Some sample debugging code to see what parameters are available.
			if (fShadowShaderInstance)
			{
				if (debugShader)
					debugShaderParameters( fShadowShaderInstance );
			}
			else
			{
				fprintf(stderr, "Failed to load shadower shader for hwPhong\n");
			}
		}

		// If no point light shadow shader instance created yet acquire one. Use
		// the stock shadow shader provided.
		//
		if (!fPointLightShadowShaderInstance)
		{
			fPointLightShadowShaderInstance = shaderMgr->getStockShader( MHWRender::MShaderManager::k3dPointLightShadowerShader );

			// Some sample debugging code to see what parameters are available.
			if (fPointLightShadowShaderInstance)
			{
				if (debugShader)
					debugShaderParameters( fPointLightShadowShaderInstance );
			}
			else
			{
				fprintf(stderr, "Failed to load point light shadower shader for hwPhong\n");
			}
		}

		// If no color shader instance created yet acquire one. For
		// now it's just using an internal shader for convenience but
		// a custom shader could be written here as well.
		if (!fColorShaderInstance)
		{
			fColorShaderInstance = shaderMgr->getStockShader( MHWRender::MShaderManager::k3dBlinnShader );

			// Some sample debugging code to see what parameters are available.
			if (fColorShaderInstance && debugShader)
			{
				debugShaderParameters( fColorShaderInstance );
			}
		}
		if (!fNTColorShaderInstance)
		{
			fNTColorShaderInstance = shaderMgr->getStockShader( MHWRender::MShaderManager::k3dBlinnShader );
			if (fNTColorShaderInstance)
			{
				float val[4] = { 0.3f, 0.5f, 1.0f, 1.0f };
				fNTColorShaderInstance->setParameter("diffuseColor", &val[0]);
			}
		}
	}

	// Code to update a cached MShaderInstance
	//
	void updateShaderInstance()
	{
		TRACE_API_CALLS("hwPhongShaderOverride::updateShaderInstance");

		MHWRender::MRenderer *renderer = MHWRender::MRenderer::theRenderer();
		if (!renderer)
			return;

		// Update the parameters on the color shader
		//
		if (fColorShaderInstance)
		{
			// Update shader to mark it as drawing with transparency or not.
			fColorShaderInstance->setIsTransparent( isTransparent() );

			if (fShaderNode && fShaderNode->attributesChangedVP2())
			{
				fColorShaderInstance->setParameter("emissionColor", &fAmbient[0] );
				fColorShaderInstance->setParameter("diffuseColor", &fDiffuse[0] );
				fColorShaderInstance->setParameter("specularColor", &fSpecular[0] );
				// "specularPower" is set using single-float argument version of setParameter()
				float specPower = fShininess[0];
				fColorShaderInstance->setParameter("specularPower", specPower );

				fShaderNode->markAttributesCleanVP2();
			}
		}
	}

	// Utility code to print the current context information
	//
	static void printContextInformation( const MHWRender::MDrawContext & context )
	{
		TRACE_API_CALLS("hwPhongShaderOverride::printContextInformation");

		// Sample code to print out the information found in MDrawContext
		//
		MDoubleArray dtuple;
		printf("Draw Context Diagnostics {\n");
		{
			dtuple = context.getTuple( MHWRender::MFrameContext::kViewPosition );
			printf("\tView position: %f, %f, %f\n", dtuple[0], dtuple[1], dtuple[2]);
			dtuple = context.getTuple( MHWRender::MFrameContext::kViewPosition );
			printf("\tView dir : %f, %f, %f\n", dtuple[0], dtuple[1], dtuple[2]);
			dtuple = context.getTuple( MHWRender::MFrameContext::kViewUp );
			printf("\tView up : %f, %f, %f\n", dtuple[0], dtuple[1], dtuple[2]);
			dtuple = context.getTuple( MHWRender::MFrameContext::kViewRight );
			printf("\tView right : %f, %f, %f\n", dtuple[0], dtuple[1], dtuple[2]);
			printf("\n");

			MBoundingBox bbox = context.getSceneBox();
			MPoint bmin = bbox.min();
			MPoint bmax = bbox.max();
			printf("\tScene bounding box = %g,%g,%g -> %g,%g,%g\n",
				bmin[0],bmin[1],bmin[2],
				bmax[0],bmax[1],bmax[2]);

			int originX; int originY; int width; int height;
			context.getRenderTargetSize( width, height );
			printf("\tRender target size: %d x %d\n", width, height);
			context.getViewportDimensions( originX, originY, width, height);
			printf("\tViewport dimensions: %d, %d, -> %d, %d\n", originX, originY, width, height);
			MStatus xStatus;
			printf("\tView direction along neg z = %d\n", context.viewDirectionAlongNegZ( &xStatus ));

			// Flag to test getting all scene lights or the subset used for lighting
			//
			static MHWRender::MDrawContext::LightFilter considerAllSceneLights = MHWRender::MDrawContext::kFilteredToLightLimit;
			if (considerAllSceneLights == MHWRender::MDrawContext::kFilteredToLightLimit)
				considerAllSceneLights = MHWRender::MDrawContext::kFilteredIgnoreLightLimit;
			else
				considerAllSceneLights = MHWRender::MDrawContext::kFilteredToLightLimit;

			printf("\tLight Information for %s\n",
				considerAllSceneLights == MHWRender::MDrawContext::kFilteredToLightLimit
				? "only lights clamped to light limit." : "lights not clamped to light limit.");
				
			if ( considerAllSceneLights == MHWRender::MDrawContext::kFilteredIgnoreLightLimit )
				MHWRender::MRenderer::needEvaluateAllLights();

			unsigned int lightCount = context.numberOfActiveLights(considerAllSceneLights);
			MFloatPointArray positions;
			MFloatPoint position;
			MFloatVector direction;
			float intensity = 1.0f;
			MColor color;
			bool hasDirection = false;
			bool hasPosition = false;

			bool visualizeLighting = false;
			// Reuse for each light.
			MMatrix identity;
			if (visualizeLighting )
			{
				for (unsigned int i=0; i<8; i++)
				{
					GLenum light = GL_LIGHT0+i;
					glDisable(light);
				}
				if (!lightCount)
					glDisable(GL_LIGHTING);
				else
					glEnable(GL_LIGHTING);
			}

			for (unsigned int i=0; i<lightCount; i++)
			{
				//
				// Shown here are 2 avenues for accessing light data:
				//
				// If only some basic information which is common to all
				// light types is required then the getLightInformation()
				// interface can be used. It is a simpler interface and
				// in general will be faster to execute.
				//
				// The alternative is to access the lights by parameter
				// via the getLightParameterInformation() interface
				// which allows us to inspect all the per light type specific
				// information (e.g. the drop off on spot lights).
				// As shown it is a bit more complex
				// to use but provides more information.
				//
				bool getCommonParametersOnly = false;

				// Look at common information only
				//
				if (getCommonParametersOnly)
				{
					context.getLightInformation( i,
									 positions, direction,
									 intensity, color, hasDirection,
									 hasPosition,
									 considerAllSceneLights);
					printf("\tLight %d {\n", i);
					printf("\t\tDirectional %d, Positional %d\n", hasDirection, hasPosition);
					printf("\t\tDirection = %g, %g, %g\n", direction[0],direction[1],direction[2]);
					unsigned int positionCount = positions.length();
					if (hasPosition && positionCount)
					{
						for (unsigned int p=0; p<positions.length(); p++)
						{
							printf("\t\tPosition[%d] = %g, %g, %g\n", p, positions[p][0], positions[p][1], positions[p][2]);
							position += positions[p];
						}
						position[0] /= (float)positionCount;
						position[1] /= (float)positionCount;
						position[2] /= (float)positionCount;
					}
					printf("\t\tColor = %g, %g, %g\n", color[0], color[1], color[2]);
					printf("\t\tIntensity = %g\n", intensity);
					printf("\t}\n");
				}

				// Look at all information which may differ per light type
				//
				else
				{
					unsigned int positionCount = 0;
					position[0] = position[1] = position[2] = 0.0f;
					MHWRender::MLightParameterInformation *lightParam =
						context.getLightParameterInformation( i, considerAllSceneLights );
					if (lightParam)
					{
						printf("\tLight %d {\n", i);

						MStringArray params;
						lightParam->parameterList(params);
						for (unsigned int p=0; p<params.length(); p++)
						{
							MString pname = params[p];
							MHWRender::MLightParameterInformation::ParameterType ptype = lightParam->parameterType( pname );
							MFloatArray floatVals;
							MIntArray intVals;
							MMatrix matrixVal;
							MHWRender::MSamplerStateDesc samplerDesc;
							switch (ptype)
							{
							case MHWRender::MLightParameterInformation::kBoolean:
								lightParam->getParameter( pname, intVals );
								printf("\t\tLight parameter %s. Bool[%d]\n", pname.asChar(),
									intVals[0]);
								break;
							case MHWRender::MLightParameterInformation::kInteger:
								lightParam->getParameter( pname, intVals );
								printf("\t\tLight parameter %s. Integer[%d]\n", pname.asChar(),
									intVals[0]);
								break;
							case MHWRender::MLightParameterInformation::kFloat:
								lightParam->getParameter( pname, floatVals );
								printf("\t\tLight parameter %s. Float[%g]\n", pname.asChar(),
									floatVals[0]);
								break;
							case MHWRender::MLightParameterInformation::kFloat2:
								lightParam->getParameter( pname, floatVals );
								printf("\t\tLight parameter %s. Float[%g,%g]\n", pname.asChar(),
									floatVals[0], floatVals[1]);
								break;
							case MHWRender::MLightParameterInformation::kFloat3:
								lightParam->getParameter( pname, floatVals );
								printf("\t\tLight parameter %s. Float3[%g,%g,%g]\n", pname.asChar(),
									floatVals[0], floatVals[1], floatVals[2]);
								break;
							case MHWRender::MLightParameterInformation::kFloat4:
								lightParam->getParameter( pname, floatVals );
								printf("\t\tLight parameter %s. Float4[%g,%g,%g,%g]\n", pname.asChar(),
									floatVals[0], floatVals[1], floatVals[2], floatVals[3]);
								break;
							case MHWRender::MLightParameterInformation::kFloat4x4Row:
								lightParam->getParameter( pname, matrixVal );
								printf("\t\tLight parameter %s. Float4x4Row [%g,%g,%g,%g]\n\t\t[%g,%g,%g,%g]\n\t\t[%g,%g,%g,%g]\n\t\t[%g,%g,%g,%g]\n",
									pname.asChar(),
									matrixVal[0][0], matrixVal[0][1], matrixVal[0][2], matrixVal[0][3],
									matrixVal[1][0], matrixVal[1][1], matrixVal[1][2], matrixVal[1][3],
									matrixVal[2][0], matrixVal[2][1], matrixVal[2][2], matrixVal[2][3],
									matrixVal[3][0], matrixVal[3][1], matrixVal[3][2], matrixVal[3][3]
									);
								break;
							case MHWRender::MLightParameterInformation::kFloat4x4Col:
								lightParam->getParameter( pname, matrixVal );
								printf("\t\tLight parameter %s. Float4x4Row\n", pname.asChar() );
								break;
							case MHWRender::MLightParameterInformation::kTexture2:
							{
								void *handle = lightParam->getParameterTextureHandle( pname );
								printf("\t\tLight texture parameter %s. OpenGL texture id = %d\n", pname.asChar(),
									*((int *)handle));
								break;
							}
							case MHWRender::MLightParameterInformation::kSampler:
								lightParam->getParameter( pname, samplerDesc );
								printf("\t\tLight sampler parameter %s. filter = %d\n", pname.asChar(),
									samplerDesc.filter );
								break;
							default:
								break;
							}

							// Do some discovery to map stock parameters to usable values
							// based on the semantic
							//
							MHWRender::MLightParameterInformation::StockParameterSemantic semantic = lightParam->parameterSemantic( pname );
							switch (semantic)
							{
							case MHWRender::MLightParameterInformation::kLightEnabled:
								printf("\t\t- Parameter semantic : light enabled\n");
								break;
							case MHWRender::MLightParameterInformation::kWorldPosition:
								printf("\t\t- Parameter semantic : world position\n");
								position += MFloatPoint( floatVals[0], floatVals[1], floatVals[2] );
								positionCount++;
								hasPosition = true;
								break;
							case MHWRender::MLightParameterInformation::kWorldDirection:
								printf("\t\t- Parameter semantic : world direction\n");
								direction = MFloatVector( floatVals[0], floatVals[1], floatVals[2] );
								hasDirection = true;
								break;
							case MHWRender::MLightParameterInformation::kIntensity:
								printf("\t\t- Parameter semantic : intensity\n");
								intensity = floatVals[0];
								break;
							case MHWRender::MLightParameterInformation::kColor:
								printf("\t\t- Parameter semantic : color\n");
								color = MColor( floatVals[0], floatVals[1], floatVals[2] );
								break;
							case MHWRender::MLightParameterInformation::kEmitsDiffuse:
								printf("\t\t- Parameter semantic : emits-diffuse\n");
								break;
							case MHWRender::MLightParameterInformation::kEmitsSpecular:
								printf("\t\t- Parameter semantic : emits-specular\n");
								break;
							case MHWRender::MLightParameterInformation::kDecayRate:
								printf("\t\t- Parameter semantic : decay rate\n");
								break;
							case MHWRender::MLightParameterInformation::kDropoff:
								printf("\t\t- Parameter semantic : drop-off\n");
								break;
							case MHWRender::MLightParameterInformation::kCosConeAngle:
								printf("\t\t- Parameter semantic : cosine cone angle\n");
								break;
							case MHWRender::MLightParameterInformation::kShadowMap:
								printf("\t\t- Parameter semantic : shadow map\n");
								break;
							case MHWRender::MLightParameterInformation::kShadowSamp:
								printf("\t\t- Parameter semantic : shadow map sampler\n");
								break;
							case MHWRender::MLightParameterInformation::kShadowBias:
								printf("\t\t- Parameter semantic : shadow map bias\n");
								break;
							case MHWRender::MLightParameterInformation::kShadowMapSize:
								printf("\t\t- Parameter semantic : shadow map size\n");
								break;
							case MHWRender::MLightParameterInformation::kShadowViewProj:
								printf("\t\t- Parameter semantic : shadow map view projection matrix\n");
								break;
							case MHWRender::MLightParameterInformation::kShadowColor:
								printf("\t\t- Parameter semantic : shadow color\n");
								break;
							case MHWRender::MLightParameterInformation::kGlobalShadowOn:
								printf("\t\t- Parameter semantic : global shadows on \n");
								break;
							case MHWRender::MLightParameterInformation::kShadowOn:
								printf("\t\t- Parameter semantic : local shadows on\n");
								break;
							default:
								break;
							}

						}

						// Compute an average position
						if (positionCount > 1)
						{
							position[0] /= (float)positionCount;
							position[1] /= (float)positionCount;
							position[2] /= (float)positionCount;
							printf("\t\tCompute average position [%g,%g,%g]\n", position[0],
								position[1], position[2]);
						}
						//delete lightParam;
						printf("\t}\n");
					}
				}

				MHWRender::MRenderer *theRenderer = MHWRender::MRenderer::theRenderer();
				if (theRenderer && theRenderer->drawAPIIsOpenGL() && visualizeLighting
					&& lightCount < 8)
				{
					GLenum light = GL_LIGHT0+i;

					// Set light parameters
					float ambient[3] = { 0.0f, 0.0f, 0.0f };
					float specular[3] = { 1.0f, 1.0f, 1.0f };
					glLightfv(light, GL_AMBIENT, &ambient[0]);
					color[0] *= intensity;
					color[1] *= intensity;
					color[2] *= intensity;
					glLightfv(light, GL_DIFFUSE, &color[0]);
					glLightfv(light, GL_SPECULAR, &specular[0]);

					glLightf(light,  GL_CONSTANT_ATTENUATION, 1.0f);
					glLightf(light,  GL_LINEAR_ATTENUATION, 0.0f);
					glLightf(light,  GL_QUADRATIC_ATTENUATION, 0.0f);

					glPushMatrix();
					glLoadMatrixd( identity.matrix[0] );

					// Set light position
					if (hasPosition)
						glLightfv(light, GL_POSITION, &position[0]);
					else {
						position[0] = position[1] = position[2] = 0.0f;
						glLightfv(light, GL_POSITION, &position[0]);
					}

					// Set rest of parameters.
					if (hasDirection)
					{
						glLightf(light,  GL_SPOT_CUTOFF, 90.0f);
						glLightf(light,  GL_SPOT_EXPONENT, 64.0f);
						glLightfv(light, GL_SPOT_DIRECTION, &direction[0]);
					}
					else
					{
						glLightf(light, GL_SPOT_CUTOFF, 180.0f);
						glLightf(light, GL_SPOT_EXPONENT, 0.0f);
					}

					glEnable(light);
					glPopMatrix();
				}
			}

		}
		printf("}\n");
	}

protected:
	hwPhongShaderOverride(const MObject& obj)
		: MHWRender::MPxShaderOverride(obj)
		, fShaderNode(NULL)
		, fDebugConsolidation(sDebugConsolidation) // Debug consolidation status during draw
		, fHandlesConsolidateGeometry(true) // Set false to disable geometry consolidation
		, fUseCustomData(false) // Set true to use custom data. Will disable consolidation
		, fShaderBound(false)
		, fInColorPass(false)
		, fColorShaderInstance(NULL)
		, fNTColorShaderInstance(NULL)
		, fInShadowPass(false)
		, fInPointLightShadowPass(false)
		, fShadowShaderInstance(NULL)
		, fPointLightShadowShaderInstance(NULL)
		, fTransparency(0.0f)
	{
#if MAYA_API_VERSION < 201650 
		// Option to disable not available in earlier versions
		fHandlesConsolidateGeometry = true;
#endif

		// Create a shader instance to use for drawing
		//
		createShaderInstance();

		fAmbient[0] = fAmbient[1] = fAmbient[2] = 0.0f;
		fDiffuse[0] = fDiffuse[1] = fDiffuse[2] = fDiffuse[3] = 0.0f;
		fSpecular[0] = fSpecular[1] = fSpecular[2] = 0.0f;
		fShininess[0] = fShininess[1] = fShininess[2] = 500.0f;
	}

	// override blend state when there is blending
    static const MHWRender::MBlendState *sBlendState;

	// Current hwPhongShader node associated with the shader override.
	// Updated during doDG() time.
	hwPhongShader *fShaderNode;
	// Shader inputs values including transparency
	float fTransparency;
	float fAmbient[3];
	float fDiffuse[4];
	float fSpecular[3];
	float fShininess[3];

	// Pass tracking
	bool fInColorPass;
	bool fInShadowPass;
	bool fInPointLightShadowPass;

	// Debug consolidation status during draw
	bool fDebugConsolidation;
	// Use custom data. Disables consolidation
	bool fUseCustomData;
	// Handles consolidated geometry flag
	bool fHandlesConsolidateGeometry;

	// VP2 color shader
	MHWRender::MShaderInstance *fColorShaderInstance;
	// VP2 shadow shader
	MHWRender::MShaderInstance *fShadowShaderInstance;
	// VP2 point light shadow shader
	MHWRender::MShaderInstance *fPointLightShadowShaderInstance;
	// VP2 non-textured shader
	MHWRender::MShaderInstance *fNTColorShaderInstance;
	mutable bool fShaderBound;
};

const MHWRender::MBlendState* hwPhongShaderOverride::sBlendState = NULL;



MStatus initializePlugin( MObject obj )
{
	TRACE_API_CALLS("initializePlugin");
	MStatus   status;

	const MString& swatchName =	MHWShaderSwatchGenerator::initialize();
	const MString UserClassify( "shader/surface/utility/:drawdb/shader/surface/hwPhongShader:swatch/"+swatchName );

	MFnPlugin plugin( obj, PLUGIN_COMPANY, "4.5", "Any");
	status = plugin.registerNode( "hwPhongShader", hwPhongShader::id,
			                      hwPhongShader::creator, hwPhongShader::initialize,
								  MPxNode::kHwShaderNode, &UserClassify );
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	plugin.registerDragAndDropBehavior("hwPhongShaderBehavior",
									   hwPhongShaderBehavior::creator);

	// Register a shader override for this node
	MHWRender::MDrawRegistry::registerShaderOverrideCreator(
		"drawdb/shader/surface/hwPhongShader",
		sHWPhongShaderRegistrantId,
		hwPhongShaderOverride::Creator);
	if (status != MS::kSuccess) return status;

	return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj )
{
	TRACE_API_CALLS("uninitializePlugin");
	MStatus   status;

	MFnPlugin plugin( obj );

	// Unregister all chamelion shader nodes
	plugin.deregisterNode( hwPhongShader::id );
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	plugin.deregisterDragAndDropBehavior("hwPhongShaderBehavior");

	// Deregister the shader override
	status = MHWRender::MDrawRegistry::deregisterShaderOverrideCreator(
		"drawdb/shader/surface/hwPhongShader", sHWPhongShaderRegistrantId);
	if (status != MS::kSuccess) return status;

	return MS::kSuccess;
}

void * hwPhongShader::creator()
{
	TRACE_API_CALLS("creator");
    return new hwPhongShader();
}

MStatus hwPhongShader::initialize()
{
	// Make sure that all attributes are cached internal for
	// optimal performance !

	TRACE_API_CALLS("initialize");
    MFnNumericAttribute nAttr;
	MFnCompoundAttribute cAttr;

    // Create input attributes
    aColor = nAttr.createColor( "color", "c");
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.1f, 0.1f, 0.1f);
	nAttr.setCached( true );
	nAttr.setInternal( true );
	nAttr.setAffectsAppearance( true );
	
	aTransparency = nAttr.create( "transparency", "tr", MFnNumericData::kFloat );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setMin(0.0f);
	nAttr.setCached( true );
	nAttr.setInternal( true );
	nAttr.setAffectsAppearance( true );

    aDiffuseColor = nAttr.createColor( "diffuseColor", "dc" );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.f, 0.5f, 0.5f);
	nAttr.setCached( true );
	nAttr.setInternal( true );
	nAttr.setAffectsAppearance( true );

    aSpecularColor = nAttr.createColor( "specularColor", "sc" );
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0.5f, 0.5f, 0.5f);
	nAttr.setCached( true );
	nAttr.setInternal( true );
	nAttr.setAffectsAppearance( true );

	// This is defined as a compound attribute, users can easily enter
	// values beyond 1.
	aShininessX = nAttr.create( "shininessX", "shx", MFnNumericData::kFloat, 100.0 );
	aShininessY = nAttr.create( "shininessY", "shy", MFnNumericData::kFloat, 100.0 );
	aShininessZ = nAttr.create( "shininessZ", "shz", MFnNumericData::kFloat, 100.0 );
    aShininess = cAttr.create( "shininess", "sh" );
	cAttr.addChild(aShininessX);
	cAttr.addChild(aShininessY);
	cAttr.addChild(aShininessZ) ;
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setCached( true );
	nAttr.setInternal( true );
	nAttr.setAffectsAppearance(true);	
    cAttr.setHidden(false);
	
	aGeometryShape = nAttr.create( "geometryShape", "gs", MFnNumericData::kInt );
    nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(0);
	nAttr.setCached( true );
	nAttr.setInternal( true );

	// create output attributes here
	// outColor is the only output attribute and it is inherited
	// so we do not need to create or add it.
	//

	// Add the attributes here

    addAttribute(aColor);
	addAttribute(aTransparency);
	addAttribute(aDiffuseColor);
	addAttribute(aSpecularColor);
	addAttribute(aShininess);
	addAttribute(aGeometryShape);

    attributeAffects (aColor,			outColor);
    attributeAffects (aTransparency,	outColor);
    attributeAffects (aDiffuseColor,	outColor);
	attributeAffects (aSpecularColor,	outColor);
	attributeAffects (aShininessX,		outColor);
	attributeAffects (aShininessY,		outColor);
	attributeAffects (aShininessZ,		outColor);
	attributeAffects (aShininess,		outColor);

    return MS::kSuccess;
}


// DESCRIPTION:
//
MStatus hwPhongShader::compute(
const MPlug&      plug,
      MDataBlock& block )
{
	TRACE_API_CALLS("compute");

    if ((plug != outColor) && (plug.parent() != outColor))
		return MS::kUnknownParameter;

	MFloatVector & color  = block.inputValue( aDiffuseColor ).asFloatVector();

    // set output color attribute
    MDataHandle outColorHandle = block.outputValue( outColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
	outColor = color;

    outColorHandle.setClean();
    return MS::kSuccess;
}

/* virtual */
bool hwPhongShader::setInternalValue( const MPlug &plug, const MDataHandle &handle )
{
	if (plug == aColor)
	{
		float3 & val = handle.asFloat3();
		if (val[0] != mAmbientColor[0] ||
			val[1] != mAmbientColor[1] ||
			val[2] != mAmbientColor[2])
		{
			mAmbientColor[0] = val[0];
			mAmbientColor[1] = val[1];
			mAmbientColor[2] = val[2];
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aTransparency)
	{
		float val = handle.asFloat();
		if (val != mTransparency)
		{
			mTransparency = val;
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aDiffuseColor)
	{
		float3 & val = handle.asFloat3();
		if (val[0] != mDiffuseColor[0] ||
			val[1] != mDiffuseColor[1] ||
			val[2] != mDiffuseColor[2])
		{
			mDiffuseColor[0] = val[0];
			mDiffuseColor[1] = val[1];
			mDiffuseColor[2] = val[2];
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aSpecularColor)
	{
		float3 & val = handle.asFloat3();
		if (val[0] != mSpecularColor[0] ||
			val[1] != mSpecularColor[1] ||
			val[2] != mSpecularColor[2])
		{
			mSpecularColor[0] = val[0];
			mSpecularColor[1] = val[1];
			mSpecularColor[2] = val[2];
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aShininessX)
	{
		float val = handle.asFloat();
		if (val != mShininess[0])
		{
			mShininess[0] = val; 
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aShininessY)
	{
		float val = handle.asFloat();
		if (val != mShininess[1])
		{
			mShininess[1] = val; 
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aShininessZ)
	{
		float val = handle.asFloat();
		if (val != mShininess[2])
		{
			mShininess[2] = val; 
			mAttributesChanged = true;
			mAttributesChangedVP2 = true;
		}
	}
	else if (plug == aGeometryShape)
	{
		mGeometryShape = handle.asInt();
	}

	return false;
}

/* virtual */
bool
hwPhongShader::getInternalValue( const MPlug& plug, MDataHandle& handle )
{
	if (plug == aColor)
	{
		handle.set( mAmbientColor[0], mAmbientColor[1], mAmbientColor[2] );
	}
	if (plug == aTransparency)
	{
		handle.set( mTransparency );
	}
	else if (plug == aDiffuseColor)
	{
		handle.set( mDiffuseColor[0], mDiffuseColor[1], mDiffuseColor[2] );
	}
	else if (plug == aSpecularColor)
	{
		handle.set( mSpecularColor[0], mSpecularColor[1], mSpecularColor[2] );
	}
	else if (plug == aShininessX)
	{
		handle.set( mShininess[0] );
	}
	else if (plug == aShininessY)
	{
		handle.set( mShininess[1] );
	}
	else if (plug == aShininessZ)
	{
		handle.set( mShininess[2] );
	}
	else if (plug == aGeometryShape)
	{
		handle.set( (int) mGeometryShape );
	}
	return false;
}

////////////////////////////////////////////////////////////////////////////////////
// Swatch rendering: 
// Does not matter the mode for the viewport VP1 or VP2
// Uses material viewer utility which uses the VP2 render to draw the swatch.
////////////////////////////////////////////////////////////////////////////////////
MStatus hwPhongShader::renderSwatchImage(MImage & outImage)
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

/* virtual */
int	hwPhongShader::normalsPerVertex()
{
	TRACE_API_CALLS("normalsPerVertex");
	return 1;
}

/* virtual */
int	hwPhongShader::texCoordsPerVertex()
{
	TRACE_API_CALLS("texCoordsPerVertex");
	return 0;
}

/* virtual */
int	hwPhongShader::getTexCoordSetNames(MStringArray& names)
{
	return 0;
}

/* virtual */
bool hwPhongShader::hasTransparency()
{
	return (mTransparency > 0.0f);
}

/* virtual */
bool hwPhongShader::provideVertexIDs()
{
	return sTestVertexIdAndFaceId || sTestIndexing;
}

/* virtual */
bool hwPhongShader::provideFaceIDs()
{
	return sTestVertexIdAndFaceId || sTestIndexing;
}

/* virtual */
bool hwPhongShader::provideLocalUVCoord()
{
	return sTestLocalUVCoord || sTestIndexing;
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
