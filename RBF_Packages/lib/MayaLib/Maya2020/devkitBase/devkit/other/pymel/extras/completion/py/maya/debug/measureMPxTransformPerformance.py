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
    pass
def measureMPxTransformPerformance():
    """
    Run two performance tests with 1000 transforms keyed randomly over 1000 frames
    for both the native Ttransform and the API leanTransformTest. Report the timing
    for playback of the two, and dump profile files for both for manual inspection.
    """
    pass
def animate(node_list, keyframe_count):
    """
    Animate the TRS attributes of every node in the list with random
    values for each frame from 1 to "keyframe_count"
    """
    pass


KEY_COUNT = 500

NODE_NAME = 'leanTransformTest'

PLUGIN_PROFILE = 'MPxTransform_profile.txt'

NATIVE_PROFILE = 'Ttransform_profile.txt'


