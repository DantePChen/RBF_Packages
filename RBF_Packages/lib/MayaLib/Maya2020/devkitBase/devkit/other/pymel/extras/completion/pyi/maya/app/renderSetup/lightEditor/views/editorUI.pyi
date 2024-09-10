from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.lightEditor.model.light as lightModel
import maya.app.renderSetup.views.utils as utils
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.views.labelColorConstants as labelColorConstants
import maya.app.renderSetup.model.utils as modelUtils
import maya.app.renderSetup.views.proxy.renderSetup as rsProxy
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya.app.renderSetup.views.pySide.menu as menu
import maya.app.renderSetup.lightEditor.model.group as groupModel
import maya
import maya.app.renderSetup.lightEditor.model.update as updateModel
import maya.app.renderSetup.model.renderSetup as renderSetup
import maya.mel as mel
import maya.app.renderSetup.model.undo as undo
import maya.cmds as cmds
import maya.OpenMayaUI as OpenMayaUI
from . import proxy
import maya.app.renderSetup.lightEditor.model.item as itemModel
import maya.app.renderSetup.lightEditor.model.typeManager as typeMgr
import maya.app.renderSetup.lightEditor.model.editor as editorModel
import maya.app.renderSetup.views.pySide.action as action
import maya.app.renderSetup.common.errorAndWarningDeferrer as errorAndWarningDeferrer
import maya.app.renderSetup.views.propertyEditor.main as propertyEditor


from PySide2.QtCore import QSize
from PySide2.QtWidgets import QMainWindow
from maya.app.renderSetup.views.renderSetupStyle import RenderSetupStyle
from PySide2.QtWidgets import QLayoutItem
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QMenu
from maya.app.general.mayaMixin import MayaQWidgetDockableMixin
from PySide2.QtWidgets import QPushButton
from PySide2.QtGui import QPen
from PySide2.QtWidgets import QTreeView
from PySide2.QtWidgets import QToolTip
from PySide2.QtCore import QEvent
from PySide2.QtWidgets import QLabel
from PySide2.QtCore import QModelIndex
from maya.app.renderSetup.views.renderSetupButton import RenderSetupButton
from PySide2.QtWidgets import QAbstractItemView
from PySide2.QtGui import QPixmap
from PySide2.QtCore import Qt
from PySide2.QtWidgets import QFrame
from PySide2.QtGui import QIcon
from maya.app.renderSetup.lightEditor.views.delegate import LightEditorDelegate
from PySide2.QtCore import QPersistentModelIndex
from PySide2.QtGui import QPainter
from PySide2.QtWidgets import QHBoxLayout
from PySide2.QtWidgets import QWidget
from PySide2.QtGui import QColor
from PySide2.QtWidgets import QMenuBar
from PySide2.QtCore import QItemSelectionModel
from PySide2.QtWidgets import QBoxLayout


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LightEditorCentralWidget(QWidget):
    """
    This class implements the controls inside the light editor window
    """
    
    
    
    def __init__(self, parent): ...
    def aboutToDelete(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def getLightCreator(self, lightType): ...
    def minimumSizeHint(self): ...
    def newGroup(*args, **kwargs): ...
    def newLight(*args, **kwargs): ...
    def renderSetupAdded(self): ...
    def renderSetupPreDelete(self): ...
    def selectionChanged(self): ...
    def sizeHint(self): ...
    BUTTON_SIZE : float
    
    MINIMUM_SIZE : QSize
    
    PREFERRED_SIZE : QSize
    
    staticMetaObject : proxy.QMetaObject


class LightEditorView(QTreeView):
    """
    This class implements the light editor view.
    """
    
    
    
    def __init__(self, parent): ...
    def aboutToDelete(self): ...
    def eventFilter(self, object, event): ...
    def leaveEvent(self, *args, **kwargs): ...
    def mouseDoubleClickEvent(self, event): ...
    def mouseMoveEvent(self, event): ...
    def mousePressEvent(self, event): ...
    def mouseReleaseEvent(self, event): ...
    def paintEvent(self, e):
        """
        Overrides the paint event to make it so that place holder text is displayed when the list is empty.
        """
        ...
    def refreshView(self): ...
    def rowsInserted(self, parent, start, end): ...
    def setExpanded(self, index, state):
        """
        Override from the Qt class
        Expands the group and saves its state
        """
        ...
    def showContextMenu(self, point):
        """
        Rebuild from scratch the context menu
        """
        ...
    HALF_FONT_HEIGHT : float
    
    NO_LIGHTS_IMAGE_SIZE : float
    
    PLACEHOLDER_TEXT_PEN : QPen
    
    staticMetaObject : proxy.QMetaObject


class LightEditorWindow(MayaQWidgetDockableMixin, QWidget):
    """
    This class implements the dockable light editor window.
    """
    
    
    
    def __init__(self): ...
    def aboutToDelete(self): ...
    def minimumSizeHint(self): ...
    def setSizeHint(self, size): ...
    def show(self, *args, **kwargs): ...
    def sizeHint(self): ...
    MINIMUM_SIZE : QSize
    
    PREFERRED_SIZE : QSize
    
    STARTING_SIZE : QSize
    
    staticMetaObject : proxy.QMetaObject
    
    width : int


class LookThroughWindow(MayaQWidgetDockableMixin, QWidget):
    """
    This class implements the look through window.
    """
    
    
    
    def __init__(self): ...
    def aboutToDelete(self): ...
    def lookThroughLight(self, light):
        """
        Opens a model panel with camera looking through the given light.
        """
        ...
    def minimumSizeHint(self): ...
    def setSizeHint(self, size): ...
    def sizeHint(self): ...
    MINIMUM_SIZE : QSize
    
    PREFERRED_SIZE : QSize
    
    STARTING_SIZE : QSize
    
    staticMetaObject : proxy.QMetaObject




def lookThroughWindowChanged(): ...
def lookThroughSelectedLight(): ...
def createLightEditorWindow(restore='False'):
    """
    # Sets up the light editor main window
    """
    ...
def saveWindowState(editor, optionVar): ...
def lookThroughWindowClosed(): ...
def getCppPointer(*args, **kwargs): ...
def createLookThroughWindow(restore='False'): ...
def propertyEditorWindowClosed(): ...
def lightEditorWindowChanged(): ...
def lightEditorWindowClosed(): ...
def propertyEditorWindowChanged(): ...
def createPropertyEditorWindow(restore='False'): ...


kLayerText : unicode
kLookThroughLightsToolTip : unicode
kSnapToSelectedToolTip : unicode
kHelp : unicode
_lightEditorWindow : NoneType
kLightEditorTitle : unicode
kMayaHelp : unicode
kLightEditorHelp : unicode
kDefaultLayerText : unicode
kPropertyEditorToolTip : unicode

