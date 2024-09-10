from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.app.renderSetup.model.utils as utils
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import itertools
import maya.app.renderSetup.model.modelCmds as modelCmds


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


class RenderSetupHighlightCmd(_MPxCommand):
    """
    Command that can be used to highlights given object names/paths
    in the collections.
    
    This helps visualize how objects ended up being part of the 
    selected collections. For ex: by pattern or by static selection.
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


class RenderSetupSelectCmd(_MPxCommand):
    """
    Command that can be used to select render setup elements as well as to
    query the elements in the render setup selection.
    
    Five optional flags can be used with this command:
    
    -additive (-a) adds elements to the selection (without clearing it)
    
    -deselect (-d) removes elements from the current selection
    
    -renderLayers is a query only flag that specifies that renderLayers 
    should be returned as part of the query
    
    -collections is a query only flag that specifies that collections should 
    be returned as part of the query
    
    -overrides is a query only flag that specifies that overrides should be 
    returned as part of the query
    
    By default the selection is cleared before selecting elements. Also the
    additive and deselect flags cannot be used in conjunction.
    
    Sample Usage:
    // Select "renderSetupLayer1" and "renderSetupLayer2" 
    renderSetupSelect "renderSetupLayer1" "renderSetupLayer2"
    
    // Add "renderSetupLayer1" and "renderSetupLayer2" to the selection
    renderSetupSelect -additive "renderSetupLayer1" "renderSetupLayer2"
    
    // Deselect "renderSetupLayer1" and "renderSetupCollection2" from the selection
    renderSetupSelect -deselect "renderSetupLayer1" "renderSetupCollection2"
    
    // Query the selected render setup items
    renderSetupSelect -query
    
    // Query the selected render setup items that are renderLayers and 
    // overrides
    renderSetupSelect -query -renderLayers -overrides
    """
    
    
    
    def doIt(self, args): ...
    def isUndoable(self): ...
    @staticmethod
    def createSyntax(): ...
    @staticmethod
    def creator(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kAddFlag : str
    
    kAddFlagLong : str
    
    kCmdName : str
    
    kCollectionsFlag : str
    
    kCollectionsFlagLong : str
    
    kDeselectFlag : str
    
    kDeselectFlagLong : str
    
    kOverridesFlag : str
    
    kOverridesFlagLong : str
    
    kRenderLayersFlag : str
    
    kRenderLayersFlagLong : str
    
    kRenderSettingsChildCollectionsFlag : str
    
    kRenderSettingsChildCollectionsFlagLong : str




def notInSelectedRenderLayers(*args, **kwargs):
    """
    Tests for nodes not in any currently selected layer.
    args: an array of nodes to test
    kwargs:
        attributes: an array of attributes to test
    """
    ...
def highlight(names): ...
def getPropertyEditor(): ...
def getSelection(renderLayers='False', collections='False', renderSettingsChildCollections='False', overrides='False'): ...
def getSelectedCollections(): ...
def _itemMatches(item, renderLayers, overrides, collections, renderSettingsChildCollections): ...
def getSelectionModel(): ...
def inSelectedRenderLayers(*args, **kwargs):
    """
    Tests for nodes in any currently selected layer.
    args: an array of nodes to test
    kwargs:
        attributes: an array of attributes to test
    """
    ...
def getRenderSetupView(): ...
def getRenderSetupWindow(): ...


kParsingError : unicode
kAddAndDeselectNoTogether : unicode
kAddAndDeselectEditOnly : unicode
kSelectionEditFailed : unicode
kNotEditableFlags : unicode

