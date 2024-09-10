from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.OpenMaya as OpenMayaV1


"""
This module provides a context manager to have a convenient 
profiler guard mechanism.  It ensures that the profiler is started 
and stopped at expected time.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ProfilerMgr:
    """
    Safe way to manage profiler guard
    
    Example:
        with ProfilerMgr('Profile Cube Creation'):
            cmds.polyCube()
            cmds.polyCube()
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, name): ...




def profile(decoratedFunc):
    """
    Profile decorator to manage the profiling scope
    """
    ...


_profilerCategory : int
_profilerColor : int

