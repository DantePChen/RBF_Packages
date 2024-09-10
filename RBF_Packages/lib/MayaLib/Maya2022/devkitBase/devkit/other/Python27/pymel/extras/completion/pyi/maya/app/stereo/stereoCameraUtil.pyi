from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
import maya.cmds as cmds
import sys
import imp
from . import stereoCameraErrors
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

def unloadPlugin(*args): ...
def reloadScripts(): ...
def runCallbackChecks(): ...
def __call(language, method, rigName, kwords='{}', cmd_args='[]'):
    """
    Private method to call a MEL or Python callback. Return 'Error' in
    case of error. We avoid None, [], '' because those are more likely
    to be returned by the command we call
    """
    ...
def performReloadChk():
    """
    Scans the current module database and looks for any modules that we
    own.  If we find a module that we own, reload it in case any changes
    have been made. In terms of module reloading, this module cannot be
    reloaded because it would imply that the code is changing while it
    is being executed. If you change this module, you must invoke a
    reload in the python shell prior to calling this script.
    """
    ...
def loadPlugin(): ...

