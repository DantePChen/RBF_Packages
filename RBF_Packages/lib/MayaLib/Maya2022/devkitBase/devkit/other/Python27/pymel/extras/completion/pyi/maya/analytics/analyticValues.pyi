from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import math
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticValues(BaseAnalytic):
    """
    Look for specific values on the nodes in the scene.
    Here is a complete list of what will be counted and reported:
        - transforms using each of the non-standard rotation orders
        - transforms using scale limits, min and/or max
        - transforms using rotate limits, min and/or max
        - transforms using translation limits, min and/or max
        - joints with incoming connections on their scale attribute(s)
        - joints with incoming connections on their shear attribute(s)
        - joints with incoming connections on their translate attribute(s)
        - joints with non-uniform scale values (and no incoming connection)
        - joints with non-default shear values (and no incoming connection)
        - meshes with displaySmoothMesh turned on
    
    If the 'details' option is set then instead of showing one line per type
    of match with the number of matches found there will be a line for every
    match showing the node name matched.
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        Generate a CSV file containing the output described in the class doc.
        :return: None
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
OPTION_DETAILS : str
kAnalyticLabel : unicode

