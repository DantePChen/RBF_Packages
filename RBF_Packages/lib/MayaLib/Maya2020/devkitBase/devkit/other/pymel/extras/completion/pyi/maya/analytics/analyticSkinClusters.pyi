from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


"""
Analyze type and usage of skin cluster deformers to discover usage
patterns contrary to the assumptions of the code.
"""


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticSkinClusters(BaseAnalytic):
    """
    Examine the skin cluster nodes in the scene for connection on the
    driver points attribute. Checks for any connection first, and then for
    the size of the driver versus the size of the driven mesh second. The
    assumption was that the driver would always be much smaller than the
    driven mesh since that's kind of the point of a skin cluster.
    
    The analytics output contains the following columns
        Deformer    : Name of the skin cluster found
        Connection    : Name of the node connected at the driver points
                      input or '' if none
        DriverSize    : Number of points in the driver points input
        DrivenSize    : Number of points in the driven object
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        Generates CSV output as described by the class doc.
        :return: JSON with error message if the analytic failed, else None
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

