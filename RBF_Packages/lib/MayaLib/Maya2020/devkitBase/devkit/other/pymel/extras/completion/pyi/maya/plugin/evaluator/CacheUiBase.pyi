from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.debug.DebugTrace
import re
import maya.cmds as cmds


from functools import partial
from maya.common.ui import callback_tool
from maya.plugin.evaluator.cache_preferences import CachePreferences


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CacheUiBase(object):
    """
    Base class providing common behaviour for all CacheUiXXX caching UI element manager classes.
    
    :member delete_window_jobs: A dictionary containing the currently active uiDeleted scriptJobs
                                KEY = Window being monitored for deletion
                                VALUE = (ID, COUNT)
                                    ID    = scriptJob ID of the monitoring job
                                    COUNT = number of elements monitoring this particular window
    """
    
    
    
    def __init__(self):
        """
        Initialize local variables to be empty
        """
        ...
    def monitor_window(self):
        """
        Find the window above the current tool location and monitor for its deletion.
        This should be called whenever the UI code has setParent() to the proper location
        for inserting this UI element (even when it won't be inserted if the plug-in
        is unloaded since it has to monitor for plug-in loading events).
        :param element: UI element whose window is to be monitored
        """
        ...
    @staticmethod
    def callback_update_plugin_state(tool, new_state):
        """
        Callback to match the visibility of the UI elements with the plug-in loaded state.
        Derived classes must implement the plugin_state_change() methods to use this.
        :param tool: Reference to this object (self)
        :param new_state: True if the plug-in was just loaded, False if just unloaded
        """
        ...
    @staticmethod
    def callback_window_deleted(tool, window):
        """
        Callback invoked when the window of the UI element was destroyed
        :param tool: Reference to this object (self)
        :param window: Window that was just deleted
        """
        ...
    @staticmethod
    def find_window():
        """
        :return: The name of the window in which the current UI context lives
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def local_dbg(msg):
    """
    Access to the local debugging function, which just prepends the class name to debugging information
    """
    ...


CP_DBG : maya.debug.DebugTrace.DebugTrace
RE_WINDOW_FROM_ELEMENT : re.SRE_Pattern

