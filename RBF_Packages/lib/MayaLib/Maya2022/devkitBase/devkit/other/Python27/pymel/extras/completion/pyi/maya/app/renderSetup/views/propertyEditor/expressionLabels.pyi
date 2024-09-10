from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya


from PySide2.QtWidgets import QMenu
from PySide2.QtCore import Signal
from PySide2.QtWidgets import QLabel
from PySide2.QtCore import Qt


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ExcludeExpressionLabel(QLabel):
    """
    This class represents an exclude expression label which has a right click menu used to create additional exclude expressions.
    """
    
    
    
    def __init__(self, text): ...
    def addExcludeExpression(self): ...
    def createStandardContextMenu(self, position):
        """
        Creates the right-click menu for creating additional exclude expressions.
        """
        ...
    excludeExpressionAdded : Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class IncludeExpressionLabel(QLabel):
    """
    This class represents an include expression label which has a right click menu used to create additional include expressions.
    """
    
    
    
    def __init__(self, text): ...
    def addIncludeExpression(self): ...
    def createStandardContextMenu(self, position):
        """
        Creates the right-click menu for creating additional include expressions.
        """
        ...
    includeExpressionAdded : Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject



