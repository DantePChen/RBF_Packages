from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya.app.renderSetup.views.proxy.renderSetup as renderSetup
import maya.app.renderSetup.views.renderSetupDelegate as renderSetupDelegate
import maya.app.renderSetup.views.utils as utils
import maya.app.renderSetup.lightEditor.model.typeManager as typeMgr
import maya.cmds as cmds
from . import proxy
import maya
import math


from copy import deepcopy
from PySide2.QtGui import QColor
from PySide2.QtCore import Qt
from PySide2.QtGui import QPen


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DataDelegate(object):
    def draw(self, painter, rect, data, mapped): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class LightEditorDelegate(renderSetupDelegate.RenderSetupDelegate):
    """
    This class provides customization of the appearance of items in the model.
    """
    
    
    
    def __init__(self, treeView): ...
    def getTextRect(self, rect, item): ...
    def updateEditorGeometry(self, editor, option, index):
        """
        Sets the location for the double-click editor for renaming light editor entries.
        """
        ...
    LIGHT_ATTR_WIDTH : float
    
    LIGHT_ICON_OFFSET_X : float
    
    LIGHT_ICON_OFFSET_Y : float
    
    LIGHT_ICON_SIZE : float
    
    TEXT_LEFT_OFFSET : float
    
    TEXT_RIGHT_OFFSET : float
    
    dataDelegates : dict
    
    kTooltips : dict
    
    staticMetaObject : proxy.QMetaObject


class IntDelegate(DataDelegate):
    def draw(self, painter, rect, data, mapped): ...


class FloatDelegate(DataDelegate):
    def draw(self, painter, rect, data, mapped): ...


class BoolDelegate(DataDelegate):
    def draw(self, painter, rect, data, mapped): ...


class ColorDelegate(DataDelegate):
    def draw(self, painter, rect, data, mapped): ...
    COLOR_SWATCH_WIDTH : float



