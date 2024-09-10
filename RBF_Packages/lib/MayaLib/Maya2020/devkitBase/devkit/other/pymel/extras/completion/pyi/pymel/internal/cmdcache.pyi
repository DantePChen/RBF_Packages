from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import keyword
import pymel.versions as versions
import maya.mel as mm
import inspect
from . import startup
import maya.cmds as cmds
import pymel.util as util
import re
from . import plogging
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CmdCache(startup.SubItemCache):
    def build(self): ...
    def fromRawData(self, data): ...
    def rebuild(self):
        """
        Build and save to disk the list of Maya Python commands and their arguments
        
        WARNING: will unload existing plugins, then (re)load all maya-installed
        plugins, without making an attempt to return the loaded plugins to the
        state they were at before this command is run.  Also, the act of
        loading all the plugins may crash maya, especially if done from a
        non-GUI session
        """
        ...
    def toRawData(self, data): ...
    DESC : str
    
    ITEM_TYPES : dict
    
    NAME : str


class CmdExamplesCache(startup.PymelCache):
    DESC : str
    
    NAME : str
    
    USE_VERSION : bool


class CmdDocsCache(startup.PymelCache):
    DESC : str
    
    NAME : str


class CmdProcessedExamplesCache(CmdExamplesCache):
    USE_VERSION : bool




def getCmdInfoBasic(command): ...
def cmdArgMakers(force='False'): ...
def getCmdInfo(command, version, python='True'):
    """
    Since many maya Python commands are builtins we can't get use getargspec on them.
    besides most use keyword args that we need the precise meaning of ( if they can be be used with
    edit or query flags, the shortnames of flags, etc) so we have to parse the maya docs
    """
    ...
def getModule(funcName, knownModuleCmds): ...
def nodeCreationCmd(func, nodeType): ...
def getCallbackFlags(cmdInfo):
    """
    used parsed data and naming convention to determine which flags are callbacks
    """
    ...
def _getNodeHierarchy(version='None'):
    """
    get node hierarchy as a list of 3-value tuples:
        ( nodeType, parents, children )
    """
    ...
def testNodeCmd(funcName, cmdInfo, nodeCmd='False', verbose='False'): ...
def fixCodeExamples(style="'maya'", force='False'):
    """
    cycle through all examples from the maya docs, replacing maya.cmds with pymel and inserting pymel output.
    
    NOTE: this can only be run from gui mode
    WARNING: back up your preferences before running
    
    TODO: auto backup and restore of maya prefs
    """
    ...
def getModuleCommandList(category, version='None'): ...


cmdlistOverrides : dict
nodeTypeToNodeCommand : dict
secondaryFlags : dict
_logger : plogging.Logger
UI_COMMANDS : list
moduleCommandAdditions : dict
_cmdArgMakers : dict
moduleNameShortToLong : dict

