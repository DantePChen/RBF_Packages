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

#include "gpuCacheSample.h"
#include "gpuCacheVBOProxy.h"

#include <Alembic/Util/Murmur3.h>

#include <memory>
#include <unordered_map>

#include <cassert>

namespace {

using namespace GPUCache;

//==============================================================================
// LOCAL FUNCTIONS & CLASSES
//==============================================================================

//==============================================================================
// CLASS ArrayBaseImp
//==============================================================================

class ArrayBaseImp 
{
public:
    typedef ArrayBase::Callback Callback;
    typedef ArrayBase::Key      Key;
    
    static void registerCreationCallback(Callback callback)
    {
        creationCallbacks.push_back(callback);
    }
    
    static void unregisterCreationCallback(Callback callback)
    {
        Callbacks::iterator it = std::find(
            creationCallbacks.begin(), creationCallbacks.end(), callback);
        if (it != creationCallbacks.end()) {
            creationCallbacks.erase(it);
        }
    }

    static void invokeCreationCallback(const Key& key)
    {
        for(const Callback& callback : creationCallbacks) {
            (*callback)(key);
        }
    }
    
    static void registerDestructionCallback(Callback callback)
    {
        destructionCallbacks.push_back(callback);
    }
        
    static void unregisterDestructionCallback(Callback callback)
    {
        Callbacks::iterator it = std::find(
            destructionCallbacks.begin(), destructionCallbacks.end(), callback);
        if (it != destructionCallbacks.end()) {
            destructionCallbacks.erase(it);
        }
    }

    static void invokeDestructionCallback(const Key& key)
    {
        for(const Callback& callback : destructionCallbacks) {
            (*callback)(key);
        }
    }
    
private:
    typedef std::vector<ArrayBase::Callback> Callbacks;
    
    static Callbacks creationCallbacks;
    static Callbacks destructionCallbacks;
};

ArrayBaseImp::Callbacks ArrayBaseImp::creationCallbacks;
ArrayBaseImp::Callbacks ArrayBaseImp::destructionCallbacks;


//==============================================================================
// CLASS ArrayRegistryImp
//==============================================================================

template <typename T>
class ArrayRegistryImp
{
public:
    typedef ArrayBase::Digest     Digest;
    typedef ArrayBase::Key        Key;
    typedef ArrayBase::KeyHash    KeyHash;
    typedef ArrayBase::KeyEqualTo KeyEqualTo;

    static ArrayRegistryImp<T>& singleton()
    { return fsSingleton; }

    ~ArrayRegistryImp()
    {
        // Unfortunately, we can't check that all buffers have been
        // freed here. The reason is Maya does not take the time to
        // clean-up the dependency graph when exiting. Therefore,
        // there might still exist some ShapeNode alived at exit time
        // and these will indirectly keep these buffers alive.
        //
        // The way to check that the mechanism is working correctly is
        // therefore to perform the following MEL commands "file -f
        // -new; gpuCache -q -sgs;" and check that everything has
        // been freed. The gpuCache regression test does that.
        //
        // assert(fMap.size() == 0);
    } 


    std::mutex& mutex() 
    { return fMutex; }
    
    std::shared_ptr<Array<T> > lookup(
        const Digest& digest,
        size_t size
    )
    {
        // Caller will accept either readable or non-readable.  First look for non-readable.
        std::shared_ptr<Array<T> > ret = lookupNonReadable(digest, size);
        if (!ret) {
            ret = lookupReadable(digest, size);
        }
        return ret;
    }

    std::shared_ptr<Array<T> > lookupNonReadable(
        const Digest& digest,
        size_t size
    )
    {
        typename Map::const_iterator it = fMapNonReadable.find(Key(size * sizeof(T), digest));
        if (it != fMapNonReadable.end()) {
            // Might return null if the weak_ptr<> is now dangling
            // but not yet removed from the map...
            std::shared_ptr<Array<T> > ret = it->second.lock();
            if (!ret) {
                fMapNonReadable.erase(it);
            }
            return ret;
        }
        else {
            return std::shared_ptr<Array<T> >();
        }
    }   

    std::shared_ptr<ReadableArray<T> > lookupReadable(
        const Digest& digest,
        size_t size
    )
    {
        typename MapReadable::const_iterator it = fMapReadable.find(Key(size * sizeof(T), digest));
        if (it != fMapReadable.end()) {
            // Might return null if the weak_ptr<> is now dangling
            // but not yet removed from the map...
            std::shared_ptr<ReadableArray<T> > ret = it->second.lock();
            if (!ret) {
                fMapReadable.erase(it);
            }
            return ret;
        }
        else {
            return std::shared_ptr<ReadableArray<T> >();
        }
    }

    void insert(std::shared_ptr<Array<T> > array)
    {
        if (array->isReadable()) {
            fMapReadable.insert(std::make_pair(array->key(), array->getReadableArray()));
        } else {
            fMapNonReadable.insert(std::make_pair(array->key(), array));
        }
    }

    void removeIfStaled(const Key& key, bool readable)
    {
        if (readable) {
            typename MapReadable::const_iterator it = fMapReadable.find(key);
            if (it != fMapReadable.end()) {
                // Might return null if the weak_ptr<> is now dangling
                // but not yet removed from the map...
                std::shared_ptr<Array<T> > ret = it->second.lock();
                if (!ret) {
                    // Get rid of the stalled entry so that insert() can
                    // work properly.
                    fMapReadable.erase(it);
                }
            }
        } else {
            typename Map::const_iterator it = fMapNonReadable.find(key);
            if (it != fMapNonReadable.end()) {
                // Might return null if the weak_ptr<> is now dangling
                // but not yet removed from the map...
                std::shared_ptr<Array<T> > ret = it->second.lock();
                if (!ret) {
                    // Get rid of the stalled entry so that insert() can
                    // work properly.
                    fMapNonReadable.erase(it);
                }
            }
        }
    }

private:
    typedef std::unordered_map<
        Key,
        std::weak_ptr<Array<T> >,
        KeyHash,
        KeyEqualTo> Map;
    typedef std::unordered_map<
        Key,
        std::weak_ptr<ReadableArray<T> >,
        KeyHash,
        KeyEqualTo> MapReadable;

    static ArrayRegistryImp fsSingleton;

    std::mutex fMutex;
    Map fMapNonReadable;
    MapReadable fMapReadable;
};

template <typename T>
ArrayRegistryImp<T> ArrayRegistryImp<T>::fsSingleton;

template class ArrayRegistryImp<IndexBuffer::index_t>;
template class ArrayRegistryImp<float>;


//==============================================================================
// CLASS IndexBufferRegistry
//==============================================================================

class IndexBufferRegistry
{
public:
    typedef IndexBuffer::index_t    index_t;
    typedef IndexBuffer::Key        Key;
    typedef IndexBuffer::KeyHash    KeyHash;
    typedef IndexBuffer::KeyEqualTo KeyEqualTo;
    
    static IndexBufferRegistry& singleton()
    { return fsSingleton; }

    ~IndexBufferRegistry() {}

    std::mutex& mutex() 
    { return fMutex; }
    
    
    std::shared_ptr<IndexBuffer> lookup(
        const std::shared_ptr<Array<index_t> >& array,
        const size_t beginIdx,
        const size_t endIdx
    )
    {
        Map::const_iterator it = fMap.find(Key(array, beginIdx, endIdx));
        if (it != fMap.end()) {
            // Might return null if the weak_ptr<> is now dangling
            // but not yet removed from the map...
            std::shared_ptr<IndexBuffer> ret = it->second.lock();
            if (!ret) {
                // Get rid of the stalled entry so that insert() can
                // work properly.
                fMap.erase(it);
            }
            return ret;
        }
        else {
            return std::shared_ptr<IndexBuffer>();
        }
    }   

    void insert(std::shared_ptr<IndexBuffer> buffer)
    {
        fMap.insert(
            std::make_pair(
                Key(buffer->array(), buffer->beginIdx(), buffer->endIdx()),
                buffer));
    }

    void removeIfStaled(
        const std::shared_ptr<Array<index_t> >& array,
        const size_t beginIdx,
        const size_t endIdx
    )
    {
        Map::const_iterator it = fMap.find(Key(array, beginIdx, endIdx));
        if (it != fMap.end()) {
            // Might return null if the weak_ptr<> is now dangling
            // but not yet removed from the map...
            std::shared_ptr<IndexBuffer> ret = it->second.lock();
            if (!ret) {
                // Get rid of the stalled entry so that insert() can
                // work properly.
                fMap.erase(it);
            }
        }
    }

