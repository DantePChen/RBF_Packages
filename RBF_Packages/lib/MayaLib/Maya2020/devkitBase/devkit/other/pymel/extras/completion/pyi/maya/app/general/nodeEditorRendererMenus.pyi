from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def addMrShaderMenuItems(ned, node):
    """
    Check for mental ray shader node and create necessary menu items
    """
    ...
def _addMrLightShaderMenuItems(ned, node):
    """
    Check for mental ray light shader node and create necessary menu items
    """
    ...
def _addMrTextureShaderMenuItems(ned, node):
    """
    Check for mental ray texture shader node and create necessary menu items
    """
    ...
def _isClassified(node, classification): ...
def _createMrLightShaderMenuItems(node):
    """
    Create node item marking menu items specific to mental ray light shader nodes
    """
    ...
def _createMrTextureShaderMenuItems(node):
    """
    Create node item marking menu items specific to mental ray texture shader nodes
    """
    ...


customMrNodeItemMenuCallbacks : list

