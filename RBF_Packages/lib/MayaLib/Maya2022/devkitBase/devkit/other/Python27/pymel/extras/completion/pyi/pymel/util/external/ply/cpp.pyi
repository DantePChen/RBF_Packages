from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import __future__
import re
import copy
import os
import time


"""
# -----------------------------------------------------------------------------
# cpp.py
#
# Author:  David Beazley (http://www.dabeaz.com)
# Copyright (C) 2007
# All rights reserved
#
# This module implements an ANSI-C style lexical preprocessor for PLY. 
# -----------------------------------------------------------------------------
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Preprocessor(object):
    def __init__(self, lexer='None'): ...
    def add_path(self, path): ...
    def collect_args(self, tokenlist): ...
    def define(self, tokens): ...
    def error(self, file, line, msg): ...
    def evalexpr(self, tokens): ...
    def expand_macros(self, tokens, expanded='None'): ...
    def group_lines(self, input): ...
    def include(self, tokens): ...
    def lexprobe(self): ...
    def macro_expand_args(self, macro, args): ...
    def macro_prescan(self, macro): ...
    def parse(self, input, source='None', ignore='{}'):
        """
        # ----------------------------------------------------------------------
        # parse()
        #
        # Parse input text.
        # ----------------------------------------------------------------------
        """
        ...
    def parsegen(self, input, source='None'):
        """
        # ----------------------------------------------------------------------
        # parsegen()
        #
        # Parse an input string/
        # ----------------------------------------------------------------------
        """
        ...
    def token(self):
        """
        # ----------------------------------------------------------------------
        # token()
        #
        # Method to return individual tokens
        # ----------------------------------------------------------------------
        """
        ...
    def tokenize(self, text): ...
    def tokenstrip(self, tokens): ...
    def undef(self, tokens): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class Macro(object):
    def __init__(self, name, value, arglist='None', variadic='False'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def t_error(t): ...
def trigraph(input): ...
def t_CPP_WS(t):
    """
    \s+
    """
    ...
def t_CPP_STRING(t):
    """
    \"([^\\\n]|(\\(.|\n)))*?\"
    """
    ...
def t_CPP_CHAR(t):
    """
    (L)?\'([^\\\n]|(\\(.|\n)))*?\'
    """
    ...
def t_CPP_COMMENT(t):
    """
    (/\*(.|\n)*?\*/)|(//.*?\n)
    """
    ...
def CPP_INTEGER(t):
    """
    (((((0x)|(0X))[0-9a-fA-F]+)|(\d+))([uU]|[lL]|[uU][lL]|[lL][uU])?)
    """
    ...
t_CPP_INTEGER = CPP_INTEGER


t_CPP_ID : str
t_CPP_POUND : str
_trigraph_rep : dict
generators : __future__._Feature
literals : str
t_CPP_DPOUND : str
tokens : tuple
t_CPP_FLOAT : str
_trigraph_pat : re.SRE_Pattern

