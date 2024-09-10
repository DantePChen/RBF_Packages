from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
import maya
import maya.cmds as cmds


from maya.app.renderSetup.lightEditor.model.group import LightGroup
from maya.app.renderSetup.lightEditor.model.item import LightItemBase
from maya.app.renderSetup.lightEditor.model.light import LightItem
from maya.app.renderSetup.lightEditor.model.editor import LightEditor


if False:
    from typing import Dict, List, Tuple, Union, Optional

def initialize(mplugin): ...
def uninitialize(mplugin): ...


invisibleNodeTypes : list
nodeTypes : list
kUnregisterFailed : unicode
kRegisterFailed : unicode

