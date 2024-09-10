from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.OpenMayaUI as OpenMayaUI


from PySide2.QtCore import QFileInfo
from functools import partial
from PySide2.QtWidgets import QFileDialog
from PySide2.QtCore import QTimer


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _findFileDialog(): ...
def wrapInstance(*args, **kwargs): ...
def acceptFileDialog(fileName, interval='200'):
    """
    Set up a timer-based event to accept a file dialog with the argument
    file name.
    
    The timer interval is optionally specified in milliseconds.
    """
    ...
def _fileDialogTimerExpired(fileName): ...

