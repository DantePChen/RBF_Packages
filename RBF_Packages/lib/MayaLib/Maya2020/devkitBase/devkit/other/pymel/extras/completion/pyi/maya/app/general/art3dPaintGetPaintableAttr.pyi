from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def attributeToPaint_melToUI(value): ...
def attributeToPaint_uiToMel(value): ...
def art3dPaintGetPaintableAttr(allowCustomAttrs='True'):
    """
    Return list of all the names of 
    all color attrs common (to all selected shader) paintable attributes.
    This includes custom attributes.
    """
    ...


_dict_attributeToPaint_uiToMel : dict
_dict_attributeToPaint_melToUI : dict
k : str
v : unicode

