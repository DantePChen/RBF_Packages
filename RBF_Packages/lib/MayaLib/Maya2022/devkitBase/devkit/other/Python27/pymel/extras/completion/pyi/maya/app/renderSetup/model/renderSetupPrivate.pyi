from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import renderLayerSwitchInfo
from . import namespace
from . import undo
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.common.guard as guard
import time
import logging


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


class PostApplyCmd(_MPxCommand):
    """
    Command to apply collection or override when the layer is already visible.
    This should apply the overrides in the right order, i.e. apply override nodes
    must be inserted at the right position in the apply chain.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, applicable): ...
    def doIt(self, args): ...
    def isUndoable(self): ...
    def redoIt(*args, **kwargs): ...
    def undoIt(*args, **kwargs): ...
    @classmethod
    def creator(cls): ...
    @classmethod
    def execute(cls, applicable):
        """
        Applies an applicable (collection/override) after the layer was already set visible.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    applicable : NoneType
    
    kCmdName : str


class SwitchVisibleRenderLayerCmd(_MPxCommand):
    """
    Command to switch the visible layer.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, newLayer): ...
    def doIt(self, args): ...
    def isUndoable(self): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(newLayer):
        """
        Switch to given RenderLayer
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str
    
    newLayer : NoneType




def moveModel(modelToMove, destinationModel, destinationPosition):
    """
    Helper method to move a model from a location to antoher location
    """
    ...
def _renderSetupInstance(): ...


kSwitchVisibleRenderLayer : unicode
kCmdPrivate : unicode
logger : logging.Logger

