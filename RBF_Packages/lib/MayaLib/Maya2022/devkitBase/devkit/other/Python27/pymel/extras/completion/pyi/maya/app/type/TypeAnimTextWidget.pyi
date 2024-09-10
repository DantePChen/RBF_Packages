from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as nom
import maya.OpenMaya as om
import maya.OpenMaya as old
import PySide2.QtXml as qt
import maya.app.flux.core as fx
import maya.mel as mel
import PySide2.QtXml as QtXml
import PySide2.QtCore as QtCore
import PySide2.QtSvg as QtSvg
import PySide2.QtWidgets as QtWidgets
import json
import shiboken2 as shiboken
import maya
import maya.cmds as cmds
import maya.OpenMayaUI as mui
import PySide2.QtGui as QtGui


from PySide2.QtCore import Slot


if False:
    from typing import Dict, List, Tuple, Union, Optional

class AnimTextQTreeWidgetExtend(QtWidgets.QTreeWidget):
    def __init__(self, node, parent='None'): ...
    def addItem(self, frame, name): ...
    def addItemAction(self): ...
    def deleteItems(self): ...
    def eventFilter(self, source, event): ...
    def mousePressEvent(self, event): ...
    def onChanged(self): ...
    def openMenu(self, point): ...
    def updateContent(self): ...
    staticMetaObject : QtCore.QMetaObject


class MyDelegate(QtWidgets.QStyledItemDelegate):
    def createEditor(self, parent, option, index): ...
    def setEditorData(self, editor, index): ...
    def setModelData(self, editor, model, index): ...
    def sizeHint(self, option, index): ...
    staticMetaObject : QtCore.QMetaObject


class TypeAnimTextWidget(QtWidgets.QWidget):
    def __init__(self, node, parent='None'): ...
    def set_node(self, node):
        """
        #update connections
        """
        ...
    staticMetaObject : QtCore.QMetaObject




def wrapInstance(*args, **kwargs): ...
def HexToUni(hexStr): ...
def unwrapInstance(*args, **kwargs): ...
def build_qt_widget(lay, node): ...
def update_qt_widget(layout, node): ...
def ByteToHex(byteStr):
    """
    Convert a byte string to it's hex string representation e.g. for output.
    """
    ...


kText : unicode
kFrame : unicode

