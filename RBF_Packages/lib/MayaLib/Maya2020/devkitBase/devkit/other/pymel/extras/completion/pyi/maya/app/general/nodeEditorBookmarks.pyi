from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import maya
import maya.cmds as cmds


"""
Bookmarks are saved using the special node nodeGraphEditorBookmarkInfo.

Each Node Editor Bookmark is represented by a nodeGraphEditorBookmarkInfo which stores the bookmark info, i.e. the view state and the graph state, including the state of each individual node in the scene.
"""


from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NodeEditorBookmarksWindow(object):
    """
    Tracks the state of the Bookmarks window UI
    """
    
    
    
    def __init__(self, ned): ...
    def reset(self): ...
    def selectLastLoadedInfo(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def showWindow(ned, *args):
    """
    Show the Bookmarks window.  If it already exists, re-build it
    """
    ...
def loadBookmark(ned, info, *args):
    """
    apply the supplied bookmark
    """
    ...
def _getInstance(): ...
def _getBookmarkName(name): ...
def buildToolbar(ned):
    """
    builds the bookmarking toolbar elmenets for the given editor
    """
    ...
def _deleteBookmarks(infos): ...
def _getNextBookmark(ned, incr): ...
def _naturalKey(str_): ...
def buildMenu(ned):
    """
    builds the Bookmarks menu for the panel menuBar
    """
    ...
def renameBookmark(ned, oldname, info, *args):
    """
    Rename the supplied bookmark, given the old name
    """
    ...
def _getBookmarkInfos(): ...
def _refreshWindow(): ...
def loadPreviousBookmark(ned):
    """
    load the previous bookmark, based on alphabetical ordering of bookmarks
    """
    ...
def removeCallback(ned='None'):
    """
    called when the owning panel is removed, and on file-new
    """
    ...
def createBookmark(ned, *args):
    """
    create a new bookmark
    """
    ...
def loadNextBookmark(ned):
    """
    load the next bookmark, based on alphabetical ordering of bookmarks
    """
    ...
def _getDescriptionForInfo(bookmarkInfo): ...
def addCallback(ned):
    """
    called when a panel is added
    """
    ...
def _renameBookmark(info, name): ...


_panelInfos : dict
_instance : NoneType
windowName : str

