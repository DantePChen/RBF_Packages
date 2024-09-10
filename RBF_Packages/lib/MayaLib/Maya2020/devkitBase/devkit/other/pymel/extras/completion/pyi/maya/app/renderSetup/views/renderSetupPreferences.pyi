from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.renderSettings as renderSettings
import maya
import maya.app.renderSetup.model.renderSetup as renderSetupModel
import maya.app.renderSetup.model.aovs as aovs
import maya.cmds as cmds
import json
import maya.app.renderSetup.model.renderSetupPreferences as prefs
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

def loadUserPreset(preset):
    """
    # Loads the specified user preset.
    """
    ...
def getGlobalPresets(renderer):
    """
    # Returns the list of presets in the global presets directory.
    """
    ...
def getUserPresets(renderer):
    """
    # Returns the list of presets in the user presets directory.
    """
    ...
def loadGlobalPreset(preset):
    """
    # Loads the specified global preset.
    """
    ...
def _exportNodesOfGivenType(encodeFunction, captionName, filePath='None'): ...
def deleteUserPreset(preset, warn='True'):
    """
    # Deletes a user preset. Note: for testing purposes, noWarn can be set to True
    # to prevent a warning popup box on delete, this should only be used for 
    # testing!
    """
    ...
def exportAOVs(captionName, fp='None'): ...
def _selectPath(userTextField, option, title): ...
def addRenderSetupPreferences(): ...
def _syncOptionVarWithTextField(userTextField, option): ...
def setDefaultPreset(): ...
def savePreset(captionName, fp='None'):
    """
    # Saves a preset to a user specified location. Note: for testing purposes, a
    # filename can be passed in, this should only be used for testing!
    """
    ...
def _loadPreset(preset, basePath):
    """
    # Loads the specified preset from the specified directory
    """
    ...
def _getPresets(renderer, basePath):
    """
    # Returns the list of presets in the specified base path.
    """
    ...


kSelectUserPresetsLocation : unicode
kGlobalTemplatesLocation : unicode
kSelectGlobalPresetsLocation : unicode
kDeleteUserRenderSettingsMsg : unicode
kUserPresetsLocation : unicode
kCancel : unicode
kDelete : unicode
kRenderSettingsPresetsTitle : unicode
kExportRenderSettings : unicode
kRenderSetupTemplatesTitle : unicode
kSelectUserTemplatesLocation : unicode
kDeleteUserRenderSettings : unicode
kUserTemplatesLocation : unicode
kInvalidPresetFound : unicode
kSelectGlobalTemplatesLocation : unicode
kGlobalPresetsLocation : unicode

