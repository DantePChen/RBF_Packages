from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import logging
import maya.OpenMaya as OpenMaya1_0
from . import utils
import maya.api.OpenMaya as OpenMaya
import maya.app.renderSetup.common.guard as guard
import time


"""
It listens to Maya scene change events and it forwards them to the
registered callbacks when global observation is enabled (sceneObserversEnabled).
"""


from maya.app.renderSetup.model.observableProxy import ObservableDGProxy
from maya.app.renderSetup.model.observableProxy import ObservableProxy
from maya.app.renderSetup.model.observableProxy import ObservableDagProxy


if False:
    from typing import Dict, List, Tuple, Union, Optional

class SceneObservable(object):
    def __del__(self): ...
    def __init__(self): ...
    def activate(self):
        """
        Create callbacks to listen to scene changes.
        """
        ...
    def activated(self): ...
    def beforeNew(self):
        """
        Infrastructure method called on file new.
        
        This method should not be called by SceneObservable clients.
        """
        ...
    def deactivate(self):
        """
        Removes callbacks to listen to scene changes
        """
        ...
    def isInSceneChangeCallback(self): ...
    def register(self, eventType, observer):
        """
        Add a callback for the given event(s).
        """
        ...
    def unregister(self, eventType, observer):
        """
        Removes a callback for the given event(s).
        """
        ...
    BEFORE_FILE_EXPORT : str
    
    BEFORE_FILE_IMPORT : str
    
    BEFORE_FILE_OPEN : str
    
    BEFORE_REFERENCE_CREATE : str
    
    BEFORE_REFERENCE_LOAD : str
    
    BEFORE_REFERENCE_REMOVE : str
    
    BEFORE_REFERENCE_UNLOAD : str
    
    CONNECTION_CHANGED : str
    
    FILE_EXPORTED : str
    
    FILE_IMPORTED : str
    
    FILE_OPENED : str
    
    NODE_ADDED : str
    
    NODE_REMOVED : str
    
    NODE_RENAMED : str
    
    NODE_REPARENTED : str
    
    REFERENCE_CREATED : str
    
    REFERENCE_LOADED : str
    
    REFERENCE_REMOVED : str
    
    REFERENCE_UNLOADED : str
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def enableSceneObservers(value): ...
def sceneObserversEnabled(): ...
def isInSceneChangeCallback(): ...
def instance(): ...


_renderSetup_sceneObserversEnabled : bool
_sceneObservable : SceneObservable
logger : logging.Logger

