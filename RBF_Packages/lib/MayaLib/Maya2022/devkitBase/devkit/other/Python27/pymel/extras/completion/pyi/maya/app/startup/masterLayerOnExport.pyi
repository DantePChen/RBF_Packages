from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.mel as mel
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.model.renderSetup as renderSetup
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _renderSetupEnabled(): ...
def finalize():
    """
    Remove the created callbacks
    """
    ...
def initialize():
    """
    Create 2 callbacks, one before and one after export, to change to the master
    layer and then back again.
    """
    ...
def _noSwitchToMaster(): ...
def _beforeFileExport(_):
    """
    Change to the master layer before file export.
    """
    ...
def _fileExported(_):
    """
    Change back to the previously saved render layer after export.
    """
    ...


_first_call_to_callback : bool
_exported_file_extension : NoneType
_callbackIDs : list
_supported_renderer_extensions : set

