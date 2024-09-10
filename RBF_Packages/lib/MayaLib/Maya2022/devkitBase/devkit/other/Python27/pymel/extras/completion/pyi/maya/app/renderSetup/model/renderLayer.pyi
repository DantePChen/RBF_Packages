from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import traverse
from . import typeIDs
from . import jsonTranslatorUtils
from . import utils
from . import rendererCallbacks
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.common.guard as guard
from . import memberSet
from . import namespace
from . import jsonTranslatorGlobals
from . import sceneObservable
from . import renderSetupPreferences
import maya
import maya.app.renderSetup.common.utils as commonUtils
from . import collection
from . import selector
from . import undo
import maya.cmds as cmds
from . import nodeList
from . import overrideManager
from . import enabled
import maya.app.renderSetup.common.profiler as profiler
import itertools
from . import context
from . import legacyRenderLayer as legacyLayer
from . import childNode


"""
This module provides the render layer class, as well as utility
functions to operate on render layers.


Note:
    In order to better control the update of the render layer, two flags were added 
    to each render layer instance to control the update of 1) the list of nodes owned
    by the legacy layer and 2) the rendering. The controls were introduced to avoid
    performance penalty on any user requests.
    
    The flag RenderLayer.needsMembershipUpdate is set to True when the list of nodes 
    part of the render layer changed meaning that the legacy layer must be updated. 
    The update is managed by an evalDeferred() so it will only be executed during 
    the next idle time. If an update is already planned, 
    the flag RenderLayer.isUpdatingMembership will be True. These flags only apply
    to the visible render layer. No updates are performed on the not visible ones.
    
    The flag RenderLayer.needsApplyUpdate is set to True when the rendering must be updated. 
    The default dirty mechanism of the scene is not enough as the render setup behavior implies
    to sometime apply or unapply some overrides. The first 'not optimized' implementation 
    of the rendering refresh is to impose a switchToLayer() 
    (i.e. unapply and apply all overrides). This flag only applies to the visible render layer. 
    No updates are performed on the not visible ones.
"""


from maya.app.renderSetup.model.renderSetupPrivate import PostApplyCmd
from functools import partial
from maya.app.renderSetup.model.observable import Observable


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderLayerBase(object):
    """
    Abstract base class for RenderLayer and DefaultRenderLayer classes
    Defines functions for toggling visibility and renderability.
    Children must implement:
      - _getLegacyNodeName()
      - getLegacyNode()
      - _updateLegacyRenderLayerVisibility()
      - apply()
      - unapply()
    """
    
    
    
    def __init__(self): ...
    def isRenderable(self): ...
    def isVisible(self): ...
    def makeVisible(self): ...
    def setObjectDeleted(self): ...
    def setRenderable(self, value): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class DefaultRenderLayer(RenderLayerBase, Observable):
    """
    Singleton class to access and modify default render layer properties
    This singleton instance is also observable: it will notify observers
    on visibility and renderability changes.
    
    Singleton instance belongs to renderSetup instance
    Access it using renderSetup.instance().getDefaultRenderLayer()
    """
    
    
    
    def __init__(self): ...
    def apply(*args, **kwargs): ...
    def clearMemberNodesCache(self): ...
    def getChildren(self): ...
    def getLegacyNode(self):
        """
        Return the legacy render layer MObject.
        """
        ...
    def getMemberNodesCache(self): ...
    def hasLightsCollectionInstance(self): ...
    def name(self): ...
    def needsRefresh(self): ...
    def setMemberNodesCache(self, cache): ...
    def unapply(*args, **kwargs): ...


