from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import maya
import maya.cmds as cmds
import os
import subprocess


from maya.debug.em_debug_utilities import convert_exception_to_unicode


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DotFormatManager(object):
    """
    Class to manage interactions with the DOT graph visualization format
    """
    
    
    
    def __init__(self): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class GraphVizManager(object):
    """
    Class to manage operations performed by the external graphViz tool
    """
    
    
    
    def __init__(self, use_system_graphviz):
        """
        Initialize the location of the graphviz command path for later use
        """
        ...
    def convert_dot_to_pdf(self, input_file_name, output_file_name, transitive_reduction):
        """
        Convert a DOT file to PDF using Graphviz.
        :param input_file_name: Name of file where the .dot file resides
        :param output_file_name: Name of file where the .pdf file should be generated
        """
        ...
    def get_command(self, command_name):
        """
        Build a string for the Graphviz command to run.
        """
        ...
    def get_version(self):
        """
        :return: output from the graphviz command "dot -V"
        """
        ...
    @staticmethod
    def run_command(command_argv, stdin='None', stdout='None'):
        """
        Run a Graphviz command and handle errors.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor



