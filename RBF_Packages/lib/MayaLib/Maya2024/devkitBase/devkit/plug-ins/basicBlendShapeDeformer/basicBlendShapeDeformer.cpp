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

//
//  File: basicBlendShapeDeformer.cpp
//
//  Description:
//      Rudimentary implementation of a blendshape.
//
//      Use this script to create a simple example.
/*      
loadPlugin basicBlendShapeDeformer;
torus -p 0 0 0 -ax 0 1 0 -ssw 0 -esw 360 -msw 360 -r 1 -hr 0.5 -d 3 -ut 0 -tol 0.01 -s 8 -nsp 4 -ch 1 -n bendy;
torus -p 0 0 0 -ax 0 1 0 -ssw 0 -esw 360 -msw 360 -r 1 -hr 0.5 -d 3 -ut 0 -tol 0.01 -s 8 -nsp 4 -ch 1 -n blendTarget;
scale -r 0.5 1 1;
move 2 0 0;
makeIdentity -apply true -t 1 -r 1 -s 1 -n 0 -pn 1;
select -r bendy;
deformer -type "basicBlendShapeDeformer";
blendShape -edit -t bendy 0 blendTarget 1.0 basicBlendShapeDeformer1;
setAttr "basicBlendShapeDeformer1.blendTarget" 0.5;

*/


#include <maya/MFnPlugin.h>
#include <maya/MTypeId.h> 

#include <maya/MMatrixArray.h>
#include <maya/MStringArray.h>
#include <maya/MFloatArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>

#include <maya/MPxBlendShape.h> 
#include <maya/MItGeometry.h>
#include <maya/MFnPointArrayData.h>
#include <maya/MFnComponentListData.h>

#include <map>

class basicBlendShapeDeformer : public MPxBlendShape
{
public:
    static  void*   creator();
    static  MStatus initialize();

    // Deformation function
    //
    MStatus deform(MDataBlock& block,
                   MItGeometry& iterator,
                   const MMatrix& matrix,
                   unsigned int multiIndex) override;

    static const MTypeId id;
};

const MTypeId basicBlendShapeDeformer::id( 0x00080042 );


void* basicBlendShapeDeformer::creator()
{
    return new basicBlendShapeDeformer();
}

MStatus basicBlendShapeDeformer::initialize()
{
    return MStatus::kSuccess;
}

MStatus
basicBlendShapeDeformer::deform(MDataBlock& block,
                                MItGeometry& iterator,
                                const MMatrix& matrix,
                                unsigned int multiIndex)
{
    MStatus returnStatus;

   // get the input geometry
    MPlug inPlug(thisMObject(),input);
    inPlug.selectAncestorLogicalIndex(multiIndex,input);
    MDataHandle inputDataHandle = block.inputValue(inPlug);
    MDataHandle inputGeomDataHandle = inputDataHandle.child(inputGeom);

    // get the weights for each target
    //
    MArrayDataHandle weightMH = block.inputArrayValue( weight );
    unsigned int numTargets = weightMH.elementCount();
    MFloatArray targetEnvelope;
    for (unsigned int w=0; w < numTargets; ++w) {
        targetEnvelope.append( weightMH.inputValue().asFloat() );
        weightMH.next();
    }

    // get the input targets
    // as a point array per weight
    //
    MArrayDataHandle inputTargetMH = block.inputArrayValue( inputTarget );
    returnStatus = inputTargetMH.jumpToElement( multiIndex );
    if ( !returnStatus ) {
        return returnStatus;
    }

    MDataHandle inputTargetDataHandle = inputTargetMH.inputValue();
    MArrayDataHandle inputTargetGroupDataHandle = inputTargetDataHandle.child( inputTargetGroup );
    for (unsigned int w=0; w < numTargets; ++w) {
        // inputPointsTarget is computed on pull,
        // so can't just read it out of the datablock
        MPlug plug( thisMObject(), inputPointsTarget );
        plug.selectAncestorLogicalIndex( multiIndex, inputTarget );
        plug.selectAncestorLogicalIndex( w, inputTargetGroup );
        // ignore deformer chains here and just take the first one
        plug.selectAncestorLogicalIndex( 6000, inputTargetItem );
        MObject pointArray = plug.asMObject();
        MPointArray deltaPts = MFnPointArrayData( pointArray ).array();

        // get the target component list
        plug = plug.parent();
        plug = plug.child( inputComponentsTarget );
        MFnComponentListData compList( plug.asMObject() );
        if ( compList.length() == 0 ) {
            continue;
        }
        MObject targetComp = compList[0];

        // Get the components for the target
        float vertWeight = targetEnvelope[w];
        inputTargetGroupDataHandle.jumpToArrayElement( w );
        MArrayDataHandle targetVertWeightArray = inputTargetGroupDataHandle.inputValue().child( targetWeights );

        // Find out how the target components map to the deformed geometry
        unsigned int ptIndex = 0;
        std::map<unsigned int, unsigned int> componentToPointMap;
        MItGeometry iter( inputGeomDataHandle, targetComp);
        for ( ; !iter.isDone(); iter.next(), ++ptIndex )
            componentToPointMap[iter.index()] = ptIndex;

        // Loop over the verts of the geometry being deformed
        for ( ; !iterator.isDone(); iterator.next() ) {
            unsigned int compIndex = iterator.index();

            auto r = componentToPointMap.find(compIndex);
            if (r == componentToPointMap.end())
                continue; // we are not deforming this point

            ptIndex = r->second;
            float wgt = vertWeight;
            if ( targetVertWeightArray.jumpToElement( compIndex ) ) {
                wgt *= targetVertWeightArray.inputValue().asFloat();
            }

            MPoint pt = iterator.position();
            pt += deltaPts[ptIndex] * wgt;
            iterator.setPosition( pt );
        }
    }
    return returnStatus;
}

// standard initialization procedures
//

MStatus initializePlugin( MObject obj )
{
    MStatus result;

    MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");
    result = plugin.registerNode(
        "basicBlendShapeDeformer" ,
        basicBlendShapeDeformer::id ,
        &basicBlendShapeDeformer::creator ,
        &basicBlendShapeDeformer::initialize ,
        MPxNode::kBlendShape
        );

    return result;
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus result;

    MFnPlugin plugin( obj );
    result = plugin.deregisterNode( basicBlendShapeDeformer::id );

    return result;
}
