from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import renderSetup as rs
import maya.app.renderSetup.model.connectionOverride as conOvModel
import maya.app.renderSetup.model.collection as colModel
from . import proxyFactory
import maya.app.renderSetup.model.override as ovModel


if False:
    from typing import Dict, List, Tuple, Union, Optional

def uninitialize(): ...
def initialize(): ...


_entries : dict

