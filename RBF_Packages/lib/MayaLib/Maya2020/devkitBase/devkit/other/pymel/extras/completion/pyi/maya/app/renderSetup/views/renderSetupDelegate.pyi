from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya
from . import utils
from . import baseDelegate
import maya.app.renderSetup.model.selector as selector
import maya.app.renderSetup.views.proxy.renderSetup as renderSetup


from PySide2.QtGui import QPen
from PySide2.QtCore import QRect
from PySide2.QtGui import QBrush
from PySide2.QtWidgets import QLineEdit
from PySide2.QtCore import Qt
from copy import deepcopy
from PySide2.QtGui import QPainter
from PySide2.QtGui import QColor
from PySide2.QtGui import QFontMetrics
from PySide2.QtGui import QCursor


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderSetupDelegate(baseDelegate.BaseDelegate):
    """
    This class provides customization of the appearance of items in the Model.
    """
    
    
    
    def __init__(self, treeView): ...
    def createEditor(self, parent, option, index):
        """
        Creates the double-click editor for renaming render setup entries. The override entry is right aligned.
        """
        ...
    def getTextRect(self, rect, item): ...
    def updateEditorGeometry(self, editor, option, index):
        """
        Sets the location for the double-click editor for renaming render setup entries.
        """
        ...
    @staticmethod
    def getFilterIcon(filter): ...
    DISABLED_IMAGE : utils.QPixmap
    
    DISCLOSURE_IMAGE : utils.QPixmap
    
    HIGHLIGHTED_FILL_OFFSET : int
    
    INFO_COLOR : QColor
    
    INVALID_IMAGE : utils.QPixmap
    
    ISOLATE_IMAGE : utils.QPixmap
    
    LEFT_NON_TEXT_OFFSET : float
    
    RIGHT_NON_TEXT_OFFSET : float
    
    kTooltips : dict
    
    staticMetaObject : renderSetup.QMetaObject




def createFilterPixmaps(): ...


kRenderableSelectionToolTip : unicode
kRenderableToolTip : unicode
kNonRenderableSelectionToolTip : unicode
kNonRenderableToolTip : unicode

