from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import pymel.util.external.ply.lex as lex


"""
# ----------------------------------------------------------------------
# clex.py
#
# A lexer for ANSI C.
# ----------------------------------------------------------------------
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def t_LPAREN(t):
    """
    \(
    """
    ...
def t_RBRACKET(t):
    """
    \]
    """
    ...
def t_ELLIPSIS(t):
    """
    \.\.
    """
    ...
def t_COMMENT(t):
    """
    //.*
    """
    ...
def t_RPAREN(t):
    """
    \)
    """
    ...
def t_COMMENT_BLOCK(t):
    """
    /\*(.|\n)*?\*/|/\*(.|\n)*?$
    """
    ...
def t_ID(t):
    """
    ([|]?([:]?([.]?[A-Za-z_][\w]*)+)+)+?
    """
    ...
def t_SEMI(t):
    """
    ;
    """
    ...
def t_CAPTURE(t):
    """
    `
    """
    ...
def t_VAR(t):
    """
    \$[A-Za-z_][\w_]*
    """
    ...
def t_COMPONENT(t):
    """
    \.[xyz]
    """
    ...
def t_NEWLINE(t):
    """
    \n+|\r+
    """
    ...
def t_LBRACKET(t):
    """
    \[
    """
    ...


t_LE : str
t_MINUSMINUS : str
t_GT : str
t_MINUSEQUAL : str
t_ICONST : str
t_LAND : str
t_CROSSEQUAL : str
t_LVEC : str
t_EQ : str
t_MINUS : str
reserved_map : dict
t_DIVEQUAL : str
t_MODEQUAL : str
t_FCONST : str
t_LBRACE : str
t_TIMES : str
t_NOT : str
t_CROSS : str
t_ignore : str
t_RBRACE : str
id_state : NoneType
t_GE : str
t_RVEC : str
t_NE : str
t_TIMESEQUAL : str
r : str
t_LOR : str
t_COMMA : str
t_EQUALS : str
t_PLUS : str
t_PLUSEQUAL : str
suspend_depth : int
tokens : tuple
t_LT : str
reserved : tuple
t_SCONST : str
t_PLUSPLUS : str
t_COLON : str
t_CONDOP : str
t_MOD : str
t_DIVIDE : str

