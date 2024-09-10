//-
// ==========================================================================
// Copyright 2017 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

// Example Plugin: captureViewRenderCmd.cpp
//
//
// This is a plugin to capture a range of frames from modelPanel4 and show a
// specified frame. Output color transform can be enabled/disabled when capturing.
//
// loadPlugin captureViewRenderCmd;
//
// captureViewRender -captureFrame 1 10 true;
// captureViewRender -showFrame 6;

// captureViewRender -captureFrame 1 10 false;
// captureViewRender -showFrame 6;
//

#include <maya/MApiNamespace.h>

#include <maya/MArgDatabase.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>
#include <maya/MSyntax.h>

#include <maya/MAnimControl.h>
#include <maya/MTime.h>

#include <maya/MRenderTargetManager.h>
#include <maya/MViewport2Renderer.h>

#include <vector>

#define kCaptureFrame "-cf"
#define kCaptureFrameLong "-captureFrame"

#define kShowFrame "-sf"
#define kShowFrameLong "-showFrame"

class MayaViewportRenderData
{
public:
    MayaViewportRenderData(const MTime& renderTime, bool outputColorTransform)
    : mRenderTime(renderTime)
    , mOutputColorTransform(outputColorTransform)
    {
        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        if (theRenderer)
        {
            const MHWRender::MRenderTargetManager* targetManager =
                theRenderer->getRenderTargetManager();
            if (!targetManager)
                return;
            mTarget = targetManager->acquireRenderTargetFromScreen(
                MString("CapturedTarget") + mRenderTime.value());
        }
        mIsDone = false;
    }

    ~MayaViewportRenderData()
    {
        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        if (theRenderer)
        {
            const MHWRender::MRenderTargetManager* targetManager =
                theRenderer->getRenderTargetManager();
            if (targetManager)
            {
                targetManager->releaseRenderTarget(mTarget);
            }
        }
    }

    static void doRenderTask(void* clientData)
    {
        MayaViewportRenderData* renderData =
            static_cast<MayaViewportRenderData*>(clientData);
        if (!renderData)
            return;

        MHWRender::MRenderer* theRenderer = MHWRender::MRenderer::theRenderer();
        if (!theRenderer)
        {
            return;
        }
        MHWRender::MRenderTarget*& target = renderData->mTarget;
        if (!target)
        {
            return;
        }

        MAnimControl::setCurrentTime(renderData->mRenderTime);

        // Turn on/off output color transform for renderer and playblast as required
        static const MString cmQR("colorManagementPrefs -q -outputTarget renderer -outputTransformEnabled");
        static const MString cmER("colorManagementPrefs -e -outputTarget renderer -inhibitEvents -outputTransformEnabled ");
        static const MString cmQP("colorManagementPrefs -q -outputTarget playblast -outputTransformEnabled");
        static const MString cmEP("colorManagementPrefs -e -outputTarget playblast -inhibitEvents -outputTransformEnabled ");

        int result = 0;
        MGlobal::executeCommand(cmQR, result);
        bool outputColorTransformR = (result != 0);
        if (outputColorTransformR != renderData->mOutputColorTransform)
        {
            MString cmd = cmER + (renderData->mOutputColorTransform ? 1 : 0);
            MGlobal::executeCommand(cmd);
        }

        MGlobal::executeCommand(cmQP, result);
        bool outputColorTransformP = (result != 0);
        if (outputColorTransformP != renderData->mOutputColorTransform)
        {
            MString cmd = cmEP + (renderData->mOutputColorTransform ? 1 : 0);
            MGlobal::executeCommand(cmd);
        }

        theRenderer->render("modelPanel4", &target, 1);

        // Restore output color transform for renderer and playblast
        if (outputColorTransformR != renderData->mOutputColorTransform)
        {
            MString cmd = cmER + (outputColorTransformR ? 1 : 0);
            MGlobal::executeCommand(cmd);
        }

        if (outputColorTransformP != renderData->mOutputColorTransform)
        {
            MString cmd = cmEP + (outputColorTransformP ? 1 : 0);
            MGlobal::executeCommand(cmd);
        }

        renderData->mIsDone = true;
    }

