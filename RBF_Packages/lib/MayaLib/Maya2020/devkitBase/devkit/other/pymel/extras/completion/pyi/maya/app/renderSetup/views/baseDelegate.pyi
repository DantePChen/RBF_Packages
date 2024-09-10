from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import weakref
from . import utils


from PySide2.QtGui import QPalette
from PySide2.QtGui import QCursor
from PySide2.QtCore import QRect
from PySide2.QtGui import QColor
from PySide2.QtCore import Qt
from PySide2.QtWidgets import QStyledItemDelegate
from PySide2.QtCore import QPointF
from PySide2.QtGui import QPen
from copy import deepcopy
from PySide2.QtWidgets import QStyle


if False:
    from typing import Dict, List, Tuple, Union, Optional

class BaseDelegate(QStyledItemDelegate):
    """
    This class provides customization of the appearance of items in a Model.
    """
    
    
    
    def __init__(self, treeView): ...
    def drawAction(self, painter, actionName, pixmap, left, top, highlightedColor, drawDisclosure, borderColor): ...
    def drawPixmap(self, painter, pixmap, left, top): ...
    def drawToolbarFrame(self, painter, rect, toolbarCount): ...
    def getTextRect(self, rect, item): ...
    def paint(self, painter, option, index):
        """
        Renders the delegate using the given painter and style option for the item specified by index.
        """
        ...
    ACTION_BORDER : float
    
    ACTION_WIDTH : float
    
    ARROW_COLOR : QColor
    
    BACKGROUND_RECT_LEFT_OFFSET : float
    
    BACKGROUND_RECT_LENGTH : float
    
    BOTTOM_GAP_OFFSET : float
    
    COLLAPSED_ARROW : tuple
    
    COLLAPSED_ARROW_OFFSET : float
    
    COLLECTION_TEXT_RIGHT_OFFSET : float
    
    COLOR_BAR_WIDTH : float
    
    DISABLED_BACKGROUND_IMAGE : utils.QPixmap
    
    DISABLED_HIGHLIGHT_IMAGE : utils.QPixmap
    
    EXPANDED_ARROW : tuple
    
    EXPANDED_ARROW_OFFSET : float
    
    ICON_TOP_OFFSET : float
    
    ICON_WIDTH : float
    
    LAST_ICON_RIGHT_OFFSET : float
    
    LAYER_TEXT_RIGHT_OFFSET : float
    
    RENDERABLE_IMAGE : utils.QPixmap
    
    TEXT_LEFT_OFFSET : float
    
    TEXT_RIGHT_OFFSET : float
    
    VISIBILITY_IMAGE : utils.QPixmap
    
    WARNING_ICON_WIDTH : float
    
    WARNING_IMAGE : utils.QPixmap
    
    staticMetaObject : PySide2.QtCore.QMetaObject




def createPixmap(fileName): ...

