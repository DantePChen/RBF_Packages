from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.dg_utilities import node_type_hierarchy_list
from maya.analytics.dg_utilities import default_nodes_and_connections
from maya.analytics.decorators import addMethodDocs
from maya.analytics.dg_utilities import node_level_connections
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticNodeTypes(BaseAnalytic):
    """
    This analytic collects the number of each node type in the scene.
    
    All of the persistent and default nodes are skipped unless they have a new
    connection. The way these two things are measured is different by necessity
    for the cases of analyzing a file that is read and analyzing the current scene.
    
    If the file is being read:
        - persistent and default nodes are defined as any node present before the
          file is loaded
        - exceptions are made if a new connection is formed to a persistent or
          default node after the file is loaded
    
    If the current scene is used:
        - persistent and default nodes are taken to be those marked as such by the
          Maya 'ls' command. This won't include any special persistent nodes
          created after-the-fact, such as those a newly loaded plug-in might create.
        - exceptions are made if there is any connection at all to these default
          or persistent nodes to a scene node.
    
    If the 'summary' option is used then the output includes a dictionary
    consisting of NODE_TYPE keys with value equal to the number of nodes of that
    type in the scene, not including default node types. Only node types with at
    least 1 node of that type are included.
    
        "summary" : {
            "transform" : 3,
            "mesh" : 1
        }
    
    For normal output the output is a dictionary whose keys are the node types and
    the values are a list of nodes of that type. The information is put into an
    object named "node_types". This avoids the potential for a name conflict
    between the object "summary" and a node type also named "summary".
    
        "nodeTypes" : {
            "transform" : ["transform1", "transform2", "group1"],
            "mesh" : ["cubeShape1"]
        }
    
    If the 'details' option is used then the output is arranged hierarchically by
    node type layers instead of a flat dictionary.
    
        "nodeTypeTree" : {
            "ROOT_NODE" : {
                "nodes" : [],
                "children" : {
                    "CHILD_NODE" : {
                        "nodes" : [],
                        "children" : {
                            "GRANDCHILD_NODE_TYPE1" : {
                                "nodes" : ["GC1_NODE_NAME],
                                "children" : []
                            },
                            "GRANDCHILD_NODE_TYPE2" : {
                                "nodes" : ["GC2_NODE_NAME],
                                "children" : []
                            }
                        }
                    }
                }
            }
        }
    
    If the analytic-specific option 'use_defaults' is used then the default nodes
    will be included in the output.
    """
    
    
    
    def __init__(self):
        """
        Initialize the persistent class members
        
        default_nodes:            Set of all default nodes
        default_node_connections: Set of (src,dst) pairs for all connections
                                  between default nodes.
        """
        ...
    def establish_baseline(self):
        """
        This is run on an empty scene, to find all of the nodes/node types
        present by default. They will all be ignored for the purposes of
        the analytic since they are not relevant to scene contents.
        """
        ...
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
    
    KEY_CHILDREN : str
    
    KEY_NODES : str
    
    KEY_NODE_TYPES : str
    
    __fulldocs__ : str
    
    is_static : bool




kAnalyticDescriptionShort : unicode
kAnalyticLabel : unicode
OPTION_SUMMARY : str
OPTION_DETAILS : str
OPTION_USE_DEFAULTS : str

