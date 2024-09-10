from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.common.guard as guard
import maya
import maya.app.renderSetup.common.utils as commonUtils
import maya.app.renderSetup.views.utils as utils
import maya.app.renderSetup.model.selector as selector


from maya.app.renderSetup.common.devtools import *


from PySide2.QtGui import QFont
from maya.app.renderSetup.views.propertyEditor.expressionLabels import IncludeExpressionLabel
from PySide2.QtWidgets import QSizePolicy
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QPushButton
from PySide2.QtGui import QIcon
from PySide2.QtWidgets import QWidget
from maya.app.renderSetup.views.renderSetupDelegate import RenderSetupDelegate
from PySide2.QtWidgets import QGroupBox
from maya.app.renderSetup.views.propertyEditor.collectionFilterLineEdit import CollectionFilterLineEdit
from PySide2.QtWidgets import QLineEdit
from maya.app.renderSetup.views.propertyEditor.layout import Layout
from maya.app.renderSetup.views.propertyEditor.collectionStaticSelectionWidget import CollectionStaticSelectionWidget
from PySide2.QtCore import Qt
from PySide2.QtWidgets import QComboBox
from PySide2.QtWidgets import QHBoxLayout
from PySide2.QtWidgets import QCheckBox


if False:
    from typing import Dict, List, Tuple, Union, Optional

class SimpleSelector(object):
    def __init__(self, selector): ...
    def build(self, layout, populate='True'): ...
    def customFilterEntered(*args, **kwargs):
        """
        # See undo module for decorator comments.
        """
        ...
    def displayType(self):
        """
        Return the user-visible display type string.
        
        By default this is the same for all objects of a selector class.
        """
        ...
    def highlight(self, names): ...
    def includeExpressionChanged(self, text): ...
    def includeExpressionEntered(*args, **kwargs):
        """
        # See undo module for decorator comments.
        """
        ...
    def populateFields(*args, **kwargs): ...
    def selectIncludeExpression(self): ...
    def selectStaticEntries(self): ...
    CREATE_EXPRESSION_STRING : unicode
    
    DRAG_DROP_FILTER_STRING : unicode
    
    EXPRESSION_BUTTON_WIDTH : float
    
    INVERSE_STRING : unicode
    
    LIST_BOX_HEIGHT : float
    
    SELECT_STRING : unicode
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kModelType : str


class BasicSelector(SimpleSelector):
    def __init__(self, selector): ...
    def build(self, layout): ...
    def populateFields(*args, **kwargs): ...
    def setIncludeHierarchy(*args, **kwargs):
        """
        # See undo module for decorator comments.
        """
        ...
    kDisplayType : unicode
    
    kModelType : str
    
    kUsage : unicode




def ifNotBlockChangeMessages(f):
    """
    Avoid calling the decorated function if change messages are blocked.
    """
    ...

