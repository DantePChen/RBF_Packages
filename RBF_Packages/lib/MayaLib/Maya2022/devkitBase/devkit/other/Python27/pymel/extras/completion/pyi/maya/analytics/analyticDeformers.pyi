from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.analytics.decorators import addHelp


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticDeformers(BaseAnalytic):
    """
    Examine the meshes in the scene for deformation. There will be two
    types of data in the output file under the column headings
    'Deformer','Member','Value':
        - Deformer Name, Member Object, Membership Information, Member Count
            One line per object being affected by the deformer
        - Deformer Name, '', Name of next deformer in the chain, Deformer Chain length
            Only if more than one deformer is being applied to the same object
    
    If the 'details' option is not set then the Member Information is omitted,
    otherwise it will be a selection-list format of all members on that
    object subject to deformation by the given deformer.
    """
    
    
    
    def run(self):
        """
        Run the analytic on the current scene.
        :return: JSON with any error in the analytic run. None means success.
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




RE_VERTEX_ALL : re.SRE_Pattern
kAnalyticLabel : unicode
RE_VERTEX : re.SRE_Pattern
kAnalyticDescriptionShort : unicode
RE_VERTEX_PAIR : re.SRE_Pattern
OPTION_DETAILS : str

