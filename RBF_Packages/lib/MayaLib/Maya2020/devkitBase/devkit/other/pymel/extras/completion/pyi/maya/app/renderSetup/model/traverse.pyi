from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import nodeList


if False:
    from typing import Dict, List, Tuple, Union, Optional

def depthFirst(node, children):
    """
    Generator for depth-first traversal of a tree.
    
    The node argument is the starting point of the traversal.
    
    The children argument is a callable that must produce an iterable on
    the node's children.  This is used by the traversal to iterate on the
    node's children and thus recurse.
    """
    ...
def nodeListChildren(node):
    """
    Utility function to iterate on children of a data model node.
    
    If the node has no children, an empty list is returned.
    """
    ...

