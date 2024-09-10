from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as nom
import maya.OpenMaya as om
import PySide2.QtGui as QtGui
import maya.app.flux.core as fx
import maya.mel as mel
import PySide2.QtXml as QtXml
import PySide2.QtSvg as QtSvg
import PySide2.QtWidgets as QtWidgets
import shiboken2 as shiboken
import maya.cmds as cmds
import PySide2.QtCore as QtCore
import PySide2.QtXml as qt


from maya.app.flux.ui.core import dpi
from PySide2.QtCore import Slot
from maya.app.flux.ui.core import pix


if False:
    from typing import Dict, List, Tuple, Union, Optional

def wrapInstance(*args, **kwargs): ...
def unwrapInstance(*args, **kwargs): ...
def lineEditStyleSheet(): ...
def adjustPosition(window): ...
def resultsListStyleSheet(): ...
def getColoredCircle(color, width='14', height='14'): ...
def getColoredXshape(color): ...
def windowStyleSheet(): ...
def fixTTFIconScaling(pixmap): ...


colors : dict

