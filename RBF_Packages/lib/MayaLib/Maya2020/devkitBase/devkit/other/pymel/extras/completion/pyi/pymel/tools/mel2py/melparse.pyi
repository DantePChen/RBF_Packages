from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import _sre
import pymel.core.uitypes as uitypes
from . import melscan
import pymel.util.external.ply.yacc as yacc
import pymel.core.rendering as rendering
import pymel.api as api
import pymel
import __builtin__ as builtin_module
import tempfile
import pymel.core.animation as animation
import pymel.core.nodetypes as nt
import pymel.core.nodetypes as nodetypes
import string
import maya.cmds as cmds
import pymel.core.effects as effects
import pymel.util.external.ply.lex as lex
import pymel.core.modeling as modeling
import pymel.core.datatypes as dt
import pymel.internal.factories as factories
import pymel.core.language as language
from . import mellex
import pymel.core.windows as windows
import pymel.core.uitypes as ui
import pymel.core.context as context
import pymel.core.runtime as runtime


from pymel.core.system import *
from pymel.core.animation import *
from pymel.core.context import *
from pymel.core.language import *
from pymel.core.other import *
from pymel.core.rendering import *
from pymel.core.modeling import *
from pymel.core.windows import *
from pymel.core.effects import *
from pymel.core.general import *


from pymel.util.utilitytypes import TwoWayDict
from pymel.util.common import unescape


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Token(str):
    def __add__(self, other): ...
    def __getslice__(self, start, end): ...
    @staticmethod
    def __new__(cls, val, type, lineno='None', **kwargs): ...
    __dict__ : dictproxy


class MelScanner(object):
    """
    Basic mel parser which only tries to get information about procs
    """
    
    
    
    def build(self): ...
    def parse(self, data): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class MelParser(object):
    """
    The MelParser class around which all other mel2py functions are based.
    """
    
    
    
    def build(self, rootModule='None', pymelNamespace="''", verbosity='0', addPymelImport='True', expressionsOnly='False', forceCompatibility='True', parentData='None'): ...
    def parse(self, data): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class MelParseError(builtin_module.Exception):
    def __init__(self, *args, **kwargs): ...
    def __str__(self) -> str: ...
    __weakref__ : getset_descriptor


class Comment(object):
    def __init__(self, token): ...
    def format(self): ...
    def leadingSpace(self): ...
    def withLeadingSpace(self): ...
    @classmethod
    def join(cls, comments, stripCommonSpace='False'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class BatchData(object):
    def __init__(self, **kwargs): ...
    @staticmethod
    def __new__(cls, *p, **k):
        """
        # redefine __new__
        """
        ...
    __dict__ : dictproxy
    
    
    
    __weakref__ : getset_descriptor


class ExpressionParseError(MelParseError, builtin_module.TypeError):
    """
    Error when mel code cannot be parsed into a python expression
    """
    
    
    
    pass


class ArrayToken(Token):
    @staticmethod
    def __new__(cls, val, type, size, lineno='None', **kwargs): ...




def p_seen_FOR(t):
    """
    seen_FOR :
    """
    ...
def merge_assignment_spillover(t, curr_lineno, title="''"): ...
def p_command_input(t):
    """
    command_input : unary_expression
    | command_flag
    """
    ...
def p_labeled_statement_list(t):
    """
    labeled_statement_list : labeled_statement
    | labeled_statement_list labeled_statement
    """
    ...
def p_unary_command_expression(t):
    """
    unary_command_expression : procedure_expression
    | unary_operator procedure_expression
    """
    ...
def p_primary_expression(t):
    """
    primary_expression :    boolean
    |    numerical_constant
    """
    ...
def p_selection_statement_2(t):
    """
    selection_statement : IF LPAREN expression RPAREN statement_required ELSE hold_comments statement_required
    """
    ...
def p_int_constant(t):
    """
    int_constant :     ICONST
    """
    ...
def p_expression_list(t):
    """
    expression_list : expression
    | expression_list COMMA expression
    """
    ...
def format_singleline_comments(comments): ...
def p_selection_statement_3(t):
    """
    selection_statement : SWITCH LPAREN expression RPAREN hold_comments LBRACE labeled_statement_list RBRACE
    """
    ...
def p_labeled_statement_2(t):
    """
    labeled_statement : CASE constant_expression COLON statement_list_opt
    """
    ...
def store_assignment_spillover(token, t): ...
def p_command_expression(t):
    """
    command_expression : CAPTURE command CAPTURE
    """
    ...
def vprint(t, *args): ...
def p_float_constant(t):
    """
    float_constant :     FCONST
    """
    ...
def format_fopen(x, t): ...
def p_logical_or_expression_1(t):
    """
    logical_or_expression : logical_and_expression
    | logical_or_expression LOR logical_and_expression
    """
    ...
def format_held_comments_and_docstring(t, funcname="''"):
    """
    Splits the held comments into the last comment block and the rest of the
    comments, formats the rest of the comments normally, and formats the last
    block as a multiline string.
    
    A comment block is either a group of mel-single-line-comments, or a single
    mel-comment-block
    
    This is useful for grabbing the "last" comment before something, and
    formatting it for use as a docstrign - for instance, if we have, in mel:
    
        // Some section
        // -----------------------
    
        /*
         * docProc: does stuff
         */
        global proc docProc() {};
    
    ...then we only want to grab the last comment block, and format it as
    a docstring... similarly, if the situation is something like this:
    
        /*
         * Some long notes here
         *    batman
         *    superman
         *    king kong
         *    the tick
         */
    
        // proctopus
        // does stuff
        global proc proctopus() {};
    """
    ...
def p_matrix_row_list_2(t):
    """
    matrix_row_list : matrix_row_list SEMI vector_element_list
    """
    ...
def p_statement_required(t):
    """
    statement_required : statement
    """
    ...
def p_postfix_expression_5(t):
    """
    postfix_expression : postfix_expression LBRACKET expression RBRACKET
    """
    ...
def toList(t): ...
def p_command_statement(t):
    """
    command_statement : ID SEMI
    | ID command_statement_input_list SEMI
    """
    ...
def p_iteration_statement_3(t):
    """
    iteration_statement : FOR LPAREN variable IN expression seen_FOR RPAREN hold_comments statement_required
    """
    ...
def p_relational_expression_1(t):
    """
    relational_expression : shift_expression
    | relational_expression LT shift_expression
    | relational_expression GT shift_expression
    | relational_expression LE shift_expression
    | relational_expression GE shift_expression
    """
    ...
def fileInlist(file, fileList): ...
def p_iteration_statement_4(t):
    """
    iteration_statement : DO statement_required WHILE LPAREN expression RPAREN SEMI
    """
    ...
def p_iteration_statement_2(t):
    """
    iteration_statement : FOR LPAREN expression_list_opt SEMI expression_list_opt SEMI expression_list_opt RPAREN hold_comments statement_required
    """
    ...
def p_function_declarator(t):
    """
    function_declarator : GLOBAL PROC
    | PROC
    """
    ...
def format_command(command, args, t): ...
def p_expression_statement(t):
    """
    expression_statement : expression_opt SEMI
    """
    ...
def p_command(t):
    """
    command : ID
    | ID command_input_list
    """
    ...
def p_expression_opt(t):
    """
    expression_opt : empty
    | expression
    """
    ...
def p_unary_expression(t):
    """
    unary_expression : postfix_expression
    | unary_operator cast_expression
    """
    ...
def p_vector_element_list(t):
    """
    vector_element_list : expression
    | vector_element_list COMMA expression
    """
    ...
def hasNonCommentPyCode(pyCode):
    """
    Returns True if the given chunk of python code has any lines that contain
    something other than a comment or whitespace
    """
    ...
def p_function_arg(t):
    """
    function_arg : type_specifier variable
    | type_specifier variable LBRACKET RBRACKET
    """
    ...
def find_num_leading_space(text):
    """
    Given a text block consisting of multiple lines, find the number of
    characters in the longest common whitespace that appears at the start of
    every non-empty line
    """
    ...
def p_compound_statement(t):
    """
    compound_statement   : LBRACE statement_list RBRACE
    | LBRACE RBRACE
    """
    ...
def p_object_list(t):
    """
    object_list : object
    | object_list object
    """
    ...
def p_selection_statement_1(t):
    """
    selection_statement : IF LPAREN expression RPAREN statement_required
    """
    ...
def p_numerical_constant(t):
    """
    numerical_constant : int_constant
    | float_constant
    """
    ...
def pythonizeName(name): ...
def format_tokenize_size(tokenized, sizeVar):
    """
    tokenize fix:
    tokenize passes a buffer by reference, and returns a size.
    we must return a list, and compute the size as a second operation::
    
        int $size = `tokenize "foo bar", $buffer, " "`;
    
        buffer = "foo bar".split(' ')
        size = len(buffer)
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
def p_conditional_expression_1(t):
    """
    conditional_expression : logical_or_expression
    """
    ...
def p_function_arg_list(t):
    """
    function_arg_list : function_arg
    | function_arg_list COMMA function_arg
    """
    ...
def append_comments(t, funcname="''"): ...
def _error(t): ...
def p_postfix_expression_3(t):
    """
    postfix_expression : LVEC vector_element_list RVEC
    """
    ...
def p_variable(t):
    """
    variable : VAR
    """
    ...
def getModuleBasename(script): ...
def p_unary_operator(t):
    """
    unary_operator : PLUS
    | MINUS
    | NOT
    """
    ...
def strip_leading_space(text):
    """
    Given a text block consisting of multiple lines, strip out common
    whitespace that appears at the start of every non-empty line
    """
    ...
def p_multiplicative_expression(t):
    """
    multiplicative_expression : cast_expression
    | multiplicative_expression TIMES cast_expression
    | multiplicative_expression DIVIDE cast_expression
    | multiplicative_expression MOD cast_expression
    | multiplicative_expression CROSS cast_expression
    """
    ...
def p_declaration_statement(t):
    """
    declaration_statement : declaration_specifiers init_declarator_list SEMI
    """
    ...
def p_function_specifiers_opt(t):
    """
    function_specifiers_opt : type_specifier
    | type_specifier LBRACKET RBRACKET
    | empty
    """
    ...
def format_assignment_value(val, typ):
    """
    when assigning a value in mel, values will be auto-cast to the type of the variable, but in python, the variable
    will simply become the new type.  to ensure that the python operates as in mel, we need to cast when assigning
    a value to a variable of a different type
    """
    ...
def p_primary_expression1(t):
    """
    primary_expression :     SCONST
    """
    ...
def format_multiline_string_comment(comments): ...
def format_substring(x, t):
    """
    convert:
        substring( var, 2, (len(var)) )
    to:
        var[1:]
    
    or:
        substring( var, 2, var2 )
    to:
        var[1:var2]
    
    or:
        substring( var, 3, var2 )
    to:
        var[1:var2]
    """
    ...
def p_object_2(t):
    """
    object    : ID LBRACKET expression RBRACKET
    """
    ...
def p_labeled_statement_3(t):
    """
    labeled_statement : DEFAULT COLON statement_list_opt
    """
    ...
def p_expression(t):
    """
    expression : conditional_expression
    """
    ...
def p_translation_unit(t):
    """
    translation_unit : external_declaration
    | translation_unit external_declaration
    """
    ...
def p_function_arg_list_opt(t):
    """
    function_arg_list_opt : function_arg_list
    |  empty
    """
    ...
def p_postfix_expression(t):
    """
    postfix_expression : primary_expression
    | postfix_expression PLUSPLUS
    | postfix_expression MINUSMINUS
    """
    ...
def p_command_input_list(t):
    """
    command_input_list : command_input
    | command_input_list command_input
    """
    ...
def p_boolean_true(t):
    """
    boolean : ON
    | TRUE
    | YES
    """
    ...
def p_declarator_1(t):
    """
    declarator : variable
    """
    ...
def p_logical_and_expression_1(t):
    """
    logical_and_expression : assignment_expression
    | logical_and_expression LAND assignment_expression
    """
    ...
def p_matrix_row_list_1(t):
    """
    matrix_row_list : vector_element_list SEMI vector_element_list
    """
    ...
def p_command_statement_input_list(t):
    """
    command_statement_input_list : command_statement_input
    | command_statement_input_list command_statement_input
    """
    ...
def p_shift_expression(t):
    """
    shift_expression : additive_expression
    """
    ...
def _melObj_to_pyModule(script):
    """
    Return the module name this mel script / procedure will be converted to / found in.
    
    If the mel script is not being translated, returns None.
    """
    ...
def p_seen_func(t):
    """
    seen_func :
    """
    ...
def p_init_declarator(t):
    """
    init_declarator : declarator
    | declarator EQUALS expression
    """
    ...
def p_flag(t):
    """
    command_flag : MINUS ID
    | MINUS BREAK
    | MINUS CASE
    | MINUS CONTINUE
    | MINUS DEFAULT
    | MINUS DO
    | MINUS ELSE
    | MINUS FALSE
    | MINUS FLOAT
    | MINUS FOR
    | MINUS GLOBAL
    | MINUS IF
    | MINUS IN
    | MINUS INT
    | MINUS NO
    | MINUS ON
    | MINUS OFF
    | MINUS PROC
    | MINUS RETURN
    | MINUS STRING
    | MINUS SWITCH
    | MINUS TRUE
    | MINUS VECTOR
    | MINUS WHILE
    | MINUS YES
    """
    ...
def p_unary_expression_2(t):
    """
    unary_expression : PLUSPLUS unary_expression
    | MINUSMINUS unary_expression
    """
    ...
def p_declaration_specifiers(t):
    """
    declaration_specifiers : type_specifier
    | GLOBAL type_specifier
    """
    ...
def p_primary_expression_paren(t):
    """
    primary_expression :    LPAREN expression RPAREN
    """
    ...
def p_expression_list_opt(t):
    """
    expression_list_opt : expression_list
    | empty
    """
    ...
def p_object_1(t):
    """
    object    : ID
    """
    ...
def p_boolean_false(t):
    """
    boolean : OFF
    | FALSE
    | NO
    """
    ...
def p_procedure_expression_list(t):
    """
    procedure_expression_list : constant_expression
    | procedure_expression_list COMMA constant_expression
    """
    ...
def format_fread(x, t): ...
def p_init_declarator_list(t):
    """
    init_declarator_list : init_declarator
    | init_declarator_list COMMA init_declarator
    """
    ...
def p_procedure(t):
    """
    procedure : ID LPAREN procedure_expression_list RPAREN
    | ID LPAREN RPAREN
    """
    ...
def p_conditional_expression_2(t):
    """
    conditional_expression : logical_or_expression CONDOP expression COLON conditional_expression
    """
    ...
def format_held_comments(t, funcname="''"): ...
def p_declarator_2(t):
    """
    declarator :  declarator LBRACKET constant_expression_opt RBRACKET
    """
    ...
def p_error(t): ...
def p_postfix_expression_4(t):
    """
    postfix_expression : LVEC matrix_row_list RVEC
    """
    ...
def p_variable_vector_component(t):
    """
    variable :  VAR COMPONENT
    """
    ...
def p_constant_expression_opt_2(t):
    """
    constant_expression_opt : constant_expression
    """
    ...
def p_equality_expression_1(t):
    """
    equality_expression : relational_expression
    | equality_expression EQ relational_expression
    | equality_expression NE relational_expression
    """
    ...
def p_external_declaration(t):
    """
    external_declaration : statement
    | function_definition
    """
    ...
def p_constant_expression_opt_1(t):
    """
    constant_expression_opt : empty
    """
    ...
def entabLines(line): ...
def p_command_statement_input_3(t):
    """
    command_statement_input     : ELLIPSIS
    """
    ...
def p_cast_expression(t):
    """
    cast_expression : unary_expression
    | unary_command_expression
    | type_specifier LPAREN expression RPAREN
    | LPAREN type_specifier RPAREN cast_expression
    """
    ...
def findModule(moduleName): ...
def p_assignment_operator(t):
    """
    assignment_operator : EQUALS
                        | TIMESEQUAL
                        | DIVEQUAL
                        | MODEQUAL
                        | PLUSEQUAL
                        | MINUSEQUAL
                        | CROSSEQUAL
    """
    ...
def p_jump_statement(t):
    """
    jump_statement : CONTINUE SEMI
    | BREAK SEMI
    | RETURN expression_opt SEMI
    """
    ...
def assemble(t, funcname, separator="''", tokens='None', matchFormatting='False'): ...
def p_command_input_3(t):
    """
    command_input     : ELLIPSIS
    """
    ...
def p_statement_list(t):
    """
    statement_list   : statement
    | statement_list statement
    """
    ...
def p_procedure_expression(t):
    """
    procedure_expression : command_expression
    | procedure
    """
    ...
def p_primary_expression2(t):
    """
    primary_expression :     variable
    """
    ...
def format_tokenize(x, t): ...
def p_command_input_2(t):
    """
    command_input     : object_list
    """
    ...
def p_constant_expression(t):
    """
    constant_expression : conditional_expression
    """
    ...
def format_comments(comments): ...
def p_empty(t):
    """
    empty :
    """
    ...
def p_postfix_expression_2(t):
    """
    postfix_expression : LBRACE expression_list_opt RBRACE
    """
    ...
def p_statement_simple(t):
    """
    statement : expression_statement
    | command_statement
    | compound_statement
    """
    ...
def p_assignment_expression(t):
    """
    assignment_expression : equality_expression
    | postfix_expression assignment_operator assignment_expression
    """
    ...
def p_statement_list_opt(t):
    """
    statement_list_opt : statement_list
    | empty
    """
    ...
def p_command_statement_input(t):
    """
    command_statement_input : unary_expression
    | command_flag
      | command_expression
    """
    ...
def p_function_definition(t):
    """
    function_definition :  function_declarator function_specifiers_opt ID seen_func LPAREN function_arg_list_opt RPAREN hold_comments compound_statement
    """
    ...
def p_statement_complex(t):
    """
    statement : selection_statement
    | iteration_statement
    | jump_statement
    | declaration_statement
    """
    ...
def format_source(x, t): ...
def p_iteration_statement_1(t):
    """
    iteration_statement : WHILE LPAREN expression RPAREN hold_comments statement_required
    """
    ...
def p_command_statement_input_2(t):
    """
    command_statement_input     : object_list
    """
    ...
def p_additive_expression(t):
    """
    additive_expression : multiplicative_expression
    | additive_expression PLUS multiplicative_expression
    | additive_expression MINUS multiplicative_expression
    """
    ...
def _melProc_to_pyModule(t, procedure):
    """
    determine if this procedure has been or will be converted into python, and if so, what module it belongs to
    """
    ...
def p_hold_comments(t):
    """
    hold_comments :
    """
    ...


FLAG_RE : _sre.SRE_Pattern
proc_remap : dict
default_values : dict
scanner : yacc.LRParser
melCmdFlagList : dict
_outputdir : str
NON_COMMENT_LINE_RE : _sre.SRE_Pattern
tokens : tuple
tag : str
reserved : set
mel_type_to_python_type : dict
melCmdList : list
filteredCmds : list
pythonReservedWords : list
parser : yacc.LRParser
batchData : BatchData
lexer : lex.Lexer
x : str

