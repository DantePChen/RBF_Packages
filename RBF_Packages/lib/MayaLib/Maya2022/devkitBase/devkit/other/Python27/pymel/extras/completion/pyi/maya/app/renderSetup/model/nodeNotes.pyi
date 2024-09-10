from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
from . import jsonTranslatorGlobals
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NodeNotes(object):
    """
    The class adds the support to the notes dynamic attribute.
    """
    
    
    
    def __init__(self): ...
    def getNotes(self): ...
    def setNotes(self, string): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




_NOTES_ATTRIBUTE_SHORT_NAME : str

