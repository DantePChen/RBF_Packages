//-
// ==========================================================================
// Copyright 2019 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

//////////////////////////////////////////////////////////////////////////////
//
// apiMeshSubSceneOverride.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "apiMeshSubSceneOverride.h"
#include "apiMeshShape.h"
#include "apiMeshGeom.h"

#include <maya/M3dView.h>
#include <maya/MDagPathArray.h>
#include <maya/MDrawRegistry.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MGLFunctionTable.h>
#include <maya/MGlobal.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MHardwareRenderer.h>
#include <maya/MItSelectionList.h>
#include <maya/MObject.h>
#include <maya/MObjectArray.h>
#include <maya/MPlugArray.h>
#include <maya/MPxComponentConverter.h>
#include <maya/MSelectionContext.h>
#include <maya/MSelectionList.h>
#include <maya/MShaderManager.h>
#include <maya/MUserData.h>
#include <maya/MViewport2Renderer.h>

#include <algorithm>
#include <iterator>
#include <memory>

#ifdef _WINDOWS
#include <d3d11.h>
#endif

namespace apiMeshSubSceneOverrideHelpers
{
	// Helper class for link lost callback
	class ShadedItemUserData : public MUserData
	{
	public:
		ShadedItemUserData(apiMeshSubSceneOverride* ov)
		: MUserData(false), fOverride(ov) {}
		~ShadedItemUserData() override { fOverride = NULL; }

		apiMeshSubSceneOverride* fOverride;
	};

	// Custom user data class to attach to render items
	// to help with viewport 2.0 selection
	class apiMeshHWSelectionUserData : public MUserData
	{
	public:
		apiMeshHWSelectionUserData()
		: MUserData(true) // let Maya clean up
		, fMeshGeom(NULL)
		, fInstanceIndex(0)
		, fFaceViewSelectedStates()
		{
		}

		~apiMeshHWSelectionUserData() override
		{
		}

		apiMeshGeom* fMeshGeom;
		int fInstanceIndex;
		std::unique_ptr<bool[]> fFaceViewSelectedStates;
	};

	// Custom component converter to select components
	// Attached to the vertex, edge and face selection render items
	// respectively apiMeshSubSceneOverride::sVertexSelectionName, apiMeshSubSceneOverride::sEdgeSelectionName
	// and apiMeshSubSceneOverride::sFaceSelectionName
	class simpleComponentConverter : public MHWRender::MPxComponentConverter
	{
	public:
		simpleComponentConverter(MFn::Type componentType, MSelectionMask selectionType)
		: MHWRender::MPxComponentConverter()
		, fComponentType(componentType)
		, fSelectionType(selectionType) {}
		~simpleComponentConverter() override {}

		void initialize(const MHWRender::MRenderItem& renderItem) override
		{
			// Create the component selection object
			fComponentObject = fComponent.create( fComponentType );

			// For face selection, 
			// create a lookup table to match triangle intersection with face id :
			// One face may contains more than one triangle
			if (fComponentType == MFn::kMeshPolygonComponent) {
				apiMeshHWSelectionUserData* selectionData = dynamic_cast<apiMeshHWSelectionUserData*>(renderItem.customData());
				if(selectionData) {
					apiMeshGeom* meshGeom = selectionData->fMeshGeom;
					bool* faceStates = selectionData->fFaceViewSelectedStates.get();

					// Allocate faces lookup table
					unsigned int numTriangles = 0;
					for (int i=0; i<meshGeom->faceCount; ++i) {
						int numVerts = meshGeom->face_counts[i];
						if (numVerts > 2) {
							if (!faceStates || faceStates[i]) {
								numTriangles += numVerts - 2;
							}
						}
					}
					fLookupTable.resize(numTriangles);

					// Fill faces lookup table
					unsigned int idx = 0;
					for (int faceIdx=0; faceIdx<meshGeom->faceCount; ++faceIdx) {
						// ignore degenerate faces
						int numVerts = meshGeom->face_counts[faceIdx];
						if (numVerts > 2) {
							if (!faceStates || faceStates[faceIdx]) {
								for (int v=1; v<numVerts-1; ++v) {
									fLookupTable[idx++] = faceIdx;
								}
							}
						}
					}
				}
			}
		}

		void addIntersection(MHWRender::MIntersection& intersection) override
		{
			// Convert the intersection index, which represent the primitive position in the
			// index buffer, to the correct component id

			// For vertex and edge: the primitive index value is the same as the component id
			// For face: get the face id that matches the triangle index from the lookup table

			if (fComponentType == MFn::kMeshEdgeComponent) {
				// Only accept edge selection intersection on draw instance #2 -- scaled by 2
				// and instance #-1 (when useDrawInstancingOnEdgeSelectionItem is false)
				if (intersection.instanceID() == 1 || intersection.instanceID() == 3)
					return;
			}

			int idx = intersection.index();

			if (fComponentType == MFn::kMeshPolygonComponent) {
				if (idx >= 0 && idx < (int)fLookupTable.size()) {
					idx = fLookupTable[idx];
				}
			}

			fComponent.addElement(idx);
		}

		MObject component() override
		{
			// Return the component object that contains the ids of the selected components
			return fComponentObject;
		}

		MSelectionMask selectionMask() const override
		{
			// This converter is only valid for specified selection type
			return fSelectionType;
		}

		static MPxComponentConverter* creatorVertexSelection()
		{
			// creator function to instanciate a component converter for vertex selection
			MSelectionMask mask;
			mask.setMask(MSelectionMask::kSelectMeshVerts);
			mask.addMask(MSelectionMask::kSelectPointsForGravity);
			return new simpleComponentConverter(MFn::kMeshVertComponent, mask);
		}

		static MPxComponentConverter* creatorEdgeSelection()
		{
			// creator function to instanciate a component converter for edge selection
			return new simpleComponentConverter(MFn::kMeshEdgeComponent, MSelectionMask::kSelectMeshEdges);
		}

		static MPxComponentConverter* creatorFaceSelection()
		{
			// creator function to instanciate a component converter for face selection
			return new simpleComponentConverter(MFn::kMeshPolygonComponent, MSelectionMask::kSelectMeshFaces);
		}

	private:
		const MFn::Type fComponentType;
		const MSelectionMask fSelectionType;

		MFnSingleIndexedComponent fComponent;
		MObject fComponentObject;
		std::vector<int> fLookupTable;
	};

	static const int sViewSelectedInstanceMark = -1;

	// Gather the view-selected face indices of each instance. Use -1 to mark a
	// view-selected instance (but not any of its faces) for easier processing
	// later in shouldDrawInstance().
	bool gatherViewSelectedFaceInfo(ViewSelectedFaceInfo& vsfInfo,
		const MHWRender::MFrameContext& frameContext,
		const MDagPathArray& instances,
		const apiMeshGeom* meshGeom)
	{
		if (!meshGeom || meshGeom->faceCount <= 0)
			return false;

		MString panelName;
		if (frameContext.renderingDestination(panelName) != MFrameContext::k3dViewport)
			return false;

		M3dView view;
		if (!M3dView::getM3dViewFromModelPanel(panelName, view) || !view.viewIsFiltered())
			return false;

		MSelectionList viewSelectedList;
		if (view.filteredObjectList(viewSelectedList))
		{
			unsigned int instanceArrayLength = instances.length();
			for (unsigned int instIdx = 0; instIdx < instanceArrayLength; instIdx++)
			{
				MSelectionList intersectionList;
				intersectionList.add(instances[instIdx]);
				intersectionList.intersect(viewSelectedList, true/*expandToLeaves*/);

				for (MItSelectionList it(intersectionList); !it.isDone(); it.next())
				{
					MDagPath dummyPath;
					MObject comp;
					it.getDagPath(dummyPath, comp);
					if (comp.isNull())
					{
						vsfInfo.emplace(instIdx, sViewSelectedInstanceMark);
					}
					else
					{
						MFnSingleIndexedComponent fnComp(comp);
						if (fnComp.componentType() == MFn::kMeshPolygonComponent)
						{
							MIntArray faceIds;
							fnComp.getElements(faceIds);
							for (unsigned int i = 0; i < faceIds.length(); i++)
							{
								int faceId = faceIds[i];
								if (faceId >= 0 && faceId < meshGeom->faceCount)
								{
									vsfInfo.emplace(instIdx, faceId);
								}
							}
						}
					}
				}
			}
		}

		return true;
	}

	// If an instance has only -1 stored in its ViewSelectedFaceInfo map, it is
	// view-selected but none of its faces is, so the instance should be drawn.
	bool shouldDrawInstance(const ViewSelectedFaceInfo& info, int instIdx)
	{
		bool found = false;

		auto range = info.equal_range(instIdx);
		for (auto i = range.first; i != range.second; ++i)
		{
			if (i->second != sViewSelectedInstanceMark)
			{
				return false;
			}

			found = true;
		}

		return found;
	}
}
using namespace apiMeshSubSceneOverrideHelpers;

// Compile time switching for regular MVertexBuffer management vs. custom
// user buffer management. Just an example to show
// how custom buffers can be used. See the bounding box code below.		
// Note this now works for both OpenGL and DirectX11 Viewport 2.0
static const bool sUseCustomUserBuffersForBoundingBox = true;

static MHWRender::DrawAPI sDrawAPI = MHWRender::kNone;

static bool useSelectHighlight(const MSelectionList& selectedList, const MDagPath& path);
static bool floatApproxEqual(float left, float right);

static const MString sNameSeparator("_");

std::set<std::string> apiMeshSubSceneOverride::sViewSelectedFaceSelectionNames;

const MString apiMeshSubSceneOverride::sWireName("apiMeshWire");
const MString apiMeshSubSceneOverride::sSelectName("apiMeshSelection");
const MString apiMeshSubSceneOverride::sBoxName("apiMeshBox");
const MString apiMeshSubSceneOverride::sSelectedBoxName("apiMeshBoxSelection");
const MString apiMeshSubSceneOverride::sShadedName("apiMeshShaded");
const MString apiMeshSubSceneOverride::sTexturedName("apiMeshTextured");

const MString apiMeshSubSceneOverride::sVertexSelectionName("apiMeshVertexSelection");
const MString apiMeshSubSceneOverride::sEdgeSelectionName("apiMeshEdgeSelection");
const MString apiMeshSubSceneOverride::sFaceSelectionName("apiMeshFaceSelection");

const MString apiMeshSubSceneOverride::sActiveVertexName("apiMeshActiveVertex");
const MString apiMeshSubSceneOverride::sActiveEdgeName("apiMeshActiveEdge");
const MString apiMeshSubSceneOverride::sActiveFaceName("apiMeshActiveFace");

