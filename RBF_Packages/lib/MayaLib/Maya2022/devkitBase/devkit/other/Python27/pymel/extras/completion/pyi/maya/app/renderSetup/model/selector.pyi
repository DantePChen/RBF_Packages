from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import typeIDs
import logging
from . import algorithm
import maya.OpenMaya as OpenMaya1_0
from . import plug as plugModel
from . import undo
import maya.cmds as cmds
import weakref
import re
from . import context
import maya
from . import sceneObservable


"""
In the render setup system, a selector is a node that identifies which
nodes (or more properly instances) to apply overrides to.  One selector
node is associated with each collection: the collection is
considered to own its associated selector.  The output of a selector
node is a multi-line string, with one node name (or instance name) per
line.
"""


from maya.app.renderSetup.model.dagPath import *
from maya.app.renderSetup.common.devtools import *


from maya.app.renderSetup.model.selection import Selection
from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Filters(object):
    @staticmethod
    def filterFunction(ftype): ...
    @staticmethod
    def filterName(ftype): ...
    @staticmethod
    def filterTypes(ftype): ...
    @staticmethod
    def filterUIName(ftype): ...
    @staticmethod
    def getFiltersFor(typeName): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kAll : int
    
    kCameras : int
    
    kCustom : int
    
    kGenerators : int
    
    kLights : int
    
    kSets : int
    
    kShaders : int
    
    kShadingEngines : int
    
    kShapes : int
    
    kTransforms : int
    
    kTransformsAndShapes : int
    
    kTransformsShapesShaders : int
    
    names : dict


