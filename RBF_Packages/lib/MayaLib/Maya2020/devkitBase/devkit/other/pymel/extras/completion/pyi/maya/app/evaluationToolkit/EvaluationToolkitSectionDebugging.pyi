from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import maya
import os
import maya.cmds as cmds


from maya.debug.em_debug_utilities import require_evaluation_graph
from maya.common.ui import showMessageBox
from maya.debug.emModeManager import emModeManager
from maya.common.ui import callback_tool
from maya.debug.em_debug_utilities import open_file
from maya.app.evaluationToolkit.evaluation_toolkit_utilities import EvaluationToolkitSubsection
from maya.debug.em_debug_utilities import select_inverse_visible_dag_objects
from maya.debug.em_debug_utilities import dbg_nodes
from maya.debug.em_debug_utilities import dbg_scheduling_types
from maya.debug.em_debug_utilities import get_default_directory
from maya.debug.em_debug_utilities import get_minimal_scene_objects_from
from maya.debug.TODO import TODO
from maya.common.ui import LayoutManager
from maya.debug.em_debug_utilities import dbg_scheduling_graph_to_dot
from maya.app.evaluationToolkit.evaluation_toolkit_utilities import EvaluationToolkitSection
from maya.app.evaluationToolkit.evaluation_toolkit_utilities import section_layout
from functools import partial
from maya.debug.em_debug_utilities import dbg_scheduling_graph
from maya.debug.em_debug_utilities import dbg_graph_to_dot
from maya.debug.em_debug_utilities import dbg_graph


if False:
    from typing import Dict, List, Tuple, Union, Optional

class SubsectionTools(EvaluationToolkitSubsection):
    """
    Class managing the "tools" subsection of the debugging section.
    It will contain trace objects and buttons to launch windows.
    
    :member checkbox_traces: Checkbox widgets for the trace object controls
    :member textfield_traces: Textfield widgets for the trace object controls
    """
    
    
    
    def __init__(self):
        """
        Set up the widgets used in the subsection
        """
        ...
    def update_ui(self):
        """
        Updates the trace object UI to match the trace object states
        """
        ...
    @staticmethod
    def callback_launch_analytics_window(tool):
        """
        Callback invoked from the button that launches the analytics window
        """
        ...
    @staticmethod
    def callback_launch_profiler(tool):
        """
        Callback invoked from the button that launches the profiler
        """
        ...
    @staticmethod
    def callback_launch_scene_lint_window(tool):
        """
        Callback invoked from the button that launches the scene lint window
        """
        ...
    @staticmethod
    def callback_show_trace_help(tool, trace):
        """
        Callback invoked when asking for help on a particular trace object
        """
        ...
    @staticmethod
    def callback_trace_enable(tool, trace):
        """
        Callback invoked whenever the enabled state of the trace object changes
        """
        ...
    @staticmethod
    def callback_trace_output(tool, trace):
        """
        Callback invoked whenever the location of the trace output changes
        """
        ...


class SubsectionDynamicAttributes(EvaluationToolkitSubsection):
    """
    Class managing the "dynamic attribute" subsection of the debugging section
    """
    
    
    
    def __init__(self):
        """
        Set up the widgets used in the subsection
        """
        ...
    @staticmethod
    def callback_print_extra_connections(*args, **kwargs):
        """
        Callback invoked when asking to print extra dynamic attribute connections
        """
        ...
    @staticmethod
    def callback_remove_extra_connections(*args, **kwargs):
        """
        Callback invoked when asking to remove extra dynamic attribute connections
        """
        ...
    @staticmethod
    def process_connections(disconnect):
        """
        Looks for dynamic attribute connections in and out of DAG nodes as they
        are a potential source of ineffeciency.
        
        Prints the details and action to be performed on matching connections.
        Last line to print is the count of each action type.
        
        :param disconnect: If true then remove the connections that were found
        """
        ...


