#ifndef _gpuCacheMaterialBakers_h_
#define _gpuCacheMaterialBakers_h_

//-
//**************************************************************************/
// Copyright 2012 Autodesk, Inc. All rights reserved. 
//
// Use of this software is subject to the terms of the Autodesk 
// license agreement provided at the time of installation or download, 
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
//**************************************************************************/
//+

#include "gpuCacheMaterial.h"

#include <memory>
#include <unordered_map>

// Forward Declaration


namespace GPUCache {

/*==============================================================================
 * CLASS MaterialBaker
 *============================================================================*/

// This class bakes a shading network that has a surface material as its root.
class MaterialBaker
{
public:
    // Constructor and Destructor
    MaterialBaker();
    virtual ~MaterialBaker();
	MaterialBaker(const MaterialBaker&) = delete;
	MaterialBaker& operator=(const MaterialBaker&) = delete;

    // Add a surface shape to this material baker.
    // All connected surface materials are going to be baked.
    MStatus addShapePath(const MDagPath& dagPath);

    // Sample the material graph at the given time.
    MStatus sample(const MTime& time);

    // Connect the baked shading graph.
    MStatus buildGraph();

    // Get the materials.
    MaterialGraphMap::Ptr get();

private:
    class MaterialGraphBaker;
    typedef std::shared_ptr<MaterialGraphBaker> MaterialGraphBakerPtr;

    // Bakers for each root surface material.
    typedef std::unordered_map<MString,MaterialGraphBakerPtr,MStringHash> MaterialGraphBakers;
    MaterialGraphBakers fMaterialGraphBakers;

    // Existing materials for recursive baking.
    typedef std::unordered_map<MString,MaterialGraph::Ptr,MStringHash> NamedMaterialGraphs;
    NamedMaterialGraphs fExistingGraphs;
};

} // namespace GPUCache

#endif

