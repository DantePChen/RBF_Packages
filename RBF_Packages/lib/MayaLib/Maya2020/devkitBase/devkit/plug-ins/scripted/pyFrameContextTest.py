import sys
import maya.api.OpenMayaRender as omr
import maya.api.OpenMaya as om
import collections # for OrderedDict

# Description:
# This plug-in shows the usage of MRenderOverride.getFrameContext() to be able
# to extract various per-frame data. This is performed within the MRenderOverride.setup() method.
# 
# Sample out from a 3d viewport will look like this:
# 
# MRenderOverride Frame Information Extraction Trace
# > --------------------------------------------------
# > Destination: modelPanel4
# > Viewport: 0,0,885,541
# > Camera path: |persp|perspShape
# > Projection matrix:(((1.94444, 0, 0, 0), (0, 3.18084, 0, 0), (0, 0, -1.00001, -1), (0, 0, -0.100001, 0)))
# > View matrix:(((-0.656059, 0.459493, -0.598709, 0), (-0, 0.793297, 0.608834, 0), (0.75471, 0.399431, -0.52045, -0), (2.87398e-012, 1.02103e-012, -169.54, 1)))
# > Background:
# > - gradient = False
# > - colorColor1 = (0.106538, 0.106538, 0.106538, 0)
# > - colorColor2 = (0.106538, 0.106538, 0.106538, 0)
# > Display style: 1
# > Wire on shaded: 0
# > Lighting mode: 2
# > Light limit: 8
# > Color management enabled
# > Depth-of-field enabled: True
# > - Focus distance: 100.0
# > - Alpha: 0.00782441254705
# > Anti-aliasing enabled
# > Fog Enabled:
# > - Fog mode: 0
# > - Fog start: 83.9160842896
# > - Fog end: 335.664337158
# > - Fog density: 0.146853148937
# > - Fog color: (0.477876, 0.477876, 0.477876, 0.307692)
#
# From the render view would look like this:
# > MRenderOverride Frame Information Extraction Trace
# > --------------------------------------------------
# > Destination: renderView
# > Viewport: 0,0,960,540
# > Camera path: |persp|perspShape
# > Projection matrix:(((1.94444, 0, 0, 0), (0, 3.45679, 0, 0), (0, 0, -1, -1), (0, 0, -0.001, 0)))
# etc...

# Define maya_useNewAPI to indicate using Maya Python API 2.0.
maya_useNewAPI = True

class frameContextTest(omr.MRenderOverride):
    ''' Sample class which prints some trace information about the current frame context during setup()
    '''
    def __init__(self, name):
        omr.MRenderOverride.__init__(self, name)
        self.mRenderOperations = collections.OrderedDict()
        self.mUIName = "Frame Context Test Override"
        self.mCurrentOperationIndex = -1

    def supportedDrawAPIs(self):
        return ( omr.MRenderer.kAllDevices )

    def startOperationIterator(self):
        self.mCurrentOperationIndex = 0
        return True

    def renderOperation(self):
        if self.mCurrentOperationIndex >= 0 and self.mCurrentOperationIndex < len(self.mRenderOperations):
            return self.mRenderOperations[self.mRenderOperations.keys()[self.mCurrentOperationIndex]]
        else:
            return None

    def nextRenderOperation(self):
        self.mCurrentOperationIndex += 1   # increment iterator index
        return self.mCurrentOperationIndex < len(self.mRenderOperations)

    def setup(self, panelName ):
        # Print out some information by using the getFrameContext() method
        # Note that any per object information will not be relevant since
        # we have not begin executing the render pipeline at this point.
        #
        frameCtx = self.getFrameContext()
        if frameCtx:
            print('> MRenderOverride Frame Information Extraction Trace')
            print('> --------------------------------------------------')
            dim = frameCtx.getViewportDimensions()
            originX = dim[0]
            originY = dim[1]
            width = dim[2]
            height = dim[3]
            print('> Destination: ' + panelName)
            print('> Viewport: ' + str(originX) + ',' + str(originY) + ',' + str(width) + ',' + str(height))
            cameraPath = frameCtx.getCurrentCameraPath()
            cameraName = cameraPath.fullPathName()
            print('> Camera path: ' + cameraName)
            projection = frameCtx.getMatrix(omr.MFrameContext.kProjectionMtx)
            print('> Projection matrix:' + str(projection))
            projection = frameCtx.getMatrix(omr.MFrameContext.kViewMtx)
            print('> View matrix:' + str(projection))
            
            bparams = frameCtx.getBackgroundParameters();
            print("> Background:")
            print("> - gradient = " + str(bparams[0]))      # displayGradient
            print("> - colorColor1 = " + str(bparams[4]))   # clearColor1
            print("> - colorColor2 = " + str(bparams[5]))   # clearColor2

            print("> Display style: " + str(frameCtx.getDisplayStyle()))
            print("> Wire on shaded: " + str(frameCtx.wireOnShadedMode()))
            print("> Lighting mode: " + str(frameCtx.getLightingMode()))
            print("> Light limit: " + str(frameCtx.getLightLimit()))
            
            if frameCtx.getPostEffectEnabled( omr.MFrameContext.kAmbientOcclusion ):
                print("> SSAO enabled")
            if frameCtx.getPostEffectEnabled( omr.MFrameContext.kMotionBlur ):
                print("> Motion blur enabled");
            if frameCtx.getPostEffectEnabled( omr.MFrameContext.kViewColorTransformEnabled ):
                print("> Color management enabled")
            if frameCtx.getPostEffectEnabled( omr.MFrameContext.kDepthOfField ):
                dofParams = frameCtx.getDOFParameters()
                print("> Depth-of-field enabled: " + str(dofParams[0]))
                print("> - Focus distance: " + str(dofParams[1])) 
                print("> - Alpha: " + str(dofParams[2]))
            if frameCtx.getPostEffectEnabled( omr.MFrameContext.kAntiAliasing ):
                print("> Anti-aliasing enabled") 
            fogParams = frameCtx.getHwFogParameters()
            if (fogParams[0]):
                print("> Fog Enabled:")
                print("> - Fog mode: " + str(fogParams[1]))
                print("> - Fog start: " + str(fogParams[2]))
                print("> - Fog end: " + str(fogParams[3]))
                print("> - Fog density: " + str(fogParams[4]))
                print("> - Fog color: " + str(fogParams[5]))             

        # Create render operations (if not exist)
        if not self.mRenderOperations:
            self.mRenderOperations['SceneRender'] = omr.MSceneRender('SceneRender')
            self.mRenderOperations['Present'] = omr.MPresentTarget('Present')

    def cleanup(self):
        self.mCurrentOperationIndex = -1

mFrameContextInstance = None

def initializePlugin(obj):
    plugin = om.MFnPlugin(obj)
    global mFrameContextInstance
    mFrameContextInstance = frameContextTest('my_frame_context_test')
    omr.MRenderer.registerOverride(mFrameContextInstance)

def uninitializePlugin(obj):
    plugin = om.MFnPlugin(obj)
    global mFrameContextInstance
    if mFrameContextInstance is not None:
        omr.MRenderer.deregisterOverride(mFrameContextInstance)
        mFrameContextInstance = None

