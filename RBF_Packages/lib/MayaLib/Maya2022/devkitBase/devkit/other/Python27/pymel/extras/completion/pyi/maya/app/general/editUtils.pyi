from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.OpenMaya as OpenMaya
import re
import maya
import string
import maya.cmds as cmds


from maya.maya_to_py_itr import PyEditItr
from collections import deque


if False:
    from typing import Dict, List, Tuple, Union, Optional

class SelectionModel(object):
    """
    Encapsulate the lists edits window (mel UI) 
    selection model what indexes and strings are selected
    and how many edits are displayed in the UI
    This is to solve MAYA-45020
    """
    
    
    
    def __init__(self, text_scroll_list='None', indexes='None', edit_strings='None', edit_count='None'): ...
    def edit_count(self): ...
    def has_edit(self, edit_string): ...
    def has_index(self, index): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def displayUnappliedEdits(unappliedEdits, textScrollList, index):
    """
    Add all the unappliedEdits in the list to the textScrollList.
    Assumes proper filter has been handled by the caller and 
    only unappliedEdits that should be displayed have been added
    to the filter
    
    unappliedEdits   = list of unapplied edits to display
    textScrollList   = 'List Assembly Edits' list widget
    index                    = first unoccupied index in textScrollList
    
    Returns the next unoccupied index in the textScrollList
    """
    ...
def getEditsThatAffect(target):
    """
    Query edits that affect nodes contained in the specified
    assembly
    
    This will include edits made to nodes in the from any other
    assembly in the hierarchy above it
    """
    ...
def makeDependNode(name): ...
def printDivider(title, textScrollList, index): ...
def displayEditsOn(owner, textScrollList, filterWidget, failedEditsMenuItem, unappliedEditsMenuItem, nonRemovableEditsMenuItem):
    """
    Query edits that are stored on the owner node and add
    them to the 'List Assembly Edits' UI
    
    owner                   = assembly the edits are stored on
    textScrollList  = 'List Assembly Edits' list widget
    filterWidget    = 'List Assembly Edits' filter widget
    unappliedEdits  = list to gather all the unapplied edits, they will be
                                      displayed at the very end of the textScrollList
    failedEditsMenuItem = 'List Assembly Edits' show failed edit menu item
    unappliedEditsMenuItem = 'List Assembly Edits' show unapplied edit menu item
    nonRemovableEditsMenuItem = 'List Assembly Edits' show non-removable edit menu item
    """
    ...
def removeSelectedEdits(assembly, textScrollList, listEditsOnSelectedAssemblyMenuItem):
    """
    Try to remove the edits that are selected in 'List Assembly Edits' window.
    Note that only top-level edits can be removed.
    
    assembly                                                        = remove selected edits that affect nodes in this assembly
    textScrollList                                          = 'List Assembly Edits' list widget
    listEditsOnSelectedAssemblyMenuItem = 'List Assembly Edits' show edits stored on the assembly menu item
    
    Returns true if all the selected edits were removable.
    Returns false if any of the selected edits live on a nested assembly and can't be removed
    """
    ...
def doRemoveSelectedEdits(assemblyName, selection_model, listEditsOnSelectedAssembly):
    """
    This function does the work described for the "removeSelectedEdits"
    method below.
    """
    ...
def displayEditsWithIter(it, textScrollList, filterWidget, index, unappliedEdits, failedEditsMenuItem, unappliedEditsMenuItem, nonRemovableEditsMenuItem):
    """
    Iterate over edits using given iterator, and add them to the
    textScrollList. Make sure we comply with the appropriate the
    show options and colours.
    
    it                              = MItEdits setup with the appropriate owner/targetNode
    textScrollList  = 'List Assembly Edits' list widget
    filterWidget    = 'List Assembly Edits' filter widget
    index                   = first unoccupied index in textScrollList
    unappliedEdits  = list to gather all the unapplied edits, since
                                      they should be displayed at the end of the
                                      textScrollList. The calling function will be responsible
                                      for display of these edits
    failedEditsMenuItem = 'List Assembly Edits' show failed edit menu item
    unappliedEditsMenuItem = 'List Assembly Edits' show unapplied edit menu item
    nonRemovableEditsMenuItem = 'List Assembly Edits' show non-removable edit menu item
    
    Returns the next unoccupied index in the textScrollList
    """
    ...
def getEdits(owner, target):
    """
    Query edits that are stored on the given owner node.
    
    If target is not empty, we will only list
    edits that affect nodes in this assembly.
    
    If target is empty, we will list all edits
    stored on the given node
    """
    ...
def canActiveRepApplyEdits(assembly):
    """
    Is the active representation for this assembly one that can
    have edits applied to it?
    """
    ...
def displayEditsThatAffect(target, textScrollList, filterWidget, failedEditsMenuItem, unappliedEditsMenuItem, nonRemovableEditsMenuItem):
    """
    Query edits that affect nodes in the target assembly and
    add to 'List Assembly Edits' UI.
    
    Will list edits stored on any level in the hierarchy that
    affect nodes in 'target'. So if you have a hierarchy like this:
    
    A_AR
      |_ B_AR
        |_ C_AR
              |_ nodeInAssemblyC
    
    displayEditsThatAffect(C) will list...
    1) Edits made from C.ma that affect nodeInAssemblyC
    2) Edits made from B.ma that affect nodeInAssemblyC
    3) Edits made from A.ma that affect nodeInAssemblyC
    
    target                  = list edits that affect nodes in this assembly
    textScrollList  = 'List Assembly Edits' list widget
    filterWidget    = 'List Assembly Edits' filter widget
    failedEditsMenuItem = 'List Assembly Edits' show failed edit menu item
    unappliedEditsMenuItem = 'List Assembly Edits' show unapplied edit menu item
    nonRemovableEditsMenuItem = 'List Assembly Edits' show non-removable edit menu item
    """
    ...

