from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import observable
import maya.app.renderSetup.common.errorAndWarningDeferrer as errorAndWarningDeferrer
import os
import maya.cmds as cmds


from distutils.dir_util import copy_tree


if False:
    from typing import Dict, List, Tuple, Union, Optional

class IncludeAllLightsSettingContextManager:
    """
    Make sure to update visible layer membership when the value of
    the setting include all lights changes during a set/unset of the
    optionVar or the envVar.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_val, exc_tb): ...
    def __init__(self): ...
    @staticmethod
    def updateVisibleLayerMembership(): ...


class DisplayRSNodesSettingContextManager:
    """
    Make sure to change the visibility of render setup nodes in editors
    if needed during a set/unset of the corresponding optionVar or envVar.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_val, exc_tb): ...
    def __init__(self): ...


class BaseSetting(object):
    """
    Accessor and mutator class methods to be used by derived classes
    for various Render Setup Options/Settings.
    All of those Render Setup Options have to implement their own boolean
    variables kEnvVar, kOptionVar and kDefault to be derived properly from this
    base class.
    Manipulate the environment and option variables that toggle
    the setting on or off. If an environment variable is set, we
    ignore the option variable.
    Else, take care of toggling the setting on or off when asked.
    This class should be considered abstract and should not be used as is
    (only the derived classes are usable), hence why its variables are None.
    """
    
    
    
    def __init__(self):
        """
        # No need for an instance of this class, it is only encapsulating methods to
        # access and set various settings. (this is to make pylint happy)
        """
        ...
    @classmethod
    def getEnvVar(cls):
        """
        " Return the value of the environment variable
        or None if it is unset. The user can use an environment variable
        to override user preferences and enable/disable the setting.
        """
        ...
    @classmethod
    def getOptionVar(cls): ...
    @classmethod
    def hasEnvVar(cls): ...
    @classmethod
    def hasOptionVar(cls): ...
    @classmethod
    def isEnabled(cls):
        """
        Return whether or not the setting is enabled according
        to the corresponding environment variable or user preference
        """
        ...
    @classmethod
    def setEnvVar(cls, val): ...
    @classmethod
    def setOptionVar(cls, val): ...
    @classmethod
    def toggleOptionVar(cls): ...
    @classmethod
    def unsetEnvVar(cls): ...
    @classmethod
    def unsetOptionVar(cls): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    kDefault : NoneType
    
    kEnvVar : NoneType
    
    kOptionVar : NoneType


class DisplayRSNodesSetting(BaseSetting):
    """
    Accessor and mutator static/class methods for
    Display Render Setup Nodes setting.
    By default, we hide render setup nodes from the editors.
    """
    
    
    
    @classmethod
    def setEnvVar(*args, **kwargs): ...
    @classmethod
    def setOptionVar(*args, **kwargs): ...
    @classmethod
    def unsetEnvVar(*args, **kwargs): ...
    @classmethod
    def unsetOptionVar(*args, **kwargs): ...
    kDefault : bool
    
    kEnvVar : str
    
    kOptionVar : str


class IncludeAllLightsSetting(BaseSetting):
    """
    Accessor and mutator static/class methods for
    Include All Lights setting.
    """
    
    
    
    @classmethod
    def setEnvVar(*args, **kwargs): ...
    @classmethod
    def setOptionVar(*args, **kwargs): ...
    @classmethod
    def unsetEnvVar(*args, **kwargs): ...
    @classmethod
    def unsetOptionVar(*args, **kwargs): ...
    kDefault : bool
    
    kEnvVar : str
    
    kOptionVar : str


class AlwaysListVisibleLayerSetting(BaseSetting):
    """
    Accessor and mutator static/class methods for
    Always list visible layer setting.
    """
    
    
    
    @classmethod
    def addOptionChangeObserver(cls, obsMethod): ...
    @classmethod
    def getEnvVar(cls): ...
    @classmethod
    def hasEnvVar(cls): ...
    @classmethod
    def hasOptionChangeObserver(cls, obsMethod): ...
    @classmethod
    def removeOptionChangeObserver(cls, obsMethod): ...
    @classmethod
    def setEnvVar(cls, val): ...
    @classmethod
    def setOptionVar(cls, val): ...
    kDefault : bool
    
    kOptionVar : str
    
    optionChangeObservable : observable.Observable


class ExportRenderSettingsAOVs(BaseSetting):
    """
    Accessor and mutator static/class methods for
    Export Render Settings AOVs.
    """
    
    
    
    @classmethod
    def isEnabled(cls):
        """
        Return whether or not the setting is enabled according
        to the corresponding environment variable or user preference
        """
        ...
    @classmethod
    def setToggled(cls, value):
        """
        When the user checks/unchecks the box this value will update.
        This value will override environment variables temporarily.
        """
        ...
    kDefault : bool
    
    kEnvVar : str
    
    kOptionVar : str
    
    kToggled : bool


class UntitledCollectionsSetting(BaseSetting):
    """
    Accessor and mutator static/class methods for
    Untitled Collections setting.
    """
    
    
    
    kDefault : bool
    
    kEnvVar : str
    
    kOptionVar : str




def getUserPresetsDirectory(): ...
def getGlobalTemplateDirectory(): ...
def _getUserDirectory(userDirectoryOptionVar, defaultUserDirectoryName): ...
def _getGlobalDirectory(globalDirectoryOptionVar): ...
def getEditMode(): ...
def getGlobalTemplateDirectoryWithoutCheck():
    """
    For asynchronous purpose, we want to check if the path
    really exists AFTER getting the path string
    """
    ...
def getGlobalPresetsDirectory(): ...
def initialize(): ...
def displayRSNodesSettingDecorator(f):
    """
    Use the DisplayRSNodesSetting's context manager.
    """
    ...
def setEditMode(value): ...
def includeAllLightsSettingDecorator(f):
    """
    Use the IncludeAllLightsSetting's context manager.
    """
    ...
def getUserTemplateDirectory(): ...
def getFileExtension(): ...


kOptionVarUserTemplateDirectory : str
kOptionVarUserPresetsDirectory : str
kOptionVarGlobalTemplateDirectory : str
kOptionVarEditMode : str
kOptionVarGlobalPresetsDirectory : str
kGlobalPresetsPathInvalid : unicode
kGlobalTemplatePathInvalid : unicode

