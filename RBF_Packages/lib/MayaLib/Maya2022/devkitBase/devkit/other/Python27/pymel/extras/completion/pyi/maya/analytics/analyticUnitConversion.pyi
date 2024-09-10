from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticUnitConversion(BaseAnalytic):
    """
    This analytic collects the types of nodes before and after a unit conversion node.
    
    The output consists of a dictionary entry with a before and after section.
    The 'before' section contains node type/count pairs indicating how many times
    they appear before a unit conversion node. The 'after' section contains node
    type/count pairs indicating how many times they appear after a unit conversion
    node. ('Before' and 'after' refer to upstream and downstream connections
    respectively.) The 'conversion' section contains counts of the unit conversion
    nodes that use various conversion factors.
    
        "before" : {
            "animCurveTL" : 3,
            "animCurveTU" : 1
        },
        "after" : {
            "transform" : 3,
            "mesh" : 1
        }
        "conversion" : {
            1.2345 : 2,
            7.123 : 2
        }
    
    If the 'details' option is used then the set of all unit conversion connections
    is output as a dictionary where the key is the unit conversion node type and
    the values are lists of pairs
    
        "details" : {
            "unitToTimeConversion" : [
                [ "node1.tx", "node2.input1"],
                [ "node1.ty", "node2.input1"],
                [ "node1.tz", "node2.input1"]
            ],
            "timeToUnitConversion" : [
                [ "node1_tx", "node1.dynInput1"],
                [ "node1_ty", "node1.dynInput2"],
                [ "node1_tz", "node1.dynInput3"]
            ]
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
KEY_AFTER : str
kAnalyticDescriptionShort : unicode
KEY_BEFORE : str
KEY_ROOT : str
KEY_CONVERSION : str
KEY_DETAILS : str
OPTION_DETAILS = KEY_DETAILS

