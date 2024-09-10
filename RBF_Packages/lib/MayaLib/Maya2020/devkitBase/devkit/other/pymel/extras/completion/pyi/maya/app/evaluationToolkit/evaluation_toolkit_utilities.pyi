from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel


if False:
    from typing import Dict, List, Tuple, Union, Optional

class EvaluationToolkitSection(object):
    """
    Base class with common methods for sections in the toolkit.
    """
    
    
    
    def update_ui(self):
        """
        Nothing to do by default for updating UI
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class EvaluationToolkitSubsection(object):
    """
    Base class with common methods for subsections underneath sections in the toolkit.
    """
    
    
    
    def update_ui(self):
        """
        Nothing to do by default for updating UI
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def set_gpu_override_active(state):
    """
    This method activates or deactivates the OpenCL evaluator.
    
    TODO: This could be better implemented by moving the gpuOverride optionVar
          into one of the OptionVar class objects for automatic management.
    
    :param state: New activation state of the evaluator.
    """
    ...
def section_layout(start_closed):
    """
    :param start_closed: True means the frame should be closed when the UI is initially created
    :return: A dictionary consisting of the parameters used by the top level frames
    """
    ...


BUTTON_WIDTH : int
FILE_TEXT_FIELD_WIDTH : int
COLUMN_SPACING : int
ROW_SPACING : int

