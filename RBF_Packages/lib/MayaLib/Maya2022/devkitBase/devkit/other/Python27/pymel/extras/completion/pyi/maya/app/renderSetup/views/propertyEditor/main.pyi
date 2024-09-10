from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.views.proxy.renderSetupRoles as renderSetupRoles
import maya.app.renderSetup.views.proxy.renderSetup as renderSetup
import maya.app.renderSetup.model.renderSetup as renderSetupModel
import maya.app.renderSetup.views.utils as utils
import maya.cmds as cmds
import weakref
import maya.app.renderSetup.lightEditor.views.proxy as lightEditor
import maya
import PySide2.QtCore as QtCore
from . import collectionFactory


from PySide2.QtWidgets import QWidgetItem
from maya.app.renderSetup.lightEditor.views.properties import GroupProperties
from PySide2.QtCore import QSize
from PySide2.QtWidgets import QVBoxLayout
from PySide2.QtGui import QStandardItem
from maya.app.general.mayaMixin import MayaQWidgetDockableMixin
from PySide2.QtWidgets import QScrollArea
from maya.app.renderSetup.views.frameLayout import FrameLayout
from PySide2.QtWidgets import QWidget
from PySide2.QtCore import QItemSelection
from PySide2.QtCore import QPersistentModelIndex
from maya.app.renderSetup.views.propertyEditor.override import Override
from maya.app.renderSetup.lightEditor.views.properties import LightProperties
from PySide2.QtCore import QTimer
from maya.app.renderSetup.views.propertyEditor.renderLayer import RenderLayer
from functools import partial
from PySide2.QtCore import Slot


if False:
    from typing import Dict, List, Tuple, Union, Optional

class PropertyEditor(MayaQWidgetDockableMixin, QWidget):
    """
    This class represents the property editor which displays the selected render setup item's property information.
    
    
    Note: The Qt should never called any 'deferred' actions because all the design is based on synchronous notifications
          and any asynchronous events will change the order of execution of these events.
    
          For example when the selection in the Render Setup Window is changed (so the Property Editor must be updated). 
          The delete must be synchronous on the 'unselected' layouts otherwise they will be updated along with selected ones. 
          The two main side effects are that lot of unnecessary processings are triggered (those one the deleted layouts) 
          and the infamous 'C++ already deleted' issue appears because the Data Model & Qt Model objects were deleted 
          but not their corresponding Layout (instance used by the Property Editor to display a render setup object).
    """
    
    
    
    def __del__(self):
        """
        This is a workaround for a runtime error raised when the window is closed.
        'Internal C++ object (PropertyEditor) already deleted.
        See MAYA-82966 for details.
        """
        ...
    def __init__(self, treeView, parent, observeRenderSetup='True'): ...
    def aboutToDelete(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def dispose(self):
        """
        Cleanup method to be called immediately before the object is deleted.
        """
        ...
    def highlight(self, names): ...
    def itemChanged(self, item):
        """
        When an item in the model changes, update the control and 
        frameLayout that make use of that item (if one exists).
        """
        ...
    def minimumSizeHint(self): ...
    def populateFields(self, item): ...
    def rebuild(self):
        """
        regenerate our collection/override/layer controls.
        """
        ...
    def renderSetupAdded(self):
        """
        RenderSetup node was created
        """
        ...
    def renderSetupPreDelete(self):
        """
        RenderSetup node is about to be deleted
        """
        ...
    def selectionChanged(self, selected, deselected):
        """
        On selection changed we lazily regenerate our collection/override/layer 
        controls.
        """
        ...
    def setSizeHint(self, size): ...
    def sizeHint(self): ...
    def triggerRebuild(self): ...
    def triggerRepopulate(self, item): ...
    MINIMUM_SIZE : QSize
    
    PREFERRED_SIZE : QSize
    
    staticMetaObject : QtCore.QMetaObject
    
    width : int


class PropertyEditorScrollArea(QScrollArea):
    def sizeHint(self): ...
    STARTING_SIZE : QSize
    
    staticMetaObject : QtCore.QMetaObject



