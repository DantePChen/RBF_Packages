from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import weakref
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya.app.renderSetup.views.proxy.renderSetup as renderSetup
from . import utils


from PySide2.QtGui import QFontMetrics
from PySide2.QtWidgets import QVBoxLayout
from maya.app.renderSetup.views.baseDelegate import BaseDelegate
from PySide2.QtGui import QPen
from PySide2.QtCore import Signal
from PySide2.QtGui import QPainter
from PySide2.QtGui import QBrush
from PySide2.QtWidgets import QLineEdit
from PySide2.QtCore import Qt
from PySide2.QtCore import QRect
from PySide2.QtWidgets import QHBoxLayout
from PySide2.QtGui import QColor
from PySide2.QtWidgets import QFrame
from PySide2.QtGui import QTextOption


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Frame(QFrame):
    def __init__(self, item, parent): ...
    staticMetaObject : renderSetup.QMetaObject


class FrameLayout(QFrame):
    """
    This class defines the FrameLayout which emulates the behaviour of the Maya Python API's maya.cmds.frameLayout function. 
     The only difference is that the render setup version also provides a color bar and background color to differentiate layers, 
     collections, and overrides.
    """
    
    
    
    def __init__(self, item, parent): ...
    def addWidget(self, widget): ...
    def getWidget(self, index): ...
    def initContentFrame(self): ...
    def initFrameLayout(self): ...
    def initMainLayout(self): ...
    def initTitleFrame(self): ...
    def toggleCollapsed(self):
        """
        When the title frame is clicked, this function is called to toggle the collapsed state
        """
        ...
    staticMetaObject : renderSetup.QMetaObject


class NameLineEdit(QLineEdit):
    """
    This class is used to display the editable name associated with a 
    collection, layer, or override.
    """
    
    
    
    def __init__(self, item, parent): ...
    def focusInEvent(self, event): ...
    def focusOutEvent(self, event): ...
    def keyPressEvent(self, event): ...
    def nameChanged(self): ...
    def paintEvent(self, event): ...
    staticMetaObject : renderSetup.QMetaObject


class TitleFrame(Frame):
    """
    This class defines the frame for the FrameLayout
    """
    
    
    
    def __init__(self, item, parent): ...
    def mousePressEvent(self, event): ...
    def paintEvent(self, e):
        """
        Draws the disabled or enabled title frame background.
        """
        ...
    FRAME_HEIGHT : float
    
    clicked : Signal
    
    staticMetaObject : renderSetup.QMetaObject


class CollapsibleArrowAndTitle(Frame):
    """
    This class defines the arrow used for a FrameLayout
    """
    
    
    
    def __init__(self, item, parent): ...
    def paintEvent(self, e):
        """
        Draws the color bar and arrow
        """
        ...
    def setArrow(self, isCollapsed):
        """
        Sets the arrow direction
        """
        ...
    ARROW_COLOR : QColor
    
    COLLAPSED_ARROW_OFFSET : float
    
    COLOR_BAR_HEIGHT : float
    
    COLOR_BAR_WIDTH : float
    
    EXPANDED_ARROW_OFFSET : float
    
    HEIGHT : float
    
    NODE_TYPE_TEXT_RECT : QRect
    
    WIDTH : float
    
    staticMetaObject : renderSetup.QMetaObject



