#ifndef _gpuCacheGeometry_h_
#define _gpuCacheGeometry_h_

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

///////////////////////////////////////////////////////////////////////////////
//
// Geometry
//
// Data structures about the animated geometry held by the memory cache.
//
////////////////////////////////////////////////////////////////////////////////

#include "gpuCacheSample.h"
#include "gpuCacheTimeInterval.h"

#include <maya/MTime.h>
#include <maya/MString.h>

#include <map>
#include <memory>


namespace GPUCache {

class SubNode;
class SubNodeVisitor;


/*==============================================================================
 * CLASS SubNodeData
 *============================================================================*/

class SubNodeData
{
public:

    /*----- types and enumerations -----*/

    typedef std::shared_ptr<const SubNodeData> Ptr;


    /*----- member functions -----*/

    virtual void accept(SubNodeVisitor& visitor,
                        const SubNode&  subNode) const = 0;
    

    // This range represents the animation time range of
    // the entire sub-tree in seconds.
    const TimeInterval& animTimeRange() const
    { return fAnimTimeRange; }

    void setAnimTimeRange(const TimeInterval& animTimeRange)
    { fAnimTimeRange = animTimeRange; }

protected:

    /*----- member functions -----*/

    SubNodeData() : fAnimTimeRange(TimeInterval::kInvalid) {}
    virtual ~SubNodeData() = 0;

    /*----- data members -----*/
    TimeInterval fAnimTimeRange;
};


/*==============================================================================
 * CLASS ShapeData
 *============================================================================*/

class ShapeData: public SubNodeData
{
public:

    /*----- types and enumerations -----*/

    // Pointer to immutable shape data
    typedef std::shared_ptr<const ShapeData> Ptr;

    // Pointer to mutable shape data
    typedef std::shared_ptr<ShapeData> MPtr;

    typedef std::map<double, std::shared_ptr<const ShapeSample> > SampleMap;


    /*----- static member functions -----*/

    static MPtr create();

    /*----- member functions -----*/

    ~ShapeData() override;

    const std::shared_ptr<const ShapeSample>&
    getSample(double seconds) const;
    
    const std::shared_ptr<const ShapeSample>&
    getSample(const MTime& time) const
    {
        return getSample(time.as(MTime::kSeconds));
    }
    
    const SampleMap& getSamples() const { return fSamples; }
    
    void addSample(const std::shared_ptr<const ShapeSample>& sample);

    void accept(SubNodeVisitor& visitor,
                        const SubNode&  subNode) const override;

    // Set a single material to the whole shape
    void setMaterial(const MString& material);

    // Set per-group materials to the shape
    void setMaterials(const std::vector<MString>& materials);

    const std::vector<MString>& getMaterials() const;

protected:

    struct MakeSharedEnabler;


    /*----- member functions -----*/

    // The constructor is declare private to force user to go through
    // the create() factory member function.
    ShapeData();

    // Prohibited and not implemented.
    ShapeData(const ShapeData&);
    const ShapeData& operator=(const ShapeData&);


    /*----- data members -----*/

    SampleMap            fSamples;
    std::vector<MString> fMaterials;
};


/*==============================================================================
 * CLASS XformData
 *============================================================================*/

class XformData: public SubNodeData
{
public:
    /*----- types and enumerations -----*/

    // Pointer to immutable shape data
    typedef std::shared_ptr<const XformData> Ptr;

    // Pointer to mutable shape data
    typedef std::shared_ptr<XformData> MPtr;

    typedef std::map<double, std::shared_ptr<const XformSample> > SampleMap;


    /*----- static member functions -----*/

    static MPtr create();


    /*----- member functions -----*/

    ~XformData() override;

    const std::shared_ptr<const XformSample>&
    getSample(double seconds) const;
    
    const std::shared_ptr<const XformSample>&
    getSample(const MTime& time) const
    {
        return getSample(time.as(MTime::kSeconds));
    }

    const SampleMap& getSamples() const { return fSamples; }

    void addSample(const std::shared_ptr<const XformSample>& sample)
    {
        fSamples[sample->timeInSeconds()] = sample;
    }

    void accept(SubNodeVisitor& visitor,
                        const SubNode&  subNode) const override;

    
private:
    
    struct MakeSharedEnabler;

    /*----- member functions -----*/

    // The constructor is declare private to force user to go through
    // the create() factory member function.
    XformData() {}
    
    // Prohibited and not implemented.
    XformData(const XformData&);
    const XformData& operator=(const XformData&);

    
    /*----- data members -----*/

    SampleMap fSamples;
};


/*==============================================================================
 * CLASS SubNodeVisitor
 *============================================================================*/

// Visitor for sub nodes.
//
// The visitor dispatches on the sub node data type, i.e. transform vs
// shape. It is up to the visitor to recurse into the children of the
// sub node. This allows the visitor to control the traversal of the
// sub nodes. Note that this is somewhat different from the canonical
// visitor design pattern.
class SubNodeVisitor
{
public:
        
    virtual void visit(const XformData&   xform,
                       const SubNode&     subNode) = 0;

    virtual void visit(const ShapeData&   shape,
                       const SubNode&     subNode) = 0;

    virtual ~SubNodeVisitor();
};


/*==============================================================================
 * CLASS  SubNode
 *============================================================================*/

class SubNode
{
public:

    /*----- types and enumerations -----*/

    // Pointer to a mutable sub node
    typedef std::shared_ptr<SubNode>       MPtr;

    // Pointer to an immutable sub node
    typedef std::shared_ptr<const SubNode> Ptr;

    // Weak pointer to an immutable sub node
    typedef std::weak_ptr<const SubNode>   WPtr;

    enum TransparentType {
        kOpaque,
        kTransparent,
        kOpaqueAndTransparent,
        kUnknown
    };


    /*----- static member functions -----*/
    
    static MPtr create(const MString& name,
                       const SubNodeData::Ptr& nodeData);

    static void connect(const MPtr& parent, const MPtr& child)
    {
        parent->fChildren.push_back(child);
        child->fParents.push_back(parent);
    }

    static void swapNodeData(const MPtr& left, const MPtr& right)
    {
        assert(left);
        assert(right);
        left->fNodeData.swap(right->fNodeData);
        std::swap(left->fTransparentType, right->fTransparentType);
    }


    /*----- member functions -----*/
    
    ~SubNode();

    const MString& getName() const              { return fName; }
    const SubNodeData::Ptr& getData() const     { return fNodeData; }
    const std::vector<WPtr>& getParents() const { return fParents; }
    const std::vector<Ptr>& getChildren() const { return fChildren;}

    void setName(const MString& name)           { fName = name; }

    // Transparent includes children and descendants
    TransparentType transparentType() const { return fTransparentType; }
    void setTransparentType(const TransparentType transparentType)
    { fTransparentType = transparentType; }

    // Traverse the DAG using the visitor. Note that the traveral
    // ordering is under the control of the visitor.
    void accept(SubNodeVisitor& visitor) const
    { fNodeData->accept(visitor, *this); }
    

private:

    struct MakeSharedEnabler;

    /*----- member functions -----*/
    
    // The constructor is declare private to force user to go through
    // the create() factory member function.
    SubNode(
        const MString& name,
        const SubNodeData::Ptr& nodeData);

    
    /*----- data members -----*/
    
    MString fName;

    SubNodeData::Ptr   fNodeData;
    std::vector<WPtr>  fParents;
    std::vector<Ptr>   fChildren;
    TransparentType    fTransparentType;
};

}

#endif

