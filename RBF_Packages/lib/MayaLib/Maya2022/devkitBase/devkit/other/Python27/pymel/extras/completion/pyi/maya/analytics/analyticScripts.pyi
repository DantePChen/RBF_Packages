from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addHelp
from maya.analytics.decorators import addMethodDocs


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticScripts(BaseAnalytic):
    """
    Generates the number of scriptJobs active in the scene, grouped
    by the type of event that they are watching. No details of the
    actual event are collected. Output is in CSV form with the
    columns 'eventType,count', ordered from most frequent to least
    frequent.
    
    If the 'details' option is set then include the name of the script
    called and detail parameters for certain other triggers, for
    example, the name of the node whose name change is being monitored.
    """
    
    
    
    def __init__(self):
        """
        Initialize the class members
        """
        ...
    def run(self):
        """
        Run the analytic on the current scene.
        Generates a CSV file output with scriptJob information as
        described in the class doc.
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




RE_JOB : re.SRE_Pattern
kAnalyticLabel : unicode
OPTION_DETAILS : str
kAnalyticDescriptionShort : unicode

