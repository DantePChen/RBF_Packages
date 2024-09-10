from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import pymel.util as util
import inspect
import string
import textwrap


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DocstringBuilder(object):
    def __init__(self, cmdName, indentation="'    '"): ...
    def addFlag(self, flag, typ, shortname, descr, modes):
        """
        Generate docs for a single flag
        """
        ...
    def addFooter(self): ...
    def build(self, docstring):
        """
        Add the generated docstrings to the existing docstring
        
        Parameters
        ----------
        docstring : str
            original docstring
        
        Returns
        -------
        str
        """
        ...
    def getTypeIdentifier(self, typ): ...
    def indent(self, lines): ...
    def startFlagSection(self): ...
    DOC_WIDTH : int
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class PyDocstringBuilder(DocstringBuilder):
    """
    Docstring builder that generates human-readable docstrings for use with
    the python help() function.
    """
    
    
    
    def addFlag(self, flag, typ, shortname, descr, modes): ...
    def startFlagSection(self): ...
    @staticmethod
    def section(title): ...
    DOC_WIDTH : int


class RstDocstringBuilder(DocstringBuilder):
    """
    Docstring builder that outputs reStructuredText for building HTML docs
    """
    
    
    
    def addFlag(self, flag, typ, shortname, descr, modes): ...
    def addFooter(self): ...
    def startFlagSection(self): ...
    @staticmethod
    def makerow(items, widths): ...
    @staticmethod
    def section(title): ...
    altwidths : list
    
    headersep : str
    
    rowsep : str
    
    w : int
    
    widths : list


class NumpyDocstringBuilder(DocstringBuilder):
    def addFlag(self, flag, typ, shortname, descr, modes): ...
    def getTypeIdentifier(self, typ): ...
    def startFlagSection(self): ...
    @staticmethod
    def section(title): ...
    DOC_WIDTH : int




def indent(s, margin): ...

