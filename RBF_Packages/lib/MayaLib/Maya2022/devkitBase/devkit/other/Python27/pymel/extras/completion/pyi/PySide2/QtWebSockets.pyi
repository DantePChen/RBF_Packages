from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import QtCore


from PySide2.QtCore import QObject as _QObject


if False:
    from typing import Dict, List, Tuple, Union, Optional

class _Object(object):
    __dict__ : dictproxy


class QMaskGenerator(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def nextMask(*args, **kwargs): ...
    def seed(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    destroyed : PySide2.QtCore.Signal
    
    objectNameChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QWebSocketCorsAuthenticator(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def allowed(*args, **kwargs): ...
    def origin(*args, **kwargs): ...
    def setAllowed(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QWebSocketProtocol(_Object):
    CloseCode : Type[CloseCode]
    
    CloseCodeAbnormalDisconnection : CloseCode
    
    CloseCodeBadOperation : CloseCode
    
    CloseCodeDatatypeNotSupported : CloseCode
    
    CloseCodeGoingAway : CloseCode
    
    CloseCodeMissingExtension : CloseCode
    
    CloseCodeMissingStatusCode : CloseCode
    
    CloseCodeNormal : CloseCode
    
    CloseCodePolicyViolated : CloseCode
    
    CloseCodeProtocolError : CloseCode
    
    CloseCodeReserved1004 : CloseCode
    
    CloseCodeTlsHandshakeFailed : CloseCode
    
    CloseCodeTooMuchData : CloseCode
    
    CloseCodeWrongDatatype : CloseCode
    
    Version : Type[Version]
    
    Version0 : Version
    
    Version13 : Version
    
    Version4 : Version
    
    Version5 : Version
    
    Version6 : Version
    
    Version7 : Version
    
    Version8 : Version
    
    VersionLatest : Version
    
    VersionUnknown : Version


class QWebSocket(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def abort(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def closeCode(*args, **kwargs): ...
    def closeReason(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def flush(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def localAddress(*args, **kwargs): ...
    def localPort(*args, **kwargs): ...
    def maskGenerator(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def origin(*args, **kwargs): ...
    def pauseMode(*args, **kwargs): ...
    def peerAddress(*args, **kwargs): ...
    def peerName(*args, **kwargs): ...
    def peerPort(*args, **kwargs): ...
    def ping(*args, **kwargs): ...
    def proxy(*args, **kwargs): ...
    def readBufferSize(*args, **kwargs): ...
    def request(*args, **kwargs): ...
    def requestUrl(*args, **kwargs): ...
    def resourceName(*args, **kwargs): ...
    def resume(*args, **kwargs): ...
    def sendBinaryMessage(*args, **kwargs): ...
    def sendTextMessage(*args, **kwargs): ...
    def setMaskGenerator(*args, **kwargs): ...
    def setPauseMode(*args, **kwargs): ...
    def setProxy(*args, **kwargs): ...
    def setReadBufferSize(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def version(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    aboutToClose : PySide2.QtCore.Signal
    
    binaryFrameReceived : PySide2.QtCore.Signal
    
    binaryMessageReceived : PySide2.QtCore.Signal
    
    bytesWritten : PySide2.QtCore.Signal
    
    connected : PySide2.QtCore.Signal
    
    disconnected : PySide2.QtCore.Signal
    
    error : PySide2.QtCore.Signal
    
    pong : PySide2.QtCore.Signal
    
    proxyAuthenticationRequired : PySide2.QtCore.Signal
    
    readChannelFinished : PySide2.QtCore.Signal
    
    sslErrors : PySide2.QtCore.Signal
    
    stateChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject
    
    textFrameReceived : PySide2.QtCore.Signal
    
    textMessageReceived : PySide2.QtCore.Signal


class QWebSocketServer(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def close(*args, **kwargs): ...
    def error(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def hasPendingConnections(*args, **kwargs): ...
    def isListening(*args, **kwargs): ...
    def listen(*args, **kwargs): ...
    def maxPendingConnections(*args, **kwargs): ...
    def nextPendingConnection(*args, **kwargs): ...
    def pauseAccepting(*args, **kwargs): ...
    def proxy(*args, **kwargs): ...
    def resumeAccepting(*args, **kwargs): ...
    def secureMode(*args, **kwargs): ...
    def serverAddress(*args, **kwargs): ...
    def serverName(*args, **kwargs): ...
    def serverPort(*args, **kwargs): ...
    def serverUrl(*args, **kwargs): ...
    def setMaxPendingConnections(*args, **kwargs): ...
    def setProxy(*args, **kwargs): ...
    def setServerName(*args, **kwargs): ...
    def setSocketDescriptor(*args, **kwargs): ...
    def socketDescriptor(*args, **kwargs): ...
    def supportedVersions(*args, **kwargs): ...
    NonSecureMode : SslMode
    
    SecureMode : SslMode
    
    SslMode : Type[SslMode]
    
    __new__ : builtin_function_or_method
    
    acceptError : PySide2.QtCore.Signal
    
    closed : PySide2.QtCore.Signal
    
    newConnection : PySide2.QtCore.Signal
    
    originAuthenticationRequired : PySide2.QtCore.Signal
    
    peerVerifyError : PySide2.QtCore.Signal
    
    serverError : PySide2.QtCore.Signal
    
    sslErrors : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject



