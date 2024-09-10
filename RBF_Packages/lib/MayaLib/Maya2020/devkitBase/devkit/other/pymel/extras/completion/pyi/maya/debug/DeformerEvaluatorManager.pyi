from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import DeformerEvaluatorManager
import maya.cmds as cmds


"""
Helper class that maintains the deformer evaluator mode information.
Manages the deformer evaluator-specific data, the common data is
managed by the base class.

The object is set up to use the Python "with" syntax as follows:

    from maya.debug.DeformerEvaluatorManager import DeformerEvaluatorManager
    with DeformerEvaluatorManager() as mgr:
        mgr.setMode( someMode )

That will ensure the original states are all restored. There's no other
reliable way to do it in Python. If you need different syntax you can
manually call the method to complete the sequence:

    mgr = DeformerEvaluatorManager()
    mgr.save_state()
    mgr.setMode( someMode )
    mgr.restore_state()
"""


from maya.debug.EvaluatorManager import EvaluatorManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DeformerEvaluatorManager(EvaluatorManager):
    """
    Class for managing the deformer evaluator state in a 'with' format. Remembers
    and restores the deformer parameters.
    """
    
    
    
    def __exit__(self, type, value, traceback):
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
        Remember the current state of all related parameters so that they
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
    @property
    def override_option(self):
        """
        Return the optionVar for the GPU override value
        """
        ...
    @override_option.setter
    def override_option(self, value): ...
    @property
    def verify_buffers(self):
        """
        Return the evaluator's verify_buffers state
        """
        ...
    @verify_buffers.setter
    def verify_buffers(self, value): ...
    DeformerEvaluatorState : Type[maya.debug.DeformerEvaluatorManager.DeformerEvaluatorState]




PLUGIN_NAME : str
KEY_VERIFY_BUFFERS : str
OPTIONVAR_GPU_OVERRIDE : str
EVALUATOR_NAME : str