class StaticSelection(object):
    """
    Class that represents a static selection of nodes, without duplicates (a set).
    It has specialized functions to add/remove/set nodes and optimized query functions.
    """
    
    
    
    def __contains__(self, node):
        """
        Return True if node is in the static selection. False otherwise.
        """
        ...
    def __init__(self, selector):
        """
        Constructor of the StaticSelection.
        "selector" is the BasicSelector this static selection belongs to.
        """
        ...
    def __iter__(self):
        """
        Generator over the all the selection in deterministic order.
        """
        ...
    def __len__(self) -> int:
        """
        Return the length of the static selection (including missing/filtered out items).
        """
        ...
    def add(self, selection):
        """
        Adds the given nodes to the static selection.
        "selection" can either be a MSelectionList or an iterable of node names or dag paths or dag nodes or dependency nodes.
        """
        ...
    def asList(self):
        """
        Returns the static selection items as a list.
        """
        ...
    def asSet(self):
        """
        Returns the static selection items as a set.
        """
        ...
    def decode(self, string):
        """
        Decodes the static selection from a string.
        """
        ...
    def dirtyFilterCB(self):
        """
        Clears filter-related cache.
        """
        ...
    def dirtyMissingCB(self):
        """
        Clears missing objects related cache.
        """
        ...
    def dirtySelectionCB(self):
        """
        Clears all cache.
        """
        ...
    def encode(self):
        """
        Encodes the static selection into a string.
        """
        ...
    def hasFilteredOutObjects(self):
        """
        Returns True if static selection contains filtered out nodes. False otherwise.
        """
        ...
    def hasMissingObjects(self):
        """
        Returns True if static selection contains an object that doesn't exist in the scene. False otherwise.
        """
        ...
    def isFilteredOut(self, node):
        """
        Returns True if given node is in the static selection but filtered out.
        Raises RuntimeError if given node is not in the static selection.
        """
        ...
    def isMissing(self, node):
        """
        Returns True if the given node is in the static selection but missing in the scene.
        Raises RuntimeError if given node is not in the static selection.
        """
        ...
    def onNodeAdded(self, obj): ...
    def onNodeRemoved(self, obj): ...
    def onNodeRenamed(self, obj, oldName): ...
    def onNodeReparented(self, msgType, child, parent): ...
    def remove(self, selection):
        """
        Removes the given nodes from the static selection.
        "selection" can either be a MSelectionList or an iterable of node names or dag paths or dag nodes or dependency nodes.
        """
        ...
    def set(self, selection):
        """
        Sets the given nodes as the new static selection.
        "selection" can either be a MSelectionList or an iterable of node names or dag paths or dag nodes or dependency nodes.
        """
        ...
    def setCache(self, names):
        """
        Caches the static selection items as a list.
        """
        ...
    def setWithoutExistenceCheck(self, selection):
        """
        Sets the given nodes as the new static selection.
        "selection" can either be a MSelectionList or an iterable of node names or dag paths or dag nodes or dependency nodes.
        NOTE: Do not use. Reserved for Render Settings nodes.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class Strategy(object):
    """
    Abstract parent class for strategies for finding nodes given an input Selection.
    items(self, selection) returns an iterable of items found by the strategy.
    An item can be a MObject, a MDagPath or a string (existing node name or existing dag path).
    """
    
    
    
    def contentType(*args, **kwargs): ...
    def create(*args, **kwargs): ...
    def isTraversingConnections(*args, **kwargs): ...
    def items(*args, **kwargs): ...
    def members(self, selector): ...
    def onConnectionChanged(*args, **kwargs): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class _MPxNode(object):
    """
    Base class for user defined dependency nodes.
    """
    
    
    
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addExternalContentForFileAttr(*args, **kwargs):
        """
        addExternalContentForFileAttr(table, attr) -> bool
        
        This method is a helper for derived clases implementing getExternalContent().  It augments the external content info table passed in with an entry describing external content whose location is described by the specified attribute.
        
        The method will not overwrite existing items, i.e. items with the same key. (attribute name).  In this context, overwriting an item means the caller has called this function twice with the same attribute, or that two separate but identically named attributes were used.  If replacing an entry is the desired effect, it is the caller's responsibility to erase the previous item first.
        
        * table [OUT] (MExternalContentInfoTable) - table The table in which the new entry will be added.
        * attr (MObject) - The attribute for which the plug value will be queried for a location.
        
        Returns True if an item was sucessfully added to the table.  False if the attribute does not describe a non-empty location, or an item with the same key was already present in the table.
        """
        ...
    def compute(*args, **kwargs):
        """
        compute(plug, dataBlock) -> self
        
        This method should be overridden in user defined nodes.
        
        Recompute the given output based on the nodes inputs.  The plug represents the data value that needs to be recomputed, and the data block holds the storage for all of the node's attributes.
        
        The MDataBlock will provide smart handles for reading and writing this node's attribute values.  Only these values should be used when performing computations.
        
        When evaluating the dependency graph, Maya will first call the compute method for this node.  If the plug that is provided to the compute indicates that that the attribute was defined by the Maya parent node, the compute method should return None.  When this occurs, Maya will call the internal Maya node from which the user-defined node is derived to compute the plug's value. Returning any othervalue (including self) will tell Maya that this node successfully computed theplug. Raising an exception will tell Maya that this node failed at computingthe plug. Note that in most cases, Maya ignores node compute failures.
        
        In other words, the compute method should return None to get the Maya parent class to compute the plug. It should return self (or any other value) to indicate that the plug was computed successfully.
        
        This means that a user defined node does not need to be concerned with computing inherited output attributes.  However, if desired, these can be safely recomputed by this method to change the behaviour of the node.
        
        * plug (MPlug) - plug representing the attribute that needs to be recomputed.
        * block (MDataBlock) - data block containing storage for the node's attributes.
        """
        ...
    def connectionBroken(*args, **kwargs):
        """
        connectionBroken( plug, otherPlug, asSrc) -> self
        
        This method gets called when connections are broken with attributes of this node.
        
        * plug (MPlug) - attribute on this node.
        * otherPlug (MPlug) - attribute on other node.
        * asSrc (bool) - is this plug a source of the connection.
        """
        ...
    def connectionMade(*args, **kwargs):
        """
        connectionMade(plug, otherPlug, asSrc) -> self
        
        This method gets called when connections are made to attributes of this node.
        
        * plug (MPlug) - attribute on this node.
        * otherPlug (MPlug) - attribute on other node.
        * asSrc (bool) - is this plug a source of the connection.
        """
        ...
    def copyInternalData(*args, **kwargs):
        """
        copyInternalData(node) -> self
        
        This method is overriden by nodes that store attribute data in some internal format.
        
        On duplication this method is called on the duplicated node with the node being duplicated passed as the parameter.  Overriding this method gives your node a chance to duplicate any internal data you've been storing and manipulating outside of normal attribute data.
        
        * node (MPxNode) - the node that is being duplicated.
        """
        ...
    def dependsOn(*args, **kwargs):
        """
        dependsOn( plug, otherPlug) -> bool/None
        
        This method may be overridden by the user defined node. It should only be required to override this on rare occasions.
        
        This method determines whether a specific attribute depends on another attribute.
        
        You should return None to specify that Maya should determines the dependency (default).
        
        This is mainly to define dependency of dynamic attributes, since attributeAffects does not work with dynamic attributes.
        
        * plug (MPlug) - attribute on this node.
        * otherPlug (MPlug) - attribute on other node.
        """
        ...
    def doNotWrite(*args, **kwargs):
        """
        doNotWrite() -> bool
        
        use this method to query the "do not write" state of this proxy node. True is returned if this node will not be saved when the maya model is written out.
        """
        ...
    def forceCache(*args, **kwargs):
        """
        forceCache(ctx=MDGContext::current()) -> MDataBlock
        
        Get the datablock for this node. If there is no datablock then one will be created.
        NOTE: This should be used only in places where fast access to the datablock outside of a compute is critical such as the transformUsing method of MPxSurfaceShape.
        
        * ctx (MDGContext) - The context in which the datablock will be retrieved.
        """
        ...
    def getExternalContent(*args, **kwargs):
        """
        getExternalContent(table) -> self
        
        The table populated by this method must include the location of all the content (files) used by this node, including those that do not exist.  See MExternalContentInfoTable for details.
        
        Keys used to add items to this table will be the same that get passed to setExternalContent through its MExternalContentLocationTable parameter to perform a batched change of content location.
        
        When implementing getExternalContent, you are responsible for forwarding the call to the base class when it makes sense to do so, so that base classes  can also add their external content to the table.
        
        The default implementation does nothing.
        
        * table [OUT] (MExternalContentInfoTable) - Content information table that this method must populate.
        """
        ...
    def getFilesToArchive(*args, **kwargs):
        """
        getFilesToArchive(shortName=False, unresolvedName=False, markCouldBeImageSequence=False) -> list of strings
        
        Use this method to return all external files used by this node. This file list will be used by the File > Archive zip feature, maya.exe -archive and the `file -q -list` mel command.
        
        Only include files that exist.
        
        If shortName is True, return just the filename portion of the path. Otherwise, return a full path.
        
        If unresolvedName is True, return the path before any resolution has been done (i.e leave it as a relative path, include unexpanded environment variables,  tildes, ".."s etc). Otherwise, resolve the file     path and return an absolute path (to resolve with standard Maya path resolution, use MFileObject.resolvedFullName()).
        
        * shortName (bool) - If True, only add the filename of the path.
        * unresolvedName (bool) - If True, add paths before any resolution, rather than absolute paths.
        * markCouldBeImageSequence (bool) - If True, append an asterisk after any file path that could be an image sequence (note: only used by maya.exe -archive).
        """
        ...
    def getInternalValue(*args, **kwargs):
        """
        getInternalValue(plug, dataHandle) -> bool
        
        This method is overridden by nodes that store attribute data in some internal format.
        
        The internal state of attributes can be set or queried using the setInternal and internal methods of MFnAttribute.
        
        When internal attribute values are queried via getAttr or MPlug.getValue() this method is called.
        
        All internal data should respect the current context, which may be obtained from MDGContext::current()
        
        * plug (MPlug) - the attribute that is being queried.
        * dataHandle [OUT] (MDataHandle) - the dataHandle to store the attribute value.
        """
        ...
    def getInternalValueInContext(*args, **kwargs):
        """
        getInternalValueInContext(plug, dataHandle, ctx) -> bool [OBSOLETE]
        
        This method is obsolete. Override MPxNode.getInternalValue instead.
        
        * plug (MPlug) - the attribute that is being queried.
        * dataHandle [OUT] (MDataHandle) - the dataHandle to store the attribute value.
        * ctx (MDGContext) - the context the method is being evaluated in.
        """
        ...
    def internalArrayCount(*args, **kwargs):
        """
        internalArrayCount(plug) -> int
        internalArrayCount(plug, ctx) -> int  [OBSOLETE]
        
        This method is overridden by nodes that have internal array attributes which are not stored in Maya's datablock. This method is used by Maya to determine the non-sparse count of array elements during file IO. If the internal array is stored sparsely, you should return the maximum index of the array plus one. If the internal array is non-sparse then return the length of the array.
        
        This method does not need to be implemented for attributes that are stored in the datablock since Maya will use the datablock size.
        
        If this method is overridden, it should return -1 for attributes which it does not handle. Maya will use the datablock size to determine the array length when -1 is returned.
        
        All internal data should respect the current context, which may be obtained from MDGContext.current()
        
        * plug (MPlug) - the array plug.
        * ctx (MDGContext) - the context, default to MDGContext.current().
        """
        ...
    def isAbstractClass(*args, **kwargs):
        """
        isAbstractClass() -> bool
        
        Override this class to return True if this node is an abstract node. An abstract node can only be used as a base class.  It cannot be created using the 'createNode' command.
        
        It is not necessary to override this method.
        """
        ...
    def isPassiveOutput(*args, **kwargs):
        """
        isPassiveOutput(plug) -> bool
        
        This method may be overridden by the user defined node if it wants to provide output attributes which do not prevent value modifications to the destination attribute. For example, output plugs on animation curve nodes are passive. This allows the attributes driven by the animation curves to be set to new values by the user.
        
        * plug (MPlug) - plug representing output in question.
        """
        ...
    def legalConnection(*args, **kwargs):
        """
        legalConnection(plug, otherPlug, asSrc) -> bool/None
        
        This method allows you to check for legal connections being made to attributes of this node.
        
        You should return None to specify that maya should handle this connection if you are unable to determine if it is legal.
        
        * plug (MPlug) - attribute on this node.
        * otherPlug (MPlug) - attribute on other node.
        * asSrc (bool) - is this plug a source of the connection.
        """
        ...
    def legalDisconnection(*args, **kwargs):
        """
        legalDisconnection(plug, otherPlug, arsSrc) -> bool/None
        
        This method allows you to check for legal disconnections being made to attributes of this node.
        
        You should return None to specify that maya should handle this disconnection if you are unable to determine if it is legal.
        
        * plug (MPlug) - attribute on this node.
        * otherPlug (MPlug) - attribute on other node.
        * asSrc (boool) - is this plug a source of the connection.
        """
        ...
    def name(*args, **kwargs):
        """
        name() -> string
        
        Returns the name of this particular instance of this class.  Each objectin the dependency graph has a name.  This name will be used by the UIand by MEL.
        
        It is not necessary to override this method.
        
        Returns the name of the node
        """
        ...
    def passThroughToMany(*args, **kwargs):
        """
        passThroughToMany(plug, plugArray) -> bool
        
        This method is overriden by nodes that want to control the traversal behavior of some Maya search algorithms which traverse the history/future of shape nodes looking for directly related nodes. In particular, the Artisan paint code uses this method when searching for paintable nodes, and the disk cache code uses this method when searching for upstream cacheFile nodes.
        
        If this method is not implemented or returns False, the base class Maya implementation of this method calls passThroughToOne and returns the results of that call.
        
        * plug (MPlug) - the plug.
        * plugArray (MPlugArray) - the corresponding plugs.
        """
        ...
    def passThroughToOne(*args, **kwargs):
        """
        passThroughToOne(plug) -> plug
        
        This method may be overriden by nodes that have a one-to-one relationship between an input attribute and a corresponding output attribute. This method is used by Maya to perform the following capabilities:
        
        - When this node is deleted, the delete command will rewire the source of the input attribute to the destination of the output attribute if the source and destination are connected to nodes that are not deleted.
        - History traversal algorithms such as the bakePartialHistory command use this method to direct its traversal through a shape's construction history.
        - The base class Maya implementation of passThroughToAll will call this method if passThroughToAll returns False.
        
        * plug (MPlug) - the plug.
        """
        ...
    def postConstructor(*args, **kwargs):
        """
        postConstructor() -> self
        
        Internally maya creates two objects when a user defined node is created, the internal MObject and the user derived object.
        The association between the these two objects is not made until after the MPxNode constructor is called. This implies that no MPxNode member function can be called from the MPxNode constructor.
        The postConstructor will get called immediately after the constructor when it is safe to call any MPxNode member function.
        """
        ...
    def postEvaluation(*args, **kwargs):
        """
        postEvaluation(context, evalNode, evalType) -> None
        
        Clean up node's internal state after threaded evaluation.
        
        After the evaluation graph execution, each node gets a chance to restore / update its internal states.For example, resetting draw state.
        
        This code has to be thread safe, non - blocking and work only on data owned by the node.
        
        This call will most likely happen from a worker thread.
        
        * context (MDGContext) - Context in which the evaluation is happening.
                                 This should be respected and only internal state
                                 information pertaining to it should be modified.
        * evaluationNode (MEvaluationNode) - Evaluation node which contains
                                             information about the dirty plugs the
                                             dirty plugs that were evaluated for this
                                             context.
        * evalType (PostEvaluationType)
          * kEvaluatedIndirectly : The node's compute function handled evaluation.
          * kEvaluatedDirectly   : Evaluation was performed externally and the results injected
                                   back into the node.  This would happen in situations such as
                                   extracting values from an external cache.The node needs to
                                   update any additional internal state based on the new values.
          * kLeaveDirty          : Evaluation was performed without updating this node. Internal
                                   state should be updated to reflect that the node is dirty.
        """
        ...
    def preEvaluation(*args, **kwargs):
        """
        preEvaluation(context, evalNode) -> None
        
        Prepare a node's internal state for threaded evaluation.
        
        During the evaluation graph execution each node gets a chance to reset its internal states just before being evaluated.
        
        This code has to be thread safe, non - blocking and work only on data owned by the node.
        
        The timing of this callback is at the discretion of evaluation graph dependencies and individual evaluators.This means, it should be used purely to prepare this node for evaluation and no particular order should be assumed.
        
        This call will most likely happen from a worker thread.
        
        * context (MDGContext) - Context in which the evaluation is happening.
                                 This should be respected and only internal state
                                 information pertaining to it should be modified.
        * evaluationNode (MEvaluationNode) - Evaluation node which contains
                                             information about the dirty plugs that
                                             are about to be evaluated for the context.
                                             Should be only used to query information.
        """
        ...
    def setDependentsDirty(*args, **kwargs):
        """
        setDependentsDirty(plug, plugArray) -> self
        
        This method can be overridden in user defined nodes to specify which plugs should be set dirty based upon an input plug which Maya is marking dirty. The list of plugs for Maya to mark dirty is returned by the plug array. This method handles both dynamic as well as non-dynamic plugs and is useful in the following ways:
        
        
        
        - Allows attributeAffects-style relationships to be handled for dynamically-added attributes. Since MPxNode.attributeAffects() can only be used with non-dynamic attributes, use of this method allows a way for all attributes of a node to affect one another, both dynamic and non-dynamic.
        
        - Provides more flexible relationships than what is available with MPxNode.attributeAffects(). For example, you may wish to not dirty plugs when the current frame is one. However, as the routine is called during dirty propagation, there are restrictions on what can be done within the routine, most importantly you must not cause any dependency graph computation. For details, see the IMPORTANT NOTE below.
        
        
        
        This method is designed to work harmoniously with MPxNode.attributeAffects() on the same node. Alternately, you can do all affects relationships within a yourNode.setDependentsDirty() implementation.
        
        The body of a user-implemented setDependentsDirty() implementation might look like the following example, which causes the plug called "B" to be set dirty whever plug "A" is changed, i.e. A affects B.
        
        * plug (MPlug) - plug which is being set dirty by Maya.
        * plugArray the programmer should add any plugs which they want to set dirty to this list.
        """
        ...
    def setDoNotWrite(*args, **kwargs):
        """
        setDoNotWrite(bool) -> self
        
        Use this method to mark the "do not write" state of this proxy node.  If set, this node will not be saved when the Maya model is written out. 
        
        NOTES:
        1. Plug-in "requires" information will be written out with the model when saved.  But a subsequent reload and resave of the file will cause these to go away.
        2. If this node is a DAG and has a parent or children, the "do not write" flag of the parent or children will not be set. It is the developer's responsibility to ensure that the resulting scene file is capable of being read in without errors due to unwritten nodes.
        """
        ...
    def setExternalContent(*args, **kwargs):
        """
        setExternalContent(table) -> self
        
        This is useful in the context of content relocation.  This will be called while the scene is being loaded to apply path changes performed externally. Consequently, interaction with the rest of the scene must be kept to a minimum.  It is however valid to call this method outside of scene loading contexts.
        
        The keys in the map must be the same as the ones provided by the node in getExternalContent.  The values are the new locations.
        
        When implementing setExternalContent, you are responsible for forwarding the call to the base class when it makes sense to do so, so that base classes  can also set their external content.
        
        The default implementation does nothing.
        
        * table Key->location table with new content locations.
        """
        ...
    def setExternalContentForFileAttr(*args, **kwargs):
        """
        setExternalContentForFileAttr(attr, table) -> bool
        
        This method is a helper for derived clases implementing setExternalContent().  It assigns a value to a plug with the one from the table whose key is the same as the passed in attribute name.
        
        The method will not write to the plug if the attribute is not found in the  table.
        
        * attr (MObject) - The attribute of the plug we want to write to.
        * table (MExternalContentLocationTable) - A table which may hold or not the value for a given plug.
        
        Returns True if the plug was successfully written to. False if no entry in the table was named after the attribute or if no plug was found.
        """
        ...
    def setInternalValue(*args, **kwargs):
        """
        setInternalValue(plug, dataHandle) -> bool
        
        
        This method is overriden by nodes that store attribute data in some internal format.
        
        The internal state of attributes can be set or queried using the setInternal and internal methods of MFnAttribute.
        
        When internal attribute values are set via setAttr or MPlug.setValue() this method is called.
        
        Another use for this method is to impose attribute limits.
        
        All internal data should respect the current context, which may be obtained from MDGContext::current()
        
        * plug (MPlug) - the attribute that is being set.
        * dataHandle (MDataHandle) - the dataHandle containing the value to set.
        """
        ...
    def setInternalValueInContext(*args, **kwargs):
        """
        setInternalValueInContext(plug, dataHandle, ctx) -> bool  [OBSOLETE]
        
        This method is obsolete. Override MPxNode.setInternalValue instead.
        
        * plug (MPlug) - the attribute that is being set.
        * dataHandle (MDataHandle) - the dataHandle containing the value to set.
        * ctx (MDGContext) - the context the method is being evaluated in.
        """
        ...
    def setMPSafe(*args, **kwargs):
        """
        setMPSafe(bool) -> self
        
        This method is obsolete. Override MPxNode.setSchedulingType instead.
        
        Set a flag to specify if a user defined shading node is safe for multi-processor rendering. For a shading node to be MP safe, it cannot access any shared global data and should only use attributes in the datablock to get input data and store output data. 
        
        NOTE: This should be called from the postConstructor() method for shading node plug-ins only. If a shading node is non-safe, then it will only be useful during single processor rendering.
        """
        ...
    def shouldSave(*args, **kwargs):
        """
        shouldSave(plug) -> bool/None
        
        This method may be overridden by the user defined node.  It should only be required to override this on rare occasions.
        
        This method determines whether a specific attribute of this node should be written out during a file save.  The default behavior is to only write the value if it differs from the default and is not being supplied by a connection.  This behavior should be sufficient in most cases.
        This method is not called for ramp attributes since they should always be written.
        
        * plug (MPlug) - plug representing the attribute to be saved.
        """
        ...
    def thisMObject(*args, **kwargs):
        """
        thisMObject() -> MObject
        
        Returns the MObject associated with this user defined node.  This makes it possible to use MFnDependencyNode or to construct plugs to this node's attributes.
        """
        ...
    def type(*args, **kwargs):
        """
        type() -> int
        
        Returns the type of node that this is.  This is used to differentiate user defined nodes that are derived off different MPx base classes.
        
        It is not necessary to override this method.
        
          kDependNode                    Custom node derived from MPxNode
          kLocatorNode                   Custom locator derived from MPxLocatorNode
          kDeformerNode                  Custom deformer derived from MPxDeformerNode
          kManipContainer                Custom container derived from MPxManipContainer
          kSurfaceShape                  Custom shape derived from MPxSurfaceShape
          kFieldNode                     Custom field derived from MPxFieldNode
          kEmitterNode                   Custom emitter derived from MPxEmitterNode
          kSpringNode                    Custom spring derived from MPxSpringNode
          kIkSolverNode                  Custom IK solver derived from MPxIkSolverNode
          kHardwareShader                Custom shader derived from MPxHardwareShader
          kHwShaderNode                  Custom shader derived from MPxHwShaderNode
          kTransformNode                 Custom transform derived from MPxTransform
          kObjectSet                     Custom set derived from MPxObjectSet
          kFluidEmitterNode              Custom fluid emitter derived from MpxFluidEmitterNode
          kImagePlaneNode                Custom image plane derived from MPxImagePlane
          kParticleAttributeMapperNode   Custom particle attribute mapper derived from MPxParticleAttributeMapperNode
          kCameraSetNode                 Custom director derived from MPxCameraSet
          kConstraintNode                Custom constraint derived from MPxConstraint
          kManipulatorNode               Custom manipulator derived from MPxManipulatorNode
          kClientDeviceNode              Custom threaded device derived from MPxThreadedDeviceNode
          kThreadedDeviceNode            Custom threaded device node
          kAssembly                      Custom assembly derived from MPxAssembly
          kSkinCluster                                  Custom deformer derived from MPxSkinCluster
          kGeometryFilter                               Custom deformer derived from MPxGeometryFilter
                 kBlendShape                                    Custom deformer derived from MPxBlendShape
        """
        ...
    def typeId(*args, **kwargs):
        """
        typeId() -> MTypeId
        
        Returns the TYPEID of this node.
        """
        ...
    def typeName(*args, **kwargs):
        """
        typeName() -> string
        
        Returns the type name of this node.  The type name identifies the node type to the ASCII file format
        """
        ...
    @staticmethod
    def addAttribute(*args, **kwargs):
        """
        addAttribute(attr) -> None
        
        This method adds a new attribute to a user defined node type during the type's initialization.
        
        This method will only work during the static initialization method of the user defined node class.  The initialization method is the one that is passed into  MFnPlugin.registerNode(). The attributes must first be created using one of the MFnAttribute classes, and can then be added using this method.
        
        For compound attributes, the proper way to use this method is by calling it with the parent attribute. If a compound attribute is passed, this method will add all of its children.
        NOTE: A failure will occur if you attempt to call addAttribute() on the children of a compound attribute.
        
        * attr (MObject) - new attribute to add.
        """
        ...
    @staticmethod
    def attributeAffects(*args, **kwargs):
        """
        attributeAffects(whenChanges, isAffected) -> None
        
        This method specifies that a particular input attribute affects a specific output attribute.  This is required to make evaluation efficient.  When an input changes, only the affected outputs will be computed. Output attributes cannot be keyable - if they are keyable, this method will fail.
        
        This method must be called for every attribute dependency when initializing the node's attributes.  The attributes must first be added using the MPxNode.addAttribute() method.  Failing to call this method will cause the node not to update when its inputs change. If there are no calls to this method in a node's initialization, then the compute method will never be called.
        
        This method will only work during the static initialization method of the user defined node class.  The initialization method is the one that is passed into MFnPlugin.registerNode().  As a result, it does not work with dynamic attributes. For an alternate solution which handles dynamic as well as non-dynamic attributes refer to MPxNode.setDependentsDirty.()
        
        * whenChanges (MObject) - input attribute - MObject that points to an input attribute that has already been added.
        * isAffected (MObject) - affected output attribute - MObject that points to an output attribute that has already been added.
        """
        ...
    @staticmethod
    def inheritAttributesFrom(*args, **kwargs):
        """
        inheritAttributesFrom(parentClassName) -> None
        
        This method allows a class of plugin node to inherit all of the attributes of a second class of plugin node.
        
        This method will only work during the static initialization method of the user defined node class and must be called before any other attributes have been added.  The initialization method is the one that is passed into  MFnPlugin.registerNode().
        
        A plugin node may only inherit attributes from one other class of plugin node. Attempting to call this method multiple times within a node's initialization method will result in an error.
        
        Both node classes must be registered using the same MPxNode type, listed in MPxNode.type().
        
        * parentClassName (string) - class of node to inherit attributes from.
        """
        ...
    __new__ : builtin_function_or_method
    
    kAssembly : int
    
    kBlendShape : int
    
    kCameraSetNode : int
    
    kClientDeviceNode : int
    
    kConstraintNode : int
    
    kDeformerNode : int
    
    kDependNode : int
    
    kEmitterNode : int
    
    kEvaluatedDirectly : int
    
    kEvaluatedIndirectly : int
    
    kFieldNode : int
    
    kFluidEmitterNode : int
    
    kGeometryFilter : int
    
    kHardwareShader : int
    
    kHwShaderNode : int
    
    kIkSolverNode : int
    
    kImagePlaneNode : int
    
    kLast : int
    
    kLeaveDirty : int
    
    kLocatorNode : int
    
    kManipContainer : int
    
    kManipulatorNode : int
    
    kMotionPathNode : int
    
    kObjectSet : int
    
    kParticleAttributeMapperNode : int
    
    kPostEvaluationTypeLast : int
    
    kSkinCluster : int
    
    kSpringNode : int
    
    kSurfaceShape : int
    
    kThreadedDeviceNode : int
    
    kTransformNode : int


class Selector(_MPxNode):
    """
    Selector node base class (abstract).
    
    A selector is a node that is responsible for finding nodes and/or dag paths
    given some search criterions.
    """
    
    
    
    def __init__(self): ...
    def activate(self):
        """
        Creates observation callbacks. 
        This is called when a selector is created or connected to a collection.
        Override this method to do any scene specific initialization.
        """
        ...
    def beforeBulkChange(self, *args, **kwargs): ...
    def compute(self, plug, dataBlock): ...
    def contentType(*args, **kwargs): ...
    def deactivate(self):
        """
        Removes all observation callbacks. 
        This is called when a selector is deleted or disconnected from a collection.
        Override this method to do any scene specific deinitialization.
        """
        ...
    def getAbsoluteNames(self):
        """
        # for backward compatibility
        """
        ...
    def getSelectionStandIn(self, nodeName):
        """
        Return a selection string to use as stand-in for the given nodeName.
        Can be overridden by derived classes if another node should be used as 
        stand-in for the given node.
        """
        ...
    def hasDagNodes(self): ...
    def isAbstractClass(self): ...
    def isActivated(self): ...
    def isDirty(self): ...
    def isTraversingConnections(self): ...
    def members(self):
        """
        Return selection content as MDagPath iterable representing the layer members.
        """
        ...
    def minimalClone(self, other):
        """
        Does a minimal copy of other (Selector) to search for the same kind of objects.
        "other" must be the same type as "self".
        To be overriden by subclasses if needed.
        """
        ...
    def names(self):
        """
        Return selection content as name string iterable.
        """
        ...
    def nodes(self):
        """
        Return selection content as MObjects iterable.
        """
        ...
    def onBulkChange(self, *args, **kwargs): ...
    def onFileOpened(self): ...
    def owner(self):
        """
        Find the collection owner of this selector.
        """
        ...
    def parent(self):
        """
        Returns the parent of this selector if any, None otherwise.
        """
        ...
    def paths(self):
        """
        Return selection content as DagPath iterable.
        """
        ...
    def postConstructor(self): ...
    def selectionChanged(*args, **kwargs): ...
    def setParent(self, parent): ...
    def status(self):
        """
        Returns the status of this selector (a string warning/error or None).
        """
        ...
    def templateNodeName(self):
        """
        Returns a node name in this selector's selection.
        """
        ...
    @classmethod
    def affectsOutput(cls, attr): ...
    @classmethod
    def create(cls, name): ...
    @classmethod
    def creator(cls): ...
    @staticmethod
    def createInput(attr, args): ...
    @staticmethod
    def initializer(): ...
    @staticmethod
    def synced(f):
        """
        Decorator for Selector's functions to guarantee selection is up to date (_update() is called if needed).
        Must decorate an instance function (starting with self).
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    aIn
    
    aOut
    
    collection
    
    kDagOnly : int
    
    kDagOrNonDag : int
    
    kNonDagOnly : int
    
    kTypeId
    
    kTypeName : str
    
    onConnectionChanged : NoneType
    
    onNodeAdded : NoneType
    
    onNodeRemoved : NoneType
    
    onNodeRenamed : NoneType
    
    onNodeReparented : NoneType


