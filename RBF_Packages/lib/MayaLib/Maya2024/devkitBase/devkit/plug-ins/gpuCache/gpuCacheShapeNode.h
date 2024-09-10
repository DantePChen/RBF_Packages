#ifndef _gpuCacheShapeNode_h_
#define _gpuCacheShapeNode_h_

//-
//**************************************************************************/
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk 
// license agreement provided at the time of installation or download, 
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
//**************************************************************************/
//+

#include "gpuCacheGeometry.h"
#include "gpuCacheMaterial.h"
#include "CacheReader.h"

#include <maya/MAnimControl.h>
#include <maya/MObjectHandle.h>
#include <maya/MPxSurfaceShape.h>
#include <maya/MPxSurfaceShapeUI.h>
#include <maya/MMessage.h>
#include <maya/MPxGeometryData.h>

#include <unordered_map>

#include "gpuCacheSpatialSubdivision.h"
#include <vector>


namespace GPUCache {

namespace ShapeNodePrivate {

	/*
	This class is used to cache pointers to vertex/index buffers
	and other necessary geometry information to be used by the 
	acceleration structures ( required by the make live functionality)
	*/
	class BufferCache{
	public:
		BufferCache(double seconds){fBufferReadTime = seconds; fUseCachedBuffers = false; fTotalNumTris=0; fTotalNumVerts=0; fNumShapes=0;}
		~BufferCache(){
		}

		std::vector<MMatrix> fXFormMatrix;
		std::vector<MMatrix> fXFormMatrixInverse;

		std::vector<IndexBuffer::ReadInterfacePtr> fTriangleVertIndices;
		std::vector<IndexBuffer::ReadInterfacePtr> fEdgeVertIndices;
		std::vector<VertexBuffer::ReadInterfacePtr> fPositions;
		std::vector<size_t> fNumTriangles;
		std::vector<size_t> fNumEdges;
		std::vector<MBoundingBox> fBoundingBoxes;

		unsigned int fNumShapes;
		unsigned int fTotalNumTris;
		unsigned int fTotalNumVerts;
		bool fUseCachedBuffers;
		double fBufferReadTime;
	};
}


///////////////////////////////////////////////////////////////////////////////
//
// ShapeNode
//
// Keeps track of animated shapes held in a memory cache.
//
////////////////////////////////////////////////////////////////////////////////


class ShapeNode : public MPxSurfaceShape
{
public:

    static void* creator();
    static MStatus initialize();
    static MStatus uninitialize();
    static MStatus init3dViewPostRenderCallbacks();

    static const MTypeId id;
    static const MString drawDbClassificationGeometry;
    static const MString drawDbClassificationSubScene;
    static const MString drawRegistrantId;
    static MObject aCacheFileName;
    static MObject aCacheGeomPath;

	// Name of dynamic attribute to hold cache start frame
	//
	static const MString sLongNameCacheStartFrame;
	static const MString sShortNameCacheStartFrame;

	static const char* nodeTypeName;
	static const char* selectionMaskName;

	enum CacheReadingState {
		kCacheReadingFile,
		kCacheReadingDone
	};

    ShapeNode();
    ~ShapeNode() override;

    void postConstructor() override;

    bool isBounded() const override;
    MBoundingBox boundingBox() const override;

	bool getInternalValue( const MPlug&, MDataHandle&)		override;
    bool setInternalValue( const MPlug&, const MDataHandle&)	override;
    void copyInternalData(MPxNode* source) override;

    MStringArray	getFilesToArchive( bool shortName = false,
                                               bool unresolvedName = false,
                                               bool markCouldBeImageSequence = false ) const override;
	bool match( const MSelectionMask & mask,
		const MObjectArray& componentList ) const override;

	MSelectionMask getShapeSelectionMask() const override;

	bool excludeAsPluginShape() const override;

