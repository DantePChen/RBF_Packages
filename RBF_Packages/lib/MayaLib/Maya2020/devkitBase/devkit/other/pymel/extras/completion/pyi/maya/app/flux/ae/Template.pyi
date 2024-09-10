from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Template(object):
    def __init__(self, nodeName): ...
    def addCustom(self, obj): ...
    def suppress(self, control): ...
    def suppressAll(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor



