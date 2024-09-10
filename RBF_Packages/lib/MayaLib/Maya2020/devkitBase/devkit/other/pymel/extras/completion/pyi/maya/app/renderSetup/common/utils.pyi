from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import maya.api.OpenMaya as OpenMaya
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ReportableException(exceptions.Exception):
    def __init__(self, text): ...
    def __str__(self) -> str: ...
    @property
    def text(self): ...
    __weakref__ : getset_descriptor


class SettingContextManager:
    """
    For use with the Render Setup Options menu tests.
    Store a cached version of the option var and environment variable for
    the corresponding Setting. This is used in the exit method to restore
    the corresponding values to both those variables, since we might have
    modified them during the called functions for testing purposes.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_val, exc_tb): ...
    def __init__(self, classOfSetting):
        """
        #This is callable on any render setup option menu settings.
        """
        ...


class SingletonMetaClass(type):
    """
    Setting this class as a class' metaclass allows it to become
    a Singleton, meaning that at most one instance of that class may
    exist at any given time. Acts as a class-factory.
    """
    
    
    
    def __call__(cls, *args, **kwargs): ...
    classInstances : dict




def removeDuplicates(seq):
    """
    Removes all duplicated elements from a list.
    Note that order is not preserved.
    """
    ...
def nodeToShortName(node):
    """
    Returns the minimum name string necessary to uniquely identify the node.
    """
    ...
def getSubClasses(classType): ...
def isNodeInstance(node, nodeType): ...
def _selectPlug(name): ...
def nodeToLongName(node):
    """
    Returns the full name of the node, 
    i.e. the absolute path for dag nodes and name for dependency (non-dag) nodes.
    """
    ...
def settingDecorator(classOfSetting):
    """
    Use the setting's context manager.
    This ensures that the correct values are restored to the environment
    and option variables after calling function f.
    """
    ...
def findPlug(node, attr):
    """
    Return the MPlug corresponding to attr on argument node or None if not found.
    The node argument can be an MObject or a node string name.
    The attr argument can be an MObject or a attr string name.
    Raises RuntimeError if plug is ambiguous.
    """
    ...
def findRecursivelyInEncodedProperties(d, searched): ...
def nameToDagPath(name):
    """
    Returns the MDagPath matching given name or None if not found.
    Raises RuntimeError if name is ambiguous.
    """
    ...
def echoSelect(*args, **kwargs):
    """
    Calls 'select' command with given args, then prints the obtained selection.
    """
    ...
def nameToPlug(name):
    """
    Returns the MPlug matching given name or None if not found.
    Raises RuntimeError if name is ambiguous.
    """
    ...
def nameToNode(name):
    """
    Returns the MObject matching given name or None if not found.
    Raises RuntimeError if name is ambiguous.
    """
    ...


kAmbiguousName : unicode
kNameToNodeTypeMismatch : unicode

