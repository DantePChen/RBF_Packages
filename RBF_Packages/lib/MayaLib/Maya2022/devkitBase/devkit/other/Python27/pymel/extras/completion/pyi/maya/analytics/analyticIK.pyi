from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticIK(BaseAnalytic):
    """
    Analyze structure and usage of standard IK system.
    """
    
    
    
    def __init__(self):
        """
        Initialize the class members
        """
        ...
    def run(self):
        """
        Scan all of the standard IK connections to pull out usage statistics.
        "standard" means "not HIK". See analyticHIK() for specific details on
        that IK subsystem.
        
        The CSV output provides columns for the name of the statistic
        collected and the count of occurences of that statistic with the
        headings 'Handle', 'Parameter', 'Value'. If the 'details' option is not
        set then any node names in the output are replaced by their generic form
        'NODETYPEXXX' where 'NODETYPE' is the type of node and 'XXX' is a
        unique ID per node type. The following are collected:
            - IK Handle Name, 'Solver', Name of the solver the handle uses
            - IK Handle Name, 'Chain Start', Starting node of chain
            - IK Handle Name, 'Chain End', Ending node of chain
            - IK Handle Name, 'Chain Length', Number of nodes in the chain
            - IK Handle Name, 'End Effector', Name of chain's end effector
            - "", 'Solver', Name/Type of solver with no associated handles
            - "", 'Chain Link', Number of Joint nodes with no associated Handle
                (Not reported if the 'details' option is set.)
        
        If the 'details' option is set these columns are added to the output:
            - IK Handle Name, 'Chain Link', Node in the middle of a chain
            - "", 'Chain Link', Joint node with no associated Handle
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

