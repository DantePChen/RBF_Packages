from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import sys


from PySide2.QtWidgets import QAction
from contextlib import contextmanager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Action(QAction):
    def excepthook(self, type, value, traceback): ...
    def trigger(self):
        """
        The only method Render Setup is currently using from the QAction interface
        """
        ...
    exceptionRaised : NoneType
    
    staticMetaObject : PySide2.QtCore.QMetaObject




def excepthookGuard(*args, **kwds): ...

