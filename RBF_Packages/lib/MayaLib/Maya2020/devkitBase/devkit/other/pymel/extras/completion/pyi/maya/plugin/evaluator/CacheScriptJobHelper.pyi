from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.cmds as cmds


from maya.common.ui import callback_tool
from maya.common.utils import Singleton
from maya.plugin.evaluator.CacheEvaluatorManager import CacheEvaluatorManager
from maya.debug.DebugTrace import DebugTrace


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CacheScriptJobHelper(object):
    """
    Class to manage the lifespan of scriptJobs so that they come and go when the plug-in is unloaded and loaded
    """
    
    
    
    def __init__(self):
        """
        Start monitoring the plug-in so that the state information is known
        """
        ...
    def add_client(self, client, event_name, callback_function):
        """
        Add a new scriptJob client function to an event relying on the cacheEvaluator plug-in being loaded.
        :param client: The "self" pointer of the class calling this
        :param event_name: Name of the scriptJob event to monitor
        :param callback_function: Function to call when the event fires
        """
        ...
    @staticmethod
    def callback_plugin_loaded(plugin_name):
        """
        Callback invoked whenever a plug-in is loaded.
        :param plugin_name: Name of the plug-in that was loaded. Only 'cache' is of interest.
        """
        ...
    @staticmethod
    def callback_plugin_unloaded(plugin_name):
        """
        Callback invoked whenever a plug-in is unloaded.
        :param plugin_name: Name of the plug-in that was unloaded. Only 'cache' is of interest.
        """
        ...
    __dict__ : dictproxy
    
    
    
    __weakref__ : getset_descriptor




def dbg(msg):
    """
    Print the debugging string with the class name prepended
    """
    ...


EVENT_SAFE_MODE : str
EVENT_EM_DISABLED : str
EVENT_LIMIT_CHANGE : str
CACHE_PLUGIN_NAME : str
DBG : DebugTrace
EVENT_CACHE_DESTROYED : str

