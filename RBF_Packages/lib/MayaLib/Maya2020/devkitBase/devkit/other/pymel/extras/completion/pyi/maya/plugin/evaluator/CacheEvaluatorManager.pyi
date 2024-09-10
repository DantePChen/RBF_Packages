from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import CacheEvaluatorManager
import json
import warnings
import maya
import maya.cmds as cmds


"""
Helper class that maintains the cache evaluator mode information.
Manages the cache evaluator-specific data, the common data is
managed by the base class.

The object is set up to use the Python "with" syntax as follows:

    from maya.plugin.evaluator.CacheEvaluatorManager import CacheEvaluatorManager
    with CacheEvaluatorManager() as mgr:
        mgr.cache_mode = some_mode

That will ensure the original states are all restored. There's no other
reliable way to do it in Python. If you need different syntax you can
manually call the method to complete the sequence:

    mgr = CacheEvaluatorManager()
    mgr.save_state()
    mgr.cache_mode = some_mode
    mgr.restore_state()

If you attempt to read or write a state value and the plug-in is not loaded
then a ValueError will be raised with a plug-in not loaded message.
"""


from maya.debug.TODO import TODO
from maya.debug.EvaluatorManager import EvaluatorManager
from maya.plugin.evaluator.cache_optionvar_states import CachePreferenceDiscardFramesOutOfRange
from maya.app.prefs.OptionVar import OptionVar
from traceback import format_list
from maya.debug.PlaybackManager import PlaybackManager
from maya.plugin.evaluator.cache_optionvar_states import CachePreferenceMemoryThreshold
from maya.plugin.evaluator.cache_optionvar_states import CachePreferenceResourceGuard
from maya.plugin.evaluator.cache_optionvar_states import CachePreferenceHud
from traceback import extract_tb


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CacheEvaluatorManager(EvaluatorManager):
    """
    Class for managing the cache evaluator state in a 'with' format. Remembers
    and restores the caching mode and parameters.
    """
    
    
    
    def __exit__(self, event_type, value, traceback):
        """
        Ensure the state is restored if this object goes out of scope
        """
        ...
    def __init__(self):
        """
        __enter__ is defined in the parent class
        """
        ...
    def as_json(self):
        """
        Display mechanism to retrieve evaluator information in a format conducive to JSON formatting
        """
        ...
    def cached_nodes(self, potential_nodes):
        """
        Find the currently cached nodes. This method doesn't care which cache the values are in
        only that at least one cache has data for a given node.
        :param potential_nodes: Set of nodes to check for caching. If None then use the list of
                                all evaluation graph nodes instead.
        :return: Set of nodes with data in a cache
        """
        ...
    def check_plugin(self):
        """
        Confirm that the plug-in is loaded.
        :raise OptionVar.StateError: The plug-in is not currently loaded
        """
        ...
    def rebuild_cache(self, wait_for_rebuild):
        """
        Rebuild the current cache, if available
        :param wait_for_rebuild: If true then do not return until the cache has rebuilt
        :return: True if the cache was rebuilt and is available
        """
        ...
    def restore_state(self):
        """
        Restore the cache evaluator to its original mode prior to creation of
        this object. Using the "with" syntax this will be called automatically.
        You only need to call explicitly when you instantiate the mode manager
        as an object.
        """
        ...
    def save_state(self):
        """
        Remember the current state of all EM related parameters so that they
        can be restored on exit.
        """
        ...
    def set_state(self, new_state):
        """
        Define the cache evaluator state parameters.
        :param new_state: State information in the format provided by as_json
                          Only key values specified will change. Others retain
                          their current values (*not* default values).
        """
        ...
    @staticmethod
    def flush_cache():
        """
        Flush the current cache, if any
        """
        ...
    @staticmethod
    def flush_cache_range(min_time, max_time, flush_inside_range):
        """
        Flush a portion of the current cache, if any
        :param min_time: Start of the time range for flushing
        :param max_time: End of the time range for flushing
        :param flush_inside_range: If True then flush anything in [min_time,max_time],
            otherwise flush anything outside that range (i.e. [-inf,min_time) U (max_time, inf])
        """
        ...
    @staticmethod
    def invalidate_cache():
        """
        Invalidate the current cache, if any
        """
        ...
    @staticmethod
    def wait_for_cache(max_time='2.0'):
        """
        Wait for any pending cache fills.
        :param max_time: Maximum number of seconds to wait for the cache to fill (0 = just check without waiting)
        :return: True if the cache is ready after waiting
        """
        ...
    @property
    def asynchronous(self):
        """
        :return: the evaluator's asynchronous state.
        The conversions are needed because the command returns the true/false value as a string.
        """
        ...
    @asynchronous.setter
    def asynchronous(self, value): ...
    @property
    def cache_mode(self):
        """
        :return: the list of rules descriptions being used for the current caching mode
        """
        ...
    @cache_mode.setter
    def cache_mode(self, value): ...
    @property
    def discard_frames_out_of_range(self):
        """
        :return: the evaluator's state value for discarding cache data outside the playback range
        """
        ...
    @discard_frames_out_of_range.setter
    def discard_frames_out_of_range(self, value): ...
    @property
    def fill_mode(self):
        """
        :return: the evaluator's current fill mode, decide when the cache will be filled.
        """
        ...
    @fill_mode.setter
    def fill_mode(self, value): ...
    @property
    def fill_order(self):
        """
        :return: the evaluator's background fill order
        """
        ...
    @fill_order.setter
    def fill_order(self, value): ...
    @property
    def flush_sync(self):
        """
        :return: the evaluator's flush synchronization mode
        """
        ...
    @flush_sync.setter
    def flush_sync(self, value): ...
    @property
    def hud(self):
        """
        :return: the evaluator's HUD display state value
        """
        ...
    @hud.setter
    def hud(self, value): ...
    @property
    def memory_threshold(self):
        """
        :return: the evaluator's resource guard state value
        """
        ...
    @memory_threshold.setter
    def memory_threshold(self, value): ...
    @property
    def prevent_frame_skipping(self):
        """
        :return: the evaluator's prevent-frame-skipping state value
        """
        ...
    @prevent_frame_skipping.setter
    def prevent_frame_skipping(self, value): ...
    @property
    def resource_guard(self):
        """
        :return: the evaluator's resource guard state value
        """
        ...
    @resource_guard.setter
    def resource_guard(self, value): ...
    @property
    def resource_state(self):
        """
        :return: String representing current resource usage compared to the limits
            unlimited   Usage is not being checked
            out         No Memory Left - 100% hit
            low         Between 90% and 100% usage
            okay        Less than 90% usage
        """
        ...
    @property
    def safe_mode(self):
        """
        :return: the evaluator's safe mode value
        """
        ...
    @safe_mode.setter
    def safe_mode(self, value): ...
    @property
    def safe_mode_messages(self):
        """
        :return: The messages, if safe mode was triggered. Otherwise returns None. (a read-only value)
        """
        ...
    @property
    def safe_mode_triggered(self):
        """
        :return: the evaluator's safe mode triggered value (a read-only value)
        """
        ...
    CacheEvaluatorState : Type[maya.plugin.evaluator.CacheEvaluatorManager.CacheEvaluatorState]




