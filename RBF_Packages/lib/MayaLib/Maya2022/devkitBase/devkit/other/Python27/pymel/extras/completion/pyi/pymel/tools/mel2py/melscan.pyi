from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import pymel.util as util
from . import mellex
import os
import pymel.util.external.ply.yacc as yacc
import sys
import re
import pymel.util.external.ply.lex as lex
import pymel.internal.factories as factories
import pymel


from pymel.util.common import unescape


if False:
    from typing import Dict, List, Tuple, Union, Optional

def p_translation_unit(t):
    """
    translation_unit : external_declaration
    | translation_unit external_declaration
    """
    ...
def p_type_specifier(t):
    """
    type_specifier : INT
    | FLOAT
    | STRING
    | VECTOR
    | MATRIX
    """
    ...
def p_declaration_specifiers(t):
    """
    declaration_specifiers : type_specifier
    | GLOBAL type_specifier
    """
    ...
def p_group_list(t):
    """
    group_list : group_list group
    | group
    """
    ...
def p_empty(t):
    """
    empty :
    """
    ...
def p_group(t):
    """
    group : element
    | LBRACE group_list_opt RBRACE
    """
    ...
def p_element(t):
    """
    element : declaration_specifiers
    | BREAK
    | CASE
    | CONTINUE
    | DEFAULT
    | DO
    | ELSE
    | FALSE
    | FOR
    | IF
    | IN
    | NO
    | ON
    | OFF
    | RETURN
    | SWITCH
    | TRUE
    | WHILE
    | YES
    | ID
    | VAR
    | ICONST
    | FCONST
    | SCONST
    | PLUS
    | MINUS
    | TIMES
    | DIVIDE
    | MOD
    | NOT
    | CROSS
    | LOR
    | LAND
    | LT
    | LE
    | GT
    | GE
    | EQ
    | NE
    | EQUALS
    | TIMESEQUAL
    | DIVEQUAL
    | MODEQUAL
    | PLUSEQUAL
    | MINUSEQUAL
    | CROSSEQUAL
    | COMPONENT
    | PLUSPLUS
    | MINUSMINUS
    | CONDOP
    | LPAREN
    | RPAREN
    | LBRACKET
    | RBRACKET
    | COMMA
    | SEMI
    | COLON
    | CAPTURE
    | LVEC
    | RVEC
    | COMMENT
    | COMMENT_BLOCK
    | ELLIPSIS
    """
    ...
def p_function_arg(t):
    """
    function_arg : type_specifier VAR
    | type_specifier VAR LBRACKET RBRACKET
    """
    ...
def p_function_declarator(t):
    """
    function_declarator : GLOBAL PROC
    | PROC
    """
    ...
def p_function_specifiers_opt(t):
    """
    function_specifiers_opt : type_specifier
    | type_specifier LBRACKET RBRACKET
    | empty
    """
    ...
def p_function_arg_list_opt(t):
    """
    function_arg_list_opt : function_arg_list
    |  empty
    """
    ...
def p_group_list_opt(t):
    """
    group_list_opt : group_list
    | empty
    """
    ...
def p_function_arg_list(t):
    """
    function_arg_list : function_arg
    | function_arg_list COMMA function_arg
    """
    ...
def p_external_declaration(t):
    """
    external_declaration : function_definition
    | group
    """
    ...
def p_function_definition(t):
    """
    function_definition :  function_declarator function_specifiers_opt ID LPAREN function_arg_list_opt RPAREN group
    """
    ...


tokens : tuple

