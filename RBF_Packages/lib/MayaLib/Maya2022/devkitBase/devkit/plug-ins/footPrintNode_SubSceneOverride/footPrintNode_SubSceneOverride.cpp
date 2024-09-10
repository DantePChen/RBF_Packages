//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
// 
// The plug-in demonstrates how to draw custom locators using MPxSubSceneOverride.
//
// It can be compared with other plugins which draw the same footprint locator,
// including footPrintNode, rawfootPrintNode and footPrintNode_GeometryOverride.
// For more info, please refer to the developer guide documentation on Choices
// for Porting Plug-in Locators to Viewport 2.0.
//
// The implementation doesn't monitor DAG changes, so the update() method needs
// to be called in every frame and walk through all the instances to look for
// color/matrix changes in order to determine whether render items and UI drawables
// need to be updated. If expecting large numbers of instances, monitoring change
// events and changing only the required instances should be done instead to
// improve efficiency.
//
// The MRenderItem class is used to draw the mesh and allow hardware instancing
// and consolidation to kick in. When the DAG object is instanced, plug-ins can
// call setInstanceTransformArray()/setExtraInstanceData() to convert a regular
// render item to an instanced one with the supplied per-instance data such as
// matrices and colors. Both regular and instanced render items can opt in to
// leverage VP2 consolidation by calling setWantConsolidation(). This example
// implements a mini shader cache to allow for sharing shaders among multiple
// render items so that VP2 consolidation can kick in.
//
// The MUIDrawManager class is used to draw the texts. UI drawables created in a
// subscene will be kept alive until the next addUIDrawables() call. Although UI
// drawables cannot leverage hardware instancing or consolidation, it is possible
// for the plug-in to batch UI drawables for all DAG instances together and
// reduce the draw overhead. Note this method might not be applicable for complex
// meshes because of the cost of transforming a large number of vertices between
// different object spaces using CPU, thus for this case the MRenderItem class
// should be used. See the addUIDrawables() method for batching UI drawables and
// the transformation between spaces.
//
// Selection is handled by the getInstancedSelectionPath() method. To prevent it
// from being called for any unqualified selection hits, each render item should
// be assigned with a proper selection mask.
//
// Example usage is to load the plug-in and create the node:
//
// loadPlugin footPrintNode_SubSceneOverride;
// createNode footPrint_SubSceneOverride;
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxLocatorNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MColor.h>
#include <maya/M3dView.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MFnDagNode.h>
#include <maya/MSelectionContext.h>
#include <maya/MDagMessage.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxSubSceneOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>

#include <unordered_map>


namespace
{
	// Foot Data
	//
	float sole[][3] = { {  0.00f, 0.0f, -0.70f },
						{  0.04f, 0.0f, -0.69f },
						{  0.09f, 0.0f, -0.65f },
						{  0.13f, 0.0f, -0.61f },
						{  0.16f, 0.0f, -0.54f },
						{  0.17f, 0.0f, -0.46f },
						{  0.17f, 0.0f, -0.35f },
						{  0.16f, 0.0f, -0.25f },
						{  0.15f, 0.0f, -0.14f },
						{  0.13f, 0.0f,  0.00f },
						{  0.00f, 0.0f,  0.00f },
						{ -0.13f, 0.0f,  0.00f },
						{ -0.15f, 0.0f, -0.14f },
						{ -0.16f, 0.0f, -0.25f },
						{ -0.17f, 0.0f, -0.35f },
						{ -0.17f, 0.0f, -0.46f },
						{ -0.16f, 0.0f, -0.54f },
						{ -0.13f, 0.0f, -0.61f },
						{ -0.09f, 0.0f, -0.65f },
						{ -0.04f, 0.0f, -0.69f },
						{ -0.00f, 0.0f, -0.70f } };
	float heel[][3] = { {  0.00f, 0.0f,  0.06f },
						{  0.13f, 0.0f,  0.06f },
						{  0.14f, 0.0f,  0.15f },
						{  0.14f, 0.0f,  0.21f },
						{  0.13f, 0.0f,  0.25f },
						{  0.11f, 0.0f,  0.28f },
						{  0.09f, 0.0f,  0.29f },
						{  0.04f, 0.0f,  0.30f },
						{  0.00f, 0.0f,  0.30f },
						{ -0.04f, 0.0f,  0.30f },
						{ -0.09f, 0.0f,  0.29f },
						{ -0.11f, 0.0f,  0.28f },
						{ -0.13f, 0.0f,  0.25f },
						{ -0.14f, 0.0f,  0.21f },
						{ -0.14f, 0.0f,  0.15f },
						{ -0.13f, 0.0f,  0.06f },
						{ -0.00f, 0.0f,  0.06f } };
	int soleCount = 21;
	int heelCount = 17;

