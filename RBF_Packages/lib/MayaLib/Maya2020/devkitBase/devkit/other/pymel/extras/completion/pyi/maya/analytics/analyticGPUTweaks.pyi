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

class analyticGPUTweaks(BaseAnalytic):
    """
    Examine animated tweak nodes and check how they are used.  It checks
    whether they use the relative or absolute mode and whether individual
    tweaks themselves are actually used.
    
    If the 'details' option is set the CSV columns are:
        TweakNode  : Name of the animated tweak node
        Relative   : Value of the relative_tweak attribute of the animated tweak node
        uses_tweaks : True if tweaks are used in the node
        UsesMesh   : True if some of the geometry processed by animated tweak node is a mesh
    
    otherwise the CSV columns are:
        TweakType   : Description of the usage for the animated tweak node
        Relative    : Value of the relative_tweak attribute of the animated
                      tweak nodes meeting this criteria
        uses_tweaks  : True if tweaks are used in the nodes meeting this criteria
        UsesMesh    : True if some of the geometry processed by animated tweak
                      nodes meeting this criteria is a mesh
        Count       : Number of animated tweak nodes meeting this criteria
    
    One row is output for every animated tweak node.
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
    ANALYTIC_DESCRIPTION_DETAILED : str
    
    ANALYTIC_DESCRIPTION_SHORT : unicode
    
    ANALYTIC_LABEL : unicode
    
    ANALYTIC_NAME : str
    
    __fulldocs__ : str
    
    is_static : bool




kAnalyticDescriptionShort : unicode
OPTION_DETAILS : str
kAnalyticLabel : unicode

