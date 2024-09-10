from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya
import maya.app.renderSetup.views.utils as utils
import maya.OpenMayaUI as mui
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
from . import layout as propEdLayout


from maya.app.general.mayaMixin import MayaQWidgetBaseMixin
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QLabel
from PySide2.QtWidgets import QGroupBox


if False:
    from typing import Dict, List, Tuple, Union, Optional

class BasicCollection(MayaQWidgetBaseMixin, QGroupBox):
    """
    Empty collection property editor UI.
    
    This class provides a very simple property editor UI for a collection.
    It displays the "path" to the collection within the render setup data
    model tree.  It can be used as a base class for more complex collection
    property editor UIs.
    """
    
    
    
    def __init__(self, item, parent): ...
    def paintEvent(self, event): ...
    def preSelector(self): ...
    def setupSelector(self, layout): ...
    staticMetaObject : PySide2.QtCore.QMetaObject




def getCppPointer(*args, **kwargs): ...

