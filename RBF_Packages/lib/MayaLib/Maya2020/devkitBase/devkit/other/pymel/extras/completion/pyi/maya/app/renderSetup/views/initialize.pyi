from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import viewCmds
import maya.mel as mel
from . import utils
import maya.app.renderSetup.views.proxy.initialize as proxyInitialize
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.views.propertyEditor.initialize as propertyEditorInitialize


from maya.app.renderSetup.views.renderSetupPreferences import addRenderSetupPreferences


if False:
    from typing import Dict, List, Tuple, Union, Optional

def initialize(mplugin): ...
def uninitialize(mplugin): ...


_initializeModules : list
commands : list

