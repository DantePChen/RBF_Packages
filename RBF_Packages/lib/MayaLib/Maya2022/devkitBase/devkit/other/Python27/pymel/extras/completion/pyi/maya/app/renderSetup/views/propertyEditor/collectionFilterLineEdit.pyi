from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import PySide2.QtCore
import maya
import maya.cmds as cmds


from PySide2.QtWidgets import QLineEdit


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CollectionFilterLineEdit(QLineEdit):
    def __init__(self, text='None', parent='None'): ...
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
        Adds the dropped object types to the list
        """
        ...
    staticMetaObject : PySide2.QtCore.QMetaObject




def _textContainsObject(text):
    """
    Does the first line of text contain an object that exists?
    """
    ...

