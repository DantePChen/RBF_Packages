from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import json
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.dg_utilities import node_type_hierarchy_list
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp


if False:
    from typing import Dict, List, Tuple, Union, Optional

class AnalyticStaticNodeTypes(BaseAnalytic):
    """
    The information regarding the node type hierarchy and their flags is returned
    in JSON format.
    
        "nodeTypes" :
        {
            "type"       : "node",
            "children"   :
            [
                {
                    "type" : "childNodeType",
                    "children" :
                    [
                        { "type" : "grandchildNodeType" }
                    ]
                }
            ]
        }
    
    Options Available
        details = Include all of the attribute information for each node type.
                  Attribute information is not inherited; the node type will only
                  show those attributes it has added, none of the parent node
                  type's attributes.
    
                    "attributes" :
                    {
                        "staticAttributes" :
                        [
                            { "weightedPair" : [ "weight", { "pair" : [ "pairA", "pairB" ] } ] }
                        ]
                        ,
                        "extensionAttributes" :
                        [
                            ...etc...
                        ]
                    }
    """
    
    
    
    def __init__(self):
        """
        Initialize the persistent class members
        """
        ...
    def run(self):
        """
        Run the analytic on the current scene.
        :return: JSON results as described by the class doc.
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