class OutputHelper(object):
    """
    Helper class to direct output to the proper location from the debugging section.
    This avoids a bunch of code duplication for selecting and using separate output
    locations.
    """
    
    
    
    def __init__(self, tool):
        """
        Set up the output location from the tool data
        """
        ...
    def close(self):
        """
        Close the file, if one was selected
        """
        ...
    def write(self, msg):
        """
        Print "msg" to the currently selected output location
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class SubsectionGraphInspection(EvaluationToolkitSubsection):
    """
    Class managing the "evaluation graph inspection" subsection of the debugging section
    
    There are a lot of magic numbers in here, normally frowned on but deemed okay in
    this situation since they create the alignment and offsets needed to make the UI
    look good, and any names would be mostly meaningless.
    """
    
    
    
    def __init__(self, get_graphviz_manager):
        """
        Set up the widgets used in the subsection
        :param get_graphviz_manager: Function to retrieve an up-to-date GraphVizManager
        """
        ...
    def get_output_file_info(self):
        """
        :return: 2-tuple containing
        1. Name of the .dot file to generate
        2. Name of the .pdf file to generate (None if no PDF file requested)
        """
        ...
    def get_selection_info(self):
        """
        :return: 2-tuple containing:
        1. A boolean that's True if the selection list should be respected
        2. A depth value indicating how many steps away from the current selection the operation
           should look.
        """
        ...
    def graphical_frame_layout(self):
        """
        Add the frame that contains the graph visual inspection features
        """
        ...
    def main_frame_layout(self):
        """
        Add the frame that contains the shared graph inspection features
        """
        ...
    def text_frame_layout(self):
        """
        Add the frame that contains the graph text inspection features
        """
        ...
    @staticmethod
    def callback_choose_graphical_output_location(tool):
        """
        Callback invoked when the graphical inspection file browser button is selected
        """
        ...
    @staticmethod
    def callback_choose_text_output(tool):
        """
        Callback invoked when the text inspection file browser button is selected
        """
        ...
    @staticmethod
    def callback_print_graph(*args, **kwargs):
        """
        Callback invoked when the text style graph printing button is pushed
        """
        ...
    @staticmethod
    def callback_print_nodes(*args, **kwargs):
        """
        Callback invoked when the text style node printing button is pushed
        """
        ...
    @staticmethod
    def callback_print_scheduling_graph(*args, **kwargs):
        """
        Callback invoked when the text style scheduling printing button is pushed
        """
        ...
    @staticmethod
    def callback_print_scheduling_types(*args, **kwargs):
        """
        Callback invoked when the text style scheduling type printing button is pushed
        """
        ...
    @staticmethod
    def callback_update_graphical_output_format(tool):
        """
        Callback invoked when graphical inspection output format is changed
        """
        ...
    @staticmethod
    def callback_update_scope(tool):
        """
        Callback invoked when node inspection scope type is changed
        """
        ...
    @staticmethod
    def callback_update_text_output_format(tool):
        """
        Callback invoked when text inspection output format is changed
        """
        ...
    @staticmethod
    def callback_update_text_output_location(tool):
        """
        Callback invoked when text inspection output location is changed
        """
        ...
    @staticmethod
    def callback_visualize_evaluation_graph(*args, **kwargs):
        """
        Callback invoked when the visual style evaluation graph printing button is pushed
        """
        ...
    @staticmethod
    def callback_visualize_scheduling_graph(*args, **kwargs):
        """
        Callback invoked when the visual style scheduling graph printing button is pushed
        """
        ...


class SubsectionSceneSimplification(EvaluationToolkitSubsection):
    """
    Class managing the "scene simplification" subsection of the debugging section
    """
    
    
    
    def __init__(self):
        """
        Set up the widgets used in the subsection
        """
        ...
    @staticmethod
    def callback_remove_all_but_minimal_scene(tool):
        """
        Callback invoked when the "remove all but minimal scene" button is pressed
        """
        ...
    @staticmethod
    def callback_select_minimal_scene(*args, **kwargs):
        """
        Callback invoked when the "select minimal scene" button is pressed
        """
        ...


class EvaluationToolkitSectionDebugging(EvaluationToolkitSection):
    """
    Class providing support for UI and functionality of the evaluation toolkit debugging section.
    """
    
    
    
    def __init__(self, title, start_closed, get_graphviz_manager):
        """
        Set up the framework for the debugging tools
        :param title: Name of the main debugging section
        :param start_closed: True means the section should be initially closed when the UI window is created
        :param get_graphviz_manager: Callback to retrieve a GraphVizManager for use in DOT/PDF file generation
        """
        ...
    def update_ui(self):
        """
        Update the UI values for all of the subsections
        """
        ...




BUTTON_WIDTH : int
DEBUGGING_TRACES : list
LABEL_SHOW : unicode
WIDGET_HEIGHT : int
LABEL_REMOVE : unicode

