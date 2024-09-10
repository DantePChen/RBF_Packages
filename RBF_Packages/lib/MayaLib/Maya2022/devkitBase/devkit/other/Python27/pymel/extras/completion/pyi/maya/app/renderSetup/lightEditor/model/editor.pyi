from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.plug as plug
import maya.app.renderSetup.model.typeIDs as typeIDs
import maya
import maya.app.renderSetup.model.utils as utils
import maya.app.renderSetup.model.namespace as namespace
from . import typeManager as typeMgr
import maya.app.renderSetup.model.undo as undo
import maya.app.renderSetup.common.utils as commonUtils
import maya.api.OpenMaya as OpenMaya
import maya.cmds as cmds
from . import item as itemModel


from maya.app.renderSetup.lightEditor.model.group import LightGroup


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LightEditor(LightGroup):
    """
    Singleton group item that is the root of the light editor items.
    
    The light editor node is a singleton: at most one can exist in a scene.
    It is not implemented as a default node, and therefore is not created
    on file new, but rather created on demand.
    """
    
    
    
    def __init__(self): ...
    def ancestors(self):
        """
        Returns a single-element deque with the render setup node itself.
        """
        ...
    def createGroupItem(*args, **kwargs): ...
    def createLightItem(*args, **kwargs): ...
    def findEditorItem(self, obj): ...
    def isAbstractClass(self): ...
    def parent(self):
        """
        Returns None, as the render setup node is the root of the hierarchy.
        """
        ...
    def postConstructor(self): ...
    def rebuildScene(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str




def _createInstance(*args, **kwargs): ...
def instance():
    """
    Return the light editor singleton node, creating it if required.
    """
    ...
def hasInstance():
    """
    Return true if the light editor node exists
    """
    ...


_LIGHT_EDITOR_NODE_NAME : str
kLightEditorNodeNameMismatch : unicode
_LIGHT_EDITOR_NODE_TYPE : str

