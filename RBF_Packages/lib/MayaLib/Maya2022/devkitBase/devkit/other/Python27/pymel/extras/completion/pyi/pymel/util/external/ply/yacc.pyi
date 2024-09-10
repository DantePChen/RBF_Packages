from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import sys
import re
import os
import types


"""
# -----------------------------------------------------------------------------
# ply: yacc.py
#
# Copyright (C) 2001-2009,
# David M. Beazley (Dabeaz LLC)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
# 
# * Redistributions of source code must retain the above copyright notice,
#   this list of conditions and the following disclaimer.  
# * Redistributions in binary form must reproduce the above copyright notice, 
#   this list of conditions and the following disclaimer in the documentation
#   and/or other materials provided with the distribution.  
# * Neither the name of the David Beazley or Dabeaz LLC may be used to
#   endorse or promote products derived from this software without
#  specific prior written permission. 
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# -----------------------------------------------------------------------------
#
# This implements an LR parser that is constructed from grammar rules defined
# as Python functions. The grammer is specified by supplying the BNF inside
# Python documentation strings.  The inspiration for this technique was borrowed
# from John Aycock's Spark parsing system.  PLY might be viewed as cross between
# Spark and the GNU bison utility.
#
# The current implementation is only somewhat object-oriented. The
# LR parser itself is defined in terms of an object (which allows multiple
# parsers to co-exist).  However, most of the variables used during table
# construction are defined in terms of global variables.  Users shouldn't
# notice unless they are trying to define multiple parsers at the same
# time using threads (in which case they should have their head examined).
#
# This implementation supports both SLR and LALR(1) parsing.  LALR(1)
# support was originally implemented by Elias Ioup (ezioup@alumni.uchicago.edu),
# using the algorithm found in Aho, Sethi, and Ullman "Compilers: Principles,
# Techniques, and Tools" (The Dragon Book).  LALR(1) has since been replaced
# by the more efficient DeRemer and Pennello algorithm.
#
# :::::::: WARNING :::::::
#
# Construction of LR parsing tables is fairly complicated and expensive.
# To make this module run fast, a *LOT* of work has been put into
# optimization---often at the expensive of readability and what might
# consider to be good Python "coding style."   Modify the code at your
# own risk!
# ----------------------------------------------------------------------------
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Grammar(object):
    def __getitem__(self, index): ...
    def __init__(self, terminals): ...
    def __len__(self) -> int: ...
    def add_production(self, prodname, syms, func='None', file="''", line='0'): ...
    def build_lritems(self): ...
    def compute_first(self):
        """
        # -------------------------------------------------------------------------
        # compute_first()
        #
        # Compute the value of FIRST1(X) for all symbols
        # -------------------------------------------------------------------------
        """
        ...
    def compute_follow(self, start='None'):
        """
        # ---------------------------------------------------------------------
        # compute_follow()
        #
        # Computes all of the follow sets for every non-terminal symbol.  The
        # follow set is the set of all symbols that might follow a given
        # non-terminal.  See the Dragon book, 2nd Ed. p. 189.
        # ---------------------------------------------------------------------
        """
        ...
    def find_unreachable(self): ...
    def infinite_cycles(self): ...
    def set_precedence(self, term, assoc, level): ...
    def set_start(self, start='None'): ...
    def undefined_symbols(self):
        """
        # -----------------------------------------------------------------------------
        # undefined_symbols()
        #
        # Find all symbols that were used the grammar, but not defined as tokens or
        # grammar rules.  Returns a list of tuples (sym, prod) where sym in the symbol
        # and prod is the production where the symbol was used. 
        # -----------------------------------------------------------------------------
        """
        ...
    def unused_precedence(self): ...
    def unused_rules(self): ...
    def unused_terminals(self):
        """
        # -----------------------------------------------------------------------------
        # unused_terminals()
        #
        # Find all terminals that were defined, but not used by the grammar.  Returns
        # a list of all symbols.
        # -----------------------------------------------------------------------------
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class YaccError(exceptions.Exception):
    """
    # Exception raised for yacc-related errors
    """
    
    
    
    __weakref__ : getset_descriptor


class LRTable(object):
    def __init__(self): ...
    def bind_callables(self, pdict):
        """
        # Bind all production function names to callable objects in pdict
        """
        ...
    def read_pickle(self, filename): ...
    def read_table(self, module): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class YaccSymbol:
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...


