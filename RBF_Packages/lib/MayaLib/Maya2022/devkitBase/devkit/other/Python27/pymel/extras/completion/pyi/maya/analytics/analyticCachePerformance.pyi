from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import json
import maya
import maya.cmds as cmds


"""
See the class docs for a description of the data collected.
"""


from maya.analytics.decorators import makeAnalytic
from maya.analytics.BaseAnalytic import BaseAnalytic
from maya.debug.emModeManager import emModeManager
from maya.analytics.decorators import addMethodDocs
from maya.debug.PlaybackManager import PlaybackManager
from maya.analytics.decorators import addHelp
from maya.plugin.evaluator.CacheEvaluatorManager import CacheEvaluatorManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticCachePerformance(BaseAnalytic):
    """
    The normal output is the set of cache performance statistics.
    
        {
          "baseline"    : {
              "memory"      : [0,0]  // baseline [physical,virtual] memory usage after animation, but before caching
            , "playback"    : 0      // frames-per-second for non-caching EMP playback
            , "start_frame" : 0
            , "end_frame"   : 0      // start and end frames being used for the test
            }
        // for each of the cache testing modes
        , "caching" : [
            { "configuration_name" : "XXX"     // the name given to this configuration for identification purposes
            , "configuration"      : { ... }   // the configuration information for the cache evaluator
            , "caching_points"     : { ... }   // caching points used by the cache evaluator for this run
            , "frames_cached"      : [[S1,E1],
                                      [S2,E2]] // list of frame ranges cached
            , "cached_data"        : {         // counts and memory used for each node type enabled as a caching point
               "NODE_TYPE" : {
                 "count"  : 0,
                 "memory" : 0} }
            , "filling_playback"       : 0       // frames-per-second for playback while the cache is refilling synchronously
            , "fill_time"              : 0       // amount of time it took to fill the cache in the background, in seconds
                                                 //    (while nothing is going on in the foreground)
            , "cached_playback"        : 0       // frames-per-second for playback from cached data
            , "memory_before_playback" : [0,0]   // overall Maya [physical,virtual] memory usage after caching
            , "memory_after_playback"  : [0,0]   // overall Maya [physical,virtual] memory usage after caching playback
            , "evacuation_time"        : 0       // amount of time it takes to evacuate the cache, in seconds
            }
          ]
        }
    
    Several values in this output are lists. If the 'details' option is specified then the full
    list is included in the output. Otherwise the list is replaced by a count of the list members.
    These members include:
        OUT[KEY_CACHING][KEY_CACHING_POINTS]
        OUT[KEY_CACHING][KEY_CONFIGURATION][KEY_CACHE_MODE]
    """
    
    
    
    def __init__(self):
        """
        Initialize the persistent class members
        """
        ...
    def get_memory(self):
        """
        :return: A 2 member list with current physical and virtual memory in use by Maya
        """
        ...
    def run(self):
        """
        Run the analytic on the current scene.
        Performs a playback for a set of caching modes, recording the memory usage
        and timing for each of them along with the default configuration for reference.
        :result: JSON data as described in the class doc
        """
        ...
    @staticmethod
    def help():
        """
        Call this method to print the class documentation, including all methods.
        """
        ...
    ANALYTIC_DESCRIPTION_DETAILED : str
    
    ANALYTIC_DESCRIPTION_SHORT : unicode
    
    ANALYTIC_LABEL : unicode
    
    ANALYTIC_NAME : str
    
    __fulldocs__ : str
    
    is_static : bool




KEY_CONFIGURATION : str
KEY_CACHE_MODE : str
KEY_MEMORY_BEFORE : str
KEY_CACHED_DATA : str
CACHE_UNAVAILABLE : float
KEY_EVACUATION : str
KEY_END_FRAME : str
KEY_BASELINE : str
KEY_MEMORY : str
KEY_NODE_COUNT : str
KEY_CACHING_POINTS : str
KEY_CONFIGURATION_NAME : str
CACHE_STANDARD_MODE_VP2_SW : list
CACHE_STANDARD_MODE_EVAL_SHAPES : list
KEY_MEMORY_AFTER : str
CACHE_STANDARD_MODE_EVAL : list
CACHE_STANDARD_MODE_VP2_HW : list
KEY_FILLING_PLAYBACK : str
KEY_LOADED : str
KEY_ENABLED : str
KEY_CACHED_PLAYBACK : str
KEY_CACHING : str
KEY_PLAYBACK : str
KEY_START_FRAME : str
KEY_FRAMES_CACHED : str
EVALUATOR_NAME : str
MAX_FRAMES : int
OPTION_DETAILS : str
KEY_FILL_TIME : str