apiMeshSubSceneOverride::apiMeshSubSceneOverride(const MObject& obj)
: MPxSubSceneOverride(obj)
, fObject(obj)
, fMesh(NULL)
, fWireShader(NULL)
, fThickWireShader(NULL)
, fSelectShader(NULL)
, fThickSelectShader(NULL)
, fShadedShader(NULL)
, fVertexComponentShader(NULL)
, fEdgeComponentShader(NULL)
, fFaceComponentShader(NULL)
, fPositionBuffer(NULL)
, fNormalBuffer(NULL)
, fBoxPositionBuffer(NULL)
, fBoxPositionBufferId(0)
, fBoxPositionBufferDX(NULL)
, fBoxIndexBufferId(0)
, fBoxIndexBufferDX(NULL)
, fWireIndexBuffer(NULL)
, fBoxIndexBuffer(NULL)
, fShadedIndexBuffer(NULL)
, fActiveVerticesIndexBuffer(NULL)
, fActiveEdgesIndexBuffer(NULL)
, fActiveFacesIndexBuffer(NULL)
, fThickLineWidth(-1.0)
, fQueuedLineWidth(-1.0)
, fNumInstances(0)
, fIsInstanceMode(false)
, fAreUIDrawablesDirty(true)
, fQueueUpdate(false)
, fUseQueuedLineUpdate(false) // Set to true to run sample line width update code
{
	// get the real apiMesh object from the MObject
	MStatus status;
	MFnDependencyNode node(obj, &status);
	if (status)
	{
		fMesh = dynamic_cast<apiMesh*>(node.userNode());
	}
}

apiMeshSubSceneOverride::~apiMeshSubSceneOverride()
{
	fMesh = NULL;

	// Clear out LinkLost callbacks still in flight:
	// They might get triggered if the shader are destroyed before
	// the render items, and at this point the fOverride member is clearly invalid.
	std::vector<ShadedItemUserData*>::iterator it;
	for (it = fLinkLostCallbackData.begin(); it != fLinkLostCallbackData.end(); ++it)
	{
		ShadedItemUserData* cbData = *it;
		cbData->fOverride = 0;
	}
	fLinkLostCallbackData.clear();

	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	_OPENMAYA_POP_WARNING

	if (renderer)
	{
		const MHWRender::MShaderManager* shaderMgr =
			renderer->getShaderManager();
		if (shaderMgr)
		{
			if (fWireShader) {
				shaderMgr->releaseShader(fWireShader);
				fWireShader = NULL;
			}
			if (fThickWireShader) {
				shaderMgr->releaseShader(fThickWireShader);
				fThickWireShader = NULL;
			}
			if (fSelectShader) {
				shaderMgr->releaseShader(fSelectShader);
				fSelectShader = NULL;
			}
			if (fThickSelectShader) {
				shaderMgr->releaseShader(fThickSelectShader);
				fThickSelectShader = NULL;
			}
			if (fShadedShader) {
				shaderMgr->releaseShader(fShadedShader);
				fShadedShader = NULL;
			}
			if (fVertexComponentShader) {
				shaderMgr->releaseShader(fVertexComponentShader);
				fVertexComponentShader = NULL;
			}
			if (fEdgeComponentShader) {
				shaderMgr->releaseShader(fEdgeComponentShader);
				fEdgeComponentShader = NULL;
			}
			if (fFaceComponentShader) {
				shaderMgr->releaseShader(fFaceComponentShader);
				fFaceComponentShader = NULL;
			}
		}
	}

	deleteBuffers();
}

void apiMeshSubSceneOverride::untrackLinkLostData(ShadedItemUserData* data)
{
	for (size_t i = 0; i < fLinkLostCallbackData.size(); ++i)
	{
		if (fLinkLostCallbackData[i] == data)
		{
			fLinkLostCallbackData[i] = fLinkLostCallbackData.back();
			fLinkLostCallbackData.pop_back();
			break;
		}
	}
}

MHWRender::DrawAPI apiMeshSubSceneOverride::supportedDrawAPIs() const
{
	// this plugin supports both GL and DX
	return (MHWRender::kOpenGL | MHWRender::kDirectX11 | MHWRender::kOpenGLCoreProfile);
}

bool apiMeshSubSceneOverride::requiresUpdate(
	const MHWRender::MSubSceneContainer& container,
	const MHWRender::MFrameContext& frameContext) const
{
	// Nothing in the container, definitely need to update
	if (container.count() == 0)
	{
		return true;
	}

	// Update always. This could be optimized to only update when the
	// actual shape node detects a change.
	return true;
}

void apiMeshSubSceneOverride::update(
	MHWRender::MSubSceneContainer& container,
	const MHWRender::MFrameContext& frameContext)
{
	// Update render items based on current set of instances
	manageRenderItems(container, frameContext,
		fMesh->shapeDirty() || container.count() == 0);

	// Always reset shape dirty flag
	fMesh->resetShapeDirty();
}

bool apiMeshSubSceneOverride::furtherUpdateRequired(
		const MHWRender::MFrameContext& frameContext)
{
	if (fUseQueuedLineUpdate)
	{
		if (!frameContext.inUserInteraction()
			&& !frameContext.userChangingViewContext())
		{
			return fQueueUpdate;
		}
	}
	return false;
}

