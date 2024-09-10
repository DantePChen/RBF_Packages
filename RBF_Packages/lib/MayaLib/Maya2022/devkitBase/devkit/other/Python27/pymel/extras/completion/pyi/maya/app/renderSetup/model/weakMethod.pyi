from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import weakref


"""
Weak references to bound methods cannot be created directly: if no
strong reference to the bound method is kept, the bound method object is
immediately garbage collected, and the weak reference immediately points to
a dead object and returns None.  See:

http://stackoverflow.com/questions/599430/why-doesnt-the-weakref-work-on-this-bound-method

This module supports the intent, namely a bound method that can only be called
while its object is alive, and keeps its object by weak reference.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Method(object):
    """
    Wraps a method such that the bound method's object is held only by
    weak reference.
    """
    
    
    
    def __call__(self, *posArgs, **kwArgs): ...
    def __eq__(self, other: Any) -> bool:
        """
        # As per recommendations
        # https://docs.python.org/2/reference/datamodel.html#object.__eq__
        # implement both __eq__ and __ne__
        """
        ...
    def __init__(self, method): ...
    def __ne__(self, other: Any) -> bool: ...
    def isAlive(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def create(method):
    """
    # Convenience function.
    """
    ...

