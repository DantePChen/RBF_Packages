from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import maya.api.OpenMaya as OpenMaya


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Node(object):
    """
    Node in the MemberSet structure. This should only be used by the MemberSet.
    MemberSet should be the only instance to ever create a node.
    """
    
    
    
    def __contains__(self, child): ...
    def __eq__(self, other: Any) -> bool: ...
    def __hash__(self): ...
    def __init__(self, path="'<OpenMaya.MDagPath object>'", status='0'): ...
    def add(self, child): ...
    def get(self, child):
        """
        # children accessors, mutators, queries
        """
        ...
    def remove(self, child): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class AlreadyHasStatus(exceptions.Exception):
    __weakref__ : getset_descriptor


class MemberSet(object):
    """
    Class for creating a set of layer members that will handle explicit 
    inclusion and exclusion of dag paths.
    """
    
    
    
    def __init__(self, paths='()'): ...
    def exclude(self, path): ...
    def include(self, path): ...
    def paths(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




EXCLUDE : int
NEUTRAL : int
INCLUDE : int

