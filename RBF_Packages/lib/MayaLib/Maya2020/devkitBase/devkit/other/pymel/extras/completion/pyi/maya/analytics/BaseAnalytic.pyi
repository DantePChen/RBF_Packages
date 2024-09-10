from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import os


from maya.analytics.ObjectNamer import ObjectNamer


if False:
    from typing import Dict, List, Tuple, Union, Optional

class BaseAnalytic(object):
    """
    Base class for output for analytics.
    
    The default location for the anlaytic output is in a subdirectory
    called 'MayaAnalytics' in your temp directory. You can change that
    at any time by calling set_output_directory().
    
    Class static member:
         ANALYTIC_NAME : Name of the analytic
    
    Class members:
         directory     : Directory the output will go to
         is_static     : True means this analytic doesn't require a file to run
         logger        : Logging object for errors, warnings, and messages
         plug_namer    : Object creating plug names, possibly anonymous
         node_namer    : Object creating node names, possibly anonymous
         csv_output    : Location to store legacy CSV output
         plug_namer    : Set by option 'anonymous' - if True then make plug names anonymous
         node_namer    : Set by option 'anonymous' - if True then make node names anonymous
         __options     : Dictionary of per-analytic options
    """
    
    
    
    def __init__(self):
        """
        Start out the analytic with no data and pointing to stdout
        """
        ...
    def debug(self, msg):
        """
        Utility to standardize debug messages coming from analytics.
        """
        ...
    def error(self, msg):
        """
        Utility to standardize errors coming from analytics.
        """
        ...
    def establish_baseline(self):
        """
        This is run on an empty scene, to give the analytic a chance to
        establish any baseline data it might need (e.g. the nodes in an
        empty scene could all be ignored by the analytic)
        
        Base implementation does nothing. Derived classes should call
        their super() method though, in case something does get added.
        """
        ...
    def json_file(self):
        """
        Although an analytic is free to create any set of output files it
        wishes there will always be one master JSON file containing the
        """
        ...
    def log(self, msg):
        """
        Utility to standardize logging messages coming from analytics.
        """
        ...
    def marker_file(self):
        """
        Returns the name of the marker file used to indicate that the
        computation of an analytic is in progress. If this file remains
        in a directory after the analytic has run that means it was
        interrupted and the data is not up to date.
        
        This file provides a safety measure against machines going down
        or analytics crashing.
        """
        ...
    def name(self):
        """
        Get the name of this type of analytic
        """
        ...
    def option(self, option):
        """
        :param option: Name of option to check
        :return: the current value of the named option
        """
        ...
    def output_files(self):
        """
        This is used to get the list of files the analytic will generate.
        There will always be a JSON file generated which contains at minimum
        the timing information. An analytic should override this method only
        if they are adding more output files (e.g. a .jpg file).
        
        This should only be called after the final directory has been set.
        """
        ...
    def set_options(self, options):
        """
        Modify the settings controlling the run operation of the analytic.
        Override this method if your analytic has some different options
        available to it, but be sure to call this parent version after since
        it sets common options.
        
        Note: Options are merged with existing options so make sure they are unique
        :param options: Dictionary of OPTION:OPTION_VALUE to be set
        """
        ...
    def set_output_directory(self, directory):
        """
        Call this method to set a specific directory as the output location.
        The special names 'stdout' and 'stderr' are recognized as the
        output and error streams respectively rather than a directory.
        """
        ...
    def warning(self, msg):
        """
        Utility to standardize warnings coming from analytics.
        """
        ...
    ANALYTIC_NAME : str
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




OPTION_DETAILS : str
OPTION_SUMMARY : str
OPTION_ANONYMOUS : str

