from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import simpleSelector
from . import collection
from . import selectorFactory
import maya.app.renderSetup.model.selector as modelSelector
import maya.app.renderSetup.model.collection as modelCollection
from . import collectionFactory


from maya.app.renderSetup.views.propertyEditor.basicCollection import BasicCollection
from maya.app.renderSetup.views.propertyEditor.staticCollection import StaticCollection


if False:
    from typing import Dict, List, Tuple, Union, Optional

def initialize(): ...
def uninitialize(): ...


_collectionEntries : dict
_selectorEntries : dict

