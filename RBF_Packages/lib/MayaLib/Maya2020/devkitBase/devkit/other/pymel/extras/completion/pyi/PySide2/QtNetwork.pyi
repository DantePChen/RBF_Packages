from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import QtCore


from PySide2.QtCore import QObject as _QObject
from PySide2.QtCore import QIODevice as _QIODevice


if False:
    from typing import Dict, List, Tuple, Union, Optional

class _Object(object):
    __dict__ : dictproxy


class QSsl(_Object):
    AlternativeNameEntryType : Type[AlternativeNameEntryType]
    
    AnyProtocol : SslProtocol
    
    Der : EncodingFormat
    
    DnsEntry : AlternativeNameEntryType
    
    Dsa : KeyAlgorithm
    
    Ec : KeyAlgorithm
    
    EmailEntry : AlternativeNameEntryType
    
    EncodingFormat : Type[EncodingFormat]
    
    KeyAlgorithm : Type[KeyAlgorithm]
    
    KeyType : Type[KeyType]
    
    Opaque : KeyAlgorithm
    
    Pem : EncodingFormat
    
    PrivateKey : KeyType
    
    PublicKey : KeyType
    
    Rsa : KeyAlgorithm
    
    SecureProtocols : SslProtocol
    
    SslOption : Type[SslOption]
    
    SslOptionDisableCompression : SslOption
    
    SslOptionDisableEmptyFragments : SslOption
    
    SslOptionDisableLegacyRenegotiation : SslOption
    
    SslOptionDisableServerCipherPreference : SslOption
    
    SslOptionDisableServerNameIndication : SslOption
    
    SslOptionDisableSessionPersistence : SslOption
    
    SslOptionDisableSessionSharing : SslOption
    
    SslOptionDisableSessionTickets : SslOption
    
    SslOptions : Type[SslOptions]
    
    SslProtocol : Type[SslProtocol]
    
    SslV2 : SslProtocol
    
    SslV3 : SslProtocol
    
    TlsV1SslV3 : SslProtocol
    
    TlsV1_0 : SslProtocol
    
    TlsV1_0OrLater : SslProtocol
    
    TlsV1_1 : SslProtocol
    
    TlsV1_1OrLater : SslProtocol
    
    TlsV1_2 : SslProtocol
    
    TlsV1_2OrLater : SslProtocol
    
    UnknownProtocol : SslProtocol


