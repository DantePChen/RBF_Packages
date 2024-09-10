//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include "TessellatedQuadGeomOverride.h"
#include "CrackFreePrimitiveGenerator.h"

#include <maya/MViewport2Renderer.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MFragmentManager.h>
#include <maya/MShaderManager.h>
#include <maya/MTextureManager.h>
#include <maya/MUintArray.h>
#include <maya/MSelectionMask.h>
#include <maya/MGlobal.h>

#include <string.h>
#include <iostream>

namespace
{
	const MString kWireframeItemName("tessellatedQuadWireframe");

	const MString kTessellatedItemName("tessellatedQuadTessellated");
	const MString kTessellatedSelectorItemName("tessellatedQuadTessellatedSelector");

	const MString kTessellatedWireframeItemName("tessellatedQuadTessellatedWireframe");
	const MString kTessellatedWireframeSelectorItemName("tessellatedQuadTessellatedWireframeSelector");

	/*
			3----2 (1,1)
			|   /
			| 0/
			| /  2
			|/  /|
			0  / |
			  /1 |
			 /   |
	(-1,-1)	0----1

	*/
	const unsigned int kNumPoints = 4;
	const unsigned int kNumTris = 2;
	const float	kPlanePnts[kNumPoints][3] = {
		{-1.0, 0.0, -1.0},
		{1.0, 0.0, -1.0},
		{1.0, 0.0, 1.0},
		{-1.0, 0.0, 1.0}
	};
	const unsigned int kPlaneTriVertIndexes[kNumTris][3] = {
		{0, 1, 2},
		{2, 3, 0}
	};
	const float	kPlaneUVs[kNumPoints][2] = {
		{0.0, 1.0},
		{1.0, 1.0},
		{1.0, 0.0},
		{0.0, 0.0}
	};
	const unsigned int kPlaneWireVertIndexes[kNumPoints][2] = {
		{0, 1},
		{1, 2},
		{2, 3},
		{3, 0}
	};
}

