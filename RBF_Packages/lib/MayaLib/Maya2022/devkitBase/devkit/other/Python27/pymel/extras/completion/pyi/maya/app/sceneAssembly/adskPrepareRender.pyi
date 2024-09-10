from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.OpenMaya as OpenMaya
import re
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def activateRender(a):
    """
    Recursively activate the render representation on an assembly node.
    Return true if the representation matches criteria at least once.
    """
    ...
def renderRepPredicate(aFnSet, aRepName):
    """
    Return true if the representation matches the criteria for name, label, and type. 
    If all criteria are empty, the representation does not match. 
    If one or more criteria are non-empty, those are used to determine the match.
    """
    ...
def preRender():
    """
    Prepare a scene for rendering by activating the render representation on all assembly nodes.
    """
    ...
def repNameChanged(repNameControl):
    """
    Callback invoked when the render settings UI control for the representation name regular expression text field changes.
    """
    ...
def repLabelChanged(repLabelControl):
    """
    Callback invoked when the render settings UI control for the representation label text field changes.
    """
    ...
def repTypeChanged(repTypeControl):
    """
    Callback invoked when the render settings UI control for the representation type text field changes.
    """
    ...
def useRegExChkBoxChanged(useRegExChkBoxControl):
    """
    Callback invoked when the render settings UI control for the representation check box to use regular expression changes.
    """
    ...
def settingsUI():
    """
    Populate a parent form layout with UI controls for our traversal set.
    """
    ...
def readFromGlobalsNode(): ...
def createPrepareRenderGlobalsNode():
    """
    # Ideally, we would create a default node (not saved in the scene file), and not implicitly created 
    # (only set attribute changes are saved to file, not connections, which we don't need). Unfortunately, 
    # default node creation from a command or the API is not possible. Furthermore, we should only be 
    # creating this node when the default traversal set is changed to adskPrepareRender.
    """
    ...


adskPrepareRenderGlobalsNode : str
useRegularExpression : bool

