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

// This plug-in demonstrates how to use the MPxNode::getCacheSetup() method
// to interact with Cached Playback.
//
// It implements a very simple simulation and tells the Cached Playback
// system about it.  That way, when filling the cache in the background,
// the filling direction will be adjusted accordingly to make sure simulation
// is evaluated on consecutive frames.
//
// To run this example, execute the MEL code below.
//

/*
    MEL:
    loadPlugin simpleSimulationNode;

    file -f -new;
    polySphere;
    setAttr pSphere1.translateZ 5;

    polyCube;
    setKeyframe -time   1 -value   0 pCube1.translateX ;
    setKeyframe -time  20 -value  10 pCube1.translateX ;
    setKeyframe -time  40 -value   0 pCube1.translateX ;
    setKeyframe -time  60 -value -10 pCube1.translateX ;
    setKeyframe -time  80 -value   0 pCube1.translateX ;
    setKeyframe -time 100 -value  10 pCube1.translateX ;
    setKeyframe -time 120 -value   0 pCube1.translateX ;

    createNode simpleSimulationNode;
    setAttr simpleSimulationNode1.dampingFactor 0.1;
    connectAttr time1.outTime simpleSimulationNode1.currentTime;
    setAttr simpleSimulationNode1.simulationStartTime 20;

    connectAttr pCube1.translateX simpleSimulationNode1.target;
    connectAttr simpleSimulationNode1.output pSphere1.translateX ;
*/

#include <string.h>
#include <maya/MIOStream.h>

#include <maya/MPxNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFnPlugin.h>
#include <maya/MNodeCacheDisablingInfoHelper.h>

#include <cassert>
#include <limits>

// This class implements a simple simulation that soften the movement
// of what is driving it.
//
// It basically computes the following simulation:
//
// output         = previousOutput + (target - previousOutput) * dampingFactor
// previousOutput = output
//
// Basically, it tries to reach the target by bridging the distance between the
// previous output and the current target.
class simpleSimulationNode : public MPxNode
{
public:
    MStatus     compute(const MPlug& plug, MDataBlock& data) override;
    void        getCacheSetup(const MEvaluationNode& evaluationNode,
                              MNodeCacheDisablingInfo& disablingInfo,
                              MNodeCacheSetupInfo& setupInfo,
                              MObjectArray& monitoredAttributes) const override;
    MTimeRange  transformInvalidationRange(const MPlug& source, const MTimeRange& input) const override;

    static  void*       creator();
    static  MStatus     initialize();

public:
    // The target value this node tries to reach (input attribute).
    static  MObject     aTarget;
    // The amount of the different between the previous output and
    // current target to be bridged in an evaluation (input attribute).
    static  MObject     aDampingFactor;
    // The current time value used to control the simulation (input attribute).
    // This attribute should be connected to the time node.
    static  MObject     aCurrentTime;
    // Whether or not the simulation is enabled.
    // Disabling this attribute makes this node a passthrough where output = target (input attribute)
    // This attribute should NOT be animated.
    static  MObject     aSimulationEnabled;
    // The time at which the simulation starts (intput attribute).
    // This attribute should NOT be animated.
    static  MObject     aSimulationStartTime;
    // The output of this node, i.e. where the simulation is when
    // trying to reach the target (output attribute).
    static  MObject     aOutput;
    // The previous state, i.e. basically the simulation state
    // (internal attribute, only used internally).
    static  MObject     aPreviousOutput;

    static  MTypeId     id;
};

// Statics
MObject     simpleSimulationNode::aTarget;
MObject     simpleSimulationNode::aDampingFactor;
MObject     simpleSimulationNode::aCurrentTime;
MObject     simpleSimulationNode::aSimulationEnabled;
MObject     simpleSimulationNode::aSimulationStartTime;
MObject     simpleSimulationNode::aOutput;
MObject     simpleSimulationNode::aPreviousOutput;
MTypeId     simpleSimulationNode::id{ 0x00081160 };

