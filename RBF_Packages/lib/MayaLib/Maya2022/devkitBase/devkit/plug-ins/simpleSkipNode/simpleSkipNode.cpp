//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// This plug-in demonstrates how to use the MPxNode::skipEvaluation() method
// to mark attributes that are expensive to compute and might not be needed
// all the time.
//
// To run this example, execute the MEL code below.
//

/*
    MEL:
    loadPlugin simpleSkipNode;

    file -f -new;
    createNode simpleSkipNode;
    setKeyframe -attribute "input" -value 1 -t 1 simpleSkipNode1;
    setKeyframe -attribute "input" -value 4 -t 120 simpleSkipNode1;

    polySphere;
    connectAttr simpleSkipNode1.fastSquare pSphere1.translateX;
*/

// Playing back this example will NOT evaluate the slowSquare attribute.
// If you comment out the MPxNode::skipEvaluation() call in
// simpleSkipNode::preEvaluation(), then it will get evaluated, even
// thought it's not connected.
//
// If you run the following code to add an extra connection to the
// slowSquare attribute:
//

/*
    MEL:
    polySphere;
    connectAttr simpleSkipNode1.slowSquare pSphere2.translateY;
*/

// Then the attribute will still not be evaluated directly by the
// Evaluation Manager, but will be evaluated indirectly by pull evaluation.
//
// If you hide that second sphere:
//

/*
    MEL:
    setAttr pSphere2.visibility 0;
*/

// Then the slow attribute will not be pulled on.


#include <maya/MPxNode.h>

#include <maya/MFnNumericAttribute.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MEvaluationNode.h>

#include <maya/MFnPlugin.h>

#include <cassert>
#include <chrono>
#include <thread>

class simpleSkipNode : public MPxNode
{
public:
    MStatus compute( const MPlug& plug, MDataBlock& data ) override;

    MStatus preEvaluation( const MDGContext& context, const MEvaluationNode& evaluationNode ) override;

    static  void*   creator();
    static  MStatus initialize();

    static MTypeId id;
    static MObject input;         // The input value.
    static MObject fastSquare;    // The fast-to-compute attribute.
    static MObject slowSquare;    // The slow-to-compute attribute.
};

// Statics
MTypeId simpleSkipNode::id( 0x00081166 );
MObject simpleSkipNode::input;
MObject simpleSkipNode::fastSquare;
MObject simpleSkipNode::slowSquare;

// Class implementation
//
MStatus simpleSkipNode::compute( const MPlug& plug, MDataBlock& data )
{
    MStatus returnStatus;
    MDataHandle inputData = data.inputValue( input, &returnStatus );
    assert(returnStatus);

    const float inputValue = inputData.asFloat();
    const float outputValue = inputValue * inputValue;

    if ( plug == fastSquare )
    {
        MDataHandle outputHandle = data.outputValue( fastSquare );
        outputHandle.set( outputValue );
        data.setClean(plug);
    }
    else if ( plug == slowSquare )
    {
        // Sleep to mimic expensive computation.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        MDataHandle outputHandle = data.outputValue( slowSquare );
        outputHandle.set( outputValue );
        data.setClean(plug);
    }
    else
    {
        return MS::kUnknownParameter;
    }

    return MS::kSuccess;
}

MStatus simpleSkipNode::preEvaluation( const MDGContext& context, const MEvaluationNode& evaluationNode )
{
    MStatus returnStatus;
    // Let the attribute be skipped if there is a single downstream dependency,
    // because this is the only skippable attribute in this node.
    constexpr bool allowSingleDownstreamDependency = true;
    evaluationNode.skipEvaluation( slowSquare, allowSingleDownstreamDependency, &returnStatus );
    assert(returnStatus);

    return MS::kSuccess;
}

void* simpleSkipNode::creator()
{
	return new simpleSkipNode();
}

MStatus simpleSkipNode::initialize()
{
    MFnNumericAttribute nAttr;
    MStatus             status;

    input = nAttr.create( "input", "in", MFnNumericData::kFloat, 0.0 );
    nAttr.setStorable(true);

    fastSquare = nAttr.create( "fastSquare", "fs", MFnNumericData::kFloat, 0.0 );
    nAttr.setWritable(false);
    nAttr.setStorable(false);

    slowSquare = nAttr.create( "slowSquare", "ss", MFnNumericData::kFloat, 0.0 );
    nAttr.setWritable(false);
    nAttr.setStorable(false);

    status = addAttribute( input );
        if (!status) { status.perror("addAttribute"); return status;}
    status = addAttribute( fastSquare );
        if (!status) { status.perror("addAttribute"); return status;}
    status = addAttribute( slowSquare );
        if (!status) { status.perror("addAttribute"); return status;}

    status = attributeAffects( input, fastSquare );
        if (!status) { status.perror("attributeAffects"); return status;}
    status = attributeAffects( input, slowSquare );
        if (!status) { status.perror("attributeAffects"); return status;}

    return MS::kSuccess;
}

// Plug-in entry points
//
MStatus initializePlugin( MObject obj )
{
    MStatus   status;
    MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");

    status = plugin.registerNode(
        "simpleSkipNode",
        simpleSkipNode::id,
        simpleSkipNode::creator,
        simpleSkipNode::initialize
        );
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus   status;
    MFnPlugin plugin( obj );

    status = plugin.deregisterNode( simpleSkipNode::id );
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}
