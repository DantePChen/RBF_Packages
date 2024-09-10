from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.utils as utils
from . import viewCmds
import maya.app.renderSetup.model.renderSetup as renderSetupModel
import maya.app.renderSetup.model.collection as collection


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _getSources(nodeName):
    """
    Returns objects to add override in.
    i.e. selected render settings child collections in the visible render layer, otherwise,
    selected collections in the visible render layer if any, otherwise, the layer itself.
    """
    ...
def createRelativeOverride(nodeName, attrName):
    """
    Add a relative override to the selected collections part of the visible render layer
    or to the layer itself if no collections are selected
    """
    ...
def createConnectionOverride(nodeName, attrName):
    """
    Add a connection override to the selected collections part of the visible render layer
    or to the layer itself if no collections are selected
    """
    ...
def createAbsoluteOverride(nodeName, attrName):
    """
    Add an absolute override to selected collections part of the visible render layer 
    or to the layer itself if no collections are selected
    """
    ...

