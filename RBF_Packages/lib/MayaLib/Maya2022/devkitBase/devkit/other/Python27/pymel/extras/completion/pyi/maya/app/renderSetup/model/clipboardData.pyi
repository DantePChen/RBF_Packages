from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import jsonTranslatorUtils
import json


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ClipboardData(object):
    def __init__(self, typeName, parentTypeName): ...
    def typeName(self): ...
    @property
    def parentTypeName(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def getClipboardDataFromJson(clipBoardJson): ...


PARENT_TYPE_NAME : str