class CompositeStrategy(Strategy):
    def __init__(self, strategies): ...
    def __str__(self) -> str: ...
    def contentType(self): ...
    def isTraversingConnections(self): ...
    def items(self, selection): ...
    def members(self, selector): ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...


class DagStrategy(Strategy):
    """
    Strategy for searching in the dag hierarchy.
    """
    
    
    
    def __init__(self, filterType): ...
    def contentType(self): ...
    def isTraversingConnections(self): ...
    def items(self, selection): ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...
    @staticmethod
    def create(filterType, customs): ...


class NonDagStrategy(Strategy):
    """
    Concrete strategy that returns non-DAG nodes without searching.
    
        The default strategy is a catch-all for all node type filtering, or for
    custom type filtering.
    """
    
    
    
    def contentType(self): ...
    def isTraversingConnections(self): ...
    def items(self, selection): ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...
    @staticmethod
    def create(filterType, customs): ...


class TraversingConnectionStrategy(Strategy):
    def isTraversingConnections(self): ...


class SetStrategy(TraversingConnectionStrategy):
    """
    Strategy for finding sets containing the DAG paths in the selection.
    
    This strategy will return the union of sets containing DAG paths
    in its input, and sets directly in its input.
    """
    
    
    
    def contentType(self): ...
    def items(self, selection): ...
    def members(self, selector):
        """
        Return all dag members of the sets in the selector recursively.
        """
        ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...
    @staticmethod
    def create(filterType, customs): ...


