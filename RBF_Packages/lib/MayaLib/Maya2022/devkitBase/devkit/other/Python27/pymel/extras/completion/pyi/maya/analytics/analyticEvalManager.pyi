from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import json
import os
import re
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.debug.emModeManager import emModeManager
from maya.analytics.decorators import addHelp


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticEvalManager(BaseAnalytic):
    """
    The information regarding the evaluation graph and scheduling information
    for the current scene is output. The evaluation graph is updated before
    dumping the information so it is guaranteed to be current. If no options
    are selected the format shows the names of the nodes grouped by scheduling
    types as well as a list of the node clusters created for scheduling.
    
        "BuildTime"      : GRAPH_BUILD_TIME_IN_MICROSECONDS
        "Parallel"       : [ LIST_OF_NODES_SCHEDULED_AS_PARALLEL ],
        "Serial"         : [ LIST_OF_NODES_SCHEDULED_AS_SERIAL ],
        "GloballySerial" : [ LIST_OF_NODES_SCHEDULED_AS_GLOBALLY_SERIAL ],
        "Untrusted"      : [ LIST_OF_NODES_SCHEDULED_AS_UNTRUSTED ]
        "Clusters"       : [ { CLUSTER_NAME : [ LIST_OF_NODES_IN_CLUSTER ] },
                             { CLUSTER_NAME : [ LIST_OF_NODES_IN_CLUSTER ] }
                             ...
                           ]
        The last is presented as an array of objects because the cluster
        names are not necessarily unique.
    
    Options Available
        summary = Show a count of the various scheduling and cluster types
                  in the graph. Appends this section to the above.
    
                  "summary" : {
                      "Parallel" : COUNT_OF_PARALLEL_NODES,
                      "Serial" : COUNT_OF_SERIAL_NODES,
                      "GloballySerial" : COUNT_OF_GLOBALLY_SERIAL_NODES,
                      "Untrusted" : COUNT_OF_UNTRUSTED_NODES,
                      "Clusters" : [ COUNT_OF_NODES_PER_CLUSTER ]
                  }
    
        details = Include all of the plug and connection information for each
                  evaluation node. Instead of a list of node names each node
                  will be an object containing plug and connection information:
    
                  "NODE_NAME" : {
                     "inputPlugs"            : [ LIST_OF_INPUT_PLUGS_TO_DIRTY ],
                     "outputPlugs"           : [ LIST_OF_OUTPUT_PLUGS_TO_DIRTY ],
                     "affectsWorldPlugs"     : [ LIST_OF_WORLD_AFFECTING_PLUGS_TO_DIRTY ],
                     "upstreamConnections"   : [ LIST_OF_NODES_CONNECTED_UPSTREAM ],
                     "downstreamConnections" : [ LIST_OF_NODES_CONNECTED_DOWNSTREAM ]
                  }
    
    Example of a graph with two nodes in one cluster dumped with the 'summary' option:
    
        "output" : {
            "summary" : {
                "Parallel" : 1,
                "Serial" : 1,
                "GloballySerial" : 0,
                "Untrusted" : 0,
                "Clusters" : [1,1]
            },
            "BuildTime" : 12318,
            "Parallel" : [ "node1" ],
            "Serial" : [ "node2" ],
            "GloballySerial" : [],
            "Untrusted" : [],
            "Clusters" : [
                { "pruneRootsEvaluator" : [ "node1" ] },
                { "cacheEvaluator" : [ "node2" ] }
            ]
        }
    
    The same graph with no options:
    
        output" : {
            "BuildTime" : 12318,
            "Parallel" : [ "node1" ],
            "Serial" : [ "node2" ],
            "GloballySerial" : [],
            "Untrusted" : [],
            "Clusters" : [
                { "pruneRootsEvaluator" : [ "node1" ] },
                { "cacheEvaluator" : [ "node2" ] }
            ]
        }
    
    The same graph with both 'summary' and 'details' options:
    
        "output" : {
            "summary" : {
                "Parallel" : 1,
                "Serial" : 1,
                "GloballySerial" : 0,
                "Untrusted" : 0,
                "Clusters" : [1,1]
            },
            "BuildTime" : 12318,
            "Parallel" : {
                "node1" : {
                    "inputPlugs" : [ "node1.i" ],
                    "outputPlugs" : [ "node1.wm", "node1.pm" ],
                    "affectsWorldPlugs" : [],
                    "upstreamConnections" : [ "node2" ],
                    "downstreamConnections" : []
                    }
                },
            },
            "Serial" : {
                "node2" : {
                    "inputPlugs" : [],
                    "outputPlugs" : [ "node2.o" ],
                    "affectsWorldPlugs" : [],
                    "upstreamConnections" : [],
                    "downstreamConnections" : [ "node1" ]
                }
            },
            "GloballySerial" : {},
            "Untrusted" : {},
            "Clusters" : [
                { "pruneRootsEvaluator" : ["node1"] },
                { "cacheEvaluator" : ["node2"] }
            ]
        }
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        :return: a JSON structure as described in the class doc
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
    
    CONNECTION_TYPES : list
    
    PLUG_TYPES : list
    
    __fulldocs__ : str
    
    is_static : bool




kAnalyticLabel : unicode
OPTION_SUMMARY : str
OPTION_DETAILS : str
kAnalyticDescriptionShort : unicode

