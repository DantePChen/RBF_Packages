from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.undo as undo
import weakref
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya.app.renderSetup.views.proxy.renderSetup as rsProxy
import maya.api.OpenMaya as OpenMaya
import maya.mel as mel
import maya.app.renderSetup.views.utils as utils
import maya.app.renderSetup.views.pySide.standardItem as standardItem
import maya.cmds as cmds
import maya
import maya.app.renderSetup.lightEditor.model.editor as editorModel
import maya.app.renderSetup.common.guard as guard
import maya.app.renderSetup.model.renderSetupPrivate as renderSetupPrivate
from . import proxyFactory
import maya.app.renderSetup.model.applyOverride as applyOverride
import maya.app.renderSetup.lightEditor.model.item as itemModel


from PySide2.QtCore import *


from PySide2.QtGui import QGuiApplication
from PySide2.QtGui import QStandardItemModel
from PySide2.QtGui import QColor
from PySide2.QtGui import QFontMetrics
from PySide2.QtGui import QStandardItem
from PySide2.QtGui import QFont
from PySide2.QtWidgets import QApplication


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LightEditorProxy(rsProxy.DataModelListObserver, QStandardItemModel):
    """
    The class provides the Qt model counterpart for the LightEditor model
    """
    
    
    
    def __eq__(self, other: Any) -> bool: ...
    def __init__(self, parent='None'): ...
    def __ne__(self, other: Any) -> bool: ...
    def aboutToDelete(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def attachChild(self, child, pos): ...
    def child(self, row, column='0'): ...
    def createListItemProxy(self, listItem): ...
    def dispose(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def dropMimeData(self, mimeData, action, row, column, parentIndex): ...
    def findProxyItem(self, name): ...
    def flags(self, index): ...
    def mimeData(self, indices):
        """
        This method builds the mimeData if the selection is correct
        """
        ...
    def mimeTypes(self): ...
    def refreshModel(self): ...
    def resetModel(self): ...
    def supportedDropActions(self): ...
    def type(self): ...
    def typeIdx(self): ...
    @property
    def model(self):
        """
        Get the data model object for this proxy item.
        If the data model object does not exist, None is returned.
        """
        ...
    staticMetaObject : QMetaObject


class LabelColor(object):
    """
    Base class for all the proxy classes to offer the label color option
    """
    
    
    
    def getLabelColor(self): ...
    def setLabelColor(self, label): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class LightItemProxyBase(LabelColor, standardItem.StandardItem):
    def __init__(self, model): ...
    def aboutToDelete(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def data(self, role): ...
    def delete(self): ...
    def dispose(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def equalsDragType(self, dragType): ...
    def findProxyItem(self, name): ...
    def genericTypeIdx(self): ...
    def getActionButton(self, column): ...
    def getActionButtonCount(self): ...
    def getDefaultColor(self): ...
    def handleDragMoveEvent(self, event): ...
    def handleDropEvent(self, event, sceneView): ...
    def headingWidth(self, heading): ...
    def isActive(self): ...
    def isCopyable(self): ...
    def isDropAllowed(self, destinationModel): ...
    def isModelDirty(self):
        """
        # The next function (isModelDirty) is a workaround.
        # It should not be necessary but it is currently because we set tooltips in the treeview
        # and that triggers emitDataChanged which triggers the rebuild or repopulate of the property editor.
        # The proper fix will be to use columns in the treeview where each column has its own static tooltip
        # and the tooltips should no longer be dynamically set by the delegate (views/renderSetupDelegate.py)
        # depending on the lastHitAction
        """
        ...
    def modelChanged(*args, **kwargs): ...
    def onClick(self, view): ...
    def onDoubleClick(self, view): ...
    def setData(self, value, role): ...
    def supportsAction(self, action, numIndexes): ...
    @property
    def model(self):
        """
        Get the data model object for this proxy item.
        """
        ...


class LightItemProxy(LightItemProxyBase):
    """
    The class provides the Qt model counterpart for the LightItem
    """
    
    
    
    def __init__(self, model): ...
    def acceptsDrops(self, attribute): ...
    def columnData(self, role, column): ...
    def data(self, role): ...
    def delete(self): ...
    def type(self): ...
    def typeIdx(self): ...


class LightGroupProxy(rsProxy.DataModelListObserver, LightItemProxyBase):
    """
    The class provides the Qt model counterpart for the LightGroup
    """
    
    
    
    def __init__(self, model): ...
    def aboutToDelete(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def acceptsDrops(self, attribute): ...
    def attachChild(self, override, pos): ...
    def createListItemProxy(self, listItem): ...
    def data(self, role): ...
    def dispose(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def getDefaultColor(self): ...
    def type(self): ...
    def typeIdx(self): ...




def getProxy(dataModel): ...


LIGHT_ITEM_TYPE_IDX : int
kSetLocalRender : unicode
LIGHT_ITEM_BASE_TYPE_IDX : int
LIGHT_ITEM_TYPE : int
kFilterCustom : unicode
LIGHT_EDITOR_TYPE_IDX : int
DEFAULT_LIGHT_GROUP_COLOR : QColor
kFilterAll : unicode
kNoOverride : unicode
DEFAULT_LIGHT_ITEM_COLOR : QColor
kCreateShaderOverrideAction : unicode
kDragAndDropFailed : unicode
kFilterTransformsShapesShaders : unicode
kCreateRelativeOverrideAction : unicode
kAbsolute : unicode
kFiltersMenu : unicode
kRelativeType : unicode
kSetEnabledAction : unicode
kCollectionWarningStr : unicode
kFilterGeometry : unicode
kFilterTransformsAndShapes : unicode
LIGHT_TEXT_COLOR_ANIMATED : QColor
kRenderLayerWarningStr : unicode
LIGHT_GROUP_TYPE : int
DISABLED_LIGHT_TEXT_COLOR : QColor
kCameras : unicode
kFilterLights : unicode
LIGHT_EDITOR_MIME_TYPE : str
kOverrideWarningStr : unicode
kFilterShaders : unicode
kCreateCollectionAction : unicode
LIGHT_TEXT_COLOR : QColor
kAOVs : unicode
kCreateRenderSettingsChildCollectionAction : unicode
colors : dict
kRenameAction : unicode
kRenderSettings : unicode
kCreateConnectionOverrideAction : unicode
kDragAndDrop : unicode
LIGHT_EDITOR_TYPE : int
kFilterCameras : unicode
LIGHT_ITEM_BASE_TYPE : int
kAbsoluteType : unicode
kFullyExpandCollapseAction : unicode
kShaderType : unicode
kNewFilter : unicode
kCreateMaterialOverrideAction : unicode
kFilterTransforms : unicode
kMaterialType : unicode
kSetIsolateSelectedAction : unicode
kRelative : unicode
LIGHT_TEXT_COLOR_LOCKED : QColor
kConnectionType : unicode
kSelectionTypeError : unicode
kExpandCollapseAction : unicode
kFilterSets : unicode
kCreateAbsoluteOverrideAction : unicode
kDeleteAction : unicode
kSetRenderableAction : unicode
LIGHT_GROUP_TYPE_IDX : int
kSetVisibilityAction : unicode
kLights : unicode
LIGHT_TEXT_COLOR_OVERRIDEN_BY_US : QColor

