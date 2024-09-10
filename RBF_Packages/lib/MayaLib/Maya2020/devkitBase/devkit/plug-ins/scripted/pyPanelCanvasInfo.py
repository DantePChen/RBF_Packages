import maya.api.OpenMaya as om
import maya.api.OpenMayaAnim as oma
import maya.api.OpenMayaUI as omu
import maya.api.OpenMayaRender as omr
import maya.mel as mel
import maya.cmds as cmds

##############################################################################
#                                                                            #
# Primitive drawings with MUIDrawManager using callbacks                     #
#                                                                            #
##############################################################################

def createScene():

    # Create an animated cylinder, and set a time range of [0,30]
    #
    cmds.file(force=1,new=1)
    myCyl = cmds.polyCylinder()
    cmds.setAttr( "pCylinder1.s",1, 3.3522764559816802, 1, type="double3"  )

    cmds.playbackOptions(min=0, ast=0, max=30, aet=30)
    cmds.currentTime( 0 )
    cmds.setAttr( 'pCylinder1.tz', 12 )
    cmds.setAttr( 'pCylinder1.rx', 30 )
    cmds.setKeyframe( ['pCylinder1.tz','pCylinder1.rx'])

    cmds.currentTime(15)
    cmds.setAttr( 'pCylinder1.tz', 0 )
    cmds.setAttr( 'pCylinder1.rx', 0 )
    cmds.setKeyframe( ['pCylinder1.tz','pCylinder1.rx'])

    cmds.currentTime(30)
    cmds.setAttr( 'pCylinder1.tz', -12 )
    cmds.setAttr( 'pCylinder1.rx', -30 )
    cmds.setKeyframe( ['pCylinder1.tz','pCylinder1.rx'])

def createGraphEditor():
    # Create a Graph Editor for testing purposes
    #
    geName = 'myGraphEditorGraphEd';
    if( cmds.scriptedPanel('myGraphEditor', exists=1) ):
        cmds.deleteUI('myGraphEditor')

    geWindow = cmds.window(width=1200,height=800)
    gePane = cmds.paneLayout(parent=geWindow)
    cmds.scriptedPanel('myGraphEditor',type='graphEditor', parent=gePane,tearOff=False)
    cmds.setParent('..')
    cmds.showWindow(geWindow)
    return [geName, geWindow]

def maya_useNewAPI():
    pass

def initializePlugin( mobject ):
    """
    Description:
        this method is called when the plug-in is loaded into Maya.  It 
        registers all of the services that this plug-in provides with 
        Maya.

    Arguments:
        mobject - a handle to the plug-in object (use MFnPlugin to access it)
    """
    createScene()
    [geName, geWindow] = createGraphEditor()
    geCanvasInfo = omu.MPanelCanvasInfo(geName)
    global myCanvasInfo
    myCanvasInfo = [geCanvasInfo, geName, geWindow]

    # Get the viewport size, and print it
    #
    viewportSize = geCanvasInfo.getViewportSize()
    print 'viewportSize'
    print viewportSize

    # Get the bounds, and print them
    #
    viewportBounds = geCanvasInfo.getViewportBounds()
    print 'viewportBounds'
    print viewportBounds

    # Shrink the bounds by 10%, set, get and print new values
    #
    viewportBounds[0] = viewportBounds[0]*0.9
    viewportBounds[1] = viewportBounds[1]*0.9
    viewportBounds[2] = viewportBounds[2]*0.9
    viewportBounds[3] = viewportBounds[3]*0.9
    
    geCanvasInfo.setViewportBounds( viewportBounds )
    newViewportBounds = geCanvasInfo.getViewportBounds()

    if( geCanvasInfo.supportsUIDrawing() ):
        print 'Editor supports UI Drawing'

    defaultCanvasInfo = omu.MPanelCanvasInfo('graphEditor1GraphEd')

    if( defaultCanvasInfo.supportsUIDrawing() ):
        print 'Default Graph Editor is ready for drawing'
    else:
        print 'Default Graph Editor is not opened'

    print 'newViewportBounds'
    print newViewportBounds


def uninitializePlugin( mobject ):
    """
    Description:
        this method is called when the plug-in is unloaded from Maya. It 
        deregisters all of the services that it was providing.

    Arguments:
        mobject - a handle to the plug-in object (use MFnPlugin to access it)
    """
    global myCanvasInfo
    [geCanvas, geName, geWindow] = myCanvasInfo
    cmds.deleteUI(geWindow)

