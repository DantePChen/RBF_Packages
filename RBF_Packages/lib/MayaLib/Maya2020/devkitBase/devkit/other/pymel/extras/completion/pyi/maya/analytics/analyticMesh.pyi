from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticMesh(BaseAnalytic):
    """
    Analyze the volume and distribution of mesh data.
    """
    
    
    
    def run(self):
        """
        Scan all of the Mesh shapes in the scene and provide a column for
        each node with the following statistics in it:
            - Number of vertices in the mesh
            - Number of faces in the mesh
            - Number of edges in the mesh
            - Number of triangles in the mesh
            - Number of UV coordinates in the mesh
            - Number of vertices "tweaked"
            - Is it using user normals?
            - What is the source node for the mesh? For meshes with no
              construction history it will be the mesh itself. For others
              it could be a polySphere or other creation operation, or some
              other mesh at the beginning of a deformation chain.
        """
        ...
    @staticmethod
    def help():
        """
        Call this method to print the class documentation, including all methods.
        """
        ...
    ANALYTIC_DESCRIPTION_DETAILED : str
    
    ANALYTIC_DESCRIPTION_SHORT : unicode
    
    ANALYTIC_LABEL : unicode
    
    ANALYTIC_NAME : str
    
    __fulldocs__ : str
    
    is_static : bool




kAnalyticDescriptionShort : unicode
kAnalyticLabel : unicode
RE_MULTI_GEOMETRY_OUTPUT : re.SRE_Pattern

