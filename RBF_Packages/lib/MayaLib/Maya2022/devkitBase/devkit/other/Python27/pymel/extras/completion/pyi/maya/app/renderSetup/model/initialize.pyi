from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import plug
from . import nodeList
from . import renderSetupPreferences as rs_prefs
from . import fileLoadMonitor
from . import shadingNodes
from . import nodeListPrivate
from . import dragAndDropBehavior
from . import undo
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
from . import overriddenAttributeManager
from . import renderSetup
from . import renderSetupPrivate
from . import override
from . import modelCmds
import maya
from . import localOverride
from . import childNode


from maya.app.renderSetup.model.overrideUtils import getAllOverrideClasses
from maya.app.renderSetup.model.selector import SimpleSelector
from maya.app.renderSetup.model.selector import Selector
from maya.app.renderSetup.model.collection import getAllCollectionClasses
from maya.app.renderSetup.model.applyOverride import getAllApplyOverrideClasses
from maya.app.renderSetup.model.renderSetup import RenderSetup as nodeType
from maya.app.renderSetup.model.renderLayer import RenderLayer
from maya.app.renderSetup.model.selector import LightsCollectionSelector
from maya.app.renderSetup.model.selector import BasicSelector


if False:
    from typing import Dict, List, Tuple, Union, Optional

def setVisibilityNodes(val, nodeTypeNamesList):
    """
    Set the visibility of the nodes in nodeTypeNamesList to val (either
    True or False). This affects the visibility of those nodes in editors.
    """
    ...
def initialize(mplugin): ...
def uninitialize(mplugin): ...


nodeDragAndDropBehaviors : list
renderSetupNodeNamesToShowInOutliner : list
nodeTypes : list
syntaxCommands : list
commands : list

