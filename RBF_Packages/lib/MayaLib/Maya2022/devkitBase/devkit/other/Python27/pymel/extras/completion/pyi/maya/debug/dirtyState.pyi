from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import re
import json
import maya.cmds as cmds


"""
Utility to read and analyze dependency graph dirty state information.
Allows you to produce a comparision of two sets of state information.

    from dirtyState import *

    # Get the current scene's dirty data state information
    stateBefore = dirtyState( check_data=True )

    # Perform some operation that may change the dirty state
    doMyOperation()

    # Get the new dirty data state information
    stateAfter = dirtyState( check_data=True )

    # Compare them to see if they are the same
    stateBefore.compare(stateAfter)
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class dirtyState(object):
    """
    Provides access and manipulation of dirty state data that has been
    produced by various invocations of the 'dgdirty' command.
    """
    
    
    
    def __init__(self, state_file_name='None', long_names='False', check_plugs='True', check_data='True', check_connections='True'):
        """
        Create a dirty state object from a file or the current scene.
        
        The dirty data is read in and stored internally in a format that makes
        formatting and comparison easy.
        
            name              : Name of the state object's data (e.g. file name)
            state_file_name   : If None then the current scene will be used,
                                otherwise the file will be read.
            long_names        : If True then don't attempt to shorten the node
                                names by removing namespaces and DAG path elements.
            check_plugs       : If True then check for plugs that are dirty
            check_data        : If True then check for plug data that is dirty
            check_connections : If True then check for connections that are dirty
        
        This is generated data, not to be used externally:
            _plugs[]       : List of plugs that are dirty
            _data[]        : List of data values that are dirty
            _connections[] : List of connections that are dirty
        """
        ...
    def __str__(self) -> str:
        """
        Return a stringified version of the saved dirty state information.
        Uses a JSON format for easy parsing of return values, but indents
        it so that it is already human readable.
        """
        ...
    def compare(self, other):
        """
        Compare this dirty state against another one and generate a
        summary of how the two sets differ. Differences will be returned
        as a string list consisting of difference descriptions. That way
        when testing, an empty return means the graphs are the same.
        
        The difference type formats are:
        
            plug dirty N            Plug was dirty in other but not in self
            plug clean N            Plug was dirty in self but not in other
            data dirty N            Data was dirty in other but not in self
            data clean N            Data was dirty in self but not in other
            connection dirty S D    Connection was dirty in other but not in self
            connection clean S D    Connection was dirty in self but not in other
        """
        ...
    def compare_one_type(self, other, request_type, made_dirty):
        """
        Compare this dirty state against another one and return the values
        that differ in the way proscribed by the parameters:
        
            request_type    : Type of dirty state to check [plug/data/connection]
            made_dirty    : If true return things that became dirty, otherwise
                          return things that became clean
        
        Nothing is returned for items that did not change.
        """
        ...
    def write(self, fileName='None'):
        """
        Dump the states in the .dirty format it uses for reading. Useful for
        creating a dump file from the current scene, or just viewing the
        dirty state generated from the current scene. If the fileName is
        specified then the output is sent to that file, otherwise it goes
        to stdout.
        """
        ...
    CLEAN_TYPE : str
    
    CONNECTION_TYPE : str
    
    DATA_TYPE : str
    
    DIRTY_TYPE : str
    
    PLUG_TYPE : str
    
    RE_CONNECTION : re.SRE_Pattern
    
    RE_DATA : re.SRE_Pattern
    
    RE_PLUG : re.SRE_Pattern
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def checkMaya():
    """
    Returns True if this script is running from inside Maya, which it
    needs to be in order to work.
    """
    ...


MAYA_IS_AVAILABLE : bool