    size_t nbAllocated()
    { return fMap.size(); }
    
    size_t nbAllocatedBytes()
    {
        size_t bytes = 0;
        for(const Map::value_type& v : fMap) {
            std::shared_ptr<IndexBuffer> buf = v.second.lock();
            if (buf) {
                bytes += buf->bytes();
            }
        }
        return bytes;
    }

private:
    typedef std::unordered_map<
        Key,
        std::weak_ptr<IndexBuffer>,
        KeyHash,
        KeyEqualTo
    > Map;

    static IndexBufferRegistry fsSingleton;

    std::mutex fMutex;
    Map fMap;
};

IndexBufferRegistry IndexBufferRegistry::fsSingleton;


//==============================================================================
// CLASS VertexBufferRegistry
//==============================================================================

class VertexBufferRegistry
{
public:
    typedef VertexBuffer::Key Key;
    typedef VertexBuffer::KeyHash KeyHash;
    typedef VertexBuffer::KeyEqualTo KeyEqualTo;
    
    static VertexBufferRegistry& singleton()
    { return fsSingleton; }

    ~VertexBufferRegistry() {}

    std::mutex& mutex() 
    { return fMutex; }
    
    std::shared_ptr<VertexBuffer> lookup(
        const std::shared_ptr<Array<float> >&     array,
        const MHWRender::MVertexBufferDescriptor&   desc
    )
    {
        Map::const_iterator it = fMap.find(Key(array, desc));
        if (it != fMap.end()) {
            // Might return null if the weak_ptr<> is now dangling
            // but not yet removed from the map...
            std::shared_ptr<VertexBuffer> ret = it->second.lock();
            if (!ret) {
                // Get rid of the stalled entry so that insert() can
                // work properly.
                fMap.erase(it);
            }
            return ret;
        }
        else {
            return std::shared_ptr<VertexBuffer>();
        }
    }   

    void insert(std::shared_ptr<VertexBuffer> buffer)
    {
        fMap.insert(
            std::make_pair(
                Key(buffer->array(), buffer->descriptor()),
                buffer));
    }

    void removeIfStaled(
        const std::shared_ptr<Array<float> >&     array,
        const MHWRender::MVertexBufferDescriptor&   desc
    )
    {
        Map::const_iterator it = fMap.find(Key(array, desc));
        if (it != fMap.end()) {
            // Might return null if the weak_ptr<> is now dangling
            // but not yet removed from the map...
            std::shared_ptr<VertexBuffer> ret = it->second.lock();
            if (!ret) {
                // Get rid of the stalled entry so that insert() can
                // work properly.
                fMap.erase(it);
            }
        }
    }

    size_t nbAllocated()
    { return fMap.size(); }
    
    size_t nbAllocatedBytes()
    {
        size_t bytes = 0;
        for(const Map::value_type& v : fMap) {
            std::shared_ptr<VertexBuffer> buf = v.second.lock();
            if (buf) {
                bytes += buf->bytes();
            }
        }
        return bytes;
    }

private:
    typedef std::unordered_map<
        Key,
        std::weak_ptr<VertexBuffer>,
        KeyHash,
        KeyEqualTo
    > Map;

    static VertexBufferRegistry fsSingleton;

    std::mutex fMutex;
    Map fMap;
};

VertexBufferRegistry VertexBufferRegistry::fsSingleton;

}


