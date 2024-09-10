from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ImageFormats:
    """
    Storage to keep track of all defined formats
    """
    
    
    
    def __init__(self):
        """
        # ---------------------------------------------------------------------------
        """
        ...
    def addFormat(self, ident, extension, description, properties='1', imfKey='None', otherExtensions='None'):
        """
        Add a new format to the table
        """
        ...
    def findIdent(self, ident):
        """
        return the format description corresponding to this Id, or None
        """
        ...
    def findKey(self, key):
        """
        return the format description corresponding to this IMF key, or None
        """
        ...
    def listFormats(self, type='255', returnFormat='None'):
        """
        List all the image formats that match the given type
        \param[in] type Property bits to test.
        \param[in] returnFormat Callback to format the result of matching
        items.
        """
        ...
    def popRenderGlobals(self): ...
    def pushRenderGlobalsForDesc(self, description):
        """
        return the format description corresponding to this Id, or None
        """
        ...


class ImageDescriptor:
    """
    All information necessary to identify an image format
    """
    
    
    
    def __init__(self, ident, extension, description, properties, imfKey='None', otherExtensions='None'):
        """
        # ---------------------------------------------------------------------------
        """
        ...
    def filterFormat(self):
        """
        Helper method to return this format as a file browser filter
        """
        ...
    def menuFormat(self):
        """
        Helper method to return this format as a suitable menu item
        """
        ...
    def prop(self, propBits):
        """
        True if the file format has all those propeties
        """
        ...
    kIsImage : int
    
    kIsLayered : int
    
    kIsMovie : int
    
    kIsSixteen : int
    
    kIsVector : int




def cmpFormat(x, y):
    """
    Helper method to compare two formats, using the description as
    the key
    """
    ...

