from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import weakref


from PySide2.QtWidgets import QGroupBox
from maya.app.general.mayaMixin import MayaQWidgetBaseMixin


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderLayer(MayaQWidgetBaseMixin, QGroupBox):
    """
    This class represents the property editor view of a layer.
    """
    
    
    
    def __init__(self, item, parent): ...
    staticMetaObject : PySide2.QtCore.QMetaObject



