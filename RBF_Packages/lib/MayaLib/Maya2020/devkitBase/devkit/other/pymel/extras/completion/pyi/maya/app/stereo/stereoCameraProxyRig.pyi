from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import stereoCameraRig
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def registerThisRig():
    """
    Registers the rig in Maya's database
    """
    ...
def createRig(basename="'stereoCameraProxy'", centerCam='None', camRoot='None', partOfMulti='False'):
    """
    Creates a new custom stereoCameraProxy rig.
    """
    ...
def __createAControlCamera():
    """
    Creates a main control camera. This is a private function and not
    intended to be called outside of this module.
    """
    ...
def __createSlave(name, parent):
    """
    Creates a left / right slave camera.  This is private method and
    not intended to be called from outside this module.
    """
    ...

