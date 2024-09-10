from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import struct


"""
Class for storing apiVersions, which are the best method for comparing versions. ::

    >>> from pymel import versions
    >>> if versions.current() >= versions.v2008:
    ...     print "The current version is later than Maya 2008"
    The current version is later than Maya 2008
"""


from maya.OpenMaya import MGlobal as _MGlobal


if False:
    from typing import Dict, List, Tuple, Union, Optional

def is64bit():
    """
    Whether this instance of Maya is 64-bit
    
    Returns
    -------
    bool
    """
    ...
def installName(): ...
def bitness():
    """
    The bitness of python running inside Maya as an int.
    
    Returns
    -------
    int
    """
    ...
def isEval():
    """
    Whether this instance of Maya is an evaluation edition
    
    Requires ``maya.cmds``.
    
    Returns
    -------
    bool
    """
    ...
def current():
    """
    Get the current version of Maya
    
    Returns
    -------
    int
    """
    ...
def shortName(): ...
def isComplete():
    """
    Whether this instance of Maya is 'Unlimited' (deprecated)
    
    Returns
    -------
    bool
    """
    ...
def flavor():
    """
    The 'flavor' of this instance of Maya
    
    Requires ``maya.cmds``.
    
    Returns
    -------
    unicode
    """
    ...
def parseVersionStr(versionStr, extension='False'):
    """
    Parse a verbose version string (like the one displayed in the Maya title
    bar) and return the base version.
    
    Parameters
    ----------
    versionStr : str
    extension : bool
        if True, leave the -x64 tag
    
    Returns
    -------
    str
    
    Examples
    --------
    >>> from pymel.all import *
    >>> versions.parseVersionStr('2008 Service Pack1 x64')
    '2008'
    >>> versions.parseVersionStr('2008 Service Pack1 x64', extension=True)
    '2008-x64'
    >>> versions.parseVersionStr('2008x64', extension=True)
    '2008-x64'
    >>> versions.parseVersionStr('8.5', extension=True)
    '8.5'
    >>> versions.parseVersionStr('2008 Extension 2')
    '2008'
    >>> versions.parseVersionStr('/Applications/Autodesk/maya2009/Maya.app/Contents', extension=True)
    '2009'
    >>> versions.parseVersionStr('C:\Program Files (x86)\Autodesk\Maya2008', extension=True)
    '2008'
    """
    ...
def isRenderNode():
    """
    Returns
    -------
    bool
    """
    ...
def fullName(): ...
def isUnlimited():
    """
    Whether this instance of Maya is 'Unlimited' (deprecated)
    
    Returns
    -------
    bool
    """
    ...


v2013 : int
v2018_2 : int
v2012_HOTFIX4 : int
v2018 : int
v2009_SP1A : int
v20165_SP1 : int
v2008_EXT2 : int
v2016_EXT1SP4 : int
v2017 : int
v85_SP1 : int
v2014_EXT1 : int
v2014_SP1 : int
v2016_EXT1 : int
_is64 : bool
v2014_SP3 : int
v2012_HOTFIX1 : int
v2012_SAP1 : int
v2015_EXT1SP5 : int
v2011_HOTFIX1 : int
v2011 : int
_current : int
v2018_1 : int
v2014_EXT1SP2 : int
v2009_EXT1 : int
v2018_3 : int
v20165_SP2 : int
v2012_HOTFIX3 : int
v2018_4 : int
v2014_SP2 : int
v2012 : int
v2016 : int
v2014 : int
v2012_SAP1SP1 : int
_fullName : unicode
v2017U2 : int
v2015_SP2 : int
v2015_EXT1 : int
v2015 : int
v2019 : int
v20165 : int
v2011_HOTFIX3 : int
v2015_SP3 : int
v2017U1 : int
v2015_SP1 : int
v85 : int
v2011_SP1 : int
v2010 : int
v2011_HOTFIX2 : int
v2014_EXT1SP1 : int
v2017U3 : int
v2012_HOTFIX2 : int
v2009 : int
v2008 : int

