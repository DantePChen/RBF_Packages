from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import pkgutil
import platform
import inspect
import sys
import re
import os


from pymel.util.path import path
from re import escape


if False:
    from typing import Dict, List, Tuple, Union, Optional

def interpreterBits():
    """
    Returns the number of bits of the architecture the interpreter was compiled on
    (ie, 32 or 64).
    
    Returns
    -------
    int
    """
    ...
def uncapitalize(s, preserveAcronymns='False'):
    """
    preserveAcronymns enabled ensures that 'NTSC' does not become 'nTSC'
    
    Returns
    -------
    str
    """
    ...
def inMaya():
    """
    Returns True if called from a fully initialized Maya session.
    """
    ...
def toZip(directory, zipFile):
    """
    Sample for storing directory to a ZipFile
    """
    ...
def timer(command="'pass'", number='10', setup="'import pymel'"): ...
def subpackages(packagemod):
    """
    Given a module object, returns an iterator which yields a tuple
    (modulename, moduleobject, ispkg)
    for the given module and all it's submodules/subpackages.
    """
    ...
def capitalize(s):
    """
    Python's string 'capitalize' method is NOT equiv. to mel's capitalize, which preserves
    capital letters.
    
        >>> capitalize( 'fooBAR' )
        'FooBAR'
        >>> 'fooBAR'.capitalize()
        'Foobar'
    
    Returns
    -------
    str
    """
    ...
def cacheProperty(getter, attr_name, fdel='None', doc='None'):
    """
    a property type for getattr functions that only need to be called once per instance.
    future calls to getattr for this property will return the previous non-null value.
    attr_name is the name of an attribute in which to store the cached values
    """
    ...
def unescape(s):
    """
    Returns
    -------
    str
    """
    ...

