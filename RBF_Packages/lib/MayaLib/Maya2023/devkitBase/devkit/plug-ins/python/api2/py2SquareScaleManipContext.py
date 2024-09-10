########################################################################
# 
# DESCRIPTION:
# 
# This example is based on the squareScaleManip example but uses
# a context and context command. If the plug-in context is active,
# selecting geometry will show the manipulator.  Only the right and
# left sides of the square currently modify the geometry if moved.
#
########################################################################
#
# First make sure that py2SquareScaleManipContext.py is in your
# MAYA_PLUG_IN_PATH.  Then, to use the tool, execute the following in the 
# script editor:
#
# from maya import cmds
# cmds.file(new=1,force=1)
# cmds.polySphere()
# cmds.loadPlugin("py2SquareScaleManipContext.py")
# 
# ctx = cmds.py2SquareScaleManipContext(nop=1)
# cmds.setToolTo(ctx)
# cmds.py2SquareScaleManipContext(ctx,q=1,nop=1)
# 
# Once the tool is active, click on the sphere. Move the right and left 
# edges of the square to modify the selected object's scale.
#
# The query should return "Doing absolutely nothing"
# 
########################################################################

from __future__ import division
from builtins import object
import logging
import math
import sys
from maya.api import OpenMaya, OpenMayaUI, OpenMayaRender
from maya import OpenMayaRender as OpenMayaRenderV1

logger = logging.getLogger('py2SquareScaleManipContext')

# tell Maya that we want to use Python API 2.0
maya_useNewAPI = True

#
# Utility classes
#
class PlaneMath(object):
    """
    This utility class represents a mathematical plane and performs intersection
    tests with a line.
    """
    def __init__(self):
        """
        Initialze the member variables of the class.
        """
        self.a = 0.0
        self.b = 0.0
        self.c = 0.0
        self.d = 0.0

    def set_plane( self, point_on_plane, normal_to_plane ):
        """
        Define the plane by supplying a point on the plane and the plane's normal.
        """
        _normal_to_plane = OpenMaya.MVector(normal_to_plane)
        _normal_to_plane.normalize()

        # Calculate a,b,c,d based on input
        self.a = _normal_to_plane.x
        self.b = _normal_to_plane.y
        self.c = _normal_to_plane.z
        self.d = -(self.a*point_on_plane.x + self.b*point_on_plane.y + self.c*point_on_plane.z)

    def intersect( self, line ):
        """
        Intersect the plane with the given line.  Return the intersection point if an intersection
		occurs.  Otherwise, raise an exception.
        """
        denominator = self.a*line[1].x + self.b*line[1].y + self.c*line[1].z

        # Verify that the vector and the plane are not parallel.
        if (denominator < .00001):
            raise Exception

        t = -(self.d + self.a*line[0].x + self.b*line[0].y + self.c*line[0].z) / denominator

        # Calculate the intersection point.
        return line[0] + t * line[1]

class LineMath(object):
    """
    This utility class represents a mathematical line and returns the closest point
    on the line to a given point.
    """
    def __init__(self):
        """
        Initialze the member variables of the class.
        """
        self.point = OpenMaya.MPoint()
        self.direction = OpenMaya.MVector()

    def set_line( self, line_point, line_direction ):
        """
        Define the line by supplying a point on the line and the line's direction.
        """
        self.point = OpenMaya.MPoint(line_point)
        self.direction = OpenMaya.MVector(line_direction)
        self.direction.normalize()

    def closest_point( self, to_point ):
        """
        Determine and return the point on the line which is closest to the given point.
        """
        t = self.direction * ( to_point - self.point )
        return self.point + ( self.direction * t )

