import maya.api.OpenMaya as om
import maya.api.OpenMayaAnim as oma
import maya.api.OpenMayaUI as omui
import maya.api.OpenMayaRender as omr
import maya.mel as mel
import maya.cmds as cmds

##############################################################################
#                                                                            #
# Primitive drawings with MUIDrawManager using callbacks                     #
#                                                                            #
##############################################################################

# Define a function that draws an icon
#
def iconFunc( drawMgr, userData ):
    drawMgr.beginDrawable()

    path = mel.eval('getenv("MAYA_LOCATION")') + "/icons/mayaico.png"
    drawMgr.icon( om.MPoint(0.0, 0.0, 3.0), path, 3.14 )

    drawMgr.endDrawable()

# Define a function that draws text primitives in various alignments and sizes
#
def textFunc( drawMgr, userData ):
    drawMgr.beginDrawable()

    drawMgr.text( om.MPoint(200.1, 200.2, 0.3), "left", omr.MUIDrawManager.kLeft, [200,200])
    drawMgr.setColor( om.MColor( (1.0, 0.0, 0.0, 1.0) ) )
    drawMgr.text( om.MPoint(200.1, 150.2, 0.3), "center", omr.MUIDrawManager.kCenter, [200,200])
    drawMgr.setColor( om.MColor( (0.0, 1.0, 0.0, 1.0) ) )
    drawMgr.text( om.MPoint(200.1, 200.2, 0.3), "right", omr.MUIDrawManager.kRight, [200,200])
    
    # Specify a background color
    #
    drawMgr.setFontSize( omr.MUIDrawManager.kSmallFontSize )
    drawMgr.setColor( om.MColor( (0.0, 0.0, 1.0, 1.0) ) )
    drawMgr.text( om.MPoint(700.1, 50.2, 0.3), "small", omr.MUIDrawManager.kLeft, [200,200], om.MColor((0., 1., 1., 1.)), False )

    drawMgr.endDrawable()


# Define a function that draws an outline of a rectangle, and a solid rectangle
#
def rectFunc( drawMgr, userData ):
    drawMgr.beginDrawable()

    drawMgr.setColor( om.MColor( (1.0, 0.0, 1.0, 1.0) ) )
    drawMgr.rect2d( om.MPoint(50, 150, 0.3), om.MVector(0.0,1.0,0.0), 100, 300, False )

    drawMgr.setColor( om.MColor( (0.0, 1.0, 1.0, 1.0) ) )
    drawMgr.rect2d( om.MPoint(450.1, 500.2, 0.3), om.MVector(-0.5,0.866025,0.0), 100, 300, True )

    drawMgr.endDrawable()

# Define a function that draws an arc, and a pie wedge (filled arc)
#
def arcFunc( drawMgr, userData ):
    drawMgr.beginDrawable()

    drawMgr.setColor( om.MColor( (1.0, 1.0, 1.0, 1.0) ) )
    drawMgr.arc2d( om.MPoint(750.1, 500.2, 0.3), om.MVector(0.866025, 0.5, 0.0), om.MVector(-0.5,0.866025,0.0), 100, False )

    drawMgr.setColor( om.MColor( (1.0, 1.0, 0.5, 1.0) ) )
    drawMgr.arc2d( om.MPoint(750.1, 500.2, 0.3), om.MVector(-0.866025, -0.5, 0.0), om.MVector(0.5,-0.866025,0.0), 100, True )

    drawMgr.endDrawable()


# Define a function that draws the outline of a circle, and a solid circle
#
def circleFunc( drawMgr, userData ):
    drawMgr.beginDrawable()

    drawMgr.setColor( om.MColor( (0.5, 1.0, 1.0, 1.0) ) )
    drawMgr.circle2d( om.MPoint(500.1, 200.2, 0.3), 75, False )
    drawMgr.setColor( om.MColor( (0.5, 1.0, 0.5, 1.0) ) )
    drawMgr.circle2d( om.MPoint(675.1, 675.2, 0.3), 50, True )

    drawMgr.endDrawable()


