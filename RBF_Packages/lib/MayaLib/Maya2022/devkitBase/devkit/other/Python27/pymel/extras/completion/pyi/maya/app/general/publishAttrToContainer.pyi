from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import warnings


if False:
    from typing import Dict, List, Tuple, Union, Optional

def removePublishedAttrsFromContainer(obj, inAttrs): ...
def collectTripleAttrs(obj, attrs):
    """
    # Take a list of attrs and convert any contained triple children into
    # their parent. For example if the input is:
    #   diffuse, colorR, colorG, colorB
    # the return would be:
    #   diffuse, color
    """
    ...
def publishAttrToContainer(): ...
def pyError(errorString):
    """
    print an error message
    """
    ...
def unpublishAttrFromContainer(): ...
def addAndConnectObjAttrsToContainer(obj, inAttrs): ...