	// Viewport 2.0 specific data
	//
	const MString colorParameterName_ = "solidColor";
	const MString wireframeItemName_  = "footPrintLocatorWires";
	const MString shadedItemName_     = "footPrintLocatorTriangles";

	// Maintain a mini cache for 3d solid shaders in order to reuse the shader
	// instance whenever possible. This can allow Viewport 2.0 optimization e.g.
	// the GPU instancing system and the consolidation system to be leveraged.
	//
	struct MColorHash
	{
		std::size_t operator()(const MColor& color) const
		{
			std::size_t seed = 0;
			CombineHashCode(seed, color.r);
			CombineHashCode(seed, color.g);
			CombineHashCode(seed, color.b);
			CombineHashCode(seed, color.a);
			return seed;
		}

		void CombineHashCode(std::size_t& seed, float v) const
		{
			std::hash<float> hasher;
			seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}
	};

	std::unordered_map<MColor, MHWRender::MShaderInstance*, MColorHash> the3dSolidShaders;

	MHWRender::MShaderInstance* get3dSolidShader(const MColor& color)
	{
		// Return the shader instance if exists.
		auto it = the3dSolidShaders.find(color);
		if (it != the3dSolidShaders.end())
		{
			return it->second;
		}

		MHWRender::MShaderInstance* shader = NULL;

		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
			if (shaderMgr)
			{
				shader = shaderMgr->getStockShader(MHWRender::MShaderManager::k3dSolidShader);
			}
		}

		if (shader)
		{
			float solidColor[] = { color.r, color.g, color.b, 1.0f };
			shader->setParameter(colorParameterName_, solidColor);

			the3dSolidShaders[color] = shader;
		}

		return shader;
	}

	MStatus releaseShaders()
	{
		MHWRender::MRenderer* renderer = MHWRender::MRenderer::theRenderer();
		if (renderer)
		{
			const MHWRender::MShaderManager* shaderMgr = renderer->getShaderManager();
			if (shaderMgr)
			{
				for (auto it = the3dSolidShaders.begin(); it != the3dSolidShaders.end(); it++)
				{
					shaderMgr->releaseShader(it->second);
				}

				the3dSolidShaders.clear();
				return MS::kSuccess;
			}
		}

		return MS::kFailure;
	}

} // anonymous namespace

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Node implementation with legacy viewport draw
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class footPrint : public MPxLocatorNode
{
public:
	footPrint();
	~footPrint() override;

    MStatus   		compute( const MPlug& plug, MDataBlock& data ) override;

	bool            isBounded() const override;
	MBoundingBox    boundingBox() const override;

	MSelectionMask getShapeSelectionMask() const override;

	static  void *          creator();
	static  MStatus         initialize();

	static  MObject         size;         // The size of the foot

public:
	static	MTypeId		id;
	static	MString		drawDbClassification;
	static	MString		drawRegistrantId;
};

MObject footPrint::size;
MTypeId footPrint::id( 0x00080037 );
MString	footPrint::drawDbClassification("drawdb/subscene/footPrint_SubSceneOverride");
MString	footPrint::drawRegistrantId("FootprintNode_SubSceneOverridePlugin");

footPrint::footPrint() {}
footPrint::~footPrint() {}

MStatus footPrint::compute( const MPlug& /*plug*/, MDataBlock& /*data*/ )
{
	return MS::kUnknownParameter;
}

bool footPrint::isBounded() const
{
	return true;
}