    const MTime& renderTime() const
    {
        return mRenderTime;
    }

    const MHWRender::MRenderTarget* renderTarget() const
    {
        return mTarget;
    }

    bool isDone() const
    {
        return mIsDone;
    }

private:
    MHWRender::MRenderTarget* mTarget;
    MTime mRenderTime;
    bool mOutputColorTransform;
    bool mIsDone;
};

// global variables
std::vector<MayaViewportRenderData*> gRenderData;

class captureViewRenderCmd : public MPxCommand
{
public:
    captureViewRenderCmd()
    {
    }
    virtual ~captureViewRenderCmd()
    {
    }

    static void* creator()
    {
        return new captureViewRenderCmd;
    }

    static MSyntax newSyntax()
    {
        MSyntax syntax;

        syntax.addFlag(kCaptureFrame, kCaptureFrameLong,
                       MSyntax::kTime /*startTime*/,
                       MSyntax::kTime /*endTime*/,
                       MSyntax::kBoolean /*outputColorTransform*/);
        syntax.addFlag(kShowFrame, kShowFrameLong, MSyntax::kTime);

        syntax.enableQuery(false);
        syntax.enableEdit(false);

        return syntax;
    }

    virtual MStatus doIt(const MArgList&);
};

MStatus captureViewRenderCmd::doIt(const MArgList& args)
{

    MStatus status = MStatus::kFailure;

    MArgDatabase argData(syntax(), args, &status);
    if (!status)
        return status;

    if (argData.isFlagSet(kCaptureFrame))
    {
        // Remove all previous captured frames
        for (unsigned int i = 0; i < gRenderData.size(); i++)
        {
            delete gRenderData[i];
        }
        gRenderData.clear();

        MTime startTime;
        MTime endTime;
        bool outputColorTransform = false;
        argData.getFlagArgument(kCaptureFrame, 0, startTime);
        argData.getFlagArgument(kCaptureFrame, 1, endTime);
        argData.getFlagArgument(kCaptureFrame, 2, outputColorTransform);

        for (MTime currentTime = startTime; currentTime <= endTime;
             currentTime++)
        {
            // schedule idle render tasks
            MayaViewportRenderData* newRenderTask =
                new MayaViewportRenderData(currentTime, outputColorTransform);
            gRenderData.push_back(newRenderTask);

            MGlobal::executeTaskOnIdle(MayaViewportRenderData::doRenderTask,
                                       newRenderTask,
                                       MGlobal::kHighIdlePriority);
        }
    }
    else if (argData.isFlagSet(kShowFrame))
    {
        MTime time;
        argData.getFlagArgument(kShowFrame, 0, time);

        // Find the frame in the render data list
        unsigned int frameToShow = gRenderData.size();
        for (unsigned int i = 0; i < gRenderData.size(); i++)
        {
            if (time == gRenderData[i]->renderTime())
            {
                frameToShow = i;
                break;
            }
        }

        if (frameToShow == gRenderData.size())
        {
            MGlobal::displayError("No captured frame found!");
            return MS::kFailure;
        }
        if (!gRenderData[frameToShow]->isDone())
        {
            MGlobal::displayError("The frame is not captured yet!");
            return MS::kFailure;
        }

        // Copy the captured target to screen.
        MRenderer::theRenderer()->copyTargetToScreen(
            gRenderData[frameToShow]->renderTarget());
    }

    return MS::kSuccess;
}

MStatus initializePlugin(MObject _obj)
{
    MFnPlugin plugin(_obj, PLUGIN_COMPANY, "1.0");
    MStatus stat = plugin.registerCommand("captureViewRender",
                                          captureViewRenderCmd::creator,
                                          captureViewRenderCmd::newSyntax);
    if (!stat)
        stat.perror("registerCommand");
    return stat;
}

MStatus uninitializePlugin(MObject _obj)
{
    MFnPlugin plugin(_obj);
    MStatus stat = plugin.deregisterCommand("captureViewRender");
    if (!stat)
        stat.perror("deregisterCommand");

    // Remove all captured frames' render data
    for (unsigned int i = 0; i < gRenderData.size(); i++)
    {
        delete gRenderData[i];
    }
    gRenderData.clear();

    return stat;
}