class SquareGeometry(object):
    """
    This utility class defines methods for returning the four corner points of a unit square
    in the X-Y plane.
    """
    @staticmethod
    def top_left():
        """
        Return the top left corner of the square.
        """
        return OpenMaya.MPoint(-0.5, 0.5, 0.0)
    @staticmethod
    def top_right():
        """
        Return the top right corner of the square.
        """
        return OpenMaya.MPoint( 0.5, 0.5, 0.0)
    @staticmethod
    def bottom_left():
        """
        Return the bottom left corner of the square.
        """
        return OpenMaya.MPoint(-0.5, -0.5, 0.0)
    @staticmethod
    def bottom_right():
        """
        Return the bottom right corner of the square.
        """
        return OpenMaya.MPoint( 0.5, -0.5, 0.0)

#
# SquareScaleManipulator
#

class SquareScaleManipulator (OpenMayaUI.MPxManipulatorNode):
    """
    This is the subclassed manipulator node.  It scales the selected objects
	in the X direction based on dragging movements by the user.
    """
    kNodeName = 'py2SquareScaleContextManipulator'
    kTypeId = OpenMaya.MTypeId( 0x00081162 )

    def __init__(self):
        """
        Initialize the manipulator member variables.
        """
        OpenMayaUI.MPxManipulatorNode.__init__(self)

        # Setup the plane with a point on the plane along with a normal
        self.point_on_plane = SquareGeometry.top_left()

        # Set plug indicies to a default
        self.top_index = -1
        self.right_index = -1
        self.bottom_index = -1
        self.left_index = -1
        self.top_name = -1
        self.right_name = -1
        self.bottom_name = -1
        self.left_name = -1

        # initialize rotate/translate to a good default
        self.rotate_x = 0.0
        self.rotate_y = 0.0
        self.rotate_z = 0.0
        self.translate_x = 0.0
        self.translate_y = 0.0
        self.translate_z = 0.0

        # Normal = cross product of two vectors on the plane
        v1 = OpenMaya.MVector(SquareGeometry.top_left())  - OpenMaya.MVector(SquareGeometry.top_right())
        v2 = OpenMaya.MVector(SquareGeometry.top_right()) - OpenMaya.MVector(SquareGeometry.bottom_right())
        self.normal_to_plane = v1 ^ v2

        # Necessary to normalize
        self.normal_to_plane.normalize()
        self.plane = PlaneMath()
        self.plane.set_plane( self.point_on_plane, self.normal_to_plane )
            

    @classmethod
    def creator(cls):
        return cls()

    @classmethod
    def initialize(cls):
        pass

    # virtual
    def postConstructor(self):
        self.top_index = self.addDoubleValue( 'topValue', 0 )
        self.right_index = self.addDoubleValue( 'rightValue', 0 )
        self.bottom_index = self.addDoubleValue( 'bottomValue', 0 )
        self.left_index = self.addDoubleValue( 'leftValue', 0 )

        gl_pickable_item = self.glFirstHandle()
        self.top_name = gl_pickable_item
        self.bottom_name = gl_pickable_item + 1
        self.right_name = gl_pickable_item + 2
        self.left_name = gl_pickable_item + 3

    # virtual
    def connectToDependNode(self, depend_node):
        """
        Connect the manipulator to the given dependency node.
        """

        # Make sure we have a scaleX plug and connect the
        # plug to the rightIndex we created in the postConstructor
        scale_x_plug = None
        nodeFn = OpenMaya.MFnDependencyNode(depend_node)

        try:
            scale_x_plug = nodeFn.findPlug('scaleX', True)
        except:
            logger.info("    Could not find scaleX plug!")
            return

        plug_index = 0
        try:
            plug_index = self.connectPlugToValue(scale_x_plug, self.right_index)
        except:
            logger.info("    Could not connectPlugToValue!")
            return

        self.finishAddingManips()
        return OpenMayaUI.MPxManipulatorNode.connectToDependNode(self, depend_node)

    def pre_draw(self):
        """
        Update the region dragged by the mouse.
        """

        # Populate the point arrays which are in local space
        tl = SquareGeometry.top_left()
        tr = SquareGeometry.top_right()
        bl = SquareGeometry.bottom_left()
        br = SquareGeometry.bottom_right()

        # Depending on what's active, we modify the
        # end points with mouse deltas in local space
        active = self.glActiveName()
        if active:
            if ( active == self.top_name ):
                tl += self.mouse_point_gl_name
                tr += self.mouse_point_gl_name
            if ( active == self.bottom_name ):
                bl += self.mouse_point_gl_name
                br += self.mouse_point_gl_name
            if ( active == self.right_name ):
                tr += self.mouse_point_gl_name
                br += self.mouse_point_gl_name
            if ( active == self.left_name ):
                tl += self.mouse_point_gl_name
                bl += self.mouse_point_gl_name

        return [tl, tr, bl, br]

    # virtual
    def draw(self, view, path, style, status):
        """
        Draw the manupulator in a legacy viewport.
        """

        # drawing in VP1 views will be done using V1 Python APIs:
        gl_renderer = OpenMayaRenderV1.MHardwareRenderer.theRenderer()
        gl_ft = gl_renderer.glFunctionTable()

        [tl, tr, bl, br] = self.pre_draw()

        # Begin the drawing
        view.beginGL()

        # Push the matrix and set the translate/rotate. Perform
        # operations in reverse order
        gl_ft.glMatrixMode( OpenMayaRenderV1.MGL_MODELVIEW )
        gl_ft.glPushMatrix()
        gl_ft.glTranslatef( self.translate_x, self.translate_y, self.translate_z )
        gl_ft.glRotatef( math.degrees(self.rotate_z), 0.0, 0.0, 1.0 )
        gl_ft.glRotatef( math.degrees(self.rotate_y), 0.0, 1.0, 0.0 )
        gl_ft.glRotatef( math.degrees(self.rotate_x), 1.0, 0.0, 0.0 )

        # Top
        # Place before you draw the manipulator component that can be pickable.
        self.colorAndName( view, self.top_name, False, self.mainColor() )
        gl_ft.glBegin( OpenMayaRenderV1.MGL_LINES )
        gl_ft.glVertex3f( tl.x, tl.y, tl.z )
        gl_ft.glVertex3f( tr.x, tr.y, tr.z )
        gl_ft.glEnd()

        # Right
        self.colorAndName( view, self.right_name, True, self.mainColor() )
        gl_ft.glBegin( OpenMayaRenderV1.MGL_LINES )
        gl_ft.glVertex3f( tr.x, tr.y, tr.z )
        gl_ft.glVertex3f( br.x, br.y, br.z )
        gl_ft.glEnd()

        # Bottom
        self.colorAndName( view, self.bottom_name, False, self.mainColor() )
        gl_ft.glBegin( OpenMayaRenderV1.MGL_LINES )
        gl_ft.glVertex3f( br.x, br.y, br.z )
        gl_ft.glVertex3f( bl.x, bl.y, bl.z )
        gl_ft.glEnd()

        # Left
        self.colorAndName( view, self.left_name, True, self.mainColor() )
        gl_ft.glBegin( OpenMayaRenderV1.MGL_LINES )
        gl_ft.glVertex3f( bl.x, bl.y, bl.z )
        gl_ft.glVertex3f( tl.x, tl.y, tl.z )
        gl_ft.glEnd()

        # Pop matrix
        gl_ft.glPopMatrix()

        # End the drawing
        view.endGL()

    # virtual
    def preDrawUI(self, view):
        """
        Cache the viewport for use in VP 2.0 drawing.
        """
        pass

    # virtual
    def drawUI(self, draw_manager, frame_context):
        """
        Draw the manupulator in a VP 2.0 viewport.
        """

        [tl, tr, bl, br] = self.pre_draw()

        xform = OpenMaya.MTransformationMatrix()
        xform.rotateByComponents([math.degrees(self.rotate_x), \
		                          math.degrees(self.rotate_y), \
		                          math.degrees(self.rotate_z), \
		                          OpenMaya.MTransformationMatrix.kZYX], \
		                          OpenMaya.MSpace.kWorld)

        mat = xform.asMatrix()
        tl *= mat
        tr *= mat
        bl *= mat
        br *= mat

        # Top
        draw_manager.beginDrawable(OpenMayaRender.MUIDrawManager.kNonSelectable, self.top_name)
        self.setHandleColor(draw_manager, self.top_name, self.dimmedColor())
        draw_manager.line(tl, tr)
        draw_manager.endDrawable()

        # Right
        draw_manager.beginDrawable(OpenMayaRender.MUIDrawManager.kSelectable, self.right_name)
        self.setHandleColor(draw_manager, self.right_name, self.mainColor())
        draw_manager.line(tr, br)
        draw_manager.endDrawable()

        # Bottom
        draw_manager.beginDrawable(OpenMayaRender.MUIDrawManager.kNonSelectable, self.bottom_name)
        self.setHandleColor(draw_manager, self.bottom_name, self.dimmedColor())
        draw_manager.line(br, bl)
        draw_manager.endDrawable()

        # Left
        draw_manager.beginDrawable(OpenMayaRender.MUIDrawManager.kSelectable, self.left_name)
        self.setHandleColor(draw_manager, self.left_name, self.mainColor())
        draw_manager.line(bl, tl)
        draw_manager.endDrawable()

    # virtual
    def doPress( self, view ):
        """
        Handle the mouse press event in a VP2.0 viewport.
        """
        # Reset the mousePoint information on a new press.
        self.mouse_point_gl_name = OpenMaya.MPoint.kOrigin
        self.update_drag_information()

    # virtual
    def doDrag( self, view ):
        """
        Handle the mouse drag event in a VP2.0 viewport.
        """
        self.update_drag_information()

    # virtual
    def doRelease( self, view ):
        """
        Handle the mouse release event in a VP2.0 viewport.
        """
        pass

    def set_draw_transform_info( self, rotation, translation ):
        """
        Store the given rotation and translation.
        """
        self.rotate_x = rotation[0]
        self.rotate_y = rotation[1]
        self.rotate_z = rotation[2]
        self.translate_x = translation[0]
        self.translate_y = translation[1]
        self.translate_z = translation[2]

    def update_drag_information( self ):
        """
        Update the mouse's intersection location with the manipulator
        """
        # Find the mouse point in local space
        self.local_mouse = self.mouseRay()

        # Find the intersection of the mouse point with the manip plane
        mouse_intersection_with_manip_plane = self.plane.intersect( self.local_mouse )

        self.mouse_point_gl_name = mouse_intersection_with_manip_plane

        active = self.glActiveName()
        if active:
            start = OpenMaya.MPoint([0, 0, 0])
            end = OpenMaya.MPoint([0, 0, 0])
            if ( active == self.top_name ):
                start = OpenMaya.MPoint(-0.5, 0.5, 0.0)
                end   = OpenMaya.MPoint( 0.5, 0.5, 0.0)
            if ( active == self.bottom_name ):
                start = OpenMaya.MPoint(-0.5, -0.5, 0.0)
                end   = OpenMaya.MPoint( 0.5, -0.5, 0.0)
            if ( active == self.right_name ):
                start = OpenMaya.MPoint( 0.5, 0.5, 0.0)
                end   = OpenMaya.MPoint( 0.5, -0.5, 0.0)
            if ( active == self.left_name ):
                start = OpenMaya.MPoint(-0.5, 0.5, 0.0)
                end   = OpenMaya.MPoint(-0.5, -0.5, 0.0)

            if ( active ):
                # Find a vector on the plane
                a = OpenMaya.MPoint( start.x, start.y, start.z )
                b = OpenMaya.MPoint( end.x, end.y, end.z )
                vab = a - b

                # Define line with a point and a vector on the plane
                line = LineMath()
                line.set_line( start, vab )

                # Find the closest point so that we can get the
                # delta change of the mouse in local space
                cpt = line.closest_point( self.mouse_point_gl_name )
                self.mouse_point_gl_name -= cpt
 
                min_change_value = min( self.mouse_point_gl_name.x, self.mouse_point_gl_name.y, self.mouse_point_gl_name.z )
                max_change_value = max( self.mouse_point_gl_name.x, self.mouse_point_gl_name.y, self.mouse_point_gl_name.z )
                if ( active == self.right_name ):
                    self.setDoubleValue( self.right_index, max_change_value )
                if ( active == self.left_name ):
                    self.setDoubleValue( self.right_index, min_change_value )


