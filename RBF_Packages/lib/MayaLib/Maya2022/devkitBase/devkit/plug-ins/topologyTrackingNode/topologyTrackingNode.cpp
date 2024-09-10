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

////////////////////////////////////////////////////////////////////////
//
// DESCRIPTION:
//
// Produces the dependency graph node "topologyTrackingNode".
//
// This plug-in demonstrates the use of the new version of the attributeAffects()
// relationship which can take a parameter specifying whether or not the
// relationship affects topology.
//
// It creates a simple node taking a mesh (inMesh) as an input parameter.  It then
// creates two copies (outMeshLeft and outMeshRight) of the input mesh.
// Those new copies will be subdivided based on the divisionCount attribute
// (which therefore affects topology) and will be translated by the offset attribute
// (which does not affect topology).
//
// Use this script to create a simple example with the topologyTrackingNode node:
//
//  loadPlugin topologyTrackingNode;
//  polySphere;
//  createNode topologyTrackingNode;
//  createNode mesh -name outMeshLeft;
//  createNode mesh -name outMeshRight;
//  connectAttr pSphereShape1.outMesh topologyTrackingNode1.inMesh;
//  connectAttr topologyTrackingNode1.outMeshLeft outMeshLeft.inMesh;
//  connectAttr topologyTrackingNode1.outMeshRight outMeshRight.inMesh;
//  setAttr topologyTrackingNode1.offset 1 2 3;
//
// You can then animate the "offset" attribute (which does not affect topology)
// or the "divisionCount" attribute (which affects topology).
//
// Then, using the profiler to record events during playback, you can see
// Vp2TranslateGeometry events happening during the evaluation phase of the
// frame in the case topology is not affected, and during the rendering phase
// of the frame in the case of affected topology.  Being able to update the
// geometry during evaluation allows for more parallelism and earlier
// scheduling of translation tasks, which usually results in increased
// performance.
//
////////////////////////////////////////////////////////////////////////

#include <maya/MPxNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnGenericAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFloatPointArray.h>

#include <maya/MFnPlugin.h>

#include <algorithm>


// This definition enables the use of the new API.
#define USE_TOPOLOGY_TRACKING_OPTIMIZATION    1


class topologyTrackingNode : public MPxNode
{
public:
    static const MString name;
    static const MTypeId id;

    static void*         creator();
    static MStatus       initialize();

    MStatus compute(const MPlug &plug, MDataBlock &data) override;

#if USE_TOPOLOGY_TRACKING_OPTIMIZATION
    bool isTrackingTopology() const override;
#endif // USE_TOPOLOGY_TRACKING_OPTIMIZATION

private:
    typedef MPxNode ParentClass;
    static MObject divisionCount;
    static MObject offset;
    static MObject inMesh;
    static MObject outMeshLeft;
    static MObject outMeshRight;
};




const MString topologyTrackingNode::name{"topologyTrackingNode"};
const MTypeId topologyTrackingNode::id{0x00081150};

MObject topologyTrackingNode::divisionCount;
MObject topologyTrackingNode::offset;
MObject topologyTrackingNode::inMesh;
MObject topologyTrackingNode::outMeshLeft;
MObject topologyTrackingNode::outMeshRight;


void* topologyTrackingNode::creator()
{
    return new topologyTrackingNode();
}