namespace GPUCache {

//==============================================================================
// CLASS ArrayBase
//==============================================================================

void ArrayBase::registerCreationCallback(Callback callback)
{
    ArrayBaseImp::registerCreationCallback(callback);
}

void ArrayBase::unregisterCreationCallback(Callback callback)
{
    ArrayBaseImp::unregisterCreationCallback(callback);
}

void ArrayBase::registerDestructionCallback(Callback callback)
{
    ArrayBaseImp::registerDestructionCallback(callback);
}

void ArrayBase::unregisterDestructionCallback(Callback callback)
{
    ArrayBaseImp::unregisterDestructionCallback(callback);
}

ArrayBase::ArrayBase(size_t bytes, const Digest& digest, bool isReadable)
    : fKey(bytes, digest)
    , fIsReadable(isReadable)
{
    ArrayBaseImp::invokeCreationCallback(fKey);
}

ArrayBase::~ArrayBase()
{
    ArrayBaseImp::invokeDestructionCallback(fKey);
}

//==============================================================================
// CLASS ArrayReadInterface
//==============================================================================

template class ArrayReadInterface<IndexBuffer::index_t>;
template class ArrayReadInterface<float>;


//==============================================================================
// CLASS Array
//==============================================================================

template <typename T>
Array<T>::~Array()
{
    std::lock_guard<std::mutex> lock(ArrayRegistryImp<T>::singleton().mutex());
    ArrayRegistryImp<T>::singleton().removeIfStaled(key(), isReadable());
}

template class Array<IndexBuffer::index_t>;
template class Array<float>;


//==============================================================================
// CLASS ReadableArray
//==============================================================================

template class ReadableArray<IndexBuffer::index_t>;
template class ReadableArray<float>;


//==============================================================================
// CLASS ArrayRegistry
//==============================================================================

template <typename T>
std::mutex& ArrayRegistry<T>::mutex()
{
    return ArrayRegistryImp<T>::singleton().mutex();
}

template <typename T>
std::shared_ptr<Array<T> > ArrayRegistry<T>::lookup(
    const Digest& digest,
    size_t size
)
{
    std::shared_ptr<Array<T> > result =
        ArrayRegistryImp<T>::singleton().lookup(digest, size);

    assert(!result || result->digest() == digest);
    assert(!result || result->bytes()  == size * sizeof(T));
    
    return result;
}

template <typename T>
std::shared_ptr<Array<T> > ArrayRegistry<T>::lookupNonReadable(
    const Digest& digest,
    size_t size
)
{
    std::shared_ptr<Array<T> > result =
        ArrayRegistryImp<T>::singleton().lookupNonReadable(digest, size);

    assert(!result || result->digest() == digest);
    assert(!result || result->bytes()  == size * sizeof(T));
    
    return result;
}

template <typename T>
 std::shared_ptr<ReadableArray<T> >  ArrayRegistry<T>::lookupReadable(
    const Digest& digest,
    size_t size
)
{
    std::shared_ptr<ReadableArray<T> > result =
        ArrayRegistryImp<T>::singleton().lookupReadable(digest, size);

    assert(!result || result->digest() == digest);
    assert(!result || result->bytes()  == size * sizeof(T));
    
    return result;
}

template <typename T>
void ArrayRegistry<T>::insert(
    std::shared_ptr<Array<T> > array
)
{
    ArrayRegistryImp<T>::singleton().insert(array);
}

template class ArrayRegistry<IndexBuffer::index_t>;
template class ArrayRegistry<float>;

//==============================================================================
// CLASS SharedArray
//==============================================================================

template <typename T>
struct SharedArray<T>::MakeSharedEnabler : public SharedArray<T> {
    MakeSharedEnabler(
            const GPUCache::shared_array<T> &data,
            size_t size,
            const Digest &digest
    ) : SharedArray<T>(
            data,
            size,
            digest
    ) {}
};

template <typename T>
std::shared_ptr<ReadableArray<T> >
SharedArray<T>::create(
    const GPUCache::shared_array<T>& data, size_t size)
{
    // Compute the Murmur3 cryptographic hash-key.
    Digest digest;
    Alembic::Util::MurmurHash3_x64_128(
        data.get(), size * sizeof(T), sizeof(T), digest.words);

    return create(data, digest, size);
}

template <typename T>
std::shared_ptr<ReadableArray<T> >
SharedArray<T>::create(
    const GPUCache::shared_array<T>& data, Digest digest, size_t size)
{
    // We first look if a similar array already exists in the
    // cache. If so, we return the cached array to promote sharing as
    // much as possible.
    std::shared_ptr<ReadableArray<T> > ret;
    {
        std::lock_guard<std::mutex> lock(ArrayRegistry<T>::mutex());

        ret = ArrayRegistry<T>::lookupReadable(digest, size);
        
        if (!ret) {
            ret = std::make_shared<MakeSharedEnabler>(
                data, size, digest);
            ArrayRegistry<T>::insert(ret);
        }
    }
    return ret;
}

template <typename T>
SharedArray<T>::~SharedArray()
{}

template <typename T>
const T* SharedArray<T>::get() const
{
    return fData.get();
}

template class SharedArray<IndexBuffer::index_t>;
template class SharedArray<float>;


//==============================================================================
// CLASS IndexBuffer
//==============================================================================

struct IndexBuffer::MakeSharedEnabler: public IndexBuffer {
    MakeSharedEnabler(
            const std::shared_ptr<Array<index_t> >& array,
            const size_t beginIdx,
            const size_t endIdx
    ): IndexBuffer(
            array,
            beginIdx,
            endIdx
    ) {}
};

std::shared_ptr<IndexBuffer> IndexBuffer::create(
    const std::shared_ptr<Array<index_t> >& array,
    const size_t beginIdx,
    const size_t endIdx
)
{
    // We first look if a similar array already exists in the
    // cache. If so, we return the cached array to promote sharing as
    // much as possible.
    std::shared_ptr<IndexBuffer> ret;
    {
        std::lock_guard<std::mutex> lock(
            IndexBufferRegistry::singleton().mutex());

        ret = IndexBufferRegistry::singleton().lookup(
            array, beginIdx, endIdx);
        
        if (!ret) {
            ret = std::make_shared<MakeSharedEnabler>(
                array, beginIdx, endIdx);
            IndexBufferRegistry::singleton().insert(ret);
        }
    }

    return ret;
}

size_t IndexBuffer::nbAllocated()
{
    std::lock_guard<std::mutex> lock(
        IndexBufferRegistry::singleton().mutex());
    return IndexBufferRegistry::singleton().nbAllocated();
}

    
size_t IndexBuffer::nbAllocatedBytes()
{
    std::lock_guard<std::mutex> lock(
        IndexBufferRegistry::singleton().mutex());
    return IndexBufferRegistry::singleton().nbAllocatedBytes();
}

IndexBuffer::~IndexBuffer()
{
    std::lock_guard<std::mutex> lock(
        IndexBufferRegistry::singleton().mutex());
    IndexBufferRegistry::singleton().removeIfStaled(
        fArray, fBeginIdx, fEndIdx);
}

void IndexBuffer::ReplaceArrayInstance(std::shared_ptr<Array<index_t> >& newArray) const
{
    assert(ArrayBase::KeyEqualTo()(fArray->key(), newArray->key()));

    if (fArray != newArray) {
        std::shared_ptr<Array<index_t> >& nonConstArray = const_cast<std::shared_ptr<Array<index_t> >& >(fArray);
        nonConstArray = newArray;
    }
}


//==============================================================================
// CLASS VertexBuffer
//==============================================================================

struct VertexBuffer::MakeSharedEnabler: public VertexBuffer{
    MakeSharedEnabler(
            const std::shared_ptr<Array<float> >&     array,
            const MHWRender::MVertexBufferDescriptor&   desc
    ): VertexBuffer(
            array,
            desc
    ) {}
};

std::shared_ptr<VertexBuffer>
VertexBuffer::createPositions(
    const std::shared_ptr<Array<float> >& array)
{
    return create(array,
                  MHWRender::MVertexBufferDescriptor(
                      MString(""),
                      MHWRender::MGeometry::kPosition,
                      MHWRender::MGeometry::kFloat, 3));
}

std::shared_ptr<VertexBuffer>
VertexBuffer::createNormals(
    const std::shared_ptr<Array<float> >& array)
{
    return create(array,
                  MHWRender::MVertexBufferDescriptor(
                      MString(""),
                      MHWRender::MGeometry::kNormal,
                      MHWRender::MGeometry::kFloat, 3));
}

std::shared_ptr<VertexBuffer>
VertexBuffer::createUVs(
    const std::shared_ptr<Array<float> >& array)
{
    return create( array,
                   MHWRender::MVertexBufferDescriptor(
                       MString("mayaUVIn"),
                       MHWRender::MGeometry::kTexture,
                       MHWRender::MGeometry::kFloat, 2));
}

std::shared_ptr<VertexBuffer>
VertexBuffer::create(
    const std::shared_ptr<Array<float> >&     array,
    const MHWRender::MVertexBufferDescriptor&   desc)
{
    // We first look if a similar array already exists in the
    // cache. If so, we return the cached array to promote sharing as
    // much as possible.
    std::shared_ptr<VertexBuffer> ret;
    {
        std::lock_guard<std::mutex> lock(
            VertexBufferRegistry::singleton().mutex());

        ret = VertexBufferRegistry::singleton().lookup(array, desc);
        
        if (!ret) {
            ret = std::make_shared<MakeSharedEnabler>(
                array, desc);
            VertexBufferRegistry::singleton().insert(ret);
        }
    }

    return ret;
}

size_t VertexBuffer::nbAllocated()
{
    std::lock_guard<std::mutex> lock(
        VertexBufferRegistry::singleton().mutex());
    return VertexBufferRegistry::singleton().nbAllocated();
}

    
size_t VertexBuffer::nbAllocatedBytes()
{
    std::lock_guard<std::mutex> lock(
        VertexBufferRegistry::singleton().mutex());
    return VertexBufferRegistry::singleton().nbAllocatedBytes();
}

VertexBuffer::~VertexBuffer()
{
    std::lock_guard<std::mutex> lock(
        VertexBufferRegistry::singleton().mutex());
    VertexBufferRegistry::singleton().removeIfStaled(
        fArray, fDescriptor);
}

void VertexBuffer::ReplaceArrayInstance(std::shared_ptr<Array<float> >& newArray) const
{
    assert(ArrayBase::KeyEqualTo()(fArray->key(), newArray->key()));

    if (fArray != newArray) {
        std::shared_ptr<Array<float> >& nonConstArray = const_cast<std::shared_ptr<Array<float> >& >(fArray);
        nonConstArray = newArray;
    }
}


//==============================================================================
// CLASS ShapeSample
//==============================================================================

struct ShapeSample::MakeSharedEnabler: public ShapeSample {
    MakeSharedEnabler(
            double timeInSeconds,
            size_t numWires,
            size_t numVerts,
            const std::shared_ptr<IndexBuffer>&  wireVertIndices,
            const std::shared_ptr<IndexBuffer>&  triangleVertIndices,
            const std::shared_ptr<VertexBuffer>& positions,
            const MBoundingBox& boundingBox,
            const MColor&       diffuseColor,
            bool                visibility
    ):ShapeSample(
            timeInSeconds,
            numWires,
            numVerts,
            wireVertIndices,
            triangleVertIndices,
            positions,
            boundingBox,
            diffuseColor,
            visibility
    ) {}

