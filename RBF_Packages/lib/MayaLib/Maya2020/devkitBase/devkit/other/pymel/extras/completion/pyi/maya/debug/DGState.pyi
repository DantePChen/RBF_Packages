from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import hashlib
import maya.cmds as cmds
import sys
import re
import json
import subprocess
import os
import math


"""
Utility to contain the DG state information captured by a correctness
analysis. It reads the information from a file and provides analysis
tools, such as comparison.

State information is in JSON format for easy storage, parsing, and comparison.
Data is stored in a method that provides a parsing clue for what types of
values are stored on the plug:
    isinstance(VALUE, numbers.Number) : Simple numeric value
    type(VALUE) is list : List of simple numeric values (matrix or vector)
    type(VALUE) is dict : Named data type, with a named parser

    {
        "state" :
        {
            "file" : "NAME_OF_FILE",
            "time" : "TIME_AND_DATE_OF_STATE_CAPTURE",
            "dirty" :
            {
                "connections" : [ LIST_OF_ALL_DIRTY_CONNECTIONS ],
                "data" : [ LIST_OF_ALL_DIRTY_DATA_PLUGS ]
            },
            "data" :
            {
                NODE :
                {
                    PLUG : PLUG_VALUE,
                    PLUG : [ PLUG_MATRIX ],
                    PLUG : { "mesh" : { MESH_DATA } },
                    ...
                },
                ...
            },
            "screenshot" : FILE_WHERE_SCREENSHOT_IS_SAVED
        }
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DGState(object):
    """
    State object containing all data values that come out of the dbpeek
    command using the 'data' operation for simple data values and the
    dbpeek 'mesh' operation for mesh geometry.
    
    results_files: Where the intermediate results are stored. None means don't store them.
    image_file:    Where the screenshot is stored. None means don't store it.
    state:         Data state information from the scene.
    """
    
    
    
    def __init__(self):
        """
        Create a new state object.
            :member results_file    : Destination for the raw numerical data for comparison
            :member image_file      : Destination for the viewport screenshot
            :member filtered_plugs  : List of plugs to use for the scan
            :member state           : Newline-separated list of state information
            :member md5_value       : MD5 value of the screenshot
            :member nodes_blacklist : List of nodes to omit from the scan
        """
        ...
    def __str__(self) -> str:
        """
        Dump the state as a string. This converts the state CSV into a JSON
        indented format to make it easier to read.
        """
        ...
    def compare(self, other, output_mode, ignored_nodes='None', whitelist='None'):
        """
        Compare two state information collections and return a count of the
        number of differences. The first two fields (node,plug) are used to
        uniquely identify the line so that we are sure we are comparing the
        same two things.
        
        The 'clean' flag in column 2 is omitted from the comparison since
        the DG does funny things with the flag to maintain the holder/writer
        states of the data.
        
        :param other:         Other DGstate to compare against
        :param output_mode:   Type of output to return (DGState.OUTPUT_CSV, DGState.OUTPUT_JSON)
                              Modes return a tuple of (ERROR_DETAILS, ERROR_COUNT, WORST_ERROR_METRIC, NUMBER_OF_ITEMS_COMPARED)
        :param ignored_nodes: List of nodes to omit from the comparison
        :param whitelist:     List of nodes to compare. If None then include all available nodes.
        
        ignored_nodes takes precedence over whitelist so if it appears in both it will be ignored.
        
             The ERROR_DETAILS for each mode are:
                 OUTPUT_CSV  : CSV data with each mismatching plug on its own line
                               PLUG,THIS_STATE_VALUES,COMPARED_STATE_VALUES
                 OUTPUT_JSON : JSON data with each mismatching plug as a key in dictionary:
                               {
                                   PLUG : { 'match' : ERROR_METRIC
                                          , 'value' : [THIS_STATE_VALUES]
                                          , 'other' : [COMPARED_STATE_VALUES]
                                          }
                               }
        :return: Comparison results, in a format described by the output_mode
        """
        ...
    def filter_state(self, plug_filter):
        """
        Take the current state information and filter out all of the plugs
        not on the plug_filter list. This is used to restrict the output to
        the set of plugs the EM is evaluating.
        
        :param plug_filter: Dictionary of nodes whose values are dictionaries of
                            root level attributes that are to be used for the
                            purpose of the comparison.
        
                            None means no filter, i.e. accept all plugs.
        """
        ...
    def get_md5(self):
        """
        :return: md5 checksum from the image file, if it exists, otherwise
                 return '' if the image file wasn't generated, for an easy match.
        """
        ...
    def read_graph(self, graph_file):
        """
        Read the graph configuration from a file. If this graph plug list
        exists when doing the comparison then only plugs on the list will
        be considered, otherwise everything will be checked.
        
        :param graph_file: File from which to read the graph
        
        The format of the file is a sequence of nodes followed by their
        attributes demarcated by leading tabs:
            NODE1
                ATTRIBUTE1
                ATTRIBUTE2
                ...
            NODE2
                ATTRIBUTE1
                ATTRIBUTE2
                ...
            ...
        """
        ...
    def read_state(self, results_file='None', image_file='None'):
        """
        Read in the results from a previous state capture from the given results and
        image files.
        
        :param results_file: Name of file from which to load the results.
                             Do not load anything if None.
        :param image_file:   Name of file from which to load the current viewport screenshot.
                             Do not load anything if None.
        """
        ...
    def scan_scene(self, do_eval, data_types):
        """
        Read in the state information from the current scene.
        Create a new state object, potentially saving results offline if
        requested.
        
        :param do_eval:    True means force evaluation of the plugs before checking
                           state. Used in DG mode since not all outputs used for
                           (e.g.) drawing will be in the datablock after evaluation.
        :param data_types: Type of data to look for - {mesh, vertex, number, vector, screen}
                           If screen is in the list the 'image_file' argument must also be specified.
        """
        ...
    def store_state(self, results_file='None', image_file='None'):
        """
        Store the existing state in the files passed in.
        
        :param results_file: Destination for the raw numerical data for comparison
        :param image_file:   Destination for the viewport screenshot
        """
        ...
    OUTPUT_CSV : int
    
    OUTPUT_JSON : int
    
    RE_INVERSE_MATRIX : str
    
    SCREENSHOT_NODE : str
    
    SCREENSHOT_PLUG_IMF : str
    
    SCREENSHOT_PLUG_MAG : str
    
    SCREENSHOT_PLUG_MD5 : str
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




RE_ROOT_ATTRIBUTE : re.SRE_Pattern
SIGNIFICANT_DIGITS_INVERSE : float
IMAGEMAGICK_METRIC : str
RE_IMF_IDENTICAL : re.SRE_Pattern
NO_SIGNIFICANT_DIGITS_MATCH : int
IMF_DIFF_MATCH_TOLERANCE : float
MD5_AS_DEFAULT_COMPARATOR : bool
MD5_DEFAULT : str
RELATIVE_TOLERANCE : float
IMF_DIFF_MIN_DIFFERENCE : str
ALL_SIGNIFICANT_DIGITS_MATCH : int
RE_IMF_COMPARE : re.SRE_Pattern
IMAGEMAGICK_MATCH_TOLERANCE : int
MD5_BLOCKSIZE : int
RE_IMG_COMPARE : re.SRE_Pattern
ABSOLUTE_ZERO_TOLERANCE : float
SIGNIFICANT_DIGITS : float