void apiMeshSubSceneOverride::manageRenderItems(
	MHWRender::MSubSceneContainer& container,
	const MHWRender::MFrameContext& frameContext,
	bool updateGeometry)
{
	// Preamble
	using namespace MHWRender;
	if (!fMesh || fObject.isNull()) return;
	MRenderer* renderer = MRenderer::theRenderer();
	if (!renderer) return;
	const MShaderManager* shaderMgr = renderer->getShaderManager();
	if (!shaderMgr) return;
	MStatus status;
	MFnDagNode node(fObject, &status);
	if (!status) return;
	MDagPathArray instances;
	if (!node.getAllPaths(instances) || instances.length() == 0) return;

	// Constants
	static const float sRed[]   = {1.0f, 0.0f, 0.0f, 1.0f};
	static const float sGreen[] = {0.0f, 1.0f, 0.0f, 1.0f};
	static const float sWhite[] = {1.0f, 1.0f, 1.0f, 1.0f};

	// Set up shared shaders if needed
	if (!fWireShader)
	{
		fWireShader = shaderMgr->getStockShader(
			MShaderManager::k3dSolidShader);
		fWireShader->setParameter("solidColor", sRed);
	}
	if (!fThickWireShader)
	{
		fThickWireShader = shaderMgr->getStockShader(
			MShaderManager::k3dThickLineShader);
		fThickWireShader->setParameter("solidColor", sRed);
	}
	if (!fSelectShader)
	{
		fSelectShader = shaderMgr->getStockShader(
			MShaderManager::k3dSolidShader);
		fSelectShader->setParameter("solidColor", sGreen);
	}
	if (!fThickSelectShader)
	{
		fThickSelectShader = shaderMgr->getStockShader(
			MShaderManager::k3dThickLineShader);
		fThickSelectShader->setParameter("solidColor", sGreen);
	}

	if (!fVertexComponentShader)
	{
		fVertexComponentShader = shaderMgr->getStockShader(
			MShaderManager::k3dFatPointShader);
		fVertexComponentShader->setParameter("solidColor", sWhite);
		const float pointSize[2] = {5.0, 5.0};
		fVertexComponentShader->setParameter("pointSize", pointSize);
	}

	if (!fEdgeComponentShader)
	{
		fEdgeComponentShader = shaderMgr->getStockShader(
			MShaderManager::k3dThickLineShader);
		fEdgeComponentShader->setParameter("solidColor", sWhite);
		const float lineWidth[2] = {2.0, 2.0};
		fEdgeComponentShader->setParameter("lineWidth", lineWidth);
	}

	if (!fFaceComponentShader)
	{
		fFaceComponentShader = shaderMgr->getStockShader(
			MShaderManager::k3dSolidShader);
		fFaceComponentShader->setParameter("solidColor", sWhite);
	}

	if (!fShadedShader)
	{
		fShadedShader = shaderMgr->getStockShader(MShaderManager::k3dBlinnShader);
	}

	// Set up shared geometry if necessary
	if (updateGeometry) rebuildGeometryBuffers();
	if (!fPositionBuffer || !fNormalBuffer || !fBoxPositionBuffer ||
		!fWireIndexBuffer || !fBoxIndexBuffer || !fShadedIndexBuffer)
	{
		return;
	}

	ViewSelectedFaceInfo vsfInfo;
	bool isActiveViewFiltered = gatherViewSelectedFaceInfo(vsfInfo,
		frameContext, instances, fMesh->meshGeomToUse());

	MSelectionList selectedList;
	MGlobal::getActiveSelectionList(selectedList);

    bool anyMatrixChanged = false;
    bool itemsChanged = false;
	unsigned int instanceArrayLength = instances.length();
    unsigned int numInstanceSelected = 0;
    unsigned int numInstanceUnselected = 0;
    unsigned int numInstances = 0;
    MMatrixArray instanceMatrixArray(instanceArrayLength);
    MMatrixArray selectedInstanceMatrixArray(instanceArrayLength);
    MMatrixArray unselectedInstanceMatrixArray(instanceArrayLength);
	for (unsigned int instIdx=0; instIdx<instanceArrayLength; instIdx++)
    {
        // If expecting large numbers of instances, then walking through the whole
        // list of instances every time to look for changes is not efficient
        // enough.  Watching for change events and changing only the required
        // instances should be done instead.  This method of checking for selection
        // status is also not fast.
		const MDagPath& instance = instances[instIdx];
		const unsigned int instanceNum = instance.instanceNumber();
		if (instance.isValid() && instance.isVisible() &&
			(!isActiveViewFiltered || shouldDrawInstance(vsfInfo, instIdx)))
		{
			InstanceInfo instanceInfo(instance.inclusiveMatrix(),
				useSelectHighlight(selectedList, instance));

			InstanceInfoMap::iterator iter = fInstanceInfoCache.find(instanceNum);
			if (iter == fInstanceInfoCache.end() ||
				iter->second.fIsSelected != instanceInfo.fIsSelected ||
				!iter->second.fTransform.isEquivalent(instanceInfo.fTransform))
			{
				if (!fAreUIDrawablesDirty &&
					(iter == fInstanceInfoCache.end() ||
					 !iter->second.fTransform.isEquivalent(instanceInfo.fTransform)))
				{
					fAreUIDrawablesDirty = true;
				}

				fInstanceInfoCache[instanceNum] = instanceInfo;
				anyMatrixChanged = true;
			}

			instanceMatrixArray[numInstances++] = instanceInfo.fTransform;
			if (instanceInfo.fIsSelected)
			{
				selectedInstanceMatrixArray[numInstanceSelected++] = instanceInfo.fTransform;
			}
			else
			{
				unselectedInstanceMatrixArray[numInstanceUnselected++] = instanceInfo.fTransform;
			}
		}
		else
		{
			InstanceInfoMap::iterator iter = fInstanceInfoCache.find(instanceNum);
			if (iter != fInstanceInfoCache.end())
			{
				fInstanceInfoCache.erase(instanceNum);

				fNumInstances--;

				anyMatrixChanged = true;
				fAreUIDrawablesDirty = true;
			}
		}
	}

	instanceMatrixArray.setLength(numInstances);  //collapse to correct length
	selectedInstanceMatrixArray.setLength(numInstanceSelected);
	unselectedInstanceMatrixArray.setLength(numInstanceUnselected);
	if (fNumInstances != numInstances)
	{
		for (unsigned int i=numInstances; i<fNumInstances; i++)
		{
			fInstanceInfoCache.erase(i);
		}

		anyMatrixChanged = true;
		fAreUIDrawablesDirty = true;
		fNumInstances = numInstances;
	}

	if (fAreUIDrawablesDirty)
	{
		fInstance0Transform = instances[0].inclusiveMatrix();
	}

	const bool anyInstanceSelected = numInstanceSelected > 0;
	const bool anyInstanceUnselected = numInstanceUnselected > 0;

	std::set<int> activeVerticesSet;
	std::set<int> activeEdgesSet;
	std::set<int> activeFacesSet;

	if (fMesh->hasActiveComponents())
	{
		MObjectArray activeComponents = fMesh->activeComponents();
		if (activeComponents.length() > 0)
		{
			MFnSingleIndexedComponent fnComponent( activeComponents[0] );
			if (fnComponent.elementCount())
			{
				MIntArray activeIds;
				fnComponent.getElements( activeIds );

				if(fnComponent.componentType() == MFn::kMeshVertComponent)
				{
					for (unsigned int i=0; i<activeIds.length(); ++i)
						activeVerticesSet.insert( activeIds[i] );
				}
				else if(fnComponent.componentType() == MFn::kMeshEdgeComponent)
				{
					for (unsigned int i=0; i<activeIds.length(); ++i)
						activeEdgesSet.insert( activeIds[i] );
				}
				else if(fnComponent.componentType() == MFn::kMeshPolygonComponent)
				{
					for (unsigned int i=0; i<activeIds.length(); ++i)
						activeFacesSet.insert( activeIds[i] );
				}
			}
		}
	}

	// Update index buffer of active items if necessary
	const bool updateActiveItems =
		updateGeometry ||
	   	fActiveVerticesSet != activeVerticesSet ||
	   	fActiveEdgesSet != activeEdgesSet ||
	   	fActiveFacesSet != activeFacesSet;

	fActiveVerticesSet = activeVerticesSet;
	fActiveEdgesSet = activeEdgesSet;
	fActiveFacesSet = activeFacesSet;

	if (updateActiveItems)
		rebuildActiveComponentIndexBuffers();

	const bool anyVertexSelected = !fActiveVerticesSet.empty();
	const bool anyEdgeSelected = !fActiveEdgesSet.empty();
	const bool anyFaceSelected = !fActiveFacesSet.empty();

	if( (anyVertexSelected && fActiveVerticesIndexBuffer == NULL) ||
		(anyEdgeSelected && fActiveEdgesIndexBuffer == NULL) ||
		(anyFaceSelected && fActiveFacesIndexBuffer == NULL) )
		return;

	// Add render items if necessary.  Remove any pre-existing render items
	// that are no longer needed. Assign selection mask to each render item in
	// order to prevent getInstancedSelectionPath() from being called for
	// unqualified selection hits.
	MRenderItem* wireItem = container.find(sWireName);
	if (!wireItem && anyInstanceUnselected)
	{
		wireItem = MRenderItem::Create(
			sWireName,
			MRenderItem::DecorationItem,
			MGeometry::kLines);
		wireItem->setDrawMode(MGeometry::kWireframe);
		wireItem->depthPriority(MRenderItem::sActiveWireDepthPriority);
		wireItem->setShader(fWireShader);
		wireItem->setSelectionMask(MSelectionMask::kSelectMeshes);
		container.add(wireItem);
		itemsChanged = true;
	}
    else if (wireItem && !anyInstanceUnselected)
    {
        container.remove(sWireName);
        wireItem = NULL;
		itemsChanged = true;
    }

	MRenderItem* selectItem = container.find(sSelectName);
	if (!selectItem && anyInstanceSelected)
	{
		selectItem = MRenderItem::Create(
			sSelectName,
			MRenderItem::DecorationItem,
			MGeometry::kLines);
		selectItem->setDrawMode((MGeometry::DrawMode)(MGeometry::kWireframe | MGeometry::kShaded | MGeometry::kTextured));
		selectItem->depthPriority(MRenderItem::sActiveWireDepthPriority);
		selectItem->setShader(fSelectShader);
		selectItem->setSelectionMask(MSelectionMask::kSelectMeshes);
		container.add(selectItem);
		itemsChanged = true;
	}
    else if (selectItem && !anyInstanceSelected)
    {
        container.remove(sSelectName);
        selectItem = NULL;
		itemsChanged = true;
    }

	MRenderItem* boxItem = container.find(sBoxName);
	if (!boxItem && anyInstanceUnselected)
	{
		boxItem = MRenderItem::Create(
			sBoxName,
			MRenderItem::NonMaterialSceneItem,
			MGeometry::kLines);
		boxItem->setDrawMode(MGeometry::kBoundingBox);
		boxItem->setShader(fWireShader);
		boxItem->setSelectionMask(MSelectionMask::kSelectMeshes);
		container.add(boxItem);
		itemsChanged = true;
	}
    else if (boxItem && !anyInstanceUnselected)
    {
        container.remove(sBoxName);
        boxItem = NULL;
		itemsChanged = true;
    }

	MRenderItem* selectedBoxItem = container.find(sSelectedBoxName);
	if (!selectedBoxItem && anyInstanceSelected)
	{
		selectedBoxItem = MRenderItem::Create(
			sSelectedBoxName,
			MRenderItem::NonMaterialSceneItem,
			MGeometry::kLines);
		selectedBoxItem->setDrawMode(MGeometry::kBoundingBox);
		selectedBoxItem->setShader(fSelectShader);
		selectedBoxItem->setSelectionMask(MSelectionMask::kSelectMeshes);
		container.add(selectedBoxItem);
		itemsChanged = true;
	}
    else if (selectedBoxItem && !anyInstanceSelected)
    {
        container.remove(sSelectedBoxName);
        selectedBoxItem = NULL;
		itemsChanged = true;
    }

	MRenderItem* shadedItem = container.find(sShadedName);
	if (!shadedItem)
	{
        // We always want a shaded item
		shadedItem = MRenderItem::Create(
			sShadedName,
			MRenderItem::MaterialSceneItem,
			MGeometry::kTriangles);
		shadedItem->setDrawMode(MGeometry::kShaded);
		shadedItem->setExcludedFromPostEffects(false);
		shadedItem->castsShadows(true);
		shadedItem->receivesShadows(true);
		shadedItem->setSelectionMask(MSelectionMask::kSelectMeshes);
		container.add(shadedItem);
		itemsChanged = true;
	}

	MRenderItem* texturedItem = container.find(sTexturedName);
	if (!texturedItem)
	{
		// We always want a textured item
		texturedItem = MRenderItem::Create(
			sTexturedName,
			MRenderItem::MaterialSceneItem,
			MGeometry::kTriangles);
		texturedItem->setDrawMode(MGeometry::kTextured);
		texturedItem->setExcludedFromPostEffects(false);
		texturedItem->castsShadows(true);
		texturedItem->receivesShadows(true);
		texturedItem->setSelectionMask(MSelectionMask::kSelectMeshes);
		container.add(texturedItem);
		itemsChanged = true;
	}

	// Grab shading node from first component of first instance of the
	// object and use it to get an MShaderInstance. This could be expanded
	// to support full instancing and components if necessary.
	MObject shader;
	MObjectArray sets, comps;
	if (node.getConnectedSetsAndMembers(0, sets, comps, true))
	{
		for (unsigned int i=0; i<sets.length(); i++)
		{
			MFnDependencyNode fnSet(sets[i], &status);
			if (status)
			{
				MPlug shaderPlug = fnSet.findPlug("surfaceShader", true);
				if (!shaderPlug.isNull())
				{
					MPlugArray connectedPlugs;
					shaderPlug.connectedTo(connectedPlugs, true, false);
					if (connectedPlugs.length() > 0)
					{
						shader = connectedPlugs[0].node(&status);
						if (status && !shader.isNull()) break;
					}
				}
			}
		}
	}

	// Check the dirty flag and make necessary update for shader assignment.
	const bool updateMaterial = fMesh->materialDirty();
	fMesh->setMaterialDirty(false);

	// Update shader for shaded item
	if (updateMaterial || !shadedItem->isShaderFromNode())
	{
		ShadedItemUserData *userData = new ShadedItemUserData(this);

		if (!shader.isNull() &&
			shadedItem->setShaderFromNode(shader,
										  instances[0],
										  shadedItemLinkLost,
										  userData,
										  true))
		{
			fLinkLostCallbackData.push_back(userData);
		}
		else
		{
			shadedItem->setShader(fShadedShader);
			delete userData;
		}
	}

	// Update shader for textured item
	if (updateMaterial || !texturedItem->isShaderFromNode())
	{
		ShadedItemUserData *userData = new ShadedItemUserData(this);

		if (!shader.isNull() &&
			texturedItem->setShaderFromNode(shader,
											instances[0],
											shadedItemLinkLost,
											userData,
											false))
		{
			fLinkLostCallbackData.push_back(userData);
		}
		else
		{
			texturedItem->setShader(fShadedShader);
			delete userData;
		}
	}

	// We've enabled update for selection by overriding enableUpdateForSelection().
	// If the subscene update method is called in the selection pass, we can query
	// the selection state information to create render items as needed, instead
	// of pre-creating everything inefficiently.
	const MSelectionInfo* selInfo = frameContext.getSelectionInfo();
	const bool isVertexSelectable =
		selInfo && selInfo->selectableComponent(true, MSelectionMask::kSelectMeshVerts);
	const bool isEdgeSelectable =
		selInfo && selInfo->selectableComponent(true, MSelectionMask::kSelectMeshEdges);
	const bool isFaceSelectable =
		selInfo && selInfo->selectableComponent(true, MSelectionMask::kSelectMeshFaces);

	// render item for vertex selection
	MRenderItem* vertexSelectionItem = container.find(sVertexSelectionName);
	if (!vertexSelectionItem && isVertexSelectable)
	{
		vertexSelectionItem = MRenderItem::Create(
			sVertexSelectionName,
			MRenderItem::DecorationItem,
			MGeometry::kPoints);
		// use for selection only : not visible in viewport
		vertexSelectionItem->setDrawMode(MHWRender::MGeometry::kSelectionOnly);
		// set the selection mask to kSelectMeshVerts : we want the render item to be used for Vertex Components selection
		MSelectionMask mask(MSelectionMask::kSelectMeshVerts);
		mask.addMask(MSelectionMask::kSelectPointsForGravity);
		vertexSelectionItem->setSelectionMask( mask );
		// set selection priority : on top
		vertexSelectionItem->depthPriority(MRenderItem::sSelectionDepthPriority);
		vertexSelectionItem->setShader(fVertexComponentShader);
		container.add(vertexSelectionItem);
		itemsChanged = true;
	}

	// change this value to enable item instancing : same item will be rendered multiple times
	// the edge selection item will be visible in the viewport and rendered 3 times (with different scaling)
	// only the second instance (scale 1.5) will be selectable (see simpleComponentConverter)
	static bool useDrawInstancingOnEdgeSelectionItem = false;

	// render item for edge selection
	MRenderItem* edgeSelectionItem = container.find(sEdgeSelectionName);
	if (!edgeSelectionItem && (isEdgeSelectable || useDrawInstancingOnEdgeSelectionItem))
	{
		// use for selection only : not visible in viewport
		unsigned int drawMode = MHWRender::MGeometry::kSelectionOnly;
		unsigned int depthPriority = MRenderItem::sSelectionDepthPriority;
		if (useDrawInstancingOnEdgeSelectionItem) {
			// display in viewport and in selection
			drawMode = MHWRender::MGeometry::kAll;
			// reduce priority so we can see the active item
			depthPriority = MRenderItem::sActiveWireDepthPriority-1;
		}

		edgeSelectionItem = MRenderItem::Create(
			sEdgeSelectionName,
			MRenderItem::DecorationItem,
			MGeometry::kLines);
		edgeSelectionItem->setDrawMode( (MGeometry::DrawMode)drawMode);
		// set the selection mask to kSelectMeshEdges : we want the render item to be used for Edge Components selection
		edgeSelectionItem->setSelectionMask( MSelectionMask::kSelectMeshEdges );
		edgeSelectionItem->depthPriority(depthPriority);
		edgeSelectionItem->setShader(fWireShader);
		container.add(edgeSelectionItem);
		itemsChanged = true;
	}

	bool updateSelectionUserData = updateGeometry;

	// render item for face selection
	MRenderItem* faceSelectionItem = container.find(sFaceSelectionName);
	if (!faceSelectionItem && isFaceSelectable)
	{
		faceSelectionItem = MRenderItem::Create(
			sFaceSelectionName,
			MRenderItem::DecorationItem,
			MGeometry::kTriangles);
		// use for selection only : not visible in viewport
		faceSelectionItem->setDrawMode(MHWRender::MGeometry::kSelectionOnly);
		// set the selection mask to kSelectMeshFaces : we want the render item to be used for Face Components selection
		faceSelectionItem->setSelectionMask( MSelectionMask::kSelectMeshFaces );
		// set selection priority : on top
		faceSelectionItem->depthPriority(MRenderItem::sSelectionDepthPriority);
		faceSelectionItem->setShader(fFaceComponentShader);
		container.add(faceSelectionItem);
		itemsChanged = true;

		updateSelectionUserData = true;
	}

	// create and add a custom data to help the face component converter
	if (faceSelectionItem && updateSelectionUserData)
	{
		apiMeshHWSelectionUserData* mySelectionData = new apiMeshHWSelectionUserData();
		mySelectionData->fMeshGeom = fMesh->meshGeomToUse();
		faceSelectionItem->setCustomData(mySelectionData);
	}

	// render item to display active (selected) vertices
	MRenderItem* activeVertexItem = container.find(sActiveVertexName);
	if (!activeVertexItem && anyVertexSelected)
	{
		activeVertexItem = MRenderItem::Create(
			sActiveVertexName,
			MRenderItem::DecorationItem,
			MGeometry::kPoints);
		activeVertexItem->setDrawMode(MGeometry::kAll);
		activeVertexItem->depthPriority(MRenderItem::sActivePointDepthPriority);
		activeVertexItem->setShader(fVertexComponentShader);
		activeVertexItem->setSelectionMask(MSelectionMask::kSelectMeshVerts);
		container.add(activeVertexItem);
		itemsChanged = true;
	}
    else if (activeVertexItem && !anyVertexSelected)
    {
		container.remove(sActiveVertexName);
		activeVertexItem = NULL;
		itemsChanged = true;
	}

	// render item to display active (selected) edges
	MRenderItem* activeEdgeItem = container.find(sActiveEdgeName);
	if (!activeEdgeItem && anyEdgeSelected)
	{
		activeEdgeItem = MRenderItem::Create(
			sActiveEdgeName,
			MRenderItem::DecorationItem,
			MGeometry::kLines);
		activeEdgeItem->setDrawMode(MGeometry::kAll);
		activeEdgeItem->depthPriority(MRenderItem::sActiveLineDepthPriority);
		activeEdgeItem->setShader(fEdgeComponentShader);
		activeEdgeItem->setSelectionMask(MSelectionMask::kSelectMeshEdges);
		container.add(activeEdgeItem);
		itemsChanged = true;
	}
    else if (activeEdgeItem && !anyEdgeSelected)
    {
		container.remove(sActiveEdgeName);
		activeEdgeItem = NULL;
		itemsChanged = true;
	}

	// render item to display active (selected) faces
	MRenderItem* activeFaceItem = container.find(sActiveFaceName);
	if (!activeFaceItem && anyFaceSelected)
	{
		activeFaceItem = MRenderItem::Create(
			sActiveFaceName,
			MRenderItem::DecorationItem,
			MGeometry::kTriangles);
		activeFaceItem->setDrawMode(MGeometry::kAll);
		activeFaceItem->depthPriority(MRenderItem::sActiveLineDepthPriority);
		activeFaceItem->setShader(fFaceComponentShader);
		activeFaceItem->setSelectionMask(MSelectionMask::kSelectMeshFaces);
		container.add(activeFaceItem);
		itemsChanged = true;
	}
    else if (activeFaceItem && !anyFaceSelected)
    {
		container.remove(sActiveFaceName);
		activeFaceItem = NULL;
		itemsChanged = true;
	}

    // update the line width on the shader for our wire items if it changed
	float lineWidth = frameContext.getGlobalLineWidth();
	bool userWidthChange = !floatApproxEqual(lineWidth, fThickLineWidth);

	bool doUpdate = false;
	const float targetRefinedLineWidth = 50.0f;
	if (userWidthChange)
	{
        fThickLineWidth = lineWidth;
		doUpdate = true;

		// First user change will trigger an update requirement
		if (fUseQueuedLineUpdate)
		{
			fQueuedLineWidth = lineWidth;
			if (fQueuedLineWidth < targetRefinedLineWidth)
			{
				fQueueUpdate = true;
			}
		}
	}
	else
	{
		// Increment by 1 until we reach the target width.
		// If we haven't reached it yet then queue another update
		// so we can increment and retest against the target width.
		if (fUseQueuedLineUpdate && fQueueUpdate)
		{
			if (fQueuedLineWidth < targetRefinedLineWidth)
			{
				lineWidth = fQueuedLineWidth;
				fQueuedLineWidth++;
				fQueueUpdate = true;
				doUpdate = true;
			}
			else
			{
				// Reached target. Stop asking for more refinement
				fQueueUpdate = false;
			}
		}
	}

	if (doUpdate)
    {
		if (!floatApproxEqual(lineWidth, 1.0f))
	    {
            // Only set the shader if the line width changes (or the first time)
		    float lineWidthArray[2] = {0};
		    lineWidthArray[0] = lineWidth;
		    lineWidthArray[1] = lineWidth;
		    fThickWireShader->setParameter("lineWidth", lineWidthArray);
		    fThickSelectShader->setParameter("lineWidth", lineWidthArray);
            if (wireItem)
		        wireItem->setShader(fThickWireShader);
            if (selectItem)
		        selectItem->setShader(fThickSelectShader);
	    }
	    else
	    {
            if (wireItem)
		        wireItem->setShader(fWireShader);
            if (selectItem)
		        selectItem->setShader(fSelectShader);
	    }
    }

	// Update geometry if required
	if (itemsChanged || updateGeometry)
	{
		MBoundingBox bounds = fMesh->boundingBox();

		MVertexBufferArray wireBuffers;
		wireBuffers.addBuffer("positions", fPositionBuffer);

        if (wireItem)
		    setGeometryForRenderItem(*wireItem, wireBuffers, *fWireIndexBuffer, &bounds);
        if (selectItem)
		    setGeometryForRenderItem(*selectItem, wireBuffers, *fWireIndexBuffer, &bounds);

		MVertexBufferArray boxBuffers;
		boxBuffers.addBuffer("positions", fBoxPositionBuffer);

        if (boxItem)
		    setGeometryForRenderItem(*boxItem, boxBuffers, *fBoxIndexBuffer, &bounds);
        if (selectedBoxItem)
		    setGeometryForRenderItem(*selectedBoxItem, boxBuffers, *fBoxIndexBuffer, &bounds);

		MVertexBufferArray shadedBuffers;
		shadedBuffers.addBuffer("positions", fPositionBuffer);
		shadedBuffers.addBuffer("normals", fNormalBuffer);

		setGeometryForRenderItem(*shadedItem, shadedBuffers, *fShadedIndexBuffer, &bounds);
		setGeometryForRenderItem(*texturedItem, shadedBuffers, *fShadedIndexBuffer, &bounds);

		// Point vertex buffer is fully sequential, use an empty index buffer to draw non-indexed.
		if (vertexSelectionItem)
			setGeometryForRenderItem(*vertexSelectionItem, wireBuffers, MIndexBuffer(MGeometry::kUnsignedInt32), &bounds);
		if (edgeSelectionItem)
			setGeometryForRenderItem(*edgeSelectionItem, wireBuffers, *fWireIndexBuffer, &bounds);
		if (faceSelectionItem)
			setGeometryForRenderItem(*faceSelectionItem, wireBuffers, *fShadedIndexBuffer, &bounds);
	}

	// Update active component items if required
	if (itemsChanged || updateActiveItems)
	{
		MBoundingBox bounds = fMesh->boundingBox();

		MVertexBufferArray vertexBuffer;
		vertexBuffer.addBuffer("positions", fPositionBuffer);

		if (activeVertexItem)
			setGeometryForRenderItem(*activeVertexItem, vertexBuffer, *fActiveVerticesIndexBuffer, &bounds);
		if (activeEdgeItem)
			setGeometryForRenderItem(*activeEdgeItem, vertexBuffer, *fActiveEdgesIndexBuffer, &bounds);
		if (activeFaceItem)
			setGeometryForRenderItem(*activeFaceItem, vertexBuffer, *fActiveFacesIndexBuffer, &bounds);
	}

	// Update render item matrices if necessary
    if (itemsChanged || anyMatrixChanged)
    {
        if (!fIsInstanceMode && numInstances == 1)
        {
            // When not dealing with multiple instances, don't convert the render items into instanced
            // mode.  Set the matrices on them directly.
            MMatrix& objToWorld = instanceMatrixArray[0];

            if (wireItem)
			    wireItem->setMatrix(&objToWorld);
            if (selectItem)
			    selectItem->setMatrix(&objToWorld);
            if (boxItem)
			    boxItem->setMatrix(&objToWorld);
            if (selectedBoxItem)
			    selectedBoxItem->setMatrix(&objToWorld);

			shadedItem->setMatrix(&objToWorld);
			texturedItem->setMatrix(&objToWorld);

			if (vertexSelectionItem)
				vertexSelectionItem->setMatrix(&objToWorld);
			if (edgeSelectionItem)
				edgeSelectionItem->setMatrix(&objToWorld);
			if (faceSelectionItem)
				faceSelectionItem->setMatrix(&objToWorld);

			if (useDrawInstancingOnEdgeSelectionItem) {
				// create/update draw instances
				// first instance : no scaling - won't be selectable see simpleComponentConverter::addIntersection
				MMatrix transform1 = objToWorld;
				transform1[3][3] = 1;  // make sure we don't scale the w
				// second instance : scaled by 2
				MMatrix transform2 = objToWorld * 2;
				transform2[3][3] = 1;  // make sure we don't scale the w
				// third instance : scaled by 3 - won't be selectable see simpleComponentConverter::addIntersection
				MMatrix transform3 = objToWorld * 3;
				transform3[3][3] = 1;  // make sure we don't scale the w

				if (true) {
					MMatrixArray transforms;
					transforms.append(transform1);
					transforms.append(transform2);
					transforms.append(transform3);
					setInstanceTransformArray(*edgeSelectionItem, transforms);
				}
				else {
					// another way to set up the instances
					// useful to get the instance ID
					removeAllInstances(*edgeSelectionItem);
					unsigned int newInstanceId = addInstanceTransform(*edgeSelectionItem, transform1);
					printf("newInstanceId %u\n", newInstanceId);
					newInstanceId = addInstanceTransform(*edgeSelectionItem, transform2);
					printf("newInstanceId %u\n", newInstanceId);
					newInstanceId = addInstanceTransform(*edgeSelectionItem, transform3);
					printf("newInstanceId %u\n", newInstanceId);
				}
			}

			if (activeVertexItem)
				activeVertexItem->setMatrix(&objToWorld);
			if (activeEdgeItem)
				activeEdgeItem->setMatrix(&objToWorld);
			if (activeFaceItem)
				activeFaceItem->setMatrix(&objToWorld);
		}
        else
        {
            // If we have DAG instances of this shape then use the MPxSubSceneOverride instance
            // transform API to set up instance copies of the render items.  This will be faster
            // to render than creating render items for each instance, especially for large numbers
            // of instances.
            // Note: for simplicity, this code is not tracking the actual shaded material binding
            // of the shape.  MPxSubSceneOverride render item instances require that the shader
            // and other properties of the instances all match.  So if we were to bind a shader based
            // on the DAG shape material binding, then we would need one render item per material. We
            // could then group up the instance transforms based matching materials.

            // Note this has to happen after the geometry and shaders are set, otherwise it will fail.
            if (wireItem)
                setInstanceTransformArray(*wireItem, unselectedInstanceMatrixArray);
            if (selectItem)
                setInstanceTransformArray(*selectItem, selectedInstanceMatrixArray);
            if (boxItem)
                setInstanceTransformArray(*boxItem, unselectedInstanceMatrixArray);
            if (selectedBoxItem)
                setInstanceTransformArray(*selectedBoxItem, selectedInstanceMatrixArray);

            setInstanceTransformArray(*shadedItem, instanceMatrixArray);
			setInstanceTransformArray(*texturedItem, instanceMatrixArray);

			if (vertexSelectionItem)
				setInstanceTransformArray(*vertexSelectionItem, instanceMatrixArray);
			if (edgeSelectionItem)
				setInstanceTransformArray(*edgeSelectionItem, instanceMatrixArray);
			if (faceSelectionItem)
				setInstanceTransformArray(*faceSelectionItem, instanceMatrixArray);

			if (activeVertexItem)
				setInstanceTransformArray(*activeVertexItem, instanceMatrixArray);
			if (activeEdgeItem)
				setInstanceTransformArray(*activeEdgeItem, instanceMatrixArray);
			if (activeFaceItem)
				setInstanceTransformArray(*activeFaceItem, instanceMatrixArray);

            // Once we change the render items into instance rendering they can't be changed back without
            // being deleted and re-created.  So if instances are deleted to leave only one remaining,
            // just keep treating them the instance way.
            fIsInstanceMode = true;
        }
    }

	manageIsolateSelectRenderItems(container, frameContext, instances, vsfInfo,
		shader, updateMaterial, updateGeometry);

	if (itemsChanged || anyMatrixChanged || updateGeometry)
    {
	    // On transform or geometry change, force recalculation of shadow maps
	    MRenderer::setLightsAndShadowsDirty();
    }
}