def non_vp2_viewport_exists():
    """
    Return True if a viewport that does not use Viewport 2.0 exists.
    """
    ...
def supported_shape_types_filter():
    """
    :return: List of node types supporting caching
    """
    ...
def standard_modes():
    """
    :return: List of (name,parameter_list) state setup values for the current standard set of usable modes
    """
    ...


KEY_FLUSH_SYNC : str
CACHE_STANDARD_MODE_VP2_HW_NO_FALLBACK : list
KEY_CACHE_MODE : str
CACHE_PLUGIN_NAME : str
KEY_HUD : str
KEY_SAFE_MODE : str
KEY_MEMORY_THRESHOLD : str
KEY_FILL_MODE : str
KEY_DISCARD_FRAMES_OUT_OF_RANGE : str
CACHE_STANDARD_MODE_VP2_SW : list
CACHE_STANDARD_MODE_EVAL : list
CACHE_STANDARD_MODE_EVAL_SHAPES : list
CACHE_STANDARD_MODE_VP2_HW : list
KEY_PREVENT_FRAME_SKIPPING : str
CACHE_STANDARD_MODE_VP2_SW_NO_FALLBACK : list
EVALUATOR_NAME : str
KEY_ASYNCHRONOUS : str
KEY_RESOURCE_GUARD : str
KEY_FILL_ORDER : str

