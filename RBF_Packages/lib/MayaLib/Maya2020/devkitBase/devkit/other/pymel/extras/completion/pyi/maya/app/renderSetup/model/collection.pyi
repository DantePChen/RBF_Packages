from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import rendererCallbacks
from . import jsonTranslatorUtils
from . import utils
from . import traverse
import maya.api.OpenMaya as OpenMaya
from . import enabled as computeEnabled
from . import namespace
from . import jsonTranslatorGlobals
import re
from . import override
from . import typeIDs
import maya
import maya.app.renderSetup.common.utils as commonUtils
from . import undo
import maya.cmds as cmds
from . import overrideUtils
from . import clipboardData
from . import selector
from . import plug
from . import nodeList
import maya.app.renderSetup.common.profiler as profiler
from . import context
from . import childNode


"""
This module provides the collection class, as well as utility
functions to operate on collections.

The collection owns its associated selector node: on collection
delete, the collection is deleted as well.

Conceptually, a collection fulfills four roles in render setup:

1) It is a container of overrides.  If enabled, the collection will
   apply all its enabled overrides on nodes it selects (see (2)).
2) It selects nodes onto which overrides will be applied.  These nodes
   can be DAG or DG nodes.
3) It is a container of child collections.  Child collections always
   select nodes based on their parent's selected nodes (see (2)).
4) It defines render layer membership.  Members of a render layer can
   only be DAG nodes.  These are always a subset of the nodes selected
   by the collection (see (2)).  The members of the render layer are the
   union of the top-level collection members; children collections can
   exclude or re-include members.  See RenderLayer.getMembers for more
   details (including the effect of isolate select mode).

The application of overrides only obeys enabled / disabled status.

Render layer membership is determined from enabled / disabled, in
conjunction with isolate select.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Collection(nodeList.ListBase, childNode.TreeOrderedItem, childNode.ChildNode):
    """
    Collection node.
    
    A collection has an ordered list of children, and a selector to
    determine nodes to which the children apply.
    
    MAYA-59277: 
      - When we start implementing proper hierarchical collections we 
        need to decide on the relationship between parent and child
        selectors. Do we always consider a parent collection to be the 
        union of its child collections, and propagate the selector 
        information upwards when a child collection is added or changed?
        Or do we go the opposite direction and restrict the child collection
        to use the intersection between its selector and its parent's selector?
    
      - Light child collections always have a single light source member.
        We should utilize this and create a specific selector for such
        use cases for better performance.
    """
    
    
    
    def acceptImport(self): ...
    def activate(self):
        """
        Called when this list item is inserted into the list.
        Override this method to do any scene specific initialization.
        """
        ...
    def ancestorCollections(self):
        """
        Return this collection's ancestors.
        
        Neither the collection itself, nor the render layer, are included
        in the ancestors.  Therefore, a top-level collection has no
        ancestors.
        """
        ...
    def appendChild(*args, **kwargs): ...
    def apply(*args, **kwargs): ...
    def attachChild(*args, **kwargs): ...
    def compute(self, plug, dataBlock): ...
    def createAbsoluteOverride(*args, **kwargs): ...
    def createCollection(self, collectionName):
        """
        Add a child collection to the Collection.
        """
        ...
    def createConnectionOverride(*args, **kwargs): ...
    def createOverride(*args, **kwargs): ...
    def createRelativeOverride(*args, **kwargs): ...
    def deactivate(self):
        """
        Called when this list item is removed from the list.
        Override this method to do any scene specific teardown.
        """
        ...
    def detachChild(*args, **kwargs): ...
    def enabledChanged(self): ...
    def findChild(self, predicate, creator='None'):
        """
        Find the child of this collection satisfying the predicate function or creates it
        with the creator function if not found and a creator function is specified.
        Function signatures are:
          predicate(childNode): returns boolean.
          creator(void) : returns the created node.
        """
        ...
    def getChild(self, childName, cls='"<class \'maya.app.renderSetup.model.childNode.ChildNode\'>"'):
        """
        Look for an existing child by name and optionally class.
        
        @type childName: string
        @param childName: Name of child to look for
        @type cls: class name
        @param cls: Class name for the type of class to look for
        @rtype: Child model instance
        @return: Found instance or throw an exception
        """
        ...
    def getChildren(self, cls='"<class \'maya.app.renderSetup.model.childNode.ChildNode\'>"'):
        """
        Get the list of all children. 
        Optionally only the children matching the given class.
        """
        ...
    def getCollectionByName(self, collectionName, nested='False'): ...
    def getCollections(self): ...
    def getLayerNumIsolatedChildren(self): ...
    def getNumIsolatedAncestors(self): ...
    def getNumIsolatedChildren(self, includeSelf='False'): ...
    def getOverrides(self): ...
    def getRenderLayer(self): ...
    def getSelector(self):
        """
        Return the selector user node for this collection.
        
        If there is no selector node, or if the selector node does not
        have an MPxNode interface (e.g. an unknown node), an exception
        of type Exception is raised.
        """
        ...
    def getSelectorType(self): ...
    def hasApplyOverridesRecursive(self): ...
    def hasChildren(self): ...
    def hasIsolatedAncestors(self, dataBlock='None'): ...
    def hasIsolatedChildren(self, dataBlock='None'): ...
    def hasOverridesRecursive(self): ...
    def isAbstractClass(self): ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the model could be a child
        """
        ...
    def isEmptyRecursive(self):
        """
        Return whether or not the collection or any of its parents is empty
        """
        ...
    def isEnabled(self, dataBlock='None'): ...
    def isIsolateSelected(self, dataBlock='None'):
        """
        Get if isolate selected. Will always return False in batch mode
        """
        ...
    def isSelfAcceptableChild(self):
        """
        Overridden instances that return False, prevent copy/paste of the collection type to itself.
        """
        ...
    def isSelfEnabled(self, dataBlock='None'): ...
    def isTopLevel(self):
        """
        Is the collection's parent a render layer?
        """
        ...
    def isTraversingConnections(self): ...
    def itemChangedRecursive(self): ...
    def overridesConnections(self): ...
    def postApply(*args, **kwargs): ...
    def setIsolateSelected(self, value): ...
    def setSelectorType(self, typeName):
        """
        Sets the selector type of this collection.
        """
        ...
    def setSelfEnabled(self, value): ...
    def typeId(self): ...
    def typeName(self): ...
    def unapply(*args, **kwargs): ...
    def update(*args, **kwargs): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    aSelector : OpenMaya.MObject
    
    childHighest : OpenMaya.MObject
    
    childLowest : OpenMaya.MObject
    
    children : OpenMaya.MObject
    
    enabled : OpenMaya.MObject
    
    isolateSelected : OpenMaya.MObject
    
    kDefaultSelectorTypeName : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str
    
    layerNumIsolatedChildren : OpenMaya.MObject
    
    numIsolatedAncestors : OpenMaya.MObject
    
    numIsolatedChildren : OpenMaya.MObject
    
    parentEnabled : OpenMaya.MObject
    
    selfEnabled : OpenMaya.MObject


