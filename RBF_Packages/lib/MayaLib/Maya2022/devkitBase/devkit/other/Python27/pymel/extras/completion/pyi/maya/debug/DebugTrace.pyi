from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import DebugTrace


"""
    class A(DebugTrace):
        def __init__(self):
            super( self.__class__, self ).__init__()
        def inner(self):
            self.dbg( "Inner method" )
        def outer(self):
            self.dbg( "Outer method" )
            self.inner()

The trace output for these commands:
    a = A()
    a.debugging = True
    a.outer()
will be:
    DBG: Outer method
    DBG:     Inner method
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DebugTrace(object):
    """
    Simple base class that lets you selectively debug code, with hierarchical
    printing capabilities for more readability.
    
    The output lines are tagged with a 3 character identifier line to make them
    easier to differentiate in your output.
    """
    
    
    
    def __init__(self): ...
    def dbg(self, message):
        """
        Print out the message if debugging is enabled.
        :param message: Message to print
        """
        ...
    def dbg_err(self, message):
        """
        Print out the error message if debugging is enabled.
        :param message: Message to print
        """
        ...
    def dbg_warn(self, message):
        """
        Print out the arning message if debugging is enabled.
        :param message: Message to print
        """
        ...
    def err(self, message):
        """
        Print out the error message whether debugging is enabled or not.
        :param message: Message to print
        """
        ...
    def section(self, title):
        """
        Start a new debug section, returning the section object for manual closing.
        :param title: Title of the new section
        Be careful to match the starts and ends or the indentation level will
        go wonky as no sanity checks are performed on it.
        
            dbg.debugging( True )
            section = dbg_section( "New section" )
            dbg.dbg( "Hello" )
            section.end_section()
        or
            with dbg_section( "New section" ) as section:
                dbg.dbg( "Hello" )
        """
        ...
    def warn(self, message):
        """
        Print out the arning message whether debugging is enabled or not.
        :param message: Message to print
        """
        ...
    @property
    def debugging(self):
        """
        Retrieve the internal enabled state
        """
        ...
    @debugging.setter
    def debugging(self, value): ...
    @property
    def indent_level(self):
        """
        Retrieve the internal indentation level
        """
        ...
    @indent_level.setter
    def indent_level(self, value): ...
    DebugSection : Type[maya.debug.DebugTrace.DebugSection]
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor



