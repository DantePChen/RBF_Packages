from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import maya.api.OpenMaya as OpenMaya
import itertools
from . import utils
import maya.app.renderSetup.common.utils as commonUtils


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DagPath(object):
    """
    Helper class wrapper over MDagPath with specialized queries.
    """
    
    
    
    def __init__(self, dagPath):
        """
        Constructor. "dagPath" is a OpenMaya.MDagPath.
        """
        ...
    def findDisplacementShaders(self):
        """
        Generator over the displacement shaders assigned to this DAG path.
        There can be more than one if shape has per-face shading.
        """
        ...
    def findGeometryGenerator(self):
        """
        Returns the mesh or nurbs generator of this DAG path if any, None otherwise.
        """
        ...
    def findMaterialPlugs(self):
        """
        Find and yield all the shape plugs connected to materials (shading engines)
        """
        ...
    def findSets(self): ...
    def findSetsConnections(self, fnType='467'):
        """
        Generator over all the connections from this path to a node of type
        fnType. There can be more than one if the shape has per-face shading.
        
        Connections are returned as tuples (srcPlug, destPlug)
        "srcPlug" belongs to the shape node. "destPlug" belongs to the assigned node of type fnType.
        srcPlug ---> destPlug
        """
        ...
    def findShadingEngineConnections(self):
        """
        Find and yield all connections from the shape to a shading engine.
        """
        ...
    def findShadingEngines(self):
        """
        Generator over all the shading engines assigned to this DAG path.
        There can be more than one if shape has per-face shading.
        """
        ...
    def findSurfaceShaders(self):
        """
        Generator over the surface shaders assigned to this DAG path.
        There can be more than one if shape has per-face shading.
        """
        ...
    def findVolumeShaders(self):
        """
        Generator over the volume shaders assigned to this DAG path.
        There can be more than one if shape has per-face shading.
        """
        ...
    def fullPathName(self):
        """
        Returns the full path string of this DAG path.
        """
        ...
    def node(self):
        """
        Returns the DAG node at the end of the path (as MObject).
        """
        ...
    @staticmethod
    def create(pathStr):
        """
        Create a DagPath object for the given string path or None if it doesn't exist.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class _PlugNotFound(exceptions.Exception):
    __weakref__ : getset_descriptor




def _filterConnectionsToSpecificNodeType(plugs, fnType):
    """
    Return a generator of all connections to nodes of type fnType
    """
    ...