// Class implementation
//
MStatus simpleSimulationNode::compute(const MPlug& plug, MDataBlock& data)
{
    MStatus returnStatus;

    if (plug == aOutput)
    {
        assert(!aTarget.isNull());
        const float targetValue = data.inputValue(aTarget, &returnStatus).asFloat();
        assert(returnStatus);

        assert(!aSimulationEnabled.isNull());
        bool simulationEnabledValue = data.inputValue(aSimulationEnabled, &returnStatus).asBool();
        assert(returnStatus);

        if (simulationEnabledValue)
        {
            assert(!aCurrentTime.isNull());
            const MTime currentTime = data.inputValue(aCurrentTime, &returnStatus).asTime();
            assert(returnStatus);

            assert(!aSimulationStartTime.isNull());
            const MTime simulationStartTime = data.inputValue(aSimulationStartTime, &returnStatus).asTime();
            assert(returnStatus);

            // Apply the simulation if we are past the start time; otherwise just take the target directly.
            simulationEnabledValue = (simulationStartTime < currentTime);
        }

        float outputValue = targetValue;
        if (simulationEnabledValue)
        {
            assert(!aDampingFactor.isNull());
            const float dampingFactorValue = data.inputValue(aDampingFactor, &returnStatus).asFloat();
            assert(returnStatus);

            assert(!aPreviousOutput.isNull());
            const float previousOutputValue = data.inputValue(aPreviousOutput, &returnStatus).asFloat();
            assert(returnStatus);

            outputValue = previousOutputValue + (targetValue - previousOutputValue) * dampingFactorValue;
        }

        MDataHandle outputHandle = data.outputValue(aOutput);
        outputHandle.set(outputValue);
        data.setClean(plug);

        MDataHandle previousInputHandle = data.outputValue(aPreviousOutput);
        previousInputHandle.set(outputValue);
        data.setClean(aPreviousOutput);
    }
    else
    {
        return MS::kUnknownParameter;
    }

    return MS::kSuccess;
}

void simpleSimulationNode::getCacheSetup(
    const MEvaluationNode& evaluationNode,
    MNodeCacheDisablingInfo& disablingInfo,
    MNodeCacheSetupInfo& setupInfo,
    MObjectArray& monitoredAttributes) const
{
    // Check if the attribute is animated.  If it is animated, requestSimulation
    // will be true so that the behavior is the same as if the scene was played back from
    // the beginning of the time range.  That being said, it makes very little sense
    // to animate whether the simulation is active or not.
    const bool requestSimulation = MNodeCacheDisablingInfoHelper::testBooleanAttribute(
        nullptr, monitoredAttributes, evaluationNode, aSimulationEnabled, false
    );

    if (requestSimulation)
    {
        // Request simulation support
        setupInfo.setRequirement(MNodeCacheSetupInfo::kSimulationSupport, true);
    }
}

