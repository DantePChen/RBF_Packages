from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
import logging


"""
This module defines proxy observables for receiving notifications from
Maya.  The proxy observables have an observer reference count that is used
to attach to the Maya notification when greater than zero, and detach from
the Maya notification when the reference count falls to zero.

In this way, no time is spent processing Maya notifications when no
observers are listening.
"""


from maya.app.renderSetup.model.observable import Observable


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ObservableProxy(Observable):
    """
    Base proxy observable class.
    
    This class provides a no-op implementation of activate() and deactivate().
    """
    
    
    
    def __del__(self): ...
    def activate(self): ...
    def deactivate(self): ...


class ObservableDGProxy(ObservableProxy):
    """
    Proxy observable class for DG callbacks.
    """
    
    
    
    def __init__(self, cbName, cb='None', cbArgs='None'): ...
    def activate(self): ...
    def addItemObserver(self, obs): ...
    def deactivate(self): ...
    def removeItemObserver(self, obs): ...


class ObservableDagProxy(ObservableDGProxy):
    """
    Proxy observable class for DAG callbacks.
    """
    
    
    
    def __init__(self, cbName, dagCbTypes, cb): ...
    def activate(self): ...
    def deactivate(self): ...




logger : logging.Logger