void apiMeshSubSceneOverride::manageIsolateSelectRenderItems(
	MHWRender::MSubSceneContainer& container,
	const MHWRender::MFrameContext& frameContext,
	const MDagPathArray& instances,
	const ViewSelectedFaceInfo& currInfo,
	const MObject& shader,
	const bool updateMaterial,
	const bool updateGeometry)
{
	using namespace MHWRender;

	if (!fMesh) return;

	apiMeshGeom* meshGeom = fMesh->meshGeomToUse();
	if (!meshGeom) return;

	MString destinationName;
	if (frameContext.renderingDestination(destinationName) != MFrameContext::k3dViewport)
	{
		return;
	}

	// Check whether the view-selected faces of the DAG instances changed for the current view
	//
	bool updateViewSelectedFaces = false;

	const std::string activeViewName = destinationName.asChar();
	if (fViewSelectedFaceInfoCache.find(activeViewName) == fViewSelectedFaceInfoCache.end())
	{
		if (!currInfo.empty())
		{
			updateViewSelectedFaces = true;
		}
	}
	else if (fViewSelectedFaceInfoCache[activeViewName] != currInfo)
	{
		updateViewSelectedFaces = true;
	}

	// Create/remove render items for the current view
	//
	if (updateViewSelectedFaces)
	{
		// Gather previous instances which own view-selected faces
		std::set<int> prevInstIdxArray;
		if (fViewSelectedFaceInfoCache.find(activeViewName) != fViewSelectedFaceInfoCache.end())
		{
			const ViewSelectedFaceInfo& prevInfo = fViewSelectedFaceInfoCache[activeViewName];
			for (const auto &p : prevInfo)
			{
				const int instIdx = p.first;
				const int faceIdx = p.second;
				if (faceIdx != sViewSelectedInstanceMark)
				{
					prevInstIdxArray.insert(instIdx);
				}
			}
		}

		// Gather current instances which own view-selected faces
		std::set<int> currInstIdxArray;
		for (const auto &p : currInfo)
		{
			const int instIdx = p.first;
			const int faceIdx = p.second;
			if (faceIdx != sViewSelectedInstanceMark)
			{
				currInstIdxArray.insert(instIdx);
			}
		}

		// Update the cache now that we've gathered the previous data
		fViewSelectedFaceInfoCache[activeViewName] = currInfo;

		// For the DAG instances which don't own view-selected faces any more,
		// their isolate select render items are removed to reduce the number of
		// render items. Plug-ins can also choose to disable the render items if
		// frequent reuse of the render items is expected.
		std::set<int> diffInstIdxArray;
		std::set_difference(
			prevInstIdxArray.begin(), prevInstIdxArray.end(),
			currInstIdxArray.begin(), currInstIdxArray.end(),
			std::inserter(diffInstIdxArray, diffInstIdxArray.begin()));

		for (int instIdx : diffInstIdxArray)
		{
			MString namePostfix = sNameSeparator + instIdx + sNameSeparator + destinationName;

			MString shadedName = sShadedName + namePostfix;
			container.remove(shadedName);

			MString texturedName = sTexturedName + namePostfix;
			container.remove(texturedName);

			MString faceSelectionName = sFaceSelectionName + namePostfix;
			container.remove(faceSelectionName);
		}

		// Create render items for new DAG instances which own view-selected
		// faces since the last update
		for (int instIdx : currInstIdxArray)
		{
			MString namePostfix = sNameSeparator + instIdx + sNameSeparator + destinationName;

			MString shadedName = sShadedName + namePostfix;
			MRenderItem* viewSelectedShadedItem = container.find(shadedName);
			if (!viewSelectedShadedItem)
			{
				viewSelectedShadedItem = MRenderItem::Create(
					shadedName,
					MRenderItem::MaterialSceneItem,
					MGeometry::kTriangles);

				viewSelectedShadedItem->setDrawMode(MGeometry::kShaded);
				viewSelectedShadedItem->setExcludedFromPostEffects(false);
				viewSelectedShadedItem->castsShadows(true);
				viewSelectedShadedItem->receivesShadows(true);
				viewSelectedShadedItem->setSelectionMask(MSelectionMask::kSelectMeshes);
				container.add(viewSelectedShadedItem);

				apiMeshHWSelectionUserData* userData = new apiMeshHWSelectionUserData();
				userData->fMeshGeom = meshGeom;
				userData->fInstanceIndex = instIdx;
				viewSelectedShadedItem->setCustomData(userData);
			}

			MString texturedName = sTexturedName + namePostfix;
			MRenderItem* viewSelectedTexturedItem = container.find(texturedName);
			if (!viewSelectedTexturedItem)
			{
				viewSelectedTexturedItem = MRenderItem::Create(
					texturedName,
					MRenderItem::MaterialSceneItem,
					MGeometry::kTriangles);

				viewSelectedTexturedItem->setDrawMode(MGeometry::kTextured);
				viewSelectedTexturedItem->setExcludedFromPostEffects(false);
				viewSelectedTexturedItem->castsShadows(true);
				viewSelectedTexturedItem->receivesShadows(true);
				viewSelectedTexturedItem->setSelectionMask(MSelectionMask::kSelectMeshes);
				container.add(viewSelectedTexturedItem);

				apiMeshHWSelectionUserData* userData = new apiMeshHWSelectionUserData();
				userData->fMeshGeom = meshGeom;
				userData->fInstanceIndex = instIdx;
				viewSelectedTexturedItem->setCustomData(userData);
			}

			MString faceSelectionName = sFaceSelectionName + namePostfix;
			MRenderItem* viewSelectedFaceSelectionItem = container.find(faceSelectionName);
			if (!viewSelectedFaceSelectionItem)
			{
				viewSelectedFaceSelectionItem = MRenderItem::Create(
					faceSelectionName,
					MRenderItem::DecorationItem,
					MGeometry::kTriangles);

				// use for selection only : not visible in viewport
				viewSelectedFaceSelectionItem->setDrawMode(MGeometry::kSelectionOnly);
				// set the selection mask to kSelectMeshFaces : we want the render item to be used for Face Components selection
				viewSelectedFaceSelectionItem->setSelectionMask(MSelectionMask::kSelectMeshFaces);
				// set selection priority : on top
				viewSelectedFaceSelectionItem->depthPriority(MRenderItem::sSelectionDepthPriority);
				viewSelectedFaceSelectionItem->setShader(fFaceComponentShader);
				container.add(viewSelectedFaceSelectionItem);

				apiMeshHWSelectionUserData* userData = new apiMeshHWSelectionUserData();
				userData->fMeshGeom = meshGeom;
				userData->fInstanceIndex = instIdx;
				viewSelectedFaceSelectionItem->setCustomData(userData);

				// Register component converter for isolate select render items
				const std::string key = faceSelectionName.asChar();
				if (sViewSelectedFaceSelectionNames.find(key) == sViewSelectedFaceSelectionNames.end())
				{
					MDrawRegistry::registerComponentConverter(faceSelectionName,
						simpleComponentConverter::creatorFaceSelection);

					sViewSelectedFaceSelectionNames.emplace(key);
				}
			}
		}
	}

	// Update render items for all views as required
	//
	for (auto& p : fViewSelectedFaceInfoCache)
	{
		const std::string& viewName = p.first;
		const ViewSelectedFaceInfo& faceInfo = p.second;
		const bool isActiveView = (viewName == activeViewName);

		std::set<int> instIdxArray;
		for (auto& q : faceInfo)
		{
			const int instIdx = q.first;
			const int faceIdx = q.second;
			if (faceIdx != sViewSelectedInstanceMark)
			{
				instIdxArray.insert(instIdx);
			}
		}

		for (int instIdx : instIdxArray)
		{
			const MString namePostfix = sNameSeparator + instIdx + sNameSeparator + viewName.c_str();

			MRenderItem* viewSelectedShadedItem = container.find(sShadedName + namePostfix);
			MRenderItem* viewSelectedTexturedItem = container.find(sTexturedName + namePostfix);
			MRenderItem* viewSelectedFaceSelectionItem = container.find(sFaceSelectionName + namePostfix);
			if (!viewSelectedShadedItem || !viewSelectedTexturedItem || !viewSelectedFaceSelectionItem)
			{
				continue;
			}

			// Enable isolate select render items for the active view and disable those for other views
			viewSelectedShadedItem->enable(isActiveView);
			viewSelectedTexturedItem->enable(isActiveView);
			viewSelectedFaceSelectionItem->enable(isActiveView);

			// Update matrix
			const MDagPath& instance = instances[instIdx];
			const MMatrix objToWorld = instance.inclusiveMatrix();
			viewSelectedShadedItem->setMatrix(&objToWorld);
			viewSelectedTexturedItem->setMatrix(&objToWorld);
			viewSelectedFaceSelectionItem->setMatrix(&objToWorld);

			// Perform necessary update for shader, geometry, bounding box
			if (updateViewSelectedFaces || updateMaterial || updateGeometry)
			{
				if (updateMaterial || !viewSelectedShadedItem->isShaderFromNode())
				{
					ShadedItemUserData *userData = new ShadedItemUserData(this);

					if (!shader.isNull() &&
						viewSelectedShadedItem->setShaderFromNode(shader,
							instance,
							shadedItemLinkLost,
							userData,
							true))
					{
						fLinkLostCallbackData.push_back(userData);
					}
					else
					{
						viewSelectedShadedItem->setShader(fShadedShader);
						delete userData;
					}
				}

				if (updateMaterial || !viewSelectedTexturedItem->isShaderFromNode())
				{
					ShadedItemUserData *userData = new ShadedItemUserData(this);

					if (!shader.isNull() &&
						viewSelectedTexturedItem->setShaderFromNode(shader,
							instance,
							shadedItemLinkLost,
							userData,
							false))
					{
						fLinkLostCallbackData.push_back(userData);
					}
					else
					{
						viewSelectedTexturedItem->setShader(fShadedShader);
						delete userData;
					}
				}

				MVertexBufferArray shadedBuffers;
				shadedBuffers.addBuffer("positions", fPositionBuffer);
				shadedBuffers.addBuffer("normals", fNormalBuffer);

				MVertexBufferArray selectionBuffers;
				selectionBuffers.addBuffer("positions", fPositionBuffer);

				std::unique_ptr<bool[]> faceStates(new bool[meshGeom->faceCount]);
				for (int faceIdx = 0; faceIdx < meshGeom->faceCount; faceIdx++)
				{
					faceStates[faceIdx] = false;
				}

				auto range = faceInfo.equal_range(instIdx);
				for (auto it = range.first; it != range.second; it++)
				{
					const int faceIdx = it->second;
					if (faceIdx != sViewSelectedInstanceMark)
					{
						faceStates[faceIdx] = true;
					}
				}

				unsigned int numTriangles = 0;
				for (int faceIdx = 0; faceIdx < meshGeom->faceCount; ++faceIdx)
				{
					int numVerts = meshGeom->face_counts[faceIdx];
					if (numVerts > 2)
					{
						if (faceStates[faceIdx])
						{
							numTriangles += numVerts - 2;
						}
					}
				}

				MIndexBuffer* indexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt32);
				unsigned int* data = (unsigned int*)indexBuffer->acquire(numTriangles * 3, true);
				if (data)
				{
					unsigned int base = 0;
					unsigned int idx = 0;
					for (int faceIdx = 0; faceIdx < meshGeom->faceCount; faceIdx++)
					{
						// Ignore degenerate faces
						int numVerts = meshGeom->face_counts[faceIdx];
						if (numVerts > 2)
						{
							if (faceStates[faceIdx])
							{
								for (int v = 1; v < numVerts - 1; v++)
								{
									data[idx++] = meshGeom->face_connects[base];
									data[idx++] = meshGeom->face_connects[base + v];
									data[idx++] = meshGeom->face_connects[base + v + 1];
								}
							}
							base += numVerts;
						}
					}

					indexBuffer->commit(data);
					data = nullptr;
				}

				const MBoundingBox bounds = fMesh->boundingBox();
				setGeometryForRenderItem(*viewSelectedShadedItem, shadedBuffers, *indexBuffer, &bounds);
				setGeometryForRenderItem(*viewSelectedTexturedItem, shadedBuffers, *indexBuffer, &bounds);
				setGeometryForRenderItem(*viewSelectedFaceSelectionItem, selectionBuffers, *indexBuffer, &bounds);

				// The apiMeshGeom object can be re-created during DG evaluation, thus update the pointer.
				apiMeshHWSelectionUserData* userData;
				userData = dynamic_cast<apiMeshHWSelectionUserData*>(viewSelectedShadedItem->customData());
				if (userData)
				{
					userData->fMeshGeom = meshGeom;
				}

				userData = dynamic_cast<apiMeshHWSelectionUserData*>(viewSelectedTexturedItem->customData());
				if (userData)
				{
					userData->fMeshGeom = meshGeom;
				}

				userData = dynamic_cast<apiMeshHWSelectionUserData*>(viewSelectedFaceSelectionItem->customData());
				if (userData)
				{
					userData->fMeshGeom = meshGeom;
					userData->fFaceViewSelectedStates = std::move(faceStates);
				}
			}
		}
	}
}

