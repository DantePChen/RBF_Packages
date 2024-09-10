########################################################################
# 
# DESCRIPTION:
# 
# Produces the MEL command "marqueeToolContext".
# 
# This plug-in demonstrates "marquee selection" in a user defined context.
# It is supported in Viewport 2.0 and OpenGL.  Selection is done through 
# the API (MGlobal).
# 
########################################################################
# First make sure that marqueeTool.py is in your MAYA_PLUG_IN_PATH.  Then, 
# to use the tool, execute the following in the script editor:
#
# from maya import cmds
# cmds.loadPlugin("marqueeTool.py")
# ctx = cmds.marqueeToolContext()
# cmds.setToolTo(ctx)
# 
# Once the tool is active, you can select objects in the 3D windows in the 
# same way that you do with the selection tool (it can be used for either 
# click selection or drag selection). Both will also work with the shift 
# key held down in the same manner as the selection tool. 
#
########################################################################

import logging
import math
import sys
from maya.api import OpenMaya, OpenMayaRender, OpenMayaUI
from maya import OpenMayaRender as OpenMayaRenderV1

logger = logging.getLogger('marqueeTool')

# tell Maya that we want to use Python API 2.0
maya_useNewAPI = True

# command
class MarqueeContextCmd (OpenMayaUI.MPxContextCommand):
    """
    This context command class creates instances of the MarqueeContext.
    """
    kPluginCmdName = "marqueeToolContext"

    def __init__(self):
        OpenMayaUI.MPxContextCommand.__init__(self)

    @classmethod
    def creator(cls):
        """
        This factory method creates an instance of the MarqueeContextCmd class.
        """
        return cls()

    def makeObj(self):
        """
        This factory method creates an instance of the MarqueeContext class.
        """
        return MarqueeContext()

class MarqueeContext(OpenMayaUI.MPxContext):
    """
    This context class extends a bounding box as the user drags the cursor during a selection
    opeartion.
    """

    help_string = "Click with left button or drag with middle button to select"

    @classmethod
    def creator(cls):
        """
        Create and return an instance of the MarqueeContext class.
        """
        return MarqueeContext(cls)

    def __init__(self):
        """
        Initialize the context member variables.
        """
        OpenMayaUI.MPxContext.__init__(self)
        self.fs_drawn = False
        self.list_adjustment = 0
        self.view = None
        self.setTitleString('Marquee Tool')
        self.setImage('marqueeTool.xpm', OpenMayaUI.MPxContext.kImage1)

    def stringClassName(self):
        """
        Return the class name string.
        """
        return 'Marquee Tool'

    def toolOnSetup( self, event ):
        """
        Perform any setup operations when the tool is created.  In this case,
        set the help string.
        """
        self.setHelpString( MarqueeContext.helpString )

    def check_event( self, event ):
        """
        Print out some information for the given event, such as its position which button
        was pressed, and if any modifiers were also pressed.
        """
        logger.info("Check event:")
        logger.info("    position: (%d, %d)" % event.position)
        logger.info("    windowPosition: (%d, %d)" % event.getWindowPosition())
        mouse_btn = event.mouseButton()
        if (mouse_btn == OpenMayaUI.MEvent.kLeftMouse):
            logger.info("    mouseButton: kLeftMouse")
        elif (mouse_btn == OpenMayaUI.MEvent.kMiddleMouse):
            logger.info("    mouseButton: kMiddleMouse")
        else:
            logger.info("    mouseButton: unknown!")
        if event.isModifierShift():
            logger.info("    Shift key was pressed")
        if event.isModifierControl():
            logger.info("    Control key was pressed")
        if event.isModifierNone():
            logger.info("    No modifiers were pressed")

    def do_press_common( self, event ):
        """
        Perfom the press operations common to both VP2.0 and the Legacy Viewport.
        """
        try:
            if event.isModifierShift() or event.isModifierControl():
                if event.isModifierShift():
                    if event.isModifierControl():
                        # both shift and control pressed, merge new selections
                        self.list_adjustment = OpenMaya.MGlobal.kAddToList
                    else:
                        # shift only, xor new selections with previous ones
                        self.list_adjustment = OpenMaya.MGlobal.kXORWithList

                elif event.isModifierControl():
                    # control only, remove new selections from the previous list
                    self.list_adjustment = OpenMaya.MGlobal.kRemoveFromList
            else:
                self.list_adjustment = OpenMaya.MGlobal.kReplaceList

            # Extract the event information
            #
            self.start = event.position
        except:
            logger.info("EXCEPTION IN do_press_common!")

    def do_release_common( self, event ):
        """
        Perfom the release operations common to both VP2.0 and the Legacy Viewport.
        """
        # Get the end position of the marquee
        self.last = event.position

        # Save the state of the current selections.  The "selectFromSceen"
        # below will alter the active list, and we have to be able to put
        # it back.
        incoming_list = OpenMaya.MGlobal.getActiveSelectionList()

        # If we have a zero dimension box, just do a point pick
        # 
        if ( math.fabs(self.start[0] - self.last[0]) < 2 and math.fabs(self.start[1] - self.last[1]) < 2 ):
            # This will check to see if the active view is in wireframe or not.
            selection_method = OpenMaya.MGlobal.selectionMethod()
            OpenMaya.MGlobal.selectFromScreen(self.start[0], self.start[1],
                                              listAdjustment = OpenMaya.MGlobal.kReplaceList, 
                                              selectMethod = selection_method )
        else:
            # The Maya select tool goes to wireframe select when doing a marquee, so
            # we will copy that behaviour.
            # Select all the objects or components within the marquee.
            OpenMaya.MGlobal.selectFromScreen(self.start[0], self.start[1], 
                                              self.last[0], self.last[1],
                                              listAdjustment = OpenMaya.MGlobal.kReplaceList, 
                                              selectMethod = OpenMaya.MGlobal.kWireframeSelectMethod )

        # Get the list of selected items
        marquee_list = OpenMaya.MGlobal.getActiveSelectionList()

        # Restore the active selection list to what it was before the "selectFromScreen"
        OpenMaya.MGlobal.setActiveSelectionList(incoming_list, OpenMaya.MGlobal.kReplaceList)

        # Update the selection list as indicated by the modifier keys.
        OpenMaya.MGlobal.selectCommand(marquee_list, self.list_adjustment)

		# Test the functionality of several of the Mevent methods.
        logger.info("Mouse button release state:")
        self.check_event(event)

    def draw_marquee_gl( self ):
        """
        Draw the Marquee bounding box using OpenGL.  This method is used by the Legacy Viewport.
        """
        self.view.beginGL()

        ## drawing in VP1 views will be done using V1 Python APIs:
        gl_renderer = OpenMayaRenderV1.MHardwareRenderer.theRenderer()
        gl_ft = gl_renderer.glFunctionTable()
        gl_ft.glBegin( OpenMayaRenderV1.MGL_LINE_LOOP )
        gl_ft.glVertex2i( self.start[0], self.start[1] )
        gl_ft.glVertex2i( self.last[0],  self.start[1] )
        gl_ft.glVertex2i( self.last[0],  self.last[1] )
        gl_ft.glVertex2i( self.start[0], self.last[1] )
        gl_ft.glEnd()

        self.view.endGL()

    def doPressLegacy( self, event ):
        """
        Handle the mouse press event in the Legacy Viewport.
        """
        self.do_press_common(event)
        self.view = OpenMayaUI.M3dView.active3dView()
        self.fs_drawn = False

    def doDragLegacy( self, event ):
        """
        Handle the mouse drag event in the Legacy Viewport.
        """
        self.view.beginXorDrawing()
        if self.fs_drawn:
            # Redraw the marquee at its old position to erase it.
            self.draw_marquee_gl()

        self.fs_drawn = True

        # Get the marquee's new end position.
        self.last = event.position

        # Draw the marquee at its new position.
        self.draw_marquee_gl()
        self.view.endXorDrawing()

    def doHoldLegacy( self, event ):
        """
        Handle the mouse hold event in the Legacy Viewport.
        """
        self.do_hold_common(event)

    def doReleaseLegacy( self, event ):
        """
        Handle the mouse release event in the Legacy Viewport.
        """
        # Selects objects within the marquee box.
        if self.fs_drawn:
            self.view.beginXorDrawing();

            # Redraw the marquee at its old position to erase it.
            self.draw_marquee_gl()
            self.view.endXorDrawing()

        self.do_release_common( event );

    def doPress( self, event, drawMgr, context ):
        """
        Handle the mouse press event in VP2.0.
        """
        self.do_press_common(event)

    def doRelease( self, event, drawMgr, context ):
        """
        Handle the release press event in VP2.0.
        """
        self.do_release_common(event)

    def doDrag( self, event, draw_mgr, context ):
        """
        Handle the mouse drag event in VP2.0.
        """
        # Get the marquee's new end position.
        self.last = event.position

        # Draw the marquee at its new position.
        draw_mgr.beginDrawable()
        draw_mgr.setColor( OpenMaya.MColor((1.0, 1.0, 0.0)) )
        draw_mgr.line2d( OpenMaya.MPoint( (self.start[0], self.start[1])), OpenMaya.MPoint((self.last[0], self.start[1])) )
        draw_mgr.line2d( OpenMaya.MPoint( (self.last[0], self.start[1])),  OpenMaya.MPoint((self.last[0], self.last[1])) )
        draw_mgr.line2d( OpenMaya.MPoint( (self.last[0], self.last[1])),   OpenMaya.MPoint((self.start[0], self.last[1])) )
        draw_mgr.line2d( OpenMaya.MPoint( (self.start[0], self.last[1])),  OpenMaya.MPoint((self.start[0], self.start[1])) )
        draw_mgr.endDrawable()

    def doHold( self, event, drawMgr, context ):
        """
        Handle the mouse hold event in VP2.0.
        """
        self.do_hold_common(event)

    def doEnterRegion( self, event ):
        """
        Handle the enter region event.  This method is called from both VP2.0 and the Legacy Viewport.
        """
        self.setHelpString( MarqueeContext.help_string )

