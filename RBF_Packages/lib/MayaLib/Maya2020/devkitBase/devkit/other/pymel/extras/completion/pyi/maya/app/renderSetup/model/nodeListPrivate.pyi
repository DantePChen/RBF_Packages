from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
import maya
import maya.cmds as cmds


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


class ListCmdBase(_MPxCommand):
    """
    Base class for list commands that write to node lists.
    
    This command is intended as a base class for concrete list commands.
    """
    
    
    
    def doIt(self, args): ...
    def isUndoable(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    nodeList : NoneType


class PopCmd(ListCmdBase):
    """
    Remove and return the last item from a list.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, nodeList): ...
    def doIt(self, args): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(nodeList):
        """
        Remove and return the last list item from the node list, and add an
        entry to the undo queue.
        """
        ...
    kCmdName : str
    
    listItem : NoneType


class PrependCmd(ListCmdBase):
    """
    Add an item to the head of the list.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, nodeList, listItem): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(nodeList, listItem):
        """
        Prepend the item to the list, and add an entry to the undo queue.
        """
        ...
    kCmdName : str
    
    listItem : NoneType


class InsertBeforeCmd(ListCmdBase):
    """
    Insert a list item before another.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, nodeList, nextItem, listItem): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(nodeList, nextItem, listItem):
        """
        Insert the list item into the node list before nextItem, and add an
        entry to the undo queue.
        """
        ...
    kCmdName : str
    
    listItem : NoneType
    
    nextItem : NoneType


class AppendCmd(ListCmdBase):
    """
    Append an item to a list.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, nodeList, listItem): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(nodeList, listItem):
        """
        Append the item to the list, and add an entry to the undo queue.
        """
        ...
    kCmdName : str
    
    listItem : NoneType


class InsertCmd(ListCmdBase):
    """
    Insert a list item before a given position in a list.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, nodeList, ndx, listItem): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(nodeList, ndx, listItem):
        """
        Insert the list item into the node list before position ndx, and
        add an entry to the undo queue.
        """
        ...
    kCmdName : str
    
    listItem : NoneType
    
    ndx : NoneType


class RemoveCmd(ListCmdBase):
    """
    Remove an item from a list.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, nodeList, listItem): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(nodeList, listItem):
        """
        Remove the list item from the node list, and add an entry to the
        undo queue.
        """
        ...
    kCmdName : str
    
    listItem : NoneType




def remove(list, x):
    """
    Remove node x from the list.
    
    This function will detach the node x from the list if x is an element
    of the list, otherwise nothing will be done.
    
    It has O(1) time complexity.
    """
    ...
def forwardListGenerator(list):
    """
    # Not a write operation, but needed by the implementation of insert().
    """
    ...
def pop(list):
    """
    Pop the last node from the list.
    
    The method disconnects the last node from list and returns it.  It has
    O(1) time complexity.
    """
    ...
def prepend(list, x):
    """
    Add x to the head of the list.
    
    This function is a convenience for insert(list, 0, x).  It has O(1) time
    complexity.
    """
    ...
def insertBefore(list, nextItem, x):
    """
    Insert node x before item nextItem in list.
    
    If nextItem is None, element x will be appended to the list.  This function
    has O(1) time complexity.
    """
    ...
def insert(list, ndx, x):
    """
    Insert node x before position ndx in list.
    
    Positions run from 0 to n-1, for a list of length n.  Inserting at
    position 0 calls prepend(), and thus has O(1) time complexity.
    Inserting mid-list has O(n) time complexity.  Inserting at position n
    or beyond appends to the list, with O(n) time complexity.  To append to
    the list use append() directly, as it has O(1) time complexity.
    """
    ...
def append(list, x):
    """
    Append node x to the list.
    
    This function has O(1) time complexity.
    """
    ...


kListCmdPrivate : unicode

