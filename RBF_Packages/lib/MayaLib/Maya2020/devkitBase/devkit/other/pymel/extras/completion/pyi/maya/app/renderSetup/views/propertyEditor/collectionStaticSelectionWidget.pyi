from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.views.utils as utils
import maya
import maya.cmds as cmds


from PySide2.QtCore import QSize
from PySide2.QtGui import QTextDocument
from PySide2.QtGui import QPen
from PySide2.QtWidgets import QMenu
from PySide2.QtGui import QTextOption
from PySide2.QtWidgets import QListWidget
from PySide2.QtWidgets import QAction
from PySide2.QtCore import Qt
from PySide2.QtWidgets import QAbstractItemView
from PySide2.QtGui import QPainter
from PySide2.QtWidgets import QStyle
from PySide2.QtGui import QPalette
from PySide2.QtWidgets import QApplication
from PySide2.QtCore import QRect
from PySide2.QtWidgets import QStyledItemDelegate
from PySide2.QtGui import QAbstractTextDocumentLayout
from PySide2.QtWidgets import QStyleOptionViewItem
from PySide2.QtGui import QColor
from functools import partial
from PySide2.QtWidgets import QShortcut
from PySide2.QtGui import QKeySequence


if False:
    from typing import Dict, List, Tuple, Union, Optional

class HTMLDelegate(QStyledItemDelegate):
    def __init__(self, selector): ...
    def paint(self, painter, option, index):
        """
        Renders the delegate using the given painter and style option for the item specified by index.
        """
        ...
    def sizeHint(self, option, index): ...
    staticMetaObject : PySide2.QtCore.QMetaObject


class CollectionStaticSelectionWidget(QListWidget):
    """
    This class implements a widget that displays a list of dropped Outliner objects.
    """
    
    
    
    def __init__(self, selector, parent='None'): ...
    def addEntry(self):
        """
        Adds the selected items to the list
        """
        ...
    def dragEnterEvent(self, event):
        """
        Accepts drag events if the dragged event text contains only commands
        """
        ...
    def dragMoveEvent(self, event):
        """
        Accepts drag move events. Validation is already done in the enter event
        """
        ...
    def dropEvent(self, event):
        """
        Adds the dropped object names to the list (if they don't already exist)
        """
        ...
    def highlight(self, names): ...
    def onDoubleClick(self, item): ...
    def paintEvent(self, e):
        """
        Overrides the paint event to make it so that place holder text is displayed when the list is empty.
        """
        ...
    def populate(self): ...
    def removeEntry(self):
        """
        Removes the selected items from the list
        """
        ...
    def removeFilteredObjects(self): ...
    def removeMissingObjects(self): ...
    def rightClicked(self, point):
        """
        Displays the right click context menu
        """
        ...
    def selectEntry(self):
        """
        Selects the selected items from the list
        """
        ...
    def selectFilteredObjects(self): ...
    def selectMembers(self): ...
    def selectMissingObjects(self): ...
    def setSortingEnabled(self, enabled): ...
    def sizeHintForRow(self, row): ...
    MAX_VISIBLE_ENTRIES : int
    
    MIN_VISIBLE_ENTRIES : int
    
    PLACEHOLDER_TEXT_PEN : QPen
    
    ROW_HEIGHT : float
    
    staticMetaObject : PySide2.QtCore.QMetaObject




kCouldNotSelectMissingObject : unicode

