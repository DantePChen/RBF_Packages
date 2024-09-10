from __future__ import division
########################################################################
# 
# DESCRIPTION:
# 
# Produces the MEL command "lassoToolContext".
# 
# This plug-in demonstrates "lasso selection" in a user defined context.
# It is supported in Viewport 2.0 and the Legacy Viewport.
# Selection is done through the API (MGlobal).
# 
########################################################################
#
# First make sure that lassoTool.py is in your MAYA_PLUG_IN_PATH.  Then, 
# to use the tool, execute the following in the script editor:
#
# cmds.loadPlugin("lassoTool.py")
# ctx = cmds.lassoToolContext()
# cmds.setToolTo(ctx)
#
# With Maya in component mode and the tool active, lasso select components
# in the viewport.  Holding the shift key down will toggle the selection 
# of the enclosed components.
#
########################################################################

from builtins import next
from builtins import range
import logging
import math
import sys
from maya.api import OpenMaya, OpenMayaRender, OpenMayaUI
from maya import OpenMayaRender as OpenMayaRenderV1

logger = logging.getLogger('lassoTool')

# tell Maya that we want to use Python API 2.0
maya_useNewAPI = True

# command
class LassoToolContextCmd (OpenMayaUI.MPxContextCommand):
    """
    This context command class creates instances of the LassoToolContext.
    """
    kPluginCmdName = "py2LassoToolContext"

    def __init__(self):
        OpenMayaUI.MPxContextCommand.__init__(self)

    @classmethod
    def creator(cls):
        """
        This factory method creates an instance of the LassoToolContextCmd class.
        """
        return cls()

    def makeObj(self):
        """
        This factory method creates an instance of the LassoToolContext class.
        """
        return LassoToolContext()