# command
class SquareScaleManipContextCmd (OpenMayaUI.MPxContextCommand):
    """
    This command class is used to create instances of the SquareScaleManipContext class.
    """
    kPluginCmdName = "py2SquareScaleManipContext"

    kNopFlag = "-nop"
    kNopLongFlag = "-noOperation"

    def __init__(self):
        OpenMayaUI.MPxContextCommand.__init__(self)

    @staticmethod
    def creator():
        return SquareScaleManipContextCmd()

    def doQueryFlags(self):
        theParser = self.parser()
        if( theParser.isFlagSet(SquareScaleManipContextCmd.kNopFlag) ):
            print("Doing absolutely nothing")
        return

    def makeObj(self):
        """
        Create and return an instance of the SquareScaleManipContext class.
        """
        return SquareScaleManipContext()

    def appendSyntax(self):
        theSyntax = self.syntax()
        theSyntax.addFlag(SquareScaleManipContextCmd.kNopFlag, SquareScaleManipContextCmd.kNopLongFlag)


class SquareScaleManipContext(OpenMayaUI.MPxSelectionContext):
    """
    This context handles all mouse interaction in the viewport when activated.
	When activated, it creates and manages an instance of the SquareScaleManuplator
    class on the selected objects.
    """

    kContextName = 'SquareScaleManipContext'

    @classmethod
    def creator(cls):
        return cls()

    def __init__(self):
        """
        Initialize the members of the SquareScaleManipContext class.
        """
        OpenMayaUI.MPxSelectionContext.__init__(self)
        self.setTitleString('Plug-in manipulator: ' + SquareScaleManipContext.kContextName)
        self.manipulator_class_ptr = None
        self.first_object_selected = None
        self.active_list_modified_msg_id = -1

    # virtual
    def toolOnSetup(self, event):
        """
        Set the help string and selection list callback.
        """
        self.setHelpString('Move the object using the manipulator')

        SquareScaleManipContext.update_manipulators_cb(self)
        try:
            self.active_list_modified_msg_id = OpenMaya.MModelMessage.addCallback( \
			                             OpenMaya.MModelMessage.kActiveListModified, \
										 SquareScaleManipContext.update_manipulators_cb, self)
        except:
            OpenMaya.MGlobal.displayError("SquareScaleManipContext.toolOnSetup(): Model addCallback failed")

    # Removes the callback
    # virtual
    def toolOffCleanup(self):
        """
        Unregister the selection list callback.
        """
        try:
            OpenMaya.MModelMessage.removeCallback(self.active_list_modified_msg_id)
            self.active_list_modified_msg_id = -1
        except:
            OpenMaya.MGlobal.displayError("SquareScaleManipContext.toolOffCleanup(): Model remove callback failed")

        OpenMayaUI.MPxSelectionContext.toolOffCleanup(self)

    # virtual
    def namesOfAttributes(self, attribute_names):
        """
        Return the names of the attributes of the selected objects this context will be modifying.
        """
        attribute_names.append('scaleX')

    # virtual
    def setInitialState(self):
        """
        Set the initial transform of the manipulator.
        """
        xform = OpenMaya.MFnTransform( self.first_object_selected )
        xformMatrix = xform.transformation()
        translation = xformMatrix.translation( OpenMaya.MSpace.kWorld )
        rotation = xformMatrix.rotation(False)

        self.manipulator_class_ptr.set_draw_transform_info( rotation, translation )

    # Ensure that valid geometry is selected
    def valid_geometry_selected(self):
        """
        Check to make sure the selected objects have transforms.
        """
        list = None
        iter = None
        try:
            list = OpenMaya.MGlobal.getActiveSelectionList()
            iter = OpenMaya.MItSelectionList(list)
        except:
            logger.info("    Could not get active selection")
            return False

        if (not list) or (list.length() == 0):
            return False

        while not iter.isDone():
            depend_node = iter.getDependNode()
            if (depend_node.isNull() or (not depend_node.hasFn(OpenMaya.MFn.kTransform))):
                OpenMaya.MGlobal.displayWarning('Object in selection list is not right type of node')
                return False
            next(iter)
        return True

    def update_manipulators_cb(ctx):
        """
        Callback that creates the manipulator if valid geometry is selected. Also removes
        the manipulator if no geometry is selected. Handles connecting the manipulator to
        multiply selected nodes.
        """
        try:
            ctx.deleteManipulators()
        except:
            logger.info("    No manipulators to delete")

        try:
            if not ctx.valid_geometry_selected():
                return

            # Clear info
            ctx.manipulator_class_ptr = None
            ctx.first_object_selected = OpenMaya.MObject.kNullObj

            (manipulator, manip_object) = SquareScaleManipulator.newManipulator('py2SquareScaleContextManipulator')

            if manipulator:
                # Save state
                ctx.manipulator_class_ptr = manipulator

                # Add the manipulator
                ctx.addManipulator(manip_object)

                list = OpenMaya.MGlobal.getActiveSelectionList()
                iter = OpenMaya.MItSelectionList(list)

                while not iter.isDone():
                    depend_node = iter.getDependNode()
                    depend_node_fn = OpenMaya.MFnDependencyNode(depend_node)

                    # Connect the manipulator to the object in the selection list.
                    if (not manipulator.connectToDependNode(depend_node)):
                        OpenMaya.MGlobal.displayWarning('Error connecting manipulator to object %s' % depend_node_fn.name())
                        next(iter)
                        continue

                    if ( ctx.first_object_selected == OpenMaya.MObject.kNullObj ):
                        ctx.first_object_selected = depend_node
                    next(iter)

                # Allow the manipulator to set initial state
                ctx.setInitialState()

        except:
            OpenMaya.MGlobal.displayError('Failed to create new manipulator')
            return

    update_manipulators_cb = staticmethod(update_manipulators_cb)


