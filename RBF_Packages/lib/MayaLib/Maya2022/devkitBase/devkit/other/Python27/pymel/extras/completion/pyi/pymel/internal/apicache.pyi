from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import pymel.versions as versions
import inspect
from . import startup
import pymel.util as _util
from . import plogging as _plogging
import itertools
import pymel.api as api
import re
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

class InvalidNodeTypeError(exceptions.Exception):
    __weakref__ : getset_descriptor


class _GhostObjMaker(object):
    """
    Context used to get an mobject which we can query within this context.
    
    Automatically does any steps need to create and destroy the mobj within
    the context
    
    (Note - None may be returned in the place of any mobj)
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_value, traceback): ...
    def __init__(self, mayaTypes, dagMod='None', dgMod='None', manipError='True', multi='False'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ApiEnum(tuple):
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def pymelName(self): ...
    __dict__ : dictproxy


class GhostObjsOkHere(object):
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_value, traceback): ...
    @classmethod
    def OK(cls): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class BaseApiClassInfoCache(startup.SubItemCache):
    def fromRawData(self, data): ...
    def toRawData(self, data): ...
    CLASSINFO_SUBCACHE_NAME : NoneType


class ApiCache(BaseApiClassInfoCache):
    def __init__(self, docLocation='None'): ...
    def addMayaType(self, mayaType, apiType='None', updateObj='None'):
        """
        Add a type to the MayaTypes lists. Fill as many dictionary caches as we have info for.
        
            - mayaTypesToApiTypes
            - mayaTypesToApiEnums
        
        if updateObj is given, this instance will first be updated from it,
        before the mayaType is added.
        """
        ...
    def extraDicts(self): ...
    def fromRawData(self, data): ...
    def melBridgeContents(self): ...
    def rebuild(self):
        """
        Rebuild the api cache from scratch
        
        Unlike 'build', this does not attempt to load a cache file, but always
        rebuilds it by parsing the docs, etc.
        """
        ...
    def removeMayaType(self, mayaType, updateObj='None'):
        """
        Remove a type from the MayaTypes lists.
        
            - mayaTypesToApiTypes
            - mayaTypesToApiEnums
        
        if updateObj is given, this instance will first be updated from it,
        before the mayaType is added.
        """
        ...
    def toRawData(self, data): ...
    @classmethod
    def allVersions(cls, allowEmpty='False'): ...
    API_TO_MFN_OVERRIDES : dict
    
    CLASSINFO_SUBCACHE_NAME : str
    
    CRASH_TYPES : dict
    
    DEFAULT_API_TYPE : str
    
    DESC : str
    
    EXTRA_GLOBAL_NAMES : tuple
    
    MAYA_TO_API_OVERRIDES : dict
    
    MFN_TO_API_OVERRIDES : dict
    
    NAME : str
    
    USE_VERSION : bool


class ManipNodeTypeError(InvalidNodeTypeError):
    pass


class ApiMelBridgeCache(BaseApiClassInfoCache):
    def rebuild(self): ...
    def write(self, data, ext='None'):
        """
        # override write to preserve comments
        """
        ...
    @classmethod
    def applyComments(cls, origText, origTextNoComments, newPath): ...
    @classmethod
    def stripComments(cls, sourcelines):
        """
        Returns the text of the input python source lines with no comments,
        or None if the source did not have any comments
        
        sourcelines should have trailing newlines, ie, as returned by readlines
        """
        ...
    CLASSINFO_SUBCACHE_NAME : str
    
    DESC : str
    
    NAME : str
    
    USE_VERSION : bool




def _getRealMayaTypes(**kwargs): ...
def isPluginNode(nodeName):
    """
    # if we have MNodeClass, this is easy...
    """
    ...
def _getMayaTypes(real='True', abstract='True', basePluginTypes='True', addAncestors='True', noManips='True', noPlugins='False', returnRealAbstract='False'):
    """
    Returns a list of maya types
    
    Parameters
    ----------
    real : bool
        Include the set of real/createable nodes
    abstract : bool
        Include the set of abstract nodes (as defined by allNodeTypes(includeAbstract=True)
    basePluginTypes : bool
        Include the set of "base" plugin maya types (these are not returned by
        allNodeTypes(includeAbstract=True), and so, even though these types are
        abstract, this set shares no members with those added by the abstract
        flag
    addAncestors : bool
        If true, add to the list of nodes returned all of their ancestors as
        well
    noManips : Union[bool, str]
        If true, filter out any manipulator node types; if the special value
        'fast', then it will filter out manipulator node types, but will do so
        using a faster method that may potentially be less thorough
    noPlugins : bool
        If true, filter out any nodes defined in plugins (note - if
        basePluginTypes is True, and noPlugins is False, the basePluginTypes
        will still be returned, as these types are not themselves defined in
        the plugin)
    returnRealAbstract : bool
        if True, will return two sets, realNodes and abstractNodes; otherwise,
        returns a single set of all the desired nodes (more precisely, realNodes
        is defined as the set of directly createdable nodes matching the
        criteria, and abstract are all non-createable nodes matching the
        criteria)
    """
    ...
def getInheritance(mayaType, checkManip3D='True', checkCache='True', updateCache='True'):
    """
    Get parents as a list, starting from the node after dependNode, and
    ending with the mayaType itself.
    
    Raises a ManipNodeTypeError if the node type fed in was a manipulator
    """
    ...
def _defaultdictdict(cls, val='None'): ...
def _makeDgModGhostObject(mayaType, dagMod, dgMod): ...
def _getAbstractMayaTypes(**kwargs): ...
def nodeToApiName(nodeName): ...
def getLowerCaseMapping(names): ...
def _getAllMayaTypes(**kwargs): ...


_ASSET_PREFIX : str
API_NAME_MODIFIERS : list
_ABSTRACT_SUFFIX : str
_logger : _plogging.Logger
_fixedLineages : dict
_cachedInheritances : dict
apiSuffixes : list
replace : str
find : str
mpxNamesToApiEnumNames : dict

