from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
from . import ServerRegistryMMap
import maya
import socket
import SocketServer
import StringIO
import maya.mel as mel
import maya.utils as utils
import maya.OpenMaya as OpenMaya
import maya.cmds as cmds
import Queue
import threading
import select
import os
import time
import operator
import cPickle as PICKLE
import types


"""
Input to a commandPort is handled by a subclass of TcommandHandler.
Customization of commandPort behaviour could be achieved by implementing
a custom subclass, and adding it to commandHandlersMap, with a string key
which will be passed to openCommandPort as the 'lang' parameter.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class TcommandServer(threading.Thread):
    """
    Server for one command port
    
    socketServer - StreamServer instance to serve on endpoint
    """
    
    
    
    def __del__(self): ...
    def __init__(self, socketServer): ...
    def run(self):
        """
        called by Thread.start
        """
        ...
    def shutdown(self):
        """
        tell the server to shutdown
        """
        ...


class TcommandHandler(SocketServer.StreamRequestHandler):
    """
    StreamRequestHandler base class for command ports, 
    handle() is called when client connects.  Subclasses must implement _languageExecute.
    """
    
    
    
    def handle(self): ...
    def postSecurityWarning(self): ...
    def receiveData(self): ...
    resp_term : str


class TpythonCommandHandler(TcommandHandler):
    """
    The StreamRequestHandler instance for deferred python execution
    """
    
    
    
    pass


class TMELCommandHandler(TcommandHandler):
    """
    The StreamRequestHandler for deferred MEL execution
    """
    
    
    
    pass




def closeCommandPort(portName):
    """
    Close the specified command port  
    Returns error string on failure, None on success
    """
    ...
def openCommandPort(portName, lang, prefix, sendResults, returnNbCommands, echoOutput, bufferSize, securityWarning, pickleOutput='False'):
    """
    Open a command port with the given name
    Can be INET (host:port) or Unix local format (/tmp/endpoint)
    
    On Windows, the Unix-style format will create a named binary file-mapping
    object which contains a mapping of paths to ports.
    On Mac and Linux a Unix-domain socket will be created.
    
    Environment variable MAYA_IP_TYPE can be used to override the default 
    address family by setting it to either 'ipv4' or 'ipv6'.  The default is 
    ipv4.
    
    portname         - name of the port, used to refer to the server, follows 
                       above format
    lang             - must be a key in commandHandlersMap eg 'mel' or 'python'
    prefix           - string prefix to prepend to every command
    sendResults      - True means send results of commands back to client
    returnNbCommands - True means return the number of commands executed
    echoOutput       - True means echo command output to the port
    bufferSize       - byte size of the socket buffer
    securityWarning  - True means issue a security warning
    pickleOutput     - True means the return string for python command would be
                       pickled, default is false
    Returns error string on failure, None on success
    """
    ...
def commandOutputCallback(message, msgType, qu): ...
def listCommandPorts():
    """
    Returns the list of command port names, in the format used by openCommandPort
    """
    ...


mayaEncoding : unicode
commandHandlersMap : dict
__commandPorts : dict
transformEncoding : str
EINTR : int

