from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya


from maya.common.utils import Singleton
from maya.app.prefs.OptionVar import OptionVar


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CachePreferenceHud(OptionVar):
    """
    Class containing the information for the "caching playback HUD visibility" preference.
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceShowWarningMessages(OptionVar):
    """
    Class containing the information for the "caching playback shows warning messages" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceDiscardFramesOutOfRange(OptionVar):
    """
    Class containing the information for the "caching playback discards cache data outside of playback range" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceMemoryThreshold(OptionVar):
    """
    Class containing the information for the "caching playback resource guard memory threshold" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    @staticmethod
    def ranges():
        """
        :return 4-tuple with the min/max/soft_min/soft_max values for the threshold
        """
        ...
    
    
    ov_id : str


class CachePreferenceShowWarningFrames(OptionVar):
    """
    Class containing the information for the "caching playback shows warning frames in the timeslider" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceTimesliderBarSpacing(OptionVar):
    """
    Class containing the information for the "timeslider bar custom draw spacing" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceTimesliderBarPosition(OptionVar):
    """
    Class containing the information for the "caching playback timeslider bar position" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    def find_index(self, value):
        """
        Find the index in the DATA list of the given enum value
        """
        ...
    DATA : list
    
    KEYS : list
    
    
    
    data : list
    
    ov_id : str


class CachePreferenceShowInvalidatedFrames(OptionVar):
    """
    Class containing the information for the "caching playback shows invalid frames in the timeslider" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceResourceGuard(OptionVar):
    """
    Class containing the information for the "caching playback resource guard mode" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceTimesliderBarHeight(OptionVar):
    """
    Class containing the information for the "caching playback timeslider bar height" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceShowSubframes(OptionVar):
    """
    Class containing the information for the "caching playback shows subframes in the timeslider" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str


class CachePreferenceShowCacheStatus(OptionVar):
    """
    Class containing the information for the "caching playback show cache status" preference
    """
    
    
    
    def __init__(self):
        """
        Initialize the preference interface
        """
        ...
    
    
    ov_id : str




OPTION_VAR_TYPE_FLOAT : int
OPTION_VAR_TYPE_INT : int
OPTION_VAR_TYPE_BOOL : int

