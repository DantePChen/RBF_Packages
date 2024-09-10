from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticCharacter(BaseAnalytic):
    """
    Examine the characters in the scene for a few basic structure
    elements. The CSV file headings are generic so as to maximize the
    ability to process the data - 'Character','Type','Value'.
    
    When the 'details' option is set then the data looks like this:
        Character Name, 'Member', Character Member Plug name
        Character Name, 'Map', Character to which it is mapped
    
    otherwise it looks like this
        Character Name, 'Member', Number of members in the character
        Character Name, 'Map', Character to which it is mapped
    """
    
    
    
    def run(self):
        """
        Runs the analytic on the currently loaded file.
        :return: JSON containing the success status of the run
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

