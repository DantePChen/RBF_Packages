from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import plug
from . import typeIDs
from . import localOverride
import maya
import maya.app.renderSetup.common.guard as guard
from . import enabled
from . import utils
from . import namespace
import maya.app.renderSetup.common.utils as commonUtils
from . import undo
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import itertools
from . import context
from . import applyOverride
import weakref
from . import sceneObservable
from . import childNode


"""
This module provides the override base and concrete classes, as well as
utility functions to operate on overrides.
"""


from maya.app.renderSetup.model.renderSetupPrivate import PostApplyCmd
from collections import namedtuple


if False:
    from typing import Dict, List, Tuple, Union, Optional

class OverridePlugHandle(object):
    """
    Plug class that handles dynamic plug and missing dependencies.
    
    Has functions for finalization, encoding, decoding and handling missing dependencies.
    
    Finalization creates a dynamic attribute based on another plug. It has 3 available modes:
     - kModeClone clones the input plug
     - kModeMultiply will have the same arity as the input plug but has float scalar units
       (can be used to multiply input's plug type by a scalar)
     - kModeOffset clones the input plug but with more flexible min/max, softMin/softMax
       (can be used to offset input's plug type by some value in the same units)
       
    Encode/decode creates/read dictionary of attributes that defines the plug attributes.
    
    Handles missing dependency:
     If the plug is decoded and can't find the source plug it's supposed to connect to, then
     it has a missing dependency. It creates a dynamic string attribute containing the name of the
     missing dependency (this allows the "missing dependency" state to persist on file new) and it 
     starts listening to scene changes to find the missing dependency and connect to it if it's created.
    """
    
    
    
    def __init__(self, ovr, longName, shortName, mode='0'): ...
    def decode(self, dict): ...
    def encode(self, dict): ...
    def finalize(self, plg): ...
    def getMissingDependency(self): ...
    def getPlug(self):
        """
        Return the Plug object (plug.py). (this is not a MPlug)
        """
        ...
    def getSource(self): ...
    def hasMissingDependency(self): ...
    def isFinalized(self): ...
    def isValid(self): ...
    def name(self): ...
    def node(self): ...
    def setMissingDependency(self, source): ...
    def setSource(self, source): ...
    def update(self): ...
    @property
    def attr(self): ...
    @property
    def attrDependency(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kModeClone : int
    
    kModeMultiply : int
    
    kModeOffset : int


_Property = namedtuple('Property', ('name', 'encode', 'decode'))


class Override(childNode.TreeOrderedItem, childNode.ChildNode):
    """
    Override node base class.
    
    An override represents a non-destructive change to a scene property
    that can be reverted or disabled.  Render setup uses overrides to describe
    changes that apply in a single render layer, and are unapplied when
    switching to another render layer.  When working within a render layer, an
    override can be preserved but disabled, to remove its effect.
    
    The override node base class cannot be directly created in Maya.  It is
    derived from the ListItem base class, so that overrides can be inserted
    in a list.
    """
    
    
    
    def attrValuePlugName(self): ...
    def attributeName(self): ...
    def compute(self, plug, dataBlock): ...
    def enabledChanged(self): ...
    def getApplyOverrides(self):
        """
        Return the list of apply override nodes that correspond to this override.
        """
        ...
    def getRenderLayer(self): ...
    def hasApplyOverrides(self): ...
    def isAbstractClass(self): ...
    def isAcceptableChild(self, model):
        """
        Check if the model could be a child
        """
        ...
    def isApplied(self): ...
    def isEnabled(self): ...
    def isLocalRender(self, dataBlock='None'): ...
    def isSelfEnabled(self, dataBlock='None'): ...
    def isValid(self): ...
    def itemChangedRecursive(self): ...
    def setAttributeName(self, attributeName):
        """
        Set the name of the attribute to be overridden.
        """
        ...
    def setLocalRender(self, value): ...
    def setSelfEnabled(self, value): ...
    def typeId(self): ...
    def typeName(self): ...
    @classmethod
    def creator(cls):
        """
        # Awkwardly, abstract base classes seem to need a creator method.
        """
        ...
    @staticmethod
    def initializer(): ...
    attrLocal : OpenMaya.MObject
    
    attrName : OpenMaya.MObject
    
    enabled : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str
    
    layerNumIsolatedChildren : OpenMaya.MObject
    
    parentEnabled : OpenMaya.MObject
    
    selfEnabled : OpenMaya.MObject


class UniqueOverride(object):
    """
    Mixin class for override that applies to a unique node. This override 
    unconditionnaly applies to the node it was finalized on (if it exists).
    It doesn't care about the collection's content.
    """
    
    
    
    def finalize(*args, **kwargs): ...
    def setTargetNodeName(*args, **kwargs): ...
    def targetNodeName(self, dataBlock='None'): ...
    @classmethod
    def addTargetAttribute(cls): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kTargetNodeName : OpenMaya.MObject


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


class LeafClass(object):
    """
    To be used by leaf classes only
    """
    
    
    
    def isAbstractClass(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ValueOverride(Override):
    """
    Override node base class for all value overrides.
    """
    
    
    
    def __init__(self): ...
    def acceptsAttr(*args, **kwargs): ...
    def apply(*args, **kwargs): ...
    def applyInsertOne(self, node, attr, nextOvr='None'): ...
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
    def overridesConnections(self): ...
    def postApply(*args, **kwargs): ...
    def reapply(*args, **kwargs): ...
    def unapply(*args, **kwargs): ...
    def update(*args, **kwargs): ...
    @staticmethod
    def initializer(): ...
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class Property(_Property):
    """
    Namedtuple to hold what is needed to encode a property (name, encode function, decode function).
    'name' will be the name of the given attribute (MObject).
    """
    
    
    
    @staticmethod
    def __new__(cls, attr, encode, decode): ...
    __dict__ : dictproxy


class UnapplyCmd(_MPxCommand):
    """
    Command to unapply and reapply an override.
    
    This command is a private implementation detail of this module and should
    not be called otherwise.
    """
    
    
    
    def __init__(self, override): ...
    def doIt(self, args): ...
    def isUndoable(self): ...
    def redoIt(*args, **kwargs): ...
    def undoIt(*args, **kwargs): ...
    @staticmethod
    def creator(): ...
    @staticmethod
    def execute(override):
        """
        Unapply the override, and add an entry to the undo queue.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kCmdName : str
    
    override : NoneType


class AbsOverride(LeafClass, ValueOverride):
    """
    Absolute override node.
    
    This concrete override node type implements an absolute override
    (replace value) for an attribute.
    """
    
    
    
    def __init__(self): ...
    def compute(self, plg, dataBlock): ...
    def finalize(*args, **kwargs): ...
    def getAttrValue(self):
        """
        Returns the value of the override.
        
        This value is always in internal units.  See OpenMaya documentation for
        MAngle, MDistance, and MTime.  plug.toUiUnits and plug.toInternalUnits
        can be used to convert between units.
        """
        ...
    def hasMissingDependencies(self): ...
    def isFinalized(self): ...
    def isValid(self): ...
    def postConstructor(self): ...
    def setAttrValue(self, value):
        """
        Sets the value of the override.
        
        This value must be in internal units.  See OpenMaya documentation for
        MAngle, MDistance, and MTime.  plug.toUiUnits and plug.toInternalUnits
        can be used to convert between units.
        """
        ...
    def status(self):
        """
        Returns a problem string if there is a problem with override or None otherwise.
        """
        ...
    @staticmethod
    def initializer(): ...
    kAttrValueLong : str
    
    kAttrValueShort : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class RelOverride(LeafClass, ValueOverride):
    """
    Relative override node to transform a attribute using:
    
    newValue = originalValue * multiply + offset
    
    This concrete override node type implements a relative override
    for a float scalar attribute.
    """
    
    
    
    def __init__(self): ...
    def compute(self, plug, dataBlock): ...
    def finalize(*args, **kwargs): ...
    def getMultiply(self): ...
    def getOffset(self): ...
    def isFinalized(self): ...
    def isValid(self): ...
    def multiplyPlugName(self): ...
    def offsetPlugName(self): ...
    def postConstructor(self): ...
    def setMultiply(self, value): ...
    def setOffset(self, value): ...
    def status(self):
        """
        Returns a problem string if there is a problem with override or None otherwise.
        """
        ...
    @staticmethod
    def initializer(): ...
    kMultiplyLong : str
    
    kMultiplyShort : str
    
    kOffsetLong : str
    
    kOffsetShort : str
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class RelUniqueOverride(UniqueOverride, RelOverride):
    @staticmethod
    def initializer(): ...
    kTargetNodeName : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str


class AbsUniqueOverride(UniqueOverride, AbsOverride):
    @staticmethod
    def initializer(): ...
    kTargetNodeName : OpenMaya.MObject
    
    kTypeId : OpenMaya.MTypeId
    
    kTypeName : str




def create(*args, **kwargs): ...
def fillVector(value, dimension):
    """
    Return a list of specified dimension initialized with value.
    
    If dimension is 0, return the argument value as a scalar.
    """
    ...
def attributeToPlug(f):
    """
    Decorator that adds a node name to an attribute name, if not present. (attribute => plug)
    Using template node name of parent collection's selector.
    """
    ...
def delete(*args, **kwargs): ...
def valid(f):
    """
    Decorator that calls the decorated method if and only if self.isValid() evaluates to True.
    """
    ...
def finalizationChanged(f):
    """
    Decorator for functions that may change the finalization of an override (decode, finalize).
    This will ensure that if the layer in which this override lives is visible, then the override
    should be unapplied and reapplied with the new finalization.
    """
    ...


kMissingDependencies : unicode
kUnfinalized : unicode
kUnconnectableAttr : unicode
kUnapplyCmdPrivate : unicode

