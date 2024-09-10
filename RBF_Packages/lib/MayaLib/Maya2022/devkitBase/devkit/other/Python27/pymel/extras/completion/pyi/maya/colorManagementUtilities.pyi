from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def setViewTransform(editor, optionMenuPath, cmdName):
    """
    # Purpose: Sets the viewTransformName used by the Editor
    """
    ...
def toggleCM(editor, buttonPath, cmdName):
    """
    # Purpose: Toggles the color management in the editor
    """
    ...
def updateGamma(editor, floatFieldPath, cmdName):
    """
    # Purpose: Updates the gamma of the Editor
    """
    ...
def updateExposure(editor, floatFieldPath, cmdName):
    """
    # Purpose: Updates the exposure of the Editor
    """
    ...
def __runCommand(cmdName, parameterContent): ...
def syncGammaField(editor, floatFieldPath, cmdName):
    """
    # Purpose: Synchronizes the gamma of the editor with a floatField
    """
    ...
def syncExposureField(editor, floatFieldPath, cmdName):
    """
    # Purpose: Synchronizes the exposure of the editor with a floatField
    """
    ...

