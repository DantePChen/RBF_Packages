from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import re
import json
import os
import maya.cmds as cmds


"""
Gather basic scene timing information from the profiler,
with and without the invisibility evaluator active.
"""


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.debug.emModeManager import emModeManager
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticTiming(BaseAnalytic):
    """
    Examine timing information for basic operations in different modes.
    The list of modes can be modified as needs change by altering the
    MODE_LIST value in the script.
    
    WARNING: Since this test gets timing for file-new you will lose your
             data if you run it on the current scene.
    
    It measures the following events for each of the modes, in microseconds. If
    multiple instances of the event are found then the last one found is used,
    under the assumption that the state is most steady by then.
    
        EvaluationGraphConstruction    : Graph build time
        EvaluationGraphPartitioning    : Graph partitioning time
        EvaluationGraphExecution       : Graph execution time
        Vp2SceneRender                 : Viewport 2 rendering time
        Vp1SceneRender                 : Legacy Viewport rendering time
        ClusterCount                   : Total number of custom evaluator clusters
        ClusterNodeCount               : Total number of nodes in custom evaluator clusters
        InvisibilityClusterCount       : Total number of invisibility clusters
        InvisibilityClusterNodeCount   : Total number of nodes in invisibility clusters
        InvisibilityCreateClusters     : Time taken by the invisibility evaluator to create its clusters
        InvisibilityDiscover           : Time taken by the invisibility evaluator to discover invisible nodes
        InvisibilityMarkNodes          : Time taken by the invisibility evaluator to mark its nodes
        InvisibilityCreatePartitioning : Time taken by the invisibility evaluator to create its partitions
    
    Note: InvisibilityCreateClusters is the parent of these three steps so don't add them:
        - InvisibilityDiscover
        - InvisibilityMarkNodes
        - InvisibilityCreatePartitioning
    
    and these, which are independent of the evaluator configuration:
        FileNew        : Time to empty the scene of the current file, in seconds
        CycleCount     : Total number of cycle clusters
        CycleNodeCount : Total number of nodes in cycle clusters
    
    Example output running in parallel mode both with and without the
    invisibility for a scene that uses VP2:
    
        "output" : {
            "emp-invisibility" : {
                "EvaluationGraphConstruction"    : 5632,
                "EvaluationGraphPartitioning"    : 392,
                "EvaluationGraphExecution"       : 2020211,
                "Vp2SceneRender"                 : 7152,
                "Vp1SceneRender"                 : 0,
                "ClusterCount"                   : 72,
                "ClusterNodeCount"               : 1230,
                "InvisibilityClusterCount"       : 0,
                "InvisibilityClusterNodeCount"   : 0,
                "InvisibilityDiscover"           : 0,
                "InvisibilityCreateClusters"     : 0,
                "InvisibilityMarkNodes"          : 0,
                "InvisibilityCreatePartitioning" : 0
            },
            "emp+invisibility" : {
                "EvaluationGraphConstruction"    : 7801,
                "EvaluationGraphPartitioning"    : 738,
                "EvaluationGraphExecution"       : 19374,
                "Vp2SceneRender"                 : 7326,
                "Vp1SceneRender"                 : 0,
                "ClusterCount"                   : 129,
                "ClusterNodeCount"               : 7183,
                "InvisibilityClusterCount"       : 11,
                "InvisibilityClusterNodeCount"   : 11,
                "InvisibilityDiscover"           : 12341,
                "InvisibilityCreateClusters"     : 123,
                "InvisibilityMarkNodes"          : 1110,
                "InvisibilityCreatePartitioning" : 84
            },
            "CycleCount"     : 3,
            "CycleNodeCount" : 14,
            "FileNew"        : 4.19238
        }
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        :return: JSON results as described in the class doc
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




kAnalyticLabel : unicode
kAnalyticDescriptionShort : unicode
MODE_LIST : list

