from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.OpenMayaRender as OpenMayaRender
import tempfile
import maya.OpenMayaMPx as OpenMayaMPx
import maya.cmds as cmds
import maya.OpenMaya as OpenMaya
import re


from maya.app.edl.fcp import *


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ImportExport(OpenMayaMPx.MPxCommand):
    def __del__(self): ...
    def __init__(self): ...


class Exporter(ImportExport):
    def __init__(self): ...
    def doIt(self, fileName): ...
    def setAllowPlayblast(self, allow):
        """
        If true, will re-playblast of all shots whose clips are out of date
        or non-existent.
        """
        ...


class Importer(ImportExport):
    def __init__(self): ...
    def doIt(self, fileName):
        """
        Reads an EDL file into Maya. Will generate shots, tracks and audio in Maya that 
        corresponds to the tracks and clips in the EDL.
        """
        ...
    def setStartFrameOverride(self, frame): ...




def getTimeCode(): ...
def doMel(*args, **kwargs):
    """
    Takes as input a string containing MEL code, evaluates it, and returns the result.
    
    This function takes a string which contains MEL code and evaluates it using 
    the MEL interpreter. The result is converted into a Python data type and is 
    returned.
    
    If an error occurs during the execution of the MEL script, a Python exception
    is raised with the appropriate error message.
    """
    ...
def videoClipCompare(a, b): ...
def audioClipCompare(a, b): ...
def _getValidClipObjectName(clipName, isVideo): ...
def getShotsResolution():
    """
    Returns the video resolution of the sequencer if all the shots have the same resolution 
    Otherwise it returns False, 0, 0
    """
    ...
def doExport(fileName, allowPlayblast):
    """
    Exports the Maya sequence using the EDL Exporter class.
    """
    ...
def doImport(fileName, useStartFrameOverride, startFrame):
    """
    Imports the specified file using the EDL Importer class.
    """
    ...
def _nameToNode(name): ...
def _setTimeCode(timecode): ...


mayaFrameRates : dict

