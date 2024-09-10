from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import renderSetup as renderSetupModel
from . import utils
import maya.app.renderSetup.common.utils as commonUtils
import maya.api.OpenMaya as OpenMaya
import itertools


if False:
    from typing import Dict, List, Tuple, Union, Optional

class _MPxCommand(object):
    """
    Base class for custom commands.
    """
    
    
    
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def doIt(*args, **kwargs):
        """
        Called by Maya to execute the command.
        """
        ...
    def hasSyntax(*args, **kwargs):
        """
        Called by Maya to determine if the command provides an MSyntax object describing its syntax.
        """
        ...
    def isUndoable(*args, **kwargs):
        """
        Called by Maya to determine if the command supports undo.
        """
        ...
    def redoIt(*args, **kwargs):
        """
        Called by Maya to redo a previously undone command.
        """
        ...
    def syntax(*args, **kwargs):
        """
        Returns the command's MSyntax object, if it has one.
        """
        ...
    def undoIt(*args, **kwargs):
        """
        Called by Maya to undo a previously executed command.
        """
        ...
    @staticmethod
    def appendToResult(*args, **kwargs):
        """
        Append a value to the result to be returned by the command.
        """
        ...
    @staticmethod
    def clearResult(*args, **kwargs):
        """
        Clears the command's result.
        """
        ...
    @staticmethod
    def currentResult(*args, **kwargs):
        """
        Returns the command's current result.
        """
        ...
    @staticmethod
    def currentResultType(*args, **kwargs):
        """
        Returns the type of the current result.
        """
        ...
    @staticmethod
    def displayError(*args, **kwargs):
        """
        Display an error message.
        """
        ...
    @staticmethod
    def displayInfo(*args, **kwargs):
        """
        Display an informational message.
        """
        ...
    @staticmethod
    def displayWarning(*args, **kwargs):
        """
        Display a warning message.
        """
        ...
    @staticmethod
    def isCurrentResultArray(*args, **kwargs):
        """
        Returns true if the command's current result is an array of values.
        """
        ...
    @staticmethod
    def setResult(*args, **kwargs):
        """
        Set the value of the result to be returned by the command.
        """
        ...
    __new__ : builtin_function_or_method
    
    commandString : getset_descriptor
    
    historyOn : getset_descriptor
    
    kDouble : int
    
    kLong : int
    
    kNoArg : int
    
    kString : int


class RenderLayerMembersCmd(_MPxCommand):
    """
    Command that filters a list of passed in DAG node nodes and returns the 
    filtered results based on the flags you set. This command is query only.
    The flags for this command are:
    
    -notIn: keep objects that do not belong to the provided render layers
    (default false, keep only objects that are render layer members in list. 
    When notIn is set to false, passing in a list of DAG nodes is optional).
    
    -renderLayers <renderLayers>: the render layers to check for membership 
    (-notIn false) or not check for membership (-notIn true).
    
    Example:
    // Isolate objects in the provided list ("pSphere1", "pCube1"), that are 
    // not in any of the provided render layers
    renderLayerMembers "pSphere1" "pCube1" -notIn true -renderLayers "layer1" "layer2"
    """
    
    
    
    def doIt(self, args): ...
    def isUndoable(self): ...
    @staticmethod
    def creator(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str
    
    kNotInFlag : str
    
    kNotInFlagLong : str
    
    kNotInFlags : set
    
    kRenderLayersFlag : str
    
    kRenderLayersFlagLong : str
    
    kRenderLayersFlags : set


class RenderSetupLegacyLayerCmd(_MPxCommand):
    """
    Command used to query the renderLayer associated to a specific renderSetupLayer
        Usage:
    "renderSetupLegacyLayer renderSetupLayerName".
    """
    
    
    
    def doIt(self, args): ...
    def isUndoable(self): ...
    @staticmethod
    def createSyntax(): ...
    @staticmethod
    def creator(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str


class RenderSetupFindCmd(_MPxCommand):
    """
    Command that finds collections, the members of which match any of the 
    provided DAG objects. This command takes flags:
    
    -inRenderLayers (mandatory flag) which only searches for collections under
    the specified render layer.
    
    -includeLayers which will also return the layer names if the objects are members
    of that layer (because included by a collection or implicit member (ex: light shapes)
    
    Examples:
    // Finds from the "layer1" and "layer2" render layers, the collections 
    // that "pSphere1" and "pCube1" belong to
    renderSetupFind "pSphere1" "pCube1" -inRenderLayers "layer1" "layer2"
    """
    
    
    
    def doIt(self, args): ...
    def isUndoable(self): ...
    @staticmethod
    def creator(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str
    
    kInRenderLayersFlag : str
    
    kInRenderLayersFlagLong : str
    
    kInRenderLayersFlags : set
    
    kIncludeLayersFlag : str
    
    kIncludeLayersFlagLong : str
    
    kIncludeLayersFlags : set


class RenderSetupCmd(_MPxCommand):
    """
    Command that will be used for querying and editing the render setup 
    state. At present a user can only query the list of render layers with
    "renderSetup -query -renderLayers".
    """
    
    
    
    def doIt(self, args): ...
    def isUndoable(self): ...
    @staticmethod
    def createSyntax(): ...
    @staticmethod
    def creator(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str
    
    kRenderLayersFlag : str
    
    kRenderLayersFlagLong : str




def getCollections(renderLayers):
    """
    # Returns the collection models under the specified render layers
    """
    ...
def notInRenderLayers(*args, **kwargs): ...
def getMembersAsLongNames(renderLayers): ...
def inRenderLayers(*args, **kwargs): ...
def isCollectionMember(objectNodeName, collections): ...
def renderSetupFind(objectNodeNames, renderLayerNames, includeLayers): ...
def getLongName(name): ...
def longNamesToNamesDict(names): ...
def renderLayerMembers(objectNodeNames, renderLayerNames, notInRenderLayers='False'): ...


kNotInNeedAListToFilter : unicode
kInvalidNodeName : unicode

