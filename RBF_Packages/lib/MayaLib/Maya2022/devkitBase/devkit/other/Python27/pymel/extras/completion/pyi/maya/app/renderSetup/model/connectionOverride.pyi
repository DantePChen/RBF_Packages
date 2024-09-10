from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import plug
from . import typeIDs
import maya
import maya.OpenMaya as OpenMaya1_0
from . import utils
from . import dagPath as dagPathUtils
import maya.app.renderSetup.common.utils as commonUtils
from . import undo
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import re
import maya.app.renderSetup.common.guard as guard
from . import context
from . import override
from . import applyOverride
from . import sceneObservable


from maya.app.renderSetup.model.renderSetupPrivate import PostApplyCmd


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DstConnectionHandle(object):
    """
    Plug class that handles and persists a destination connection.
    
    The source of a destination connection can be in a referenced file.  If
    so, the connection is recorded by this class as a string, and stored in
    a dynamic attribute.
    
    On connect, if the source is referenced, store the string
    representation of the source.  On disconnect, if we have a string
    representation, remove it.  On access, if disconnected, check if we
    have a string representation.  If so, use it and connect.
    """
    
    
    
    def __init__(self, node, aDst, srcStrAttrNameLong, srcStrAttrNameShort):
        """
        Create the handle on the argument MObject node, for the destination
        attribute aDst.
        
        If the source is referenced, store its string representation in the
        attribute named srcStrAttrNameLong, srcStrAttrNameShort.
        """
        ...
    def connect(self, src):
        """
        Connect this destination to the argument source MPlug.  If the
        source node is referenced, store a string representation of the
        source.
        """
        ...
    def disconnect(self):
        """
        Disconnect this destination from its source (if any).  If we
        have a string representation of the source, it is removed.
        """
        ...
    def dst(self):
        """
        Return the destination MPlug of this handle.
        
        If disconnected, we check if we have a string representation.  If
        so, we use it and try to re-connect to the source.
        """
        ...
    def src(self):
        """
        Convenience to return the source MPlug of this destination plug
        handle.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ConnectionOverride(override.AbsOverride):
    """
    Attribute connection override node.
    
    This override node type implements an attribute override by setting a
    new value or connection on an attribute.
    
    A value attribute is used to let the user set a value or make a connection to
    the node to be used as override. This attribute is dynamic and created 
    with a type matching the target attribute. The override node is not valid 
    until such a dynamic attribute is created. An override that is not valid 
    will not be applied when its layer is made active. However an override that 
    later becomes valid (the user drags and drops a target attribute), will then
    automatically be applied if the layer is active.
    
    Source connections to the value attribute is encoded/decoded for export/import
    using OverridePlugHandle (in override.py). This class handles missing dependencies
    on decode and remakes the connection when the dependency node is added.
    """
    
    
    
    def __del__(self): ...
    def __init__(self): ...
    def activate(self): ...
    def apply(*args, **kwargs): ...
    def attrChangedCB(self, msg, plg, otherPlug, clientData):
        """
        Update the connection override when attrValue changes.
        """
        ...
    def deactivate(self): ...
    def doAction(*args, **kwargs): ...
    def doSaveOriginal(self, target, storage):
        """
        This method performs saving of original state for a given target
        and a storage plug for storing the state.
        """
        ...
    def getOverridden(self):
        """
        Return the list of nodes being overridden.
        
        The items in the return list are triplets of (MObject, attrName, ovrNext).
        MObject is the object being overridden, attrName is the name of the attribute 
        being overridden and ovrNext is the override node in the position of the next 
        override in the apply override list.
        
        Returns an empty list if no attribute is being overridden.
        """
        ...
    def isApplicable(self): ...
    def onFileOpened(self): ...
    def overridesConnections(self): ...
    def postApply(*args, **kwargs): ...
    def postConstructor(self):
        """
        Method running after the node is constructed. 
        All initialization that will access the MObject or other 
        methods of the MPxNode must be done here. Since the node 
        is not fully created until after the call to __init__
        """
        ...
    def reapply(*args, **kwargs): ...
    def setSource(*args, **kwargs): ...
    def unapply(*args, **kwargs): ...
    def update(*args, **kwargs): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class ApplyConnectionOverride(applyOverride.LeafClass, applyOverride.ApplyOverride):
    """
    Connection override apply class. 
    
    Class for applying all connection overrides. It is similar to apply nodes for value overrides, 
    but with some notable differences. Firstly, since it is generating connections it cannot be connected to 
    the target attribute like value apply nodes. Secondly, there is no numeric values flowing between these 
    nodes. Instead message attributes are used to chain the nodes together and the chain represents the order 
    of priority for the nodes.
    
    When an override needs updating, e.g. if the enabled state is changed, the chain of apply nodes is 
    traversed to find the highest priority enabled apply node. The override action from that node 
    is then executed on the target attribute to perform the override change.
    """
    
    
    
    def __init__(self): ...
    def connectTarget(self, target):
        """
        Connect the argument MPlug source to this node's target plug
        destination, to store it.
        """
        ...
    def extract(self):
        """
        Removes self from the apply override chain. This will trigger an update of the chain.
        """
        ...
    def finalize(self, ovrValuePlug): ...
    def getNextPlug(self): ...
    def getOriginalPlug(self): ...
    def getPrevPlug(self): ...
    def getTarget(self):
        """
        Return the target's plug. This is held by the apply override with the highest priority.
        """
        ...
    def insert(self, target, nextOvr='None'):
        """
        Insert self in the override chain for given target, or start the chain if none exists.
        """
        ...
    def isApplicable(self): ...
    def isEnabled(self): ...
    def moveTargetTo(self, to):
        """
        Move the target of this ApplyConnectionOverride to the argument
        ApplyConnectionOverride.
        """
        ...
    def next(self):
        """
        Returns the next connection override in the chain (higher priority) if any, None otherwise.
        """
        ...
    def postConstructor(self): ...
    def prev(self):
        """
        Returns the previous connection override in the chain (lower priority) if any, None otherwise.
        """
        ...
    def typeId(self): ...
    def typeName(self): ...
    def update(self): ...
    @classmethod
    def create(cls, name): ...
    @staticmethod
    def forwardGenerator(applyNode):
        """
        Generator to iterate on apply override nodes in the direction of
        higher-priority apply override nodes.
        
        See reverseGenerator() documentation. Moving down a chain of apply
        override nodes from lower priority to higher priority means traversing
        the connection from the 'next' plug (source) of the lower-priority
        node to the 'previous' plug (destination) of the higher-priority node.
        """
        ...
    @staticmethod
    def initializer(): ...
    @staticmethod
    def reverseGenerator(applyNode):
        """
        Generator to iterate on apply override nodes in the direction of
        lower-priority apply override nodes.
        
        When more than one override applies to a single overridden attribute, a
        chain of apply override nodes is formed, with the highest priority
        apply override nodes directly connected to the overridden attribute,
        and previous overrides having lower priority.
        
        In such a case, the 'next' plug of a lower-priority apply override node
        is connected to the 'previous' plug of a higher-priority apply override
        node. Moving up a chain of apply override nodes from higher priority
        to lower priority therefore means traversing the connection from the
        'previous' plug (destination) of the higher-priority node to the 'next'
        plug (source) of the lower-priority node.
        """
        ...
    aNext : OpenMaya.MObject
    
    aPrevious : OpenMaya.MObject
    
    aTarget : OpenMaya.MObject
    
    kOriginalLong : str
    
    kOriginalShort : str
    
    kTargetLong : str
    
    kTargetNameLong : str
    
    kTargetNameShort : str
    
    kTargetShort : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class MaterialOverride(ConnectionOverride):
    """
    Material override node.
    
    Specialization of connection override for material (shading engine) assignments.
    
    This override node type implements a material override
    (replace shading engine assignments) for DAG nodes.
    
    Shading group assignments in Maya are represented by connections to the 
    instObjGroups attribute on the shape node. It's an array attribute that represents 
    per-instance assignments and per-face group assignments in the following way:
    
    myShape.instObjGroups[N] - connection to this represents material assignment to
    instance number N.
    
    myShape.instObjGroups[N].objectGroups[M] - connection to this represents assignment 
    to face group M of instance number N.
    
    The connections are made from myShape.instObjGroups[N] -> mySG.dagSetMembers[X],
    where mySG is a shadingEngine node, which represents that this shading engine is
    assigned to that instance of the shape. The dagSetMembers attribute is special and is
    using disconnectBehavior = kDelete which means its array elements are deleted as soon
    as they are disconnected. So we cannot save these element plugs explicitly. Instead we 
    use the message attribute to have a reference to the node. Then we override the
    doAction() and doSaveOriginal() methods to handle the shading engine set assignments.
    
    Since this override type is replacing the whole shadingEngine with a new one,
    it will not preserve any displacement or volume material set on the shadingEngine.
    
    Care must be taken when applying an override to shapes whose original
    material is from a referenced file.  In addition to preserving the
    state of the original material through a connection, we also save the
    name of the material in the apply override node as a string, if the
    material was referenced.
    
    WHen a shading group used for per-face shading is deleted, the compInstObjGroups
    compound multi-attribute is created. It applies the overall object shading group to
    the unassigned faces of the object. When a material override is applied, we need to 
    replace the compInstObjGroup with the shading group of the material override so that
    unassigned faces have the material override's shading group applied to them.
    
    For more information on instObjGroups and compInstObjGroup, see the following wiki page:
    https://wiki.autodesk.com/pages/viewpage.action?spaceKey=~yees&title=Polygon+Per-Face+Material+Assignments+in+Maya
    """
    
    
    
    def __init__(self): ...
    def doAction(*args, **kwargs): ...
    def doSaveOriginal(self, target, storage):
        """
        This method performs saving of original state for a given target
        and a storage plug for storing the state.
        """
        ...
    def isApplicable(self): ...
    def isValid(self): ...
    def postConstructor(self):
        """
        Method running after the node is constructed. 
        All initialization that will access the MObject or other 
        methods of the MPxNode must be done here. Since the node 
        is not fully created until after the call to __init__
        """
        ...
    def setMaterial(*args, **kwargs): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    @staticmethod
    def saveShadingEngine(shadingEngineObj, storagePlug):
        """
        Save a connection to the shading engine node in the storage plug.
        
        This function unconditionally connects the shading engine to the
        storage plug.  It also stores the name of the shading engine as a full
        name with the namespace path in the storage plug's node, if the shading
        engine is not in the main scene.
        """
        ...
    kShadingEngineNameLong : str
    
    kShadingEngineNameShort : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class ConnectionUniqueOverride(override.UniqueOverride, ConnectionOverride):
    @staticmethod
    def initializer(): ...
    kTargetNodeName : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class ShaderOverride(ConnectionOverride):
    """
    Shader override node.
    
    Specialization of connection override for surface shader replacement.
    
    This override node type implements a shader override
    (replace surface shader) for shadingEngines assigned to DAG nodes.
    
    The surfaceShader attribute on shadingEngine nodes holds the shader to 
    use as surface shader for that material. See MaterialOverride docstring
    for how the assignment to shadingEngine is handled.
    
    This class will override the connection to the surfaceShader attribute
    with another shader node specified by the user. Since it is just replacing
    surfaceShader connections and keeps all shadingEngine assignments it will
    preserve displacement and volume shader assignments.
    """
    
    
    
    def __init__(self): ...
    def doAction(*args, **kwargs): ...
    def doSaveOriginal(self, target, storage):
        """
        This method performs saving of original state for a given target
        and a storage plug for storing the state.
        """
        ...
    def isApplicable(self): ...
    def isValid(self): ...
    def postConstructor(self):
        """
        Method running after the node is constructed. 
        All initialization that will access the MObject or other 
        methods of the MPxNode must be done here. Since the node 
        is not fully created until after the call to __init__
        """
        ...
    def setShader(*args, **kwargs): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str




def transferPlug(src, dst):
    """
    Transfer the connection or value set on plug 'src' on to the plug 'dst'.
    """
    ...
def setRestoringOriginal(restoringOriginal): ...
def isRestoringOriginal(): ...
def _transferConnectedPlug(src, dst):
    """
    # Copy-pasted from utils._transferConnectedPlug, so that LEGB lookup will
    # find the transferPlug function in this module, which correctly handles
    # reference edits.
    """
    ...
def handleRestoringOriginalCtx():
    """
    Create and return a context to properly set reference edits.
    
    The context turns reference edits on when restoring the original of a
    connection override.
    """
    ...


kAttrValueAlreadyCreated : unicode
_restoringOriginal : bool
kMaterialOverrideFailure : unicode
kApplyNodeNoRenderLayerConnection : unicode

