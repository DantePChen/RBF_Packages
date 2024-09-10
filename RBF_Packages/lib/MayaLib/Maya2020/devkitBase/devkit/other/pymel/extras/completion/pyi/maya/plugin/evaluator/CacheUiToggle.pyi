from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.debug.DebugTrace
import maya.mel as mel
import maya
import maya.cmds as cmds


from maya.plugin.evaluator.cache_preferences import cache_ui_enabled
from maya.common.ui import callback_tool
from maya.plugin.evaluator.cache_preferences import CachePreferenceEnabled
from maya.plugin.evaluator.cache_preferences import CachePreferenceMode
from maya.plugin.evaluator.CacheEvaluatorManager import CacheEvaluatorManager
from maya.common.utils import Singleton
from maya.plugin.evaluator.CacheScriptJobHelper import CacheScriptJobHelper
from maya.plugin.evaluator.CacheUiBase import CacheUiBase
from maya.common.ui import highlight_colour
from functools import partial
from maya.plugin.evaluator.cache_preferences import cache_preferences_initialize
from maya.plugin.evaluator.cache_preferences import CachePreferences


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CacheUiToggle(CacheUiBase):
    """
    Class managing the cache preferences toggle button within the playback range bar (playbackRange.mel)
    :member widgets:          Dictionary of widgets used by the UI (KEY=widget_id, VALUE=ui control name)
    :member mode_change_job:  scriptJob ID for the customEvaluatorChanged event
    :member state:            Current display state (one of the STATE_XX values)
    :member setting_mode:     True if currently in the middle of setting the mode from the popup (to avoid recursion)
    """
    
    
    
    def __init__(self):
        """
        Nothing to do here; the button is created on demand and this is a persistent singleton
        """
        ...
    def create_button(self):
        """
        Create the button and popup menu implementing the subset of cache preferences
        accessible from the playback range widget, as well as links to the fully detailed
        preferences in the preference window.
        :return: UI Widget ID of the button
        """
        ...
    def plugin_state_change(self, new_state):
        """
        Called when the plug-in state changed to loaded or unloaded. Updates the UI appearance to
        reflect the new state.
        :param new_state: True if the plug-in was just loaded, False if just unloaded
        """
        ...
    def rebuild_popup_menu(self):
        """
        Create the popup menu that appears over the icon.
        """
        ...
    @staticmethod
    def callback_flush_cache(value):
        """
        Callback invoked when the "Flush cache" menu item is selected
        """
        ...
    @staticmethod
    def callback_open_colour_preferences(value):
        """
        Callback invoked when the menu item to open the colour preferences is selected
        """
        ...
    @staticmethod
    def callback_toggle_enabled(tool):
        """
        Callback to toggle the enabled state of the Cached Playback icon
        :param tool: CacheUiToggle object to be updated
        """
        ...
    @staticmethod
    def callback_ui_deleted(tool):
        """
        Callback when the UI is deleted - cleans up the class variables.
        :param tool: CacheUiToggle object attached to the UI that was deleted
        """
        ...
    @staticmethod
    def callback_update_ui(tool):
        """
        Callback to update the states of the Cached Playback icon and the popup menu after a change
        :param tool: CacheUiToggle object to be updated
        """
        ...
    @staticmethod
    def enabled(state):
        """
        Returns the enabled value associated with the given state
        """
        ...
    @staticmethod
    def icon(state):
        """
        Returns the icon associated with the given state
        """
        ...
    @staticmethod
    def set_mode(tool, mode):
        """
        Callback when one of the caching mode radio buttons is clicked.
        :param tool: Radio menu item widget
        :param mode: New caching mode to be set (as enum string)
        """
        ...
    @staticmethod
    def tooltip(state):
        """
        Returns the tooltip associated with the given state
        """
        ...
    ENABLED_IDX : int
    
    ICON_IDX : int
    
    ICON_ON : str
    
    ICON_WARN : str
    
    OKAY_TIP : unicode
    
    STATE_CACHING_OFF : int
    
    STATE_DISABLED : int
    
    STATE_EM_OFF : int
    
    STATE_INFO : dict
    
    STATE_LOW_MEMORY : int
    
    STATE_NO_MEMORY : int
    
    STATE_OKAY : int
    
    STATE_SAFE_MODE : int
    
    TOOLTIP_IDX : int
    
    WID_BUTTON : int
    
    WID_FLUSH_CACHE : int
    
    WID_MODE : int
    
    WID_POPUP : int
    
    WID_PREF_CACHING : int
    
    WID_PREF_COLOURS : int
    
    WID_UNSUPPORTED_WEB : int




def callback_open_caching_preferences(value):
    """
    Callback invoked when the menu item to open the caching preferences is selected
    """
    ...
def local_dbg(msg):
    """
    Access to the local debugging function, which just prepends the class name to debugging information
    """
    ...
def in_em_mode():
    """
    :return: True if the current evaluation is one of the EM modes (i.e. not DG)
    """
    ...
def cache_preferences_menu_item():
    """
    Create a menu item that will open the preferences window to the Cached Playback section
    :return: Name of the menuItem widget that was created
    """
    ...
def use_safe_mode():
    """
    For UI purposes safe mode is not considered on when the evaluator is off
    :return: True if the UI should use the safe mode configuration
    """
    ...


CP_DBG : maya.debug.DebugTrace.DebugTrace
EVENT_LIMIT_CHANGE : str
EVENT_SAFE_MODE : str
EVENT_EM_DISABLED : str

