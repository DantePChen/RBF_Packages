from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
from . import singleton
from . import utils as futils
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def registerTemplate(classPath, nodeType): ...
def addCustom(obj): ...
def registeredTypes(): ...

