from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.dg_utilities import node_type_hierarchy_list
from maya.analytics.decorators import addMethodDocs
from maya.analytics.dg_utilities import plug_level_connections
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticConnections(BaseAnalytic):
    """
    This analytic looks at all connections in the DG and reports on them.
    
    If the 'detail' option is set the CSV file consists of the following
    columns with one connection per row:
        Source           : Source plug of the connection
        Source Type      : Node type of the source plug's node
        Destination      : Destination plug of the connection
        Destination Type : Node type of the destination plug's node
    
    Otherwise the CSV file consists of the following columns with
    one node type per row:
        Node Type         : Type of node involved in connections
        Source Count      : Number of outgoing connections on nodes of that type
        Destination Count : Number of incoming connections on nodes of that type
    """
    
    
    
    def run(self):
        """
        Run the analytic on the currently loaded file.
        Output is sent to the default CSV file.
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
OPTION_SUMMARY : str
OPTION_DETAILS : str