# Initialize the script plug-in
def initializePlugin(plugin):
    pluginFn = OpenMaya.MFnPlugin(plugin)

    try:
        pluginFn.registerContextCommand( MarqueeContextCmd.kPluginCmdName, MarqueeContextCmd.creator)
    except:
        logger.error("Failed to register context command: %s\n" % MarqueeContextCmd.kPluginCmdName)
        raise


# Uninitialize the script plug-in
def uninitializePlugin(plugin):
    pluginFn = OpenMaya.MFnPlugin(plugin)
    try:
        pluginFn.deregisterContextCommand(MarqueeContextCmd.kPluginCmdName)
    except:
        logger.error("Failed to unregister command: %s\n" % MarqueeContextCmd.kPluginCmdName)
        raise

#-
# ==========================================================================
# Copyright (C) 2016 Autodesk, Inc. and/or its licensors.  All
# rights reserved.
#
# The coded instructions, statements, computer programs, and/or related
# material (collectively the "Data") in these files contain unpublished
# information proprietary to Autodesk, Inc. ("Autodesk") and/or its
# licensors, which is protected by U.S. and Canadian federal copyright
# law and by international treaties.
#
# The Data is provided for use exclusively by You. You have the right
# to use, modify, and incorporate this Data into other products for
# purposes authorized by the Autodesk software license agreement,
# without fee.
#
# The copyright notices in the Software and this entire statement,
# including the above license grant, this restriction and the
# following disclaimer, must be included in all copies of the
# Software, in whole or in part, and all derivative works of
# the Software, unless such copies or derivative works are solely
# in the form of machine-executable object code generated by a
# source language processor.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
# AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
# WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
# NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR
# PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR
# TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS
# BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL,
# DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK
# AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY
# OR PROBABILITY OF SUCH DAMAGES.
#
# ==========================================================================
#+