def otherFunc( drawMgr, userData ):
    # This callback doesn't actually draw anything, for testing purposes
    pass

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
    geCanvas = omui.MPanelCanvas(geName)
    global myCanvas
    myCanvas = [geCanvas, geName, geWindow]
    geCanvas.setAutoRefresh( False )

    # Note the reserved Graph Editor layers are as follows:
    #
    # MPanelCanvas.kGraphEditorBackground - 0
    # MPanelCanvas.kGraphEditorGrid - 100
    # MPanelCanvas.kGraphEditorAxisLabels - 1000
    # MPanelCanvas.kGraphEditorCurves - 2000
    # MPanelCanvas.kGraphEditorCurveNames - 3000
    # MPanelCanvas.kGraphEditorTimeMarker - 4000
    # MPanelCanvas.kGraphEditorRetimeToolText - 5000
    # MPanelCanvas.kGraphEditorLastDefaultDraw - 10000
    #
    # To draw between, say, the Grid and the Axis labels, use a layer number
    # greater than 100 but less than 1000.

    ##########################################################################
    #                                                                        #
    # Screen space primitives                                                #
    #                                                                        #
    # (0,0) is the bottom left coordinate of the screen                      #
    # (pixel width, pixel height) is the top right coordinate of the screen  #
    #                                                                        #
    ##########################################################################

    # Create 8 solid diamonds in screen space, of various colours, in various
    # layers.
    #
    # The diamonds are drawn in between each reserved layer as follows:
    #
    layers = [ omui.MPanelCanvas.kGraphEditorBackground+55
             , omui.MPanelCanvas.kGraphEditorGrid+55
             , omui.MPanelCanvas.kGraphEditorAxisLabels+55
             , omui.MPanelCanvas.kGraphEditorCurves+55
             , omui.MPanelCanvas.kGraphEditorCurveNames+55
             , omui.MPanelCanvas.kGraphEditorTimeMarker+55
             , omui.MPanelCanvas.kGraphEditorTimeMarker+55
             , omui.MPanelCanvas.kGraphEditorRetimeToolText+55
             ]

    props = omui.MDrawProperties()
    props.color = om.MColor( [0.2, 0.8, 0.6])
    props.lineWidth = 0.5
    props.lineStyle = omr.MUIDrawManager.kDotted
    props.pointSize = 12.0

    # Half diagonals of each diamond
    #
    half_diags = [300-30*n for n in range(0,8)]

    # Color multipliers for each diamond
    #
    reds = [1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0]
    greens = [0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0]
    blues = [0.0, 0.0, 1.0, 1.0, 0.25, 0.25, 0.75, 0.75]

    # Each diamond consists of 6 points 3 for each triangle
    # Define the start point of each diamond
    #
    starts = [6*n for n in range(0,8)]

    # Each diamond is centered at (330, 330)
    #
    cx = 330
    cy = 330

    # Create a float vertex buffer to define the 8 diamonds
    #
    xArray = []
    yArray = []
    cArray = []

    a = 1.0
    for r,g,b,h in zip(reds, greens, blues, half_diags):
        xArray += [cx-h, cx, cx+h, cx+h, cx, cx-h]
        yArray += [cy, cy-h, cy, cy, cy+h, cy]
        cArray += [ [r*.25, g*.25, b, a], [r*.50, g*.50, b, a], [r*.75, g*.75, b, a], [r*.75, g*.75, b, a],  [r, g, b, a],  [r*.25, g*.25, b, a]]

    floatId = geCanvas.createFloatVertexBuffer( xArray, yArray, cArray )

    # Define the 8 diamonds, and add to a specific layer
    #
    for s,l in zip(starts, layers):
        p = geCanvas.createPrimitive( omr.MGeometry.kTriangles, floatId, s, 6, props )
        geCanvas.addPrimitive( p, l )

    # Create six line drawing of squares, using the various stippling patterns:
    #
    # kDotted, kShortDotted, kDashed, kShortDashed, kSolid, kSolid
    #
    layers = [ omui.MPanelCanvas.kGraphEditorCurves+55
             , omui.MPanelCanvas.kGraphEditorCurves+55
             , omui.MPanelCanvas.kGraphEditorAxisLabels+55
             , omui.MPanelCanvas.kGraphEditorAxisLabels+55
             , omui.MPanelCanvas.kGraphEditorCurveNames+55
             , omui.MPanelCanvas.kGraphEditorTimeMarker+55
             ]
    patterns = [omr.MUIDrawManager.kDotted, omr.MUIDrawManager.kShortDotted, omr.MUIDrawManager.kDashed, omr.MUIDrawManager.kShortDashed, omr.MUIDrawManager.kSolid, omr.MUIDrawManager.kDotted]
        
    # Half sides of each square
    #
    half_sides = half_diags = [300-30*n for n in range(0,6)]

    # Color multipliers for each square
    #
    reds = [1.0, 0.0, 1.0, 0.0, 1.0, 0.0]
    greens = [0.0, 1.0, 0.0, 1.0, 0.0, 1.0]
    blues = [0.5, 0.5, 1.0, 1.0, 0.25, 0.25]

    # Each square consists of 5 points (4 line segments)
    # Define the start point of each square
    #
    starts = [5*n for n in range(0,6)]

    # Each square is centered at (510, 510)
    #
    cx = 510
    cy = 510

    xArray = []
    yArray = []
    cArray = []

    a = 1.0
    for r,g,b,h in zip(reds, greens, blues, half_sides):
        xArray += [cx-h, cx+h, cx+h, cx-h, cx-h]
        yArray += [cy-h, cy-h, cy+h, cy+h, cy-h]
        cArray += [ [r*.40, g*.40, b, a], [r*.50, g*.50, b, a], [r*.60, g*.60, b, a], [r*.75, g*.75, b, a],  [r, g, b, a]]

    floatId = geCanvas.createFloatVertexBuffer( xArray, yArray, cArray )

    # Define the 6 squares, and add to a specific layer
    #
    for s,l,p in zip(starts, layers, patterns):
        props.lineStyle = p
        id = geCanvas.createPrimitive( omr.MGeometry.kLineStrip, floatId, s, 5, props )
        geCanvas.addPrimitive( id, l )


    ##########################################################################
    #                                                                        #
    # Time based primitives                                                  #
    #                                                                        #
    # The x-axis values are time (frames)                                    #
    # The y-values are as displayed in the Normal Graph Editor View          #
    #                                                                        #
    ##########################################################################

    # Draw 4 time based diamonds, in various layers
    #
    # The time based ranges of the diamonds are:
    #    [0,12], [1,11], [12,20], [13,20]
    #
    # The y based ranges of the diamonds are:
    #    [-40,+40], [-36,+36], [-20,+20], [-18,+18]
    #

    layers = [ omui.MPanelCanvas.kGraphEditorAxisLabels+55
             , omui.MPanelCanvas.kGraphEditorCurves+55
             , omui.MPanelCanvas.kGraphEditorCurveNames+55
             , omui.MPanelCanvas.kGraphEditorTimeMarker+55
             ]

    props = omui.MDrawProperties()
    props.color = om.MColor( [0.2, 0.8, 0.6])
    props.lineWidth = 0.5
    props.lineStyle = omr.MUIDrawManager.kDotted
    props.pointSize = 12.0

    # Half diagonals of each diamond
    #
    half_diags_t = [6-n for n in range(0,4)]
    half_diags_y = [40, 36, 20, 18]

    # Color multipliers for each diamond
    #
    reds = [1.0, 0.0, 1.0, 0.0]
    greens = [0.0, 1.0, 0.0, 1.0]
    blues = [0.25, 0.25, 0.75, 0.75]

    # Each diamond consists of 6 points 3 for each triangle
    # Define the start point of each diamond
    #
    starts = [6*n for n in range(0,4)]

    # First 2 diamonds are centered at (6, 0), other 2 at (16, 0)
    #
    cxArray = [6, 6, 16, 16]

    # Create a time vertex buffer to define the 4 diamonds
    #
    tArray = []
    yArray = []
    cArray = []

    a = 1.0
    for cx, r,g,b,ht, hy in zip(cxArray, reds, greens, blues, half_diags_t, half_diags_y):
        tArray += [om.MTime(t) for t in [cx-ht, cx, cx+ht, cx+ht, cx, cx-ht]]
        yArray += [0, -hy, 0, 0, hy, 0]
        cArray += [ [r*.25, g*.25, b, a], [r*.50, g*.50, b, a], [r*.75, g*.75, b, a], [r*.75, g*.75, b, a],  [r, g, b, a],  [r*.25, g*.25, b, a]]

    timeId = geCanvas.createTimeVertexBuffer( tArray, yArray, cArray )

    # Define the 4 diamonds, and add to a specific layer
    #
    for s,l in zip(starts, layers):
        p = geCanvas.createPrimitive( omr.MGeometry.kTriangles, timeId, s, 6, props )
        geCanvas.addPrimitive( p, l )

    # Draw a time based box, from frame 0 to frame 30, and from -30 to +30 in y
    # If the scene twoCurves.ma is loaded, the box should frame the two curves.
    # If you drag the time line, the box moves accordingly, and always frames
    # the two curves.
    #

    tArray =  [om.MTime(t) for t in [0,30,30,0,0]]
    yArray = [30,30,-30,-30,30]
    cArray = [[r*.2,1.0,1.0,1.0] for r in range(1,6)]

    timeId = geCanvas.createTimeVertexBuffer( tArray, yArray, cArray )

    props = omui.MDrawProperties()
    props.color = om.MColor( [0.2, 0.8, 0.6])
    props.lineWidth = 0.5
    props.lineStyle = omr.MUIDrawManager.kDashed
    props.pointSize = 12.0

    primId = geCanvas.createPrimitive( omr.MGeometry.kLineStrip, timeId, 0, 5, props )

    geCanvas.addPrimitive( primId, omui.MPanelCanvas.kGraphEditorCurveNames+56 )

    # Register the callbacks in specified layers.
    #
    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorBackground+56, otherFunc )

    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorGrid+56, iconFunc )

    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorAxisLabels+56, textFunc )

    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorCurves+56, rectFunc )

    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorCurveNames+54, arcFunc )

    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorTimeMarker+54, circleFunc )

    geCanvas.registerDrawUICallback( omui.MPanelCanvas.kGraphEditorRetimeToolText+56, otherFunc )

    geCanvas.setAutoRefresh( True )

    print geCanvas.isValid()


def uninitializePlugin( mobject ):
    """
    Description:
        this method is called when the plug-in is unloaded from Maya. It 
        deregisters all of the services that it was providing.

    Arguments:
        mobject - a handle to the plug-in object (use MFnPlugin to access it)
    """
    global myCanvas
    [geCanvas, geName, geWindow] = myCanvas
    cmds.deleteUI(geWindow)
    print geCanvas.isValid()