# Initialize the script plug-in
def initializePlugin(plugin):
    pluginFn = OpenMaya.MFnPlugin(plugin)

    try:
        pluginFn.registerContextCommand( SquareScaleManipContextCmd.kPluginCmdName, SquareScaleManipContextCmd.creator)
    except:
        sys.stderr.write("Failed to register context command: %s\n" % SquareScaleManipContextCmd.kPluginCmdName)
        raise

    try:
        pluginFn.registerNode( SquareScaleManipulator.kNodeName, SquareScaleManipulator.kTypeId, \
                               SquareScaleManipulator.creator, SquareScaleManipulator.initialize, \
                               OpenMaya.MPxNode.kManipulatorNode)
    except:
        sys.stderr.write("Failed to register node: %s\n" % SquareScaleManipulator.kNodeName)
        raise


# Uninitialize the script plug-in
def uninitializePlugin(plugin):
    pluginFn = OpenMaya.MFnPlugin(plugin)
    try:
        pluginFn.deregisterContextCommand(SquareScaleManipContextCmd.kPluginCmdName)
    except:
        sys.stderr.write(
            "Failed to unregister command: %s\n" % SquareScaleManipContextCmd.kPluginCmdName)
        raise

    try:
        pluginFn.deregisterNode(SquareScaleManipulator.kTypeId)
    except:
        sys.stderr.write(
            "Failed to unregister node: %s\n" % SquareScaleManipulator.kNodeName)
        raise

# =======================================================================
# Copyright 2020 Autodesk, Inc. All rights reserved.
#
# This computer source code and related instructions and comments are the
# unpublished confidential  and proprietary information of Autodesk, Inc.
# and are protected under applicable copyright and trade secret law. They 
# may not be disclosed to, copied  or used by any third party without the 
# prior written consent of Autodesk, Inc.
# =======================================================================
