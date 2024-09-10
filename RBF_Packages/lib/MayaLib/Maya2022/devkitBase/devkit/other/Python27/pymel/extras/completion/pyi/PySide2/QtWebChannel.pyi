from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import QtCore


from PySide2.QtCore import QObject as _QObject


if False:
    from typing import Dict, List, Tuple, Union, Optional

class QWebChannel(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def blockUpdates(*args, **kwargs): ...
    def connectTo(*args, **kwargs): ...
    def deregisterObject(*args, **kwargs): ...
    def disconnectFrom(*args, **kwargs): ...
    def registerObject(*args, **kwargs): ...
    def registerObjects(*args, **kwargs): ...
    def registeredObjects(*args, **kwargs): ...
    def setBlockUpdates(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    blockUpdatesChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QWebChannelAbstractTransport(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def sendMessage(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    messageReceived : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject



