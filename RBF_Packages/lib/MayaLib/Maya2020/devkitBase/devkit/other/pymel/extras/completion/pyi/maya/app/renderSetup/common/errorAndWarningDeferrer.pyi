from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ErrorAndWarningDeferrer:
    """
    # Note: this class was introduced as a work around. The problem is that the 
    # render setup plugin is being loaded in loadPreferredRenderGlobalsPreset,
    # and at that point, no warnings/errors can be displayed, so we decided to
    # queue up the warnings/errors for display when the render setup window was
    # opened.
    """
    
    
    
    def __init__(self): ...
    def displayErrorsAndWarnings(self, clearLog='True'): ...
    def registerError(self, error): ...
    def registerWarning(self, warning): ...




def instance(): ...


_instance : NoneType

