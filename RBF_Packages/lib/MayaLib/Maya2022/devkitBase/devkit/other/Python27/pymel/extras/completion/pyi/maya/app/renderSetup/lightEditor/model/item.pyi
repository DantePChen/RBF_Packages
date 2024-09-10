from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.typeIDs as typeIDs
import maya
import maya.app.renderSetup.model.enabled as computeEnabled
import maya.app.renderSetup.model.utils as utils
import maya.app.renderSetup.model.undo as undo
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
from . import update as updateModel
import maya.app.renderSetup.model.childNode as childNode


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LightItemBase(childNode.TreeOrderedItem, childNode.ChildNode):
    """
    Base class for light editor items
    """
    
    
    
    def __init__(self): ...
    def activate(self): ...
    def compute(self, plug, dataBlock): ...
    def deactivate(self): ...
    def detachFromParent(self): ...
    def dispose(self, deleteLight): ...
    def enabledChanged(self): ...
    def getChildren(self, cls='"<class \'maya.app.renderSetup.model.childNode.ChildNode\'>"'): ...
    def getLayerNumIsolatedChildren(self): ...
    def getNumIsolatedAncestors(self): ...
    def getNumIsolatedChildren(self, includeSelf='False'): ...
    def hasIsolatedAncestors(self, dataBlock='None'): ...
    def hasIsolatedChildren(self, dataBlock='None'): ...
    def isAbstractClass(self): ...
    def isAcceptableChild(self, model):
        """
        Check if the model could be a child
        """
        ...
    def isEnabled(self): ...
    def isIsolateSelected(self, dataBlock='None'): ...
    def isSelfEnabled(self, dataBlock='None'): ...
    def postConstructor(self): ...
    def setIsolateSelected(self, value): ...
    def setSelfEnabled(self, value): ...
    def typeId(self): ...
    def typeName(self): ...
    def updateIsolateState(self, numIsolatedAncestors='0'):
        """
        Update the isolate state in the item hierarchy.
        
        Pushing the number of isolated ancestors down the tree,
        and pulling the number of isolated children back up.
        """
        ...
    @classmethod
    def creator(cls):
        """
        # Awkwardly, abstract base classes seem to need a creator method.
        """
        ...
    @staticmethod
    def initializer(): ...
    ATTRIBUTE_COLOR : int
    
    ATTRIBUTE_EXPOSURE : int
    
    ATTRIBUTE_INTENSITY : int
    
    enabled : OpenMaya.MObject
    
    isolateSelected : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str
    
    layerNumIsolatedChildren : OpenMaya.MObject
    
    numIsolatedAncestors : OpenMaya.MObject
    
    numIsolatedChildren : OpenMaya.MObject
    
    parentEnabled : OpenMaya.MObject
    
    selfEnabled : OpenMaya.MObject




def createItem(*args, **kwargs): ...
def deleteItem(*args, **kwargs): ...
def getLightItemName(lightShapeObj): ...


kSet : unicode

