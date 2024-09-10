from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import utils
from . import progressObservable as progressObs
import maya
import maya.app.renderSetup.common.utils as commonUtils


"""
This module is used to notify of progress during layer switching, which
can be a lengthy operation.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RenderLayerSwitchInfo(object):
    """
    Gets information on layer switch operations.
    Keeps track of the layer switch progress by estimating the number
    of apply overrides that could possibly need applying/unapplying.
    When a layer switch operation is in progress, we frequently update the
    ProgressObservable with the current information.
    """
    
    
    
    def __init__(self): ...
    def info(self):
        """
        Returns information about the current operation being
        computed during the layer switch.
        """
        ...
    def progress(self): ...
    def update(self):
        """
        Update the ProgressObservable (the subject) with
        the new information of where the progress is at.
        This includes the percentage of progress estimated to be done and
        the information related to the current operation being computed.
        """
        ...
    __dict__ : dictproxy
    
    
    
    __weakref__ : getset_descriptor


class _Context(object):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ApplyOverrideContext(_Context):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, ovr): ...


class UnapplyApplyOverrideContext(_Context):
    def __exit__(self, type, value, traceback): ...


class SwitchLayerContext(_Context):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, oldLayer, newLayer): ...


class ApplyApplyOverrideContext(_Context):
    def __exit__(self, type, value, traceback): ...


class UnapplyOverrideContext(_Context):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, ovr): ...


class UnapplyLayerImportRefContext(_Context):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, oldLayer, data='None'): ...




kUnapplyLayer : unicode
kApplyOverride : unicode
kApplyLayer : unicode
kUnapplyOverride : unicode
IMPORT : int
kReadResolveConflicts : unicode
kImporting : unicode
kReferencing : unicode
REFERENCE : int

