from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import json
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.debug.emModeManager import emModeManager
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticGPUDeformers(BaseAnalytic):
    """
    Examine animated deformers nodes and check how they are used.
    
    If the 'details' option is set the CSV columns are:
        DeformerNode      : Name of the animated deformer node
        Type              : Type for this node
        SupportedGeometry : True if the geometry processed by animated
                            deformer node is supported by deformer evaluator
    
    otherwise the CSV columns are:
        DeformerMode       : Description of the usage for the animated deformer node
        Type               : Deformer type
        SupportedGeometry  : True if the geometry processed by animated
                             deformer nodes is supported by deformer evaluator
        Count              : Number of animated deformer nodes in this mode
    
        See is_supported_geometry() for what criteria a geometry must meet to be supported.
    
    One row is output for every animated deformer node.
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        :return: JSON with the error code if it failed, else None
        """
        ...
    @staticmethod
    def help():
        """
        Call this method to print the class documentation, including all methods.
        """
        ...
    @staticmethod
    def is_supported_geometry(geometry):
        """
        Checks if the geometry is supported by deformer evaluator.
        
        :param geometry: Geometry to be checked
        :return: True if it is supported by the deformer evaluator
        
        For it to be supported, it must:
            1) Be a mesh
            2) Not have a connected output
            3) Have at least k vertices, where k=2000 on NVidia hardware (hard-coded value)
        """
        ...
    ANALYTIC_DESCRIPTION_DETAILED : str
    
    ANALYTIC_DESCRIPTION_SHORT : unicode
    
    ANALYTIC_LABEL : unicode
    
    ANALYTIC_NAME : str
    
    __fulldocs__ : str
    
    is_static : bool




kAnalyticLabel : unicode
OPTION_DETAILS : str
kAnalyticDescriptionShort : unicode

