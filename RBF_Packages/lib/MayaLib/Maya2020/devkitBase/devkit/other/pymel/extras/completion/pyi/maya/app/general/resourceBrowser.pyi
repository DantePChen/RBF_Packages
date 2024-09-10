from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.mel as mel
import os
import maya.cmds as cmds


from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

class resourceBrowser:
    def __init__(self):
        """
        # ---------------------------------------------------------------------------
        """
        ...
    def buttonCallback(self, data, dismissMsg="''"):
        """
        Button callback to end the dialog
        """
        ...
    def currentResourceName(self):
        """
        Return the current resource name or None
        """
        ...
    def info(self, msg):
        """
        Print msg if verbose mode is on
        """
        ...
    def populateUI(self):
        """
        Create the resource browser window UI
        """
        ...
    def run(self):
        """
        Display the Factory Icon Browser window. Return the selected
        resource or None
        """
        ...
    def saveCopy(self, data):
        """
        Button callback to end the dialog
        """
        ...
    def updateFilter(self, data):
        """
        Update the list based on the new filter
        """
        ...
    def updatePreview(self):
        """
        Select a new icon
        """
        ...



