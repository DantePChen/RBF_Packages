from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore


from PySide2.QtGui import QCursor
from PySide2.QtWidgets import QToolTip
from PySide2.QtWidgets import QMenu


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Menu(QMenu):
    def __init__(self, title, parent='None'): ...
    def handleMenuHovered(self, action): ...
    staticMetaObject : PySide2.QtCore.QMetaObject



