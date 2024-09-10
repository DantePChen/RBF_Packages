from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import traverse
from . import renderLayerSwitchInfo
from . import utils
import maya.app.renderSetup.common.guard as guard
import maya.api.OpenMaya as OpenMaya
import itertools
from . import sceneObservable


"""
This module centralizes the treatment of applying/unapplying/updating 
layers/collections/overrides.

RenderLayers, Collections and Overrides are DG nodes they should technically be 
well-behaved, i.e. they should depend only on their local input attributes to 
compute consistently the same outputs.

In practice, they don't because some collections are traversing connections to 
populate their content and yet these connections may change because of 
previously applied overrides that will insert nodes in the unique global graph 
and/or edit connections in it. This module is tracking apply/unapply/connection 
override update in a global way (context) to insure that collection's content 
are correctly evaluated when they are applied.

Example: if we have something like this:

renderLayer1
  - collection1 with pSphere1
    - collection1_shaders1 gathering shaders assigned to pSphere1 (lambert1)
      - abs override color to yellow
    - materialOverride1 assigning blinn1 to pSphere1
    - collection1_shaders2 gathering shaders assigned to pSphere1 
      (blinn1 if materialOverrides is enabled, lambert1 otherwise)
      - abs override color to green

On apply, collection1_shaders2 must reevaluate its content depending on whether
or not materialOverride1 is enabled. On the other hand, collection1_shaders1 
remains unchanged because it is evaluated before materialOverride1.

This context module ensures that collection1_shaders1 doesn't listen to 
materialOverride1 (on apply, on enable/disable, on connection change) but 
collection1_shaders2 does. 

On apply: 
 > apply renderLayer1 
   > evaluate collection1 content
   > deactivate collection1's selector
   > apply collection1
     > evaluate collection1_shader1 content
     > deactivate collection1_shaders1's selector
     > apply collection1_shaders1
       > override lambert1's color to yellow
     > assign blinn1 to pSphere1
     > evaluate collection1_shader2 content
     > deactivate collection1_shaders2's selector
     > apply collection1_shaders2
       > override blinn1's color to green
   > reactivate all selectors from the first applied element (i.e. renderLayer1)

Now, if renderLayer1 is visible,
materialOverride1 update (enable/disable/connection changed) should:
 > create a PivotGuard on materialOverride1. This will deactivate all the 
   selectors BEFORE materialOverride1 but let the ones after activated.
 > update materialOverride1
 > look if a collection's selector AFTER that materialOverride1 was dirtied as
   they may need to reevaluate their content due to the connection change). 
   In this case, collection1_shaders2 selector will have been dirtied.
   It will then create a PivotGuard around collection1_shaders2 and reapply 
   the partially reapply the layer with that pivot collection.
   The PivotGuard guarantees that collection1 and collection1_shaders1 selectors
   are deactivated and protect these collections from being unapplied/reapplied, 
   since they would remain unchanged anyway.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class BlackList(object):
    """
    Static class to hold black listed collection or override names.
    This is used when layer needs to be partially reapplied to skip over
    blacklisted overrides and overrides in blacklisted collections.
    """
    
    
    
    @staticmethod
    def accepts(override): ...
    @staticmethod
    def add(names): ...
    @staticmethod
    def names(): ...
    @staticmethod
    def remove(names): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class BlackListGuard:
    """
    Guard context to black list some collection and/or override names
    in a "with" statement.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, names): ...


class StackContext(object):
    """
    The stack context keeps track of the application/unapplication/update
    of nodes. The first StackContext calls initiate and conclude. 
    This helps handling complex operations that might need to do some work
    at the very beginning and the very end.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, element): ...
    def conclude(self): ...
    def initiate(self): ...
    @staticmethod
    def empty(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    stack : list


class PivotGuard:
    """
    Protects every override that is before the pivot in application order from
    being unapplied or applied. Also deactivate all the selectors before pivot.
    The pivot can be either a collection or an override.
    This is useful for partially reapplying a layer from a certain point.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, pivot): ...
    def enter(self): ...
    def exit(self): ...


class ApplyLayerContext(StackContext):
    def conclude(self): ...
    def initiate(self): ...


class UnapplyLayerContext(StackContext):
    def conclude(self): ...
    def initiate(self): ...


class PivotContext(StackContext):
    def __init__(self, element): ...
    def conclude(self): ...
    def initiate(self): ...
    changed : NoneType


ApplyOverrideContext = PivotContext
UnapplyOverrideContext = PivotContext
UpdateOverrideContext = PivotContext
class UpdateLayerContext(PivotContext):
    """
    This context shall be used to update connection overrides in a layer.
    This is not equivalent to doing a layer switch from a layer to the same
    layer.
    """
    
    
    
    def conclude(self): ...
    def initiate(self): ...


class ApplyCollectionContext(PivotContext):
    def __enter__(self): ...
    def conclude(self): ...
    def initiate(self): ...


UpdateCollectionContext = ApplyCollectionContext
class UnapplyCollectionContext(PivotContext):
    def conclude(self): ...
    def initiate(self): ...




def unapplyApplyOverride(f): ...
def updateCollection(f): ...
def _getRoot(element):
    """
    Returns the last non-None ancestor of the given node or None if the node
    has no parent.
    """
    ...
def updateOverride(f): ...
def updateApplyOverride(f): ...
def _getRenderLayer(element):
    """
    Returns the node if it is already a render layer.
    Returns the render layer of the given node otherwise or None if the node
    doesn't have a render layer.
    """
    ...
def getRenderLayers():
    """
    Returns all the render layers.
    """
    ...
def applyLayer(f): ...
def beforeEnabled(node): ...
def _getCollectionsBefore(pivot):
    """
    Returns all the collections before a given node pivot.
    (in application order)
    """
    ...
def stateGuards(ignoreReferenceEdit='True', enableSceneObservers='False'): ...
def applyApplyOverride(f): ...
def afterIsolated(node): ...
def selectionChanged(f):
    """
    Decorator for Selectors to allow the context to keep track of dirtied 
    selectors in a PivotContext.
    """
    ...
def afterEnabled(node): ...
def unapplyCollection(f): ...
def updateLayer(f): ...
def beforeIsolated(node): ...
def unapplyOverride(f): ...
def applyOverride(f): ...
def getCollections(layers):
    """
    Returns all the non-blacklisted collections from the layers iterable.
    """
    ...
def getSelectors(collections):
    """
    Returns all the known selector from the collections iterable.
    """
    ...
def applyCollection(f): ...
def unapplyLayer(f): ...

