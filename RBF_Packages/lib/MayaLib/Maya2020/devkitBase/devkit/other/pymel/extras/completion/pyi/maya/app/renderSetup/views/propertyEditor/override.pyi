from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya
import maya.app.renderSetup.views.utils as utils
import maya.cmds as cmds
import weakref
import maya.OpenMayaUI as mui


from maya.app.general.mayaMixin import MayaQWidgetBaseMixin
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QWidget
from PySide2.QtCore import Qt
from PySide2.QtWidgets import QGroupBox
from PySide2.QtWidgets import QLabel
from maya.app.renderSetup.common.utils import ReportableException
from maya.app.renderSetup.views.propertyEditor.layout import Layout


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Override(MayaQWidgetBaseMixin, QGroupBox):
    """
    This class represents the property editor view of an override.
    """
    
    
    
    def __init__(self, item, parent): ...
    def paintEvent(self, event): ...
    def update(self): ...
    staticMetaObject : PySide2.QtCore.QMetaObject




def getCppPointer(*args, **kwargs): ...


kIncompatibleAttribute : unicode
kInvalidAttribute : unicode
kDragAttributeFromAE : unicode
kLayer : unicode
kAppliedToUnique : unicode

