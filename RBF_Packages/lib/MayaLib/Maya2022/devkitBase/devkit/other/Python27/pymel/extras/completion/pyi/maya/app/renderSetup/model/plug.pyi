from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import typeIDs
import maya
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import itertools


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Plug(object):
    """
    Helper class to allow seamless value assignment from one plug to another, 
    while correctly handling and abstracting away plug type.
    
    "self.type" returns the type of the plug.
        This is necessary to determine how to read and write the plug.
    "self.value" returns the value of the plug.
    "self.value = otherValue" will set the value of the plug to otherValue.
        This mutator assumes otherValue to be the same type as self.type
    
    "self.overrideType" returns the type of the override that should be created to override this plug.
    """
    
    
    
    def __init__(self, plugOrNode, attrName='None'):
        """
        Constructors:
        Plug(MPlug)
        Plug(string (full plug name))
        Plug(MObject, MObject)
        Plug(MObject, string (attribute name))
        Plug(string (node name), string (attribute name))
        """
        ...
    def __str__(self) -> str: ...
    def accepts(self, other):
        """
        Returns true if plug would accept a connection with other plug
        i.e. plug and other plug are type compatible for connection.
        """
        ...
    def acceptsOverrideType(self, typeId): ...
    def applyOverrideType(self, overType): ...
    def attribute(self):
        """
        Returns the attribute (MFnAttribute) of the plug
        """
        ...
    def availableOverrides(self): ...
    def children(self): ...
    def cloneAttribute(self, nodeObj, longName, shortName, undoable='1'):
        """
        Creates a new attribute on a node by cloning this plug's attribute.
        Undoable by default
        """
        ...
    def copyValue(self, other):
        """
        Sets the value of plug 'self' to the value contained in plug 'other' 
        The 'other' plug can be either a Plug or a MPlug.
        """
        ...
    def createAttributeFrom(self, nodeObj, longName, shortName, limits='None'):
        """
        Creates a new attribute on a node by cloning this plug's attribute. 
        
        Note: None for a limit value means that there is no limit. For example,
              if min is None, it means that there is no minimum limit.
        """
        ...
    def getAttributeLimits(self):
        """
        Get the limits of the plug
        """
        ...
    def isOvrSupported(self): ...
    def localizedTypeString(self): ...
    def node(self): ...
    def overrideType(self, overType): ...
    def parent(self): ...
    def setAttributeLimits(self, limits): ...
    @staticmethod
    def createAttribute(nodeObj, longName, shortName, dict, undoable='1'):
        """
        Create a new attribute on a node using the given names and properties dictionary. 
        Returns an MObject to the new attribute. By default, it uses the command 
        addDynamicAttribute (if it's not undoable, use MFnDependencyNode.addAttribute()) 
        to add the returned object as a new dynamic attribute on a node.
        """
        ...
    @staticmethod
    def getNames(plugName): ...
    @property
    def attributeName(self): ...
    @property
    def hasLimits(self):
        """
        Returns true if the type supports min/max limits.
        """
        ...
    @property
    def isConnectable(self):
        """
        Returns true if plug's input can be connected.
        """
        ...
    @property
    def isKeyable(self):
        """
        Returns true if the plug has the keyable property.
        
        Note that a plug that does not have this property can still have
        key frames set on it.  As per Maya documentation, a keyable
        attribute means that 'if any of the animation commands (setKeyframe,
        cutKey, copyKey,..) are issued without explicitly specifying any
        attributes with the -at/attribute flag, then the command will
        operate on all keyable attributes of the specified objects'.
        
        The autoKeyframe functionality also uses the keyable property to
        determine if key frames should be set.
        """
        ...
    @property
    def isLocked(self):
        """
        Returns true is plug or plug's children (compound) are locked.
        """
        ...
    @property
    def isNumeric(self): ...
    @property
    def isUnit(self): ...
    @property
    def isValid(self): ...
    @property
    def isVector(self):
        """
        Returns true if the type is a vector type.
        """
        ...
    @property
    def name(self): ...
    @property
    def nodeName(self): ...
    @property
    def plug(self): ...
    @property
    def type(self): ...
    @property
    def uiUnitValue(self): ...
    @uiUnitValue.setter
    def uiUnitValue(self, value): ...
    @property
    def value(self): ...
    @value.setter
    def value(self, value): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kAngle : int
    
    kArray : int
    
    kBool : int
    
    kByte : int
    
    kColor : int
    
    kDistance : int
    
    kDouble : int
    
    kEnum : int
    
    kFilename : int
    
    kFloat : int
    
    kInt : int
    
    kInvalid : int
    
    kLast : int
    
    kMessage : int
    
    kObject : int
    
    kString : int
    
    kTime : int


class UnlockedGuard:
    """
    Safe way to unlock a plug in a block. 
    Lock state will be recovered back on exit of the block (for ancestors and children plugs).
    Example:
        with UnlockedGuard(aLockedPlug):
            someActionsOnThePlug()
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, plg): ...


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


class AddDynamicAttribute(_MPxCommand):
    """
    Undoable command to add an attribute to a node
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, node, attribute, mdgModifier): ...
    def doIt(self, args): ...
    def isUndoable(self): ...
    def redoIt(self): ...
    def undoIt(self): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(node, attribute): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    attribute : NoneType
    
    kCmdName : str
    
    mdgModifier : NoneType
    
    node : NoneType




def findPlug(node, attr='None'):
    """
    Return a Plug instance if the MPlug was found, None otherwise.
    """
    ...
def relatives(plg):
    """
    Returns relatives (ancestors, plug itself and descendant) of the given plug in deterministic order.
    Parents are guaranteed to come before children in generator.
    """
    ...
def toUiUnits(type, value): ...
def _createFilenameAttr(longName, shortName): ...
def value(mPlug):
    """
    Convenience function to retrieve the value of an MPlug.
    """
    ...
def isSettable(plug):
    """
    Predicate that returns whether the MPlug argument can be set.
    """
    ...
def toInternalUnits(type, value): ...


kAddAttributePrivate : unicode
kArityMismatch : unicode
kNotUndoable : int
kPlugWithoutLimits : unicode
kPlugHasNotSettableChild : unicode
kUnknownType : unicode
kUndoable : int
kCompoundTypeStr : unicode
kPlugHasConnectedParent : unicode
kVectorTypeStr : unicode
kUnsupportedAttribute : unicode
kNotOverridablePlug : unicode