    MakeSharedEnabler(
            double timeInSeconds,
            size_t numWires,
            size_t numVerts,
            const std::shared_ptr<IndexBuffer>&  wireVertIndices,
            const std::vector<std::shared_ptr<IndexBuffer> >&  triangleVertIndices,
            const std::shared_ptr<VertexBuffer>& positions,
            const MBoundingBox& boundingBox,
            const MColor&       diffuseColor,
            bool                visibility
    ):ShapeSample(
            timeInSeconds,
            numWires,
            numVerts,
            wireVertIndices,
            triangleVertIndices,
            positions,
            boundingBox,
            diffuseColor,
            visibility
    ) {}
};


ShapeSample::ShapeSample(
    double timeInSeconds,
    size_t numWires,
    size_t numVerts,
    const std::shared_ptr<IndexBuffer>&  wireVertIndices,
    const std::shared_ptr<IndexBuffer>&  triangleVertIndices,
    const std::shared_ptr<VertexBuffer>& positions,
    const MBoundingBox& boundingBox,
    const MColor&       diffuseColor,
    bool                visibility
) 
    : fTimeInSeconds(timeInSeconds),
      fNumWires(numWires),
      fNumVerts(numVerts),
      fWireVertIndices(wireVertIndices),
	  fTriangleVertIndices(
	  std::vector<std::shared_ptr<IndexBuffer> >(1, triangleVertIndices)),
      fPositions(positions),
      fBoundingBox(boundingBox),
      fDiffuseColor(diffuseColor),
      fVisibility(visibility),
      fBoundingBoxPlaceHolder(false)
{
    assert( wireVertIndices ? (wireVertIndices->numIndices() == 2 * fNumWires) : (fNumWires == 0) );
    assert( positions ? (positions->numVerts() == fNumVerts) : (fNumVerts == 0) );
}

ShapeSample::ShapeSample(
    double timeInSeconds,
    size_t numWires,
    size_t numVerts,
    const std::shared_ptr<IndexBuffer>&  wireVertIndices,
    const std::vector<std::shared_ptr<IndexBuffer> >&  triangleVertIndices,
    const std::shared_ptr<VertexBuffer>& positions,
    const MBoundingBox& boundingBox,
    const MColor&       diffuseColor,
    bool                visibility
) 
    : fTimeInSeconds(timeInSeconds),
      fNumWires(numWires),
      fNumVerts(numVerts),
      fWireVertIndices(wireVertIndices),
      fTriangleVertIndices(triangleVertIndices),
      fPositions(positions),
      fBoundingBox(boundingBox),
      fDiffuseColor(diffuseColor),
      fVisibility(visibility),
      fBoundingBoxPlaceHolder(false)
{
    assert( wireVertIndices ? (wireVertIndices->numIndices() == 2 * fNumWires) : (fNumWires == 0) );
    assert( positions ? (positions->numVerts() == fNumVerts) : (fNumVerts == 0) );
}

ShapeSample::~ShapeSample()
{}

size_t ShapeSample::numTriangles() const
{
    size_t result = 0;
    for(size_t i=0; i<numIndexGroups(); ++i) {
        result += numTriangles(i);
    }
    return result;
}

void ShapeSample::setNormals(
    const std::shared_ptr<VertexBuffer>& normals
)
{
    assert( !normals || normals->numVerts() == fNumVerts );
    fNormals = normals;
}

void ShapeSample::setUVs(
    const std::shared_ptr<VertexBuffer>& uvs
)
{
    assert( !uvs || uvs->numVerts() == fNumVerts );
    fUVs = uvs;
}

std::shared_ptr<ShapeSample> ShapeSample::create(
        double timeInSeconds,
        size_t numWires,
        size_t numVerts,
        const std::shared_ptr<IndexBuffer>&  wireVertIndices,
        const std::shared_ptr<IndexBuffer>&  triangleVertIndices,
        const std::shared_ptr<VertexBuffer>& positions,
        const MBoundingBox& boundingBox,
        const MColor&       diffuseColor,
        bool                visibility)
{
    return std::make_shared<MakeSharedEnabler>(
            timeInSeconds,
            numWires, numVerts,
            wireVertIndices, triangleVertIndices,
            positions, boundingBox, diffuseColor, visibility);
}

std::shared_ptr<ShapeSample> ShapeSample::create(
        double timeInSeconds,
        size_t numWires,
        size_t numVerts,
        const std::shared_ptr<IndexBuffer>&  wireVertIndices,
        const std::vector<std::shared_ptr<IndexBuffer> >&  triangleVertIndices,
        const std::shared_ptr<VertexBuffer>& positions,
        const MBoundingBox& boundingBox,
        const MColor&       diffuseColor,
        bool                visibility)
{
    return std::make_shared<MakeSharedEnabler>(
            timeInSeconds,
            numWires, numVerts,
            wireVertIndices, triangleVertIndices,
            positions, boundingBox, diffuseColor, visibility);
}

/*==============================================================================
 * CLASS XformSample
 *============================================================================*/

struct XformSample::MakeSharedEnabler : public XformSample {
    MakeSharedEnabler(double timeInSeconds,
                      const MMatrix &xform,
                      const MBoundingBox &boundingBox,
                      bool visibility) :
            XformSample(
                    timeInSeconds,
                    xform,
                    boundingBox,
                    visibility) {}
};


std::shared_ptr<XformSample> XformSample::create(
        double              timeInSeconds,
        const MMatrix&      xform,
        const MBoundingBox& boundingBox,
        bool                visibility)
{
    return std::make_shared<MakeSharedEnabler>(
            timeInSeconds, xform, boundingBox, visibility);
}

}
