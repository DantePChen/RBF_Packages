from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import envparse
import pymel.util.external.ply.lex as lex
import sys
import logging
import os


from pymel.mayautils import getMayaAppDir


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ValueLex:
    """
    second level lexer to parse right-values depending on os name
    """
    
    
    
    def __init__(self, symbols, osname="'nt'"): ...
    def build(self, **kwargs): ...
    def t_PATHSEP(self, t):
        """
        \/|\\
        """
        ...
    def t_RVAR1(self, t):
        """
        \$[^\\^/^:^*^"^<^>^|^=^ ^\t^\n^#^$]+
        """
        ...
    def t_RVAR2(self, t):
        """
        \%[^\\^/^:^*^"^<^>^|^=^ ^\t^\n^#]+\%
        """
        ...
    def t_SEP(self, t):
        """
        :;
        """
        ...
    def t_VALUE(self, t):
        """
        [^=^\n^#^$]+
        """
        ...
    def t_error(self, t): ...
    def test(self, data):
        """
        # Test it
        """
        ...
    Warn : Type[pymel.tools.envparse.Warn]
    
    t_ignore : str
    
    tokens : tuple


class EnvLex:
    """
    ply.lex lexer class to parse Maya.env file
    """
    
    
    
    def __init__(self): ...
    def build(self, **kwargs): ...
    def t_ANY_error(self, t): ...
    def t_ANY_newline(self, t):
        """
        [ \t]*\n+
        """
        ...
    def t_INITIAL_error(self, t): ...
    def t_VAR(self, t):
        """
        [^\\^\/^\:^\*^\"^\<^\>^\|^=^ ^\t^\n^#]+
        """
        ...
    def t_end_ASSIGN(self, t):
        """
        [ \t]*=[ \t]*
        """
        ...
    def t_end_VALUE(self, t):
        """
        [^=^\n^#]+
        """
        ...
    def t_left_ASSIGN(self, t):
        """
        [ \t]*=[ \t]*
        """
        ...
    def t_left_error(self, t): ...
    def t_right_ASSIGN(self, t):
        """
        [ \t]*=[ \t]*
        """
        ...
    def t_right_VALUE(self, t):
        """
        [^=^\n^#]+
        """
        ...
    def t_right_error(self, t): ...
    def test(self, data):
        """
        # Test it
        """
        ...
    t_ANY_ignore_COMMENT : str
    
    t_INITIAL_ignore : str
    
    t_cancel_ignore : str
    
    t_end_ignore : str
    
    t_left_ignore : str
    
    t_right_ignore : str
    
    tokens : tuple




def parse(text, environ={'PROXY_FOR': 'os.environ'}, osname="'nt'"): ...
def parseMayaenv(envLocation='None', version='None'):
    """
    parse the Maya.env file and set the environement variablas and python path accordingly.
    You can specify a location for the Maya.env file or the Maya version
    """
    ...


_logger : logging.Logger

