from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import json
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.debug.emModeManager import emModeManager
from maya.analytics.analyticGPUDeformers import analyticGPUDeformers
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticGPUClusters(BaseAnalytic):
    """
    Examine animated cluster nodes and check how they are used.  It checks
    whether they are used for fixed rigid transform, weighted rigid transform
    or per-vertex-weighted transform.
    
    When the 'details' option is set the CSV columns are:
        ClusterNode        : Name of the animated cluster node
        envelope_is_static : True if the envelope is not animated and its value is 1
        uses_weights       : True if weights are used in the node
        uses_same_weight   : True if weight is the same for all vertices
        Mode               : Mode for this node
        supported_geometry : True if the geometry processed by animated cluster node
                             is supported by deformer evaluator
    
    otherwise the CSV columns are:
        ClusterMode        : Description of the usage for the animated cluster node
        Mode               : Mode for animated cluster nodes meeting this criteria
        supported_geometry : True if the geometry processed by animated cluster nodes
                             meeting this criteria is supported by deformer evaluator
        Count              : Number of animated cluster nodes in this mode
    
        See is_supported_geometry() for what criteria a geometry must meet to be supported.
    
    One row is output for every animated cluster node.
    
    The "Mode" is an integer value with the following meaning:
    - 1 => Rigid transform          : cluster node only performs a rigid transform
    - 2 => Weighted rigid transform : cluster node performs a rigid transform, but it
                                      is weighted down by a factor
    - 3 => Per-vertex transform     : cluster node computes a different transform for
                                      each individually-weighted vertex
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        :return: JSON with the error code if it failed, None if it succeeded
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
OPTION_DETAILS : str

