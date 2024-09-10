from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.cmds as cmds


"""
This module implements a context and an associated decorator to run 
code with the root or a given Maya namespace as current.
It must decorate every function that involves render setup
nodes creation and renaming.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NamespaceGuard:
    """
    Safe way to set namespace using the 'with' statement.
    It will set the namespace back to the previously used namespace on exit from the block.
    The namespace changes WILL affect undo stack. Make sure to wrap it in an 
    undo chunk if needed.
    
    Example:
        with NamespaceGuard(ROOT_NAMESPACE):
            someCreateNodeFunction()
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, namespace): ...




def RootNamespaceGuard(): ...
def guard(name): ...
def root(f): ...


ROOT_NAMESPACE : str

