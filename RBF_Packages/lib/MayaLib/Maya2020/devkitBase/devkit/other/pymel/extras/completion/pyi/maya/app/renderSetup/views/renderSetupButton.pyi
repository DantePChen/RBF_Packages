from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
from . import utils


from PySide2.QtGui import QColor
from PySide2.QtCore import QSize
from PySide2.QtGui import QImage
from PySide2.QtGui import QPixmap
from PySide2.QtCore import QEvent
from PySide2.QtWidgets import QStyleOptionButton
from PySide2.QtWidgets import QStyle
from PySide2.QtWidgets import QToolTip
from PySide2.QtGui import QPainter
from PySide2.QtWidgets import QAbstractButton
from PySide2.QtGui import QIcon


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderSetupButton(QAbstractButton):
    """
    This class represents a render setup button which is an icon button that produces a brighter version of the icon when hovered over.
    """
    
    
    
    def __init__(self, parent, icon, size='40.0', isEnabled='True'): ...
    def createOption(self): ...
    def createPixmap(self, option): ...
    def drawControl(self, element, option, painter, widget='None'):
        """
        Draws the icon button
        """
        ...
    def enterEvent(self, event): ...
    def event(self, event): ...
    def generateDisabledIconPixmap(self, pixmap): ...
    def generateHighlightedIconPixmap(self, pixmap): ...
    def generateIconPixmap(self, pixmap, type):
        """
        # Code taken from QadskDarkStyle.cpp - QadskDarkStyle::generatedIconPixmap
        """
        ...
    def generatePixmapActiveIcon(self, iconMode, pixmap, option): ...
    def getGeneratedIconPixmap(self, iconMode, pixmap, option):
        """
        Draws the icon button brighter when hovered over.
        """
        ...
    def isEnabled(self): ...
    def leaveEvent(self, event): ...
    def paintEvent(self, e):
        """
        Draws the render setup button
        """
        ...
    def setEnabled(self, enabled): ...
    def sizeHint(self): ...
    ADJUSTMENT : int
    
    DEFAULT_BUTTON_SIZE : float
    
    DISABLED : int
    
    HIGHLIGHTED : int
    
    HIGH_LIMIT : int
    
    LOW_LIMIT : int
    
    MAX_VALUE : int
    
    staticMetaObject : PySide2.QtCore.QMetaObject




def qRgba(*args, **kwargs): ...
def qAlpha(*args, **kwargs): ...