class ShadingStrategy(TraversingConnectionStrategy):
    """
    Strategy for finding shading nodes in shading networks of assigned materials.
    """
    
    
    
    def __init__(self, surface, displace, volume, network): ...
    def contentType(self): ...
    def items(self, selection): ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...
    @staticmethod
    def create(filterType, customs): ...


class ShadingEngineStrategy(TraversingConnectionStrategy):
    """
    Strategy for finding assigned materials (shading engines).
    
    This strategy will return the union of shading engines assigned to shapes
    in its input, and shading engines directly in its input.
    """
    
    
    
    def contentType(self): ...
    def items(self, selection): ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...
    @staticmethod
    def create(filterType, customs): ...


class SimpleSelector(Selector):
    """
    Selector node with both dynamic wildcard-based selection and static
    list of names selection.
    
    Output is the union of both selections (dynamic and static).
    
    This class maintains caches for its static and dynamic selections.
    These caches become invalid (see selectionChanged) when the selector
    observes nodes being added or removed, or for selectors that traverse
    connections, when the selector observes connections being made or
    removed (see isTraversingConnections).  The caches are updated when the
    selector node compute is called.
    
    This observation of scene changes can be costly, for large numbers of
    changes, and is in large part unnecessary during layer switch, where
    unapply and apply are called.  Unapply deletes apply override nodes, and
    apply creates apply override nodes, none of which are interesting to
    selectors.  Unapply disconnects apply override nodes, and performs
    disconnections for connection overrides.  In some circumstances (see the
    context module documentation), this can affect the result of selectors
    that traverse connection.  Finally, apply connects apply override nodes
    and performs connections for connection overrides.  Connections made by
    lower priority collections can affect the result of higher priority
    collections that traverse connections (again, see the context module
    documentation).
    """
    
    
    
    def __init__(self): ...
    def acceptsType(self, typeName, dataBlock='None'): ...
    def contentType(self): ...
    def getCustomFilterValue(self, dataBlock='None'): ...
    def getDynamicNames(*args, **kwargs): ...
    def getFilterType(self, dataBlock='None'): ...
    def getInvalidFilters(self, dataBlock='None'): ...
    def getPattern(self, dataBlock='None'): ...
    def getStaticNames(*args, **kwargs): ...
    def getStaticSelection(self):
        """
        Deprecated method.
        
        Use methods on SimpleSelector.staticSelection instead.
        """
        ...
    def getTypeFilters(self, dataBlock='None'): ...
    def hasFilteredOutObjects(self): ...
    def hasMissingObjects(self): ...
    def isAbstractClass(self): ...
    def isEmpty(self): ...
    def isTraversingConnections(self):
        """
        Returns True if this selector traverses connections to populate its content, False otherwise.
        """
        ...
    def members(*args, **kwargs): ...
    def minimalClone(self, other): ...
    def names(*args, **kwargs): ...
    def nodes(*args, **kwargs): ...
    def onBulkChange(self, *args, **kwargs): ...
    def onConnectionChanged(self, srcPlug, dstPlug, made): ...
    def onNodeAdded(self, obj):
        """
        # CALLBACKS
        # TODO optimize pattern check
        """
        ...
    def onNodeRemoved(self, obj): ...
    def onNodeRenamed(self, obj, oldName): ...
    def onNodeReparented(self, msgType, child, parent): ...
    def paths(*args, **kwargs): ...
    def patterns(self, dataBlock='None'): ...
    def selection(self): ...
    def setCustomFilterValue(self, val): ...
    def setFilterType(self, val): ...
    def setPattern(self, val): ...
    def setStaticSelection(self, ss):
        """
        Deprecated method.
        
        Use methods on SimpleSelector.staticSelection instead.
        """
        ...
    def shapes(*args, **kwargs): ...
    def status(self, dataBlock='None'): ...
    def strategy(self, dataBlock='None'): ...
    def templateNodeName(self):
        """
        Returns a node name in this selector's selection or a node in the scene that 
        would be accepted by this selector if empty.
        """
        ...
    @classmethod
    def getAvailableFilters(cls): ...
    @classmethod
    def getDefaultFilter(cls): ...
    @classmethod
    def initializer(cls): ...
    @property
    def staticSelection(self): ...
    aCustomFilterValue
    
    aPattern
    
    aStaticSelection
    
    aTypeFilter
    
    kTypeId
    
    kTypeName : str


class GeneratorStrategy(TraversingConnectionStrategy):
    """
    Strategy for finding geometry generators.
    
    This strategy will return the union of geometry generators creating shapes
    in its input, and geometry generators directly in its input.
    """
    
    
    
    def contentType(self): ...
    def items(self, selection): ...
    def onConnectionChanged(self, selector, srcPlug, dstPlug, made): ...
    @staticmethod
    def create(filterType, customs): ...


class LightsCollectionSelector(SimpleSelector):
    """
    A custom selector only to be used with the LightsCollection and LightsChildCollection.
    Will select light source and light groups, but also add in any light editor items 
    attached to light sources. This makes it possible to override attributes on both the
    light source and its corresponding light editor item.
    """
    
    
    
    def __init__(self): ...
    def getSelectionStandIn(self, nodeName): ...
    def isAbstractClass(self): ...
    def isTraversingConnections(self): ...
    def onConnectionChanged(self, srcPlug, dstPlug, made): ...
    def typeId(self): ...
    def typeName(self): ...
    @classmethod
    def initializer(cls): ...
    kTypeId
    
    kTypeName : str


class BasicSelector(SimpleSelector):
    def getIncludeHierarchy(self): ...
    def isAbstractClass(self): ...
    def onConnectionChanged(self, **kwargs): ...
    def onNodeAdded(self, **kwargs): ...
    def onNodeRemoved(self, **kwargs): ...
    def onNodeRenamed(self, **kwargs): ...
    def onNodeReparented(self, **kwargs): ...
    def setIncludeHierarchy(self, val): ...
    def shapes(*args, **kwargs): ...
    def status(self): ...
    @classmethod
    def getAvailableFilters(cls): ...
    @classmethod
    def getDefaultFilter(cls): ...
    @classmethod
    def initializer(cls): ...
    aIncludeHierarchy
    
    kForceCompute : bool
    
    kTypeId
    
    kTypeName : str




