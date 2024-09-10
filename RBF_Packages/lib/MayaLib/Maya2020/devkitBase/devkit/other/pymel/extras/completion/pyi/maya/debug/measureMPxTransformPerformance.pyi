from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import random
import maya.cmds as cmds


"""
Test the comparitive performance between regular Maya transforms and the
leanTransformTest node to see what overhead the API brings.
"""


from maya.debug.emModeManager import emModeManager
from maya.debug.PlaybackManager import PlaybackManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

def create_nodes(node_count, node_type):
    """
    Create a given number of nodes of the given type and return the
    list of nodes created.
    """
    ...
def measureMPxTransformPerformance():
    """
    Run two performance tests with 1000 transforms keyed randomly over 1000 frames
    for both the native Ttransform and the API leanTransformTest. Report the timing
    for playback of the two, and dump profile files for both for manual inspection.
    """
    ...
def animate(node_list, keyframe_count):
    """
    Animate the TRS attributes of every node in the list with random
    values for each frame from 1 to "keyframe_count"
    """
    ...


KEY_COUNT : int
NODE_NAME : str
PLUGIN_PROFILE : str
NATIVE_PROFILE : str

