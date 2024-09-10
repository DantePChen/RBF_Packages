from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import json
import maya
import maya.cmds as cmds


from maya.common.ui import LayoutManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class EventDescriptionWindow(object):
    """
    This is the main UI class for the profiler event description window.
    
    It handles creation of the UI and provides various callbacks to handle
    user interactions.
    """
    
    
    
    def __init__(self, window_name="'profilerEventDescriptionWindowId'"):
        """
        This does not create the UI.  UI creation is deferred until create() is called
        :param window_name: UI name for the window
        """
        ...
    def create(self):
        """
        This method completely builds the UI, then shows the finished window
        """
        ...
    def populate(self):
        """
        This method populates the current window with the framework necessary to
        store the event and category description information.
        """
        ...
    def selection_changed(self):
        """
        Callback for when the list of selected events in the profiler view changed
        """
        ...
    def window_closed(self):
        """
        Callback for when the profiler event description window closes
        """
        ...
    @staticmethod
    def populate_categories(category_info):
        """
        Populate the section of the window containing the category information
        :param category_info: List of (category_name, category_description) pairs
        """
        ...
    @staticmethod
    def populate_event_types(selected_event_types):
        """
        Populate the section of the window containing the event type information
        :param selected_event_types: List of dictionaries with event type description information
            KEY_TYPE        = Name of event type (Not present for anonymous events)
            KEY_DESCRIPTION = Description of event type (Not present for anonymous events)
            KEY_COLOUR      = List of 3 floats representing R, G, B of the event type
            KEY_CATEGORY    = Name of the category to which the event type belongs
            KEY_COUNT       = Number of events of this type found
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def create_window():
    """
    This method is the entry point of the Profiler Event Description window.
    
    It creates the window and brings it up.
    """
    ...


KEY_COLOUR : str
COL_SPACING : int
RC_LAYOUT_5_COLUMN : dict
KEY_CATEGORY : str
PROFILER_EVENT_DESCRIPTION_WINDOW_CONTROLLER : NoneType
KEY_COUNT : str
FRAME_MARGIN_HEIGHT : int
RC_LAYOUT_2_COLUMN : dict
KEY_TYPE : str
KEY_MAIN : str
KEY_DESCRIPTION : str
FRAME_LAYOUT : dict
FRAME_MARGIN_WIDTH : int
TABLE_HEADER : dict

