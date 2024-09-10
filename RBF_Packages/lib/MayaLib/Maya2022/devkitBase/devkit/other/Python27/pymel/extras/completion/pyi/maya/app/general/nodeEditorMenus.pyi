from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
import maya
from . import nodeEditorRendererMenus
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def _loadUIString(strId):
    """
    # This method is meant to be used when a particular string Id is needed more than
    # once in this file.  The Maya Python pre-parser will report a warning when a
    # duplicate string Id is detected.
    """
    ...
def _createLightMenuItems(node):
    """
    Create node item marking menu items specific to light nodes
    """
    ...
def createPlugMarkingMenu(ned, node, plug):
    """
    Build plug (attribute) marking menu
    """
    ...
def _createUtilityMenuItems(ned, node):
    """
    Create node item marking menu items specific to utility nodes
    """
    ...
def changeNodeLib(ned, createNodeWin):
    """
    change to the current node library
    """
    ...
def nEd(*args, **kwargs): ...
def _isClassified(node, classification): ...
def _createPsdFileTexMenuItems(ned, node):
    """
    Create node item marking menu items specific to psd file nodes
    """
    ...
def _createFileMenuItems(ned, node):
    """
    Create node item marking menu items specific to file nodes
    """
    ...
def _syncDisplayMenu(ned, menu):
    """
    Syncs the Display menu UI control state to its editor.
    """
    ...
def _createTextureMenuItems(ned, node):
    """
    Create node item marking menu items specific to texture nodes
    """
    ...
def _addBakeSetMenuItems(ned, node):
    """
    Check for bake set node and create necessary menu items
    """
    ...
def createTabContextMenu(ned, tabIndex):
    """
    Build tab context menu (markingMenu=False)
    """
    ...
def createNodeEditorMarkingMenu(ned):
    """
    Build work area marking menu
    """
    ...
def _createTextureUtilityMenuItems(ned, node):
    """
    Create node item marking menu items common to both texture and utility nodes
    """
    ...
def _addPlace3dTextureMenuItems(ned, node):
    """
    Check for place3dTexture node and create necessary menu items
    """
    ...
def _addColorProfileMenuItems(ned, node):
    """
    Check for color profile node and create necessary menu items
    """
    ...
def createNodeItemMarkingMenu(ned, node):
    """
    Build node item marking menu
    """
    ...
def _addLightMenuItems(ned, node):
    """
    Check for light node and create necessary menu items
    """
    ...
def _syncShapeMenuState(ned, menu):
    """
    Syncs the editor to the menu state.
    """
    ...
def _addAnimClipMenuItems(ned, node):
    """
    Check for anim clip node and create necessary menu items
    """
    ...
def _createBakeSetMenuItems(node):
    """
    Create node item marking menu items specific to bake set nodes
    """
    ...
def _createColorProfileMenuItems(node):
    """
    Create node item marking menu items specific to color profile nodes
    """
    ...
def _createPlace3dTextureMenuItems(node):
    """
    Create node item marking menu items specific to shader nodes
    """
    ...
def _createShaderMenuItems(ned, node):
    """
    Create node item marking menu items specific to shader nodes
    """
    ...
def _addUtilityMenuItems(ned, node):
    """
    Check for utility node and create necessary menu items
    """
    ...
def createConnectionMarkingMenu(ned):
    """
    Build connection marking menu
    """
    ...
def _addTextureMenuItems(ned, node):
    """
    Check for texture node and create necessary menu items
    """
    ...
def _createDisplayMenu(ned, menu):
    """
    Builds the Display menu for the panel.
    """
    ...
def _addCustomNodeItemMenus(ned, node):
    """
    Add custom menu items to the node item marking menu
    """
    ...
def _addShaderMenuItems(ned, node):
    """
    Check for shader node and create necessary menu items
    """
    ...
def _createAnimClipMenuItems(node):
    """
    Create node item marking menu items specific to anim clip nodes
    """
    ...


customExclusiveNodeItemMenuCallbacks : list
customInclusiveNodeItemMenuCallbacks : list

