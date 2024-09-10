from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import logging
import pymel.api as _api
from . import uitypes
import pymel.api.plugins as _plugins
import pymel.internal.pmcmds as _pmcmds
from . import nodetypes as nt
import pymel.internal.cmdcache as _cmdcache
import maya.cmds as cmds
import pymel.internal as _internal
import pymel.api as api
from . import uitypes as ui
import pymel.versions as _versions
import pymel.internal.factories as _factories
from . import datatypes as dt
from . import runtime
from . import nodetypes
import pymel as _pymel
import pymel.internal.startup as _startup


from pymel.core.system import *
from pymel.core.animation import *
from pymel.core.context import *
from pymel.core.language import *
from pymel.core.other import *
from pymel.core.rendering import *
from pymel.core.modeling import *
from pymel.core.windows import *
from pymel.core.effects import *
from pymel.core.general import *


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _addPluginNode(pluginName, mayaType): ...
def _addPluginCommand(pluginName, funcName): ...
def _pluginLoaded(*args): ...
def _installCallbacks():
    """
    install the callbacks that trigger new nodes and commands to be added to pymel when a
    plugin loads.  This is called from pymel.__init__
    """
    ...
def _removePluginNode(pluginName, node): ...
def _stripPluginExt(pluginName): ...
def _pluginUnloaded(*args): ...
def _removePluginCommand(pluginName, command): ...


_pluginData : dict
_uitypes_names : list
_pluginLoadedCB : bool
_logger : logging.Logger

