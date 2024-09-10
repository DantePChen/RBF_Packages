from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya.app.renderSetup.model.plug as plug
import maya
import maya.mel as mel
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
import maya.OpenMayaUI as omui


from PySide2.QtWidgets import QSizePolicy
from PySide2.QtWidgets import QPushButton
from PySide2.QtGui import QMouseEvent
from PySide2.QtWidgets import QWidget
from PySide2.QtWidgets import QLabel
from PySide2.QtWidgets import QLineEdit
from PySide2.QtCore import Qt
from PySide2.QtGui import QIcon
from PySide2.QtWidgets import QHBoxLayout
from maya.app.renderSetup.model.progressObservable import ProgressObservable
from functools import partial
from PySide2.QtGui import QPixmap
from PySide2.QtWidgets import QFrame


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ProgressBar(object):
    def __init__(self): ...
    def createProgressBar(self): ...
    def endProgressBar(self): ...
    def reactToItemChangedNotification(self, *posArgs, **kwArgs):
        """
        The subject of observation sends messages in the form of classes to
        indicate that progress has started or ended. Otherwise, it sends
        information about the progress estimated to be done
        (as a value from 0 to 1) and a message to be displayed.
        """
        ...
    def registerAsProgressObserver(self): ...
    def stepProgressBar(self, progress, info): ...
    def unregisterAsProgressObserver(self): ...
    __dict__ : dictproxy
    
    
    
    __weakref__ : getset_descriptor


class NodeListView:
    def __init__(self, title): ...
    def buildViewObjects(self, names): ...
    def onOKButton(self, data, msg): ...
    def onSelectAllButton(self, data, treeView, names): ...
    def selectTreeCallBack(self, *args): ...
    def show(self, names): ...


class LabelFieldButtonGrp(QWidget):
    """
    Same as cmds.textFieldButtonGrp, but with better controls on each different widgets.
    (ex: more control on callbacks, tooltips and such).
    """
    
    
    
    def __init__(self, label='None', text='None', placeholder='None', tooltip='None', button='None'): ...
    @property
    def button(self): ...
    @property
    def field(self): ...
    @property
    def label(self): ...
    @property
    def layout(self): ...
    staticMetaObject : PySide2.QtCore.QMetaObject


class Separator(QWidget):
    """
    Same as cmds.separator(), except it allows to add a text in the middle of the separator.
    Ex: -------------- My Section --------------
    """
    
    
    
    def __init__(self, text='None'): ...
    staticMetaObject : PySide2.QtCore.QMetaObject




def updateMouseEvent(event): ...
def wrapInstance(*args, **kwargs): ...
def getExpandedState(node):
    """
    Retrieves the expanded state attribute of the node
    """
    ...
def createPixmap(imageName, width='0', height='0'): ...
def createIconWithOnOffStates(pixmapOffState, pixmapOnState):
    """
    Create an icon with on/off states. Each state has a different pixmap.
    """
    ...
def createIcon(iconName): ...
def browse(fileNameAttr): ...
def dpiScale(value): ...
def setExpandedState(node, value):
    """
    Sets an attribute on the node storing the expanded state of
    this node in the view. Creates it if it doesn't exist
    """
    ...


kSelectAll : unicode
kImageNotFoundError : unicode
kIconNotFoundError : unicode
kNbObjects : unicode
kExpandedStateString : str
kOK : unicode
_DPI_SCALE : float

