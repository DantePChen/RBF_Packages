from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.debug.DebugTrace
import tempfile
import maya
import json
import os
import maya.cmds as cmds


from maya.debug.emModeManager import emModeManager
from maya.plugin.evaluator.cache_preferences import cache_preferences_initialize
from maya.plugin.evaluator.cache_preferences import cache_ui_enabled
from maya.common.ui import scrollableMessageBox
from maya.common.ui import callback_tool
from maya.plugin.evaluator.cache_preferences import CachePreferenceEnabled
from maya.app.evaluationToolkit.evaluation_toolkit_utilities import section_layout
from maya.analytics.Runner import Runner
from maya.plugin.evaluator.CacheEvaluatorManager import CacheEvaluatorManager
from maya.debug.cacheCorrectnessTest import cacheCorrectnessTest
from maya.common.utils import Singleton
from functools import partial
from maya.plugin.evaluator.CacheUiBase import CacheUiBase
from maya.common.ui import LayoutManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CacheUiToolkit(CacheUiBase):
    """
    Class managing the caching UI elements provided by the evaluation toolkit.
    
    :member preferences: CachePreferences object that handles updating the actual preference values
    :member ui_key: Unique identifier for this UI object. Used for smart deletion from CONTROLS dictionary
    :member widgets: Dictionary of widgets used by the UI (KEY=widget_id, VALUE=ui control name)
    :member mode_change_job: scriptJob ID for the customEvaluatorChanged callback
    """
    
    
    
    def __init__(self):
        """
        Nothing to do here; the elements are created on demand and this is a persistent singleton
        """
        ...
    def clear_results(self):
        """
        Clear out all of the current results
        """
        ...
    def create_validation_ui(self):
        """
        Create the layout section implementing the subset of cache validation features
        """
        ...
    def plugin_state_change(self, new_state):
        """
        Called when the plug-in state changed to loaded or unloaded. Updates the UI appearance to
        reflect the new state.
        :param new_state: True if the plug-in was just loaded, False if just unloaded
        """
        ...
    def reset_widgets(self):
        """
        Reset the widget names to uninitialized values
        """
        ...
    @staticmethod
    def callback_choose_context_monitor_output(tool):
        """
        Callback when the output folder browser button is hit. If not canceled
        then the context monitor is disabled, then re-enabled with the new
        file selected as output.
        """
        ...
    @staticmethod
    def callback_get_context_monitor_details(tool):
        """
        Callback function to get the results from the context monitor for the scrollableMessageBox
        """
        ...
    @staticmethod
    def callback_get_correctness_test_details(tool, test_type):
        """
        Callback function for the scrollableMessageBox to get the detailed contents of the test output
        """
        ...
    @staticmethod
    def callback_run_cache_correctness_tests(tool, test_type):
        """
        Invoked from one of the Cache Correctness buttons. Selects the test to be run,
        runs it, then adds the results to the widgets.
        """
        ...
    @staticmethod
    def callback_run_cache_performance_test(tool):
        """
        Invoked from the Cache Performance button. Walks through all of the cache performance
        tests and runs them, adding the results to the widgets.
        """
        ...
    @staticmethod
    def callback_show_cache_correctness_test_details(tool, test_type):
        """
        Invoked when the user clicks the info button to show cache correctness test details
        """
        ...
    @staticmethod
    def callback_show_context_monitor_results(tool):
        """
        Callback when the context monitor Details button is pressed
        """
        ...
    @staticmethod
    def callback_ui_deleted(tool):
        """
        Callback when the UI is deleted - cleans up the class variables.
        :param tool: CacheUiToolkit object attached to the UI that was deleted
        """
        ...
    @staticmethod
    def callback_update_context_monitor_output(tool):
        """
        Callback when the output folder for the context monitor changes.
        Since the context monitor is actively using the output it has to
        be disabled first, if necessary, then re-enabled with the new output.
        If not enabled then nothing is done yet.
        """
        ...
    @staticmethod
    def callback_update_context_monitor_state(tool):
        """
        Update the context monitor state based on the current settings
        """
        ...
    @staticmethod
    def callback_update_enabled(tool):
        """
        Callback to refresh the enabled state of the cache tests to match the cache evaluator state.
        :param tool: CacheUiToolkit object to be updated
        """
        ...
    @staticmethod
    def callback_update_visibility(tool):
        """
        Invoked when some external agent could affect the visibility of the widgets.
        e.g. the opening of a frame layout, which automatically makes all children visible
        """
        ...




def get_default_directory():
    """
    Get a reasonable default directory for temporary output.
    """
    ...
def local_dbg(msg):
    """
    Access to the local debugging function, which just prepends the class name to debugging information
    """
    ...
def run_cache_test(test_type, verbose):
    """
    Run the named cache correctness test.
    The test information is drawn from the test info in CACHE_CORRECTNESS_TESTS.
    
    :param test_type: Test type - from the keys of CACHE_CORRECTNESS_TESTS
    :param verbose: True means add more detail to the output
    :return: a 2-tuple with the number of errors found and the JSON error details
    """
    ...
def run_cache_performance_test():
    """
    Run the cache performance test on the current scene. Return a
    dictionary of NAME : (SPEED, MEMORY)
        NAME   = Name of the caching configuration
        SPEED  = fps of the playback
        MEMORY = Mb reported as being in use by the cache evaluator
    output will be shown in the script editor window.
    """
    ...


BUTTON_WIDTH : int
FILE_TEXT_FIELD_WIDTH : int
KEY_CACHE_MODE : str
CP_DBG : maya.debug.DebugTrace.DebugTrace
TEST_NOT_RUN : unicode
CACHE_STANDARD_MODE_VP2_SW : list
CACHE_STANDARD_MODE_VP2_HW : list
CACHE_STANDARD_MODE_EVAL_SHAPES : list
USE_VERBOSE_CORRECTNESS_TESTS : bool
CACHE_STANDARD_MODE_EVAL : list
CORRECTNESS_INVALIDATE : int
CACHE_PERFORMANCE_MODES : list
COLUMN_SPACING : int
ROW_SPACING : int
CACHE_CORRECTNESS_DATA : list
CACHE_CORRECTNESS_TESTS : dict