    void refreshCachedGeometry( bool clearFileCache = false );
	void refreshOtherCachedShapes( const MString& cacheFileName );
    
    const GPUCache::SubNode::Ptr& getCachedGeometry() const;

    const GPUCache::MaterialGraphMap::Ptr& getCachedMaterial() const;

	const CacheFileEntry::MPtr& getCacheFileEntry() const;

    CacheFileEntry::BackgroundReadingState backgroundReadingState() const;
    
	// Callback when the gpuCache node is added to the model (create / undo-delete)
	void addedToModelCB();
    // Callback when the gpuCache node is removed from model (delete)
    void removedFromModelCB();
    // Callback when the current time is changed (animation)
    void timeChangeCB(double timeInSeconds);

	void updateGeomPath( const MString& validatedGeomPath ) const;

	bool canMakeLive() const override;

	bool readBuffers(const SubNode::Ptr subNode, double seconds)const;

	void closestPoint(const MPoint &toThisPoint, MPoint &theClosestPoint, double tolerance = 0.1) override;
	MStatus closestIntersectWithNorm (const MPoint &raySource, const MVector &rayDirection, MPoint &point, MVector &normal );

	bool closestPoint( const MPoint &raySource, const MVector &rayDirection, MPoint &theClosestPoint, MVector &theClosestNormal, bool findClosestOnMiss, double tolerance=MPoint_kTol) override;

	unsigned int getIntersectionAccelerator(const gpuCacheIsectAccelParams& accelParams, double seconds) const;


	// Refresh the time offset, which is used to calculate the effective time
	// when fetching cached infor for this ShapeNode.
	//
	void refreshTimeOffset();

	// Given the currentTime, the frame at which the cached animation starts,
	// and the frame at which the user wants it to start (i.e., the value of
	// aCacheStartFrame), return the time that will allow us to fetch the cache
	// at the desired frame.
	//
	MTime getEffectiveTime() const
	{
		if( fTimeOffsetInvalid ){
			const_cast<ShapeNode*>(this)->refreshTimeOffset();
		}
		return( MTime(MAnimControl::currentTime())+fTimeOffset );
	}


	// Callback from CacheReader to dirty VP2 geometry draw status for affected shapes.
	static void dirtyVP2Geometry( const MString& fileName );

private:
    // Prohibited and not implemented.
    ShapeNode(const ShapeNode& obj);
    const ShapeNode& operator=(const ShapeNode& obj);

    bool setInternalValues(const MString& newFileName,
                           const MString& newGeomPath,
						   const MTime& newStartFrame);
	void getExternalContent(MExternalContentInfoTable& table) const override;
	void setExternalContent(const MExternalContentLocationTable& table) override;
	
	bool	getEdgeSnapPoint(const MPoint &rayPoint, const MVector &rayDirection, MPoint &theClosestPoint);

    mutable MString fCacheFileName;
	mutable MString fResolvedCacheFileName;
    mutable MString fCacheGeomPath;

	// Store the value of aCacheStartFrame. Once we know this has been
	// initialized properly, compute the difference between it and the
	// playback's start time, and record it in fTimeOffset. If/when the value
	// of the aCacheStartFrame attribute changes (e.g., if the user changes
	// the value in that AE or the channel box), re-compute fTimeOffset (which
	// is used in getEffectiveTime).
	//
    mutable MTime fCacheStartFrame;	// Value of aCacheStartFrame
    mutable MTime fAnimStartFrame;	// Frame where gpuCache starts animating

	bool fCacheStartFrameInitialized{false};// Set to true when fCacheStartFrame has been initialized

	// Set this variable to true when fTimeOffset is known to be invalid
	// (e.g., if fCacheStartFrame hasn't been initialized, either from
	// animTimeFrame or from a setAttr when reading a file, or if the value
	// of fCacheStartFrame has been changed since the last time we needed to
	// access fTimeOffset).
	//
	bool fTimeOffsetInvalid{true};
	