MBoundingBox footPrint::boundingBox() const
{
	// Get the size
	//
	MObject thisNode = thisMObject();
	MPlug plug( thisNode, size );
	MDistance sizeVal;
	plug.getValue( sizeVal );

	double multiplier = sizeVal.asCentimeters();

	MPoint corner1( -0.17, 0.0, -0.7 );
	MPoint corner2( 0.17, 0.0, 0.3 );

	corner1 = corner1 * multiplier;
	corner2 = corner2 * multiplier;

	return MBoundingBox( corner1, corner2 );
}

MSelectionMask footPrint::getShapeSelectionMask() const
{
	return MSelectionMask("footPrintSelection");
}

void* footPrint::creator()
{
	return new footPrint();
}

MStatus footPrint::initialize()
{
	MFnUnitAttribute unitFn;
	MStatus stat;

	size = unitFn.create( "size", "sz", MFnUnitAttribute::kDistance );
	unitFn.setDefault( 1.0 );

	stat = addAttribute( size );
	if (!stat) {
		stat.perror("addAttribute");
		return stat;
	}

	return MS::kSuccess;
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2.0 override implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

class FootPrintSubSceneOverride : public MHWRender::MPxSubSceneOverride
{
public:

	static MHWRender::MPxSubSceneOverride* Creator(const MObject& obj)
	{
		return new FootPrintSubSceneOverride(obj);
	}

	~FootPrintSubSceneOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override
	{
		return MHWRender::kAllDevices;
	}

	bool requiresUpdate(
		const MHWRender::MSubSceneContainer& container,
		const MHWRender::MFrameContext& frameContext) const override
	{
		return true;
	}

	void update(
		MHWRender::MSubSceneContainer& container,
		const MHWRender::MFrameContext& frameContext) override;

	bool hasUIDrawables() const override
	{
		return true;
	}

	bool areUIDrawablesDirty() const override
	{
		return fAreUIDrawablesDirty;
	}

	void addUIDrawables(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext) override;

	bool getInstancedSelectionPath(
		const MHWRender::MRenderItem& renderItem,
		const MHWRender::MIntersection& intersection,
		MDagPath& dagPath) const override;

private:

	FootPrintSubSceneOverride(const MObject& obj);

	// Create and delete VB/IB
	void rebuildGeometryBuffers();
	void deleteGeometryBuffers();

	MObject fLocatorNode;
	float fMultiplier;
	bool fIsInstanceMode;
	bool fAreUIDrawablesDirty;

	MHWRender::MVertexBuffer* fPositionBuffer;
	MHWRender::MIndexBuffer* fWireIndexBuffer;
	MHWRender::MIndexBuffer* fShadedIndexBuffer;

	struct InstanceInfo
	{
		MMatrix fMatrix;
		MColor fColor;

		InstanceInfo() {}
		InstanceInfo(const MMatrix& m, const MColor& c) : fMatrix(m), fColor(c) {}
	};
	typedef std::map<unsigned int, InstanceInfo> InstanceInfoMap;
	InstanceInfoMap fInstanceInfoCache;

	// Callbacks on instance added/removed.
	static void InstanceChangedCallback(MDagPath &child, MDagPath &parent, void *clientData);
	MCallbackId fInstanceAddedCbId;
	MCallbackId fInstanceRemovedCbId;
	MDagPathArray fInstanceDagPaths;
};

FootPrintSubSceneOverride::FootPrintSubSceneOverride(const MObject& obj)
: MHWRender::MPxSubSceneOverride(obj)
, fLocatorNode(obj)
, fMultiplier(0.0f)
, fIsInstanceMode(false)
, fAreUIDrawablesDirty(true)
, fPositionBuffer(NULL)
, fWireIndexBuffer(NULL)
, fShadedIndexBuffer(NULL)
, fInstanceAddedCbId(0)
, fInstanceRemovedCbId(0)
{
	MDagPath dagPath;
	if (MDagPath::getAPathTo(obj, dagPath))
	{
		fInstanceAddedCbId = MDagMessage::addInstanceAddedDagPathCallback(
			dagPath, InstanceChangedCallback, this);

		fInstanceRemovedCbId = MDagMessage::addInstanceRemovedDagPathCallback(
			dagPath, InstanceChangedCallback, this);
	}
}

FootPrintSubSceneOverride::~FootPrintSubSceneOverride()
{
	deleteGeometryBuffers();

	if (fInstanceAddedCbId != 0)
	{
		MMessage::removeCallback(fInstanceAddedCbId);
		fInstanceAddedCbId = 0;
	}

	if (fInstanceRemovedCbId != 0)
	{
		MMessage::removeCallback(fInstanceRemovedCbId);
		fInstanceRemovedCbId = 0;
	}
}

/* static */
void FootPrintSubSceneOverride::InstanceChangedCallback(
	MDagPath& child,
	MDagPath& parent,
	void* clientData)
{
	FootPrintSubSceneOverride* ovr = static_cast<FootPrintSubSceneOverride*>(clientData);
	if (ovr)
	{
		ovr->fInstanceDagPaths.clear();
	}
}

void FootPrintSubSceneOverride::update(
	MHWRender::MSubSceneContainer& container,
	const MHWRender::MFrameContext& frameContext)
{
	unsigned int numInstances = fInstanceDagPaths.length();
	if (numInstances == 0)
	{
		if (!MDagPath::getAllPathsTo(fLocatorNode, fInstanceDagPaths))
		{
			fprintf(stderr, "FootPrintSubSceneOverride: Failed to get all DAG paths.\n");
			return;
		}

		numInstances = fInstanceDagPaths.length();
	}

	if (numInstances == 0) return;

	MHWRender::MShaderInstance* shader = get3dSolidShader(
		MHWRender::MGeometryUtilities::wireframeColor(fInstanceDagPaths[0]));
	if (!shader)
	{
		fprintf(stderr, "FootPrintSubSceneOverride: Failed to get a 3d solid shader.\n");
		return;
	}

	MPlug plug(fLocatorNode, footPrint::size);
	float newMultiplier = 1.0f;
	if (!plug.isNull())
	{
		MDistance sizeVal;
		if (plug.getValue(sizeVal))
		{
			newMultiplier = (float)sizeVal.asCentimeters();
		}
	}

	bool updateGeometry = (container.count() == 0);

	if (fMultiplier != newMultiplier)
	{
		fMultiplier = newMultiplier;

		updateGeometry = true;
	}

	if (updateGeometry)
	{
		rebuildGeometryBuffers();
	}

	bool anyInstanceChanged = false;
	unsigned int numVisibleInstances = 0;
	const unsigned int componentsPerColor = 4; // RGBA color

	MMatrixArray instanceMatrixArray(numInstances);
	MFloatArray instanceColorArray(numInstances * componentsPerColor);

	// If expecting large numbers of instances, walking through all the instances
	// every frame to look for changes is not efficient enough. Monitoring change
	// events and changing only the required instances should be done instead.
	for (unsigned int i=0; i<numInstances; i++)
	{
		const MDagPath& instance = fInstanceDagPaths[i];
		if (instance.isValid() && instance.isVisible())
		{
			InstanceInfo instanceInfo(instance.inclusiveMatrix(),
				MHWRender::MGeometryUtilities::wireframeColor(instance));

			InstanceInfoMap::iterator iter = fInstanceInfoCache.find(i);
			if (iter == fInstanceInfoCache.end() ||
				iter->second.fColor != instanceInfo.fColor ||
				!iter->second.fMatrix.isEquivalent(instanceInfo.fMatrix))
			{
				if (!fAreUIDrawablesDirty &&
					(iter == fInstanceInfoCache.end() ||
					 !iter->second.fMatrix.isEquivalent(instanceInfo.fMatrix)))
				{
					fAreUIDrawablesDirty = true;
				}

				anyInstanceChanged = true;

				fInstanceInfoCache[i] = instanceInfo;
			}

			instanceMatrixArray[numVisibleInstances] = instanceInfo.fMatrix;
			instanceColorArray[numVisibleInstances*componentsPerColor] = instanceInfo.fColor.r;
			instanceColorArray[numVisibleInstances*componentsPerColor+1] = instanceInfo.fColor.g;
			instanceColorArray[numVisibleInstances*componentsPerColor+2] = instanceInfo.fColor.b;
			instanceColorArray[numVisibleInstances*componentsPerColor+3] = instanceInfo.fColor.a;

			numVisibleInstances++;
		}
		else
		{
			InstanceInfoMap::iterator iter = fInstanceInfoCache.find(i);
			if (iter != fInstanceInfoCache.end())
			{
				fInstanceInfoCache.erase(i);

				anyInstanceChanged = true;
				fAreUIDrawablesDirty = true;
			}
		}
	}

	// Shrink to fit
	instanceMatrixArray.setLength(numVisibleInstances);
	instanceColorArray.setLength(numVisibleInstances*componentsPerColor);

	unsigned int numInstanceInfo = fInstanceInfoCache.size();
	if (numInstanceInfo != numVisibleInstances)
	{
		for (unsigned int i=numVisibleInstances; i<numInstanceInfo; i++)
		{
			fInstanceInfoCache.erase(i);
		}

		anyInstanceChanged = true;
		fAreUIDrawablesDirty = true;
	}

	bool itemsChanged = false;

	MHWRender::MRenderItem* wireItem = container.find(wireframeItemName_);
	if (!wireItem)
	{
		wireItem = MHWRender::MRenderItem::Create(wireframeItemName_,
			MHWRender::MRenderItem::DecorationItem,
			MHWRender::MGeometry::kLines);
		wireItem->setDrawMode(MHWRender::MGeometry::kWireframe);
		wireItem->depthPriority(5);
		wireItem->setSelectionMask(MSelectionMask::kSelectLocators);
		wireItem->setWantConsolidation(true);
		container.add(wireItem);

		itemsChanged = true;
	}

	MHWRender::MRenderItem* shadedItem = container.find(shadedItemName_);
	if (!shadedItem)
	{
		shadedItem = MHWRender::MRenderItem::Create(shadedItemName_,
			MHWRender::MRenderItem::DecorationItem,
			MHWRender::MGeometry::kTriangles);
		shadedItem->setDrawMode((MHWRender::MGeometry::DrawMode)
			(MHWRender::MGeometry::kShaded|MHWRender::MGeometry::kTextured));
		shadedItem->setSelectionMask(MSelectionMask::kSelectLocators);
		shadedItem->setWantConsolidation(true);
		container.add(shadedItem);

		itemsChanged = true;
	}

	if (itemsChanged || anyInstanceChanged)
	{
		wireItem->setShader(shader);
		shadedItem->setShader(shader);
	}

	if (itemsChanged || updateGeometry)
	{
		MStatus status;
		MFnDagNode node(fLocatorNode, &status);

		footPrint* fp = status ? dynamic_cast<footPrint*>(node.userNode()) : NULL;
		MBoundingBox *bounds = fp ? new MBoundingBox(fp->boundingBox()) : NULL;

		MHWRender::MVertexBufferArray vertexBuffers;
		vertexBuffers.addBuffer("positions", fPositionBuffer);
		setGeometryForRenderItem(*wireItem, vertexBuffers, *fWireIndexBuffer, bounds);
		setGeometryForRenderItem(*shadedItem, vertexBuffers, *fShadedIndexBuffer, bounds);

		if (bounds) delete bounds;
	}

	if (itemsChanged || anyInstanceChanged)
	{
		if (!fIsInstanceMode && numInstances == 1)
		{
			// When not dealing with multiple instances, don't convert the render items into instanced
			// mode.  Set the matrices on them directly.
			MMatrix& objToWorld = instanceMatrixArray[0];
			wireItem->setMatrix(&objToWorld);
			shadedItem->setMatrix(&objToWorld);
		}
		else
		{
			// If we have DAG instances of this shape then use the MPxSubSceneOverride instance
			// transform API to set up instance copies of the render items.  This will be faster
			// to render than creating render items for each instance, especially for large numbers
			// of instances.
			// Note this has to happen after the geometry and shaders are set, otherwise it will fail.
			setInstanceTransformArray(*wireItem, instanceMatrixArray);
			setInstanceTransformArray(*shadedItem, instanceMatrixArray);
			setExtraInstanceData(*wireItem, colorParameterName_, instanceColorArray);
			setExtraInstanceData(*shadedItem, colorParameterName_, instanceColorArray);

			// Once we change the render items into instance rendering they can't be changed back without
			// being deleted and re-created.  So if instances are deleted to leave only one remaining,
			// just keep treating them the instance way.
			fIsInstanceMode = true;
		}
	}
}

void FootPrintSubSceneOverride::addUIDrawables(
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext)
{
	MPoint pos( 0.0, 0.0, 0.0 );
	MColor textColor( 0.1f, 0.8f, 0.8f, 1.0f );
	MString text( "Footprint" );

	drawManager.beginDrawable();

	drawManager.setColor( textColor );
	drawManager.setFontSize( MHWRender::MUIDrawManager::kSmallFontSize );

	// MUIDrawManager assumes the object space of the original instance. If there
	// are multiple instances, each text needs to be drawn in the origin of each
	// instance, so we need to transform the coordinates from each instance's
	// object space to the original instance's object space.
	MMatrix worldInverse0 = fInstanceInfoCache[0].fMatrix.inverse();
	for (auto it = fInstanceInfoCache.begin(); it != fInstanceInfoCache.end(); it++)
	{

		drawManager.text((pos * it->second.fMatrix) * worldInverse0,
						 text, MHWRender::MUIDrawManager::kCenter);
	}

	drawManager.endDrawable();

	fAreUIDrawablesDirty = false;
}

bool FootPrintSubSceneOverride::getInstancedSelectionPath(
	const MHWRender::MRenderItem& renderItem,
	const MHWRender::MIntersection& intersection,
	MDagPath& dagPath) const
{
	unsigned int numInstances = fInstanceDagPaths.length();
	if (numInstances == 0) return false;

	// The instance ID starts from 1 for the first DAG path. We use instanceID-1
	// as the index to DAG path array returned by MFnDagNode::getAllPaths().
	int instanceId = intersection.instanceID();
	if (instanceId > (int)numInstances) return false;

	// Return the first DAG path in case of no instancing.
	if (numInstances == 1 || instanceId == -1)
	{
		instanceId = 1;
	}

	return MDagPath::getAPathTo(fInstanceDagPaths[instanceId-1].transform(), dagPath);
}

void FootPrintSubSceneOverride::rebuildGeometryBuffers()
{
	deleteGeometryBuffers();

	// VB for positions. We concatenate the heel and sole positions into a single vertex buffer.
	// The index buffers will decide which positions will be selected for each render items.
	const MHWRender::MVertexBufferDescriptor vbDesc("",
		MHWRender::MGeometry::kPosition, MHWRender::MGeometry::kFloat, 3);
	fPositionBuffer = new MHWRender::MVertexBuffer(vbDesc);
	if (fPositionBuffer)
	{
		float* positions = (float*)fPositionBuffer->acquire(soleCount+heelCount, true);
		if (positions)
		{
			int verticesPointerOffset = 0;

			for (int currentVertex = 0 ; currentVertex < soleCount+heelCount; ++currentVertex)
			{
				if (currentVertex < heelCount)
				{
					int heelVtx = currentVertex;
					positions[verticesPointerOffset++] = heel[heelVtx][0] * fMultiplier;
					positions[verticesPointerOffset++] = heel[heelVtx][1] * fMultiplier;
					positions[verticesPointerOffset++] = heel[heelVtx][2] * fMultiplier;
				}
				else
				{
					int soleVtx = currentVertex - heelCount;
					positions[verticesPointerOffset++] = sole[soleVtx][0] * fMultiplier;
					positions[verticesPointerOffset++] = sole[soleVtx][1] * fMultiplier;
					positions[verticesPointerOffset++] = sole[soleVtx][2] * fMultiplier;
				}
			}

			fPositionBuffer->commit(positions);
		}
	}

	// IB for the wireframe item
	fWireIndexBuffer = new MHWRender::MIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
	if (fWireIndexBuffer)
	{
		int primitiveIndex = 0;
		int startIndex = 0;
		int numPrimitive = heelCount + soleCount - 2;
		int numIndex = numPrimitive * 2;

		unsigned int* indices = (unsigned int*)fWireIndexBuffer->acquire(numIndex, true);
		if (indices)
		{
			for (int i = 0; i < numIndex; )
			{
				if (i < (heelCount - 1) * 2)
				{
					startIndex = 0;
					primitiveIndex = i / 2;
				}
				else
				{
					startIndex = heelCount;
					primitiveIndex = i / 2 - heelCount + 1;
				}
				indices[i++] = startIndex + primitiveIndex;
				indices[i++] = startIndex + primitiveIndex + 1;
			}

			fWireIndexBuffer->commit(indices);
		}
	}

	// IB for the shaded item
	fShadedIndexBuffer = new MHWRender::MIndexBuffer(MHWRender::MGeometry::kUnsignedInt32);
	if (fShadedIndexBuffer)
	{
		int primitiveIndex = 0;
		int startIndex = 0;
		int numPrimitive = heelCount + soleCount - 4;
		int numIndex = numPrimitive * 3;

		unsigned int* indices = (unsigned int*)fShadedIndexBuffer->acquire(numIndex, true);
		if (indices)
		{
			for (int i = 0; i < numIndex; )
			{
				if (i < (heelCount - 2) * 3)
				{
					startIndex = 0;
					primitiveIndex = i / 3;
				}
				else
				{
					startIndex = heelCount;
					primitiveIndex = i / 3 - heelCount + 2;
				}
				indices[i++] = startIndex;
				indices[i++] = startIndex + primitiveIndex + 1;
				indices[i++] = startIndex + primitiveIndex + 2;
			}

			fShadedIndexBuffer->commit(indices);
		}
	}
}

void FootPrintSubSceneOverride::deleteGeometryBuffers()
{
	if (fPositionBuffer)
	{
		delete fPositionBuffer;
		fPositionBuffer = NULL;
	}

	if (fWireIndexBuffer)
	{
		delete fWireIndexBuffer;
		fWireIndexBuffer = NULL;
	}

	if (fShadedIndexBuffer)
	{
		delete fShadedIndexBuffer;
		fShadedIndexBuffer = NULL;
	}
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Plugin Registration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

MStatus initializePlugin( MObject obj )
{
	MStatus status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerNode(
		"footPrint_SubSceneOverride",
		footPrint::id,
		&footPrint::creator,
		&footPrint::initialize,
		MPxNode::kLocatorNode,
		&footPrint::drawDbClassification);
	if (!status)
	{
		status.perror("registerNode");
		return status;
	}

	status = MHWRender::MDrawRegistry::registerSubSceneOverrideCreator(
			footPrint::drawDbClassification,
			footPrint::drawRegistrantId,
			FootPrintSubSceneOverride::Creator);
	if (!status)
	{
		status.perror("registerSubSceneOverrideCreator");
		return status;
	}

	// Register a custom selection mask with priority 2 (same as locators by default).
	MSelectionMask::registerSelectionType("footPrintSelection", 2);
	status = MGlobal::executeCommand("selectType -byName \"footPrintSelection\" 1");
	return status;
}

MStatus uninitializePlugin( MObject obj)
{
	MStatus status;
	MFnPlugin plugin( obj );

	status = MHWRender::MDrawRegistry::deregisterSubSceneOverrideCreator(
		footPrint::drawDbClassification,
		footPrint::drawRegistrantId);
	if (!status)
	{
		status.perror("deregisterSubSceneOverrideCreator");
		return status;
	}

	status = releaseShaders();
	if (!status)
	{
		status.perror("releaseShaders");
		return status;
	}

	status = plugin.deregisterNode( footPrint::id );
	if (!status)
	{
		status.perror("deregisterNode");
		return status;
	}

	// Deregister custom selection mask
	MSelectionMask::deregisterSelectionType("footPrintSelection");
	return status;
}