class AOVChildCollection(Collection):
    """
    AOV (arbitrary output variable) Child Collection node.
    """
    
    
    
    def __init__(self): ...
    def compute(self, plug, dataBlock): ...
    def containsNodeName(self, nodeName): ...
    def isSelfAcceptableChild(self):
        """
        This code prevents copy/paste of AOV child collections to themselves/other AOV child collections.
        """
        ...
    def setSelectorType(self, typeName): ...
    def typeId(self): ...
    def typeName(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class LightsChildCollection(Collection):
    """
    LightsChildCollection node.
    
    A child collection node specific for one single light source
    and overrides on this light source.
    """
    
    
    
    def __init__(self): ...
    def compute(self, plug, dataBlock): ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the argument can be a child of this collection.
        
        Pasting is prevented because the Light Editor considers only the 
        first override in the LightsChildCollection. Additionally dragging 
        is prevented between overrides in LightsChildCollections to prevent 
        dragging between incompatible LightsChildCollection types 
        (ie. point light, spot light)
        """
        ...
    def setSelectorType(self, typeName): ...
    def typeId(self): ...
    def typeName(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class RenderSettingsChildCollection(Collection):
    """
    Render Settings Sub Collection node.
    """
    
    
    
    def __init__(self): ...
    def appendChild(self, child): ...
    def attachChild(self, pos, child): ...
    def compute(self, plug, dataBlock): ...
    def createCollection(self, collectionName):
        """
        Add a child collection to the Collection.
        """
        ...
    def getRenderSettingsChildCollectionByName(self, renderSettingsChildCollectionName, nested='False'): ...
    def getRenderSettingsChildCollections(self): ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the argument can be a child of this collection.
        """
        ...
    def isTraversingConnections(self): ...
    def setSelectorType(self, typeName): ...
    def superTypeName(self): ...
    def typeId(self): ...
    def typeName(self): ...
    @staticmethod
    def containsNodeName(nodeName): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kSelectorTypeName : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class LightsCollection(Collection):
    """
    LightsCollection node.
    
    A collection node specific for grouping light sources
    and overrides on those light sources.
    
    This collection should have all light sources as member by default. All nodes 
    matching the light classification should be returned by the selector
    on this collection.
    """
    
    
    
    def __init__(self): ...
    def compute(self, plug, dataBlock): ...
    def createCollection(self, collectionName):
        """
        Add a lights child collection to the Collection.
        """
        ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the argument can be a child of this collection.
        
        We want to prevent copying LightsChildCollections in the same 
        LightsCollection at the expense of not being able to copy 
        LightsChildCollections between different LightsCollections.
        """
        ...
    def setSelectorType(self, typeName): ...
    def typeId(self): ...
    def typeName(self): ...
    @staticmethod
    def containsNodeName(nodeName): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class AOVCollection(Collection):
    """
    AOV (arbitrary output variable) parent collection node.
    """
    
    
    
    def __init__(self): ...
    def appendChild(self, child): ...
    def attachChild(self, pos, child): ...
    def compute(self, plug, dataBlock): ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the model could be a child
        """
        ...
    def setSelectorType(self, typeName): ...
    def typeId(self): ...
    def typeName(self): ...
    @staticmethod
    def containsNodeName(nodeName): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class RenderSettingsCollection(Collection):
    """
    Render Settings Collection node.
    
    This collection has an ordered list of children, and a static & const selector
    to determine nodes to which the children apply. The list of nodes is based
    on the selected renderer at the time of creation.
    
    MAYA-66757:
    - A base collection will be needed to factorize commonalities and segregate differences.
    - A static selector is needed which could be the existing static selection or an object set.
    - The name is read-only.
    - The selector content is read-only
    - The render name should be part of the collection so that the settings are clearly linked 
      to the used renderer, or linked using a plug
    """
    
    
    
    def __init__(self): ...
    def appendChild(self, child): ...
    def attachChild(self, pos, child): ...
    def compute(self, plug, dataBlock): ...
    def createCollection(self, collectionName):
        """
        Add a child collection to the Collection.
        """
        ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the argument can be a child of this collection.
        """
        ...
    def setSelectorType(self, typeName): ...
    def typeId(self): ...
    def typeName(self): ...
    @staticmethod
    def containsNodeName(nodeName): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kSelectorTypeName : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str
    
    numIsolatedRenderSettingsChildren : OpenMaya.MObject




def delete(*args, **kwargs): ...
def getAllCollectionClasses():
    """
    Returns the list of Collection subclasses
    """
    ...
def collections(c): ...
def unapply(*args, **kwargs): ...
def create(*args, **kwargs): ...


kSet : unicode
kCollectionMissingSelector : unicode
kChildAttached : unicode
kOverrideCreationFailed : unicode
_overrideTypes : set
kUnknownChild : unicode
kRendererMismatch : unicode
kInvalidChildName : unicode
kChildDetached : unicode
_specialCollectionTypes : set
kIncorrectChildType : unicode

