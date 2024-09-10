from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import rendererCallbacks
from . import renderSetup
from . import utils
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import maya


"""
AOV information is encoded and decoded by 3rdParty renderers. These renderers
register AOV callbacks by deriving from or implementing an interface identical
to the AOVCallbacks interface located in the rendererCallbacks module. This
class is then registered by calling:

rendererCallbacks.registerCallbacks(rendererName, 
                                    rendererCallbacks.CALLBACKS_TYPE_AOVS
                                    callbacksClassImplementation)
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def decode(aovsData, decodeType):
    """
    Decode and apply all the attribute values related to the AOVs of a specific renderer
    """
    ...
def getAllAOVNodesInScene():
    """
    Return a generator of all AOV nodes in the scene using their names
    """
    ...
def sceneHasAOVsApplyOverrides(): ...
def _getCurrentRenderer(): ...
def encode(wantMasterValues='False'):
    """
    Encode all the attribute values related to the AOVs of a specific renderer
    """
    ...
def encodeMasterValues():
    """
    Encode the master values of the attributes related to the AOVS
    of a specific renderer
    """
    ...


kImportWrongRSFile : unicode
kRendererMismatch : unicode