void TessellatedQuadGeomOverride::updateRenderItems(
	const MDagPath &path,
	MHWRender::MRenderItemList& list)
{
	const MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	if (!renderer) return;

	const MHWRender::MShaderManager *shaderMgr = renderer->getShaderManager();
	MHWRender::MTextureManager* textureMgr = renderer->getTextureManager();
	if (!shaderMgr || !textureMgr) return;

	MHWRender::MRenderItem *wireframeItem = nullptr;

	int idx = list.indexOf(kWireframeItemName);
	if (idx >= 0)
	{
		wireframeItem = list.itemAt(idx);
	}
	else
	{
		wireframeItem = MHWRender::MRenderItem::Create(kWireframeItemName,
			MHWRender::MRenderItem::NonMaterialSceneItem,
			MHWRender::MGeometry::kLines,
			1);

		wireframeItem->setDrawMode(MHWRender::MGeometry::kAll);

		list.append(wireframeItem);
	}

	if (wireframeItem)
	{
		MHWRender::MShaderInstance *shader = shaderMgr->getStockShader(
			MHWRender::MShaderManager::MStockShader::k3dThickLineShader);

		if (shader)
		{
			MColor wireColor = MHWRender::MGeometryUtilities::wireframeColor(path);
			const float color[4] = { wireColor.r, wireColor.g, wireColor.b, 1.0f };
			shader->setParameter( "solidColor", color);

			wireframeItem->setShader(shader);
			shaderMgr->releaseShader(shader);
		}

		wireframeItem->depthPriority(
			MHWRender::MGeometryUtilities::displayStatus(path) == MHWRender::kDormant ?
			MHWRender::MRenderItem::sDormantWireDepthPriority :
			MHWRender::MRenderItem::sActiveWireDepthPriority);
	}

	const unsigned int triStride = CrackFreePrimitiveGenerator::computeTriangleSize(
		true /*bAddAdjacentEdges*/, true /*bAddDominantEdges*/, true /*bAddDominantPosition*/);

	// The plugin assumes that the devkit location follows "Setting up your build
	// environment" at Maya Developer Help; otherwise, shaders/textures cannot be
	// located. In this case create a mod (module description file) as below in a
	// suitable modules folder (getenv "MAYA_MODULE_PATH") and restart Maya.
	//
	//   + devkit 1.0 <local devkit path>
	//
	MString devkitLocation;
	if (!MGlobal::executeCommand(MString("getModulePath -moduleName \"devkit\""), devkitLocation))
	{
		devkitLocation = MString(getenv("MAYA_LOCATION")) + MString("/devkit");
	}

	MString shaderLocation = devkitLocation +
		MString(renderer->drawAPI() == MHWRender::kDirectX11 ? 
				"/plug-ins/tessellatedQuad/TessellationExample.fx" :
				"/plug-ins/tessellatedQuad/TessellationExample.ogsfx");

	MString textureLocation = devkitLocation +
		MString("/plug-ins/tessellatedQuad/moonHeightMapSharp.jpg");

	if (list.indexOf(kTessellatedItemName) < 0)
	{
		MHWRender::MRenderItem *shadedItem = MHWRender::MRenderItem::Create(kTessellatedItemName,
			MHWRender::MRenderItem::MaterialSceneItem,
			MHWRender::MGeometry::kPatch,
			triStride);

		shadedItem->setDrawMode(MHWRender::MGeometry::DrawMode(
			MHWRender::MGeometry::kShaded | MHWRender::MGeometry::kTextured));

		// do not allow selection on this render item
		MSelectionMask nullMask;
		shadedItem->setSelectionMask(nullMask);

		shadedItem->setExcludedFromPostEffects(false);
		shadedItem->castsShadows(true);
		shadedItem->receivesShadows(true);

		list.append(shadedItem);

		MHWRender::MShaderInstance* tessellationShader =
			shaderMgr->getEffectsFileShader(shaderLocation, "Main");

		if (tessellationShader)
		{
			MHWRender::MTexture* texture = textureMgr->acquireTexture(textureLocation, "");

			MTextureAssignment assignment;
			assignment.texture = texture;
			tessellationShader->setParameter("gHeightMap", assignment);

			shadedItem->setShader(tessellationShader);

			textureMgr->releaseTexture(texture);
			shaderMgr->releaseShader(tessellationShader);
		}
		else
		{
			MString er = shaderMgr->getLastError();
			MString erSrc = shaderMgr->getLastErrorSource(true, true);
			std::cerr << "Error when loading: " << er << std::endl;
			std::cerr << erSrc << std::endl;
		}
	}

	// set up selection item for Viewport 2.0 selection
	if (list.indexOf(kTessellatedSelectorItemName) < 0)
	{
		MHWRender::MRenderItem* selectionItem = MHWRender::MRenderItem::Create(kTessellatedSelectorItemName,
			MHWRender::MRenderItem::MaterialSceneItem,
			MHWRender::MGeometry::kPatch,
			triStride);

		selectionItem->setDrawMode(MHWRender::MGeometry::DrawMode(
			MHWRender::MGeometry::kShaded |
			MHWRender::MGeometry::kTextured |
			MHWRender::MGeometry::kSelectionOnly));

		list.append(selectionItem);

		MHWRender::MShaderInstance* selectionShader =
			shaderMgr->getEffectsFileShader(shaderLocation, "Main_Selection");

		if (selectionShader)
		{
			MHWRender::MTexture* texture = textureMgr->acquireTexture(textureLocation, "");

			MTextureAssignment assignment;
			assignment.texture = texture;
			selectionShader->setParameter("gHeightMap", assignment);

			selectionItem->setShader(selectionShader);

			textureMgr->releaseTexture(texture);
			shaderMgr->releaseShader(selectionShader);
		}
		else
		{
			MString er = shaderMgr->getLastError();
			MString erSrc = shaderMgr->getLastErrorSource(true, true);
			std::cerr << "Error when loading: " << er << std::endl;
			std::cerr << erSrc << std::endl;
		}
	}

	if (list.indexOf(kTessellatedWireframeItemName) < 0)
	{
		MHWRender::MRenderItem *shadedItem = MHWRender::MRenderItem::Create(
			kTessellatedWireframeItemName,
			MHWRender::MRenderItem::MaterialSceneItem,
			MHWRender::MGeometry::kPatch,
			triStride);

		shadedItem->setDrawMode(MHWRender::MGeometry::kWireframe);

		// do not allow selection on this render item
		MSelectionMask nullMask;
		shadedItem->setSelectionMask(nullMask);

		shadedItem->setExcludedFromPostEffects(false);
		shadedItem->castsShadows(true);
		shadedItem->receivesShadows(true);

		list.append(shadedItem);

		MHWRender::MShaderInstance* tessellationShader =
			shaderMgr->getEffectsFileShader(shaderLocation, "Wireframe");

		if (tessellationShader)
		{
			MHWRender::MTexture* texture = textureMgr->acquireTexture(textureLocation, "");

			MTextureAssignment assignment;
			assignment.texture = texture;
			tessellationShader->setParameter("gHeightMap", assignment);

			shadedItem->setShader(tessellationShader);

			textureMgr->releaseTexture(texture);
			shaderMgr->releaseShader(tessellationShader);
		}
		else
		{
			MString er = shaderMgr->getLastError();
			MString erSrc = shaderMgr->getLastErrorSource(true, true);
			std::cerr << "Error when loading: " << er << std::endl;
			std::cerr << erSrc << std::endl;
		}
	}

	// set up selection item for Viewport 2.0 selection
	if (list.indexOf(kTessellatedWireframeSelectorItemName) < 0)
	{
		MHWRender::MRenderItem* selectionItem = MHWRender::MRenderItem::Create(kTessellatedWireframeSelectorItemName,
			MHWRender::MRenderItem::MaterialSceneItem,
			MHWRender::MGeometry::kPatch,
			triStride);

		selectionItem->setDrawMode(MHWRender::MGeometry::DrawMode(
			MHWRender::MGeometry::kWireframe |
			MHWRender::MGeometry::kSelectionOnly));

		list.append(selectionItem);

		MHWRender::MShaderInstance* selectionShader =
			shaderMgr->getEffectsFileShader(shaderLocation, "Wireframe_Selection");

		if (selectionShader)
		{
			MHWRender::MTexture* texture = textureMgr->acquireTexture(textureLocation, "");

			MTextureAssignment assignment;
			assignment.texture = texture;
			selectionShader->setParameter("gHeightMap", assignment);

			selectionItem->setShader(selectionShader);

			textureMgr->releaseTexture(texture);
			shaderMgr->releaseShader(selectionShader);
		}
		else
		{
			MString er = shaderMgr->getLastError();
			MString erSrc = shaderMgr->getLastErrorSource(true, true);
			std::cerr << "Error when loading: " << er << std::endl;
			std::cerr << erSrc << std::endl;
		}
	}
}

