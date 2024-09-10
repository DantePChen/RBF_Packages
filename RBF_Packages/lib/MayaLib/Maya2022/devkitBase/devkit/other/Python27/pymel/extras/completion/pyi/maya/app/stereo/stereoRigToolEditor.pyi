from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
from . import stereoCameraErrors
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def __buildCB2(layout, func, name, lang, create, cameraSet): ...
def __changeProcedure(layout, control, rig): ...
def __changeLang(layout, control, rig): ...
def __oneItem(layout, rig, definition, mode):
    """
    Build the UI for one rig
    """
    ...
def customRigEditor():
    """
    Create the custom stereo rig editor window
    """
    ...
def __rebuildUI(layout):
    """
    Rebuild the complete UI after a tool was added or removed
    """
    ...
def __buildCB(layout, func, control, param): ...
def buildMainToolUI():
    """
    Build the UI for this window
    """
    ...
def __changeCameraSetProcedure(layout, control, rig): ...
def rebuildUI(layout):
    """
    Rebuild the complete UI after a tool was added or removed. Using
    evalDeferred so that it can be attached to the UI controls.
    """
    ...
def __add(layout, nameBox, langMenu, createBox, createCamSet): ...
def __delete(layout, control, rig): ...


__mainWin : NoneType

