from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
from . import utils
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class LocalOverrideGuard(object):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, newEnabled): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ExportListener(object):
    """
    This instance listen for export callbacks (before/after).
    Ensures that all calls to ov.isEnabled() for every applied local override
    will return false during export time.
    """
    
    
    
    def __init__(self): ...
    def isExporting(self): ...
    @staticmethod
    def deleteInstance(): ...
    @staticmethod
    def instance(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


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


class RenderSetupLocalOverrideCmd(_MPxCommand):
    def doIt(self, args): ...
    def isUndoable(self): ...
    @staticmethod
    def createSyntax(): ...
    @staticmethod
    def creator(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str
    
    kStateFlag : str
    
    kStateFlagLong : str




def _setLocalOverrideEnabled(newEnabled): ...
def _toggle(ovrs): ...
def enabled(): ...
def localOverrideEnabled(newEnabled): ...


_localOverrideEnabled : bool