class RenderLayer(RenderLayerBase, nodeList.ListBase, childNode.ChildNode):
    """
    Render layer node.
    
    A render layer has an ordered list of collections.  It can
    optionally have an ordered list of overrides.
    """
    
    
    
    def __init__(self): ...
    def acceptImport(self): ...
    def addDefaultMembers(self, objs): ...
    def addMembers(*args, **kwargs): ...
    def aovCollectionInstance(self):
        """
        Get the AOV collection instance for this render layer,
        creating it if it doesn't exists as long as renderer 
        callbacks are registered for the current renderer.
        """
        ...
    def appendChild(*args, **kwargs): ...
    def appendCollection(*args, **kwargs): ...
    def apply(*args, **kwargs): ...
    def attachChild(self, pos, child):
        """
        Attach a collection at a specific position
        """
        ...
    def attachCollection(self, pos, child):
        """
        Attach a collection at a specific position
        """
        ...
    def attachOverride(self, overrideName): ...
    def clearMemberNodesCache(self): ...
    def copyForClipboard(self):
        """
        # Pasting a render layer that's visible will trigger a layer
        # switch, which can be expensive, and changes the user's currently
        # visible render layer.  Prevent this on copy for clipboard.
        """
        ...
    def createAbsoluteOverride(*args, **kwargs): ...
    def createCollection(*args, **kwargs): ...
    def createConnectionOverride(*args, **kwargs): ...
    def createRelativeOverride(*args, **kwargs): ...
    def descendantAdded(*args, **kwargs): ...
    def detachChild(*args, **kwargs): ...
    def detachCollection(*args, **kwargs): ...
    def findCollection(self, predicate, creator='None'):
        """
        Find the collection of this layer satisfying the predicate function or creates it
        with the creator function if not found and a creator function is specified.
        Function signatures are:
          predicate(collection): returns boolean.
          creator(void) : returns the created node.
        """
        ...
    def findIn(self, nodeNames, includeSelf='True'):
        """
        Generator that returns all the collections in that layer that contain at least on of the 
        object in nodeNames. Optionally also returns self (with includeSelf=True) if the object is in the layer.
        """
        ...
    def getChildren(self):
        """
        Get list of all existing Collections
        """
        ...
    def getCollectionByName(self, collectionName, nested='False'):
        """
        Look for an existing collection by name
        """
        ...
    def getCollections(self):
        """
        Get list of all existing Collections
        """
        ...
    def getCorrespondingCollection(self, nodeName, selectedCollectionName):
        """
        The behavior is to look for Render Settings attribute to add the override
        in the Render Settings collection if it exists, then to use the selected
        collection; otherwise, to create a new collection containing the override.
        """
        ...
    def getDefaultCollection(self):
        """
        Get the default collection where newly created nodes are placed
        """
        ...
    def getEnabledSelectedNodeNames(self):
        """
        Get the names of the layer's DAG node members.
        
        The layer's members are DAG nodes selected by the layer's
        collections, based on whether a collection is enabled or solo'ed. 
        
        @rtype: set
        @return: set of node names. Empty if none found.
        """
        ...
    def getFirstCollectionIndex(self): ...
    def getLegacyNode(self):
        """
        Return the legacy render layer MObject.
        """
        ...
    def getMemberNodesCache(self): ...
    def getMembers(self):
        """
        Get the names of the layer's DAG node members.
        
        The layer's members are DAG nodes selected by the layer's
        collections, based on whether a collection is enabled or solo'ed. 
        
        @rtype: set
        @return: set of node names. Empty if none found.
        """
        ...
    def getNumIsolatedChildren(self): ...
    def getOverrides(self): ...
    def getRenderSettingsChildCollectionByName(self, renderSettingsChildCollectionName, nested='False'):
        """
        Look for an existing render settings collection by name
        """
        ...
    def hasAOVCollectionInstance(self):
        """
        Returns True if this layer has the AOV collection instance created.
        """
        ...
    def hasApplyOverridesRecursive(self): ...
    def hasCollection(self, collectionName): ...
    def hasDefaultCollection(self):
        """
        Get the default collection where newly created nodes are placed
        """
        ...
    def hasLightsCollectionInstance(self):
        """
        Returns True if this layer has the lights collection instance created.
        """
        ...
    def hasRenderSettingsCollectionInstance(self):
        """
        Returns True if this layer has the render settings collection instance created.
        """
        ...
    def isAbstractClass(self): ...
    def isAcceptableChild(self, modelOrData):
        """
        Check if the model could be a child of the render layer model
        """
        ...
    def itemChangedRecursive(self): ...
    def lightsCollectionInstance(self):
        """
        Get the lights collection instance for this render layer,
        creating it if it doesn't exists.
        """
        ...
    def needsRefresh(self):
        """
        Following some changes the instance must be updated.
        """
        ...
    def overridesConnections(self): ...
    def postConstructor(self): ...
    def renderSettingsCollectionInstance(self):
        """
        Get the render settings collection instance for this render layer,
        creating it if it doesn't exists.
        """
        ...
    def setMemberNodesCache(self, cache): ...
    def setName(*args, **kwargs): ...
    def setObjectDeleted(self): ...
    def typeId(self): ...
    def typeName(self): ...
    def unapply(*args, **kwargs): ...
    def update(*args, **kwargs): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    collectionHighest : OpenMaya.MObject
    
    collectionLowest : OpenMaya.MObject
    
    collections : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str
    
    legacyRenderLayer : OpenMaya.MObject
    
    numIsolatedChildren : OpenMaya.MObject




def create(*args, **kwargs): ...
def delete(*args, **kwargs): ...
def _syncLegacyRenderLayers(layerName): ...
def memberTraversal(node):
    """
    Traverse render setup node children to determine layer membership.
    
    During the collection traversal to determine membership, we consider
    the isolate select state of the layer and of collections, and prune
    those collections that are not included by isolate select.
    
    If the node has no children, an empty list is returned.
    """
    ...


kCreateLightsChildCollection : unicode
kCreateAOVChildCollection : unicode
kAOVCollectionNodeCreationFailed : unicode
kUnknownCollection : unicode
kCreateRenderSettingsCollection : unicode
kInvalidCollectionName : unicode
kSetRenderability : unicode
kCollectionAttached : unicode
kCollectionUnicity : unicode
kAttachCollection : unicode
kCreateAOVCollection : unicode
kCreateLightsCollection : unicode
kCollectionDetached : unicode