void TessellatedQuadGeomOverride::populateGeometry(
	const MHWRender::MGeometryRequirements &requirements,
	const MHWRender::MRenderItemList &renderItems,
	MHWRender::MGeometry &data)
{
	MHWRender::MVertexBuffer *positionBuffer = NULL;
	MHWRender::MVertexBuffer *uvBuffer = NULL;

	const MHWRender::MVertexBufferDescriptorList &vbDescriptors = requirements.vertexRequirements();
	MHWRender::MVertexBufferDescriptor vbDescriptor;
	for ( int ri = 0; ri < vbDescriptors.length(); ++ri )
	{
		if ( !vbDescriptors.getDescriptor( ri, vbDescriptor ) )
			continue;

		switch ( vbDescriptor.semantic() )
		{
		case MHWRender::MGeometry::kPosition:
			{
				positionBuffer = data.createVertexBuffer(vbDescriptor);
				if (positionBuffer)
				{
					float *verts = (float *) positionBuffer->acquire(kNumPoints, false);
					memcpy(verts, kPlanePnts, sizeof(float) * kNumPoints * 3);
					positionBuffer->commit(verts);
				}
			}
			break;
		case MHWRender::MGeometry::kNormal:
			{
				MHWRender::MVertexBuffer *vb = data.createVertexBuffer(vbDescriptor);
				if (vb)
				{
					float *normals= (float *) vb->acquire(kNumPoints, true);
					for ( unsigned int ni=0; ni<kNumPoints; ++ni )
					{
						normals[ni*3 + 0] = 0.0f;
						normals[ni*3 + 1] = 1.0f;
						normals[ni*3 + 2] = 0.0f;
					}
					vb->commit(normals);
				}
			}
			break;
		case MHWRender::MGeometry::kTexture:
			{
				uvBuffer = data.createVertexBuffer(vbDescriptor);
				if (uvBuffer)
				{
					float *uvs = (float *) uvBuffer->acquire(kNumPoints, false);
					memcpy(uvs, kPlaneUVs, sizeof(float) * kNumPoints * 2);
					uvBuffer->commit(uvs);
				}		
			}
			break;
		default:
			// ignore unknown semantics!
			std::cerr << "Unexpected vb descriptor: " << vbDescriptor.semanticName() << std::endl;
			break;
		};
	}

	// For our index buffer, we need to generate connectivity information. The
	// calculation can be expensive so we share the index buffer among render
	// items if possible to avoid repetitive calculation.
	//
	MHWRender::MIndexBuffer *patchIB = nullptr;

	const unsigned int triStride = CrackFreePrimitiveGenerator::computeTriangleSize(
		true /*bAddAdjacentEdges*/, true /*bAddDominantEdges*/, true /*bAddDominantPosition*/);

	for ( int ri = 0; ri < renderItems.length(); ++ri )
	{
		const MHWRender::MRenderItem* item = renderItems.itemAt(ri);
		if ( !item )
			continue;

		MString itemName = item->name();

		if ( itemName == kWireframeItemName )
		{
			MHWRender::MIndexBuffer *ib = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
			if (ib)
			{
                unsigned int* indexes = (unsigned int *) ib->acquire(kNumPoints * 2, false);
				memcpy(indexes, kPlaneWireVertIndexes, sizeof(unsigned int) * kNumPoints * 2);
				ib->commit(indexes);
				item->associateWithIndexBuffer(ib);
			}
		}
		else if (
			itemName == kTessellatedItemName ||
			itemName == kTessellatedSelectorItemName ||
			itemName == kTessellatedWireframeItemName ||
			itemName == kTessellatedWireframeSelectorItemName )
		{
			if (!patchIB)
			{
				patchIB = data.createIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
				if (patchIB)
				{
					MUintArray triIndices(kPlaneTriVertIndexes[0], kNumTris * 3);
					float * positionBufferFloat = (float *)positionBuffer->map();
					float * uvBufferFloat = NULL;
					if (uvBuffer) uvBufferFloat = (float *)uvBuffer->map();

					const unsigned int bufferSize = kNumTris * triStride;

					unsigned int* indexes = (unsigned int*)patchIB->acquire(bufferSize, true /*writeOnly - we don't need the current buffer values*/);

					CrackFreePrimitiveGenerator::mutateIndexBuffer(triIndices,
						positionBufferFloat,
						uvBufferFloat,
						true /*add adjacent edges*/,
						true /*add dominant edges*/,
						true /*add dominant position*/,
						patchIB->dataType(),
						indexes);

					if (positionBuffer) positionBuffer->unmap();
					if (uvBuffer) uvBuffer->unmap();

					patchIB->commit(indexes);
				}
			}

			if (patchIB)
			{
				item->associateWithIndexBuffer(patchIB);
			}
		}
	}
}