MStatus topologyTrackingNode::initialize()
{
    MStatus status = MStatus::kSuccess;

    MFnGenericAttribute gAttrFn;
    MFnNumericAttribute nAttrFn;

    divisionCount = nAttrFn.create("divisionCount", "dc", MFnNumericData::kInt, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(nAttrFn.setMin(0));
    CHECK_MSTATUS(addAttribute(divisionCount));

    offset = nAttrFn.create("offset", "ot", MFnNumericData::k3Float, 0, &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(addAttribute(offset));

    inMesh = gAttrFn.create("inMesh", "im", &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(gAttrFn.addDataAccept(MFnData::kMesh));
    CHECK_MSTATUS(addAttribute(inMesh));

    outMeshLeft = gAttrFn.create("outMeshLeft", "oml", &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(gAttrFn.addDataAccept(MFnData::kMesh));
    CHECK_MSTATUS(addAttribute(outMeshLeft));

    outMeshRight = gAttrFn.create("outMeshRight", "omr", &status);
    CHECK_MSTATUS(status);
    CHECK_MSTATUS(gAttrFn.addDataAccept(MFnData::kMesh));
    CHECK_MSTATUS(addAttribute(outMeshRight));

    CHECK_MSTATUS(attributeAffects(divisionCount, outMeshLeft));
    CHECK_MSTATUS(attributeAffects(divisionCount, outMeshRight));
#if USE_TOPOLOGY_TRACKING_OPTIMIZATION
    CHECK_MSTATUS(attributeAffects(offset, outMeshLeft, false));
    CHECK_MSTATUS(attributeAffects(offset, outMeshRight, false));
#else
    CHECK_MSTATUS(attributeAffects(offset, outMeshLeft));
    CHECK_MSTATUS(attributeAffects(offset, outMeshRight));
#endif // USE_TOPOLOGY_TRACKING_OPTIMIZATION
    CHECK_MSTATUS(attributeAffects(inMesh, outMeshLeft));
    CHECK_MSTATUS(attributeAffects(inMesh, outMeshRight));

    return status;
}


MStatus topologyTrackingNode::compute(const MPlug &plug, MDataBlock &block)
{
    auto process = [this, &block, &plug](float offsetFactor) {
        MStatus status;

        MObject thisNode = thisMObject();

        // Get the input mesh.
        MPlug inPlug(thisNode, inMesh);
        MDataHandle inputDataHandle = block.inputValue(inPlug, &status);
        CHECK_MSTATUS(status);

        MFloatVector offsetVector(block.inputValue(offset).asFloat3());
        offsetVector *= offsetFactor;

        MDataHandle outputDataHandle = block.outputValue(plug);
        outputDataHandle.copyWritable(inputDataHandle);
        MObject outMeshObject = outputDataHandle.asMesh();

        MFnMesh outMesh;
        outMesh.setObject(outMeshObject);

        // Apply offset.
        for (int i = 0, outPointsCount = outMesh.numVertices(); i < outPointsCount; ++i)
        {
            MPoint point;
            CHECK_MSTATUS(outMesh.getPoint(i, point));
            point += offsetVector;
            CHECK_MSTATUS(outMesh.setPoint(i, point));
        }

        // Subdivide.
        MPlug divisionCountPlug(thisNode, divisionCount);
        MDataHandle divisionCountDataHandle = block.inputValue(divisionCountPlug, &status);
        CHECK_MSTATUS(status);
        const int divisionCount = divisionCountDataHandle.asInt();

        if (divisionCount > 0)
        {
            const int numPolygons = outMesh.numPolygons();
            MIntArray facesToSubdivide(numPolygons);
            for (int i = 0; i < numPolygons; ++i)
            {
                facesToSubdivide[i] = i;
            }
            outMesh.subdivideFaces(facesToSubdivide, divisionCount);
        }

        outputDataHandle.set(outMeshObject);
    };
    if (plug == outMeshLeft)
    {
        process(1.5f);
        return MStatus::kSuccess;
    }
    else if (plug == outMeshRight)
    {
        process(-1.5f);
        return MStatus::kSuccess;
    }

    return ParentClass::compute(plug, block);
}


#if USE_TOPOLOGY_TRACKING_OPTIMIZATION
bool topologyTrackingNode::isTrackingTopology() const
{
    return true;
}
#endif // USE_TOPOLOGY_TRACKING_OPTIMIZATION




MStatus initializePlugin(MObject obj)
{
    MFnPlugin pluginFn(obj, PLUGIN_COMPANY, "1.0", "Any");

    // Register the node type.
    MStatus status = pluginFn.registerNode(
        topologyTrackingNode::name,
        topologyTrackingNode::id,
        topologyTrackingNode::creator,
        topologyTrackingNode::initialize
    );
    CHECK_MSTATUS(status);

    return status;
}


MStatus uninitializePlugin(MObject obj)
{
    MFnPlugin pluginFn(obj);

    // Deregister the node type.
    MStatus status = pluginFn.deregisterNode(topologyTrackingNode::id);

    return status;
}
