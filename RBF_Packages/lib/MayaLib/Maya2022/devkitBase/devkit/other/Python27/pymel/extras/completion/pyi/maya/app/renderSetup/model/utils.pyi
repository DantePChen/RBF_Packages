from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import plug
from . import typeIDs
import maya
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya


if False:
    from typing import Dict, List, Tuple, Union, Optional

class EncodeMasterValuesCtxMgr(object):
    """
    Unapply all overrides from the list of nodes that depends on the
    argument passed as argument to the constructor of the guard on
    __enter__ and postApply them on __exit__. This is so that we do not
    encode the overriden values, but their initial ones from the
    master layer. This is only an issue if the visible layer is not the
    master layer.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, getAllNodesFunction): ...
    def unapplyCorrespondingOverrideAndAddToCache(self, applyOvr): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def isSurfaceShaderType(typeName): ...
def isInheritedType(parentTypeName, childTypeName): ...
def getSrcNode(dst):
    """
    Get the node connected to the argument dst plug.
    """
    ...
def findDisplacementShader(shadingEngine, search='False'):
    """
    Returns the displacement shader (as MObject) of the given shading engine (as MObject).
    """
    ...
def getRenderSetupNodeTypes():
    """
    Returns a tuple of concrete/usable Render Setup node types
    """
    ...
def _transferConnectedPlug(src, dst): ...
def getRenderSetupNodeTypesNames(): ...
def nameToUserNode(name): ...
def deleteNode(node):
    """
    Remove the argument node from the graph.
    
    The argument can be a node name string, or an object that has a name method.
    This function is undoable.
    """
    ...
def findSurfaceShader(shadingEngine, search='False'):
    """
    Returns the surface shader (as MObject) of the given shading engine (as MObject).
    """
    ...
def plugSrc(dstPlug):
    """
    Return the source of a connected destination plug.
    
    If the destination is unconnected, returns None.
    """
    ...
def getCollectionsRecursive(parent):
    """
    Return a generator of all collections in parent.
    Note: It is recursive, so all levels of children are searched through
    """
    ...
def nameToExistingUserNode(name): ...
def isExistingClassification(t): ...
def getSrcNodeName(dst):
    """
    Get the name of the node connected to the argument dst plug.
    """
    ...
def createGenericAttr(longName, shortName): ...
def findVolumeShader(shadingEngine, search='False'):
    """
    Returns the volume shader (as MObject) of the given shading engine (as MObject).
    """
    ...
def disconnectSrc(src):
    """
    Disconnect a source (readable) plug from all its destinations.
    
    Note that a single plug can be both source and destination, so this
    interface makes the disconnection intent explicit.
    """
    ...
def isSurfaceShaderNode(obj): ...
def transferPlug(src, dst):
    """
    Transfer the connection or value set on plug 'src' on to the plug 'dst'.
    """
    ...
def createDstMsgAttr(longName, shortName):
    """
    Create a destination (a.k.a. input, or writable) message attribute.
    """
    ...
def encodeMasterValues(getAllNodesFunction): ...
def getSrcUserNode(dst):
    """
    Get the user node connected to the argument dst plug.
        Note: Only applies to MPxNode derived nodes
    
    If the dst plug is unconnected, None is returned.
    """
    ...
def plugDst(srcPlug):
    """
    Return the destinations of a connected source plug.
    
    If the source is unconnected, returns None.
    """
    ...
def getLegacyRenderLayerNodeTypesNames(): ...
def _isDestination(plug):
    """
    Returns True if the given plug is a destination plug, and False otherwise.
    
    If the plug is a compond attribute it returns True if any of it's children is a 
    destination plug.
    """
    ...
def isShadingNode(obj): ...
def createSrcMsgAttr(longName, shortName):
    """
    Create a source (a.k.a. output, or readable) message attribute.
    """
    ...
def isShadingType(typeName): ...
def disconnectDst(dst):
    """
    Disconnect a destination (writable) plug from its source.
    
    Note that a single plug can be both source and destination, so this
    interface makes the disconnection intent explicit.
    """
    ...
def findPlug(userNode, attr):
    """
    Return plug corresponding to attr on argument userNode.
    
    If the argument userNode is None, or the attribute is not found, None
    is returned.
    """
    ...
def notUndoRedoing(f):
    """
    Decorator that will call the decorated method only if not currently in undoing or redoing.
    Particularly useful to prevent callbacks from generating commands since that would clear the redo stack.
    """
    ...
def _recursiveSearch(colList):
    """
    # Fonctions to compute the number of operations when layer are switched
    """
    ...
def isExistingType(t): ...
def getDstUserNodes(src):
    """
    Get the user nodes connected to the argument src plug.
        Note: Only applies to MPxNode derived nodes
    
    If the src plug is unconnected, None is returned.
    """
    ...
def sceneHasApplyOverridesOnAGivenNodeType(getAllNodesFunction):
    """
    Returns whether there are any apply overrides on the nodes obtained
    with the function passed as argument.
    """
    ...
def disconnect(src, dst): ...
def getTotalNumberOperations(model): ...
def canOverrideNode(node): ...
def connectMsgToDst(userNode, dst):
    """
    Connect the argument userNode's message attribute to the
    argument dst plug.
    
    If the userNode is None the dst plug is disconnected
    from its sources.
    
    If the dst plug is None the userNode's message plug
    is disconnected from its destinations
    """
    ...
def connect(src, dst):
    """
    Connect source plug to destination plug.
    
    If the dst plug is None, the src plug will be disconnected from all its
    destinations (if any).  If the src plug is None, the dst plug will be
    disconnected from its source (if any).  If both are None, this function
    does nothing.  If the destination is already connected, it will be
    disconnected.
    """
    ...
def getOverridesRecursive(parent):
    """
    Return a generator of all overrides in parent.
    Note: It is recursive, so all levels of children are searched through
    """
    ...
def _findShader(shadingEngine, attribute, classification='None'):
    """
    Returns the shader connected to given attribute on given shading engine.
    Optionally search for nodes from input connections to the shading engines 
    satisfying classification if plug to attribute is not a destination and
    a classification string is specified.
    """
    ...


kNoSuchNode : unicode
kPlugTypeMismatch : unicode
kSupportedVectorTypes : set
kWrongArgTypeCtxMgr : unicode
kSupportedSimpleTypes : set

