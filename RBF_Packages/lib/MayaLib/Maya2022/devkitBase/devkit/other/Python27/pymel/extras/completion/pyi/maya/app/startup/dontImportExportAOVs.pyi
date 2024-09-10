from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.model.rendererCallbacks as rendererCallbacks
import os
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _getRenderers():
    """
    Return the list of renderers in the rendererCallbacks dict.
    """
    ...
def _getaovNodeTypes():
    """
    Return the list of AOV node types for all registered renderers.
    """
    ...
def _renderSetupEnabled(): ...
def initialize():
    """
    Create callbacks related to File Import and File Export, to prevent
    AOVs from being imported/exported. Theoretically, the callbacks should
    only be created when Render Setup is used. When legacy render layers
    are used instead, we should not execute the body of this function.
    In practice, we remove them during the first fileImport or fileExport
    that occurs.
    """
    ...
def _beforeFileExport(self): ...
def finalize():
    """
    Remove callbacks associated to file import operations.
    """
    ...
def _setDoNotWrite(nodes, state): ...
def _fileExported(self): ...
def _fileImported(_): ...
def _beforeFileImport(_):
    """
    No node added notification is sent during file import.  Therefore, we
    take a snapshot of all AOV nodes before the import, then take one
    after import, and remove all AOV nodes that were created by the
    import.
    """
    ...


_first_call_to_callback : bool
_aovNodes : list
_aovNodesPre : list
_aovNodeTypes : list
_callbackIDs : list
aovNodeTypesTest : list

