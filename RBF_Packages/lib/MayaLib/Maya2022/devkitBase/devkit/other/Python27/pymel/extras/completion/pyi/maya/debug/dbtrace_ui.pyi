from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import os
import re
import maya.cmds as cmds


"""
The window features are self-contained. Just create it using:
    from maya.debug.dbtrace_ui import dbtrace_ui
    dbtrace_window = dbtrace_ui()

Inside the window you can toggle trace objects on and off, change the location
of their output, and view the current output they have dumped (if any).
"""


from maya.common.ui import scrollableMessageBox
from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

def get_dbtrace_output(output_file):
    """
    Return the contents of the named file
    :param output_file:  Name of the file whose contents are being returned
    :return:             Array of strings, one per line in the file
    """
    ...
def dbtrace_filter_off_change(new_state):
    """
    UI state change callback.
    
    Callback when a checkbox is ticked to alter the enabled state of a
    filter to display the trace objects currently off.
    
    new_state: New value of the 'show disabled' filter
    """
    ...
def dbtrace_output_change(trace_name, trace_level, output_field):
    """
    Trace object output location change callback.
    
    trace_name:   Name of trace object
    trace_level:  Level of trace object
    output_field: UI element at which the output name can be found
    """
    ...
def dbtrace_enable_change(checkbox_widget, trace_name, trace_level, new_state):
    """
    Trace object enabled state change callback.
    
    checkbox_widget: Name of the checkbox used to change state, in case
                     reversion is needed.
    trace_name:      Name of trace object
    trace_level:     Level of trace object
    new_state:       New enabled state for the trace object
    """
    ...
def __debug(msg):
    """
    Print the message if debugging is enabled
    """
    ...
def dbtrace_filter_on_change(new_state):
    """
    UI state change callback.
    
    Callback when a checkbox is ticked to alter the enabled state of a
    filter to display the trace objects currently on.
    
    new_state: New value of the 'show enabled' filter
    """
    ...
def dbtrace_ui():
    """
    Create a simple window showing the current status of the dbtrace objects
    and providing a callback to change both the enabled state and the output
    location.
    """
    ...
def dbtrace_filter_change(filter_name_field):
    """
    UI state change callback.
    
    filter_name_field: UI textField element holding the filter text
    """
    ...
def __trace_ui_name(trace_name, trace_level):
    """
    Nice name for a trace object.
    """
    ...
def dbtrace_show_output(trace_object, output_file):
    """
    Button command to open up a window displaying the current trace output.
    
    :param trace_object: Name of the trace object whose output is being shown
    :param output_file:  Name of the file whose contents are being shown
    """
    ...


DBTRACE_FILTERS : str
DBTRACE_SCROLL : str
DBTRACE_SCRIPT_JOB : NoneType
DEBUGGING : bool
DBTRACE_FILTER_TEXT : str
DBTRACE_FRAME : str
DBTRACE_CONTENT : str
DBTRACE_WINDOW : str
DBTRACE_SHOW_ENABLED : str
DBTRACE_SHOW_DISABLED : str
RE_TRACE_NAME : re.SRE_Pattern

