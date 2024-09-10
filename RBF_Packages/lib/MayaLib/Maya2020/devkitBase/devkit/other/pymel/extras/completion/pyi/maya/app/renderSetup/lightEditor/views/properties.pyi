from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya.app.renderSetup.model.plug as plug
import maya
import maya.app.renderSetup.views.utils as utils
import maya.app.renderSetup.lightEditor.model.typeManager as typeMgr
import maya.cmds as cmds
import weakref
import maya.OpenMayaUI as OpenMayaUI
import maya.app.renderSetup.model.utils as modelUtils
import maya.app.renderSetup.model.applyOverride as applyOverride
import maya.app.renderSetup.common.utils as commonUtils


from maya.app.general.mayaMixin import MayaQWidgetBaseMixin
from maya.app.renderSetup.views.propertyEditor.layout import Layout
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QGroupBox


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LightProperties(MayaQWidgetBaseMixin, QGroupBox):
    """
    This class represents the property editor view of a light editor light item.
    """
    
    
    
    def __init__(self, item, parent): ...
    staticMetaObject : PySide2.QtCore.QMetaObject


class GroupProperties(MayaQWidgetBaseMixin, QGroupBox):
    """
    This class represents the property editor view of a light editor group item.
    """
    
    
    
    def __init__(self, item, parent): ...
    staticMetaObject : PySide2.QtCore.QMetaObject




def _createControl(plg, attrLabel, connectable='True', enabled='True'):
    """
    Create a UI control for the given attribute, 
    matching its type and considering if it's connectable.
    """
    ...
def getCppPointer(*args, **kwargs): ...


kEnable : unicode
kIsolate : unicode

