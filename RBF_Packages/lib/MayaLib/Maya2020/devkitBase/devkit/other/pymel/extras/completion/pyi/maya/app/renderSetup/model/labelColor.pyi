from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
from . import jsonTranslatorGlobals
from . import undo
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LabelColor(object):
    """
    The class adds the support to the label color dynamic attribute.
    """
    
    
    
    def __init__(self): ...
    def getLabelColor(self): ...
    def setLabelColor(self, string): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




_LABEL_COLOR_ATTRIBUTE_SHORT_NAME : str

