from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import maya.mel as mel
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class TselectionWin(object):
    """
    Base class for a dialog which works on the user's selection
    """
    
    
    
    def __del__(self): ...
    def __init__(self, title, selectionFilter="'<function <lambda>>'", objects='[]'):
        """
        selectionFilter - function which returns True if object is selectable
        """
        ...
    def activate(self, window):
        """
        Call this method once the window is created
        """
        ...
    def close(self): ...
    def getWindowTitle(self): ...
    def onSelectionChanged(self, *args):
        """
        Called anytime the selection list changes,
        self.objects is updated and window title is updated.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class TadjustBackgroundImageWin(TselectionWin):
    """
    Adjust the background image for a container Dialog
    """
    
    
    
    def __init__(self, editor): ...
    def hyperGraphCmd(self, *args, **kwargs): ...
    def loadImage(self, theFile): ...
    def onAdjustImagePositionHorizontal(self, val): ...
    def onAdjustImagePositionVertical(self, val): ...
    def onAdjustImageScale(self, val): ...
    def onFitToHeight(self, arg): ...
    def onFitToWidth(self, arg): ...
    def onImageFieldChange(self, val): ...
    def onLoadImage(self): ...
    def onSelectionChanged(self, *args):
        """
        override selection callback
        """
        ...
    def show(self):
        """
        Build and show the dialog
        """
        ...
    def update(self):
        """
        update the ui after something has changed
        """
        ...




def adjustBackgroundImageWin(editor):
    """
    Main entry point.  Create and show the adjust-background-image dialog.
    """
    ...