	mutable MTime fTimeOffset;

	mutable ShapeNodePrivate::BufferCache* fBufferCache;
	mutable std::vector<gpuCacheSpatialSubdivision *> fSpatialSub;

    mutable GPUCache::SubNode::Ptr                   fCachedGeometry;
    mutable GPUCache::MaterialGraphMap::Ptr          fCachedMaterial;
	mutable CacheReadingState						 fCacheReadingState;
	mutable CacheFileEntry::MPtr                     fCacheFileEntry;

    mutable MBoundingBox fBoundingBox;

    MCallbackId fRemoveFromModelCallbackId;
    MCallbackId fTimeChangeCallbackId;

    static MCallbackId fsModelEditorChangedCallbackId;
};


///////////////////////////////////////////////////////////////////////////////
//
// CacheShapeRegistry
//
// Keeps track of gpuCache shape nodes
//
////////////////////////////////////////////////////////////////////////////////

class CacheShapeRegistry
{
public:
	typedef std::unordered_multimap<MString, MObjectHandle, GPUCache::MStringHash> Map;

	~CacheShapeRegistry();

	static CacheShapeRegistry& theCache();

	void					getAll( std::vector<MObjectHandle>& shapes );

	void					find(const MString& key, std::vector<MObjectHandle>& shapes);
	bool					insert(const MString& key, const MObjectHandle& shape);
	bool					remove(const MString& key, const MObjectHandle& shape);
	void					clear();

private:
	CacheShapeRegistry();

	static CacheShapeRegistry fsSingleton;
	static Map fMap;
};



///////////////////////////////////////////////////////////////////////////////
//
// DisplayPref
//
// Keeps track of the display preference.
//
////////////////////////////////////////////////////////////////////////////////

class DisplayPref
{
public:
    enum WireframeOnShadedMode {
        kWireframeOnShadedFull,
        kWireframeOnShadedReduced,
        kWireframeOnShadedNone
    };

    static WireframeOnShadedMode wireframeOnShadedMode();

    static MStatus initCallback();
    static MStatus removeCallback();

private:
    static void displayPrefChanged(void*);

    static WireframeOnShadedMode fsWireframeOnShadedMode;
    static MCallbackId fsDisplayPrefChangedCallbackId;
};


///////////////////////////////////////////////////////////////////////////////
//
// ShapeUI
//
// Displays animated shapes held in a memory cache.
//
////////////////////////////////////////////////////////////////////////////////

class ShapeUI : public MPxSurfaceShapeUI
{
public:

    static void* creator();

    ShapeUI();
    ~ShapeUI() override;

    void getDrawRequests(const MDrawInfo & info,
                                 bool objectAndActiveOnly,
                                 MDrawRequestQueue & queue) override;

    // Viewport 1.0 draw
    void draw(const MDrawRequest & request, M3dView & view) const override;
    
	bool snap(MSelectInfo &snapInfo) const override;
	bool select(MSelectInfo &selectInfo, MSelectionList &selectionList,
                        MPointArray &worldSpaceSelectPts ) const override;


private:
    // Prohibited and not implemented.
	ShapeUI(const ShapeUI& obj);
	const ShapeUI& operator=(const ShapeUI& obj);

	static MPoint getPointAtDepth( MSelectInfo &selectInfo,double depth);

    // Helper functions for the viewport 1.0 drawing purposes.
    void drawBoundingBox(const MDrawRequest & request, M3dView & view ) const;
    void drawWireframe(const MDrawRequest & request, M3dView & view ) const;
    void drawShaded(const MDrawRequest & request, M3dView & view, bool depthOffset ) const;
    
	// Draw Tokens
	enum DrawToken {
        kBoundingBox,
		kDrawWireframe,
        kDrawWireframeOnShaded,
		kDrawSmoothShaded,
		kDrawSmoothShadedDepthOffset
	};
};

}

#endif