MTimeRange simpleSimulationNode::transformInvalidationRange(const MPlug& source, const MTimeRange& input) const
{
    // Never call the parent class method, it's not meant to be called from the base class,
    // the implementation is only used to detect whether there is an override or not.

    // When setting infinite values, do not use max()/min() directly, could overflow easily.
    static constexpr MTime::MTick kMaximumTimeTick = std::numeric_limits<MTime::MTick>::max()/2;
    static constexpr MTime::MTick kMinimumTimeTick = std::numeric_limits<MTime::MTick>::min()/2+1;
    static const MTime kMaximumTime{ kMaximumTimeTick / static_cast<double>(MTime::ticksPerSecond()), MTime::kSeconds };
    static const MTime kMinimumTime{ kMinimumTimeTick / static_cast<double>(MTime::ticksPerSecond()), MTime::kSeconds };

    // Get the start time and whether simulation is enabled., but it should NOT be animated, so it should be clean.
    MDataBlock data = const_cast<simpleSimulationNode*>(this)->forceCache();
    if (!data.isClean(aSimulationStartTime) || !data.isClean(aSimulationEnabled))
    {
        return MTimeRange{ kMinimumTime, kMaximumTime };
    }

    MStatus returnStatus;
    // Whether the simulation is enabled or not is clean, get its value.
    MDataHandle simulationEnabledData = data.inputValue(aSimulationEnabled, &returnStatus);
    const bool simulationEnabled = returnStatus ? simulationEnabledData.asBool() : true;
    if (!simulationEnabled)
    {
        return input;
    }

    // The start time is clean, get its value.
    MDataHandle simulationStartTimeData = data.inputValue(aSimulationStartTime, &returnStatus);
    const MTime simulationStartTime = returnStatus ? simulationStartTimeData.asTime() : kMinimumTime;
    const MTime simulationEndTime = kMaximumTime;

    if (input.intersects(simulationStartTime, simulationEndTime))
    {
        // We invalidate the whole simulation time range, because something in the range was invalidated,
        // therefore we have to recompute the whole simulation.
        return input | MTimeRange{ simulationStartTime, simulationEndTime };
    }
    else
    {
        // Since the invalidation range does not touch the simulation,
        // We don't need to invalidate anything.
        return MTimeRange{};
    }
}

void* simpleSimulationNode::creator()
{
    return new simpleSimulationNode();
}

MStatus simpleSimulationNode::initialize()
{
    MFnNumericAttribute nAttr;
    MFnUnitAttribute    uAttr;
    MStatus             status;

    aTarget = nAttr.create("target", "tg", MFnNumericData::kFloat, 0.0);
    nAttr.setStorable(true);

    aDampingFactor = nAttr.create("dampingFactor", "df", MFnNumericData::kFloat, 0.1);
    nAttr.setStorable(true);

    aCurrentTime = uAttr.create("currentTime", "ct", MFnUnitAttribute::kTime, 0.0);
    uAttr.setWritable(true);
    uAttr.setStorable(true);
    uAttr.setReadable(true);
    uAttr.setKeyable(true);

    aSimulationEnabled = nAttr.create("simulationEnabled", "se", MFnNumericData::kBoolean, 1.0);
    nAttr.setStorable(true);

    aSimulationStartTime = uAttr.create("simulationStartTime", "sst", MFnUnitAttribute::kTime, 0.0);
    uAttr.setStorable(true);

    aOutput = nAttr.create("output", "out", MFnNumericData::kFloat, 0.0);
    nAttr.setWritable(false);
    nAttr.setStorable(false);

    aPreviousOutput = nAttr.create("previousOutput", "pi", MFnNumericData::kFloat, 0.0);
    nAttr.setWritable(false);
    nAttr.setStorable(false);
    nAttr.setHidden(true);

    status = addAttribute(aTarget);
    assert(status);
    status = addAttribute(aDampingFactor);
    assert(status);
    status = addAttribute(aCurrentTime);
    assert(status);
    status = addAttribute(aSimulationEnabled);
    assert(status);
    status = addAttribute(aSimulationStartTime);
    assert(status);
    status = addAttribute(aOutput);
    assert(status);
    status = addAttribute(aPreviousOutput);
    assert(status);

    status = attributeAffects(aTarget, aOutput);
    assert(status);
    status = attributeAffects(aDampingFactor, aOutput);
    assert(status);
    status = attributeAffects(aCurrentTime, aOutput);
    assert(status);
    status = attributeAffects(aSimulationEnabled, aOutput);
    assert(status);
    status = attributeAffects(aSimulationStartTime, aOutput);
    assert(status);

    return MS::kSuccess;
}

// Plug-in entry points
//
MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

    status = plugin.registerNode(
        "simpleSimulationNode",
        simpleSimulationNode::id,
        simpleSimulationNode::creator,
        simpleSimulationNode::initialize
    );
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj);

    status = plugin.deregisterNode(simpleSimulationNode::id);
    if (!status) {
        status.perror("deregisterNode");
        return status;
    }

    return status;
}
