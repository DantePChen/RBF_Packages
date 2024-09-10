#ifndef _CacheWriter_h_
#define _CacheWriter_h_

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

// Includes
#include "gpuCacheGeometry.h"
#include "gpuCacheMaterial.h"

#include <maya/MMatrix.h>
#include <maya/MFnTransform.h>
#include <maya/MTime.h>
#include <maya/MString.h>
#include <maya/MFileObject.h>

#include <memory>
#include <vector>

// Forward Declarations
class CacheXformSampler;
class CacheMeshSampler;


//==============================================================================
// CLASS CacheWriter
//==============================================================================

class CacheWriter
{
public:
    // Create a cache writer
    static std::shared_ptr<CacheWriter> create(const MString& impl,
        const MFileObject& file, char compressLevel);
    
    typedef std::shared_ptr<CacheWriter> CreateFunction(const MFileObject& file,
        char compressLevel);
    static void registerWriter(const MString& impl, CreateFunction func);

    virtual ~CacheWriter() {}
    virtual bool valid() const = 0;

    // Write the hierarchy of nodes under the given top level node to
    // the cache file.
    virtual void writeSubNodeHierarchy(
        const GPUCache::SubNode::Ptr& topNode, 
        double secondsPerSample, double startTimeInSeconds) = 0;

    // Write the materials to the cache file.
    virtual void writeMaterials(
        const GPUCache::MaterialGraphMap::Ptr& materialGraphMap,
        double secondsPerSample, double startTimeInSeconds) = 0;

    // Returns the actual file name the implementation is writing
    virtual const MFileObject& getFileObject() const = 0;

protected:
    CacheWriter() {}

private:
    // Prohibited and not implemented.
    CacheWriter(const CacheWriter&);
    const CacheWriter& operator=(const CacheWriter&);

    static std::map<std::string,void*> fsRegistry;
};


//==============================================================================
// CLASS CacheXformSampler
//==============================================================================

class CacheXformSampler
{
public:
    static std::shared_ptr<CacheXformSampler> create(const MObject& xformObject);

    ~CacheXformSampler();

    // Bake a sample at the current time.
    void addSample();

    bool isAnimated() const {
        return fXformAnimated || fVisibilityAnimated; }

	std::shared_ptr<const GPUCache::XformSample> getSample(double timeInSeconds);

private:
    CacheXformSampler(const MObject& xformObject);

    struct MakeSharedEnabler;
    
    MFnTransform    fXform;
    bool            fIsFirstSample;
    
	// local matrix
    MMatrix         fXformSample;
    bool            fXformAnimated;

	// local visibility
    bool            fVisibilitySample;
    bool            fVisibilityAnimated;
};


//==============================================================================
// CLASS CacheMeshSampler
//==============================================================================

class CacheMeshSampler
{
public:
	// note: about needUVs, the caller might also enable the UVs by calling enableUVs() afterwards
    static std::shared_ptr<CacheMeshSampler> create(bool needUVs);

    ~CacheMeshSampler();

    bool addSample(MObject meshObject, bool visibility);
	bool addSampleFromMesh(MFnMesh& mesh);

    bool isAnimated() const 
    {
        return fIsAnimated;
    }
    void enableUVs() 
	{
        fNeedUVs = true;
	}

    void setUseBaseTessellation()
    {
        fUseBaseTessellation = true;
    }

    // diffuseColor is per-instance attribute. CacheMeshSampler does not care about
    // instances so the diffuse color is passed from outside (Bakers).
    // getSample() is called for each instance.
    std::shared_ptr<const GPUCache::ShapeSample>
        getSample(double timeInSeconds, const MColor& diffuseColor);
    
private:
    CacheMeshSampler(bool needUVs);

    struct MakeSharedEnabler;

    struct AttributeSet
    {
        size_t fNumWires;
        size_t fNumTriangles;
        size_t fNumVerts;

        std::shared_ptr<GPUCache::IndexBuffer>    fWireVertIndices;
        std::vector<std::shared_ptr<GPUCache::IndexBuffer> > fTriangleVertIndices;
        std::shared_ptr<GPUCache::VertexBuffer>   fPositions;
        std::shared_ptr<GPUCache::VertexBuffer>   fNormals;
        std::shared_ptr<GPUCache::VertexBuffer>   fUVs;
        MBoundingBox                                fBoundingBox;
        bool                                        fVisibility;

        AttributeSet() 
            : fNumWires(0), fNumTriangles(0), fNumVerts(0), fVisibility(true)
        {}
        AttributeSet(
            MObject meshObject, bool visibility, bool needUVs);
        AttributeSet(MFnMesh& mesh, bool needUVs, bool useBaseTessellation);

        // Replaces the animated channels contained in this
        // AttributeSet with the one contained in "newer". Returns
        // whether any attributes are animated.
        bool updateAnimatedChannels(
            bool& animated, const AttributeSet& newer);
    };

    bool fNeedUVs;
    bool fUseBaseTessellation;
    bool fIsAnimated;
    AttributeSet fAttributeSet;
};

#endif

