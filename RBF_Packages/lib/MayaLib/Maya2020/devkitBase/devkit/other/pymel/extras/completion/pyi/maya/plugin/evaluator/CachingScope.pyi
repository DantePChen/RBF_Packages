from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import re
import maya.cmds as cmds


from maya.plugin.evaluator.CacheEvaluatorManager import CacheEvaluatorManager
from maya.debug.emModeManager import emModeManager


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CachingScope(object):
    """
    Scope object responsible for setting up caching and restoring original setup after
    """
    
    
    
    def __enter__(self):
        """
        Enter the scope, setting up the evaluator managers and initial states
        """
        ...
    def __exit__(self, exit_type, value, traceback):
        """
        Exit the scope, restoring all of the state information
        """
        ...
    def __init__(self):
        """
        Initialize everything to be empty - only use the "with" syntax with this object
        """
        ...
    def check_valid_frames(self, unit_test, expected_valid_frames):
        """
        :param unit_test: The test object from which this method was called
        :param expected_valid_frames: The list of frames the text expected to be cached
        :return: True if the cached frame list matches the expected frame list
        """
        ...
    @staticmethod
    def get_valid_frames():
        """
        Utility to extract the list of frames cached from the cacheEvaluator command output
        """
        ...
    @staticmethod
    def is_caching_scope():
        """
        Method to determine whether caching is on or off in this object's scope
        :return: True, since this is the caching scope
        """
        ...
    @staticmethod
    def verify_scope_setup(unit_test):
        """
        Meta-test to check that the scope was defined correctly
        :param unit_test: The test object from which this method was called
        """
        ...
    @staticmethod
    def wait_for_cache(unit_test, wait_time='2'):
        """
        Fill the cache in the background, waiting for a maximum time
        :param unit_test: The test object from which this method was called
        :param wait_time: Time the test is willing to wait for cache completion (in seconds)
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class NonCachingScope(object):
    """
    Scope object responsible for setting up non cached mode and restoring default settings after
    """
    
    
    
    def __enter__(self):
        """
        Enter the scope, setting up the evaluator managers and initial states
        """
        ...
    def __exit__(self, exit_type, value, traceback):
        """
        Exit the scope, restoring all of the state information
        """
        ...
    def __init__(self):
        """
        Initialize everything to be empty - only use the "with" syntax with this object
        """
        ...
    @staticmethod
    def is_caching_scope():
        """
        Method to determine whether caching is on or off in this object's scope
        :return: False, since this is the non-caching scope
        """
        ...
    @staticmethod
    def verify_scope_setup(unit_test):
        """
        Meta-test to check that the scope was defined correctly
        :param unit_test: The test object from which this method was called
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




RE_EXTRACT_FRAMES : re.SRE_Pattern
CACHE_STANDARD_MODE_EVAL : list

