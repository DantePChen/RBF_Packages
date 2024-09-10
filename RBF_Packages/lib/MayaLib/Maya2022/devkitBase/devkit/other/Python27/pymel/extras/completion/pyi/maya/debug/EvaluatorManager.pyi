from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import EvaluatorManager
import json
import maya
import maya.cmds as cmds


"""
Derived classes will handle evaluator-specific information, this
one just handles the information that is common to all evaluators.

The object is set up to use the Python "with" syntax as follows:

    class MyEvaluatorManager(EvaluatorManager):
        ...

    from maya.debug.MyEvaluatorManager import MyEvaluatorManager
    with MyEvaluatorManager() as mgr:
        mgr.setMode( someMode )

That will ensure the original states are all restored.

If you need different control you can manually complete the sequence:

    mgr = MyEvaluatorManager()
    mgr.save_state()
    mgr.setMode( someMode )
    mgr.restore_state()
"""


from traceback import extract_tb
from maya.app.prefs.OptionVar import OptionVar
from maya.debug.DebugTrace import DebugTrace
from traceback import format_list


if False:
    from typing import Dict, List, Tuple, Union, Optional

class EvaluatorManager(DebugTrace):
    """
    Class for managing the shared evaluator state in a 'with' format. Remembers
    and restores the mode and parameters, including the load state of any plug-in
    associated with the manager.
    """
    
    
    
    def __enter__(self):
        """
        Defining both __enter__ and __init__ so that either one can be used
        """
        ...
    def __exit__(self, event_type, value, traceback):
        """
        Ensure the state is restored if this object goes out of scope
        """
        ...
    def __init__(self, name, plugin):
        """
        Defining both __enter__ and __init__ so that either one can be used
        
        Members:
            state          : Values of the various states when save_state was called
            evaluator_name : Name of the evaluator this object manages
            plugin_name    : Name of the plug-in in which the evaluator is found (None if native)
            in_debug_mode  : True if debugging messages are to be printed
        """
        ...
    def __str__(self) -> str:
        """
        Display mechanism to show evaluator information in JSON format
        """
        ...
    def as_json(self):
        """
        Display mechanism to retrieve evaluator information in a format conducive to JSON formatting
        """
        ...
    def check_evaluator(self):
        """
        Check to see if the evaluator is known by the system
        :raise OptionVar.StateError: The evaluator is not known
        """
        ...
    def claimed_nodes(self, flatten):
        """
        :param flatten: Boolean determining how to return the claimed nodes
        :return: If flatten is True then returns a single list of all nodes
                 claimed by this evaluator.
                 If flatten is False then return a list of clusters created
                 by this evaluator, where each cluster is in turn a list of
                 nodes contained within that cluster.
        """
        ...
    def restore_state(self):
        """
        Restore the evaluator to its original mode prior to creation of this
        object. Using the "with" syntax this will be called automatically.
        
        You only need to call explicitly when you instantiate the mode manager
        as an object.
        """
        ...
    def save_state(self):
        """
        Remember the current state of all evaluator parameters so that they
        can be restored on exit.
        
        self.state must be initialized to an object of type EvaluatorState
            (or derived class) before calling here
        """
        ...
    def set_state(self, new_state):
        """
        Define the evaluator state parameters.
        :param new_state: State information in the format provided by as_json
                          Only key values specified will change. Others retain
                          their current values (*not* default values).
        """
        ...
    @property
    def consolidation(self):
        """
        Return the cluster consolidation method to use (none or subgraph)
        """
        ...
    @consolidation.setter
    def consolidation(self, value): ...
    @property
    def enabled(self):
        """
        Return the evaluator's enabled state
        """
        ...
    @enabled.setter
    def enabled(self, value): ...
    @property
    def node_types(self):
        """
        Return the evaluator's enabled node type list
        """
        ...
    @node_types.setter
    def node_types(self, value): ...
    @property
    def plugin_loaded(self):
        """
        Return whether the evaluator's plug-in was loaded
        """
        ...
    @plugin_loaded.setter
    def plugin_loaded(self, value): ...
    @property
    def priority(self):
        """
        Return the evaluator's evaluation priority
        """
        ...
    @priority.setter
    def priority(self, value): ...
    EvaluatorState : Type[maya.debug.EvaluatorManager.EvaluatorState]




def as_list(thing):
    """
    Simple utility to ensure the thing is a list, return None as an empty list
    """
    ...


KEY_CONSOLIDATION : str
KEY_PRIORITY : str
KEY_PLUGIN : str
KEY_NODE_TYPES : str
KEY_LOADED : str
KEY_ENABLED : str
KEY_NAME : str

