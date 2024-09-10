from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
from . import rendererCallbacks
from . import jsonTranslatorGlobals


"""
Please have a look to http://json.org/ for the detailed Json syntax
and the official documentation is http://www.ecma-international.org/publications/files/ECMA-ST/ECMA-404.pdf
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class MergePolicy(object):
    """
    The class is the policy to manage a new object instance when decoding a list 
    of render setup object depending of the merge type.
    """
    
    
    
    def __init__(self, getFn, createFn, mergeType, prependToName): ...
    def create(self, dict, typeName): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def decodeObjectArray(list, policy):
    """
    Decode an array of Render Setups Objects
    
    list is a list of dictionaries (json representation for a array of objects)
    policy encapsulates the behavior to create the render setup object instance based on the merge type
    """
    ...
def getObjectNotes(encodedData):
    """
    Get the Notes from any Render Setup Object knowing that all nodes
    could or not contain the 'notes' dynamic attribute.
    """
    ...
def isRenderSetupTemplate(encodedData): ...
def encodeObjectArray(objects):
    """
    Encode an array of Render Setups Object as a list for the Json default encoder
    """
    ...
def _isRenderSetup(encodedData): ...
def isRenderSetup(encodedData):
    """
    Is the encodedData defining a Render Setup ?
    Note: The test is not foolproof but should segregate obvious unrelated data
    """
    ...
def getTypeNameFromDictionary(encodedData):
    """
    Get the root typename stored in the dictionary
    Note: Any node encoding always first encapsulates its data in a dictionary
          where the key is the node type.
    """
    ...

