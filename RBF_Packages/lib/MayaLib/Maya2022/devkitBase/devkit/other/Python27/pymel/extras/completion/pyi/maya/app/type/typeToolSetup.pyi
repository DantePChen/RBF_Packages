from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as nom
import maya.OpenMaya as om
import maya
import PySide2.QtXml as qt
import maya.mel as mel
import PySide2.QtXml as QtXml
import PySide2.QtSvg as QtSvg
import maya.cmds as cmds
import platform
import shiboken2 as shiboken
from . import AEtypeTemplate as typeTemplate
import PySide2.QtWidgets as QtWidgets
import PySide2.QtCore as QtCore
import PySide2.QtGui as QtGui


from PySide2.QtCore import Slot
from ntpath import expanduser


if False:
    from typing import Dict, List, Tuple, Union, Optional

def wrapInstance(*args, **kwargs): ...
def createTypeToolWithNode(typeTool, font='None', style='None', text='None', legacy='False'): ...
def unwrapInstance(*args, **kwargs): ...
def connectShellDeformer(typeMesh, typeTransform, typeExtruder, typeTool): ...
def addPolyRemeshNodeType(typeTransform, typeExtruder, typeTool, typeMesh): ...
def addUVNodeToType(typeMesh, typeExtruder): ...
def connectTypeAdjustDeformer(typeMesh, typeTransform, typeExtruder, typeTool): ...
def createTypeTool(font='None', style='None', text='None', legacy='False'): ...


IN_BATCH_MODE : bool

