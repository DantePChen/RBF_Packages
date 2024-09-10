from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import json
import os
import maya.cmds as cmds


from maya.analytics.Runner import Runner
from functools import partial
from maya.analytics.utilities import list_analytics
from maya.analytics.Logger import Logger
from maya.debug.TODO import TODO
from maya.common.ui import LayoutManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class AnalyticsWindow(object):
    """
    Helper class to manage all of the functions surrounding the creation and
    population of the analytics UI window.
    
    Create a window showing a visible interface to an analytics run. Shows
    a radio list with all available analytics, a selectable list of files
    and/or directories to include in the run, and controls for selecting
    all analytic options available through the Runner class.
    
        analytic_list       [Analytic]    List of analytics to be run, taken from the list of all analytics
        analytic_checkboxes {Name:Widget} Dictionary of analytic name mapped to the checkbox widget controlling them
        paths               [String Path] List of files or directories to be analyzed. (Empty means do the current scene.)
        anonymous_widget    String        Checkbox widget : make file and node names anonymous?
        return_json_widget  String        Checkbox widget : return the JSON result from the analytic run?
        descend_widget      String        Checkbox widget : walk subdirectories?
        details_widget      String        Checkbox widget : include full details in the output?
        force_widget        String        Checkbox widget : run analytic even if it is not out of date?
        log_file            File          Location for debugging output, None if not dumping it
        log_file_widget     String        File widget     : name of log file
        log_enabled_widget  String        Checkbox widget : log file enabled?
        options             {Key:Value}   Dictionary of options to be passed in to the analytics
        path_form           String        Form widget     : where paths are managed
        path_list           [Path]        List of paths to check for files to analyze
        path_widget_mgr     PathWidget    Owner of widgets where list of files and directories to analyze are entered
        progress_widget     String        Checkbox widget : show a progress bar while the analytics run?
        results_path        Directory     Root directory for analytics results.
        results_path_widget String        Text widget     : where the result path is entered
        skip                [String]      Regex list for files or directories to be skipped
        pattern_widget_mgr  PatternWidget Owner of widgets where filters for skipping are entered
        static_widget       String        Checkbox widget : only run static analytics, not scene-based ones?
        summary_widget      String        Checkbox widget : include the summary of all results in the output?
        __pattern_layout    String        Root widget of the pattern list section
        __path_layout       String        Root widget of the path list section
        __main_widget       String        Root widget of the entire window, for rebuilding
    """
    
    
    
    def __init__(self, analytic_list):
        """
        Does nothing initially. This is a lightweight handle on the analytics
        window over top of the regular UI.
        """
        ...
    def set_anonymous(self, anonymous):
        """
        Update the anonymous flag, along with the associated UI
        """
        ...
    def set_descend(self, descend):
        """
        Update the descend flag, along with the associated UI
        """
        ...
    def set_details(self, details):
        """
        Update the details flag, along with the associated UI
        """
        ...
    def set_force(self, force):
        """
        Update the force flag, along with the associated UI
        """
        ...
    def set_log_enabled(self, enabled):
        """
        Update the log_file value, along with the associated UI
        """
        ...
    def set_log_file(self, log_file_name):
        """
        Update the log_file value, along with the associated UI
        """
        ...
    def set_report_progress(self, report_progress):
        """
        Update the results_path value, along with the associated UI
        """
        ...
    def set_results_path(self, results_path):
        """
        Update the results_path value, along with the associated UI
        """
        ...
    def set_return_json(self, return_json):
        """
        Update the return_json flag, along with the associated UI
        """
        ...
    def set_static(self, static):
        """
        Update the static flag, along with the associated UI
        """
        ...
    def set_summary(self, summary):
        """
        Update the summary flag, along with the associated UI
        """
        ...
    def show_window(self):
        """
        If the window already exists then pop it up, otherwise create it
        and then pop it up. The window's contents are not sync'd with the
        outside world so it's possible to have references to files or
        directories that no longer exist in the window.
        
        Returns the name of the window.
        """
        ...
    @staticmethod
    def default_path():
        """
        Find a reasonable default path for results and scenes
        """
        ...
    @staticmethod
    def load_configuration(tool):
        """
        Select a file and populate the configuration with the settings in the file
        """
        ...
    @staticmethod
    def on_deselect_all_analytics(tool):
        """
        Callback to deselect all available analytics
        """
        ...
    @staticmethod
    def on_info(tool, analytic_name):
        """
        Callback when the info button is hit for the named analytic.
        Pop open a message box with the detailed help provided by the analytic.
        """
        ...
    @staticmethod
    def on_log_file_edit(tool):
        """
        Callback when the log file was manually edited.
        """
        ...
    @staticmethod
    def on_results_path_edit(tool):
        """
        Callback when the results path was manually edited. Maps an empty
        string to None since that is what the Runner expects
        """
        ...
    @staticmethod
    def on_select_all_analytics(tool):
        """
        Callback to select all available analytics
        """
        ...
    @staticmethod
    def on_select_anonymous(tool):
        """
        Callback to set anonymous option
        """
        ...
    @staticmethod
    def on_select_descend(tool):
        """
        Callback to set descend option
        """
        ...
    @staticmethod
    def on_select_details(tool):
        """
        Callback to set details option
        """
        ...
    @staticmethod
    def on_select_force(tool):
        """
        Callback to set force_run option
        """
        ...
    @staticmethod
    def on_select_log_enabled(tool):
        """
        Callback to set log file enabled option
        """
        ...
    @staticmethod
    def on_select_log_file(tool):
        """
        Callback when the logger path icon was clicked, which will find a
        new path to use for logging debugging information using the file browser.
        Nothing is changed if the dialog is canceled.
        """
        ...
    @staticmethod
    def on_select_report_progress(tool):
        """
        Callback to set report_progress option
        """
        ...
    @staticmethod
    def on_select_results_path(tool):
        """
        Callback when the results path icon was clicked, which will find a
        new path to use for results using the file browser.
        Nothing is changed if the dialog is canceled.
        """
        ...
    @staticmethod
    def on_select_return_json(tool):
        """
        Callback to set return_json option
        """
        ...
    @staticmethod
    def on_select_static_analytics(tool):
        """
        Callback to refresh analytic enabled state when switching static styles
        """
        ...
    @staticmethod
    def on_select_summary(tool):
        """
        Callback to set summary option
        """
        ...
    @staticmethod
    def run_analytics(tool, operation):
        """
        Run all of the selected analytics with the current options.
        
        tool: The Window object calling back to the function
        operation: RUN_ANALYTICS = Run the analytics specified by the configuration
                   LIST_ANALYTICS = List the analytics that would be run according to the configuration
                   SHOW_ANALYTIC_COMMAND = Print the Python script that would run the analytics
        """
        ...
    @staticmethod
    def save_configuration(tool):
        """
        Take all of the current settings and save them to a file for loading
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class PathWidget(object):
    """
    Class to manage creation and update of a variable length list path widget.
    
    The list is expanded and contracted dynamically by clicking on the +
    and trash icons rescpectively. Each path has a checkbox beside it
    so that it can be skipped without losing track of the path list.
    
        [X] Use Current Scene
    
        [X]   [  path ] [TRASH]
        [X]   [  path ] [TRASH]
        [ADD] [MVUP] [MVDN]
    """
    
    
    
    def __init__(self, parent_widget):
        """
        Initialize the widget information
        """
        ...
    def create_ui(self):
        """
        Create the generic text list UI and the buttons that control it
        """
        ...
    def paths_lists(self):
        """
        Get the pair of lists of all non-empty paths, (disabled, enabled)
        """
        ...
    def paths_to_use(self):
        """
        Get the list of all non-empty paths that are currently enabled, unless
        use_current_scene is enabled, in which case return the empty list.
        """
        ...
    def set_use_current(self, use_current_scene):
        """
        Update the use_current_scene flag, along with the associated UI
        """
        ...
    @staticmethod
    def on_add(tool, mode):
        """
        Callback when one of the add buttons was clicked. Lets the user select
        either a file or a directory, depending on mode, to add to the list.
        Nothing is added if the dialog is canceled.
        """
        ...
    @staticmethod
    def on_delete(tool, index):
        """
        Callback when the "index"th path was deleted. Removes the path from the list
        and updates the UI to reflect to the new list.
        """
        ...
    @staticmethod
    def on_enable(tool, index):
        """
        Callback when the checkbox enabling the "index"th path was clicked.
        Remembers the new enabled state.
        """
        ...
    @staticmethod
    def on_path_edit(tool, index):
        """
        Callback when the path was manually edited.
        """
        ...
    @staticmethod
    def on_use_current_scene(tool):
        """
        Callback to selectively disable the file selector and buttons if only using the current scene
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class PatternWidget(object):
    """
    Class to manage creation and update of a variable length pattern list widget.
    
    The list is expanded and contracted dynamically by clicking on the +
    and trash icons rescpectively. Each pattern has a checkbox beside it
    so that it can be skipped without losing track of what the pattern is.
    
        [X] [  pattern string   ] TRASH
        [X] [  pattern string   ] TRASH
        ADD
    """
    
    
    
    def __init__(self, parent_widget):
        """
        Initialize the widget information
        """
        ...
    def create_ui(self):
        """
        Create the generic text list UI and the buttons that control it
        """
        ...
    def patterns_lists(self):
        """
        Get the pair of lists of all non-empty patterns, (disabled, enabled)
        """
        ...
    def patterns_to_use(self):
        """
        Get the list of all non-empty patterns that are currently enabled
        """
        ...
    @staticmethod
    def on_add(tool):
        """
        Callback when the add button was clicked. Appends an empty pattern to
        the end of the list and updates the UI.
        """
        ...
    @staticmethod
    def on_delete(tool, index):
        """
        Callback when the "index"th pattern was deleted. Removes the pattern from the list
        and updates the UI to reflect to the new list.
        """
        ...
    @staticmethod
    def on_enable(tool, index):
        """
        Callback when the checkbox enabling the "index"th pattern was clicked.
        Remembers the new enabled state.
        """
        ...
    @staticmethod
    def on_pattern(tool, index):
        """
        Callback when the pattern text for the "index"th pattern was changed..
        Remembers the new pattern value.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def callback_tool(self, function):
    """
    This method returns a callback method that can be used by the UI
    elements.
    
    It wraps the "easier to define" callbacks that only takes the tool as
    an element into the callbacks that UI element expects.
    """
    ...
def analytics_ui():
    """
    Shallow interface to the analytics Window object, which only has
    the single external function of showing the Window.
    
    Returns the name of the analytics window, in case you want to
    delete it.
    """
    ...
def debug(msg):
    """
    Print the message if debugging is enabled
    """
    ...
def callback_wrapper(*args, **kwargs):
    """
    This method is a wrapper in the form expected by UI elements.
    
    Its signature allows it to be flexible with regards to what UI elements
    expects.  Then it simply calls the given functor.
    """
    ...


ANALYTICS_RUNNER : Runner
SHOW_ANALYTIC_COMMAND : int
ANALYTICS_RESULTS : NoneType
FILE_MODE_DIR : int
DEBUGGING : bool
FILE_MODE_CREATE : int
FILE_MODE_FILE : int
BUTTON_WIDTH : int
HSPC : int
TEXT_WIDTH : int
FILE_MODE_FILES : int
NO_SELECTION : int

