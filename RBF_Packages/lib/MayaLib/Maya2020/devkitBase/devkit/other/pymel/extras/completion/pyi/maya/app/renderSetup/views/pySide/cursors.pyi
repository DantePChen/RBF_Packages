from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
"""
This module provides a context manager to have a convenient
wait cursor guard mechanism.  It ensures that the wait cursor is started
and stopped at expected time.
"""


from PySide2.QtWidgets import QApplication
from PySide2.QtCore import Qt


if False:
    from typing import Dict, List, Tuple, Union, Optional

class WaitCursorMgr(object):
    """
    Safe way to manage wait cursors
    
    Example:
        with WaitCursorMgr():
            doSomeHeavyOperation()
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def waitCursor():
    """
    wait cursor decorator to manage the cursor scope
    
    Example:
        @waitCursor()
        def doSomeHeavyOperation():
    """
    ...

