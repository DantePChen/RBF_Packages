from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import basicCollection
import maya.app.renderSetup.views.utils as utils
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
from . import selectorFactory
import maya.app.renderSetup.model.plug as modelPlug
import maya.app.renderSetup.model.override as override
import maya.app.renderSetup.views.proxy.renderSetup as renderSetup


from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtWidgets import QGroupBox
from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Collection(basicCollection.BasicCollection):
    """
    This class represents the property editor view of a collection.
    """
    
    
    
    def __del__(self): ...
    def __init__(self, item, parent): ...
    def dataModelChanged(self, *posArgs, **kwArgs): ...
    def getModelSelector(self): ...
    def highlight(self, names): ...
    def isAbsoluteMode(self): ...
    def populateFields(self): ...
    def postSelector(self): ...
    def preSelector(self):
        """
        Create UI displayed above selector UI.
        """
        ...
    def setupSelector(self, layout): ...
    staticMetaObject : renderSetup.QMetaObject




kViewCollectionObjects : unicode
kRelativeWarning : unicode
kSelectAll : unicode
kViewAllTooltip : unicode
kViewAll : unicode
kAddOverrideTooltipStr : unicode
vSpc : int
kSelectAllTooltip : unicode
kDragAttributesFromAE : unicode
kNbObjects : unicode
kOK : unicode
kAddOverride : unicode
hSpc : int

