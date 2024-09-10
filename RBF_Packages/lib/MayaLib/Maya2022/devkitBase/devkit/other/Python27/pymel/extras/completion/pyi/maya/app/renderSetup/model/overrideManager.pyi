from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
from . import utils
import maya.OpenMaya as OpenMaya1_0


from maya.app.renderSetup.model.connectionOverride import ApplyConnectionOverride


if False:
    from typing import Dict, List, Tuple, Union, Optional

def postConstructor(layer): ...
def deleteApplyOverrideChain(last):
    """
    Delete a chain of apply override nodes.
    
    The argument is the last apply override node in the chain.
    """
    ...