void apiMeshSubSceneOverride::rebuildGeometryBuffers()
{
	_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
	MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
	_OPENMAYA_POP_WARNING

	if (sDrawAPI == MHWRender::kNone)
	{
		if (renderer)
		{
			sDrawAPI = renderer->drawAPI();
		}
	}
	
	// Preamble
	using namespace MHWRender;
	apiMeshGeom* meshGeom = fMesh->meshGeomToUse();
	if (!meshGeom) return;
	MBoundingBox bounds = fMesh->boundingBox();

	// Clear old
	deleteGeometryBuffers();

	// Compute mesh data size
	unsigned int numTriangles = 0;
	unsigned int totalVerts = 0;
	for (int i=0; i<meshGeom->faceCount; i++)
	{
		int numVerts = meshGeom->face_counts[i];
		if (numVerts > 2)
		{
			numTriangles += numVerts - 2;
			totalVerts += numVerts;
		}
	}

    if (numTriangles == 0 || totalVerts == 0)
        return;

	// Acquire vertex buffer resources
	const MVertexBufferDescriptor posDesc("", MGeometry::kPosition, MGeometry::kFloat, 3);
	const MVertexBufferDescriptor normalDesc("", MGeometry::kNormal, MGeometry::kFloat, 3);
	fPositionBuffer = new MVertexBuffer(posDesc);
	fNormalBuffer = new MVertexBuffer(normalDesc);
	fBoxPositionBuffer = new MVertexBuffer(posDesc);

	// Generating a compact position buffer will reduce the data size transferred to the
	// video card by leveraging the index buffer capabilities. It will also help with
	// component selection since the vertex ID from the hit record will match one to one with
	// the position in the vertices array.
	float* positions = (float*)fPositionBuffer->acquire(meshGeom->vertices.length(), true);
	float* normals = (float*)fNormalBuffer->acquire(meshGeom->vertices.length(), true);

	float* boxPositions = NULL;
	unsigned short* boxIndices = NULL;
	if (sUseCustomUserBuffersForBoundingBox)
	{
		// Just for demo of custom user buffers make box custom
		static float sBoxPositions[8*3];
		static unsigned short sBoxIndices[24];
		boxPositions = sBoxPositions;
		boxIndices = sBoxIndices;
	}
	else
	{
		boxPositions = (float*)fBoxPositionBuffer->acquire(8, true);
		boxIndices = (unsigned short*)fBoxIndexBuffer->acquire(24, true);
	}

	// Acquire index buffer resources
	fWireIndexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt32);
	fBoxIndexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt16);
	fShadedIndexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt32);

	unsigned int* wireBuffer = (unsigned int*)fWireIndexBuffer->acquire(2*totalVerts, true);
	unsigned int* shadedBuffer = (unsigned int*)fShadedIndexBuffer->acquire(3*numTriangles, true);

	// Sanity check
	if (!positions || !boxPositions || !normals || !wireBuffer || !boxIndices || !shadedBuffer)
	{
		deleteGeometryBuffers();
		return; // FAIL
	}

	// Fill vertex data for shaded/wireframe
	int vid = 0;
	int pid = 0;
	int nid = 0;
	for (unsigned int i=0; i<meshGeom->vertices.length(); i++)
	{
		MPoint position = meshGeom->vertices[i];
		positions[pid++] = (float)position[0];
		positions[pid++] = (float)position[1];
		positions[pid++] = (float)position[2];

		MVector normal = meshGeom->normals[i];
		normals[nid++] = (float)normal[0];
		normals[nid++] = (float)normal[1];
		normals[nid++] = (float)normal[2];
	}
	fPositionBuffer->commit(positions); positions = NULL;
	fNormalBuffer->commit(normals); normals = NULL;

	// Fill vertex and index data for bounding box
	MPoint bbmin = bounds.min();
	MPoint bbmax = bounds.max();
	boxPositions[0]  = (float)bbmin.x; boxPositions[1]  = (float)bbmin.y; boxPositions[2]  = (float)bbmin.z;
	boxPositions[3]  = (float)bbmin.x; boxPositions[4]  = (float)bbmin.y; boxPositions[5]  = (float)bbmax.z;
	boxPositions[6]  = (float)bbmax.x; boxPositions[7]  = (float)bbmin.y; boxPositions[8]  = (float)bbmax.z;
	boxPositions[9]  = (float)bbmax.x; boxPositions[10] = (float)bbmin.y; boxPositions[11] = (float)bbmin.z;
	boxPositions[12] = (float)bbmin.x; boxPositions[13] = (float)bbmax.y; boxPositions[14] = (float)bbmin.z;
	boxPositions[15] = (float)bbmin.x; boxPositions[16] = (float)bbmax.y; boxPositions[17] = (float)bbmax.z;
	boxPositions[18] = (float)bbmax.x; boxPositions[19] = (float)bbmax.y; boxPositions[20] = (float)bbmax.z;
	boxPositions[21] = (float)bbmax.x; boxPositions[22] = (float)bbmax.y; boxPositions[23] = (float)bbmin.z;
	boxIndices[0]  = 0; boxIndices[1]  = 1;
	boxIndices[2]  = 1; boxIndices[3]  = 2;
	boxIndices[4]  = 2; boxIndices[5]  = 3;
	boxIndices[6]  = 3; boxIndices[7]  = 0;
	boxIndices[8]  = 4; boxIndices[9]  = 5;
	boxIndices[10] = 5; boxIndices[11] = 6;
	boxIndices[12] = 6; boxIndices[13] = 7;
	boxIndices[14] = 7; boxIndices[15] = 4;
	boxIndices[16] = 0; boxIndices[17] = 4;
	boxIndices[18] = 1; boxIndices[19] = 5;
	boxIndices[20] = 2; boxIndices[21] = 6;
	boxIndices[22] = 3; boxIndices[23] = 7;
	if (sUseCustomUserBuffersForBoundingBox)
	{
		if (sDrawAPI == MHWRender::kOpenGL ||
			sDrawAPI == MHWRender::kOpenGLCoreProfile)
		{		
			static MGLFunctionTable* gGLFT = NULL;

			_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
			MHardwareRenderer* pRenderer = MHardwareRenderer::theRenderer();

			if (!gGLFT && pRenderer)
			{
				gGLFT = pRenderer->glFunctionTable();
			}
			_OPENMAYA_POP_WARNING
			if (gGLFT)
			{
				gGLFT->glGenBuffersARB(1, &fBoxPositionBufferId);
				if (fBoxPositionBufferId != 0)
				{
					gGLFT->glBindBufferARB(MGL_ARRAY_BUFFER_ARB, fBoxPositionBufferId);
					gGLFT->glBufferDataARB(MGL_ARRAY_BUFFER_ARB, 8*3*sizeof(float), boxPositions, MGL_STATIC_DRAW_ARB);
					gGLFT->glBindBufferARB(MGL_ARRAY_BUFFER_ARB, 0);
					fBoxPositionBuffer->resourceHandle(&fBoxPositionBufferId, 8*3);
				}

				gGLFT->glGenBuffersARB(1, &fBoxIndexBufferId);
				if (fBoxIndexBufferId != 0)
				{
					gGLFT->glBindBufferARB(MGL_ELEMENT_ARRAY_BUFFER_ARB, fBoxIndexBufferId);
					gGLFT->glBufferDataARB(MGL_ELEMENT_ARRAY_BUFFER_ARB, 24*sizeof(unsigned short), boxIndices, MGL_STATIC_DRAW_ARB);
					gGLFT->glBindBufferARB(MGL_ELEMENT_ARRAY_BUFFER_ARB, 0);
					fBoxIndexBuffer->resourceHandle(&fBoxIndexBufferId, 24);
				}
			}
		}
#ifdef _WINDOWS
		else if (sDrawAPI == MHWRender::kDirectX11)
		{
			if (renderer)
			{
				ID3D11Device* pDevice = (ID3D11Device*)renderer->GPUDeviceHandle();

				// Fill in a buffer description.
				D3D11_BUFFER_DESC bufferDesc;
				bufferDesc.Usage            = D3D11_USAGE_DEFAULT;
				bufferDesc.ByteWidth        = sizeof(float) * 3 * 8;
				bufferDesc.BindFlags        = D3D11_BIND_VERTEX_BUFFER;
				bufferDesc.CPUAccessFlags   = 0;
				bufferDesc.MiscFlags        = 0;

				// Fill in the sub-resource data.
				D3D11_SUBRESOURCE_DATA InitData;
				InitData.pSysMem = boxPositions;
				InitData.SysMemPitch = 0;
				InitData.SysMemSlicePitch = 0;
				
				if (pDevice)
				{
					pDevice->CreateBuffer( &bufferDesc, &InitData, &fBoxPositionBufferDX );
					if (fBoxPositionBufferDX)
						fBoxPositionBuffer->resourceHandle((void*)fBoxPositionBufferDX, 8*3);
				}

				// Index buffer
				bufferDesc.ByteWidth = sizeof(unsigned short) * 24;
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				InitData.pSysMem = boxIndices;

				if (pDevice)
				{
					pDevice->CreateBuffer( &bufferDesc, &InitData, &fBoxIndexBufferDX);
					if (fBoxIndexBufferDX)
						fBoxIndexBuffer->resourceHandle((void*)fBoxIndexBufferDX, 24);
				}
			}
		}
#endif
	}
	else
	{
		fBoxPositionBuffer->commit(boxPositions);
		fBoxIndexBuffer->commit(boxIndices);
	}
	boxPositions = NULL;
	boxIndices = NULL;

	// Fill index data for wireframe
	vid = 0;
	int first = 0;
	unsigned int idx = 0;
	for (int faceIdx=0; faceIdx<meshGeom->faceCount; faceIdx++)
	{
		// ignore degenerate faces
		int numVerts = meshGeom->face_counts[faceIdx];
		if (numVerts > 2)
		{
			first = vid;
			for (int v=0; v<numVerts-1; v++)
			{
				wireBuffer[idx++] = meshGeom->face_connects[vid++];
				wireBuffer[idx++] = meshGeom->face_connects[vid];
			}
			wireBuffer[idx++] = meshGeom->face_connects[vid++];
			wireBuffer[idx++] = meshGeom->face_connects[first];
		}
		else
		{
			vid += numVerts;
		}
	}
	fWireIndexBuffer->commit(wireBuffer); wireBuffer = NULL;

	// Fill index data for shaded
	unsigned int base = 0;
	idx = 0;
	for (int faceIdx=0; faceIdx<meshGeom->faceCount; faceIdx++)
	{
		// Ignore degenerate faces
		int numVerts = meshGeom->face_counts[faceIdx];
		if (numVerts > 2)
		{
			for (int v=1; v<numVerts-1; v++)
			{
				shadedBuffer[idx++] = meshGeom->face_connects[base];
				shadedBuffer[idx++] = meshGeom->face_connects[base+v];
				shadedBuffer[idx++] = meshGeom->face_connects[base+v+1];
			}
			base += numVerts;
		}
	}
	fShadedIndexBuffer->commit(shadedBuffer); shadedBuffer = NULL;
}