class NullLogger(object):
    """
    # Null logger is used when no output is generated. Does nothing.
    """
    
    
    
    def __call__(self, *args, **kwargs): ...
    def __getattribute__(self, name): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class ParserReflect(object):
    """
    # -----------------------------------------------------------------------------
    # ParserReflect()
    #
    # This class represents information extracted for building a parser including
    # start symbol, error function, tokens, precedence list, action functions,
    # etc.
    # -----------------------------------------------------------------------------
    """
    
    
    
    def __init__(self, pdict, log='None'): ...
    def get_all(self):
        """
        # Get all of the basic information
        """
        ...
    def get_error_func(self):
        """
        # Look for error handler
        """
        ...
    def get_pfunctions(self):
        """
        # Get all p_functions from the grammar
        """
        ...
    def get_precedence(self):
        """
        # Get the precedence map (if any)
        """
        ...
    def get_start(self):
        """
        # Get the start symbol
        """
        ...
    def get_tokens(self):
        """
        # Get the tokens map
        """
        ...
    def signature(self):
        """
        # Compute a signature over the grammar
        """
        ...
    def validate_all(self):
        """
        # Validate all of the information
        """
        ...
    def validate_error_func(self):
        """
        # Validate the error function
        """
        ...
    def validate_files(self): ...
    def validate_pfunctions(self):
        """
        # Validate all of the p_functions
        """
        ...
    def validate_precedence(self):
        """
        # Validate and parse the precedence map
        """
        ...
    def validate_start(self):
        """
        # Validate the start symbol
        """
        ...
    def validate_tokens(self):
        """
        # Validate the tokens
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class YaccProduction:
    def __getitem__(self, n): ...
    def __getslice__(self, i, j): ...
    def __init__(self, s, stack='None'): ...
    def __len__(self) -> int: ...
    def __setitem__(self, n, v): ...
    def error(self): ...
    def lexpos(self, n): ...
    def lexspan(self, n): ...
    def lineno(self, n): ...
    def linespan(self, n): ...
    def set_lineno(self, n, lineno): ...


class Production(object):
    def __getitem__(self, index): ...
    def __init__(self, number, name, prod, precedence="('right', 0)", func='None', file="''", line='0'): ...
    def __len__(self) -> int: ...
    def __nonzero__(self) -> bool: ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def bind(self, pdict):
        """
        # Bind the production function name to a callable
        """
        ...
    def lr_item(self, n):
        """
        # Return the nth lr_item from the production (or None if at the end)
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor
    
    reduced : int


class LRItem(object):
    def __init__(self, p, n): ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class MiniProduction(object):
    """
    # This class serves as a minimal standin for Production objects when
    # reading table data from files.   It only contains information
    # actually used by the LR parsing engine, plus some additional
    # debugging information.
    """
    
    
    
    def __init__(self, str, name, len, func, file, line): ...
    def __repr__(self) -> str: ...
    def __str__(self) -> str: ...
    def bind(self, pdict):
        """
        # Bind the production function name to a callable
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class LRParser:
    def __init__(self, lrtab, errorf): ...
    def errok(self): ...
    def parse(self, input='None', lexer='None', debug='0', tracking='0', tokenfunc='None'): ...
    def parsedebug(self, input='None', lexer='None', debug='None', tracking='0', tokenfunc='None'): ...
    def parseopt(self, input='None', lexer='None', debug='0', tracking='0', tokenfunc='None'): ...
    def parseopt_notrack(self, input='None', lexer='None', debug='0', tracking='0', tokenfunc='None'): ...
    def restart(self): ...


class PlyLogger(object):
    def __init__(self, f): ...
    def critical(self, msg, *args, **kwargs): ...
    def debug(self, msg, *args, **kwargs): ...
    def error(self, msg, *args, **kwargs): ...
    def info(self, msg, *args, **kwargs): ...
    def warning(self, msg, *args, **kwargs): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class GrammarError(YaccError):
    pass


class LRGeneratedTable(LRTable):
    def __init__(self, grammar, method="'LALR'", log='None'): ...
    def add_lalr_lookaheads(self, C): ...
    def add_lookaheads(self, lookbacks, followset): ...
    def compute_follow_sets(self, ntrans, readsets, inclsets): ...
    def compute_lookback_includes(self, C, trans, nullable): ...
    def compute_nullable_nonterminals(self): ...
    def compute_read_sets(self, C, ntrans, nullable): ...
    def dr_relation(self, C, trans, nullable): ...
    def find_nonterminal_transitions(self, C): ...
    def lr0_closure(self, I): ...
    def lr0_goto(self, I, x): ...
    def lr0_items(self):
        """
        # Compute the LR(0) sets of item function
        """
        ...
    def lr_parse_table(self):
        """
        # -----------------------------------------------------------------------------
        # lr_parse_table()
        #
        # This function constructs the parse tables for SLR or LALR
        # -----------------------------------------------------------------------------
        """
        ...
    def pickle_table(self, filename, signature="''"): ...
    def reads_relation(self, C, trans, empty): ...
    def write_table(self, modulename, outputdir="''", signature="''"): ...


class VersionError(YaccError):
    pass


class LALRError(YaccError):
    pass




def load_ply_lex():
    """
    # Python 2.x/3.0 compatibility.
    """
    ...
def parse(self, input='None', lexer='None', debug='0', tracking='0', tokenfunc='None'): ...
def rightmost_terminal(symbols, terminals):
    """
    # -----------------------------------------------------------------------------
    # rightmost_terminal()
    #
    # Return the rightmost terminal from a list of symbols.  Used in add_production()
    # -----------------------------------------------------------------------------
    """
    ...
def format_stack_entry(r):
    """
    # Format stack entries when the parser is running in debug mode
    """
    ...
def format_result(r):
    """
    # Format the result message that the parser produces when running in debug mode.
    """
    ...
def parse_grammar(doc, file, line):
    """
    # -----------------------------------------------------------------------------
    # parse_grammar()
    #
    # This takes a raw grammar rule string and parses it into production data
    # -----------------------------------------------------------------------------
    """
    ...
def traverse(x, N, stack, F, X, R, FP): ...
def digraph(X, R, FP): ...
def yacc(method="'LALR'", debug='1', module='None', tabmodule="'parsetab'", start='None', check_recursion='1', optimize='0', write_tables='1', debugfile="'parser.out'", outputdir="''", debuglog='None', errorlog='None', picklefile='None'): ...
def func_code(f): ...
def get_caller_module_dict(levels): ...


tab_module : str
error_count : int
debug_file : str
resultlimit : int
yaccdebug : int
MAXINT : int
__version__ : str
default_lr : str
pickle_protocol : int
_is_identifier : re.SRE_Pattern

