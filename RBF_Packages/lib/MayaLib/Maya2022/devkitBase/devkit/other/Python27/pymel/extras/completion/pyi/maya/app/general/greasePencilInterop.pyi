from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import xml
import xml.etree.ElementTree as etree
import string
import zipfile
import maya.cmds as cmds
import sys
import maya
import os


"""
# Grease Pencil interop file
#
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def readArchive(archivePath, tempDirectory):
    """
    # Reads an archive and extracts the xml file and the
    # file textures. We return a string that contains the
    # frame information along with the path to the
    # extracted files
    """
    ...
def _writeXmlSettings():
    """
    # Settings are hard coded for now
    """
    ...
def writeArchive(archivePath, frameData):
    """
    # Write a zip file containing the file textures and
    # the xml info file
    """
    ...
def writeXmlFile(xmlFilePath, timeList, fileList, layerList, durationList):
    """
    # Write out an xml file containing frame information. Each
    # frame has a time and a file path
    """
    ...
def readXmlFile(xmlFilePath, fileList):
    """
    # Read an xml file to extract frames(time,filePath). The frame information is converted
    # to a string so that it can be passed to C++. The second parameter 'fileList' is used
    # to make sure all frame files referred exist
    """
    ...


kDurationKeyword : str
kSettingsKeyword : str
kLayerKeyword : str
kFrameKeyword : str
kSettingKeyword : str
kFileKeyword : str
kTimeKeyword : str
kGreasePencilKeyword : str
kFPSKeyword : str
kFramesKeyword : str