void apiMeshSubSceneOverride::rebuildActiveComponentIndexBuffers()
{
	// Preamble
	using namespace MHWRender;
	apiMeshGeom* meshGeom = fMesh->meshGeomToUse();
	if (!meshGeom) return;

	// Clear old
	deleteActiveComponentIndexBuffers();

	// Acquire and fill index buffer for active vertices
	const unsigned int numActiveVertices = fActiveVerticesSet.size();
	if (numActiveVertices > 0)
	{
		fActiveVerticesIndexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt32);
		unsigned int* activeVerticesBuffer = (unsigned int*)fActiveVerticesIndexBuffer->acquire(numActiveVertices, true);
		if (activeVerticesBuffer) {
			unsigned int idx = 0;
			std::set<int>::const_iterator it = fActiveVerticesSet.begin();
			std::set<int>::const_iterator itEnd = fActiveVerticesSet.end();
			for(; it != itEnd; ++it)
				activeVerticesBuffer[idx++] = *it;

			fActiveVerticesIndexBuffer->commit(activeVerticesBuffer);
			activeVerticesBuffer = NULL;
		}
	}

	// Acquire and fill index buffer for active edges
	const unsigned int numActiveEdges = fActiveEdgesSet.size();
	if (numActiveEdges > 0)
	{
		fActiveEdgesIndexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt32);
		unsigned int* activeEdgesBuffer = (unsigned int*)fActiveEdgesIndexBuffer->acquire(2*numActiveEdges, true);
		if (activeEdgesBuffer) {
			unsigned int eid = 0;
			unsigned int first = 0;
			unsigned int vid = 0;
			unsigned int idx = 0;
			for (int i=0; i<meshGeom->faceCount; ++i)
			{
				// Ignore degenerate faces
				int numVerts = meshGeom->face_counts[i];
				if (numVerts > 2)
				{
					first = vid;
					for (int v=0; v<numVerts-1; ++v)
					{
						if (fActiveEdgesSet.count(eid) > 0)
						{
							activeEdgesBuffer[idx++] = meshGeom->face_connects[vid];
							activeEdgesBuffer[idx++] = meshGeom->face_connects[vid + 1];
						}
						++vid;
						++eid;
					}

					if (fActiveEdgesSet.count(eid) > 0)
					{
						activeEdgesBuffer[idx++] = meshGeom->face_connects[vid];
						activeEdgesBuffer[idx++] = meshGeom->face_connects[first];
					}
					++vid;
					++eid;
				}
				else {
					vid += numVerts;
				}
			}

			fActiveEdgesIndexBuffer->commit(activeEdgesBuffer);
			activeEdgesBuffer = NULL;
		}
	}

	// Acquire and fill index buffer for active faces
	const unsigned int numActiveFaces = fActiveFacesSet.size();
	if (numActiveFaces > 0)
	{
		unsigned int numActiveFacesTriangles = 0;
		for (int i=0; i<meshGeom->faceCount; ++i)
		{
			if (fActiveFacesSet.count(i) > 0)
			{
				int numVerts = meshGeom->face_counts[i];
				if (numVerts > 2)
					numActiveFacesTriangles += numVerts - 2;
			}
		}

		fActiveFacesIndexBuffer = new MIndexBuffer(MGeometry::kUnsignedInt32);
		unsigned int* activeFacesBuffer = (unsigned int*)fActiveFacesIndexBuffer->acquire(3*numActiveFacesTriangles, true);
		if (activeFacesBuffer)
		{
			unsigned int idx = 0;
			unsigned int vid = 0;
			for (int i=0; i<meshGeom->faceCount; ++i)
			{
				// Ignore degenerate faces
				int numVerts = meshGeom->face_counts[i];
				if (numVerts > 2)
				{
					if (fActiveFacesSet.count(i) > 0)
					{
						for (int v=1; v<numVerts-1; ++v)
						{
							activeFacesBuffer[idx++] = meshGeom->face_connects[vid];
							activeFacesBuffer[idx++] = meshGeom->face_connects[vid+v];
							activeFacesBuffer[idx++] = meshGeom->face_connects[vid+v+1];
						}
					}
					vid += numVerts;
				}
			}

			fActiveFacesIndexBuffer->commit(activeFacesBuffer);
			activeFacesBuffer = NULL;
		}
	}
}