def createClassificationFilter(classification): ...
def createTypeFilter(types): ...
def isRenderSetupOrLegacyRenderLayerNode(node):
    """
    Returns whether or not the node is a Render Setup or Legacy Render Layer node.
    """
    ...
def createCustomFilter(customs):
    """
    "customs" is a string of types and classifications. ex: 'shader/surface -blinn'
    returns a filter function over a node name that returns True if the name passes the custom filtering, False otherwise.
    """
    ...
def ls(patterns, types='None'):
    """
    Returns a set containing all the nodes matching patterns and types.
    """
    ...
def selectionToAbsoluteNames(selection, permissive='False'):
    """
    Generator that converts selected nodes to long names.
    i.e. absolute paths for dag nodes or instances and names for dependency (non-dag) nodes.
    "selection" can either be a MSelectionList or an iterable of nodes.
    if permissive, invalid nodes names (strings) are kept.
    """
    ...
def getRSExcludes():
    """
    Returns the list of Render Setup and Legacy Render Layer node types
    names of interest, with the exclude symbol in front. The list only
    needs to be computed once, especially since it will be used often.
    """
    ...
def create(name, typeid): ...
def _nodesToLongNames(selection, permissive='False'):
    """
    Generator that converts name/MObject/MDagPath/MDagNode/MFnDependencyNode to long names.
    i.e. absolute paths for dag nodes or instances and names for dependency (non-dag) nodes.
    NOTE: All Render Setup nodes are filtered out of the resulting generator.
    """
    ...
def findSetMembersRecursively(members):
    """
    Return a generator consisting of the dagPaths of all members of the set
    and its nested sets.
    """
    ...
def _mSelectionListToAbsoluteNames(selection):
    """
    Generator that converts nodes in a MSelectionList to long names.
    i.e. absolute path for dag nodes or instances and names for dependency (non-dag) nodes.
    """
    ...


rsExcludes : list
kHasMissingObjects : unicode
kParentMissing : unicode
kNodeToAbsoluteNameFailure : str
kNodeNotInStaticSelection : str
kInvalidParent : unicode
logger : logging.Logger
kSet : unicode

