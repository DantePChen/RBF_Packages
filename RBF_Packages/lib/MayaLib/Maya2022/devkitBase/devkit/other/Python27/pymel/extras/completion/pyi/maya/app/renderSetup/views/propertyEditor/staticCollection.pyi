from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya.app.renderSetup.views.utils as utils
import maya
from . import basicCollection
from . import layout as propEdLayout


from PySide2.QtWidgets import QHBoxLayout
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QWidget
from PySide2.QtGui import QFont
from PySide2.QtWidgets import QListWidget
from PySide2.QtWidgets import QGroupBox


if False:
    from typing import Dict, List, Tuple, Union, Optional

class StaticCollection(basicCollection.BasicCollection):
    """
    This class represents the property editor view of a static collection.
    A static selection is read-only and only contains a list of selected nodes.
    """
    
    
    
    def __init__(self, item, parent): ...
    def setupSelector(self, layout): ...
    LIST_BOX_HEIGHT : float
    
    staticMetaObject : PySide2.QtCore.QMetaObject