void apiMeshSubSceneOverride::deleteBuffers()
{
	deleteGeometryBuffers();
	deleteActiveComponentIndexBuffers();
}

void apiMeshSubSceneOverride::deleteGeometryBuffers()
{
	delete fPositionBuffer; fPositionBuffer = NULL;
	delete fNormalBuffer; fNormalBuffer = NULL;
	delete fBoxPositionBuffer; fBoxPositionBuffer = NULL;
	delete fWireIndexBuffer; fWireIndexBuffer = NULL;
	delete fBoxIndexBuffer; fBoxIndexBuffer = NULL;
	delete fShadedIndexBuffer; fShadedIndexBuffer = NULL;

	// Delete client buffers
	if (sUseCustomUserBuffersForBoundingBox)
	{
		if (sDrawAPI == MHWRender::kOpenGL ||
			sDrawAPI == MHWRender::kOpenGLCoreProfile)
		{
			static MGLFunctionTable* gGLFT = NULL;

			_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
			MHardwareRenderer* pRenderer = MHardwareRenderer::theRenderer();

			if (pRenderer && !gGLFT)
			{
				gGLFT = pRenderer->glFunctionTable();
			}

			_OPENMAYA_POP_WARNING

			if (gGLFT)
			{
				if (fBoxPositionBufferId != 0)
				{
					gGLFT->glDeleteBuffersARB(1, &fBoxPositionBufferId);
					fBoxPositionBufferId = 0;
				}
				if (fBoxIndexBufferId != 0)
				{
					gGLFT->glDeleteBuffersARB(1, &fBoxIndexBufferId);
					fBoxIndexBufferId = 0;
				}
			}
		}
#ifdef _WINDOWS
		else if (sDrawAPI == MHWRender::kDirectX11)
		{
			if (NULL != fBoxPositionBufferDX)
			{
				fBoxPositionBufferDX->Release();
				fBoxPositionBufferDX = NULL;
			}
			if (NULL != fBoxIndexBufferDX)
			{
				fBoxIndexBufferDX->Release();
				fBoxIndexBufferDX = NULL;
			}
		}
#endif
	}
}

