from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.cmds as cmds


"""
This module provides a context manager that provides for convenient undo
macro commands (called "chunks" in Maya terminology).  It ensures that
only the top-level call to the undo context manager will open and close
a chunk, thus providing a single, named entry on the undo stack.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class SuspendUndo:
    """
    Safe way to suspend and resume undo logging using the 'with' command.
    It will automatically resume undo on exit from the block
    
    Example:
        with SuspendUndo():
            cmds.polyCube()
            cmds.polyCube()
        cmds.undo() # Will not undo the creation calls.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self): ...


class CtxMgr:
    """
    Safe way to manage group undo chunks using the 'with' command.
    It will close the chunk automatically on exit from the block
    
    Example:
        with CtxMgr('Create Poly Cubes'):
            cmds.polyCube()
            cmds.polyCube()
        cmds.undo() # Will undo both polyCube() creation calls.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, name="'unnamedOperation'"): ...
    openChunk : bool




def suspend():
    """
    Undo decorator to suspend and resume undo for all commands
    inside the decorated callable.
    """
    ...
def undoChunk(chunkName):
    """
    Undo decorator to name and group in a single chunk all commands
    inside the decorated callable.
    """
    ...