class QNetworkCookie(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def domain(*args, **kwargs): ...
    def expirationDate(*args, **kwargs): ...
    def hasSameIdentifier(*args, **kwargs): ...
    def isHttpOnly(*args, **kwargs): ...
    def isSecure(*args, **kwargs): ...
    def isSessionCookie(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def normalize(*args, **kwargs): ...
    def path(*args, **kwargs): ...
    def setDomain(*args, **kwargs): ...
    def setExpirationDate(*args, **kwargs): ...
    def setHttpOnly(*args, **kwargs): ...
    def setName(*args, **kwargs): ...
    def setPath(*args, **kwargs): ...
    def setSecure(*args, **kwargs): ...
    def setValue(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def toRawForm(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    @staticmethod
    def parseCookies(*args, **kwargs): ...
    Full : RawForm
    
    NameAndValueOnly : RawForm
    
    RawForm : Type[RawForm]
    
    __new__ : builtin_function_or_method


class QNetworkRequest(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def attribute(*args, **kwargs): ...
    def hasRawHeader(*args, **kwargs): ...
    def header(*args, **kwargs): ...
    def maximumRedirectsAllowed(*args, **kwargs): ...
    def originatingObject(*args, **kwargs): ...
    def priority(*args, **kwargs): ...
    def rawHeader(*args, **kwargs): ...
    def rawHeaderList(*args, **kwargs): ...
    def setAttribute(*args, **kwargs): ...
    def setHeader(*args, **kwargs): ...
    def setMaximumRedirectsAllowed(*args, **kwargs): ...
    def setOriginatingObject(*args, **kwargs): ...
    def setPriority(*args, **kwargs): ...
    def setRawHeader(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    AlwaysCache : CacheLoadControl
    
    AlwaysNetwork : CacheLoadControl
    
    Attribute : Type[Attribute]
    
    AuthenticationReuseAttribute : Attribute
    
    Automatic : LoadControl
    
    BackgroundRequestAttribute : Attribute
    
    CacheLoadControl : Type[CacheLoadControl]
    
    CacheLoadControlAttribute : Attribute
    
    CacheSaveControlAttribute : Attribute
    
    ConnectionEncryptedAttribute : Attribute
    
    ContentDispositionHeader : KnownHeaders
    
    ContentLengthHeader : KnownHeaders
    
    ContentTypeHeader : KnownHeaders
    
    CookieHeader : KnownHeaders
    
    CookieLoadControlAttribute : Attribute
    
    CookieSaveControlAttribute : Attribute
    
    CustomVerbAttribute : Attribute
    
    DoNotBufferUploadDataAttribute : Attribute
    
    DownloadBufferAttribute : Attribute
    
    EmitAllUploadProgressSignalsAttribute : Attribute
    
    FollowRedirectsAttribute : Attribute
    
    HighPriority : Priority
    
    HttpPipeliningAllowedAttribute : Attribute
    
    HttpPipeliningWasUsedAttribute : Attribute
    
    HttpReasonPhraseAttribute : Attribute
    
    HttpStatusCodeAttribute : Attribute
    
    KnownHeaders : Type[KnownHeaders]
    
    LastModifiedHeader : KnownHeaders
    
    LoadControl : Type[LoadControl]
    
    LocationHeader : KnownHeaders
    
    LowPriority : Priority
    
    Manual : LoadControl
    
    MaximumDownloadBufferSizeAttribute : Attribute
    
    NormalPriority : Priority
    
    PreferCache : CacheLoadControl
    
    PreferNetwork : CacheLoadControl
    
    Priority : Type[Priority]
    
    RedirectionTargetAttribute : Attribute
    
    ServerHeader : KnownHeaders
    
    SetCookieHeader : KnownHeaders
    
    SourceIsFromCacheAttribute : Attribute
    
    SpdyAllowedAttribute : Attribute
    
    SpdyWasUsedAttribute : Attribute
    
    SynchronousRequestAttribute : Attribute
    
    User : Attribute
    
    UserAgentHeader : KnownHeaders
    
    UserMax : Attribute
    
    __new__ : builtin_function_or_method


class QNetworkConfigurationManager(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def allConfigurations(*args, **kwargs): ...
    def capabilities(*args, **kwargs): ...
    def configurationFromIdentifier(*args, **kwargs): ...
    def defaultConfiguration(*args, **kwargs): ...
    def isOnline(*args, **kwargs): ...
    def updateConfigurations(*args, **kwargs): ...
    ApplicationLevelRoaming : Capability
    
    CanStartAndStopInterfaces : Capability
    
    Capabilities : Type[Capabilities]
    
    Capability : Type[Capability]
    
    DataStatistics : Capability
    
    DirectConnectionRouting : Capability
    
    ForcedRoaming : Capability
    
    NetworkSessionRequired : Capability
    
    SystemSessionSupport : Capability
    
    __new__ : builtin_function_or_method
    
    configurationAdded : PySide2.QtCore.Signal
    
    configurationChanged : PySide2.QtCore.Signal
    
    configurationRemoved : PySide2.QtCore.Signal
    
    onlineStateChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject
    
    updateCompleted : PySide2.QtCore.Signal


class QNetworkProxyQuery(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def localPort(*args, **kwargs): ...
    def networkConfiguration(*args, **kwargs): ...
    def peerHostName(*args, **kwargs): ...
    def peerPort(*args, **kwargs): ...
    def protocolTag(*args, **kwargs): ...
    def queryType(*args, **kwargs): ...
    def setLocalPort(*args, **kwargs): ...
    def setNetworkConfiguration(*args, **kwargs): ...
    def setPeerHostName(*args, **kwargs): ...
    def setPeerPort(*args, **kwargs): ...
    def setProtocolTag(*args, **kwargs): ...
    def setQueryType(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    QueryType : Type[QueryType]
    
    TcpServer : QueryType
    
    TcpSocket : QueryType
    
    UdpSocket : QueryType
    
    UrlRequest : QueryType
    
    __new__ : builtin_function_or_method


class QNetworkSession(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def accept(*args, **kwargs): ...
    def activeTime(*args, **kwargs): ...
    def bytesReceived(*args, **kwargs): ...
    def bytesWritten(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def configuration(*args, **kwargs): ...
    def connectNotify(*args, **kwargs): ...
    def disconnectNotify(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def ignore(*args, **kwargs): ...
    def interface(*args, **kwargs): ...
    def isOpen(*args, **kwargs): ...
    def migrate(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def reject(*args, **kwargs): ...
    def sessionProperty(*args, **kwargs): ...
    def setSessionProperty(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    def usagePolicies(*args, **kwargs): ...
    def waitForOpened(*args, **kwargs): ...
    Closing : State
    
    Connected : State
    
    Connecting : State
    
    Disconnected : State
    
    Invalid : State
    
    InvalidConfigurationError : SessionError
    
    NoBackgroundTrafficPolicy : UsagePolicy
    
    NoPolicy : UsagePolicy
    
    NotAvailable : State
    
    OperationNotSupportedError : SessionError
    
    Roaming : State
    
    RoamingError : SessionError
    
    SessionAbortedError : SessionError
    
    SessionError : Type[SessionError]
    
    State : Type[State]
    
    UnknownSessionError : SessionError
    
    UsagePolicies : Type[UsagePolicies]
    
    UsagePolicy : Type[UsagePolicy]
    
    __new__ : builtin_function_or_method
    
    closed : PySide2.QtCore.Signal
    
    error : PySide2.QtCore.Signal
    
    newConfigurationActivated : PySide2.QtCore.Signal
    
    opened : PySide2.QtCore.Signal
    
    preferredConfigurationChanged : PySide2.QtCore.Signal
    
    stateChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject
    
    usagePoliciesChanged : PySide2.QtCore.Signal


class QHostInfo(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addresses(*args, **kwargs): ...
    def error(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def hostName(*args, **kwargs): ...
    def lookupId(*args, **kwargs): ...
    def setAddresses(*args, **kwargs): ...
    def setError(*args, **kwargs): ...
    def setErrorString(*args, **kwargs): ...
    def setHostName(*args, **kwargs): ...
    def setLookupId(*args, **kwargs): ...
    @staticmethod
    def abortHostLookup(*args, **kwargs): ...
    @staticmethod
    def fromName(*args, **kwargs): ...
    @staticmethod
    def localDomainName(*args, **kwargs): ...
    @staticmethod
    def localHostName(*args, **kwargs): ...
    HostInfoError : Type[HostInfoError]
    
    HostNotFound : HostInfoError
    
    NoError : HostInfoError
    
    UnknownError : HostInfoError
    
    __new__ : builtin_function_or_method


class QNetworkConfiguration(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def bearerType(*args, **kwargs): ...
    def bearerTypeFamily(*args, **kwargs): ...
    def bearerTypeName(*args, **kwargs): ...
    def children(*args, **kwargs): ...
    def identifier(*args, **kwargs): ...
    def isRoamingAvailable(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def purpose(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    Active : StateFlag
    
    Bearer2G : BearerType
    
    Bearer3G : BearerType
    
    Bearer4G : BearerType
    
    BearerBluetooth : BearerType
    
    BearerCDMA2000 : BearerType
    
    BearerEVDO : BearerType
    
    BearerEthernet : BearerType
    
    BearerHSPA : BearerType
    
    BearerLTE : BearerType
    
    BearerType : Type[BearerType]
    
    BearerUnknown : BearerType
    
    BearerWCDMA : BearerType
    
    BearerWLAN : BearerType
    
    BearerWiMAX : BearerType
    
    Defined : StateFlag
    
    Discovered : StateFlag
    
    InternetAccessPoint : Type
    
    Invalid : Type
    
    PrivatePurpose : Purpose
    
    PublicPurpose : Purpose
    
    Purpose : Type[Purpose]
    
    ServiceNetwork : Type
    
    ServiceSpecificPurpose : Purpose
    
    StateFlag : Type[StateFlag]
    
    StateFlags : Type[StateFlags]
    
    Type : Type[Type]
    
    Undefined : StateFlag
    
    UnknownPurpose : Purpose
    
    UserChoice : Type
    
    __new__ : builtin_function_or_method


class QNetworkCacheMetaData(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lshift__(*args, **kwargs):
        """
        x.__lshift__(y) <==> x<<y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __rlshift__(*args, **kwargs):
        """
        x.__rlshift__(y) <==> y<<x
        """
        ...
    def __rrshift__(*args, **kwargs):
        """
        x.__rrshift__(y) <==> y>>x
        """
        ...
    def __rshift__(*args, **kwargs):
        """
        x.__rshift__(y) <==> x>>y
        """
        ...
    def attributes(*args, **kwargs): ...
    def expirationDate(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def lastModified(*args, **kwargs): ...
    def rawHeaders(*args, **kwargs): ...
    def saveToDisk(*args, **kwargs): ...
    def setAttributes(*args, **kwargs): ...
    def setExpirationDate(*args, **kwargs): ...
    def setLastModified(*args, **kwargs): ...
    def setRawHeaders(*args, **kwargs): ...
    def setSaveToDisk(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QNetworkProxyFactory(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def queryProxy(*args, **kwargs): ...
    @staticmethod
    def proxyForQuery(*args, **kwargs): ...
    @staticmethod
    def setApplicationProxyFactory(*args, **kwargs): ...
    @staticmethod
    def setUseSystemConfiguration(*args, **kwargs): ...
    @staticmethod
    def systemProxyForQuery(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QHostAddress(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __hash__(*args, **kwargs):
        """
        x.__hash__() <==> hash(x)
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lshift__(*args, **kwargs):
        """
        x.__lshift__(y) <==> x<<y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __nonzero__(self) -> bool:
        """
        x.__nonzero__() <==> x != 0
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rlshift__(*args, **kwargs):
        """
        x.__rlshift__(y) <==> y<<x
        """
        ...
    def __rrshift__(*args, **kwargs):
        """
        x.__rrshift__(y) <==> y>>x
        """
        ...
    def __rshift__(*args, **kwargs):
        """
        x.__rshift__(y) <==> x>>y
        """
        ...
    def clear(*args, **kwargs): ...
    def isInSubnet(*args, **kwargs): ...
    def isLoopback(*args, **kwargs): ...
    def isMulticast(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def protocol(*args, **kwargs): ...
    def scopeId(*args, **kwargs): ...
    def setAddress(*args, **kwargs): ...
    def setScopeId(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def toIPv4Address(*args, **kwargs): ...
    def toIPv6Address(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    @staticmethod
    def parseSubnet(*args, **kwargs): ...
    Any : SpecialAddress
    
    AnyIPv4 : SpecialAddress
    
    AnyIPv6 : SpecialAddress
    
    Broadcast : SpecialAddress
    
    LocalHost : SpecialAddress
    
    LocalHostIPv6 : SpecialAddress
    
    Null : SpecialAddress
    
    SpecialAddress : Type[SpecialAddress]
    
    __new__ : builtin_function_or_method


class QNetworkReply(_QIODevice):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def abort(*args, **kwargs): ...
    def attribute(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def hasRawHeader(*args, **kwargs): ...
    def header(*args, **kwargs): ...
    def ignoreSslErrors(*args, **kwargs): ...
    def isFinished(*args, **kwargs): ...
    def isRunning(*args, **kwargs): ...
    def isSequential(*args, **kwargs): ...
    def manager(*args, **kwargs): ...
    def operation(*args, **kwargs): ...
    def rawHeader(*args, **kwargs): ...
    def rawHeaderList(*args, **kwargs): ...
    def rawHeaderPairs(*args, **kwargs): ...
    def readBufferSize(*args, **kwargs): ...
    def request(*args, **kwargs): ...
    def setAttribute(*args, **kwargs): ...
    def setError(*args, **kwargs): ...
    def setFinished(*args, **kwargs): ...
    def setHeader(*args, **kwargs): ...
    def setOperation(*args, **kwargs): ...
    def setRawHeader(*args, **kwargs): ...
    def setReadBufferSize(*args, **kwargs): ...
    def setRequest(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    AuthenticationRequiredError : NetworkError
    
    BackgroundRequestNotAllowedError : NetworkError
    
    ConnectionRefusedError : NetworkError
    
    ContentAccessDenied : NetworkError
    
    ContentConflictError : NetworkError
    
    ContentGoneError : NetworkError
    
    ContentNotFoundError : NetworkError
    
    ContentOperationNotPermittedError : NetworkError
    
    ContentReSendError : NetworkError
    
    HostNotFoundError : NetworkError
    
    InsecureRedirectError : NetworkError
    
    InternalServerError : NetworkError
    
    NetworkError : Type[NetworkError]
    
    NetworkSessionFailedError : NetworkError
    
    NoError : NetworkError
    
    OperationCanceledError : NetworkError
    
    OperationNotImplementedError : NetworkError
    
    ProtocolFailure : NetworkError
    
    ProtocolInvalidOperationError : NetworkError
    
    ProtocolUnknownError : NetworkError
    
    ProxyAuthenticationRequiredError : NetworkError
    
    ProxyConnectionClosedError : NetworkError
    
    ProxyConnectionRefusedError : NetworkError
    
    ProxyNotFoundError : NetworkError
    
    ProxyTimeoutError : NetworkError
    
    RemoteHostClosedError : NetworkError
    
    ServiceUnavailableError : NetworkError
    
    SslHandshakeFailedError : NetworkError
    
    TemporaryNetworkFailureError : NetworkError
    
    TimeoutError : NetworkError
    
    TooManyRedirectsError : NetworkError
    
    UnknownContentError : NetworkError
    
    UnknownNetworkError : NetworkError
    
    UnknownProxyError : NetworkError
    
    UnknownServerError : NetworkError
    
    __new__ : builtin_function_or_method
    
    downloadProgress : PySide2.QtCore.Signal
    
    encrypted : PySide2.QtCore.Signal
    
    error : PySide2.QtCore.Signal
    
    finished : PySide2.QtCore.Signal
    
    metaDataChanged : PySide2.QtCore.Signal
    
    preSharedKeyAuthenticationRequired : PySide2.QtCore.Signal
    
    redirected : PySide2.QtCore.Signal
    
    sslErrors : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject
    
    uploadProgress : PySide2.QtCore.Signal


class QNetworkInterface(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def addressEntries(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def hardwareAddress(*args, **kwargs): ...
    def humanReadableName(*args, **kwargs): ...
    def index(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    @staticmethod
    def allAddresses(*args, **kwargs): ...
    @staticmethod
    def allInterfaces(*args, **kwargs): ...
    @staticmethod
    def interfaceFromIndex(*args, **kwargs): ...
    @staticmethod
    def interfaceFromName(*args, **kwargs): ...
    CanBroadcast : InterfaceFlag
    
    CanMulticast : InterfaceFlag
    
    InterfaceFlag : Type[InterfaceFlag]
    
    InterfaceFlags : Type[InterfaceFlags]
    
    IsLoopBack : InterfaceFlag
    
    IsPointToPoint : InterfaceFlag
    
    IsRunning : InterfaceFlag
    
    IsUp : InterfaceFlag
    
    __new__ : builtin_function_or_method


class QNetworkAddressEntry(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def broadcast(*args, **kwargs): ...
    def ip(*args, **kwargs): ...
    def netmask(*args, **kwargs): ...
    def prefixLength(*args, **kwargs): ...
    def setBroadcast(*args, **kwargs): ...
    def setIp(*args, **kwargs): ...
    def setNetmask(*args, **kwargs): ...
    def setPrefixLength(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QAuthenticator(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __nonzero__(self) -> bool:
        """
        x.__nonzero__() <==> x != 0
        """
        ...
    def isNull(*args, **kwargs): ...
    def option(*args, **kwargs): ...
    def options(*args, **kwargs): ...
    def password(*args, **kwargs): ...
    def realm(*args, **kwargs): ...
    def setOption(*args, **kwargs): ...
    def setPassword(*args, **kwargs): ...
    def setRealm(*args, **kwargs): ...
    def setUser(*args, **kwargs): ...
    def user(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QNetworkProxy(_Object):
    def __copy__(*args, **kwargs): ...
    def __eq__(self, other: Any) -> bool:
        """
        x.__eq__(y) <==> x==y
        """
        ...
    def __ge__(self, other: Any) -> bool:
        """
        x.__ge__(y) <==> x>=y
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def capabilities(*args, **kwargs): ...
    def hasRawHeader(*args, **kwargs): ...
    def header(*args, **kwargs): ...
    def hostName(*args, **kwargs): ...
    def isCachingProxy(*args, **kwargs): ...
    def isTransparentProxy(*args, **kwargs): ...
    def password(*args, **kwargs): ...
    def port(*args, **kwargs): ...
    def rawHeader(*args, **kwargs): ...
    def rawHeaderList(*args, **kwargs): ...
    def setCapabilities(*args, **kwargs): ...
    def setHeader(*args, **kwargs): ...
    def setHostName(*args, **kwargs): ...
    def setPassword(*args, **kwargs): ...
    def setPort(*args, **kwargs): ...
    def setRawHeader(*args, **kwargs): ...
    def setType(*args, **kwargs): ...
    def setUser(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    def user(*args, **kwargs): ...
    @staticmethod
    def applicationProxy(*args, **kwargs): ...
    @staticmethod
    def setApplicationProxy(*args, **kwargs): ...
    CachingCapability : Capability
    
    Capabilities : Type[Capabilities]
    
    Capability : Type[Capability]
    
    DefaultProxy : ProxyType
    
    FtpCachingProxy : ProxyType
    
    HostNameLookupCapability : Capability
    
    HttpCachingProxy : ProxyType
    
    HttpProxy : ProxyType
    
    ListeningCapability : Capability
    
    NoProxy : ProxyType
    
    ProxyType : Type[ProxyType]
    
    Socks5Proxy : ProxyType
    
    TunnelingCapability : Capability
    
    UdpTunnelingCapability : Capability
    
    __new__ : builtin_function_or_method


class QAbstractSocket(_QIODevice):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def abort(*args, **kwargs): ...
    def atEnd(*args, **kwargs): ...
    def bind(*args, **kwargs): ...
    def bytesAvailable(*args, **kwargs): ...
    def bytesToWrite(*args, **kwargs): ...
    def canReadLine(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def connectToHost(*args, **kwargs): ...
    def disconnectFromHost(*args, **kwargs): ...
    def flush(*args, **kwargs): ...
    def isSequential(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def localAddress(*args, **kwargs): ...
    def localPort(*args, **kwargs): ...
    def pauseMode(*args, **kwargs): ...
    def peerAddress(*args, **kwargs): ...
    def peerName(*args, **kwargs): ...
    def peerPort(*args, **kwargs): ...
    def proxy(*args, **kwargs): ...
    def readBufferSize(*args, **kwargs): ...
    def readData(*args, **kwargs): ...
    def readLineData(*args, **kwargs): ...
    def resume(*args, **kwargs): ...
    def setLocalAddress(*args, **kwargs): ...
    def setLocalPort(*args, **kwargs): ...
    def setPauseMode(*args, **kwargs): ...
    def setPeerAddress(*args, **kwargs): ...
    def setPeerName(*args, **kwargs): ...
    def setPeerPort(*args, **kwargs): ...
    def setProxy(*args, **kwargs): ...
    def setReadBufferSize(*args, **kwargs): ...
    def setSocketDescriptor(*args, **kwargs): ...
    def setSocketError(*args, **kwargs): ...
    def setSocketOption(*args, **kwargs): ...
    def setSocketState(*args, **kwargs): ...
    def socketDescriptor(*args, **kwargs): ...
    def socketOption(*args, **kwargs): ...
    def socketType(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def waitForBytesWritten(*args, **kwargs): ...
    def waitForConnected(*args, **kwargs): ...
    def waitForDisconnected(*args, **kwargs): ...
    def waitForReadyRead(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    AddressInUseError : SocketError
    
    AnyIPProtocol : NetworkLayerProtocol
    
    BindFlag : Type[BindFlag]
    
    BindMode : Type[BindMode]
    
    BoundState : SocketState
    
    ClosingState : SocketState
    
    ConnectedState : SocketState
    
    ConnectingState : SocketState
    
    ConnectionRefusedError : SocketError
    
    DatagramTooLargeError : SocketError
    
    DefaultForPlatform : BindFlag
    
    DontShareAddress : BindFlag
    
    HostLookupState : SocketState
    
    HostNotFoundError : SocketError
    
    IPv4Protocol : NetworkLayerProtocol
    
    IPv6Protocol : NetworkLayerProtocol
    
    KeepAliveOption : SocketOption
    
    ListeningState : SocketState
    
    LowDelayOption : SocketOption
    
    MulticastLoopbackOption : SocketOption
    
    MulticastTtlOption : SocketOption
    
    NetworkError : SocketError
    
    NetworkLayerProtocol : Type[NetworkLayerProtocol]
    
    OperationError : SocketError
    
    PauseMode : Type[PauseMode]
    
    PauseModes : Type[PauseModes]
    
    PauseNever : PauseMode
    
    PauseOnSslErrors : PauseMode
    
    ProxyAuthenticationRequiredError : SocketError
    
    ProxyConnectionClosedError : SocketError
    
    ProxyConnectionRefusedError : SocketError
    
    ProxyConnectionTimeoutError : SocketError
    
    ProxyNotFoundError : SocketError
    
    ProxyProtocolError : SocketError
    
    ReceiveBufferSizeSocketOption : SocketOption
    
    RemoteHostClosedError : SocketError
    
    ReuseAddressHint : BindFlag
    
    SendBufferSizeSocketOption : SocketOption
    
    ShareAddress : BindFlag
    
    SocketAccessError : SocketError
    
    SocketAddressNotAvailableError : SocketError
    
    SocketError : Type[SocketError]
    
    SocketOption : Type[SocketOption]
    
    SocketResourceError : SocketError
    
    SocketState : Type[SocketState]
    
    SocketTimeoutError : SocketError
    
    SocketType : Type[SocketType]
    
    SslHandshakeFailedError : SocketError
    
    SslInternalError : SocketError
    
    SslInvalidUserDataError : SocketError
    
    TcpSocket : SocketType
    
    TemporaryError : SocketError
    
    TypeOfServiceOption : SocketOption
    
    UdpSocket : SocketType
    
    UnconnectedState : SocketState
    
    UnfinishedSocketOperationError : SocketError
    
    UnknownNetworkLayerProtocol : NetworkLayerProtocol
    
    UnknownSocketError : SocketError
    
    UnknownSocketType : SocketType
    
    UnsupportedSocketOperationError : SocketError
    
    __new__ : builtin_function_or_method
    
    connected : PySide2.QtCore.Signal
    
    disconnected : PySide2.QtCore.Signal
    
    error : PySide2.QtCore.Signal
    
    hostFound : PySide2.QtCore.Signal
    
    proxyAuthenticationRequired : PySide2.QtCore.Signal
    
    stateChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QLocalServer(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def close(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def fullServerName(*args, **kwargs): ...
    def hasPendingConnections(*args, **kwargs): ...
    def incomingConnection(*args, **kwargs): ...
    def isListening(*args, **kwargs): ...
    def listen(*args, **kwargs): ...
    def maxPendingConnections(*args, **kwargs): ...
    def nextPendingConnection(*args, **kwargs): ...
    def serverError(*args, **kwargs): ...
    def serverName(*args, **kwargs): ...
    def setMaxPendingConnections(*args, **kwargs): ...
    def setSocketOptions(*args, **kwargs): ...
    def socketOptions(*args, **kwargs): ...
    def waitForNewConnection(*args, **kwargs): ...
    @staticmethod
    def removeServer(*args, **kwargs): ...
    GroupAccessOption : SocketOption
    
    NoOptions : SocketOption
    
    OtherAccessOption : SocketOption
    
    SocketOption : Type[SocketOption]
    
    SocketOptions : Type[SocketOptions]
    
    UserAccessOption : SocketOption
    
    WorldAccessOption : SocketOption
    
    __new__ : builtin_function_or_method
    
    newConnection : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QNetworkAccessManager(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def activeConfiguration(*args, **kwargs): ...
    def cache(*args, **kwargs): ...
    def clearAccessCache(*args, **kwargs): ...
    def configuration(*args, **kwargs): ...
    def connectToHost(*args, **kwargs): ...
    def cookieJar(*args, **kwargs): ...
    def createRequest(*args, **kwargs): ...
    def deleteResource(*args, **kwargs): ...
    def get(*args, **kwargs): ...
    def head(*args, **kwargs): ...
    def networkAccessible(*args, **kwargs): ...
    def post(*args, **kwargs): ...
    def proxy(*args, **kwargs): ...
    def proxyFactory(*args, **kwargs): ...
    def put(*args, **kwargs): ...
    def sendCustomRequest(*args, **kwargs): ...
    def setCache(*args, **kwargs): ...
    def setConfiguration(*args, **kwargs): ...
    def setCookieJar(*args, **kwargs): ...
    def setNetworkAccessible(*args, **kwargs): ...
    def setProxy(*args, **kwargs): ...
    def setProxyFactory(*args, **kwargs): ...
    def supportedSchemes(*args, **kwargs): ...
    def supportedSchemesImplementation(*args, **kwargs): ...
    Accessible : NetworkAccessibility
    
    CustomOperation : Operation
    
    DeleteOperation : Operation
    
    GetOperation : Operation
    
    HeadOperation : Operation
    
    NetworkAccessibility : Type[NetworkAccessibility]
    
    NotAccessible : NetworkAccessibility
    
    Operation : Type[Operation]
    
    PostOperation : Operation
    
    PutOperation : Operation
    
    UnknownAccessibility : NetworkAccessibility
    
    UnknownOperation : Operation
    
    __new__ : builtin_function_or_method
    
    authenticationRequired : PySide2.QtCore.Signal
    
    encrypted : PySide2.QtCore.Signal
    
    finished : PySide2.QtCore.Signal
    
    networkAccessibleChanged : PySide2.QtCore.Signal
    
    networkSessionConnected : PySide2.QtCore.Signal
    
    preSharedKeyAuthenticationRequired : PySide2.QtCore.Signal
    
    proxyAuthenticationRequired : PySide2.QtCore.Signal
    
    sslErrors : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QLocalSocket(_QIODevice):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def abort(*args, **kwargs): ...
    def bytesAvailable(*args, **kwargs): ...
    def bytesToWrite(*args, **kwargs): ...
    def canReadLine(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def connectToServer(*args, **kwargs): ...
    def disconnectFromServer(*args, **kwargs): ...
    def flush(*args, **kwargs): ...
    def fullServerName(*args, **kwargs): ...
    def isSequential(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def readBufferSize(*args, **kwargs): ...
    def readData(*args, **kwargs): ...
    def serverName(*args, **kwargs): ...
    def setReadBufferSize(*args, **kwargs): ...
    def setServerName(*args, **kwargs): ...
    def setSocketDescriptor(*args, **kwargs): ...
    def socketDescriptor(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def waitForBytesWritten(*args, **kwargs): ...
    def waitForConnected(*args, **kwargs): ...
    def waitForDisconnected(*args, **kwargs): ...
    def waitForReadyRead(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    ClosingState : LocalSocketState
    
    ConnectedState : LocalSocketState
    
    ConnectingState : LocalSocketState
    
    ConnectionError : LocalSocketError
    
    ConnectionRefusedError : LocalSocketError
    
    DatagramTooLargeError : LocalSocketError
    
    LocalSocketError : Type[LocalSocketError]
    
    LocalSocketState : Type[LocalSocketState]
    
    OperationError : LocalSocketError
    
    PeerClosedError : LocalSocketError
    
    ServerNotFoundError : LocalSocketError
    
    SocketAccessError : LocalSocketError
    
    SocketResourceError : LocalSocketError
    
    SocketTimeoutError : LocalSocketError
    
    UnconnectedState : LocalSocketState
    
    UnknownSocketError : LocalSocketError
    
    UnsupportedSocketOperationError : LocalSocketError
    
    __new__ : builtin_function_or_method
    
    connected : PySide2.QtCore.Signal
    
    disconnected : PySide2.QtCore.Signal
    
    error : PySide2.QtCore.Signal
    
    stateChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QNetworkCookieJar(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def allCookies(*args, **kwargs): ...
    def cookiesForUrl(*args, **kwargs): ...
    def deleteCookie(*args, **kwargs): ...
    def insertCookie(*args, **kwargs): ...
    def setAllCookies(*args, **kwargs): ...
    def setCookiesFromUrl(*args, **kwargs): ...
    def updateCookie(*args, **kwargs): ...
    def validateCookie(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QAbstractNetworkCache(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def cacheSize(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def metaData(*args, **kwargs): ...
    def prepare(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def updateMetaData(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QIPv6Address(_Object):
    def __copy__(*args, **kwargs): ...
    def __delitem__(*args, **kwargs):
        """
        x.__delitem__(y) <==> del x[y]
        """
        ...
    def __getitem__(*args, **kwargs):
        """
        x.__getitem__(y) <==> x[y]
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __len__(self) -> int:
        """
        x.__len__() <==> len(x)
        """
        ...
    def __setitem__(*args, **kwargs):
        """
        x.__setitem__(i, y) <==> x[i]=y
        """
        ...
    __new__ : builtin_function_or_method


class QTcpServer(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addPendingConnection(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def hasPendingConnections(*args, **kwargs): ...
    def incomingConnection(*args, **kwargs): ...
    def isListening(*args, **kwargs): ...
    def listen(*args, **kwargs): ...
    def maxPendingConnections(*args, **kwargs): ...
    def nextPendingConnection(*args, **kwargs): ...
    def pauseAccepting(*args, **kwargs): ...
    def proxy(*args, **kwargs): ...
    def resumeAccepting(*args, **kwargs): ...
    def serverAddress(*args, **kwargs): ...
    def serverError(*args, **kwargs): ...
    def serverPort(*args, **kwargs): ...
    def setMaxPendingConnections(*args, **kwargs): ...
    def setProxy(*args, **kwargs): ...
    def setSocketDescriptor(*args, **kwargs): ...
    def socketDescriptor(*args, **kwargs): ...
    def waitForNewConnection(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    acceptError : PySide2.QtCore.Signal
    
    newConnection : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QUdpSocket(QAbstractSocket):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def hasPendingDatagrams(*args, **kwargs): ...
    def joinMulticastGroup(*args, **kwargs): ...
    def leaveMulticastGroup(*args, **kwargs): ...
    def multicastInterface(*args, **kwargs): ...
    def pendingDatagramSize(*args, **kwargs): ...
    def readDatagram(*args, **kwargs): ...
    def setMulticastInterface(*args, **kwargs): ...
    def writeDatagram(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QTcpSocket(QAbstractSocket):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QNetworkDiskCache(QAbstractNetworkCache):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def cacheDirectory(*args, **kwargs): ...
    def cacheSize(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def expire(*args, **kwargs): ...
    def fileMetaData(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def maximumCacheSize(*args, **kwargs): ...
    def metaData(*args, **kwargs): ...
    def prepare(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def setCacheDirectory(*args, **kwargs): ...
    def setMaximumCacheSize(*args, **kwargs): ...
    def updateMetaData(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : PySide2.QtCore.QMetaObject



