from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import stereoCameraDefaultRig
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def createRig(basename="'stereoCameraHier'"):
    """
    Creates a new stereo rig. Uses a series of Maya commands to build
    a stereo rig.
    The optionnal argument basename defines the base name for each DAG
    object that will be created.
    """
    ...
def registerThisRig():
    """
    Registers the rig in Maya's database
    """
    ...