class LassoToolContext(OpenMayaUI.MPxContext):
    """
    This context class extends a bounding box as the user drags the cursor during a selection
    opeartion.
    """

    help_string = "Drag mouse to select points by encircling"

    cursor_width = 16
    cursor_height = 16
    cursor_x_hot = 1
    cursor_y_hot = 16
    cursor_bits = bytearray( [
        0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x10, 0x08, 0x10, 0x10, 0x08, 0x10,
        0x08, 0x10, 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x14, 0x08, 0x14, 0x04,
        0x08, 0x07, 0xf4, 0x00, 0x02, 0x00, 0x01, 0x00] )
    cursor_mask_bits = bytearray( [
        0x00, 0x00, 0x00, 0x00, 0xe0, 0x07, 0x10, 0x08, 0x10, 0x10, 0x08, 0x10,
        0x08, 0x10, 0x08, 0x10, 0x08, 0x08, 0x08, 0x08, 0x14, 0x08, 0x14, 0x04,
        0x08, 0x07, 0xf4, 0x00, 0x02, 0x00, 0x01, 0x00] )

    @classmethod
    def creator(cls):
        """
        Create and return an instance of the LassoToolContext class.
        """
        return LassoToolContext(cls)

    def __init__(self):
        """
        Initialize the context member variables.
        """
        OpenMayaUI.MPxContext.__init__(self)
        self.lasso = []
        self.sorted_lasso = []
        self.lasso_cursor = OpenMayaUI.MCursor(width=self.cursor_width,
            height=self.cursor_height,
            hotSpotX=self.cursor_x_hot,
            hotSpotY=self.cursor_y_hot,
            bits=self.cursor_bits,
            mask=self.cursor_mask_bits)

        self.first_draw = False
        self.list_adjustment = 0
        self.view = None
        self.setTitleString('Lasso Tool')

        # Set the initial state of the cursor.
        self.setCursor(self.lasso_cursor)

        # Tell the context which XPM to use.
        self.setImage('lassoTool.xpm', OpenMayaUI.MPxContext.kImage1)

    def stringClassName(self):
        """
        Return the class name string.
        """
        return 'Lasso Tool'

    def toolOnSetup( self, event ):
        """
        Perform any setup operations when the tool is created.  In this case,
        set the help string.
        """
        self.setHelpString( LassoToolContext.help_string )

    def do_press_common( self, event ):
        """
        Perfom the press operations common to both VP2.0 and the Legacy Viewport.
        """

        # Figure out which modifier keys were pressed, and set up the
        # listAdjustment parameter to reflect what to do with the selected points.
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

        start = event.position
        self.lasso.append([start[0], start[1]])
        self.min = [start[0], start[1]]
        self.max = [start[0], start[1]]
        self.first_draw = True
        self.view = OpenMayaUI.M3dView.active3dView()

    def do_release_common( self, event ):
        """
        Perfom the release operations common to both VP2.0 and the Legacy Viewport.
        """

        # Close the lasso by appending the starting point, and sort all the points on it.
        self.append_lasso(self.lasso[0])
        self.sorted_lasso = sorted(self.lasso, key=lambda x: (x[1], x[0]))

        # Save the state of the current selections.  The "selectFromSceen"
        # below will alter the active list, and we have to be able to put
        # it back.
        incoming_list = OpenMaya.MGlobal.getActiveSelectionList()

        # As a first approximation to the lasso, select all components with
        # the bounding box that just contains the lasso.
        OpenMaya.MGlobal.selectFromScreen( self.min[0], self.min[1], self.max[0], self.max[1], 
                                           OpenMaya.MGlobal.kReplaceList )

        # Get the list of selected items from within the bounding box
        # and create a iterator for them.
        bounding_box_list = OpenMaya.MGlobal.getActiveSelectionList()

        # Restore the active selection list to what it was before we
        # the "selectFromScreen"
        OpenMaya.MGlobal.setActiveSelectionList(incoming_list, OpenMaya.MGlobal.kReplaceList)

        # Iterate over the objects within the bounding box, extract the
        # ones that are within the lasso, and add those to new_list.
        iter = OpenMaya.MItSelectionList(bounding_box_list)
        new_list = OpenMaya.MSelectionList()
        new_list.clear()

        found_entire_objects = False
        found_components = False

        while not iter.isDone():

            dag_path = iter.getDagPath()
            component = OpenMaya.MObject.kNullObj
            if iter.hasComponents():
                sel = iter.getComponent()
                component = sel[1]

            if component.isNull():
                # not a component
                found_entire_objects = True
                next(iter)
                continue

            found_components = True

            if component.apiType() == OpenMaya.MFn.kCurveCVComponent:
                curve_cv_iter = OpenMaya.MItCurveCV( dag_path, component)
                while not curve_cv_iter.isDone():
                    try:
                        point = curve_cv_iter.position(OpenMaya.MSpace.kWorld)
                        pt = self.view.worldToView(point)
                        if self.point_in_lasso(pt):
                            single_component = curve_cv_iter.currentItem()
                            new_list.add ((dag_path, single_component))
                    except:
                        OpenMaya.MGlobal.displayError("Could not get position")
                    next(curve_cv_iter)

            elif component.apiType() == OpenMaya.MFn.kSurfaceCVComponent:
                surf_cv_iter = OpenMaya.MItSurfaceCV(dag_path, component, true)
                while not surf_cv_iter.isDone():
                    try:
                        point = surf_cv_iter.position(OpenMaya.MSpace.kWorld)
                        pt = self.view.worldToView(point)
                        if self.point_in_lasso(pt):
                            single_component = surf_cv_iter.currentItem()
                            new_list.add((dag_path, single_component))
                    except:
                        OpenMaya.MGlobal.displayError("Could not get position")
                    next(surf_cv_iter)

            elif component.apiType() == OpenMaya.MFn.kMeshVertComponent:
                vertex_iter = OpenMaya.MItMeshVertex(dag_path, component)
                while not vertex_iter.isDone():
                    try:
                        point = vertex_iter.position(OpenMaya.MSpace.kWorld)
                        pt = self.view.worldToView(point)
                        if self.point_in_lasso(pt):
                            single_component = vertex_iter.currentItem()
                            new_list.add((dag_path, single_component))
                    except:
                        OpenMaya.MGlobal.displayError("Could not get position")
                    next(vertex_iter)

            elif component.apiType() == OpenMaya.MFn.kMeshEdgeComponent:
                edge_iter = OpenMaya.MItMeshEdge(dag_path, component)
                while not edge_iter.isDone():
                    try:
                        point = edge_iter.center(OpenMaya.MSpace.kWorld)
                        pt = view.worldToView(point)
                        if self.point_in_lasso(pt):
                            single_component = edge_iter.currentItem()
                            new_list.add ((dag_path, single_component))
                    except:
                        OpenMaya.MGlobal.displayError("Could not get position")
                    next(edge_iter)

            elif component.apiType() == OpenMaya.MFn.kMeshPolygonComponent:
                polygon_iter = OpenMaya.MItMeshPolygon(dag_path, component)
                while not polygon_iter.isDone():
                    try:
                        point = polygon_iter.center(OpenMaya.MSpace.kWorld)
                        pt = view.worldToView(point)
                        if self.point_in_lasso(pt):
                            single_component = polygon_iter.currentItem();
                            new_list.add ((dag_path, single_component))
                    except:
                        OpenMaya.MGlobal.displayError("Could not get position")
                    next(polygon_iter)

            next(iter)

        # Warn user if they are trying to select objects rather than components.
        if found_entire_objects and (not found_components):
            OpenMaya.MGlobal.displayWarning("lassoTool can only select components, not entire objects.")

        # Update the selection list as indicated by the modifier keys.
        OpenMaya.MGlobal.selectCommand(new_list, self.list_adjustment)

        # Free the memory that held the lasso points.
        self.lasso = []
        self.sorted_lasso = []

    def draw_lasso_gl( self ):
        """
        Draw the lasso using OpenGL.  This method is used by the Legacy Viewport.
        """
        self.view.beginGL()

        # Drawing in VP1 views will be done using V1 Python APIs:
        gl_renderer = OpenMayaRenderV1.MHardwareRenderer.theRenderer()
        gl_ft = gl_renderer.glFunctionTable()
        gl_ft.glBegin( OpenMayaRenderV1.MGL_LINE_LOOP )
        for i in range(len(self.lasso)):
            gl_ft.glVertex2i( self.lasso[i][0], self.lasso[i][1] );
        gl_ft.glEnd()

        self.view.endGL()

    def append_lasso( self, pt ):
        """
        Append the given point to the points defining the lasso.
        """
        x = pt[0]
        y = pt[1]
        [ix, iy] = self.lasso[-1]
        ydif = int(math.fabs( y - iy ))
        if ( ydif == 0 ):
            return

        # Keep track of smallest rectangular area of the screen that
        # completely contains the lasso.
        if ( self.min[0] > x ):
            self.min[0] = x
        if ( self.max[0] < x ):
            self.max[0] = x
        if ( self.min[1] > y ):
            self.min[1] = y
        if ( self.max[1] < y ):
            self.max[1] = y

        if ((y - iy ) < 0):
            yinc = -1
        else:
            yinc = 1

        xinc = (x - ix)/ydif
        fx = ix + xinc
        cy = iy + yinc
        for i in range(ydif):
            self.lasso.append([int(fx), int(cy)])
            fx = fx + xinc
            cy = cy + yinc

    def point_in_lasso( self, pt ):
        """
        Check the given point to see if it's inside the loop defined by the lasso.
        """
        for i in range(len(self.lasso)):
            if (self.sorted_lasso[i][1] == pt[1]):
                while ((self.sorted_lasso[i][1] == pt[1]) and (self.sorted_lasso[i][0] < pt[0])):
                    i = i + 1
                if (self.sorted_lasso[i][1] != pt[1]):
                    return False
                sides = 0
                i = i + 1
                while (self.sorted_lasso[i][1] == pt[1]):
                    i = i + 1
                    sides = sides + 1

                if (sides % 2):
                    return False
                else:
                    return True
        return False

    def doPressLegacy( self, event ):
        """
        Handle the mouse press event in the Legacy Viewport.
        """
        self.do_press_common(event)
        self.first_draw = False

    def doDragLegacy( self, event ):
        """
        Handle the mouse drag event in the Legacy Viewport.  Add to the growing lasso.
        """
        self.view.beginXorDrawing(True, True, 1.0, OpenMayaUI.M3dView.kStippleDashed)
        if not self.first_draw:
            # Redraw the old lasso to clear it.
            self.draw_lasso_gl()
        else:
            self.first_draw = False

        # Get the current position and append it to the lasso.
        current_pos = event.position
        self.append_lasso( current_pos )

        # Draw the new lasso.
        self.draw_lasso_gl()
        self.view.endXorDrawing()

    def doReleaseLegacy( self, event ):
        """
        Handle the mouse release event in the Legacy Viewport.
        """
        # Selects objects within the lass.
        if self.first_draw:
            # Redraw the lasso to clear it.
            self.view.beginXorDrawing(True, True, 1.0, OpenMayaUI.M3dView.kStippleDashed);
            self.draw_lasso_gl()
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
        # Get the current position and append it to the lasso.
        current_pos = event.position
        self.append_lasso( current_pos )

        # Draw the lasso.
        draw_mgr.beginDrawable()
        draw_mgr.setColor( OpenMaya.MColor((1.0, 1.0, 0.0)) )
        for i in range(len(self.lasso)-1):
            draw_mgr.line2d( OpenMaya.MPoint( (self.lasso[i][0], self.lasso[i][1])), \
                             OpenMaya.MPoint((self.lasso[i+1][0], self.lasso[i+1][1])) )

        # Draw a final segment from the last lasso point to the first to close the loop.
        draw_mgr.line2d( OpenMaya.MPoint( (self.lasso[-1][0], self.lasso[-1][1])), \
                         OpenMaya.MPoint((self.lasso[0][0], self.lasso[0][1])) )
        draw_mgr.endDrawable()

    def doEnterRegion( self, event ):
        """
        Handle the enter region event.  This method is called from both VP2.0 and the Legacy Viewport.
        """
        self.setHelpString( LassoToolContext.help_string )

# Initialize the script plug-in
def initializePlugin(plugin):
    pluginFn = OpenMaya.MFnPlugin(plugin)

    try:
        pluginFn.registerContextCommand( LassoToolContextCmd.kPluginCmdName, LassoToolContextCmd.creator)
    except:
        logger.error("Failed to register context command: %s\n" % LassoToolContextCmd.kPluginCmdName)
        raise


# Uninitialize the script plug-in
def uninitializePlugin(plugin):
    pluginFn = OpenMaya.MFnPlugin(plugin)
    try:
        pluginFn.deregisterContextCommand(LassoToolContextCmd.kPluginCmdName)
    except:
        logger.error("Failed to unregister command: %s\n" % LassoToolContextCmd.kPluginCmdName)
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
