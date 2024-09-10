from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import pymel.util as util
import maya
import sys
import logging
import os
import ConfigParser


from logging import warning
from logging import warning as warn
from logging import log
from logging import LoggerAdapter
from logging import getLevelName
from logging import info
from logging import Formatter
from logging import Logger
from logging import disable
from logging import Handler
from logging import critical
from logging import critical as fatal
from logging import basicConfig
from maya.OpenMaya import MMessage
from maya.OpenMaya import MGlobal
from logging import getLoggerClass
from logging import captureWarnings
from maya.OpenMaya import MEventMessage
from logging import BufferingFormatter
from pymel.util.decoration import decorator
from logging import error
from logging import addLevelName
from logging import NullHandler
from logging import Filter
from logging import debug
from logging import StreamHandler
from logging import setLoggerClass
from logging import exception
from logging import FileHandler
from logging import makeLogRecord
from logging import LogRecord


if False:
    from typing import Dict, List, Tuple, Union, Optional

def getLogConfigFile(): ...
def _setupLevelPreferenceHook():
    """
    Sets up a callback so that the last used log-level is saved to the user preferences file
    """
    ...
def pymelLogFileConfig(fname, defaults='None', disable_existing_loggers='False'):
    """
    Reads in a file to set up pymel's loggers.
    
    In most respects, this function behaves similarly to logging.config.fileConfig -
    consult it's help for details. In particular, the format of the config file
    must meet the same requirements - it must have the sections [loggers],
    [handlers], and [fomatters], and it must have an entry for [logger_root]...
    even if not options are set for it.
    
    It differs from logging.config.fileConfig in the following ways:
    
    1) It will not disable any pre-existing loggers which are not specified in
    the config file, unless disable_existing_loggers is set to True.
    
    2) Like logging.config.fileConfig, the default behavior for pre-existing
    handlers on any loggers whose settings are specified in the config file is
    to remove them; ie, ONLY the handlers explicitly given in the config will
    be on the configured logger.
    However, pymelLogFileConfig provides the ability to keep pre-exisiting
    handlers, by setting the 'remove_existing_handlers' option in the appropriate
    section to True.
    """
    ...
def getLogger(name):
    """
    a convenience function that allows any module to setup a logger by simply
    calling `getLogger(__name__)`.  If the module is a package, "__init__" will
    be stripped from the logger name
    """
    ...
def _fixMayaOutput(): ...
def levelToName(level): ...
def _addOldHandlers(logger, oldHandlers, secName, configParser): ...
def nameToLevel(name): ...
def raiseLog(logger, level, message, errorClass='"<type \'exceptions.RuntimeError\'>"'):
    """
    For use in situations in which you may wish to raise an error or simply
    print to a logger.
    
    Ie, if checking for things that "shouldn't" happen, may want to raise an
    error if a developer, but simply issue a warning and continue as gracefully
    as possible for normal end-users.
    
    So, if we make a call:
        raiseLog(_logger, _logger.INFO, "oh noes! something weird happened!")
    ...then what happens will depend on what the value of ERRORLEVEL (controlled
    by the environment var %s) is - if it was not set, or set to ERROR, or
    WARNING, then the call will result in issuing a _logger.info(...) call;
    if it was set to INFO or DEBUG, then an error would be raised.
    
    For convenience, raiseLog is installed onto logger instances created with
    the getLogger function in this module, so you can do:
        _logger.raiseLog(_logger.INFO, "oh noes! something weird happened!")
    """
    ...
def addErrorLog(logger):
    """
    Adds an 'raiseLog' method to the given logger instance
    """
    ...
def getConfigFile(): ...
def environLogLevelOverride(logger):
    """
    If PYMEL_LOGLEVEL is set, make sure the logging level is at least that
    much for the given logger.
    """
    ...
def timed(level='10'): ...


WARN : int
root : logging.RootLogger
PYMEL_CONF_ENV_VAR : str
ERROR : int
ERRORLEVEL : NoneType
PYMEL_ERRORLEVEL_ENV_VAR : str
PYMEL_LOGLEVEL_ENV_VAR : str
pymelLogger : Logger
NOTSET : int
logLevels : util.Enum
DEBUG : int
CRITICAL : int
INFO : int
BASIC_FORMAT : str

