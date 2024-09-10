from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.renderSetupPreferences as userPrefs
import json
import maya.app.renderSetup.model.renderSetup as renderSetup
import maya.app.renderSetup.model.jsonTranslatorGlobals as jsonTranslatorGlobals
import maya.app.renderSetup.common.guard as guard


if False:
    from typing import Dict, List, Tuple, Union, Optional

def exportFileNoLabelColor(filename): ...
def importFileNoLabelColor(filename): ...
def exportFile(filename, includeSceneSettings='True'): ...
def exportFileNoRenderSettings(filename): ...
def readFile(file):
    """
    Returns a list of the lines in the argument file.
    """
    ...
def importFile(filename):
    """
    # Cannot name function "import", as this is a reserved Python keyword.
    """
    ...

