from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import renderSettings
import maya
from . import renderSetup as renderSetupModel
from . import aovs
from . import jsonTranslatorGlobals
import maya.cmds as cmds
import json
from . import renderSetupPreferences as prefs
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

def resolveTemplateFile(fileName):
    """
    # Locate a template file by name.
    # Location:
    #    - First use the provided path (as an absolute path) if it exists
    #    - Second look in the user template directory
    #    - Third look in the global template directory
    """
    ...
def loadPreset(fileName):
    """
    # Load a render settings preset file from disk. Resolve the path if needed.
    # This method can be called when batch rendering from the command line using the "-rsp" flag.
    """
    ...
def resolvePresetFile(fileName):
    """
    # Locate a preset file by name.
    # Location:
    #    - First use the provided path (as an absolute path) if it exists
    #    - Second look in the user preset directory
    #    - Third look in the global preset directory
    """
    ...
def loadAOVs(fileName):
    """
    # Load an AOV preset file from disk.   Resolve the path if needed.
    # This method can be called when batch rendering from the command line using the "-rsa" flag.
    """
    ...
def loadTemplate(fileName):
    """
    # Load a template file from disk.  Resolve the path if needed.
    # This method can be called when batch rendering from the command line using the "-rst" flag.
    """
    ...


kTemplateFileNotFound : unicode
kPresetFileNotFound : unicode
kInvalidTemplateFound : unicode
kInvalidPresetFound : unicode

