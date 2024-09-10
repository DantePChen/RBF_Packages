from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import os


"""
##############################################################################
#
# Utility functions for resolving file paths for Maya's file texture node.
# These utilities are used for dealing with UV tiling and frame numbering in
# the file name and can be used to get the current pattern/preset and list
# of matching files.
#
##############################################################################
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def getFilePatternString(filePath, useFrameExtension, uvTilingMode):
    """
    Given a path to a file and hints about UV tiling and frame extension usage,
    convert the path to a version with appropriate tags marking the UV tile
    and frame number.
    """
    ...
def computeUVForFile(filePath, filePattern):
    """
    Given a path to a file and the UV pattern it matches compute the 0-based UV
    tile indicated by the file name. If the filePath or pattern are poorly
    formed then (0,0) is returned.
    """
    ...
def _splitPath(filePath):
    """
    ##############################################################################
    # Private Utilities
    ##############################################################################
    """
    ...
def findAllFilesForPattern(pattern, frameNumber):
    """
    Given a path, possibly containing tags in the file name, find all files in
    the same directory that match the tags. If none found, just return pattern
    that we looked for.
    """
    ...
def computeUVForFiles(filePaths, filePattern):
    """
    Given a collection of paths to a file and the UV pattern it matches compute
    the 0-based UV tile indicated by the file name. If a filePath or the pattern
    are poorly formed then (0,0) is returned for that path.
    """
    ...
def _patternToRegex(pattern): ...


_taggedZeroBasedRegex : re.SRE_Pattern
_VTag : str
_frameTag : str
_UTag : str
_vTag : str
_oneBasedRegex : re.SRE_Pattern
_taggedOneBasedRegex : re.SRE_Pattern
_frameExtensionRegex : re.SRE_Pattern
_zeroBasedRegex : re.SRE_Pattern
_uTag : str
_udimTag : str
_udimRegex : re.SRE_Pattern

