from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.typeIDs as typeIDs
import maya
import maya.app.renderSetup.model.utils as utils
import maya.app.renderSetup.model.undo as undo
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.model.nodeList as nodeList
from . import item as itemModel
import maya.app.renderSetup.model.childNode as childNode


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LightGroup(nodeList.ListBase, itemModel.LightItemBase):
    """
    Light Editor item for groups
    """
    
    
    
    def __init__(self): ...
    def appendChild(*args, **kwargs): ...
    def attachChild(*args, **kwargs): ...
    def detachChild(*args, **kwargs): ...
    def dispose(self, deleteLight): ...
    def getChildren(self, cls='"<class \'maya.app.renderSetup.model.childNode.ChildNode\'>"'):
        """
        Get the list of all children. 
        Optionally only the children matching the given class.
        """
        ...
    def isAbstractClass(self): ...
    def isAcceptableChild(self, model):
        """
        Check if the model could be a child
        """
        ...
    def rename(self, newName): ...
    @classmethod
    def creator(cls): ...
    @staticmethod
    def initializer(): ...
    firstItem : OpenMaya.MObject
    
    items : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str
    
    lastItem : OpenMaya.MObject




kChildAttached : unicode
kChildDetached : unicode

