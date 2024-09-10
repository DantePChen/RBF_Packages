from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import maya.mel as mel
import os


from maya.app.renderSetup.common.devtools import *
from maya.app.renderSetup.model.selector import *
from maya.app.renderSetup.model.issue import *


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Issue2016R2Collection(Issue):
    def __init__(self, resolveCallback='None'): ...


class ConversionFailed(exceptions.Exception):
    __weakref__ : getset_descriptor


class ConvertDialog:
    def __init__(self): ...
    def onHelp(self, *args): ...
    def onNo(self, *args): ...
    def onYes(self, *args): ...
    def prompt(self): ...


class Observer2016R2(object):
    def __init__(self): ...
    def activate(self): ...
    def assistedResolve(self): ...
    def autoResolve(self): ...
    def deactivate(self): ...
    def resolve(self): ...
    @staticmethod
    def instance(): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def sceneHasBasicSelector(): ...
def removeAutoConvertFlag(): ...
def hasAutoConvertFlag(): ...
def _findCollections(encodedData):
    """
    yields all the collection dictionary in the encodedData
    """
    ...
def _splitOverrides(ovrs): ...
def setAutoConvertFlag(value): ...
def _createSubCollection(name, filterType, customs, children): ...
def convert2016R2(encodedData):
    """
    This is the function to call to convert any encodedData (partial or not).
    It will find all the collections in encodedData and convert them to use simpleSelector if they do not already.
    See convertCollection() for more details.
    """
    ...
def convertCollection(collection):
    """
    Convert the encoded data of a collection of 2016 R2 (using a BasicSelector) into 
    a collection with a collection using a SimpleSelector.
    Creates subcollections to simulate the old "include hierarchy".
    Creates subcollections for shader overrides since they now apply to shading engines.
    """
    ...
def getAutoConvertFlag(): ...


kConvertYes : unicode
kConversionCompletedOk : unicode
kConvertNo : unicode
kConvertHelp : unicode
kIssueShortDescription : unicode
kConvertBatchError : unicode
kConversionFailedForMessage : str
kOptionVarAutoConvert : str
kConversionCompletedTitle : unicode
DECODE_AND_MERGE : int
kConversionCompletedMessage : unicode
kConversionFailedTitle : unicode
kRelativeHelpLink : str
kConvertTitle : unicode
kConversionCompletedForMessage : str
kConvertMessage : unicode
kConversionCompletedWithErrorsMessage : unicode

