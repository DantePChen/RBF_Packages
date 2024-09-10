from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import proxy
from . import proxyFactory


from maya.app.renderSetup.lightEditor.model.light import LightItem
from maya.app.renderSetup.lightEditor.model.group import LightGroup


if False:
    from typing import Dict, List, Tuple, Union, Optional

def uninitialize(mplugin): ...
def initialize(mplugin): ...


proxyEntries : dict

