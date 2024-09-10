from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import jsonTranslatorGlobals


if False:
    from typing import Dict, List, Tuple, Union, Optional

class SerializableNode(object):
    def __init__(self): ...
    def copyForClipboard(self, notes='None'): ...
    def decode(self, dict, mergeType, prependToName): ...
    def encode(self, notes='None'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor



