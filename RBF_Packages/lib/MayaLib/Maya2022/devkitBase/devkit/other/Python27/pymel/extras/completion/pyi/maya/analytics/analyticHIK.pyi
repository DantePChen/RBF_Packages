from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticHIK(BaseAnalytic):
    """
    Scan all of the HIK connections to see if any recognized patterns
    are found.
    
    FBIK pattern:
        A joint connects to an hikEffector through a message attribute.
        Interesting because it causes the EM to fail.
    """
    
    
    
    def run(self):
        """
        Run the analytics on the current scene.
        :return: JSON with the error message if it failed, else None
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

