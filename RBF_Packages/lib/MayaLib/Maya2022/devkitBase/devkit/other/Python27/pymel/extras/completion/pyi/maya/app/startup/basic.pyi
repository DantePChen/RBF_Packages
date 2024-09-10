from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import masterLayerOnExport
import maya.utils as utils
import traceback
import maya.cmds as cmds
import sys
import atexit
from . import dontImportExportAOVs
import os
import types


"""
This module is always imported during Maya's startup.  It is imported from
both the maya.app.startup.batch and maya.app.startup.gui scripts
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def setupScriptPaths():
    """
    Add Maya-specific directories to sys.path
    """
    ...
def executeUserSetupOnly():
    """
    Look for userSetup.py in the search path and execute it in the "__main__"
    namespace
    """
    ...
def executeUserSetup(): ...
def executeSiteSetup():
    """
    Look for userSetup.py in the search path and execute it in the "__main__"
    namespace
    """
    ...


old_code : code
new_consts : tuple
_runOverriddenModule_already_executed : set
new_code : code

