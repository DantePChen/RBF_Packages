from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import stereoCameraSets
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def __createSlaveCamera(masterShape, name, parent):
    """
    Private method to this module.
    Create a slave camera
    Make the default connections between the master camera and the slave one.
    """
    ...
def __createFrustumNode(mainCam, parent, baseName):
    """
    Private method to this module.
    Create a display frustum node under the given parent.
    Make the default connections between the master camera and the frustum  
    Remove some of the channel box attributes that we do not want to show
    up in the channel box.
    """
    ...
def createRig(basename="'stereoCamera'"):
    """
    Creates a new stereo rig. Uses a series of Maya commands to build
    a stereo rig.
    
    The optional argument basename defines the base name for each DAG
    object that will be created.
    """
    ...
def attachToCameraSet(*args, **keywords): ...
def registerThisRig():
    """
    Registers the rig in Maya's database
    """
    ...


rigTypeName : str

