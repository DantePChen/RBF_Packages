from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions


"""
#------------------------------------------------------------------------------
# Condition objects - used for chaining together tests that yield True/False results
#------------------------------------------------------------------------------
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NO_DATA(exceptions.Exception):
    __weakref__ : getset_descriptor


class Condition(object):
    """
    Used to chain together objects for conditional testing.
    """
    
    
    
    def __and__(self, other): ...
    def __init__(self, value='None'): ...
    def __invert__(self): ...
    def __nonzero__(self) -> bool: ...
    def __or__(self, other): ...
    def __rand__(self, other): ...
    def __ror__(self, other): ...
    def __str__(self) -> str: ...
    def eval(self, data='"<class \'pymel.util.conditions.NO_DATA\'>"'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class Inverse(Condition):
    def __init__(self, toInvert): ...
    def __str__(self) -> str: ...
    def eval(self, data='"<class \'pymel.util.conditions.NO_DATA\'>"'): ...


class AndOrAbstract(Condition):
    def __init__(self, *args): ...
    def __str__(self) -> str: ...
    def eval(self, data='"<class \'pymel.util.conditions.NO_DATA\'>"'): ...


class Or(AndOrAbstract):
    pass


class And(AndOrAbstract):
    pass




Always : Condition
Never : Condition

