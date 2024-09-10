from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya.app.renderSetup.views.utils as utils


from PySide2.QtCore import Qt
from PySide2.QtWidgets import QFormLayout
from PySide2.QtWidgets import QLabel


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Layout(QFormLayout):
    """
    This class implements a special form layout for the property editor in which the label column is of a specific size.
    """
    
    
    
    def __init__(self): ...
    def addRow(self, v1, v2='None'):
        """
        Adds a row to the layout. If v2 is None, then there is no label specified.
        """
        ...
    def insertRow(self, row, v1, v2='None'):
        """
        Inserts a row into the layout. If v2 is None, then there is no label specified.
        """
        ...
    LABEL_COLUMN_WIDTH : float
    
    staticMetaObject : PySide2.QtCore.QMetaObject



