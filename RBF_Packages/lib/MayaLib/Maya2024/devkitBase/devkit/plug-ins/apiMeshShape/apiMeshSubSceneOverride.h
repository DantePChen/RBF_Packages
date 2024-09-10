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

///////////////////////////////////////////////////////////////////////////////
//
// apiMeshSubSceneOverride.h
//
// Handles vertex data preparation for drawing the user defined shape in
// Viewport 2.0.
//
////////////////////////////////////////////////////////////////////////////////

#include <maya/MPxSubSceneOverride.h>
#include <maya/MSharedPtr.h>
#include <map>
#include <set>
#include <vector>
#include <memory>

class apiMesh;
class apiMeshGeom;
struct ID3D11Buffer;

namespace apiMeshSubSceneOverrideHelpers {
	class LinkLostUserData;
}

// A multimap to store all view-selected face indices of each DAG instance
//
// Key: instance index
// Val: face index
//
typedef std::multimap<int, int> ViewSelectedFaceInfo;

// A map to store all view-selected face indices of each DAG instance in each view
//
// Key: name of the model panel
//
typedef std::map<std::string, ViewSelectedFaceInfo> ViewSelectedFaceInfoMap;

class apiMeshSubSceneOverride : public MHWRender::MPxSubSceneOverride
{
public:
	static MPxSubSceneOverride* Creator(const MObject& obj)
	{
		return new apiMeshSubSceneOverride(obj);
	}

	~apiMeshSubSceneOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	bool requiresUpdate(
		const MHWRender::MSubSceneContainer& container,
		const MHWRender::MFrameContext& frameContext) const override;
	void update(
		MHWRender::MSubSceneContainer& container,
		const MHWRender::MFrameContext& frameContext) override;
	bool furtherUpdateRequired(
		const MHWRender::MFrameContext& frameContext) override;

	bool areUIDrawablesDirty() const override;
	bool hasUIDrawables() const override;
	void addUIDrawables(
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext) override;

	bool enableUpdateForSelection() const override;

	void updateSelectionGranularity(
		const MDagPath& path,
		MHWRender::MSelectionContext& selectionContext) override;

    bool getInstancedSelectionPath(
        const MHWRender::MRenderItem& renderItem,
        const MHWRender::MIntersection& intersection,
        MDagPath& dagPath
    ) const override;

	void untrackLinkLostData(apiMeshSubSceneOverrideHelpers::LinkLostUserData* data);

	static MStatus registerComponentConverters();
	static MStatus deregisterComponentConverters();

private:
	apiMeshSubSceneOverride(const MObject& obj);

	void manageRenderItems(
		MHWRender::MSubSceneContainer& container,
		const MHWRender::MFrameContext& frameContext,
		bool updateGeometry);

	void manageIsolateSelectRenderItems(
		MHWRender::MSubSceneContainer& container,
		const MHWRender::MFrameContext& frameContext,
		const MDagPathArray& instances,
		const ViewSelectedFaceInfo& currInfo,
		const MObject& shader,
		const bool updateMaterial,
		const bool updateGeometry);

	void rebuildGeometryBuffers();
	void rebuildActiveComponentIndexBuffers();

	void deleteBuffers();
	void deleteGeometryBuffers();
	void deleteActiveComponentIndexBuffers();

	static void onShadedItemLinkLost(MUserData* userData);

    void updateRenderItemShader(
        MRenderItem&,
        bool updateMaterial,
        const MObject& surfaceShaderNode,
        const MDagPath& dagPath,
        bool nonTextured
    );

	MObject fObject;
	apiMesh* fMesh = nullptr;

    struct InstanceInfo
    {
        MMatrix fTransform;
        bool fIsSelected;

        InstanceInfo() : fIsSelected(false) {}
        InstanceInfo(const MMatrix& matrix, bool selected)
            : fTransform(matrix), fIsSelected(selected)
        {}
    };
    typedef std::map<unsigned int, InstanceInfo> InstanceInfoMap;
	InstanceInfoMap fInstanceInfoCache;

	MMatrix fInstance0Transform;

	ViewSelectedFaceInfoMap fViewSelectedFaceInfoCache;

	static std::set<std::string> sViewSelectedFaceSelectionNames;

	static const MString sWireName;
	static const MString sSelectName;
	static const MString sBoxName;
	static const MString sSelectedBoxName;
	static const MString sShadedName;
	static const MString sTexturedName;

	static const MString sVertexSelectionName;
	static const MString sEdgeSelectionName;
	static const MString sFaceSelectionName;

	static const MString sActiveVertexName;
	static const MString sActiveEdgeName;
	static const MString sActiveFaceName;

	MHWRender::MShaderInstance* fWireShader = nullptr;
	MHWRender::MShaderInstance* fThickWireShader = nullptr;
	MHWRender::MShaderInstance* fSelectShader = nullptr;
	MHWRender::MShaderInstance* fThickSelectShader = nullptr;
	MHWRender::MShaderInstance* fShadedShader = nullptr;
	MHWRender::MShaderInstance* fVertexComponentShader = nullptr;
	MHWRender::MShaderInstance* fEdgeComponentShader = nullptr;
	MHWRender::MShaderInstance* fFaceComponentShader = nullptr;

    std::unique_ptr<MHWRender::MVertexBuffer>
        fPositionBuffer,
        fNormalBuffer,
        fBoxPositionBuffer;

    std::unique_ptr<MHWRender::MIndexBuffer>
        fWireIndexBuffer,
        fBoxIndexBuffer,
        fShadedIndexBuffer,
        fActiveVerticesIndexBuffer,
        fActiveEdgesIndexBuffer,
        fActiveFacesIndexBuffer;

	// Client buffers
	unsigned int fBoxPositionBufferId = 0;
	unsigned int fBoxIndexBufferId = 0;
	ID3D11Buffer* fBoxPositionBufferDX = nullptr;
	ID3D11Buffer* fBoxIndexBufferDX = nullptr;

    float fThickLineWidth = -1.0f;
    unsigned int fNumInstances = 0;
    bool fIsInstanceMode = false;
	bool fAreUIDrawablesDirty = true;

	// Variables to control sample queue of updates to
	// allow for line width to increase incrementally without
	// user control.
	bool fUseQueuedLineUpdate = false; // Set to true to run sample line width update code
	float fQueuedLineWidth = -1.0f;
	bool fQueueUpdate = false;

	std::set<int> fActiveVerticesSet;
	std::set<int> fActiveEdgesSet;
	std::set<int> fActiveFacesSet;

	std::vector<MSharedPtr<apiMeshSubSceneOverrideHelpers::LinkLostUserData>> fLinkLostUserDatas;
};
