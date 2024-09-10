from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import QtCore


from PySide2.QtWidgets import QWidget as _QWidget
from PySide2.QtCore import QObject as _QObject


if False:
    from typing import Dict, List, Tuple, Union, Optional

class _Object(object):
    __dict__ : dictproxy


class QWebEngineProfile(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def cachePath(*args, **kwargs): ...
    def clearAllVisitedLinks(*args, **kwargs): ...
    def clearVisitedLinks(*args, **kwargs): ...
    def httpAcceptLanguage(*args, **kwargs): ...
    def httpCacheMaximumSize(*args, **kwargs): ...
    def httpCacheType(*args, **kwargs): ...
    def httpUserAgent(*args, **kwargs): ...
    def isOffTheRecord(*args, **kwargs): ...
    def persistentCookiesPolicy(*args, **kwargs): ...
    def persistentStoragePath(*args, **kwargs): ...
    def removeAllUrlSchemeHandlers(*args, **kwargs): ...
    def removeUrlScheme(*args, **kwargs): ...
    def setCachePath(*args, **kwargs): ...
    def setHttpAcceptLanguage(*args, **kwargs): ...
    def setHttpCacheMaximumSize(*args, **kwargs): ...
    def setHttpCacheType(*args, **kwargs): ...
    def setHttpUserAgent(*args, **kwargs): ...
    def setPersistentCookiesPolicy(*args, **kwargs): ...
    def setPersistentStoragePath(*args, **kwargs): ...
    def storageName(*args, **kwargs): ...
    def visitedLinksContainsUrl(*args, **kwargs): ...
    @staticmethod
    def defaultProfile(*args, **kwargs): ...
    AllowPersistentCookies : PersistentCookiesPolicy
    
    DiskHttpCache : HttpCacheType
    
    ForcePersistentCookies : PersistentCookiesPolicy
    
    HttpCacheType : Type[HttpCacheType]
    
    MemoryHttpCache : HttpCacheType
    
    NoPersistentCookies : PersistentCookiesPolicy
    
    PersistentCookiesPolicy : Type[PersistentCookiesPolicy]
    
    __new__ : builtin_function_or_method
    
    downloadRequested : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QWebEngineDownloadItem(_QObject):
    def accept(*args, **kwargs): ...
    def cancel(*args, **kwargs): ...
    def id(*args, **kwargs): ...
    def isFinished(*args, **kwargs): ...
    def mimeType(*args, **kwargs): ...
    def path(*args, **kwargs): ...
    def receivedBytes(*args, **kwargs): ...
    def setPath(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def totalBytes(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    DownloadCancelled : DownloadState
    
    DownloadCompleted : DownloadState
    
    DownloadInProgress : DownloadState
    
    DownloadInterrupted : DownloadState
    
    DownloadRequested : DownloadState
    
    DownloadState : Type[DownloadState]
    
    downloadProgress : PySide2.QtCore.Signal
    
    finished : PySide2.QtCore.Signal
    
    stateChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject


class QWebEngineView(_QWidget):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def back(*args, **kwargs): ...
    def contextMenuEvent(*args, **kwargs): ...
    def createWindow(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def findText(*args, **kwargs): ...
    def forward(*args, **kwargs): ...
    def hasSelection(*args, **kwargs): ...
    def hideEvent(*args, **kwargs): ...
    def iconUrl(*args, **kwargs): ...
    def load(*args, **kwargs): ...
    def page(*args, **kwargs): ...
    def pageAction(*args, **kwargs): ...
    def reload(*args, **kwargs): ...
    def selectedText(*args, **kwargs): ...
    def setContent(*args, **kwargs): ...
    def setHtml(*args, **kwargs): ...
    def setPage(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def setZoomFactor(*args, **kwargs): ...
    def showEvent(*args, **kwargs): ...
    def sizeHint(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    def title(*args, **kwargs): ...
    def triggerPageAction(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    def zoomFactor(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    iconUrlChanged : PySide2.QtCore.Signal
    
    loadFinished : PySide2.QtCore.Signal
    
    loadProgress : PySide2.QtCore.Signal
    
    loadStarted : PySide2.QtCore.Signal
    
    renderProcessTerminated : PySide2.QtCore.Signal
    
    selectionChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject
    
    titleChanged : PySide2.QtCore.Signal
    
    urlChanged : PySide2.QtCore.Signal


class QWebEngineScript(_Object):
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
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def injectionPoint(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def runsOnSubFrames(*args, **kwargs): ...
    def setInjectionPoint(*args, **kwargs): ...
    def setName(*args, **kwargs): ...
    def setRunsOnSubFrames(*args, **kwargs): ...
    def setSourceCode(*args, **kwargs): ...
    def setWorldId(*args, **kwargs): ...
    def sourceCode(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def worldId(*args, **kwargs): ...
    ApplicationWorld : ScriptWorldId
    
    Deferred : InjectionPoint
    
    DocumentCreation : InjectionPoint
    
    DocumentReady : InjectionPoint
    
    InjectionPoint : Type[InjectionPoint]
    
    MainWorld : ScriptWorldId
    
    ScriptWorldId : Type[ScriptWorldId]
    
    UserWorld : ScriptWorldId
    
    __new__ : builtin_function_or_method


class QWebEngineCertificateError(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def error(*args, **kwargs): ...
    def errorDescription(*args, **kwargs): ...
    def isOverridable(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    CertificateAuthorityInvalid : Error
    
    CertificateCommonNameInvalid : Error
    
    CertificateContainsErrors : Error
    
    CertificateDateInvalid : Error
    
    CertificateInvalid : Error
    
    CertificateNameConstraintViolation : Error
    
    CertificateNoRevocationMechanism : Error
    
    CertificateNonUniqueName : Error
    
    CertificateRevoked : Error
    
    CertificateUnableToCheckRevocation : Error
    
    CertificateWeakKey : Error
    
    CertificateWeakSignatureAlgorithm : Error
    
    Error : Type[Error]
    
    SslPinnedKeyNotInCertificateChain : Error
    
    __new__ : builtin_function_or_method


class QWebEngineHistoryItem(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def iconUrl(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def lastVisited(*args, **kwargs): ...
    def originalUrl(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def title(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QWebEnginePage(_QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def acceptNavigationRequest(*args, **kwargs): ...
    def action(*args, **kwargs): ...
    def backgroundColor(*args, **kwargs): ...
    def certificateError(*args, **kwargs): ...
    def chooseFiles(*args, **kwargs): ...
    def createStandardContextMenu(*args, **kwargs): ...
    def createWindow(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def findText(*args, **kwargs): ...
    def hasSelection(*args, **kwargs): ...
    def iconUrl(*args, **kwargs): ...
    def javaScriptAlert(*args, **kwargs): ...
    def javaScriptConfirm(*args, **kwargs): ...
    def javaScriptConsoleMessage(*args, **kwargs): ...
    def javaScriptPrompt(*args, **kwargs): ...
    def load(*args, **kwargs): ...
    def profile(*args, **kwargs): ...
    def requestedUrl(*args, **kwargs): ...
    def runJavaScript(*args, **kwargs): ...
    def selectedText(*args, **kwargs): ...
    def setBackgroundColor(*args, **kwargs): ...
    def setContent(*args, **kwargs): ...
    def setFeaturePermission(*args, **kwargs): ...
    def setHtml(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def setView(*args, **kwargs): ...
    def setWebChannel(*args, **kwargs): ...
    def setZoomFactor(*args, **kwargs): ...
    def title(*args, **kwargs): ...
    def triggerAction(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    def view(*args, **kwargs): ...
    def webChannel(*args, **kwargs): ...
    def zoomFactor(*args, **kwargs): ...
    Back : WebAction
    
    Copy : WebAction
    
    CopyImageToClipboard : WebAction
    
    CopyImageUrlToClipboard : WebAction
    
    CopyLinkToClipboard : WebAction
    
    CopyMediaUrlToClipboard : WebAction
    
    Cut : WebAction
    
    DownloadImageToDisk : WebAction
    
    DownloadLinkToDisk : WebAction
    
    DownloadMediaToDisk : WebAction
    
    ErrorMessageLevel : JavaScriptConsoleMessageLevel
    
    ExitFullScreen : WebAction
    
    Feature : Type[Feature]
    
    FileSelectOpen : FileSelectionMode
    
    FileSelectOpenMultiple : FileSelectionMode
    
    FileSelectionMode : Type[FileSelectionMode]
    
    FindBackward : FindFlag
    
    FindCaseSensitively : FindFlag
    
    FindFlag : Type[FindFlag]
    
    FindFlags : Type[FindFlags]
    
    Forward : WebAction
    
    Geolocation : Feature
    
    InfoMessageLevel : JavaScriptConsoleMessageLevel
    
    InspectElement : WebAction
    
    JavaScriptConsoleMessageLevel : Type[JavaScriptConsoleMessageLevel]
    
    MediaAudioCapture : Feature
    
    MediaAudioVideoCapture : Feature
    
    MediaVideoCapture : Feature
    
    MouseLock : Feature
    
    NavigationType : Type[NavigationType]
    
    NavigationTypeBackForward : NavigationType
    
    NavigationTypeFormSubmitted : NavigationType
    
    NavigationTypeLinkClicked : NavigationType
    
    NavigationTypeOther : NavigationType
    
    NavigationTypeReload : NavigationType
    
    NavigationTypeTyped : NavigationType
    
    NoWebAction : WebAction
    
    OpenLinkInNewTab : WebAction
    
    OpenLinkInNewWindow : WebAction
    
    OpenLinkInThisWindow : WebAction
    
    Paste : WebAction
    
    PasteAndMatchStyle : WebAction
    
    PermissionDeniedByUser : PermissionPolicy
    
    PermissionGrantedByUser : PermissionPolicy
    
    PermissionPolicy : Type[PermissionPolicy]
    
    PermissionUnknown : PermissionPolicy
    
    Redo : WebAction
    
    Reload : WebAction
    
    ReloadAndBypassCache : WebAction
    
    RequestClose : WebAction
    
    SelectAll : WebAction
    
    Stop : WebAction
    
    ToggleMediaControls : WebAction
    
    ToggleMediaLoop : WebAction
    
    ToggleMediaMute : WebAction
    
    ToggleMediaPlayPause : WebAction
    
    Undo : WebAction
    
    WarningMessageLevel : JavaScriptConsoleMessageLevel
    
    WebAction : Type[WebAction]
    
    WebActionCount : WebAction
    
    WebBrowserTab : WebWindowType
    
    WebBrowserWindow : WebWindowType
    
    WebDialog : WebWindowType
    
    WebWindowType : Type[WebWindowType]
    
    __new__ : builtin_function_or_method
    
    authenticationRequired : PySide2.QtCore.Signal
    
    featurePermissionRequestCanceled : PySide2.QtCore.Signal
    
    featurePermissionRequested : PySide2.QtCore.Signal
    
    fullScreenRequested : PySide2.QtCore.Signal
    
    geometryChangeRequested : PySide2.QtCore.Signal
    
    iconUrlChanged : PySide2.QtCore.Signal
    
    linkHovered : PySide2.QtCore.Signal
    
    loadFinished : PySide2.QtCore.Signal
    
    loadProgress : PySide2.QtCore.Signal
    
    loadStarted : PySide2.QtCore.Signal
    
    proxyAuthenticationRequired : PySide2.QtCore.Signal
    
    renderProcessTerminated : PySide2.QtCore.Signal
    
    selectionChanged : PySide2.QtCore.Signal
    
    staticMetaObject : PySide2.QtCore.QMetaObject
    
    titleChanged : PySide2.QtCore.Signal
    
    urlChanged : PySide2.QtCore.Signal
    
    windowCloseRequested : PySide2.QtCore.Signal



