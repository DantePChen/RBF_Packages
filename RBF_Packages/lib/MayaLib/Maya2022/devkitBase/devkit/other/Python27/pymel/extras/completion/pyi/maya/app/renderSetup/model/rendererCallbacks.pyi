from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import plug
import maya
import maya.mel as mel
import maya.app.renderSetup.common.utils as commonUtils
from . import jsonTranslatorGlobals
import maya.api.OpenMaya as OpenMaya


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderSettingsCallbacks(object):
    """
    Renderers should either extend this class or create a class with the same signature to provide additional render settings callbacks
    """
    
    
    
    def createDefaultNodes(self):
        """
        Create the default nodes for the specific renderer
        """
        ...
    def decode(self, rendererData):
        """
        Decodes any renderer specific render settings data
        """
        ...
    def encode(self):
        """
        Encodes any renderer specific render settings data
        """
        ...
    def getNodes(self):
        """
        Returns the default render settings nodes for the specific renderer
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class NodeExporter(object):
    """
    Helper exporter to encode/decode any nodes
    """
    
    
    
    def __init__(self): ...
    def decode(self, encodedData): ...
    def encode(self): ...
    def setPlugsToIgnore(self, plugsToIgnore): ...
    def warnPlugsToIgnore(self, value): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class AOVCallbacks(object):
    """
    Renderers should either extend this class or create a class with the same signature to provide additional AOV callbacks
    """
    
    
    
    def decode(self, aovsData, decodeType):
        """
        Decodes the AOV information from some format
        
        aovsData   - The AOV data to decode
        decodeType - Overwrite, Merge
        """
        ...
    def displayMenu(self):
        """
        Displays the AOV information for the current renderer
        """
        ...
    def encode(self):
        """
        Encodes the AOV information into some format
        """
        ...
    def getAOVName(self, aovNode):
        """
        From a given AOV node, returns the AOV name
        """
        ...
    def getChildCollectionSelector(self, selectorName, aovName):
        """
        Creates the selector for the AOV child collection
        """
        ...
    def getChildCollectionSelectorAOVNodeFromDict(self, d):
        """
        Returns the child selector AOV node name from the provided dictionary
        """
        ...
    def getCollectionSelector(self, selectorName):
        """
        Creates the selector for the AOV collection
        """
        ...
    @staticmethod
    def aovNodeTypes():
        """
        Return the AOV node types supported by this renderer.
        """
        ...
    DECODE_TYPE_MERGE : int
    
    DECODE_TYPE_OVERWRITE : int
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class BasicNodeExporter(NodeExporter):
    """
    Exporter that is used to export the nodes that have been set
    """
    
    
    
    def getNodes(self): ...
    def setNodes(self, nodes): ...




def getCallbacks(*args): ...
def unregisterCallbacks(renderer, callbacksType='None'): ...
def registerCallbacks(renderer, callbacksType, callbacks): ...


rendererCallbacks : dict
kAttributesNotExportable : unicode
kDefaultNodeMissing : unicode
CALLBACKS_TYPE_AOVS : int
kDefaultNodeAttrMissing : unicode
CALLBACKS_TYPE_RENDER_SETTINGS : int

