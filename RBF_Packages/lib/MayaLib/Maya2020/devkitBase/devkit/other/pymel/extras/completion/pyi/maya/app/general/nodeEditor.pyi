from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
from . import nodeEditorMenus
import maya.cmds as cmds


"""
Plugin Support:

Plugins providing nodes with a custom creation script should add
callbacks to the lists:

pluginNodeClassificationCallbacks
pluginNodeCreationCallbacks

Following the Mentalray example at the bottom of this file.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NodeEditor(object):
    """
    Encapsulates one Node Editor panel instance.
    """
    
    
    
    def __init__(self, ned): ...
    def buildMenus(self):
        """
        Create any required UI for this editor
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def mayaNodeCategories(): ...
def nEd(*args, **kwargs): ...
def buildPanelMenus(ned):
    """
    Do the python menu creation.
    This is called by nodeEdBuildPanelMenus
    """
    ...
def _findCustomCreateCommand(nodeType):
    """
    Locate a custom command to create this nodeType, based on registered 
    custom classification categories. Return None if no match.
    """
    ...
def _findStandardCreateCommand(nodeType):
    """
    Locate a standard create command based on classification cateogries
    """
    ...
def mrClassificationCB():
    """
    eturn classification root of all nodes handled by the corresponding create callback.
    """
    ...
def createCallback(ned):
    """
    Do non-UI initialization
    """
    ...
def mrCreateNodeCB(postCmd, nodeType):
    """
    If the given node is Mentalray, return a MEL command which will create
    an instance of the supplied node type.  Return None if the given node
    is not handled by Mayatomr.
    
    \param[in] postCmd - MEL code to be executed after creation
    \param[in] nodeType - The type of the node to be created
    eturn MEL command which creates the given node, or None
    """
    ...
def addCallback(ned):
    """
    Create any required UI
    """
    ...
def removeCallback(ned):
    """
    Clean up any UI
    """
    ...
def createNode(nodeType):
    """
    Called by the editor to create a new node based on the supplied type.
    """
    ...
def mrNodeCategories(): ...
def _findCustomCreateCommand2012(nodeType):
    """
    Implementation of _findCustomCreateCommand for 2012
    """
    ...


_mrCreateNodeTable : list
editors : dict
pluginNodeCreationCallbacks : list
_createNodeTable : list
pluginNodeClassificationCallbacks : list

