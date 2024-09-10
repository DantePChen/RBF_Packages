from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import renderLayerSwitchInfo
import maya.OpenMaya as OpenMaya1_0
import maya.mel as mel
from . import utils
from . import renderLayer
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.common.guard as guard
from . import renderSetup
from . import legacyRenderLayer
from . import applyOverride
import os


"""
This module monitors file loads (including imports and reference load) and 
    displays warning messages if it discovers that new loaded files are not 
    compatible with the current renderSetup mode
    
    Implementation details:
    In order to determine if the loaded file is compatible with the current Maya state
    we first determine the type the loaded file based on its content. Then depending 
    on the current Maya RenderSetup mode error messages are displayed if incompatibilities
    are identified
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class _FileLoadMonitor(object):
    """
    Class that counts the types of nodes that are to be watched and/or cleaned
    up as they are added in the scene.
    """
    
    
    
    def __init__(self): ...
    def reset(self): ...
    def start(self): ...
    def stop(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    watchedNodeTypes : list




def finalize(): ...
def onReadStart(_): ...
def _getErrorSwitchToRenderLayerIfNeeded(): ...
def onReadEnd(_): ...
def initialize(): ...
def onImportOrReferenceEnd(data): ...


_nodeCounter : _FileLoadMonitor
kErrorCombiningNewToLegacy : unicode
kWarningVisibleRenderLayer : unicode
callbacks : list
kErrorSwitchToRenderLayer : unicode
kErrorSwitchToRenderSetup : unicode
kErrorCombiningLegacyToNew : unicode