void apiMeshSubSceneOverride::deleteActiveComponentIndexBuffers()
{
	delete fActiveVerticesIndexBuffer; fActiveVerticesIndexBuffer = NULL;
	delete fActiveEdgesIndexBuffer; fActiveEdgesIndexBuffer = NULL;
	delete fActiveFacesIndexBuffer; fActiveFacesIndexBuffer = NULL;
}

/*
	This is method is called during the pre-filtering phase of the viewport 2.0 selection
   	and is used to setup the selection context of the given DAG object.

	We want the whole shape to be selectable, so we set the selection level to kObject so that the shape
	will be processed by the selection.

	In case we are currently in component selection mode (vertex, edge or face),
	since we have created render items that can be use in the selection phase (kSelectionOnly draw mode)
	and we also registered component converters to handle this render items,
	we can set the selection level to kComponent so that the shape will also be processed by the selection.
*/
void apiMeshSubSceneOverride::updateSelectionGranularity(
	const MDagPath& path,
	MHWRender::MSelectionContext& selectionContext)
{
	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(path);
	if(displayStatus == MHWRender::kHilite)
	{
		MSelectionMask globalComponentMask = MGlobal::selectionMode() == MGlobal::kSelectComponentMode ? MGlobal::componentSelectionMask() : MGlobal::objectSelectionMask();
		MSelectionMask supportedComponents(MSelectionMask::kSelectMeshVerts);
		supportedComponents.addMask(MSelectionMask::kSelectMeshEdges);
		supportedComponents.addMask(MSelectionMask::kSelectMeshFaces);
		supportedComponents.addMask(MSelectionMask::kSelectPointsForGravity);

		if(globalComponentMask.intersects(supportedComponents))
		{
			selectionContext.setSelectionLevel(MHWRender::MSelectionContext::kComponent);
		}
	}
	else if (pointSnappingActive())
	{
		selectionContext.setSelectionLevel(MHWRender::MSelectionContext::kComponent);
	}
}

bool apiMeshSubSceneOverride::hasUIDrawables() const
{
	return true;
}

bool apiMeshSubSceneOverride::areUIDrawablesDirty() const
{
	// For performance, the UI drawables should be updated only when required.
	return fAreUIDrawablesDirty;
}

void apiMeshSubSceneOverride::addUIDrawables(
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext)
{
	// Draw a text "subSceneUIDraw"
	MPoint pos( 0.0, 0.0, 0.0 ); // Position of the text
	MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f ); // Text color
	MString text("subSceneUIDraw");

	drawManager.beginDrawable();

	drawManager.setColor( textColor );
	drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );

	// MUIDrawManager assumes the object space of the original instance. If there
	// are multiple instances, each text needs to be drawn in the origin of each
	// instance, so we need to transform the coordinates from each instance's
	// object space to the original instance's object space.
	MMatrix worldInverse0 = fInstance0Transform.inverse();
	for (auto it = fInstanceInfoCache.begin(); it != fInstanceInfoCache.end(); it++)
	{
		drawManager.text((pos * it->second.fTransform) * worldInverse0,
			text, MHWRender::MUIDrawManager::kCenter);
	}

	drawManager.endDrawable();

	fAreUIDrawablesDirty = false;
}

bool apiMeshSubSceneOverride::enableUpdateForSelection() const
{
    return true;
}

/*
   Register our component converters to the draw registry
   This should be done only once, when the plugin is initialized
*/
MStatus apiMeshSubSceneOverride::registerComponentConverters()
{
	MStatus status = MHWRender::MDrawRegistry::registerComponentConverter(apiMeshSubSceneOverride::sVertexSelectionName, simpleComponentConverter::creatorVertexSelection);
	if(status) {
		status = MHWRender::MDrawRegistry::registerComponentConverter(apiMeshSubSceneOverride::sEdgeSelectionName, simpleComponentConverter::creatorEdgeSelection);
		if(status) {
			MHWRender::MDrawRegistry::registerComponentConverter(apiMeshSubSceneOverride::sFaceSelectionName, simpleComponentConverter::creatorFaceSelection);
		}
	}
	return status;
}

/*
   Deregister our component converters from the draw registry
   This should be done only once, when the plugin is uninitialized
*/
MStatus apiMeshSubSceneOverride::deregisterComponentConverters()
{
	// Deregister component converter for isolate select render items
	for (const std::string& key : sViewSelectedFaceSelectionNames)
	{
		MHWRender::MDrawRegistry::deregisterComponentConverter(key.c_str());
	}
	sViewSelectedFaceSelectionNames.clear();

	MStatus status = MHWRender::MDrawRegistry::deregisterComponentConverter(apiMeshSubSceneOverride::sVertexSelectionName);
	if(status) {
		status = MHWRender::MDrawRegistry::deregisterComponentConverter(apiMeshSubSceneOverride::sEdgeSelectionName);
		if(status) {
			status = MHWRender::MDrawRegistry::deregisterComponentConverter(apiMeshSubSceneOverride::sFaceSelectionName);
		}
	}
	return status;
}

bool apiMeshSubSceneOverride::getInstancedSelectionPath(
    const MHWRender::MRenderItem& renderItem,
    const MHWRender::MIntersection& intersection,
    MDagPath& dagPath) const
{
    MStatus status;
    MFnDagNode node(fObject, &status);
    if (!status) 
        return false;

    MDagPathArray instances;
    if (!node.getAllPaths(instances))
        return false;

    unsigned int instanceCount = instances.length();
    if (instanceCount == 0)
        return false;

    int instanceId = intersection.instanceID();
    // In case there is only one instance or GPU instancing is not used
    if (instanceCount == 1 || instanceId == -1)
    {
        instanceId = 0;

        apiMeshHWSelectionUserData* userData =
            dynamic_cast<apiMeshHWSelectionUserData*>(renderItem.customData());
        if (userData &&
            userData->fInstanceIndex >= 0 &&
            userData->fInstanceIndex < (int)instanceCount)
        {
            instanceId = userData->fInstanceIndex;
        }

        dagPath = instances[instanceId];
        return true;
    }
    // The instance ID starts from 1 for the first DAG node. We can use instanceID - 1
    // as the index to DagPath array returned by MFnDagNode::getAllPaths().
    // HOWEVER there is an exception when there are instances within a Shape object,
    // which introduces nested instancing scenarios. For simplicity reason the selection
    // edge instancing is disabled when there are multiple apiMesh instances.
    else if (instanceId >=1 && instanceId <= (int)instanceCount)
    {
        M3dView view = M3dView::active3dView();
        if (view.viewIsFiltered())
        {
            MSelectionList viewSelectedList;
            if (view.filteredObjectList(viewSelectedList))
            {
                for (unsigned int instIdx = 0; instIdx < instanceCount; instIdx++)
                {
                    const MDagPath& instance = instances[instIdx];
                    if (instance.isValid() && instance.isVisible())
                    {
                        MSelectionList intersectionList;
                        intersectionList.add(instance);
                        intersectionList.intersect(viewSelectedList, true/*expandToLeaves*/);

                        for (MItSelectionList it(intersectionList); !it.isDone(); it.next())
                        {
                            MDagPath dummyPath;
                            MObject comp;
                            if (it.getDagPath(dummyPath, comp) && comp.isNull())
                            {
                                instanceId--;
                                if (instanceId == 0)
                                {
                                    dagPath = instance;
                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }
        else
        {
            for (unsigned int instIdx = 0; instIdx < instanceCount; instIdx++)
            {
                const MDagPath& instance = instances[instIdx];
                if (instance.isValid() && instance.isVisible())
                {
                    instanceId--;
                    if (instanceId == 0)
                    {
                        dagPath = instance;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

/*static*/
void apiMeshSubSceneOverride::shadedItemLinkLost(MUserData* userData)
{
	ShadedItemUserData* data = dynamic_cast<ShadedItemUserData*>(userData);
	if (data && data->fOverride)
	{
		if (data->fOverride->fMesh)
			data->fOverride->fMesh->setMaterialDirty(true);
		data->fOverride->untrackLinkLostData(data);
	}
	delete userData;
}

// helper function
bool useSelectHighlight(const MSelectionList& selectedList, const MDagPath& path)
{
    MStatus status = MStatus::kSuccess;

	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(path);
	if ((displayStatus == MHWRender::kHilite)
		|| (displayStatus == MHWRender::kActiveComponent))
	{
		return true;
	}

	MDagPath pathCopy = path;
	do
	{
		if (selectedList.hasItem(pathCopy))
		{
            return true;
		}
		status = pathCopy.pop();
	}
	while (status);

    return false;
}


static bool floatApproxEqual(float left, float right)
{
    return fabs(left - right) < 0.0001f;
}

