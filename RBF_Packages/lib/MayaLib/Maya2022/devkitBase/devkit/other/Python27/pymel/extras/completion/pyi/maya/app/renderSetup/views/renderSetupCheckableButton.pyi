from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
from . import renderSetupButton


from PySide2.QtGui import QIcon
from PySide2.QtWidgets import QStyle


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderSetupCheckableButton(renderSetupButton.RenderSetupButton):
    """
    This class represents a checkable render setup button. This supports
    2 different pixmaps, one for each state (checked/unchecked)
    """
    
    
    
    def __init__(self, parent, icon, size='40.0', isEnabled='True', isCheckable='False', isChecked='False'): ...
    def createOption(self): ...
    def createPixmap(self, option): ...
    def generatePixmapActiveIcon(self, iconMode, pixmap, option): ...
    def isCheckable(self): ...
    def isChecked(self): ...
    def setCheckable(self, isCheckable): ...
    def setChecked(self, checked): ...
    staticMetaObject : PySide2.QtCore.QMetaObject



