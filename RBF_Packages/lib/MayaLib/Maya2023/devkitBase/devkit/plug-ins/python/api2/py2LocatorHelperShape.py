from __future__ import division
###############################################################################
#
# pyLocatorHelperShape.py
#
# A plug-in locator with configurable shape representations. The locator
# supports reading in a library of shape representations serialized as
# JSON files, that populates its 'shapes' enumeration attribute.
#
# The plug-in provides a command to serialize combinations of meshes
# and curves into the library folder.
#
## Usage ##
#
#   1. Create a locatorHelperShape node. By default, the node will have
#      a default representation consisting of a mesh cube and
#      diamond shaped curve.
#
#       import maya.cmds as cmds
#       cmds.createNode('locatorHelperShape')
#
#   2. Populate the locatorHelperShape library.
#
#      By default, the library is located at $(PLUGIN_PATH)/library/.
#      Alternately, you can set the library location using the
#      'MAYA_LOCATOR_HELPER_SHAPE_LIB' environment variable. This must be set
#      prior to loading the plug-in.
#
#       import maya.cmds as cmds
#
#       # Create a sphere representation
#       sph = cmds.polySphere(name='sphere')[0]
#       cmds.locatorHelperShapeExport(sph, force=True, name=sph)
#
#       # Create a representation with multiple shapes
#       c = cmds.circle(name='circle')[0]
#       s = cmds.nurbsSquare(name='square')[0]
#       cmds.locatorHelperShapeExport(s, c, force=True, name='combo')
#
#       # Can alternately create using the selection list
#       cmds.select(c, s, r=True)
#       cmds.locatorHelperShapeExport(force=True, name='combo')
#
#       # Can adjust the number of samples to take from the curve
#       # when tessellating them into line segments. Default is 50.
#       cmds.locatorHelperShapeExport(force=True, name='combo', curveSamples=100)
#
#       # Shapes are exported in object space. Freeze
#       # transforms as necessary to get the desired output.
#       cmds.scale(1, 2, 1, c)
#       cmds.makeIdentity(c, apply=True,t=1,r=1,s=1,n=0,pn=1)
#       cmds.locatorHelperShapeExport(c, force=True, name='oval')
#
#   3. Reload the plug-in to see the new shapes. You may need to
#      reload the Attribute Editor to view the updated enumeration.
#      You can reload the AE by:
#
#       a. Select a locatorHelperShape node.
#       b. In the AE, navigate the AE menu: 'Show > Set Global View > Lookdev'
#       b. Restore the AE global view: 'Show > Set Global View > Default'
#
## Overview ##
#
# The plug-in can be broken down into the following components:
#
#   1. helperShapeRep / helperShapeRepItem
#
#       Defines the shape representations and are responsible for
#       serialization and generation of the vertex/index buffers
#       for display.
#
#   2. helperShapeExportCmd
#
#       Defines the command to convert a list or selection of meshes
#       and/or curves into helperShapeRepItems for serialization.
#
#   3. helperShapeNode
#
#       Defines the plug-in locator node.
#
#   4. helperShapeSubSceneOverride
#
#       Defines the VP2 override implementation for rendering the
#       helperShapeNode.
#
from builtins import range
from builtins import object
from builtins import zip
from builtins import next
import os
import sys
import ctypes
import json

import maya.cmds as cmds
import maya.mel as mel
import maya.api.OpenMaya as om
import maya.api.OpenMayaRender as omr
import maya.api.OpenMayaUI as omui

maya_useNewAPI = True

###############################################################################
#
# Constants
#
DEFAULT_COLOR = (0.0, 0.7, 0.15)
DEFAULT_LINE_WIDTH = 1.0

HELPER_SHAPE_SELECTION_MASK = 'locatorHelperShapeSelection'
HELPER_SHAPE_SELECTION_PRIORITY = 9

# SubScene Consolidation support
ENABLE_CONSOLIDATION = True
consolidation_func = getattr(omr.MRenderItem, 'wantSubSceneConsolidation', None)
SUPPORTS_CONSOLIDATION = callable(consolidation_func)
WANT_CONSOLIDATION = SUPPORTS_CONSOLIDATION and ENABLE_CONSOLIDATION

###############################################################################
#
# Utility functions
#
def isclose(a, b, rel_tol=1e-9, abs_tol=0.0):
    return abs(a-b) <= max( rel_tol * max(abs(a), abs(b)), abs_tol )

def isclose_tuple(a, b, rel_tol=1e-9, abs_tol=0.0):
    for (aa, bb) in zip(a, b):
        if not isclose(aa, bb):
            return False
    return True


###############################################################################
#
# helperShapeRep / helperShapeRepItem
#
#   The helperShapeRep* classes define the abstraction for the shapes that
#   helperShapeNode can display. These classes are responsible for:
#
#   1. Owning the data that defines the shapes.
#   2. Computing the associated vertex and index buffers.
#   3. Serializing/Deserializing to Python dictionaries.
#
class helperShapeRepItemType(object):
    ''' helperShape representation item type definition. '''
    kMesh = None
    kCurve = None
    
    def __init__(self, index, name):
        self.index = index
        self.name = name
        
    def __eq__(self, other):
        return self.index == other.index
        
    def __ne__(self, other):
        return not(self == other)
        
    def __int__(self):
        return self.index
        
    def __str__(self):
        return self.name
        
# Populate our helperShapeRepItem type variables.
helperShapeRepItemType.kMesh = helperShapeRepItemType(0, 'mesh')
helperShapeRepItemType.kCurve = helperShapeRepItemType(1, 'curve')

class helperShapeRepItem(object):
    ''' Abstract base class for a helperShape representation item. '''
    kTypeKey = 'type'
    
    def __init__(self, type):
        ''' Constructor. '''
        self.type = type
        
    @classmethod
    def deserialize_type(cls, data):
        ''' Deserialize the item type from a Python dictionary '''
        if data[cls.kTypeKey] == helperShapeRepItemType.kMesh.name:
            return helperShapeRepItemType.kMesh
        elif data[cls.kTypeKey] == helperShapeRepItemType.kCurve.name:
            return helperShapeRepItemType.kCurve
        raise RuntimeError('Invalid helperShapeRepItemType: {}'.format(data[cls.kTypeKey]))
        
    def deserialize(self, data):
        ''' Deserialize the item from a Python dictionary '''
        raise NotImplementedError
        
    def serialize(self, data):
        ''' Serialize the item to a Python dictionary '''
        if self.type == helperShapeRepItemType.kMesh:
            data[self.kTypeKey] = helperShapeRepItemType.kMesh.name
        elif self.type == helperShapeRepItemType.kCurve:
            data[self.kTypeKey] = helperShapeRepItemType.kCurve.name
        
    def vertex_buffer(self):
        ''' Returns an MVertexBufferArray containing the vertex buffers for this item. '''
        raise NotImplementedError
        
    def wire_index_buffer(self):
        ''' Returns an MIndexBuffer containing the index buffer for wireframe draw. '''
        raise NotImplementedError
        
    def shaded_index_buffer(self):
        ''' Returns an MIndexBuffer containing the index buffer for shaded draw. '''
        raise NotImplementedError
        
    def clear_buffers(self):
        ''' Clears cached vertex/index buffers for this item. '''
        raise NotImplementedError
        
    def bounding_box(self):
        ''' Returns an MBoundingBox for this item. '''
        raise NotImplementedError
        
    @classmethod
    def _create_index_buffer(cls, indices):
        ''' Returns an MIndexBuffer populated with the given indices. '''
        num_indices = len(indices)
        buffer = omr.MIndexBuffer(omr.MGeometry.kUnsignedInt32)
        address = buffer.acquire(num_indices, True)
        data = ((ctypes.c_uint)*num_indices).from_address(address)
        for (i,val) in enumerate(indices):
            data[i] = val
        buffer.commit(address)
        return buffer
        
    @classmethod
    def _create_vertex_buffer(cls, desc, vertex_data):
        ''' Returns an MVertexBuffer populated with the given vertex data. '''
        num_vertices = len(vertex_data)
        buffer = omr.MVertexBuffer(desc)
        address = buffer.acquire(num_vertices, True)
        if desc.dataType == omr.MGeometry.kFloat and desc.dimension == 3:
            data = ((ctypes.c_float * 3)*num_vertices).from_address(address)
            for (i,val) in enumerate(vertex_data):
                data[i][0] = val[0]
                data[i][1] = val[1]
                data[i][2] = val[2]
            buffer.commit(address)
        else:
            # This plug-in only requires working with float3 data.
            raise NotImplementedError
        return buffer

class helperShapeRepItemMesh(helperShapeRepItem):
    ''' Defines a mesh based helperShape representation item. '''
    
    kVerticesKey = 'vertices'
    kNormalsKey = 'normals'
    kWireIndicesKey = 'wireIndices'
    kShadedIndicesKey = 'shadedIndices'
    
    def __init__(self):
        ''' Constructor. '''
        super(helperShapeRepItemMesh, self).__init__(helperShapeRepItemType.kMesh)
        
        self.position_desc = omr.MVertexBufferDescriptor('', omr.MGeometry.kPosition, omr.MGeometry.kFloat, 3)
        self.normal_desc = omr.MVertexBufferDescriptor('', omr.MGeometry.kNormal, omr.MGeometry.kFloat, 3)
        
        self.vertices = None
        self.normals = None
        
        self.wire_indices = None
        self.shaded_indices = None
        
        self.position_buffer = None
        self.normal_buffer = None
        self.index_buffer_wire = None
        self.index_buffer_shaded = None
        
    def deserialize(self, data):
        ''' Deserialize the item from a Python dictionary '''
        self.vertices = data[self.kVerticesKey]
        self.normals = data[self.kNormalsKey]
        self.wire_indices = data[self.kWireIndicesKey]
        self.shaded_indices = data[self.kShadedIndicesKey]
        
    def serialize(self, data):
        ''' Serialize the item to a Python dictionary '''
        super(helperShapeRepItemMesh, self).serialize(data)
        data[self.kVerticesKey] = self.vertices
        data[self.kNormalsKey] = self.normals
        data[self.kWireIndicesKey] = self.wire_indices
        data[self.kShadedIndicesKey] = self.shaded_indices
        
    def vertex_buffer(self):
        ''' Returns an MVertexBufferArray containing the vertex buffers for this item. '''
        if not self.position_buffer:
            self.position_buffer = self._create_vertex_buffer(self.position_desc, self.vertices)
        if not self.normal_buffer:
            self.normal_buffer = self._create_vertex_buffer(self.normal_desc, self.normals)
        vertex_buffer = omr.MVertexBufferArray()
        vertex_buffer.append(self.position_buffer, 'positions')
        vertex_buffer.append(self.normal_buffer, 'normals')
        return vertex_buffer
        
    def wire_index_buffer(self):
        ''' Returns an MIndexBuffer containing the index buffer for wireframe draw. '''
        if not self.index_buffer_wire:
            self.index_buffer_wire = self._create_index_buffer(self.wire_indices)
        return self.index_buffer_wire
        
    def shaded_index_buffer(self):
        ''' Returns an MIndexBuffer containing the index buffer for shaded draw. '''
        if not self.index_buffer_shaded:
            self.index_buffer_shaded = self._create_index_buffer(self.shaded_indices)
        return self.index_buffer_shaded
        
    def clear_buffers(self):
        ''' Clears cached vertex/index buffers for this item. '''
        self.position_buffer = None
        self.normal_buffer = None
        self.index_buffer_wire = None
        self.index_buffer_shaded = None
        
    def bounding_box(self):
        ''' Returns an MBoundingBox for this item. '''
        box = om.MBoundingBox()
        for v in self.vertices:
            box.expand(om.MPoint(v[0], v[1], v[2]))
        return box
        
class helperShapeRepItemCurve(helperShapeRepItem):
    ''' Defines a curve based helperShape representation item. '''
    
    kVerticesKey = 'vertices'
    kIndicesKey = 'indices'
    
    def __init__(self):
        ''' Constructor. '''
        super(helperShapeRepItemCurve, self).__init__(helperShapeRepItemType.kCurve)
        
        self.position_desc = omr.MVertexBufferDescriptor('', omr.MGeometry.kPosition, omr.MGeometry.kFloat, 3)
        
        self.vertices = None
        self.indices = None
        
        self.position_buffer = None
        self.index_buffer = None
        
    def deserialize(self, data):
        ''' Deserialize the item from a Python dictionary '''
        self.vertices = data[self.kVerticesKey]
        self.indices = data[self.kIndicesKey]
        
    def serialize(self, data):
        ''' Serialize the item to a Python dictionary '''
        super(helperShapeRepItemCurve, self).serialize(data)
        data[self.kVerticesKey] = self.vertices
        data[self.kIndicesKey] = self.indices
        
    def vertex_buffer(self):
        ''' Returns an MVertexBufferArray containing the vertex buffers for this item. '''
        if not self.position_buffer:
            self.position_buffer = self._create_vertex_buffer(self.position_desc, self.vertices)
        vertex_buffer = omr.MVertexBufferArray()
        vertex_buffer.append(self.position_buffer, 'positions')
        return vertex_buffer
        
    def wire_index_buffer(self):
        ''' Returns an MIndexBuffer containing the index buffer for wireframe draw. '''
        if not self.index_buffer:
            self.index_buffer = self._create_index_buffer(self.indices)
        return self.index_buffer
        
    def clear_buffers(self):
        ''' Clears cached vertex/index buffers for this item. '''
        self.position_buffer = None
        self.index_buffer = None
        
    def bounding_box(self):
        ''' Returns an MBoundingBox for this item. '''
        box = om.MBoundingBox()
        for v in self.vertices:
            box.expand(om.MPoint(v[0], v[1], v[2]))
        return box
        
class helperShapeRepItemCube(helperShapeRepItemMesh):
    ''' A sample default cube mesh representation item. '''
    def __init__(self):
        super(helperShapeRepItemCube, self).__init__()
        
        # Exploded vertex layout (unshared normals)
        self.vertices = [
            #top
            (-0.5, 0.5,-0.5),
            (-0.5, 0.5, 0.5),
            ( 0.5, 0.5, 0.5),
            ( 0.5, 0.5,-0.5),
            
            #left
            (-0.5,-0.5,-0.5),
            (-0.5,-0.5, 0.5),
            (-0.5, 0.5, 0.5),
            (-0.5, 0.5,-0.5),
            
            #bottom
            ( 0.5,-0.5, 0.5),
            (-0.5,-0.5, 0.5),
            (-0.5,-0.5,-0.5),
            ( 0.5,-0.5,-0.5),
            
            #right
            ( 0.5,-0.5,-0.5),
            ( 0.5, 0.5,-0.5),
            ( 0.5, 0.5, 0.5),
            ( 0.5,-0.5, 0.5),
            
            #front
            (-0.5,-0.5, 0.5),
            ( 0.5,-0.5, 0.5),
            ( 0.5, 0.5, 0.5),
            (-0.5, 0.5, 0.5),
            
            #back
            (-0.5,-0.5,-0.5),
            (-0.5, 0.5,-0.5),
            ( 0.5, 0.5,-0.5),
            ( 0.5,-0.5,-0.5)
        ]
        
        self.normals = [
            # top
            ( 0.0, 1.0, 0.0),
            ( 0.0, 1.0, 0.0),
            ( 0.0, 1.0, 0.0),
            ( 0.0, 1.0, 0.0),
            
            #left
            (-1.0, 0.0, 0.0),
            (-1.0, 0.0, 0.0),
            (-1.0, 0.0, 0.0),
            (-1.0, 0.0, 0.0),
            
            #bottom
            ( 0.0,-1.0, 0.0),
            ( 0.0,-1.0, 0.0),
            ( 0.0,-1.0, 0.0),
            ( 0.0,-1.0, 0.0),
            
            #right
            ( 1.0, 0.0, 0.0),
            ( 1.0, 0.0, 0.0),
            ( 1.0, 0.0, 0.0),
            ( 1.0, 0.0, 0.0),
            
            #front
            ( 0.0, 0.0, 1.0),
            ( 0.0, 0.0, 1.0),
            ( 0.0, 0.0, 1.0),
            ( 0.0, 0.0, 1.0),
            
            #back
            ( 0.0, 0.0,-1.0),
            ( 0.0, 0.0,-1.0),
            ( 0.0, 0.0,-1.0),
            ( 0.0, 0.0,-1.0)
        ]
        
        num_prim = len(self.vertices)
        num_index = num_prim * 2
        self.wire_indices = [None] * num_index
        i = 0
        while i < num_index:
            start_index = i // 8 * 4  # 8 indices per face. 4 verts per face in the vertexBuffer
            pairs = [(0,1), (1,2), (2,3), (3,0)]
            for (index, p) in enumerate(pairs):
                self.wire_indices[i + index*2]     = start_index + p[0]
                self.wire_indices[i + index*2 + 1] = start_index + p[1]
            i += 8
        
        num_prim = len(self.vertices)
        num_index = num_prim // 4 * 6     # 4 verts per quad, 6 indices per quad (2 tris)
        self.shaded_indices = [None] * num_index
        i = 0
        while i < num_index:
            start_index = i // 6 * 4
            tris = [(0, 1, 2), (0, 2, 3)]
            for (index, t) in enumerate(tris):
                self.shaded_indices[i + index*3]     = start_index + t[0]
                self.shaded_indices[i + index*3 + 1] = start_index + t[1]
                self.shaded_indices[i + index*3 + 2] = start_index + t[2]
            i += 6
            
class helperShapeRepItemDiamond(helperShapeRepItemCurve):
    ''' A sample default diamond curve representation item. '''
    def __init__(self):
        super(helperShapeRepItemDiamond, self).__init__()
        self.vertices = [
            ( 1.0, 0.0, 0.0),
            ( 0.0, 0.0, 1.0),
            (-1.0, 0.0, 0.0),
            ( 0.0, 0.0,-1.0)
        ]
        self.indices = [0,1,1,2,2,3,3,0]

class helperShapeRep(object):
    '''
    Defines the helperShape representation class.
    A representation consists of a list of representation items.
    '''
    
    kRenderItemsKey = 'renderItems'
    
    def __init__(self):
        self.name = None
        self.items = []
        
    def deserialize(self, file_path):
        ''' Deserialize the representation from JSON. '''
        self.name = os.path.splitext(os.path.basename(file_path))[0]
        with open(file_path, 'r') as f:
            data = json.load(f)
            
        # Validate data
        if not isinstance(data, dict):
            raise RuntimeError('Invalid JSON helperShape file: %s'.format(file_path))
        if not self.kRenderItemsKey in data or not isinstance(data[self.kRenderItemsKey], list):
            raise RuntimeError('Invalid [renderItems] key in file: %s'.format(file_path))
            
        # Read data
        for item_data in data[self.kRenderItemsKey]:
            type = helperShapeRepItem.deserialize_type(item_data)
            if type == helperShapeRepItemType.kMesh:
                item = helperShapeRepItemMesh()
            elif type == helperShapeRepItemType.kCurve:
                item = helperShapeRepItemCurve()
            else:
                continue
            item.deserialize(item_data)
            self.items.append(item)
        
    def serialize(self, file_path):
        ''' Serialize the representation to JSON. '''
        data = {}
        data[self.kRenderItemsKey] = []
        for item in self.items:
            item_data = {}
            item.serialize(item_data)
            data[self.kRenderItemsKey].append(item_data)
        with open(file_path, 'w') as f:
            json.dump( data, f, sort_keys=True, indent=4, separators=[',', ': '] )
        
class helperShapeRepDefault(helperShapeRep):
    '''
    A sample default helper representation consisting of a cube mesh
    and diamond curve.
    '''
    def __init__(self):
        super(helperShapeRepDefault, self).__init__()
        self.name = 'default'
        self.items.append( helperShapeRepItemCube() )
        self.items.append( helperShapeRepItemDiamond() )


###############################################################################
#
# helperShapeExportCmd (MPxCommand)
#
class helperShapeExportCmd(om.MPxCommand):
    ''' helperShapeExportCmd command class. '''
    name = 'locatorHelperShapeExport'

    kNameFlag               = 'n'
    kNameFlagLong           = 'name'
    kCurveSamplesFlag       = 'cs'
    kCurveSamplesFlagLong   = 'curveSamples'
    kForceFlag              = 'f'
    kForceFlagLong          = 'force'
    
    def __init__(self):
        ''' Constructor. '''
        om.MPxCommand.__init__(self)

        self.name = None
        self.curve_samples = 50
        self.force = False
        
    @staticmethod
    def creator():
        ''' Creator method. '''
        return helperShapeExportCmd()
    
    @staticmethod
    def new_syntax():
        ''' Command syntax definition. '''
        syntax = om.MSyntax()
        syntax.addFlag(helperShapeExportCmd.kNameFlag, helperShapeExportCmd.kNameFlagLong, om.MSyntax.kString)
        syntax.addFlag(helperShapeExportCmd.kCurveSamplesFlag, helperShapeExportCmd.kCurveSamplesFlagLong, om.MSyntax.kUnsigned)
        syntax.addFlag(helperShapeExportCmd.kForceFlag, helperShapeExportCmd.kForceFlagLong)
        syntax.setObjectType(om.MSyntax.kStringObjects, 0)
        return syntax

    @classmethod
    def valid_name(cls, name):
        '''
        Returns true if the name does not already exist in the
        shape library path. False otherwise.
        '''
        for (root, dirs, files) in os.walk(helperShapeNode.shapes_lib_path):
            for f in files:
                (file_name, file_ext) = os.path.splitext(f)
                if not file_ext == '.json':
                    continue
                if name == file_name:
                    return False
        return True

    def get_objects(self, parser):
        '''
        Filters the command object list for meshes/curve shapes.
        If the command object list is empty, filters the selection
        list for meshes/curve shapes.

        Returns the meshes/curve shapes in a list of MObjects.
        '''
        obj_strs = parser.getObjectStrings()
        sel_list = om.MSelectionList()
        if len(obj_strs):
            for o in obj_strs:
                sel_list.add(o)
        else:
            sel_list = om.MGlobal.getActiveSelectionList()

        objects = []
        for i in range(sel_list.length()):
            try:
                path = sel_list.getDagPath(i)
            except TypeError:
                continue

            multiple_shapes = False
            try:
                path.extendToShape()
            except RuntimeError:
                multiple_shapes = True

            if not multiple_shapes:
                if path.apiType() == om.MFn.kMesh or path.apiType() == om.MFn.kNurbsCurve:
                    objects.append(path.node())
            else:
                dag_it = om.MItDag()
                dag_it.reset(path)
                while not dag_it.isDone():
                    item = dag_it.getPath()
                    if item.apiType() == om.MFn.kMesh or item.apiType() == om.MFn.kNurbsCurve:
                        objects.append(item.node())
                    next(dag_it)
        return objects

    def generate_mesh(self, obj):
        '''
        Given a mesh shape MObject, generate a helperShapeRepItemMesh.
        '''
        if not obj.apiType() == om.MFn.kMesh:
            return None

        # Store raw position/normal mesh data as lists of float3 tuples
        mesh_fn = om.MFnMesh(obj)
        raw_vertices_mpts = mesh_fn.getPoints()
        raw_vertices = [ (p.x, p.y, p.z) for p in raw_vertices_mpts ]
        raw_normals_mvcs = mesh_fn.getNormals()
        raw_normals = [ (n.x, n.y, n.z) for n in raw_normals_mvcs ]

        # Output vertex data arrays
        vertices = []
        normals = []
        wire_indices = []
        shaded_indices = []

        # Dictionary with ((position, normal), index) key
        # value pairs to test for duplicate vertex data.
        unique_vertices = {}

        # Mesh vertex IDs to output vertices array
        mesh_vertex_map = {}

        # Iterate over each face to generate kTriangles
        # vertex data.
        face_it = om.MItMeshPolygon(obj)
        while not face_it.isDone():
            count = len(vertices)
            num_face_verts = face_it.polygonVertexCount()

            # Create a vertex id to local face vertex id map
            # to help us correlate face vertex data.
            mesh_to_face_vertex_map = {}
            for i in range(num_face_verts):
                mesh_vid = face_it.vertexIndex(i)
                mesh_to_face_vertex_map[mesh_vid] = i

            # Iterate over each triangle in this face.
            (tri_pts, tri_ids) = face_it.getTriangles()
            num_tris = len(tri_ids) // 3
            for i in range(num_tris):
                for j in range(3):
                    vid = tri_ids[i*3 + j]
                    local_vid = mesh_to_face_vertex_map[vid]
                    nid = face_it.normalIndex(local_vid)

                    # Check if this vertex data combination (position, normal)
                    # already exists in our vertices list. If so, reuse
                    # previous entry. Otherwise, add it and record it in our
                    # unique_vertices lookup map.
                    vertex_data = (raw_vertices[vid], raw_normals[nid])
                    if vertex_data in unique_vertices:
                        vertex_id = unique_vertices[vertex_data]
                    else:
                        vertex_id = count
                        count += 1
                        unique_vertices[vertex_data] = vertex_id
                        vertices.append(vertex_data[0])
                        normals.append(vertex_data[1])

                    # Record the corresponding vertex_id into our
                    # shaded triangle index list.
                    shaded_indices.append(vertex_id)

                    # Record mesh vertex id to vertices list index
                    # for later wireframe generation.
                    if vid in mesh_vertex_map:
                        mesh_vertex_map[vid].append(vertex_id)
                    else:
                        mesh_vertex_map[vid] = [vertex_id,]
            next(face_it)

        # Iterate over each edge to generate wireframe data
        # in kLines format.
        edge_it = om.MItMeshEdge(obj)
        while not edge_it.isDone():
            mesh_vids = [edge_it.vertexId(0), edge_it.vertexId(1)]
            vids = [mesh_vertex_map[mesh_vids[0]][0], mesh_vertex_map[mesh_vids[1]][0]]
            for vid in vids:
                wire_indices.append(vid)
            next(edge_it)

        rep_mesh = helperShapeRepItemMesh()
        rep_mesh.vertices = vertices
        rep_mesh.normals = normals
        rep_mesh.wire_indices = wire_indices
        rep_mesh.shaded_indices = shaded_indices
        return rep_mesh

    def generate_curve(self, obj):
        '''
        Given a curve shape MObject, generate a helperShapeRepItemCurve.
        '''
        if not obj.apiType() == om.MFn.kNurbsCurve:
            return None
        curve_fn = om.MFnNurbsCurve(obj)

        vertices = []
        indices = []

        # Generate vertex positions
        if curve_fn.degree == 1:
            # For degree 1 curves, store the CVs.
            cvs = curve_fn.cvPositions()
            vertices = [ (v.x, v.y, v.z) for v in cvs ]
        else:
            # For degree >= 2 curves, sample the curve.
            curve_len = curve_fn.length()
            curve_sample_len = curve_len / (self.curve_samples)
            curve_len_samples = [ i*curve_sample_len for i in range(self.curve_samples) ]
            for s in curve_len_samples:
                param = curve_fn.findParamFromLength(s)
                pt = curve_fn.getPointAtParam(param)
                vertices.append((pt.x, pt.y, pt.z))

        # Generate indices
        for i in range(1, len(vertices)):
            indices.append(i-1)
            indices.append(i)
        if curve_fn.form == om.MFnNurbsCurve.kClosed or curve_fn.form == om.MFnNurbsCurve.kPeriodic:
            indices.append(len(vertices)-1)
            indices.append(0)

        rep_curve = helperShapeRepItemCurve()
        rep_curve.vertices = vertices
        rep_curve.indices = indices
        return rep_curve
        
    def doIt(self, arg_list):
        ''' Perform the command. '''
        parser = om.MArgParser(self.syntax(), arg_list)

        # Force
        if parser.isFlagSet(self.kForceFlag):
            self.force = True

        # Name
        if not parser.isFlagSet(self.kNameFlag):
            raise RuntimeError('The -{}/{} flag must be set.'.format(self.kNameFlag, self.kNameFlagLong))
        self.name = parser.flagArgumentString(self.kNameFlag, 0)
        if not self.force and not self.valid_name(self.name):
            raise RuntimeError('The specified name already exists: {}'.format(self.name))

        # Curve samples
        if parser.isFlagSet(self.kCurveSamplesFlag):
            self.curve_samples = parser.flagArgumentInt(self.kCurveSamplesFlag, 0)
        
        # Parse the object list or selection for meshes/curves.
        objects = self.get_objects(parser)

        # Generate representations from these objects.
        rep = helperShapeRep()
        rep.name = self.name
        for o in objects:
            if o.apiType() == om.MFn.kMesh:
                item = self.generate_mesh(o)
            elif o.apiType() == om.MFn.kNurbsCurve:
                item = self.generate_curve(o)
            else:
                continue
            if item:
                rep.items.append(item)

        # Serialize the representation.
        if len(rep.items):
            lib_path = helperShapeNode.shapes_lib_path
            if not os.path.exists(lib_path):
                os.makedirs(lib_path)
            file_name = self.name + '.json'
            file_path = os.path.join(lib_path, file_name)
            rep.serialize(file_path)


###############################################################################
#
# helperShapeNode (MPxLocatorNode)
#
class helperShapeNode(omui.MPxLocatorNode):
    ''' helperShapeNode locator class. '''
    name = 'locatorHelperShape'
    id = om.MTypeId( 0x00080041 )
    drawDbClassification = 'drawdb/subscene/locatorHelperShape'
    drawRegistrantId = 'locatorHelperShape_SubSceneOverride'
    
    aShape = om.MObject()
    aColor = om.MObject()
    aDrawOnTop = om.MObject()
    
    shapes_lib_path = None
    shapes = []
    
    attrEditorTemplate = '''
    global proc AElocatorHelperShapeColorNew(string $plug) {
        AElocatorHelperShapeColorReplace($plug);
    }

    global proc AElocatorHelperShapeColorReplace(string $plug) {
        setUITemplate -pst attributeEditorTemplate;
        string $parent = `setParent -q`;
        string $frame = $parent + "|locatorHelperShapeColorFrame";
        if (`frameLayout -ex $frame`) {
            deleteUI $frame;
        }
        $frame = `frameLayout -l "Colors" -collapse false locatorHelperShapeColorFrame`;
            string $column = `columnLayout -adj true`;
                int $colorIds[] = python( "list(set(maya.cmds.getAttr(\\"" + $plug + "\\", mi=True)))" );
                for ($id in $colorIds) {
                    string $index = "[" + $id + "]";
                    string $childPlug = $plug + $index;
                    string $childName = "color" + $index;
                    attrColorSliderGrp -l $childName -attribute $childPlug;
                }
        setUITemplate -ppt;
    }

    global proc AElocatorHelperShapeTemplate(string $node) {
        editorTemplate -beginScrollLayout;
            editorTemplate -beginLayout "Shape Attributes" -collapse 0;
                editorTemplate -addControl "shape";
                editorTemplate -addControl "drawOnTop";
                editorTemplate -callCustom "AElocatorHelperShapeColorNew"
                                           "AElocatorHelperShapeColorReplace"
                                           "color";
            editorTemplate -endLayout;
            editorTemplate -beginLayout "Locator Attributes";
                AElocatorCommon $node;
            editorTemplate -endLayout;
            AElocatorInclude $node;
        editorTemplate -addExtraControls;
        editorTemplate -endScrollLayout;
    }
    '''

    def __init__(self):
        ''' Constructor. '''
        omui.MPxLocatorNode.__init__(self)

    def postConstructor(self):
        ''' Post-constructor. '''
        enum_attr_fn = om.MFnEnumAttribute(self.aShape)
        shape_default = enum_attr_fn.default
        shape_plug = om.MPlug(self.thisMObject(), self.aShape)
        shape_plug.setShort(shape_default)
        
    @staticmethod
    def creator():
        ''' Creator method. '''
        return helperShapeNode()
        
    @classmethod
    def initialize(cls):
        ''' Initialize node attribute layout. '''
        num_attr_fn = om.MFnNumericAttribute()
        enum_attr_fn = om.MFnEnumAttribute()
        
        default_index = 0
        cls.aShape = enum_attr_fn.create('shape', 'sh')
        for (i, shape) in enumerate(cls.shapes):
            enum_attr_fn.addField(shape.name, i)
            if shape.name == 'default':
                default_index = i
        enum_attr_fn.default = default_index
        enum_attr_fn.internal = True
        
        cls.aColor = num_attr_fn.createColor('color', 'cl')
        num_attr_fn.default = DEFAULT_COLOR
        num_attr_fn.array = True

        cls.aDrawOnTop = num_attr_fn.create('drawOnTop', 'dot', om.MFnNumericData.kBoolean, False)
        
        cls.addAttribute( cls.aShape )
        cls.addAttribute( cls.aColor )
        cls.addAttribute( cls.aDrawOnTop )

    def compute(self, plug, data):
        ''' Compute method. '''
        return None

    def getShapeSelectionMask(self):
        ''' helperShape selection mask. '''
        return om.MSelectionMask(HELPER_SHAPE_SELECTION_MASK)

    def setInternalValueInContext(self, plug, data_handle, ctx):
        ''' Callback to set internal attribute values. '''
        # Use set internal callback to detect when the shape changes
        # and update the number of color fields accordingly.
        if plug == self.aShape:
            # Persist color[0] between shapes.
            color_plug = om.MPlug(self.thisMObject(), self.aColor)
            base_color_plug = color_plug.elementByLogicalIndex(0)
            base_color_obj = base_color_plug.asMObject()
            base_color = om.MFnNumericData(base_color_obj).getData()

            shape_id = data_handle.asShort()
            num_shape_items = len(self.shapes[shape_id].items)
            data_block = self.forceCache()
            color_builder = om.MArrayDataBuilder(data_block, self.aColor, num_shape_items)
            for i in range(num_shape_items):
                child = color_builder.addLast()
                child.set3Float(base_color[0], base_color[1], base_color[2])

            color_ovr_hnd = data_block.outputArrayValue(self.aColor)
            color_ovr_hnd.set(color_builder)

            # Force an AE refresh.
            mel.eval('autoUpdateAttrEd;')
        return False

    @classmethod
    def init_shapes(cls, lib_path):
        '''
        Populate the list of helperShape representations from
        the helperShape library path.
        
        This must be called prior to node registration since
        the enumeration attribute to select the active shape
        depends on the list of shapes.
        '''
        # Record lib_path
        cls.shapes_lib_path = lib_path

        # Find all *.json files in lib_path
        shape_files = []
        if os.path.exists(lib_path):
            for (root, dirs, files) in os.walk(lib_path):
                for f in files:
                    (name, ext) = os.path.splitext(f)
                    if ext == '.json':
                        file_path = os.path.join(root, f)
                        shape_files.append(file_path)

        cls.shapes = []
        if len(shape_files):
            for f in shape_files:
                rep = helperShapeRep()
                rep.deserialize(f)
                cls.shapes.append(rep)
        else:
            cls.shapes.append(helperShapeRepDefault())


###############################################################################
#
# helperShapeSubSceneOverride (MPxSubSceneOverride)
#
class helperShapeShaderItem(object):
    ''' helperShape shader class to help uniquely identify shaders. '''
    def __init__(self, shader_type, shader_color, transparent, pre_cb, post_cb):
        self.type = shader_type
        self.color = shader_color
        self.transparent = transparent
        self.pre_cb = pre_cb
        self.post_cb = post_cb

    def __eq__(self, other):
        return (isinstance(other, helperShapeShaderItem) and
                self.type == other.type and
                isclose_tuple(self.color, other.color) and
                self.transparent == other.transparent and
                self.pre_cb == other.pre_cb and
                self.post_cb == other.post_cb)

    def __ne__(self, other):
        return not(self == other)

    def __key(self):
        return (self.type, self.color, self.transparent, self.pre_cb, self.post_cb)

    def __hash__(self):
        return hash(self.__key())

class helperShapeShaderCache(object):
    ''' helperShape cache of shader instances '''
    def __init__(self):
        ''' Constructor. Initialize the shader cache. '''
        self.cache = {}

    def __del__(self):
        ''' Destructor. Clear the shader cache. '''
        shader_mgr = omr.MRenderer.getShaderManager()
        if not shader_mgr:
            return
        for (_, shader) in list(self.cache.items()):
            shader_mgr.releaseShader(shader)
        self.cache = {}

    def get_wire_shader(self, shader_color, transparent, pre_cb=None, post_cb=None):
        ''' Return a wire shader with the given parameters from the cache. '''
        shader_mgr = omr.MRenderer.getShaderManager()
        if not shader_mgr:
            return None
        shader_type = omr.MShaderManager.k3dThickLineShader
        item = helperShapeShaderItem(shader_type, shader_color, transparent, pre_cb, post_cb)
        if not item in self.cache:
            shader = shader_mgr.getStockShader(shader_type, pre_cb, post_cb)
            shader.setParameter('solidColor', shader_color)
            shader.setParameter('lineWidth', [DEFAULT_LINE_WIDTH]*2)
            shader.setIsTransparent(transparent)
            self.cache[item] = shader
        return self.cache[item]

    def get_shaded_shader(self, shader_color, transparent, pre_cb=None, post_cb=None):
        ''' Return a shaded shader with the given parameters from the cache. '''
        shader_mgr = omr.MRenderer.getShaderManager()
        if not shader_mgr:
            return None
        shader_type = omr.MShaderManager.k3dBlinnShader
        item = helperShapeShaderItem(shader_type, shader_color, transparent, pre_cb, post_cb)
        if not item in self.cache:
            shader = shader_mgr.getStockShader(shader_type, pre_cb, post_cb)
            shader.setParameter('diffuseColor', shader_color)
            shader.setIsTransparent(transparent)
            self.cache[item] = shader
        return self.cache[item]

class helperShapeSubSceneOverride(omr.MPxSubSceneOverride):
    ''' helperShape viewport subscene override class. '''
    kUpdateShaders      = 1 << 0
    kUpdateGeometry     = 1 << 1
    kUpdateMatrix       = 1 << 2
    kUpdateAll          = 0xf
    
    sLeadName           = 'lead'
    sActiveName         = 'active'
    sWireName           = 'wire'
    sShadedName         = 'shaded'
    
    shader_cache = helperShapeShaderCache()

    saved_depth_state = None

    def __init__(self, obj):
        ''' Constructor. '''
        omr.MPxSubSceneOverride.__init__(self, obj)
        
        node_fn = om.MFnDependencyNode(obj)
        self.node = node_fn.userNode()
        self.shape_index = 0
        self.shape_colors = []
        self.lead_color = (0.0, 0.0, 0.0, 1.0)
        self.active_color = (0.0, 0.0, 0.0, 1.0)
        self.draw_on_top = False
        
        self.wire_shaders = []
        self.shaded_shaders = []
        self.lead_select_shader = None
        self.active_select_shader = None

        self.update_mask = self.kUpdateAll
        
        # Cached instance data (transform_matrix, display_status)
        # keyed by instance number.
        self.instances = {}

        # Cached lists of render item names sorted by display_status
        self.lead_items = []
        self.active_items = []
        self.wire_items = []
        self.shaded_items = []
        
    @staticmethod
    def creator(obj):
        ''' Creator method. '''
        return helperShapeSubSceneOverride(obj)
        
    def supportedDrawAPIs(self):
        ''' Return the draw APIs supported by this override. '''
        return omr.MRenderer.kOpenGL | omr.MRenderer.kDirectX11 | omr.MRenderer.kOpenGLCoreProfile
        
    def hasUIDrawables(self):
        ''' Does this override have UI drawables? '''
        return False
        
    @classmethod
    def cleanup(cls):
        ''' Cleanup our shader cache. '''
        del(cls.shader_cache)
        cls.shader_cache = None

    @classmethod
    def pre_shader_cb(cls, context, render_items, shader):
        ''' Pre shader render callback. Used to facilitate drawOnTop feature. '''
        if len(render_items) > 0:
            state_mgr = context.getStateManager()
            cls.saved_depth_state = state_mgr.getDepthStencilState()
            depth_state_desc = omr.MDepthStencilStateDesc()
            depth_state_desc.depthEnable = False
            disabled_depth_state = state_mgr.acquireDepthStencilState(depth_state_desc)
            state_mgr.setDepthStencilState(disabled_depth_state)

    @classmethod
    def post_shader_cb(cls, context, render_items, shader):
        ''' Post shader render callback. Used to facilitate drawOnTop feature. '''
        if len(render_items) > 0:
            state_mgr = context.getStateManager()
            state_mgr.setDepthStencilState(cls.saved_depth_state)
            state_mgr.releaseDepthStencilState(cls.saved_depth_state)
            cls.saved_depth_state = None
        
    def requiresUpdate(self, container, frame_context):
        ''' Returns true if the update function should be called. '''
        self.requires_update_shaders()
        self.requires_update_geometry()
        self.requires_update_matrix()
        return self.update_mask > 0
        
    def requires_update_shaders(self):
        '''
        Checks if any attributes have changed that would affect
        this override's shaders. If so, sets the kUpdateShaders
        update bit.
        '''
        shape_plug = om.MPlug(self.node.thisMObject(), helperShapeNode.aShape)
        shape_id = shape_plug.asShort()
        num_shapes = len(helperShapeNode.shapes[shape_id].items)

        old_colors = self.shape_colors
        new_colors = []
        color_plug = om.MPlug(self.node.thisMObject(), helperShapeNode.aColor)
        for i in range(num_shapes):
            color_elem = color_plug.elementByLogicalIndex(i)
            color_obj = color_elem.asMObject()
            num_data_fn = om.MFnNumericData(color_obj)
            new_color = num_data_fn.getData()
            # The stock shaders expect color as float4.
            new_colors.append( (new_color[0], new_color[1], new_color[2], 1.0) )
        self.shape_colors = new_colors

        # Check for changes to color values / num colors
        if len(old_colors) != len(new_colors):
            self.update_mask |= self.kUpdateShaders
        else:
            for (o,n) in zip(old_colors, new_colors):
                if not isclose_tuple(o, n):
                    self.update_mask |= self.kUpdateShaders
                    break

        # Check for changes to lead/active colors
        old_lead_color = self.lead_color
        old_active_color = self.active_color
        view = omui.M3dView()
        lead_color = cmds.displayRGBColor('lead', q=True, alpha=True)
        self.lead_color = (lead_color[0], lead_color[1], lead_color[2], lead_color[3])

        active_color_index = cmds.displayColor('active', q=True, active=True) - 1
        active_color = view.colorAtIndex( active_color_index, omui.M3dView.kActiveColors )
        self.active_color = (active_color.r, active_color.g, active_color.b, active_color.a)

        old_sel_colors = (old_lead_color, old_active_color)
        new_sel_colors = (self.lead_color, self.active_color)
        for (o, n) in zip(old_sel_colors, new_sel_colors):
            if not isclose_tuple(o, n):
                self.update_mask |= self.kUpdateShaders
                break

        # Check drawOnTop changes
        draw_on_top_plug = om.MPlug(self.node.thisMObject(), helperShapeNode.aDrawOnTop)
        old_draw_on_top = self.draw_on_top
        self.draw_on_top = draw_on_top_plug.asBool()
        if self.draw_on_top != old_draw_on_top:
            self.update_mask |= self.kUpdateShaders
        
    def requires_update_geometry(self):
        '''
        Checks if any attributes have changed that would affect
        the geometry of any render items. If so, sets the kUpdateGeometry
        and kUpdateMatrix update bits.
        '''
        shape_plug = om.MPlug(self.node.thisMObject(), helperShapeNode.aShape)
        old_shape_index = self.shape_index
        self.shape_index = shape_plug.asInt()
        if self.shape_index >= len(helperShapeNode.shapes):
            self.shape_index = old_shape_index
        if not self.shape_index == old_shape_index:
            old_shape = helperShapeNode.shapes[old_shape_index]
            for item in old_shape.items:
                item.clear_buffers()
                
            # If geometry is requires update, then render items
            # will be rebuilt and matrices will also require
            # updates.
            self.update_mask |= self.kUpdateGeometry
            self.update_mask |= self.kUpdateMatrix
        
    def requires_update_matrix(self):
        '''
        Checks if any attributes have changed that would affect
        the matrices of any render items. If so, sets the
        kUpdateMatrix update bit.
        '''
        old_instances = self.instances
        
        # Update cached instance data.
        dag_fn = om.MFnDagNode(self.node.thisMObject())
        paths = dag_fn.getAllPaths()
        self.instances = {}
        for path in paths:
            if not path.isVisible():
                continue
            matrix = path.inclusiveMatrix()
            display_status = omr.MGeometryUtilities.displayStatus(path)
            self.instances[path.instanceNumber()] = (matrix, display_status)
        
        # Check for changes in total number of instances.
        old_instance_nums = set(old_instances.keys())
        new_instance_nums = set(self.instances.keys())
        instance_num_diff = old_instance_nums ^ new_instance_nums
        if len(instance_num_diff):
            # If the number of instances changes, rebuild all items.
            self.update_mask |= self.kUpdateGeometry
            self.update_mask |= self.kUpdateMatrix
            return
            
        # Check for changes in matrix or display_status.
        for i in list(self.instances.keys()):
            (old_matrix, old_display) = old_instances[i]
            (new_matrix, new_display) = self.instances[i]
            
            if old_display != new_display:
                self.update_mask |= self.kUpdateMatrix
                return
            if not old_matrix.isEquivalent(new_matrix):
                self.update_mask |= self.kUpdateMatrix
                return
        
    def update(self, container, frame_context):
        '''
        Updates only the portions of the override that require
        updating.
        '''
        if self.update_mask & self.kUpdateShaders:
            self.update_shaders()
        if self.update_mask & self.kUpdateGeometry:
            self.update_geometry(container, frame_context)
        if self.update_mask & self.kUpdateMatrix:
            self.update_matrix(container, frame_context)
        if self.update_mask & self.kUpdateShaders and not self.update_mask & self.kUpdateGeometry:
            self.update_shaders_assign(container)
        self.update_mask = 0
                
    def update_shaders(self):
        '''
        Initialize shaders if uninitialized and set their
        parameters (ex. color)
        '''
        shader_mgr = omr.MRenderer.getShaderManager()
        if not shader_mgr:
            return

        if self.draw_on_top:
            pre_cb = helperShapeSubSceneOverride.pre_shader_cb
            post_cb = helperShapeSubSceneOverride.post_shader_cb
        else:
            pre_cb = None
            post_cb = None

        num_colors = len(self.shape_colors)
        self.wire_shaders = []
        self.shaded_shaders = []
        for i in range(num_colors):
            shader = self.shader_cache.get_wire_shader(self.shape_colors[i], self.draw_on_top, pre_cb, post_cb)
            self.wire_shaders.append(shader)
        for i in range(num_colors):
            shader = self.shader_cache.get_shaded_shader(self.shape_colors[i], self.draw_on_top, pre_cb, post_cb)
            self.shaded_shaders.append(shader)
        self.lead_select_shader = self.shader_cache.get_wire_shader(self.lead_color, self.draw_on_top, pre_cb, post_cb)
        self.active_select_shader = self.shader_cache.get_wire_shader(self.active_color, self.draw_on_top, pre_cb, post_cb)

    def update_shaders_assign(self, container):
        '''
        Update shader assignments on geometry items.
        
        This is required for the case when shaders require updating,
        but geometry does not. In these cases we must update the
        assigned shaders on the existing render items.

        A separate routine is required for assignment so that both
        shaders and render item lists are properly initialized prior
        to assignment.
        '''
        for (i, item_name) in self.wire_items:
            item = container.find(item_name)
            shader = self.wire_shaders[i]
            if not shader is item.getShader():
                item.setShader(shader)
        for (i, item_name) in self.shaded_items:
            item = container.find(item_name)
            shader = self.shaded_shaders[i]
            if not shader is item.getShader():
                item.setShader(shader)
        for (_, item_name) in self.lead_items:
            item = container.find(item_name)
            if not shader is item.getShader():
                item.setShader(self.lead_select_shader)
        for (_, item_name) in self.active_items:
            item = container.find(item_name)
            if not shader is item.getShader():
                item.setShader(self.active_select_shader)
        
    def update_geometry(self, container, frame_context):
        '''
        Update the subscene container with the render items to draw the shape.
        This method regenerates the render items from a clean slate.
        '''
        container.clear()
        
        # For later searches into the container, record the names
        # of each render item by its selection status.
        self.lead_items = []
        self.active_items = []
        self.wire_items = []
        self.shaded_items = []

        active_shape = helperShapeNode.shapes[self.shape_index]
        for (i,item) in enumerate(active_shape.items):
            bounds = item.bounding_box()
            
            # Shared render items (wire, lead, active)
            vertex_buffer = item.vertex_buffer()
            wire_index_buffer = item.wire_index_buffer()
            
            wire_name = '_'.join((active_shape.name, str(i), self.sWireName))
            wire_item = omr.MRenderItem.create( wire_name, omr.MRenderItem.NonMaterialSceneItem, omr.MGeometry.kLines )
            wire_item.setDrawMode(omr.MGeometry.kWireframe | omr.MGeometry.kShaded | omr.MGeometry.kTextured)
            if item.type == helperShapeRepItemType.kMesh:
                # Mesh wireframe only visible in wireframe mode.
                wire_item.setDrawMode(omr.MGeometry.kWireframe)
            wire_item.setDepthPriority(omr.MRenderItem.sDormantWireDepthPriority)
            wire_item.setShader(self.wire_shaders[i])
            container.add(wire_item)
            self.setGeometryForRenderItem(wire_item, vertex_buffer, wire_index_buffer, bounds)
            self.wire_items.append((i, wire_name))
            
            lead_name = '_'.join((active_shape.name, str(i), self.sLeadName))
            lead_item = omr.MRenderItem.create( lead_name, omr.MRenderItem.DecorationItem, omr.MGeometry.kLines )
            lead_item.setDrawMode(omr.MGeometry.kWireframe | omr.MGeometry.kShaded | omr.MGeometry.kTextured)
            lead_item.setDepthPriority(omr.MRenderItem.sActiveWireDepthPriority)
            lead_item.setShader(self.lead_select_shader)
            container.add(lead_item)
            self.setGeometryForRenderItem(lead_item, vertex_buffer, wire_index_buffer, bounds)
            self.lead_items.append((i, lead_name))
            
            active_name = '_'.join((active_shape.name, str(i), self.sActiveName))
            active_item = omr.MRenderItem.create( active_name, omr.MRenderItem.DecorationItem, omr.MGeometry.kLines )
            active_item.setDrawMode(omr.MGeometry.kWireframe | omr.MGeometry.kShaded | omr.MGeometry.kTextured)
            active_item.setDepthPriority(omr.MRenderItem.sActiveWireDepthPriority)
            active_item.setShader(self.active_select_shader)
            container.add(active_item)
            self.setGeometryForRenderItem(active_item, vertex_buffer, wire_index_buffer, bounds)
            self.active_items.append((i, active_name))
            
            # Type specific render items (shaded)
            if item.type == helperShapeRepItemType.kMesh:
                shaded_index_buffer = item.shaded_index_buffer()
                shaded_name = '_'.join((active_shape.name, str(i), self.sShadedName))
                shaded_item = omr.MRenderItem.create( shaded_name, omr.MRenderItem.NonMaterialSceneItem, omr.MGeometry.kTriangles )
                shaded_item.setDrawMode( omr.MGeometry.kShaded | omr.MGeometry.kTextured )
                shaded_item.setExcludedFromPostEffects(True)
                shaded_item.setCastsShadows(False)
                shaded_item.setReceivesShadows(False)
                shaded_item.setShader(self.shaded_shaders[i])
                container.add(shaded_item)
                self.setGeometryForRenderItem(shaded_item, vertex_buffer, shaded_index_buffer, bounds)
                self.shaded_items.append((i, shaded_name))
                
    def update_matrix(self, container, frame_context):
        '''
        Updates the matrices of the render items in the container.
        '''
        # Sort items by display status.
        num_instances = len(self.instances)
        lead_instances = om.MMatrixArray()
        active_instances = om.MMatrixArray()
        dormant_instances = om.MMatrixArray()
        
        for (_, instance) in list(self.instances.items()):
            (matrix, display_status) = instance
            if display_status == omr.MGeometryUtilities.kLead:
                lead_instances.append(matrix)
            elif display_status == omr.MGeometryUtilities.kActive:
                active_instances.append(matrix)
            # Always display dormant items
            dormant_instances.append(matrix)

        dormant_items = []
        dormant_items.extend(self.wire_items)
        dormant_items.extend(self.shaded_items)
            
        if num_instances == 0:
            # Disable all render items
            #
            # BUG: The API docs and function signature suggest this
            # expects only the self argument, but it errors expecting
            # 1 argument. Pass in a dummy value of 0 as a workaround.
            container_it = container.getIterator(0)
            item = next(container_it)
            while item is not None:
                item.enable(False)
                item = next(container_it)
            container_it.destroy()
            container_it = None
        elif num_instances == 1:
            # Set our render items directly.
            matrix = dormant_instances[0]
            for (_, item_name) in dormant_items:
                item = container.find(item_name)
                item.enable(True)
                item.setMatrix(matrix)
                if WANT_CONSOLIDATION:
                    item.setWantSubSceneConsolidation(True)
            for (_, item_name) in self.lead_items:
                item = container.find(item_name)
                item.enable(len(lead_instances) > 0)
                item.setMatrix(matrix)
                if WANT_CONSOLIDATION:
                    item.setWantSubSceneConsolidation(True)
            for (_, item_name) in self.active_items:
                item = container.find(item_name)
                item.enable(len(active_instances) > 0)
                item.setMatrix(matrix)
                if WANT_CONSOLIDATION:
                    item.setWantSubSceneConsolidation(True)
        else:
            # Set instance transforms
            for (_, item_name) in dormant_items:
                item = container.find(item_name)
                item.enable(True)
                self.setInstanceTransformArray(item, dormant_instances)
            has_lead_instances = len(lead_instances) > 0
            for (_, item_name) in self.lead_items:
                item = container.find(item_name)
                item.enable(has_lead_instances)
                if has_lead_instances:
                    self.setInstanceTransformArray(item, lead_instances)
            has_active_instances = len(active_instances) > 0
            for (_, item_name) in self.active_items:
                item = container.find(item_name)
                item.enable(has_active_instances)
                if has_active_instances:
                    self.setInstanceTransformArray(item, active_instances)
        

###############################################################################
#
# helperShape plug-in initialize/uninitialize
#
def initializePlugin(obj):
    plugin = om.MFnPlugin( obj )
    
    plugin_path = os.path.normpath(plugin.loadPath())
    lib_path = os.path.join(plugin_path, 'library')
    lib_env_var = 'MAYA_LOCATOR_HELPER_SHAPE_LIB'
    if lib_env_var in os.environ:
        env_path = os.environ[lib_env_var]
        lib_path = os.path.normpath(env_path)
    helperShapeNode.init_shapes(lib_path)
    
    try:
        plugin.registerNode(helperShapeNode.name,
                            helperShapeNode.id,
                            helperShapeNode.creator,
                            helperShapeNode.initialize,
                            om.MPxNode.kLocatorNode,
                            helperShapeNode.drawDbClassification)
    except:
        sys.stderr.write('Failed to register locatorHelperShape node.\n')
        raise

    try:
        mel.eval(helperShapeNode.attrEditorTemplate)
    except:
        sys.stderr.write('Failed to load locatorHelperShape AETemplate script.\n')
        raise
        
    try:
        omr.MDrawRegistry.registerSubSceneOverrideCreator(
            helperShapeNode.drawDbClassification,
            helperShapeNode.drawRegistrantId,
            helperShapeSubSceneOverride.creator)
    except:
        sys.stderr.write('Failed to register locatorHelperShape SubSceneOverride.\n')
        raise
        
    try:
        plugin.registerCommand(helperShapeExportCmd.name,
                               helperShapeExportCmd.creator,
                               helperShapeExportCmd.new_syntax)
    except:
        sys.stderr.write('Failed to register locatorHelperShapeExportCmd.\n')
        raise

    # Register helperShape selection mask
    om.MSelectionMask.registerSelectionType(HELPER_SHAPE_SELECTION_MASK, HELPER_SHAPE_SELECTION_PRIORITY)
    cmds.selectType(byName=(HELPER_SHAPE_SELECTION_MASK, True))
    
def uninitializePlugin(obj):
    plugin = om.MFnPlugin( obj )
    
    try:
        plugin.deregisterNode(helperShapeNode.id)
    except:
        sys.stderr.write('Failed to deregister locatorHelperShape node.\n')
        pass
    
    try:
        omr.MDrawRegistry.deregisterSubSceneOverrideCreator(
            helperShapeNode.drawDbClassification,
            helperShapeNode.drawRegistrantId)
    except:
        sys.stderr.write('Failed to deregister locatorHelperShape SubSceneOverride.\n')
        pass
        
    try:
        plugin.deregisterCommand(helperShapeExportCmd.name)
    except:
        sys.stderr.write('Failed to deregister locatorHelperShapeExportCmd.\n')
        pass

    # Deregister helperShape selection mask
    om.MSelectionMask.deregisterSelectionType(HELPER_SHAPE_SELECTION_MASK)
    