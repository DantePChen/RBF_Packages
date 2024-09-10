from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
if False:
    from typing import Dict, List, Tuple, Union, Optional

class ClassInfo(object):
    def __call__(*args, **kwargs):
        """
        x.__call__(...) <==> x(...)
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    __new__ : builtin_function_or_method


class Slot(object):
    """
    Slot
    """
    
    
    
    def __call__(*args, **kwargs):
        """
        x.__call__(...) <==> x(...)
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    __new__ : builtin_function_or_method


class _Object(object):
    __dict__ : dictproxy


class MetaFunction(object):
    """
    MetaFunction
    """
    
    
    
    def __call__(*args, **kwargs):
        """
        x.__call__(...) <==> x(...)
        """
        ...
    __new__ : builtin_function_or_method


class Signal(object):
    """
    Signal
    """
    
    
    
    def __call__(*args, **kwargs):
        """
        x.__call__(...) <==> x(...)
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
    def __str__(self) -> str:
        """
        x.__str__() <==> str(x)
        """
        ...
    __new__ : builtin_function_or_method


class QtMsgType(object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __and__(*args, **kwargs):
        """
        x.__and__(y) <==> x&y
        """
        ...
    def __div__(*args, **kwargs):
        """
        x.__div__(y) <==> x/y
        """
        ...
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
    def __index__(*args, **kwargs):
        """
        x[y:z] <==> x[y.__index__():z.__index__()]
        """
        ...
    def __int__(*args, **kwargs):
        """
        x.__int__() <==> int(x)
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __long__(*args, **kwargs):
        """
        x.__long__() <==> long(x)
        """
        ...
    def __lt__(self, other: Any) -> bool:
        """
        x.__lt__(y) <==> x<y
        """
        ...
    def __mul__(*args, **kwargs):
        """
        x.__mul__(y) <==> x*y
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
    def __or__(*args, **kwargs):
        """
        x.__or__(y) <==> x|y
        """
        ...
    def __pos__(*args, **kwargs):
        """
        x.__pos__() <==> +x
        """
        ...
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rand__(*args, **kwargs):
        """
        x.__rand__(y) <==> y&x
        """
        ...
    def __rdiv__(*args, **kwargs):
        """
        x.__rdiv__(y) <==> y/x
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rmul__(*args, **kwargs):
        """
        x.__rmul__(y) <==> y*x
        """
        ...
    def __ror__(*args, **kwargs):
        """
        x.__ror__(y) <==> y|x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __rxor__(*args, **kwargs):
        """
        x.__rxor__(y) <==> y^x
        """
        ...
    def __str__(self) -> str:
        """
        x.__str__() <==> str(x)
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def __xor__(*args, **kwargs):
        """
        x.__xor__(y) <==> x^y
        """
        ...
    QtCriticalMsg : QtMsgType
    
    QtDebugMsg : QtMsgType
    
    QtFatalMsg : QtMsgType
    
    QtInfoMsg : QtMsgType
    
    QtSystemMsg : QtMsgType
    
    QtWarningMsg : QtMsgType
    
    __new__ : builtin_function_or_method
    
    name : getset_descriptor
    
    values : dict


class Property(object):
    def __call__(*args, **kwargs):
        """
        x.__call__(...) <==> x(...)
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def getter(*args, **kwargs): ...
    def read(*args, **kwargs): ...
    def setter(*args, **kwargs): ...
    def write(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QLine(_Object):
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def dx(*args, **kwargs): ...
    def dy(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def p1(*args, **kwargs): ...
    def p2(*args, **kwargs): ...
    def setLine(*args, **kwargs): ...
    def setP1(*args, **kwargs): ...
    def setP2(*args, **kwargs): ...
    def setPoints(*args, **kwargs): ...
    def toTuple(*args, **kwargs): ...
    def translate(*args, **kwargs): ...
    def translated(*args, **kwargs): ...
    def x1(*args, **kwargs): ...
    def x2(*args, **kwargs): ...
    def y1(*args, **kwargs): ...
    def y2(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QBitArray(_Object):
    def __and__(*args, **kwargs):
        """
        x.__and__(y) <==> x&y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __delitem__(*args, **kwargs):
        """
        x.__delitem__(y) <==> del x[y]
        """
        ...
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
    def __getitem__(*args, **kwargs):
        """
        x.__getitem__(y) <==> x[y]
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
    def __iand__(*args, **kwargs):
        """
        x.__iand__(y) <==> x&=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __invert__(*args, **kwargs):
        """
        x.__invert__() <==> ~x
        """
        ...
    def __ior__(*args, **kwargs):
        """
        x.__ior__(y) <==> x|=y
        """
        ...
    def __ixor__(*args, **kwargs):
        """
        x.__ixor__(y) <==> x^=y
        """
        ...
    def __le__(self, other: Any) -> bool:
        """
        x.__le__(y) <==> x<=y
        """
        ...
    def __len__(self) -> int:
        """
        x.__len__() <==> len(x)
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
    def __or__(*args, **kwargs):
        """
        x.__or__(y) <==> x|y
        """
        ...
    def __rand__(*args, **kwargs):
        """
        x.__rand__(y) <==> y&x
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __ror__(*args, **kwargs):
        """
        x.__ror__(y) <==> y|x
        """
        ...
    def __rxor__(*args, **kwargs):
        """
        x.__rxor__(y) <==> y^x
        """
        ...
    def __setitem__(*args, **kwargs):
        """
        x.__setitem__(i, y) <==> x[i]=y
        """
        ...
    def __xor__(*args, **kwargs):
        """
        x.__xor__(y) <==> x^y
        """
        ...
    def at(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def clearBit(*args, **kwargs): ...
    def count(*args, **kwargs): ...
    def fill(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def resize(*args, **kwargs): ...
    def setBit(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def testBit(*args, **kwargs): ...
    def toggleBit(*args, **kwargs): ...
    def truncate(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QMetaObject(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def cast(*args, **kwargs): ...
    def classInfo(*args, **kwargs): ...
    def classInfoCount(*args, **kwargs): ...
    def classInfoOffset(*args, **kwargs): ...
    def className(*args, **kwargs): ...
    def constructor(*args, **kwargs): ...
    def constructorCount(*args, **kwargs): ...
    def enumerator(*args, **kwargs): ...
    def enumeratorCount(*args, **kwargs): ...
    def enumeratorOffset(*args, **kwargs): ...
    def indexOfClassInfo(*args, **kwargs): ...
    def indexOfConstructor(*args, **kwargs): ...
    def indexOfEnumerator(*args, **kwargs): ...
    def indexOfMethod(*args, **kwargs): ...
    def indexOfProperty(*args, **kwargs): ...
    def indexOfSignal(*args, **kwargs): ...
    def indexOfSlot(*args, **kwargs): ...
    def method(*args, **kwargs): ...
    def methodCount(*args, **kwargs): ...
    def methodOffset(*args, **kwargs): ...
    def newInstance(*args, **kwargs): ...
    def property(*args, **kwargs): ...
    def propertyCount(*args, **kwargs): ...
    def propertyOffset(*args, **kwargs): ...
    def superClass(*args, **kwargs): ...
    def userProperty(*args, **kwargs): ...
    @staticmethod
    def checkConnectArgs(*args, **kwargs): ...
    @staticmethod
    def connectSlotsByName(*args, **kwargs): ...
    @staticmethod
    def disconnect(*args, **kwargs): ...
    @staticmethod
    def disconnectOne(*args, **kwargs): ...
    @staticmethod
    def invokeMethod(*args, **kwargs): ...
    @staticmethod
    def normalizedSignature(*args, **kwargs): ...
    @staticmethod
    def normalizedType(*args, **kwargs): ...
    Call : Type[Call]
    
    CreateInstance : Call
    
    IndexOfMethod : Call
    
    InvokeMetaMethod : Call
    
    QueryPropertyDesignable : Call
    
    QueryPropertyEditable : Call
    
    QueryPropertyScriptable : Call
    
    QueryPropertyStored : Call
    
    QueryPropertyUser : Call
    
    ReadProperty : Call
    
    RegisterMethodArgumentMetaType : Call
    
    RegisterPropertyMetaType : Call
    
    ResetProperty : Call
    
    WriteProperty : Call
    
    __new__ : builtin_function_or_method


class QElapsedTimer(_Object):
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
    def elapsed(*args, **kwargs): ...
    def hasExpired(*args, **kwargs): ...
    def invalidate(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def msecsSinceReference(*args, **kwargs): ...
    def msecsTo(*args, **kwargs): ...
    def nsecsElapsed(*args, **kwargs): ...
    def restart(*args, **kwargs): ...
    def secsTo(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    @staticmethod
    def clockType(*args, **kwargs): ...
    @staticmethod
    def isMonotonic(*args, **kwargs): ...
    ClockType : Type[ClockType]
    
    MachAbsoluteTime : ClockType
    
    MonotonicClock : ClockType
    
    PerformanceCounter : ClockType
    
    SystemTime : ClockType
    
    TickCounter : ClockType
    
    __new__ : builtin_function_or_method


class QTextBoundaryFinder(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def boundaryReasons(*args, **kwargs): ...
    def isAtBoundary(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def position(*args, **kwargs): ...
    def setPosition(*args, **kwargs): ...
    def string(*args, **kwargs): ...
    def toEnd(*args, **kwargs): ...
    def toNextBoundary(*args, **kwargs): ...
    def toPreviousBoundary(*args, **kwargs): ...
    def toStart(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    BoundaryReason : Type[BoundaryReason]
    
    BoundaryReasons : Type[BoundaryReasons]
    
    BoundaryType : Type[BoundaryType]
    
    BreakOpportunity : BoundaryReason
    
    EndOfItem : BoundaryReason
    
    Grapheme : BoundaryType
    
    Line : BoundaryType
    
    MandatoryBreak : BoundaryReason
    
    NotAtBoundary : BoundaryReason
    
    Sentence : BoundaryType
    
    SoftHyphen : BoundaryReason
    
    StartOfItem : BoundaryReason
    
    Word : BoundaryType
    
    __new__ : builtin_function_or_method


class QMessageLogContext(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    __new__ : builtin_function_or_method
    
    category : getset_descriptor
    
    file : getset_descriptor
    
    function : getset_descriptor
    
    line : getset_descriptor
    
    version : getset_descriptor


class QItemSelection(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __delitem__(*args, **kwargs):
        """
        x.__delitem__(y) <==> del x[y]
        """
        ...
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
    def __getitem__(*args, **kwargs):
        """
        x.__getitem__(y) <==> x[y]
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
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
    def __len__(self) -> int:
        """
        x.__len__() <==> len(x)
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
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rlshift__(*args, **kwargs):
        """
        x.__rlshift__(y) <==> y<<x
        """
        ...
    def __setitem__(*args, **kwargs):
        """
        x.__setitem__(i, y) <==> x[i]=y
        """
        ...
    def append(*args, **kwargs): ...
    def at(*args, **kwargs): ...
    def back(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def constFirst(*args, **kwargs): ...
    def constLast(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def count(*args, **kwargs): ...
    def detachShared(*args, **kwargs): ...
    def empty(*args, **kwargs): ...
    def endsWith(*args, **kwargs): ...
    def first(*args, **kwargs): ...
    def front(*args, **kwargs): ...
    def indexOf(*args, **kwargs): ...
    def indexes(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isSharedWith(*args, **kwargs): ...
    def last(*args, **kwargs): ...
    def lastIndexOf(*args, **kwargs): ...
    def length(*args, **kwargs): ...
    def merge(*args, **kwargs): ...
    def mid(*args, **kwargs): ...
    def move(*args, **kwargs): ...
    def pop_back(*args, **kwargs): ...
    def pop_front(*args, **kwargs): ...
    def prepend(*args, **kwargs): ...
    def push_back(*args, **kwargs): ...
    def push_front(*args, **kwargs): ...
    def removeAll(*args, **kwargs): ...
    def removeAt(*args, **kwargs): ...
    def removeFirst(*args, **kwargs): ...
    def removeLast(*args, **kwargs): ...
    def removeOne(*args, **kwargs): ...
    def replace(*args, **kwargs): ...
    def reserve(*args, **kwargs): ...
    def select(*args, **kwargs): ...
    def setSharable(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def startsWith(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def takeAt(*args, **kwargs): ...
    def takeFirst(*args, **kwargs): ...
    def takeLast(*args, **kwargs): ...
    def toSet(*args, **kwargs): ...
    def toVector(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    @staticmethod
    def fromSet(*args, **kwargs): ...
    @staticmethod
    def fromVector(*args, **kwargs): ...
    @staticmethod
    def split(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QTextStream(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __lshift__(*args, **kwargs):
        """
        x.__lshift__(y) <==> x<<y
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
    def atEnd(*args, **kwargs): ...
    def autoDetectUnicode(*args, **kwargs): ...
    def codec(*args, **kwargs): ...
    def device(*args, **kwargs): ...
    def fieldAlignment(*args, **kwargs): ...
    def fieldWidth(*args, **kwargs): ...
    def flush(*args, **kwargs): ...
    def generateByteOrderMark(*args, **kwargs): ...
    def integerBase(*args, **kwargs): ...
    def locale(*args, **kwargs): ...
    def numberFlags(*args, **kwargs): ...
    def padChar(*args, **kwargs): ...
    def pos(*args, **kwargs): ...
    def read(*args, **kwargs): ...
    def readAll(*args, **kwargs): ...
    def readLine(*args, **kwargs): ...
    def realNumberNotation(*args, **kwargs): ...
    def realNumberPrecision(*args, **kwargs): ...
    def reset(*args, **kwargs): ...
    def resetStatus(*args, **kwargs): ...
    def seek(*args, **kwargs): ...
    def setAutoDetectUnicode(*args, **kwargs): ...
    def setCodec(*args, **kwargs): ...
    def setDevice(*args, **kwargs): ...
    def setFieldAlignment(*args, **kwargs): ...
    def setFieldWidth(*args, **kwargs): ...
    def setGenerateByteOrderMark(*args, **kwargs): ...
    def setIntegerBase(*args, **kwargs): ...
    def setLocale(*args, **kwargs): ...
    def setNumberFlags(*args, **kwargs): ...
    def setPadChar(*args, **kwargs): ...
    def setRealNumberNotation(*args, **kwargs): ...
    def setRealNumberPrecision(*args, **kwargs): ...
    def setStatus(*args, **kwargs): ...
    def skipWhiteSpace(*args, **kwargs): ...
    def status(*args, **kwargs): ...
    def string(*args, **kwargs): ...
    AlignAccountingStyle : FieldAlignment
    
    AlignCenter : FieldAlignment
    
    AlignLeft : FieldAlignment
    
    AlignRight : FieldAlignment
    
    FieldAlignment : Type[FieldAlignment]
    
    FixedNotation : RealNumberNotation
    
    ForcePoint : NumberFlag
    
    ForceSign : NumberFlag
    
    NumberFlag : Type[NumberFlag]
    
    NumberFlags : Type[NumberFlags]
    
    Ok : Status
    
    ReadCorruptData : Status
    
    ReadPastEnd : Status
    
    RealNumberNotation : Type[RealNumberNotation]
    
    ScientificNotation : RealNumberNotation
    
    ShowBase : NumberFlag
    
    SmartNotation : RealNumberNotation
    
    Status : Type[Status]
    
    UppercaseBase : NumberFlag
    
    UppercaseDigits : NumberFlag
    
    WriteFailed : Status
    
    __new__ : builtin_function_or_method


class QModelIndex(_Object):
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
    def child(*args, **kwargs): ...
    def column(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def internalId(*args, **kwargs): ...
    def internalPointer(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def model(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def row(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QResource(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def absoluteFilePath(*args, **kwargs): ...
    def children(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def isCompressed(*args, **kwargs): ...
    def isDir(*args, **kwargs): ...
    def isFile(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def locale(*args, **kwargs): ...
    def setFileName(*args, **kwargs): ...
    def setLocale(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    @staticmethod
    def addSearchPath(*args, **kwargs): ...
    @staticmethod
    def registerResource(*args, **kwargs): ...
    @staticmethod
    def registerResourceData(*args, **kwargs): ...
    @staticmethod
    def searchPaths(*args, **kwargs): ...
    @staticmethod
    def unregisterResource(*args, **kwargs): ...
    @staticmethod
    def unregisterResourceData(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QJsonArray(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
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
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
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
    def append(*args, **kwargs): ...
    def at(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def count(*args, **kwargs): ...
    def empty(*args, **kwargs): ...
    def first(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def last(*args, **kwargs): ...
    def pop_back(*args, **kwargs): ...
    def pop_front(*args, **kwargs): ...
    def prepend(*args, **kwargs): ...
    def push_back(*args, **kwargs): ...
    def push_front(*args, **kwargs): ...
    def removeAt(*args, **kwargs): ...
    def removeFirst(*args, **kwargs): ...
    def removeLast(*args, **kwargs): ...
    def replace(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def takeAt(*args, **kwargs): ...
    def toVariantList(*args, **kwargs): ...
    @staticmethod
    def fromStringList(*args, **kwargs): ...
    @staticmethod
    def fromVariantList(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QUrl(_Object):
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def adjusted(*args, **kwargs): ...
    def authority(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def fragment(*args, **kwargs): ...
    def hasFragment(*args, **kwargs): ...
    def hasQuery(*args, **kwargs): ...
    def host(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isLocalFile(*args, **kwargs): ...
    def isParentOf(*args, **kwargs): ...
    def isRelative(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def matches(*args, **kwargs): ...
    def password(*args, **kwargs): ...
    def path(*args, **kwargs): ...
    def port(*args, **kwargs): ...
    def query(*args, **kwargs): ...
    def resolved(*args, **kwargs): ...
    def scheme(*args, **kwargs): ...
    def setAuthority(*args, **kwargs): ...
    def setFragment(*args, **kwargs): ...
    def setHost(*args, **kwargs): ...
    def setPassword(*args, **kwargs): ...
    def setPath(*args, **kwargs): ...
    def setPort(*args, **kwargs): ...
    def setQuery(*args, **kwargs): ...
    def setScheme(*args, **kwargs): ...
    def setUrl(*args, **kwargs): ...
    def setUserInfo(*args, **kwargs): ...
    def setUserName(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def toDisplayString(*args, **kwargs): ...
    def toEncoded(*args, **kwargs): ...
    def toLocalFile(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    def topLevelDomain(*args, **kwargs): ...
    def url(*args, **kwargs): ...
    def userInfo(*args, **kwargs): ...
    def userName(*args, **kwargs): ...
    @staticmethod
    def fromAce(*args, **kwargs): ...
    @staticmethod
    def fromEncoded(*args, **kwargs): ...
    @staticmethod
    def fromLocalFile(*args, **kwargs): ...
    @staticmethod
    def fromPercentEncoding(*args, **kwargs): ...
    @staticmethod
    def fromStringList(*args, **kwargs): ...
    @staticmethod
    def fromUserInput(*args, **kwargs): ...
    @staticmethod
    def idnWhitelist(*args, **kwargs): ...
    @staticmethod
    def setIdnWhitelist(*args, **kwargs): ...
    @staticmethod
    def toAce(*args, **kwargs): ...
    @staticmethod
    def toPercentEncoding(*args, **kwargs): ...
    @staticmethod
    def toStringList(*args, **kwargs): ...
    AssumeLocalFile : UserInputResolutionOption
    
    ComponentFormattingOption : Type[ComponentFormattingOption]
    
    DecodeReserved : ComponentFormattingOption
    
    DecodedMode : ParsingMode
    
    DefaultResolution : UserInputResolutionOption
    
    EncodeDelimiters : ComponentFormattingOption
    
    EncodeReserved : ComponentFormattingOption
    
    EncodeSpaces : ComponentFormattingOption
    
    EncodeUnicode : ComponentFormattingOption
    
    FormattingOptions : Type[FormattingOptions]
    
    FullyDecoded : ComponentFormattingOption
    
    FullyEncoded : ComponentFormattingOption
    
    locals()['None'] : UrlFormattingOption
    
    NormalizePathSegments : UrlFormattingOption
    
    ParsingMode : Type[ParsingMode]
    
    PreferLocalFile : UrlFormattingOption
    
    PrettyDecoded : ComponentFormattingOption
    
    RemoveAuthority : UrlFormattingOption
    
    RemoveFilename : UrlFormattingOption
    
    RemoveFragment : UrlFormattingOption
    
    RemovePassword : UrlFormattingOption
    
    RemovePath : UrlFormattingOption
    
    RemovePort : UrlFormattingOption
    
    RemoveQuery : UrlFormattingOption
    
    RemoveScheme : UrlFormattingOption
    
    RemoveUserInfo : UrlFormattingOption
    
    StrictMode : ParsingMode
    
    StripTrailingSlash : UrlFormattingOption
    
    TolerantMode : ParsingMode
    
    UrlFormattingOption : Type[UrlFormattingOption]
    
    UserInputResolutionOption : Type[UserInputResolutionOption]
    
    UserInputResolutionOptions : Type[UserInputResolutionOptions]
    
    __new__ : builtin_function_or_method


class QCollator(_Object):
    def __call__(*args, **kwargs):
        """
        x.__call__(...) <==> x(...)
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def caseSensitivity(*args, **kwargs): ...
    def compare(*args, **kwargs): ...
    def ignorePunctuation(*args, **kwargs): ...
    def locale(*args, **kwargs): ...
    def numericMode(*args, **kwargs): ...
    def setCaseSensitivity(*args, **kwargs): ...
    def setIgnorePunctuation(*args, **kwargs): ...
    def setLocale(*args, **kwargs): ...
    def setNumericMode(*args, **kwargs): ...
    def sortKey(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QXmlStreamReader(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addData(*args, **kwargs): ...
    def addExtraNamespaceDeclaration(*args, **kwargs): ...
    def addExtraNamespaceDeclarations(*args, **kwargs): ...
    def atEnd(*args, **kwargs): ...
    def attributes(*args, **kwargs): ...
    def characterOffset(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def columnNumber(*args, **kwargs): ...
    def device(*args, **kwargs): ...
    def documentEncoding(*args, **kwargs): ...
    def documentVersion(*args, **kwargs): ...
    def dtdName(*args, **kwargs): ...
    def dtdPublicId(*args, **kwargs): ...
    def dtdSystemId(*args, **kwargs): ...
    def entityDeclarations(*args, **kwargs): ...
    def entityResolver(*args, **kwargs): ...
    def error(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def hasError(*args, **kwargs): ...
    def isCDATA(*args, **kwargs): ...
    def isCharacters(*args, **kwargs): ...
    def isComment(*args, **kwargs): ...
    def isDTD(*args, **kwargs): ...
    def isEndDocument(*args, **kwargs): ...
    def isEndElement(*args, **kwargs): ...
    def isEntityReference(*args, **kwargs): ...
    def isProcessingInstruction(*args, **kwargs): ...
    def isStandaloneDocument(*args, **kwargs): ...
    def isStartDocument(*args, **kwargs): ...
    def isStartElement(*args, **kwargs): ...
    def isWhitespace(*args, **kwargs): ...
    def lineNumber(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def namespaceDeclarations(*args, **kwargs): ...
    def namespaceProcessing(*args, **kwargs): ...
    def namespaceUri(*args, **kwargs): ...
    def notationDeclarations(*args, **kwargs): ...
    def prefix(*args, **kwargs): ...
    def processingInstructionData(*args, **kwargs): ...
    def processingInstructionTarget(*args, **kwargs): ...
    def qualifiedName(*args, **kwargs): ...
    def raiseError(*args, **kwargs): ...
    def readElementText(*args, **kwargs): ...
    def readNext(*args, **kwargs): ...
    def readNextStartElement(*args, **kwargs): ...
    def setDevice(*args, **kwargs): ...
    def setEntityResolver(*args, **kwargs): ...
    def setNamespaceProcessing(*args, **kwargs): ...
    def skipCurrentElement(*args, **kwargs): ...
    def text(*args, **kwargs): ...
    def tokenString(*args, **kwargs): ...
    def tokenType(*args, **kwargs): ...
    Characters : TokenType
    
    Comment : TokenType
    
    CustomError : Error
    
    DTD : TokenType
    
    EndDocument : TokenType
    
    EndElement : TokenType
    
    EntityReference : TokenType
    
    Error : Type[Error]
    
    ErrorOnUnexpectedElement : ReadElementTextBehaviour
    
    IncludeChildElements : ReadElementTextBehaviour
    
    Invalid : TokenType
    
    NoError : Error
    
    NoToken : TokenType
    
    NotWellFormedError : Error
    
    PrematureEndOfDocumentError : Error
    
    ProcessingInstruction : TokenType
    
    ReadElementTextBehaviour : Type[ReadElementTextBehaviour]
    
    SkipChildElements : ReadElementTextBehaviour
    
    StartDocument : TokenType
    
    StartElement : TokenType
    
    TokenType : Type[TokenType]
    
    UnexpectedElementError : Error
    
    __new__ : builtin_function_or_method


class QXmlStreamAttributes(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __delitem__(*args, **kwargs):
        """
        x.__delitem__(y) <==> del x[y]
        """
        ...
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
    def __getitem__(*args, **kwargs):
        """
        x.__getitem__(y) <==> x[y]
        """
        ...
    def __gt__(self, other: Any) -> bool:
        """
        x.__gt__(y) <==> x>y
        """
        ...
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
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
    def __len__(self) -> int:
        """
        x.__len__() <==> len(x)
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
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rlshift__(*args, **kwargs):
        """
        x.__rlshift__(y) <==> y<<x
        """
        ...
    def __setitem__(*args, **kwargs):
        """
        x.__setitem__(i, y) <==> x[i]=y
        """
        ...
    def append(*args, **kwargs): ...
    def at(*args, **kwargs): ...
    def capacity(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def constData(*args, **kwargs): ...
    def constFirst(*args, **kwargs): ...
    def constLast(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def count(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def empty(*args, **kwargs): ...
    def endsWith(*args, **kwargs): ...
    def fill(*args, **kwargs): ...
    def first(*args, **kwargs): ...
    def front(*args, **kwargs): ...
    def hasAttribute(*args, **kwargs): ...
    def indexOf(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isSharedWith(*args, **kwargs): ...
    def last(*args, **kwargs): ...
    def lastIndexOf(*args, **kwargs): ...
    def length(*args, **kwargs): ...
    def mid(*args, **kwargs): ...
    def move(*args, **kwargs): ...
    def prepend(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def removeAll(*args, **kwargs): ...
    def removeAt(*args, **kwargs): ...
    def removeFirst(*args, **kwargs): ...
    def removeLast(*args, **kwargs): ...
    def removeOne(*args, **kwargs): ...
    def replace(*args, **kwargs): ...
    def reserve(*args, **kwargs): ...
    def resize(*args, **kwargs): ...
    def setSharable(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def squeeze(*args, **kwargs): ...
    def startsWith(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def takeAt(*args, **kwargs): ...
    def takeFirst(*args, **kwargs): ...
    def takeLast(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QDir(_Object):
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def absoluteFilePath(*args, **kwargs): ...
    def absolutePath(*args, **kwargs): ...
    def canonicalPath(*args, **kwargs): ...
    def cd(*args, **kwargs): ...
    def cdUp(*args, **kwargs): ...
    def count(*args, **kwargs): ...
    def dirName(*args, **kwargs): ...
    def entryInfoList(*args, **kwargs): ...
    def entryList(*args, **kwargs): ...
    def exists(*args, **kwargs): ...
    def filePath(*args, **kwargs): ...
    def filter(*args, **kwargs): ...
    def isAbsolute(*args, **kwargs): ...
    def isReadable(*args, **kwargs): ...
    def isRelative(*args, **kwargs): ...
    def isRoot(*args, **kwargs): ...
    def makeAbsolute(*args, **kwargs): ...
    def mkdir(*args, **kwargs): ...
    def mkpath(*args, **kwargs): ...
    def nameFilters(*args, **kwargs): ...
    def path(*args, **kwargs): ...
    def refresh(*args, **kwargs): ...
    def relativeFilePath(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def removeRecursively(*args, **kwargs): ...
    def rename(*args, **kwargs): ...
    def rmdir(*args, **kwargs): ...
    def rmpath(*args, **kwargs): ...
    def setFilter(*args, **kwargs): ...
    def setNameFilters(*args, **kwargs): ...
    def setPath(*args, **kwargs): ...
    def setSorting(*args, **kwargs): ...
    def sorting(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    @staticmethod
    def addResourceSearchPath(*args, **kwargs): ...
    @staticmethod
    def addSearchPath(*args, **kwargs): ...
    @staticmethod
    def cleanPath(*args, **kwargs): ...
    @staticmethod
    def current(*args, **kwargs): ...
    @staticmethod
    def currentPath(*args, **kwargs): ...
    @staticmethod
    def drives(*args, **kwargs): ...
    @staticmethod
    def fromNativeSeparators(*args, **kwargs): ...
    @staticmethod
    def home(*args, **kwargs): ...
    @staticmethod
    def homePath(*args, **kwargs): ...
    @staticmethod
    def isAbsolutePath(*args, **kwargs): ...
    @staticmethod
    def isRelativePath(*args, **kwargs): ...
    @staticmethod
    def listSeparator(*args, **kwargs): ...
    @staticmethod
    def match(*args, **kwargs): ...
    @staticmethod
    def nameFiltersFromString(*args, **kwargs): ...
    @staticmethod
    def root(*args, **kwargs): ...
    @staticmethod
    def rootPath(*args, **kwargs): ...
    @staticmethod
    def searchPaths(*args, **kwargs): ...
    @staticmethod
    def separator(*args, **kwargs): ...
    @staticmethod
    def setCurrent(*args, **kwargs): ...
    @staticmethod
    def setSearchPaths(*args, **kwargs): ...
    @staticmethod
    def temp(*args, **kwargs): ...
    @staticmethod
    def tempPath(*args, **kwargs): ...
    @staticmethod
    def toNativeSeparators(*args, **kwargs): ...
    AccessMask : Filter
    
    AllDirs : Filter
    
    AllEntries : Filter
    
    CaseSensitive : Filter
    
    Dirs : Filter
    
    DirsFirst : SortFlag
    
    DirsLast : SortFlag
    
    Drives : Filter
    
    Executable : Filter
    
    Files : Filter
    
    Filter : Type[Filter]
    
    Filters : Type[Filters]
    
    Hidden : Filter
    
    IgnoreCase : SortFlag
    
    LocaleAware : SortFlag
    
    Modified : Filter
    
    Name : SortFlag
    
    NoDot : Filter
    
    NoDotAndDotDot : Filter
    
    NoDotDot : Filter
    
    NoFilter : Filter
    
    NoSort : SortFlag
    
    NoSymLinks : Filter
    
    PermissionMask : Filter
    
    Readable : Filter
    
    Reversed : SortFlag
    
    Size : SortFlag
    
    SortByMask : SortFlag
    
    SortFlag : Type[SortFlag]
    
    SortFlags : Type[SortFlags]
    
    System : Filter
    
    Time : SortFlag
    
    Type : SortFlag
    
    TypeMask : Filter
    
    Unsorted : SortFlag
    
    Writable : Filter
    
    __new__ : builtin_function_or_method


class QMimeType(_Object):
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
    def aliases(*args, **kwargs): ...
    def allAncestors(*args, **kwargs): ...
    def comment(*args, **kwargs): ...
    def filterString(*args, **kwargs): ...
    def genericIconName(*args, **kwargs): ...
    def globPatterns(*args, **kwargs): ...
    def iconName(*args, **kwargs): ...
    def inherits(*args, **kwargs): ...
    def isDefault(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def parentMimeTypes(*args, **kwargs): ...
    def preferredSuffix(*args, **kwargs): ...
    def suffixes(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QRect(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __and__(*args, **kwargs):
        """
        x.__and__(y) <==> x&y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
        """
        ...
    def __iand__(*args, **kwargs):
        """
        x.__iand__(y) <==> x&=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __ior__(*args, **kwargs):
        """
        x.__ior__(y) <==> x|=y
        """
        ...
    def __isub__(*args, **kwargs):
        """
        x.__isub__(y) <==> x-=y
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
    def __or__(*args, **kwargs):
        """
        x.__or__(y) <==> x|y
        """
        ...
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rand__(*args, **kwargs):
        """
        x.__rand__(y) <==> y&x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __ror__(*args, **kwargs):
        """
        x.__ror__(y) <==> y|x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def adjust(*args, **kwargs): ...
    def adjusted(*args, **kwargs): ...
    def bottom(*args, **kwargs): ...
    def bottomLeft(*args, **kwargs): ...
    def bottomRight(*args, **kwargs): ...
    def center(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def getCoords(*args, **kwargs): ...
    def getRect(*args, **kwargs): ...
    def height(*args, **kwargs): ...
    def intersected(*args, **kwargs): ...
    def intersects(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def left(*args, **kwargs): ...
    def marginsAdded(*args, **kwargs): ...
    def marginsRemoved(*args, **kwargs): ...
    def moveBottom(*args, **kwargs): ...
    def moveBottomLeft(*args, **kwargs): ...
    def moveBottomRight(*args, **kwargs): ...
    def moveCenter(*args, **kwargs): ...
    def moveLeft(*args, **kwargs): ...
    def moveRight(*args, **kwargs): ...
    def moveTo(*args, **kwargs): ...
    def moveTop(*args, **kwargs): ...
    def moveTopLeft(*args, **kwargs): ...
    def moveTopRight(*args, **kwargs): ...
    def normalized(*args, **kwargs): ...
    def right(*args, **kwargs): ...
    def setBottom(*args, **kwargs): ...
    def setBottomLeft(*args, **kwargs): ...
    def setBottomRight(*args, **kwargs): ...
    def setCoords(*args, **kwargs): ...
    def setHeight(*args, **kwargs): ...
    def setLeft(*args, **kwargs): ...
    def setRect(*args, **kwargs): ...
    def setRight(*args, **kwargs): ...
    def setSize(*args, **kwargs): ...
    def setTop(*args, **kwargs): ...
    def setTopLeft(*args, **kwargs): ...
    def setTopRight(*args, **kwargs): ...
    def setWidth(*args, **kwargs): ...
    def setX(*args, **kwargs): ...
    def setY(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def top(*args, **kwargs): ...
    def topLeft(*args, **kwargs): ...
    def topRight(*args, **kwargs): ...
    def translate(*args, **kwargs): ...
    def translated(*args, **kwargs): ...
    def united(*args, **kwargs): ...
    def width(*args, **kwargs): ...
    def x(*args, **kwargs): ...
    def y(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QLineF(_Object):
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def angle(*args, **kwargs): ...
    def angleTo(*args, **kwargs): ...
    def dx(*args, **kwargs): ...
    def dy(*args, **kwargs): ...
    def intersect(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def length(*args, **kwargs): ...
    def normalVector(*args, **kwargs): ...
    def p1(*args, **kwargs): ...
    def p2(*args, **kwargs): ...
    def pointAt(*args, **kwargs): ...
    def setAngle(*args, **kwargs): ...
    def setLength(*args, **kwargs): ...
    def setLine(*args, **kwargs): ...
    def setP1(*args, **kwargs): ...
    def setP2(*args, **kwargs): ...
    def setPoints(*args, **kwargs): ...
    def toLine(*args, **kwargs): ...
    def toTuple(*args, **kwargs): ...
    def translate(*args, **kwargs): ...
    def translated(*args, **kwargs): ...
    def unitVector(*args, **kwargs): ...
    def x1(*args, **kwargs): ...
    def x2(*args, **kwargs): ...
    def y1(*args, **kwargs): ...
    def y2(*args, **kwargs): ...
    @staticmethod
    def fromPolar(*args, **kwargs): ...
    BoundedIntersection : IntersectType
    
    IntersectType : Type[IntersectType]
    
    NoIntersection : IntersectType
    
    UnboundedIntersection : IntersectType
    
    __new__ : builtin_function_or_method


class QGenericArgument(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def data(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QTextStreamManipulator(_Object):
    def __copy__(*args, **kwargs): ...
    def exec_(*args, **kwargs): ...


class QPersistentModelIndex(_Object):
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
    def child(*args, **kwargs): ...
    def column(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def internalId(*args, **kwargs): ...
    def internalPointer(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def model(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def row(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QRunnable(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def autoDelete(*args, **kwargs): ...
    def run(*args, **kwargs): ...
    def setAutoDelete(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QMetaProperty(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def enumerator(*args, **kwargs): ...
    def hasNotifySignal(*args, **kwargs): ...
    def hasStdCppSet(*args, **kwargs): ...
    def isConstant(*args, **kwargs): ...
    def isDesignable(*args, **kwargs): ...
    def isEditable(*args, **kwargs): ...
    def isEnumType(*args, **kwargs): ...
    def isFinal(*args, **kwargs): ...
    def isFlagType(*args, **kwargs): ...
    def isReadable(*args, **kwargs): ...
    def isResettable(*args, **kwargs): ...
    def isScriptable(*args, **kwargs): ...
    def isStored(*args, **kwargs): ...
    def isUser(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def isWritable(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def notifySignal(*args, **kwargs): ...
    def notifySignalIndex(*args, **kwargs): ...
    def propertyIndex(*args, **kwargs): ...
    def read(*args, **kwargs): ...
    def readOnGadget(*args, **kwargs): ...
    def reset(*args, **kwargs): ...
    def resetOnGadget(*args, **kwargs): ...
    def revision(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    def typeName(*args, **kwargs): ...
    def userType(*args, **kwargs): ...
    def write(*args, **kwargs): ...
    def writeOnGadget(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QCollatorSortKey(_Object):
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
    def compare(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QEvent(_Object):
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
    def accept(*args, **kwargs): ...
    def ignore(*args, **kwargs): ...
    def isAccepted(*args, **kwargs): ...
    def setAccepted(*args, **kwargs): ...
    def spontaneous(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    @staticmethod
    def registerEventType(*args, **kwargs): ...
    AcceptDropsChange : Type
    
    ActionAdded : Type
    
    ActionChanged : Type
    
    ActionRemoved : Type
    
    ActivateControl : Type
    
    ActivationChange : Type
    
    ApplicationActivate : Type
    
    ApplicationActivated : Type
    
    ApplicationDeactivate : Type
    
    ApplicationDeactivated : Type
    
    ApplicationFontChange : Type
    
    ApplicationLayoutDirectionChange : Type
    
    ApplicationPaletteChange : Type
    
    ApplicationStateChange : Type
    
    ApplicationWindowIconChange : Type
    
    ChildAdded : Type
    
    ChildPolished : Type
    
    ChildRemoved : Type
    
    Clipboard : Type
    
    Close : Type
    
    CloseSoftwareInputPanel : Type
    
    ContentsRectChange : Type
    
    ContextMenu : Type
    
    Create : Type
    
    CursorChange : Type
    
    DeactivateControl : Type
    
    DeferredDelete : Type
    
    Destroy : Type
    
    DragEnter : Type
    
    DragLeave : Type
    
    DragMove : Type
    
    DragResponse : Type
    
    Drop : Type
    
    DynamicPropertyChange : Type
    
    EmbeddingControl : Type
    
    EnabledChange : Type
    
    Enter : Type
    
    EnterWhatsThisMode : Type
    
    Expose : Type
    
    FileOpen : Type
    
    FocusAboutToChange : Type
    
    FocusIn : Type
    
    FocusOut : Type
    
    FontChange : Type
    
    FutureCallOut : Type
    
    Gesture : Type
    
    GestureOverride : Type
    
    GrabKeyboard : Type
    
    GrabMouse : Type
    
    GraphicsSceneContextMenu : Type
    
    GraphicsSceneDragEnter : Type
    
    GraphicsSceneDragLeave : Type
    
    GraphicsSceneDragMove : Type
    
    GraphicsSceneDrop : Type
    
    GraphicsSceneHelp : Type
    
    GraphicsSceneHoverEnter : Type
    
    GraphicsSceneHoverLeave : Type
    
    GraphicsSceneHoverMove : Type
    
    GraphicsSceneMouseDoubleClick : Type
    
    GraphicsSceneMouseMove : Type
    
    GraphicsSceneMousePress : Type
    
    GraphicsSceneMouseRelease : Type
    
    GraphicsSceneMove : Type
    
    GraphicsSceneResize : Type
    
    GraphicsSceneWheel : Type
    
    HelpRequest : Type
    
    Hide : Type
    
    HideToParent : Type
    
    HoverEnter : Type
    
    HoverLeave : Type
    
    HoverMove : Type
    
    IconDrag : Type
    
    IconTextChange : Type
    
    InputMethod : Type
    
    InputMethodQuery : Type
    
    KeyPress : Type
    
    KeyRelease : Type
    
    KeyboardLayoutChange : Type
    
    LanguageChange : Type
    
    LayoutDirectionChange : Type
    
    LayoutRequest : Type
    
    Leave : Type
    
    LeaveWhatsThisMode : Type
    
    LocaleChange : Type
    
    MacGLClearDrawable : Type
    
    MacGLWindowChange : Type
    
    MacSizeChange : Type
    
    MaxUser : Type
    
    MetaCall : Type
    
    ModifiedChange : Type
    
    MouseButtonDblClick : Type
    
    MouseButtonPress : Type
    
    MouseButtonRelease : Type
    
    MouseMove : Type
    
    MouseTrackingChange : Type
    
    Move : Type
    
    NativeGesture : Type
    
    NetworkReplyUpdated : Type
    
    NonClientAreaMouseButtonDblClick : Type
    
    NonClientAreaMouseButtonPress : Type
    
    NonClientAreaMouseButtonRelease : Type
    
    NonClientAreaMouseMove : Type
    
    locals()['None'] : Type
    
    OkRequest : Type
    
    OrientationChange : Type
    
    Paint : Type
    
    PaletteChange : Type
    
    ParentAboutToChange : Type
    
    ParentChange : Type
    
    PlatformPanel : Type
    
    PlatformSurface : Type
    
    Polish : Type
    
    PolishRequest : Type
    
    QueryWhatsThis : Type
    
    Quit : Type
    
    ReadOnlyChange : Type
    
    RequestSoftwareInputPanel : Type
    
    Resize : Type
    
    ScreenChangeInternal : Type
    
    Scroll : Type
    
    ScrollPrepare : Type
    
    Shortcut : Type
    
    ShortcutOverride : Type
    
    Show : Type
    
    ShowToParent : Type
    
    ShowWindowRequest : Type
    
    SockAct : Type
    
    SockClose : Type
    
    Speech : Type
    
    StateMachineSignal : Type
    
    StateMachineWrapped : Type
    
    StatusTip : Type
    
    Style : Type
    
    StyleAnimationUpdate : Type
    
    StyleChange : Type
    
    TabletEnterProximity : Type
    
    TabletLeaveProximity : Type
    
    TabletMove : Type
    
    TabletPress : Type
    
    TabletRelease : Type
    
    ThemeChange : Type
    
    ThreadChange : Type
    
    Timer : Type
    
    ToolBarChange : Type
    
    ToolTip : Type
    
    ToolTipChange : Type
    
    TouchBegin : Type
    
    TouchCancel : Type
    
    TouchEnd : Type
    
    TouchUpdate : Type
    
    Type : Type[Type]
    
    UngrabKeyboard : Type
    
    UngrabMouse : Type
    
    UpdateLater : Type
    
    UpdateRequest : Type
    
    User : Type
    
    WhatsThis : Type
    
    WhatsThisClicked : Type
    
    Wheel : Type
    
    WinEventAct : Type
    
    WinIdChange : Type
    
    WindowActivate : Type
    
    WindowBlocked : Type
    
    WindowChangeInternal : Type
    
    WindowDeactivate : Type
    
    WindowIconChange : Type
    
    WindowStateChange : Type
    
    WindowTitleChange : Type
    
    WindowUnblocked : Type
    
    ZOrderChange : Type
    
    ZeroTimerEvent : Type
    
    __new__ : builtin_function_or_method


class QSysInfo(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    @staticmethod
    def buildAbi(*args, **kwargs): ...
    @staticmethod
    def buildCpuArchitecture(*args, **kwargs): ...
    @staticmethod
    def currentCpuArchitecture(*args, **kwargs): ...
    @staticmethod
    def kernelType(*args, **kwargs): ...
    @staticmethod
    def kernelVersion(*args, **kwargs): ...
    @staticmethod
    def machineHostName(*args, **kwargs): ...
    @staticmethod
    def prettyProductName(*args, **kwargs): ...
    @staticmethod
    def productType(*args, **kwargs): ...
    @staticmethod
    def productVersion(*args, **kwargs): ...
    @staticmethod
    def windowsVersion(*args, **kwargs): ...
    BigEndian : Endian
    
    ByteOrder : Endian
    
    Endian : Type[Endian]
    
    LittleEndian : Endian
    
    Sizes : Type[Sizes]
    
    WV_10_0 : WinVersion
    
    WV_2000 : WinVersion
    
    WV_2003 : WinVersion
    
    WV_32s : WinVersion
    
    WV_4_0 : WinVersion
    
    WV_5_0 : WinVersion
    
    WV_5_1 : WinVersion
    
    WV_5_2 : WinVersion
    
    WV_6_0 : WinVersion
    
    WV_6_1 : WinVersion
    
    WV_6_2 : WinVersion
    
    WV_6_3 : WinVersion
    
    WV_95 : WinVersion
    
    WV_98 : WinVersion
    
    WV_CE : WinVersion
    
    WV_CENET : WinVersion
    
    WV_CE_5 : WinVersion
    
    WV_CE_6 : WinVersion
    
    WV_CE_based : WinVersion
    
    WV_DOS_based : WinVersion
    
    WV_Me : WinVersion
    
    WV_NT : WinVersion
    
    WV_NT_based : WinVersion
    
    WV_None : WinVersion
    
    WV_VISTA : WinVersion
    
    WV_WINDOWS10 : WinVersion
    
    WV_WINDOWS7 : WinVersion
    
    WV_WINDOWS8 : WinVersion
    
    WV_WINDOWS8_1 : WinVersion
    
    WV_XP : WinVersion
    
    WinVersion : Type[WinVersion]
    
    WindowsVersion : WinVersion
    
    WordSize : Sizes
    
    __new__ : builtin_function_or_method


class QJsonDocument(_Object):
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
    def array(*args, **kwargs): ...
    def isArray(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isObject(*args, **kwargs): ...
    def object(*args, **kwargs): ...
    def rawData(*args, **kwargs): ...
    def setArray(*args, **kwargs): ...
    def setObject(*args, **kwargs): ...
    def toBinaryData(*args, **kwargs): ...
    def toJson(*args, **kwargs): ...
    def toVariant(*args, **kwargs): ...
    @staticmethod
    def fromBinaryData(*args, **kwargs): ...
    @staticmethod
    def fromJson(*args, **kwargs): ...
    @staticmethod
    def fromRawData(*args, **kwargs): ...
    @staticmethod
    def fromVariant(*args, **kwargs): ...
    BypassValidation : DataValidation
    
    Compact : JsonFormat
    
    DataValidation : Type[DataValidation]
    
    Indented : JsonFormat
    
    JsonFormat : Type[JsonFormat]
    
    Validate : DataValidation
    
    __new__ : builtin_function_or_method


class QDataStream(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __lshift__(*args, **kwargs):
        """
        x.__lshift__(y) <==> x<<y
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
    def atEnd(*args, **kwargs): ...
    def byteOrder(*args, **kwargs): ...
    def device(*args, **kwargs): ...
    def floatingPointPrecision(*args, **kwargs): ...
    def readBool(*args, **kwargs): ...
    def readDouble(*args, **kwargs): ...
    def readFloat(*args, **kwargs): ...
    def readInt16(*args, **kwargs): ...
    def readInt32(*args, **kwargs): ...
    def readInt64(*args, **kwargs): ...
    def readInt8(*args, **kwargs): ...
    def readQChar(*args, **kwargs): ...
    def readQString(*args, **kwargs): ...
    def readQStringList(*args, **kwargs): ...
    def readQVariant(*args, **kwargs): ...
    def readRawData(*args, **kwargs): ...
    def readString(*args, **kwargs): ...
    def readUInt16(*args, **kwargs): ...
    def readUInt32(*args, **kwargs): ...
    def readUInt64(*args, **kwargs): ...
    def readUInt8(*args, **kwargs): ...
    def resetStatus(*args, **kwargs): ...
    def setByteOrder(*args, **kwargs): ...
    def setDevice(*args, **kwargs): ...
    def setFloatingPointPrecision(*args, **kwargs): ...
    def setStatus(*args, **kwargs): ...
    def setVersion(*args, **kwargs): ...
    def skipRawData(*args, **kwargs): ...
    def status(*args, **kwargs): ...
    def unsetDevice(*args, **kwargs): ...
    def version(*args, **kwargs): ...
    def writeBool(*args, **kwargs): ...
    def writeDouble(*args, **kwargs): ...
    def writeFloat(*args, **kwargs): ...
    def writeInt16(*args, **kwargs): ...
    def writeInt32(*args, **kwargs): ...
    def writeInt64(*args, **kwargs): ...
    def writeInt8(*args, **kwargs): ...
    def writeQChar(*args, **kwargs): ...
    def writeQString(*args, **kwargs): ...
    def writeQStringList(*args, **kwargs): ...
    def writeQVariant(*args, **kwargs): ...
    def writeRawData(*args, **kwargs): ...
    def writeString(*args, **kwargs): ...
    def writeUInt16(*args, **kwargs): ...
    def writeUInt32(*args, **kwargs): ...
    def writeUInt64(*args, **kwargs): ...
    def writeUInt8(*args, **kwargs): ...
    BigEndian : ByteOrder
    
    ByteOrder : Type[ByteOrder]
    
    DoublePrecision : FloatingPointPrecision
    
    FloatingPointPrecision : Type[FloatingPointPrecision]
    
    LittleEndian : ByteOrder
    
    Ok : Status
    
    Qt_1_0 : Version
    
    Qt_2_0 : Version
    
    Qt_2_1 : Version
    
    Qt_3_0 : Version
    
    Qt_3_1 : Version
    
    Qt_3_3 : Version
    
    Qt_4_0 : Version
    
    Qt_4_1 : Version
    
    Qt_4_2 : Version
    
    Qt_4_3 : Version
    
    Qt_4_4 : Version
    
    Qt_4_5 : Version
    
    Qt_4_6 : Version
    
    Qt_4_7 : Version
    
    Qt_4_8 : Version
    
    Qt_4_9 : Version
    
    Qt_5_0 : Version
    
    Qt_5_1 : Version
    
    Qt_5_2 : Version
    
    Qt_5_3 : Version
    
    Qt_5_4 : Version
    
    Qt_5_5 : Version
    
    Qt_5_6 : Version
    
    Qt_DefaultCompiledVersion : Version
    
    ReadCorruptData : Status
    
    ReadPastEnd : Status
    
    SinglePrecision : FloatingPointPrecision
    
    Status : Type[Status]
    
    Version : Type[Version]
    
    WriteFailed : Status
    
    __new__ : builtin_function_or_method


class QMetaEnum(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def isFlag(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def key(*args, **kwargs): ...
    def keyCount(*args, **kwargs): ...
    def keyToValue(*args, **kwargs): ...
    def keysToValue(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def scope(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    def valueToKey(*args, **kwargs): ...
    def valueToKeys(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QXmlStreamEntityDeclaration(_Object):
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
    def name(*args, **kwargs): ...
    def notationName(*args, **kwargs): ...
    def publicId(*args, **kwargs): ...
    def systemId(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QSize(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __div__(*args, **kwargs):
        """
        x.__div__(y) <==> x/y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
        """
        ...
    def __idiv__(*args, **kwargs):
        """
        x.__idiv__(y) <==> x/=y
        """
        ...
    def __imul__(*args, **kwargs):
        """
        x.__imul__(y) <==> x*=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __isub__(*args, **kwargs):
        """
        x.__isub__(y) <==> x-=y
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
    def __mul__(*args, **kwargs):
        """
        x.__mul__(y) <==> x*y
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
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rdiv__(*args, **kwargs):
        """
        x.__rdiv__(y) <==> y/x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rmul__(*args, **kwargs):
        """
        x.__rmul__(y) <==> y*x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __rtruediv__(*args, **kwargs):
        """
        x.__rtruediv__(y) <==> y/x
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def __truediv__(*args, **kwargs):
        """
        x.__truediv__(y) <==> x/y
        """
        ...
    def boundedTo(*args, **kwargs): ...
    def expandedTo(*args, **kwargs): ...
    def height(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def scale(*args, **kwargs): ...
    def scaled(*args, **kwargs): ...
    def setHeight(*args, **kwargs): ...
    def setWidth(*args, **kwargs): ...
    def toTuple(*args, **kwargs): ...
    def transpose(*args, **kwargs): ...
    def transposed(*args, **kwargs): ...
    def width(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QDate(_Object):
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
    def __getattribute__(*args, **kwargs):
        """
        x.__getattribute__('name') <==> x.name
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def addDays(*args, **kwargs): ...
    def addMonths(*args, **kwargs): ...
    def addYears(*args, **kwargs): ...
    def day(*args, **kwargs): ...
    def dayOfWeek(*args, **kwargs): ...
    def dayOfYear(*args, **kwargs): ...
    def daysInMonth(*args, **kwargs): ...
    def daysInYear(*args, **kwargs): ...
    def daysTo(*args, **kwargs): ...
    def getDate(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def month(*args, **kwargs): ...
    def setDate(*args, **kwargs): ...
    def toJulianDay(*args, **kwargs): ...
    def toPython(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    def weekNumber(*args, **kwargs): ...
    def year(*args, **kwargs): ...
    @staticmethod
    def currentDate(*args, **kwargs): ...
    @staticmethod
    def fromJulianDay(*args, **kwargs): ...
    @staticmethod
    def fromString(*args, **kwargs): ...
    @staticmethod
    def isLeapYear(*args, **kwargs): ...
    @staticmethod
    def isValid(*args, **kwargs): ...
    @staticmethod
    def longDayName(*args, **kwargs): ...
    @staticmethod
    def longMonthName(*args, **kwargs): ...
    @staticmethod
    def shortDayName(*args, **kwargs): ...
    @staticmethod
    def shortMonthName(*args, **kwargs): ...
    DateFormat : MonthNameType
    
    MonthNameType : Type[MonthNameType]
    
    StandaloneFormat : MonthNameType
    
    __new__ : builtin_function_or_method


class QMutexLocker(_Object):
    def __enter__(*args, **kwargs): ...
    def __exit__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def mutex(*args, **kwargs): ...
    def relock(*args, **kwargs): ...
    def unlock(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QSemaphore(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def acquire(*args, **kwargs): ...
    def available(*args, **kwargs): ...
    def release(*args, **kwargs): ...
    def tryAcquire(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QMetaClassInfo(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def name(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QTextCodec(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def aliases(*args, **kwargs): ...
    def canEncode(*args, **kwargs): ...
    def convertToUnicode(*args, **kwargs): ...
    def fromUnicode(*args, **kwargs): ...
    def makeDecoder(*args, **kwargs): ...
    def makeEncoder(*args, **kwargs): ...
    def mibEnum(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def toUnicode(*args, **kwargs): ...
    @staticmethod
    def availableCodecs(*args, **kwargs): ...
    @staticmethod
    def availableMibs(*args, **kwargs): ...
    @staticmethod
    def codecForHtml(*args, **kwargs): ...
    @staticmethod
    def codecForLocale(*args, **kwargs): ...
    @staticmethod
    def codecForMib(*args, **kwargs): ...
    @staticmethod
    def codecForName(*args, **kwargs): ...
    @staticmethod
    def codecForUtfText(*args, **kwargs): ...
    @staticmethod
    def setCodecForLocale(*args, **kwargs): ...
    ConversionFlag : Type[ConversionFlag]
    
    ConversionFlags : Type[ConversionFlags]
    
    ConvertInvalidToNull : ConversionFlag
    
    ConverterState : Type[ConverterState]
    
    DefaultConversion : ConversionFlag
    
    FreeFunction : ConversionFlag
    
    IgnoreHeader : ConversionFlag
    
    __new__ : builtin_function_or_method


class QXmlStreamWriter(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def autoFormatting(*args, **kwargs): ...
    def autoFormattingIndent(*args, **kwargs): ...
    def codec(*args, **kwargs): ...
    def device(*args, **kwargs): ...
    def hasError(*args, **kwargs): ...
    def setAutoFormatting(*args, **kwargs): ...
    def setAutoFormattingIndent(*args, **kwargs): ...
    def setCodec(*args, **kwargs): ...
    def setDevice(*args, **kwargs): ...
    def writeAttribute(*args, **kwargs): ...
    def writeAttributes(*args, **kwargs): ...
    def writeCDATA(*args, **kwargs): ...
    def writeCharacters(*args, **kwargs): ...
    def writeComment(*args, **kwargs): ...
    def writeCurrentToken(*args, **kwargs): ...
    def writeDTD(*args, **kwargs): ...
    def writeDefaultNamespace(*args, **kwargs): ...
    def writeEmptyElement(*args, **kwargs): ...
    def writeEndDocument(*args, **kwargs): ...
    def writeEndElement(*args, **kwargs): ...
    def writeEntityReference(*args, **kwargs): ...
    def writeNamespace(*args, **kwargs): ...
    def writeProcessingInstruction(*args, **kwargs): ...
    def writeStartDocument(*args, **kwargs): ...
    def writeStartElement(*args, **kwargs): ...
    def writeTextElement(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QDirIterator(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def fileInfo(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def filePath(*args, **kwargs): ...
    def hasNext(*args, **kwargs): ...
    def next(*args, **kwargs): ...
    def path(*args, **kwargs): ...
    FollowSymlinks : IteratorFlag
    
    IteratorFlag : Type[IteratorFlag]
    
    IteratorFlags : Type[IteratorFlags]
    
    NoIteratorFlags : IteratorFlag
    
    Subdirectories : IteratorFlag
    
    __new__ : builtin_function_or_method


class Qt(_Object):
    @staticmethod
    def qt_getEnumMetaObject(*args, **kwargs): ...
    @staticmethod
    def qt_getEnumName(*args, **kwargs): ...
    AA_AttributeCount : ApplicationAttribute
    
    AA_CompressHighFrequencyEvents : ApplicationAttribute
    
    AA_CompressTabletEvents : ApplicationAttribute
    
    AA_DisableHighDpiScaling : ApplicationAttribute
    
    AA_DontCreateNativeWidgetSiblings : ApplicationAttribute
    
    AA_DontShowIconsInMenus : ApplicationAttribute
    
    AA_DontUseNativeMenuBar : ApplicationAttribute
    
    AA_EnableHighDpiScaling : ApplicationAttribute
    
    AA_ForceRasterWidgets : ApplicationAttribute
    
    AA_ImmediateWidgetCreation : ApplicationAttribute
    
    AA_MSWindowsUseDirect3DByDefault : ApplicationAttribute
    
    AA_MacDontSwapCtrlAndMeta : ApplicationAttribute
    
    AA_MacPluginApplication : ApplicationAttribute
    
    AA_NativeWindows : ApplicationAttribute
    
    AA_SetPalette : ApplicationAttribute
    
    AA_ShareOpenGLContexts : ApplicationAttribute
    
    AA_SynthesizeMouseForUnhandledTouchEvents : ApplicationAttribute
    
    AA_SynthesizeTouchForUnhandledMouseEvents : ApplicationAttribute
    
    AA_Use96Dpi : ApplicationAttribute
    
    AA_UseDesktopOpenGL : ApplicationAttribute
    
    AA_UseHighDpiPixmaps : ApplicationAttribute
    
    AA_UseOpenGLES : ApplicationAttribute
    
    AA_UseSoftwareOpenGL : ApplicationAttribute
    
    AA_X11InitThreads : ApplicationAttribute
    
    ALT : Modifier
    
    AbsoluteSize : SizeMode
    
    AccessibleDescriptionRole : ItemDataRole
    
    AccessibleTextRole : ItemDataRole
    
    ActionMask : DropAction
    
    ActionsContextMenu : ContextMenuPolicy
    
    ActiveWindowFocusReason : FocusReason
    
    AddToSelection : ItemSelectionOperation
    
    AlignAbsolute : AlignmentFlag
    
    AlignBaseline : AlignmentFlag
    
    AlignBottom : AlignmentFlag
    
    AlignCenter : AlignmentFlag
    
    AlignHCenter : AlignmentFlag
    
    AlignHorizontal_Mask : AlignmentFlag
    
    AlignJustify : AlignmentFlag
    
    AlignLeading : AlignmentFlag
    
    AlignLeft : AlignmentFlag
    
    AlignRight : AlignmentFlag
    
    AlignTop : AlignmentFlag
    
    AlignTrailing : AlignmentFlag
    
    AlignVCenter : AlignmentFlag
    
    AlignVertical_Mask : AlignmentFlag
    
    Alignment : Type[Alignment]
    
    AlignmentFlag : Type[AlignmentFlag]
    
    AllButtons : MouseButton
    
    AllDockWidgetAreas : DockWidgetArea
    
    AllToolBarAreas : ToolBarArea
    
    AlphaDither_Mask : ImageConversionFlag
    
    AltModifier : KeyboardModifier
    
    AnchorBottom : AnchorPoint
    
    AnchorHorizontalCenter : AnchorPoint
    
    AnchorLeft : AnchorPoint
    
    AnchorPoint : Type[AnchorPoint]
    
    AnchorRight : AnchorPoint
    
    AnchorTop : AnchorPoint
    
    AnchorVerticalCenter : AnchorPoint
    
    ApplicationActive : ApplicationState
    
    ApplicationAttribute : Type[ApplicationAttribute]
    
    ApplicationHidden : ApplicationState
    
    ApplicationInactive : ApplicationState
    
    ApplicationModal : WindowModality
    
    ApplicationShortcut : ShortcutContext
    
    ApplicationState : Type[ApplicationState]
    
    ApplicationStates : Type[ApplicationStates]
    
    ApplicationSuspended : ApplicationState
    
    ArrowCursor : CursorShape
    
    ArrowType : Type[ArrowType]
    
    AscendingOrder : SortOrder
    
    AspectRatioMode : Type[AspectRatioMode]
    
    AutoColor : ImageConversionFlag
    
    AutoConnection : ConnectionType
    
    AutoDither : ImageConversionFlag
    
    AutoText : TextFormat
    
    AvoidDither : ImageConversionFlag
    
    Axis : Type[Axis]
    
    BDiagPattern : BrushStyle
    
    BGMode : Type[BGMode]
    
    BackButton : MouseButton
    
    BackgroundColorRole : ItemDataRole
    
    BackgroundRole : ItemDataRole
    
    BacktabFocusReason : FocusReason
    
    BeginNativeGesture : NativeGestureType
    
    BevelJoin : PenJoinStyle
    
    BitmapCursor : CursorShape
    
    BlankCursor : CursorShape
    
    BlockingQueuedConnection : ConnectionType
    
    BottomDockWidgetArea : DockWidgetArea
    
    BottomEdge : Edge
    
    BottomLeftCorner : Corner
    
    BottomLeftSection : WindowFrameSection
    
    BottomRightCorner : Corner
    
    BottomRightSection : WindowFrameSection
    
    BottomSection : WindowFrameSection
    
    BottomToolBarArea : ToolBarArea
    
    BrushStyle : Type[BrushStyle]
    
    BusyCursor : CursorShape
    
    BypassGraphicsProxyWidget : WindowType
    
    BypassWindowManagerHint : WindowType
    
    CTRL : Modifier
    
    CaseInsensitive : CaseSensitivity
    
    CaseSensitive : CaseSensitivity
    
    CaseSensitivity : Type[CaseSensitivity]
    
    CheckState : Type[CheckState]
    
    CheckStateRole : ItemDataRole
    
    Checked : CheckState
    
    ClickFocus : FocusPolicy
    
    ClipOperation : Type[ClipOperation]
    
    ClosedHandCursor : CursorShape
    
    CoarseTimer : TimerType
    
    ColorMode_Mask : ImageConversionFlag
    
    ColorOnly : ImageConversionFlag
    
    ConicalGradientPattern : BrushStyle
    
    ConnectionType : Type[ConnectionType]
    
    ContainsItemBoundingRect : ItemSelectionMode
    
    ContainsItemShape : ItemSelectionMode
    
    ContextMenuPolicy : Type[ContextMenuPolicy]
    
    ControlModifier : KeyboardModifier
    
    CoordinateSystem : Type[CoordinateSystem]
    
    CopyAction : DropAction
    
    Corner : Type[Corner]
    
    CoverWindow : WindowType
    
    CrossCursor : CursorShape
    
    CrossPattern : BrushStyle
    
    CursorMoveStyle : Type[CursorMoveStyle]
    
    CursorShape : Type[CursorShape]
    
    CustomContextMenu : ContextMenuPolicy
    
    CustomCursor : CursorShape
    
    CustomDashLine : PenStyle
    
    CustomGesture : GestureType
    
    CustomizeWindowHint : WindowType
    
    DashDotDotLine : PenStyle
    
    DashDotLine : PenStyle
    
    DashLine : PenStyle
    
    DateFormat : Type[DateFormat]
    
    DayOfWeek : Type[DayOfWeek]
    
    DecorationPropertyRole : ItemDataRole
    
    DecorationRole : ItemDataRole
    
    DefaultContextMenu : ContextMenuPolicy
    
    DefaultLocaleLongDate : DateFormat
    
    DefaultLocaleShortDate : DateFormat
    
    Dense1Pattern : BrushStyle
    
    Dense2Pattern : BrushStyle
    
    Dense3Pattern : BrushStyle
    
    Dense4Pattern : BrushStyle
    
    Dense5Pattern : BrushStyle
    
    Dense6Pattern : BrushStyle
    
    Dense7Pattern : BrushStyle
    
    DescendingOrder : SortOrder
    
    Desktop : WindowType
    
    DeviceCoordinates : CoordinateSystem
    
    DiagCrossPattern : BrushStyle
    
    Dialog : WindowType
    
    DiffuseAlphaDither : ImageConversionFlag
    
    DiffuseDither : ImageConversionFlag
    
    DirectConnection : ConnectionType
    
    DisplayPropertyRole : ItemDataRole
    
    DisplayRole : ItemDataRole
    
    DitherMode_Mask : ImageConversionFlag
    
    Dither_Mask : ImageConversionFlag
    
    DockWidgetArea : Type[DockWidgetArea]
    
    DockWidgetAreaSizes : Type[DockWidgetAreaSizes]
    
    DockWidgetArea_Mask : DockWidgetArea
    
    DockWidgetAreas : Type[DockWidgetAreas]
    
    DontStartGestureOnChildren : GestureFlag
    
    DotLine : PenStyle
    
    DownArrow : ArrowType
    
    DragCopyCursor : CursorShape
    
    DragLinkCursor : CursorShape
    
    DragMoveCursor : CursorShape
    
    Drawer : WindowType
    
    DropAction : Type[DropAction]
    
    DropActions : Type[DropActions]
    
    Edge : Type[Edge]
    
    Edges : Type[Edges]
    
    EditRole : ItemDataRole
    
    ElideLeft : TextElideMode
    
    ElideMiddle : TextElideMode
    
    ElideNone : TextElideMode
    
    ElideRight : TextElideMode
    
    EndNativeGesture : NativeGestureType
    
    EnterKeyDefault : EnterKeyType
    
    EnterKeyDone : EnterKeyType
    
    EnterKeyGo : EnterKeyType
    
    EnterKeyNext : EnterKeyType
    
    EnterKeyPrevious : EnterKeyType
    
    EnterKeyReturn : EnterKeyType
    
    EnterKeySearch : EnterKeyType
    
    EnterKeySend : EnterKeyType
    
    EnterKeyType : Type[EnterKeyType]
    
    EventPriority : Type[EventPriority]
    
    ExactHit : HitTestAccuracy
    
    ExtraButton1 : MouseButton
    
    ExtraButton10 : MouseButton
    
    ExtraButton11 : MouseButton
    
    ExtraButton12 : MouseButton
    
    ExtraButton13 : MouseButton
    
    ExtraButton14 : MouseButton
    
    ExtraButton15 : MouseButton
    
    ExtraButton16 : MouseButton
    
    ExtraButton17 : MouseButton
    
    ExtraButton18 : MouseButton
    
    ExtraButton19 : MouseButton
    
    ExtraButton2 : MouseButton
    
    ExtraButton20 : MouseButton
    
    ExtraButton21 : MouseButton
    
    ExtraButton22 : MouseButton
    
    ExtraButton23 : MouseButton
    
    ExtraButton24 : MouseButton
    
    ExtraButton3 : MouseButton
    
    ExtraButton4 : MouseButton
    
    ExtraButton5 : MouseButton
    
    ExtraButton6 : MouseButton
    
    ExtraButton7 : MouseButton
    
    ExtraButton8 : MouseButton
    
    ExtraButton9 : MouseButton
    
    FDiagPattern : BrushStyle
    
    FastTransformation : TransformationMode
    
    FillRule : Type[FillRule]
    
    FindChildOption : Type[FindChildOption]
    
    FindChildOptions : Type[FindChildOptions]
    
    FindChildrenRecursively : FindChildOption
    
    FindDirectChildrenOnly : FindChildOption
    
    FlatCap : PenCapStyle
    
    FocusPolicy : Type[FocusPolicy]
    
    FocusReason : Type[FocusReason]
    
    FontRole : ItemDataRole
    
    ForbiddenCursor : CursorShape
    
    ForegroundRole : ItemDataRole
    
    ForeignWindow : WindowType
    
    ForwardButton : MouseButton
    
    FramelessWindowHint : WindowType
    
    Friday : DayOfWeek
    
    FuzzyHit : HitTestAccuracy
    
    GestureCanceled : GestureState
    
    GestureFinished : GestureState
    
    GestureFlag : Type[GestureFlag]
    
    GestureFlags : Type[GestureFlags]
    
    GestureStarted : GestureState
    
    GestureState : Type[GestureState]
    
    GestureType : Type[GestureType]
    
    GestureUpdated : GestureState
    
    GlobalColor : Type[GlobalColor]
    
    GroupSwitchModifier : KeyboardModifier
    
    HighEventPriority : EventPriority
    
    HitTestAccuracy : Type[HitTestAccuracy]
    
    HorPattern : BrushStyle
    
    Horizontal : Orientation
    
    IBeamCursor : CursorShape
    
    ISODate : DateFormat
    
    IgnoreAction : DropAction
    
    IgnoreAspectRatio : AspectRatioMode
    
    IgnoredGesturesPropagateToParent : GestureFlag
    
    ImAbsolutePosition : InputMethodQuery
    
    ImAnchorPosition : InputMethodQuery
    
    ImCurrentSelection : InputMethodQuery
    
    ImCursorPosition : InputMethodQuery
    
    ImCursorRectangle : InputMethodQuery
    
    ImEnabled : InputMethodQuery
    
    ImEnterKeyType : InputMethodQuery
    
    ImFont : InputMethodQuery
    
    ImHints : InputMethodQuery
    
    ImMaximumTextLength : InputMethodQuery
    
    ImMicroFocus : InputMethodQuery
    
    ImPlatformData : InputMethodQuery
    
    ImPreferredLanguage : InputMethodQuery
    
    ImQueryAll : InputMethodQuery
    
    ImQueryInput : InputMethodQuery
    
    ImSurroundingText : InputMethodQuery
    
    ImTextAfterCursor : InputMethodQuery
    
    ImTextBeforeCursor : InputMethodQuery
    
    ImageConversionFlag : Type[ImageConversionFlag]
    
    ImageConversionFlags : Type[ImageConversionFlags]
    
    ImhDate : InputMethodHint
    
    ImhDialableCharactersOnly : InputMethodHint
    
    ImhDigitsOnly : InputMethodHint
    
    ImhEmailCharactersOnly : InputMethodHint
    
    ImhExclusiveInputMask : InputMethodHint
    
    ImhFormattedNumbersOnly : InputMethodHint
    
    ImhHiddenText : InputMethodHint
    
    ImhLatinOnly : InputMethodHint
    
    ImhLowercaseOnly : InputMethodHint
    
    ImhMultiLine : InputMethodHint
    
    ImhNoAutoUppercase : InputMethodHint
    
    ImhNoPredictiveText : InputMethodHint
    
    ImhNone : InputMethodHint
    
    ImhPreferLatin : InputMethodHint
    
    ImhPreferLowercase : InputMethodHint
    
    ImhPreferNumbers : InputMethodHint
    
    ImhPreferUppercase : InputMethodHint
    
    ImhSensitiveData : InputMethodHint
    
    ImhTime : InputMethodHint
    
    ImhUppercaseOnly : InputMethodHint
    
    ImhUrlCharactersOnly : InputMethodHint
    
    InitialSortOrderRole : ItemDataRole
    
    InputMethodHint : Type[InputMethodHint]
    
    InputMethodHints : Type[InputMethodHints]
    
    InputMethodQueries : Type[InputMethodQueries]
    
    InputMethodQuery : Type[InputMethodQuery]
    
    IntersectClip : ClipOperation
    
    IntersectsItemBoundingRect : ItemSelectionMode
    
    IntersectsItemShape : ItemSelectionMode
    
    InvertedLandscapeOrientation : ScreenOrientation
    
    InvertedPortraitOrientation : ScreenOrientation
    
    ItemDataRole : Type[ItemDataRole]
    
    ItemFlag : Type[ItemFlag]
    
    ItemFlags : Type[ItemFlags]
    
    ItemIsAutoTristate : ItemFlag
    
    ItemIsDragEnabled : ItemFlag
    
    ItemIsDropEnabled : ItemFlag
    
    ItemIsEditable : ItemFlag
    
    ItemIsEnabled : ItemFlag
    
    ItemIsSelectable : ItemFlag
    
    ItemIsTristate : ItemFlag
    
    ItemIsUserCheckable : ItemFlag
    
    ItemIsUserTristate : ItemFlag
    
    ItemNeverHasChildren : ItemFlag
    
    ItemSelectionMode : Type[ItemSelectionMode]
    
    ItemSelectionOperation : Type[ItemSelectionOperation]
    
    KeepAspectRatio : AspectRatioMode
    
    KeepAspectRatioByExpanding : AspectRatioMode
    
    Key : Type[Key]
    
    Key_0 : Key
    
    Key_1 : Key
    
    Key_2 : Key
    
    Key_3 : Key
    
    Key_4 : Key
    
    Key_5 : Key
    
    Key_6 : Key
    
    Key_7 : Key
    
    Key_8 : Key
    
    Key_9 : Key
    
    Key_A : Key
    
    Key_AE : Key
    
    Key_Aacute : Key
    
    Key_Acircumflex : Key
    
    Key_AddFavorite : Key
    
    Key_Adiaeresis : Key
    
    Key_Agrave : Key
    
    Key_Alt : Key
    
    Key_AltGr : Key
    
    Key_Ampersand : Key
    
    Key_Any : Key
    
    Key_Apostrophe : Key
    
    Key_ApplicationLeft : Key
    
    Key_ApplicationRight : Key
    
    Key_Aring : Key
    
    Key_AsciiCircum : Key
    
    Key_AsciiTilde : Key
    
    Key_Asterisk : Key
    
    Key_At : Key
    
    Key_Atilde : Key
    
    Key_AudioCycleTrack : Key
    
    Key_AudioForward : Key
    
    Key_AudioRandomPlay : Key
    
    Key_AudioRepeat : Key
    
    Key_AudioRewind : Key
    
    Key_Away : Key
    
    Key_B : Key
    
    Key_Back : Key
    
    Key_BackForward : Key
    
    Key_Backslash : Key
    
    Key_Backspace : Key
    
    Key_Backtab : Key
    
    Key_Bar : Key
    
    Key_BassBoost : Key
    
    Key_BassDown : Key
    
    Key_BassUp : Key
    
    Key_Battery : Key
    
    Key_Blue : Key
    
    Key_Bluetooth : Key
    
    Key_Book : Key
    
    Key_BraceLeft : Key
    
    Key_BraceRight : Key
    
    Key_BracketLeft : Key
    
    Key_BracketRight : Key
    
    Key_BrightnessAdjust : Key
    
    Key_C : Key
    
    Key_CD : Key
    
    Key_Calculator : Key
    
    Key_Calendar : Key
    
    Key_Call : Key
    
    Key_Camera : Key
    
    Key_CameraFocus : Key
    
    Key_Cancel : Key
    
    Key_CapsLock : Key
    
    Key_Ccedilla : Key
    
    Key_ChannelDown : Key
    
    Key_ChannelUp : Key
    
    Key_Clear : Key
    
    Key_ClearGrab : Key
    
    Key_Close : Key
    
    Key_Codeinput : Key
    
    Key_Colon : Key
    
    Key_Comma : Key
    
    Key_Community : Key
    
    Key_Context1 : Key
    
    Key_Context2 : Key
    
    Key_Context3 : Key
    
    Key_Context4 : Key
    
    Key_ContrastAdjust : Key
    
    Key_Control : Key
    
    Key_Copy : Key
    
    Key_Cut : Key
    
    Key_D : Key
    
    Key_DOS : Key
    
    Key_Dead_Abovedot : Key
    
    Key_Dead_Abovering : Key
    
    Key_Dead_Acute : Key
    
    Key_Dead_Belowdot : Key
    
    Key_Dead_Breve : Key
    
    Key_Dead_Caron : Key
    
    Key_Dead_Cedilla : Key
    
    Key_Dead_Circumflex : Key
    
    Key_Dead_Diaeresis : Key
    
    Key_Dead_Doubleacute : Key
    
    Key_Dead_Grave : Key
    
    Key_Dead_Hook : Key
    
    Key_Dead_Horn : Key
    
    Key_Dead_Iota : Key
    
    Key_Dead_Macron : Key
    
    Key_Dead_Ogonek : Key
    
    Key_Dead_Semivoiced_Sound : Key
    
    Key_Dead_Tilde : Key
    
    Key_Dead_Voiced_Sound : Key
    
    Key_Delete : Key
    
    Key_Direction_L : Key
    
    Key_Direction_R : Key
    
    Key_Display : Key
    
    Key_Documents : Key
    
    Key_Dollar : Key
    
    Key_Down : Key
    
    Key_E : Key
    
    Key_ETH : Key
    
    Key_Eacute : Key
    
    Key_Ecircumflex : Key
    
    Key_Ediaeresis : Key
    
    Key_Egrave : Key
    
    Key_Eisu_Shift : Key
    
    Key_Eisu_toggle : Key
    
    Key_Eject : Key
    
    Key_End : Key
    
    Key_Enter : Key
    
    Key_Equal : Key
    
    Key_Escape : Key
    
    Key_Excel : Key
    
    Key_Exclam : Key
    
    Key_Execute : Key
    
    Key_Exit : Key
    
    Key_Explorer : Key
    
    Key_F : Key
    
    Key_F1 : Key
    
    Key_F10 : Key
    
    Key_F11 : Key
    
    Key_F12 : Key
    
    Key_F13 : Key
    
    Key_F14 : Key
    
    Key_F15 : Key
    
    Key_F16 : Key
    
    Key_F17 : Key
    
    Key_F18 : Key
    
    Key_F19 : Key
    
    Key_F2 : Key
    
    Key_F20 : Key
    
    Key_F21 : Key
    
    Key_F22 : Key
    
    Key_F23 : Key
    
    Key_F24 : Key
    
    Key_F25 : Key
    
    Key_F26 : Key
    
    Key_F27 : Key
    
    Key_F28 : Key
    
    Key_F29 : Key
    
    Key_F3 : Key
    
    Key_F30 : Key
    
    Key_F31 : Key
    
    Key_F32 : Key
    
    Key_F33 : Key
    
    Key_F34 : Key
    
    Key_F35 : Key
    
    Key_F4 : Key
    
    Key_F5 : Key
    
    Key_F6 : Key
    
    Key_F7 : Key
    
    Key_F8 : Key
    
    Key_F9 : Key
    
    Key_Favorites : Key
    
    Key_Finance : Key
    
    Key_Find : Key
    
    Key_Flip : Key
    
    Key_Forward : Key
    
    Key_G : Key
    
    Key_Game : Key
    
    Key_Go : Key
    
    Key_Greater : Key
    
    Key_Green : Key
    
    Key_Guide : Key
    
    Key_H : Key
    
    Key_Hangul : Key
    
    Key_Hangul_Banja : Key
    
    Key_Hangul_End : Key
    
    Key_Hangul_Hanja : Key
    
    Key_Hangul_Jamo : Key
    
    Key_Hangul_Jeonja : Key
    
    Key_Hangul_PostHanja : Key
    
    Key_Hangul_PreHanja : Key
    
    Key_Hangul_Romaja : Key
    
    Key_Hangul_Special : Key
    
    Key_Hangul_Start : Key
    
    Key_Hangup : Key
    
    Key_Hankaku : Key
    
    Key_Help : Key
    
    Key_Henkan : Key
    
    Key_Hibernate : Key
    
    Key_Hiragana : Key
    
    Key_Hiragana_Katakana : Key
    
    Key_History : Key
    
    Key_Home : Key
    
    Key_HomePage : Key
    
    Key_HotLinks : Key
    
    Key_Hyper_L : Key
    
    Key_Hyper_R : Key
    
    Key_I : Key
    
    Key_Iacute : Key
    
    Key_Icircumflex : Key
    
    Key_Idiaeresis : Key
    
    Key_Igrave : Key
    
    Key_Info : Key
    
    Key_Insert : Key
    
    Key_J : Key
    
    Key_K : Key
    
    Key_Kana_Lock : Key
    
    Key_Kana_Shift : Key
    
    Key_Kanji : Key
    
    Key_Katakana : Key
    
    Key_KeyboardBrightnessDown : Key
    
    Key_KeyboardBrightnessUp : Key
    
    Key_KeyboardLightOnOff : Key
    
    Key_L : Key
    
    Key_LastNumberRedial : Key
    
    Key_Launch0 : Key
    
    Key_Launch1 : Key
    
    Key_Launch2 : Key
    
    Key_Launch3 : Key
    
    Key_Launch4 : Key
    
    Key_Launch5 : Key
    
    Key_Launch6 : Key
    
    Key_Launch7 : Key
    
    Key_Launch8 : Key
    
    Key_Launch9 : Key
    
    Key_LaunchA : Key
    
    Key_LaunchB : Key
    
    Key_LaunchC : Key
    
    Key_LaunchD : Key
    
    Key_LaunchE : Key
    
    Key_LaunchF : Key
    
    Key_LaunchG : Key
    
    Key_LaunchH : Key
    
    Key_LaunchMail : Key
    
    Key_LaunchMedia : Key
    
    Key_Left : Key
    
    Key_Less : Key
    
    Key_LightBulb : Key
    
    Key_LogOff : Key
    
    Key_M : Key
    
    Key_MailForward : Key
    
    Key_Market : Key
    
    Key_Massyo : Key
    
    Key_MediaLast : Key
    
    Key_MediaNext : Key
    
    Key_MediaPause : Key
    
    Key_MediaPlay : Key
    
    Key_MediaPrevious : Key
    
    Key_MediaRecord : Key
    
    Key_MediaStop : Key
    
    Key_MediaTogglePlayPause : Key
    
    Key_Meeting : Key
    
    Key_Memo : Key
    
    Key_Menu : Key
    
    Key_MenuKB : Key
    
    Key_MenuPB : Key
    
    Key_Messenger : Key
    
    Key_Meta : Key
    
    Key_MicMute : Key
    
    Key_MicVolumeDown : Key
    
    Key_MicVolumeUp : Key
    
    Key_Minus : Key
    
    Key_Mode_switch : Key
    
    Key_MonBrightnessDown : Key
    
    Key_MonBrightnessUp : Key
    
    Key_Muhenkan : Key
    
    Key_Multi_key : Key
    
    Key_MultipleCandidate : Key
    
    Key_Music : Key
    
    Key_MySites : Key
    
    Key_N : Key
    
    Key_New : Key
    
    Key_News : Key
    
    Key_No : Key
    
    Key_Ntilde : Key
    
    Key_NumLock : Key
    
    Key_NumberSign : Key
    
    Key_O : Key
    
    Key_Oacute : Key
    
    Key_Ocircumflex : Key
    
    Key_Odiaeresis : Key
    
    Key_OfficeHome : Key
    
    Key_Ograve : Key
    
    Key_Ooblique : Key
    
    Key_Open : Key
    
    Key_OpenUrl : Key
    
    Key_Option : Key
    
    Key_Otilde : Key
    
    Key_P : Key
    
    Key_PageDown : Key
    
    Key_PageUp : Key
    
    Key_ParenLeft : Key
    
    Key_ParenRight : Key
    
    Key_Paste : Key
    
    Key_Pause : Key
    
    Key_Percent : Key
    
    Key_Period : Key
    
    Key_Phone : Key
    
    Key_Pictures : Key
    
    Key_Play : Key
    
    Key_Plus : Key
    
    Key_PowerDown : Key
    
    Key_PowerOff : Key
    
    Key_PreviousCandidate : Key
    
    Key_Print : Key
    
    Key_Printer : Key
    
    Key_Q : Key
    
    Key_Question : Key
    
    Key_QuoteDbl : Key
    
    Key_QuoteLeft : Key
    
    Key_R : Key
    
    Key_Red : Key
    
    Key_Redo : Key
    
    Key_Refresh : Key
    
    Key_Reload : Key
    
    Key_Reply : Key
    
    Key_Return : Key
    
    Key_Right : Key
    
    Key_Romaji : Key
    
    Key_RotateWindows : Key
    
    Key_RotationKB : Key
    
    Key_RotationPB : Key
    
    Key_S : Key
    
    Key_Save : Key
    
    Key_ScreenSaver : Key
    
    Key_ScrollLock : Key
    
    Key_Search : Key
    
    Key_Select : Key
    
    Key_Semicolon : Key
    
    Key_Send : Key
    
    Key_Settings : Key
    
    Key_Shift : Key
    
    Key_Shop : Key
    
    Key_SingleCandidate : Key
    
    Key_Slash : Key
    
    Key_Sleep : Key
    
    Key_Space : Key
    
    Key_Spell : Key
    
    Key_SplitScreen : Key
    
    Key_Standby : Key
    
    Key_Stop : Key
    
    Key_Subtitle : Key
    
    Key_Super_L : Key
    
    Key_Super_R : Key
    
    Key_Support : Key
    
    Key_Suspend : Key
    
    Key_SysReq : Key
    
    Key_T : Key
    
    Key_THORN : Key
    
    Key_Tab : Key
    
    Key_TaskPane : Key
    
    Key_Terminal : Key
    
    Key_Time : Key
    
    Key_ToDoList : Key
    
    Key_ToggleCallHangup : Key
    
    Key_Tools : Key
    
    Key_TopMenu : Key
    
    Key_TouchpadOff : Key
    
    Key_TouchpadOn : Key
    
    Key_TouchpadToggle : Key
    
    Key_Touroku : Key
    
    Key_Travel : Key
    
    Key_TrebleDown : Key
    
    Key_TrebleUp : Key
    
    Key_U : Key
    
    Key_UWB : Key
    
    Key_Uacute : Key
    
    Key_Ucircumflex : Key
    
    Key_Udiaeresis : Key
    
    Key_Ugrave : Key
    
    Key_Underscore : Key
    
    Key_Undo : Key
    
    Key_Up : Key
    
    Key_V : Key
    
    Key_Video : Key
    
    Key_View : Key
    
    Key_VoiceDial : Key
    
    Key_VolumeDown : Key
    
    Key_VolumeMute : Key
    
    Key_VolumeUp : Key
    
    Key_W : Key
    
    Key_WLAN : Key
    
    Key_WWW : Key
    
    Key_WakeUp : Key
    
    Key_WebCam : Key
    
    Key_Word : Key
    
    Key_X : Key
    
    Key_Xfer : Key
    
    Key_Y : Key
    
    Key_Yacute : Key
    
    Key_Yellow : Key
    
    Key_Yes : Key
    
    Key_Z : Key
    
    Key_Zenkaku : Key
    
    Key_Zenkaku_Hankaku : Key
    
    Key_Zoom : Key
    
    Key_ZoomIn : Key
    
    Key_ZoomOut : Key
    
    Key_acute : Key
    
    Key_brokenbar : Key
    
    Key_cedilla : Key
    
    Key_cent : Key
    
    Key_copyright : Key
    
    Key_currency : Key
    
    Key_degree : Key
    
    Key_diaeresis : Key
    
    Key_division : Key
    
    Key_exclamdown : Key
    
    Key_guillemotleft : Key
    
    Key_guillemotright : Key
    
    Key_hyphen : Key
    
    Key_iTouch : Key
    
    Key_macron : Key
    
    Key_masculine : Key
    
    Key_mu : Key
    
    Key_multiply : Key
    
    Key_nobreakspace : Key
    
    Key_notsign : Key
    
    Key_onehalf : Key
    
    Key_onequarter : Key
    
    Key_onesuperior : Key
    
    Key_ordfeminine : Key
    
    Key_paragraph : Key
    
    Key_periodcentered : Key
    
    Key_plusminus : Key
    
    Key_questiondown : Key
    
    Key_registered : Key
    
    Key_section : Key
    
    Key_ssharp : Key
    
    Key_sterling : Key
    
    Key_threequarters : Key
    
    Key_threesuperior : Key
    
    Key_twosuperior : Key
    
    Key_unknown : Key
    
    Key_ydiaeresis : Key
    
    Key_yen : Key
    
    KeyboardModifier : Type[KeyboardModifier]
    
    KeyboardModifierMask : KeyboardModifier
    
    KeyboardModifiers : Type[KeyboardModifiers]
    
    KeypadModifier : KeyboardModifier
    
    LandscapeOrientation : ScreenOrientation
    
    LastCursor : CursorShape
    
    LastGestureType : GestureType
    
    LayoutDirection : Type[LayoutDirection]
    
    LayoutDirectionAuto : LayoutDirection
    
    LeftArrow : ArrowType
    
    LeftButton : MouseButton
    
    LeftDockWidgetArea : DockWidgetArea
    
    LeftEdge : Edge
    
    LeftSection : WindowFrameSection
    
    LeftToRight : LayoutDirection
    
    LeftToolBarArea : ToolBarArea
    
    LinearGradientPattern : BrushStyle
    
    LinkAction : DropAction
    
    LinksAccessibleByKeyboard : TextInteractionFlag
    
    LinksAccessibleByMouse : TextInteractionFlag
    
    LocalDate : DateFormat
    
    LocalTime : TimeSpec
    
    LocaleDate : DateFormat
    
    LogicalCoordinates : CoordinateSystem
    
    LogicalMoveStyle : CursorMoveStyle
    
    LowEventPriority : EventPriority
    
    META : Modifier
    
    MODIFIER_MASK : Modifier
    
    MPenCapStyle : PenCapStyle
    
    MPenJoinStyle : PenJoinStyle
    
    MPenStyle : PenStyle
    
    MSWindowsFixedSizeDialogHint : WindowType
    
    MSWindowsOwnDC : WindowType
    
    MacWindowToolBarButtonHint : WindowType
    
    MaskInColor : MaskMode
    
    MaskMode : Type[MaskMode]
    
    MaskOutColor : MaskMode
    
    MatchCaseSensitive : MatchFlag
    
    MatchContains : MatchFlag
    
    MatchEndsWith : MatchFlag
    
    MatchExactly : MatchFlag
    
    MatchFixedString : MatchFlag
    
    MatchFlag : Type[MatchFlag]
    
    MatchFlags : Type[MatchFlags]
    
    MatchRecursive : MatchFlag
    
    MatchRegExp : MatchFlag
    
    MatchStartsWith : MatchFlag
    
    MatchWildcard : MatchFlag
    
    MatchWrap : MatchFlag
    
    MaxMouseButton : MouseButton
    
    MaximizeUsingFullscreenGeometryHint : WindowType
    
    MaximumSize : SizeHint
    
    MenuBarFocusReason : FocusReason
    
    MetaModifier : KeyboardModifier
    
    MidButton : MouseButton
    
    MiddleButton : MouseButton
    
    MinimumDescent : SizeHint
    
    MinimumSize : SizeHint
    
    MiterJoin : PenJoinStyle
    
    Modifier : Type[Modifier]
    
    Monday : DayOfWeek
    
    MonoOnly : ImageConversionFlag
    
    MouseButton : Type[MouseButton]
    
    MouseButtonMask : MouseButton
    
    MouseButtons : Type[MouseButtons]
    
    MouseEventCreatedDoubleClick : MouseEventFlag
    
    MouseEventFlag : Type[MouseEventFlag]
    
    MouseEventFlagMask : MouseEventFlag
    
    MouseEventFlags : Type[MouseEventFlags]
    
    MouseEventNotSynthesized : MouseEventSource
    
    MouseEventSource : Type[MouseEventSource]
    
    MouseEventSynthesizedByApplication : MouseEventSource
    
    MouseEventSynthesizedByQt : MouseEventSource
    
    MouseEventSynthesizedBySystem : MouseEventSource
    
    MouseFocusReason : FocusReason
    
    MoveAction : DropAction
    
    NDockWidgetAreas : DockWidgetAreaSizes
    
    NSizeHints : SizeHint
    
    NToolBarAreas : ToolBarAreaSizes
    
    NativeGestureType : Type[NativeGestureType]
    
    NavigationMode : Type[NavigationMode]
    
    NavigationModeCursorAuto : NavigationMode
    
    NavigationModeCursorForceVisible : NavigationMode
    
    NavigationModeKeypadDirectional : NavigationMode
    
    NavigationModeKeypadTabOrder : NavigationMode
    
    NavigationModeNone : NavigationMode
    
    NoAlpha : ImageConversionFlag
    
    NoArrow : ArrowType
    
    NoBrush : BrushStyle
    
    NoButton : MouseButton
    
    NoClip : ClipOperation
    
    NoContextMenu : ContextMenuPolicy
    
    NoDockWidgetArea : DockWidgetArea
    
    NoDropShadowWindowHint : WindowType
    
    NoFocus : FocusPolicy
    
    NoFocusReason : FocusReason
    
    NoFormatConversion : ImageConversionFlag
    
    NoGesture : GestureState
    
    NoItemFlags : ItemFlag
    
    NoModifier : KeyboardModifier
    
    NoOpaqueDetection : ImageConversionFlag
    
    NoPen : PenStyle
    
    NoScrollPhase : ScrollPhase
    
    NoSection : WindowFrameSection
    
    NoTabFocus : TabFocusBehavior
    
    NoTextInteraction : TextInteractionFlag
    
    NoToolBarArea : ToolBarArea
    
    NonModal : WindowModality
    
    NormalEventPriority : EventPriority
    
    OddEvenFill : FillRule
    
    OffsetFromUTC : TimeSpec
    
    OpaqueMode : BGMode
    
    OpenHandCursor : CursorShape
    
    OrderedAlphaDither : ImageConversionFlag
    
    OrderedDither : ImageConversionFlag
    
    Orientation : Type[Orientation]
    
    Orientations : Type[Orientations]
    
    OtherFocusReason : FocusReason
    
    PanGesture : GestureType
    
    PanNativeGesture : NativeGestureType
    
    PartiallyChecked : CheckState
    
    PenCapStyle : Type[PenCapStyle]
    
    PenJoinStyle : Type[PenJoinStyle]
    
    PenStyle : Type[PenStyle]
    
    PinchGesture : GestureType
    
    PlainText : TextFormat
    
    PointingHandCursor : CursorShape
    
    Popup : WindowType
    
    PopupFocusReason : FocusReason
    
    PortraitOrientation : ScreenOrientation
    
    PreciseTimer : TimerType
    
    PreferDither : ImageConversionFlag
    
    PreferredSize : SizeHint
    
    PreventContextMenu : ContextMenuPolicy
    
    PrimaryOrientation : ScreenOrientation
    
    QueuedConnection : ConnectionType
    
    RFC2822Date : DateFormat
    
    RadialGradientPattern : BrushStyle
    
    ReceivePartialGestures : GestureFlag
    
    RelativeSize : SizeMode
    
    RepeatTile : TileRule
    
    ReplaceClip : ClipOperation
    
    ReplaceSelection : ItemSelectionOperation
    
    RichText : TextFormat
    
    RightArrow : ArrowType
    
    RightButton : MouseButton
    
    RightDockWidgetArea : DockWidgetArea
    
    RightEdge : Edge
    
    RightSection : WindowFrameSection
    
    RightToLeft : LayoutDirection
    
    RightToolBarArea : ToolBarArea
    
    RotateNativeGesture : NativeGestureType
    
    RoundCap : PenCapStyle
    
    RoundJoin : PenJoinStyle
    
    RoundTile : TileRule
    
    SHIFT : Modifier
    
    Saturday : DayOfWeek
    
    ScreenOrientation : Type[ScreenOrientation]
    
    ScreenOrientations : Type[ScreenOrientations]
    
    ScrollBarAlwaysOff : ScrollBarPolicy
    
    ScrollBarAlwaysOn : ScrollBarPolicy
    
    ScrollBarAsNeeded : ScrollBarPolicy
    
    ScrollBarPolicy : Type[ScrollBarPolicy]
    
    ScrollBegin : ScrollPhase
    
    ScrollEnd : ScrollPhase
    
    ScrollPhase : Type[ScrollPhase]
    
    ScrollUpdate : ScrollPhase
    
    Sheet : WindowType
    
    ShiftModifier : KeyboardModifier
    
    ShortcutContext : Type[ShortcutContext]
    
    ShortcutFocusReason : FocusReason
    
    SizeAllCursor : CursorShape
    
    SizeBDiagCursor : CursorShape
    
    SizeFDiagCursor : CursorShape
    
    SizeHint : Type[SizeHint]
    
    SizeHintRole : ItemDataRole
    
    SizeHorCursor : CursorShape
    
    SizeMode : Type[SizeMode]
    
    SizeVerCursor : CursorShape
    
    SmartZoomNativeGesture : NativeGestureType
    
    SmoothTransformation : TransformationMode
    
    SolidLine : PenStyle
    
    SolidPattern : BrushStyle
    
    SortOrder : Type[SortOrder]
    
    SplashScreen : WindowType
    
    SplitHCursor : CursorShape
    
    SplitVCursor : CursorShape
    
    SquareCap : PenCapStyle
    
    StatusTipPropertyRole : ItemDataRole
    
    StatusTipRole : ItemDataRole
    
    StretchTile : TileRule
    
    StrongFocus : FocusPolicy
    
    SubWindow : WindowType
    
    Sunday : DayOfWeek
    
    SvgMiterJoin : PenJoinStyle
    
    SwipeGesture : GestureType
    
    SwipeNativeGesture : NativeGestureType
    
    SystemLocaleDate : DateFormat
    
    SystemLocaleLongDate : DateFormat
    
    SystemLocaleShortDate : DateFormat
    
    TabFocus : FocusPolicy
    
    TabFocusAllControls : TabFocusBehavior
    
    TabFocusBehavior : Type[TabFocusBehavior]
    
    TabFocusListControls : TabFocusBehavior
    
    TabFocusReason : FocusReason
    
    TabFocusTextControls : TabFocusBehavior
    
    TapAndHoldGesture : GestureType
    
    TapGesture : GestureType
    
    TargetMoveAction : DropAction
    
    TaskButton : MouseButton
    
    TextAlignmentRole : ItemDataRole
    
    TextBrowserInteraction : TextInteractionFlag
    
    TextBypassShaping : TextFlag
    
    TextColorRole : ItemDataRole
    
    TextDate : DateFormat
    
    TextDontClip : TextFlag
    
    TextDontPrint : TextFlag
    
    TextEditable : TextInteractionFlag
    
    TextEditorInteraction : TextInteractionFlag
    
    TextElideMode : Type[TextElideMode]
    
    TextExpandTabs : TextFlag
    
    TextFlag : Type[TextFlag]
    
    TextForceLeftToRight : TextFlag
    
    TextForceRightToLeft : TextFlag
    
    TextFormat : Type[TextFormat]
    
    TextHideMnemonic : TextFlag
    
    TextIncludeTrailingSpaces : TextFlag
    
    TextInteractionFlag : Type[TextInteractionFlag]
    
    TextInteractionFlags : Type[TextInteractionFlags]
    
    TextJustificationForced : TextFlag
    
    TextLongestVariant : TextFlag
    
    TextSelectableByKeyboard : TextInteractionFlag
    
    TextSelectableByMouse : TextInteractionFlag
    
    TextShowMnemonic : TextFlag
    
    TextSingleLine : TextFlag
    
    TextWordWrap : TextFlag
    
    TextWrapAnywhere : TextFlag
    
    TexturePattern : BrushStyle
    
    ThresholdAlphaDither : ImageConversionFlag
    
    ThresholdDither : ImageConversionFlag
    
    Thursday : DayOfWeek
    
    TileRule : Type[TileRule]
    
    TimeSpec : Type[TimeSpec]
    
    TimeZone : TimeSpec
    
    TimerType : Type[TimerType]
    
    TitleBarArea : WindowFrameSection
    
    Tool : WindowType
    
    ToolBarArea : Type[ToolBarArea]
    
    ToolBarAreaSizes : Type[ToolBarAreaSizes]
    
    ToolBarArea_Mask : ToolBarArea
    
    ToolBarAreas : Type[ToolBarAreas]
    
    ToolButtonFollowStyle : ToolButtonStyle
    
    ToolButtonIconOnly : ToolButtonStyle
    
    ToolButtonStyle : Type[ToolButtonStyle]
    
    ToolButtonTextBesideIcon : ToolButtonStyle
    
    ToolButtonTextOnly : ToolButtonStyle
    
    ToolButtonTextUnderIcon : ToolButtonStyle
    
    ToolTip : WindowType
    
    ToolTipPropertyRole : ItemDataRole
    
    ToolTipRole : ItemDataRole
    
    TopDockWidgetArea : DockWidgetArea
    
    TopEdge : Edge
    
    TopLeftCorner : Corner
    
    TopLeftSection : WindowFrameSection
    
    TopRightCorner : Corner
    
    TopRightSection : WindowFrameSection
    
    TopSection : WindowFrameSection
    
    TopToolBarArea : ToolBarArea
    
    TouchPointMoved : TouchPointState
    
    TouchPointPressed : TouchPointState
    
    TouchPointReleased : TouchPointState
    
    TouchPointState : Type[TouchPointState]
    
    TouchPointStationary : TouchPointState
    
    TransformationMode : Type[TransformationMode]
    
    TransparentMode : BGMode
    
    Tuesday : DayOfWeek
    
    UIEffect : Type[UIEffect]
    
    UI_AnimateCombo : UIEffect
    
    UI_AnimateMenu : UIEffect
    
    UI_AnimateToolBox : UIEffect
    
    UI_AnimateTooltip : UIEffect
    
    UI_FadeMenu : UIEffect
    
    UI_FadeTooltip : UIEffect
    
    UI_General : UIEffect
    
    UNICODE_ACCEL : Modifier
    
    UTC : TimeSpec
    
    Unchecked : CheckState
    
    UniqueConnection : ConnectionType
    
    UpArrow : ArrowType
    
    UpArrowCursor : CursorShape
    
    UserRole : ItemDataRole
    
    VerPattern : BrushStyle
    
    Vertical : Orientation
    
    VeryCoarseTimer : TimerType
    
    VisualMoveStyle : CursorMoveStyle
    
    WA_AcceptDrops : WidgetAttribute
    
    WA_AcceptTouchEvents : WidgetAttribute
    
    WA_AlwaysShowToolTips : WidgetAttribute
    
    WA_AlwaysStackOnTop : WidgetAttribute
    
    WA_AttributeCount : WidgetAttribute
    
    WA_CanHostQMdiSubWindowTitleBar : WidgetAttribute
    
    WA_ContentsPropagated : WidgetAttribute
    
    WA_CustomWhatsThis : WidgetAttribute
    
    WA_DeleteOnClose : WidgetAttribute
    
    WA_Disabled : WidgetAttribute
    
    WA_DontCreateNativeAncestors : WidgetAttribute
    
    WA_DontShowOnScreen : WidgetAttribute
    
    WA_DropSiteRegistered : WidgetAttribute
    
    WA_ForceAcceptDrops : WidgetAttribute
    
    WA_ForceDisabled : WidgetAttribute
    
    WA_ForceUpdatesDisabled : WidgetAttribute
    
    WA_GrabbedShortcut : WidgetAttribute
    
    WA_GroupLeader : WidgetAttribute
    
    WA_Hover : WidgetAttribute
    
    WA_InputMethodEnabled : WidgetAttribute
    
    WA_InputMethodTransparent : WidgetAttribute
    
    WA_InvalidSize : WidgetAttribute
    
    WA_KeyCompression : WidgetAttribute
    
    WA_KeyboardFocusChange : WidgetAttribute
    
    WA_LaidOut : WidgetAttribute
    
    WA_LayoutOnEntireRect : WidgetAttribute
    
    WA_LayoutUsesWidgetRect : WidgetAttribute
    
    WA_MSWindowsUseDirect3D : WidgetAttribute
    
    WA_MacAlwaysShowToolWindow : WidgetAttribute
    
    WA_MacBrushedMetal : WidgetAttribute
    
    WA_MacFrameworkScaled : WidgetAttribute
    
    WA_MacMetalStyle : WidgetAttribute
    
    WA_MacMiniSize : WidgetAttribute
    
    WA_MacNoClickThrough : WidgetAttribute
    
    WA_MacNoShadow : WidgetAttribute
    
    WA_MacNormalSize : WidgetAttribute
    
    WA_MacOpaqueSizeGrip : WidgetAttribute
    
    WA_MacShowFocusRect : WidgetAttribute
    
    WA_MacSmallSize : WidgetAttribute
    
    WA_MacVariableSize : WidgetAttribute
    
    WA_Mapped : WidgetAttribute
    
    WA_MouseNoMask : WidgetAttribute
    
    WA_MouseTracking : WidgetAttribute
    
    WA_Moved : WidgetAttribute
    
    WA_NativeWindow : WidgetAttribute
    
    WA_NoBackground : WidgetAttribute
    
    WA_NoChildEventsForParent : WidgetAttribute
    
    WA_NoChildEventsFromChildren : WidgetAttribute
    
    WA_NoMousePropagation : WidgetAttribute
    
    WA_NoMouseReplay : WidgetAttribute
    
    WA_NoSystemBackground : WidgetAttribute
    
    WA_NoX11EventCompression : WidgetAttribute
    
    WA_OpaquePaintEvent : WidgetAttribute
    
    WA_OutsideWSRange : WidgetAttribute
    
    WA_PaintOnScreen : WidgetAttribute
    
    WA_PaintUnclipped : WidgetAttribute
    
    WA_PendingMoveEvent : WidgetAttribute
    
    WA_PendingResizeEvent : WidgetAttribute
    
    WA_PendingUpdate : WidgetAttribute
    
    WA_QuitOnClose : WidgetAttribute
    
    WA_Resized : WidgetAttribute
    
    WA_RightToLeft : WidgetAttribute
    
    WA_SetCursor : WidgetAttribute
    
    WA_SetFont : WidgetAttribute
    
    WA_SetLayoutDirection : WidgetAttribute
    
    WA_SetLocale : WidgetAttribute
    
    WA_SetPalette : WidgetAttribute
    
    WA_SetStyle : WidgetAttribute
    
    WA_SetWindowIcon : WidgetAttribute
    
    WA_SetWindowModality : WidgetAttribute
    
    WA_ShowModal : WidgetAttribute
    
    WA_ShowWithoutActivating : WidgetAttribute
    
    WA_StaticContents : WidgetAttribute
    
    WA_StyleSheet : WidgetAttribute
    
    WA_StyledBackground : WidgetAttribute
    
    WA_TintedBackground : WidgetAttribute
    
    WA_TouchPadAcceptSingleTouchEvents : WidgetAttribute
    
    WA_TranslucentBackground : WidgetAttribute
    
    WA_TransparentForMouseEvents : WidgetAttribute
    
    WA_UnderMouse : WidgetAttribute
    
    WA_UpdatesDisabled : WidgetAttribute
    
    WA_WState_AcceptedTouchBeginEvent : WidgetAttribute
    
    WA_WState_CompressKeys : WidgetAttribute
    
    WA_WState_ConfigPending : WidgetAttribute
    
    WA_WState_Created : WidgetAttribute
    
    WA_WState_DND : WidgetAttribute
    
    WA_WState_ExplicitShowHide : WidgetAttribute
    
    WA_WState_Hidden : WidgetAttribute
    
    WA_WState_InPaintEvent : WidgetAttribute
    
    WA_WState_OwnSizePolicy : WidgetAttribute
    
    WA_WState_Polished : WidgetAttribute
    
    WA_WState_Reparented : WidgetAttribute
    
    WA_WState_Visible : WidgetAttribute
    
    WA_WState_WindowOpacitySet : WidgetAttribute
    
    WA_WindowModified : WidgetAttribute
    
    WA_WindowPropagation : WidgetAttribute
    
    WA_X11BypassTransientForHint : WidgetAttribute
    
    WA_X11DoNotAcceptFocus : WidgetAttribute
    
    WA_X11NetWmWindowTypeCombo : WidgetAttribute
    
    WA_X11NetWmWindowTypeDND : WidgetAttribute
    
    WA_X11NetWmWindowTypeDesktop : WidgetAttribute
    
    WA_X11NetWmWindowTypeDialog : WidgetAttribute
    
    WA_X11NetWmWindowTypeDock : WidgetAttribute
    
    WA_X11NetWmWindowTypeDropDownMenu : WidgetAttribute
    
    WA_X11NetWmWindowTypeMenu : WidgetAttribute
    
    WA_X11NetWmWindowTypeNotification : WidgetAttribute
    
    WA_X11NetWmWindowTypePopupMenu : WidgetAttribute
    
    WA_X11NetWmWindowTypeSplash : WidgetAttribute
    
    WA_X11NetWmWindowTypeToolBar : WidgetAttribute
    
    WA_X11NetWmWindowTypeToolTip : WidgetAttribute
    
    WA_X11NetWmWindowTypeUtility : WidgetAttribute
    
    WA_X11OpenGLOverlay : WidgetAttribute
    
    WaitCursor : CursorShape
    
    Wednesday : DayOfWeek
    
    WhatsThisCursor : CursorShape
    
    WhatsThisPropertyRole : ItemDataRole
    
    WhatsThisRole : ItemDataRole
    
    WheelFocus : FocusPolicy
    
    WhiteSpaceMode : Type[WhiteSpaceMode]
    
    WhiteSpaceModeUndefined : WhiteSpaceMode
    
    WhiteSpaceNoWrap : WhiteSpaceMode
    
    WhiteSpaceNormal : WhiteSpaceMode
    
    WhiteSpacePre : WhiteSpaceMode
    
    Widget : WindowType
    
    WidgetAttribute : Type[WidgetAttribute]
    
    WidgetShortcut : ShortcutContext
    
    WidgetWithChildrenShortcut : ShortcutContext
    
    WindingFill : FillRule
    
    Window : WindowType
    
    WindowActive : WindowState
    
    WindowCancelButtonHint : WindowType
    
    WindowCloseButtonHint : WindowType
    
    WindowContextHelpButtonHint : WindowType
    
    WindowDoesNotAcceptFocus : WindowType
    
    WindowFlags : Type[WindowFlags]
    
    WindowFrameSection : Type[WindowFrameSection]
    
    WindowFullScreen : WindowState
    
    WindowFullscreenButtonHint : WindowType
    
    WindowMaximizeButtonHint : WindowType
    
    WindowMaximized : WindowState
    
    WindowMinMaxButtonsHint : WindowType
    
    WindowMinimizeButtonHint : WindowType
    
    WindowMinimized : WindowState
    
    WindowModal : WindowModality
    
    WindowModality : Type[WindowModality]
    
    WindowNoState : WindowState
    
    WindowOkButtonHint : WindowType
    
    WindowOverridesSystemGestures : WindowType
    
    WindowShadeButtonHint : WindowType
    
    WindowShortcut : ShortcutContext
    
    WindowState : Type[WindowState]
    
    WindowStates : Type[WindowStates]
    
    WindowStaysOnBottomHint : WindowType
    
    WindowStaysOnTopHint : WindowType
    
    WindowSystemMenuHint : WindowType
    
    WindowTitleHint : WindowType
    
    WindowTransparentForInput : WindowType
    
    WindowType : Type[WindowType]
    
    WindowType_Mask : WindowType
    
    X11BypassWindowManagerHint : WindowType
    
    XAxis : Axis
    
    XButton1 : MouseButton
    
    XButton2 : MouseButton
    
    YAxis : Axis
    
    ZAxis : Axis
    
    ZoomNativeGesture : NativeGestureType
    
    black : GlobalColor
    
    blue : GlobalColor
    
    color0 : GlobalColor
    
    color1 : GlobalColor
    
    cyan : GlobalColor
    
    darkBlue : GlobalColor
    
    darkCyan : GlobalColor
    
    darkGray : GlobalColor
    
    darkGreen : GlobalColor
    
    darkMagenta : GlobalColor
    
    darkRed : GlobalColor
    
    darkYellow : GlobalColor
    
    gray : GlobalColor
    
    green : GlobalColor
    
    lightGray : GlobalColor
    
    magenta : GlobalColor
    
    red : GlobalColor
    
    transparent : GlobalColor
    
    white : GlobalColor
    
    yellow : GlobalColor


class QProcessEnvironment(_Object):
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
    def clear(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def keys(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def toStringList(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    @staticmethod
    def systemEnvironment(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QSystemSemaphore(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def acquire(*args, **kwargs): ...
    def error(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def key(*args, **kwargs): ...
    def release(*args, **kwargs): ...
    def setKey(*args, **kwargs): ...
    AccessMode : Type[AccessMode]
    
    AlreadyExists : SystemSemaphoreError
    
    Create : AccessMode
    
    KeyError : SystemSemaphoreError
    
    NoError : SystemSemaphoreError
    
    NotFound : SystemSemaphoreError
    
    Open : AccessMode
    
    OutOfResources : SystemSemaphoreError
    
    PermissionDenied : SystemSemaphoreError
    
    SystemSemaphoreError : Type[SystemSemaphoreError]
    
    UnknownError : SystemSemaphoreError
    
    __new__ : builtin_function_or_method


class QJsonParseError(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def errorString(*args, **kwargs): ...
    DeepNesting : ParseError
    
    DocumentTooLarge : ParseError
    
    GarbageAtEnd : ParseError
    
    IllegalEscapeSequence : ParseError
    
    IllegalNumber : ParseError
    
    IllegalUTF8String : ParseError
    
    IllegalValue : ParseError
    
    MissingNameSeparator : ParseError
    
    MissingObject : ParseError
    
    MissingValueSeparator : ParseError
    
    NoError : ParseError
    
    ParseError : Type[ParseError]
    
    TerminationByNumber : ParseError
    
    UnterminatedArray : ParseError
    
    UnterminatedObject : ParseError
    
    UnterminatedString : ParseError
    
    __new__ : builtin_function_or_method
    
    error : getset_descriptor
    
    offset : getset_descriptor


class QMimeDatabase(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def allMimeTypes(*args, **kwargs): ...
    def mimeTypeForData(*args, **kwargs): ...
    def mimeTypeForFile(*args, **kwargs): ...
    def mimeTypeForFileNameAndData(*args, **kwargs): ...
    def mimeTypeForName(*args, **kwargs): ...
    def mimeTypeForUrl(*args, **kwargs): ...
    def mimeTypesForFileName(*args, **kwargs): ...
    def suffixForFileName(*args, **kwargs): ...
    MatchContent : MatchMode
    
    MatchDefault : MatchMode
    
    MatchExtension : MatchMode
    
    MatchMode : Type[MatchMode]
    
    __new__ : builtin_function_or_method


class QXmlStreamEntityResolver(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def resolveEntity(*args, **kwargs): ...
    def resolveUndeclaredEntity(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QWaitCondition(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def wait(*args, **kwargs): ...
    def wakeAll(*args, **kwargs): ...
    def wakeOne(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QStandardPaths(_Object):
    @staticmethod
    def displayName(*args, **kwargs): ...
    @staticmethod
    def enableTestMode(*args, **kwargs): ...
    @staticmethod
    def findExecutable(*args, **kwargs): ...
    @staticmethod
    def isTestModeEnabled(*args, **kwargs): ...
    @staticmethod
    def locate(*args, **kwargs): ...
    @staticmethod
    def locateAll(*args, **kwargs): ...
    @staticmethod
    def setTestModeEnabled(*args, **kwargs): ...
    @staticmethod
    def standardLocations(*args, **kwargs): ...
    @staticmethod
    def writableLocation(*args, **kwargs): ...
    AppConfigLocation : StandardLocation
    
    AppDataLocation : StandardLocation
    
    AppLocalDataLocation : StandardLocation
    
    ApplicationsLocation : StandardLocation
    
    CacheLocation : StandardLocation
    
    ConfigLocation : StandardLocation
    
    DataLocation : StandardLocation
    
    DesktopLocation : StandardLocation
    
    DocumentsLocation : StandardLocation
    
    DownloadLocation : StandardLocation
    
    FontsLocation : StandardLocation
    
    GenericCacheLocation : StandardLocation
    
    GenericConfigLocation : StandardLocation
    
    GenericDataLocation : StandardLocation
    
    HomeLocation : StandardLocation
    
    LocateDirectory : LocateOption
    
    LocateFile : LocateOption
    
    LocateOption : Type[LocateOption]
    
    LocateOptions : Type[LocateOptions]
    
    MoviesLocation : StandardLocation
    
    MusicLocation : StandardLocation
    
    PicturesLocation : StandardLocation
    
    RuntimeLocation : StandardLocation
    
    StandardLocation : Type[StandardLocation]
    
    TempLocation : StandardLocation


class QMetaMethod(_Object):
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
    def access(*args, **kwargs): ...
    def enclosingMetaObject(*args, **kwargs): ...
    def invoke(*args, **kwargs): ...
    def invokeOnGadget(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def methodIndex(*args, **kwargs): ...
    def methodSignature(*args, **kwargs): ...
    def methodType(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def parameterCount(*args, **kwargs): ...
    def parameterNames(*args, **kwargs): ...
    def parameterType(*args, **kwargs): ...
    def parameterTypes(*args, **kwargs): ...
    def returnType(*args, **kwargs): ...
    def revision(*args, **kwargs): ...
    def tag(*args, **kwargs): ...
    def typeName(*args, **kwargs): ...
    Access : Type[Access]
    
    Constructor : MethodType
    
    Method : MethodType
    
    MethodType : Type[MethodType]
    
    Private : Access
    
    Protected : Access
    
    Public : Access
    
    Signal : MethodType
    
    Slot : MethodType
    
    __new__ : builtin_function_or_method


class QTextDecoder(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def hasFailure(*args, **kwargs): ...
    def toUnicode(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QPoint(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __div__(*args, **kwargs):
        """
        x.__div__(y) <==> x/y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
        """
        ...
    def __idiv__(*args, **kwargs):
        """
        x.__idiv__(y) <==> x/=y
        """
        ...
    def __imul__(*args, **kwargs):
        """
        x.__imul__(y) <==> x*=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __isub__(*args, **kwargs):
        """
        x.__isub__(y) <==> x-=y
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
    def __mul__(*args, **kwargs):
        """
        x.__mul__(y) <==> x*y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __neg__(*args, **kwargs):
        """
        x.__neg__() <==> -x
        """
        ...
    def __nonzero__(self) -> bool:
        """
        x.__nonzero__() <==> x != 0
        """
        ...
    def __pos__(*args, **kwargs):
        """
        x.__pos__() <==> +x
        """
        ...
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rdiv__(*args, **kwargs):
        """
        x.__rdiv__(y) <==> y/x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rmul__(*args, **kwargs):
        """
        x.__rmul__(y) <==> y*x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __rtruediv__(*args, **kwargs):
        """
        x.__rtruediv__(y) <==> y/x
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def __truediv__(*args, **kwargs):
        """
        x.__truediv__(y) <==> x/y
        """
        ...
    def isNull(*args, **kwargs): ...
    def manhattanLength(*args, **kwargs): ...
    def setX(*args, **kwargs): ...
    def setY(*args, **kwargs): ...
    def toTuple(*args, **kwargs): ...
    def x(*args, **kwargs): ...
    def y(*args, **kwargs): ...
    @staticmethod
    def dotProduct(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QSizeF(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __div__(*args, **kwargs):
        """
        x.__div__(y) <==> x/y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
        """
        ...
    def __idiv__(*args, **kwargs):
        """
        x.__idiv__(y) <==> x/=y
        """
        ...
    def __imul__(*args, **kwargs):
        """
        x.__imul__(y) <==> x*=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __isub__(*args, **kwargs):
        """
        x.__isub__(y) <==> x-=y
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
    def __mul__(*args, **kwargs):
        """
        x.__mul__(y) <==> x*y
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
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rdiv__(*args, **kwargs):
        """
        x.__rdiv__(y) <==> y/x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rmul__(*args, **kwargs):
        """
        x.__rmul__(y) <==> y*x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __rtruediv__(*args, **kwargs):
        """
        x.__rtruediv__(y) <==> y/x
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def __truediv__(*args, **kwargs):
        """
        x.__truediv__(y) <==> x/y
        """
        ...
    def boundedTo(*args, **kwargs): ...
    def expandedTo(*args, **kwargs): ...
    def height(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def scale(*args, **kwargs): ...
    def scaled(*args, **kwargs): ...
    def setHeight(*args, **kwargs): ...
    def setWidth(*args, **kwargs): ...
    def toSize(*args, **kwargs): ...
    def toTuple(*args, **kwargs): ...
    def transpose(*args, **kwargs): ...
    def transposed(*args, **kwargs): ...
    def width(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QByteArray(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __delitem__(*args, **kwargs):
        """
        x.__delitem__(y) <==> del x[y]
        """
        ...
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
    def __getitem__(*args, **kwargs):
        """
        x.__getitem__(y) <==> x[y]
        """
        ...
    def __getslice__(*args, **kwargs):
        """
        x.__getslice__(i, j) <==> x[i:j]
        
        Use of negative indices is not supported.
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
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
    def __len__(self) -> int:
        """
        x.__len__() <==> len(x)
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
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __setitem__(*args, **kwargs):
        """
        x.__setitem__(i, y) <==> x[i]=y
        """
        ...
    def __str__(self) -> str:
        """
        x.__str__() <==> str(x)
        """
        ...
    def append(*args, **kwargs): ...
    def at(*args, **kwargs): ...
    def capacity(*args, **kwargs): ...
    def cbegin(*args, **kwargs): ...
    def cend(*args, **kwargs): ...
    def chop(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def count(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def endsWith(*args, **kwargs): ...
    def fill(*args, **kwargs): ...
    def indexOf(*args, **kwargs): ...
    def insert(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isSharedWith(*args, **kwargs): ...
    def lastIndexOf(*args, **kwargs): ...
    def left(*args, **kwargs): ...
    def leftJustified(*args, **kwargs): ...
    def length(*args, **kwargs): ...
    def mid(*args, **kwargs): ...
    def prepend(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def repeated(*args, **kwargs): ...
    def replace(*args, **kwargs): ...
    def reserve(*args, **kwargs): ...
    def resize(*args, **kwargs): ...
    def right(*args, **kwargs): ...
    def rightJustified(*args, **kwargs): ...
    def setNum(*args, **kwargs): ...
    def setRawData(*args, **kwargs): ...
    def simplified(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def split(*args, **kwargs): ...
    def squeeze(*args, **kwargs): ...
    def startsWith(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def toBase64(*args, **kwargs): ...
    def toDouble(*args, **kwargs): ...
    def toFloat(*args, **kwargs): ...
    def toHex(*args, **kwargs): ...
    def toInt(*args, **kwargs): ...
    def toLong(*args, **kwargs): ...
    def toLongLong(*args, **kwargs): ...
    def toLower(*args, **kwargs): ...
    def toPercentEncoding(*args, **kwargs): ...
    def toShort(*args, **kwargs): ...
    def toUInt(*args, **kwargs): ...
    def toULong(*args, **kwargs): ...
    def toULongLong(*args, **kwargs): ...
    def toUShort(*args, **kwargs): ...
    def toUpper(*args, **kwargs): ...
    def trimmed(*args, **kwargs): ...
    def truncate(*args, **kwargs): ...
    @staticmethod
    def fromBase64(*args, **kwargs): ...
    @staticmethod
    def fromHex(*args, **kwargs): ...
    @staticmethod
    def fromPercentEncoding(*args, **kwargs): ...
    @staticmethod
    def fromRawData(*args, **kwargs): ...
    @staticmethod
    def number(*args, **kwargs): ...
    Base64Encoding : Base64Option
    
    Base64Option : Type[Base64Option]
    
    Base64Options : Type[Base64Options]
    
    Base64UrlEncoding : Base64Option
    
    KeepTrailingEquals : Base64Option
    
    OmitTrailingEquals : Base64Option
    
    __new__ : builtin_function_or_method


class QRectF(_Object):
    def __and__(*args, **kwargs):
        """
        x.__and__(y) <==> x&y
        """
        ...
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
    def __iand__(*args, **kwargs):
        """
        x.__iand__(y) <==> x&=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __ior__(*args, **kwargs):
        """
        x.__ior__(y) <==> x|=y
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
    def __or__(*args, **kwargs):
        """
        x.__or__(y) <==> x|y
        """
        ...
    def __rand__(*args, **kwargs):
        """
        x.__rand__(y) <==> y&x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __ror__(*args, **kwargs):
        """
        x.__ror__(y) <==> y|x
        """
        ...
    def adjust(*args, **kwargs): ...
    def adjusted(*args, **kwargs): ...
    def bottom(*args, **kwargs): ...
    def bottomLeft(*args, **kwargs): ...
    def bottomRight(*args, **kwargs): ...
    def center(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def getCoords(*args, **kwargs): ...
    def getRect(*args, **kwargs): ...
    def height(*args, **kwargs): ...
    def intersected(*args, **kwargs): ...
    def intersects(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def left(*args, **kwargs): ...
    def moveBottom(*args, **kwargs): ...
    def moveBottomLeft(*args, **kwargs): ...
    def moveBottomRight(*args, **kwargs): ...
    def moveCenter(*args, **kwargs): ...
    def moveLeft(*args, **kwargs): ...
    def moveRight(*args, **kwargs): ...
    def moveTo(*args, **kwargs): ...
    def moveTop(*args, **kwargs): ...
    def moveTopLeft(*args, **kwargs): ...
    def moveTopRight(*args, **kwargs): ...
    def normalized(*args, **kwargs): ...
    def right(*args, **kwargs): ...
    def setBottom(*args, **kwargs): ...
    def setBottomLeft(*args, **kwargs): ...
    def setBottomRight(*args, **kwargs): ...
    def setCoords(*args, **kwargs): ...
    def setHeight(*args, **kwargs): ...
    def setLeft(*args, **kwargs): ...
    def setRect(*args, **kwargs): ...
    def setRight(*args, **kwargs): ...
    def setSize(*args, **kwargs): ...
    def setTop(*args, **kwargs): ...
    def setTopLeft(*args, **kwargs): ...
    def setTopRight(*args, **kwargs): ...
    def setWidth(*args, **kwargs): ...
    def setX(*args, **kwargs): ...
    def setY(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def toAlignedRect(*args, **kwargs): ...
    def toRect(*args, **kwargs): ...
    def top(*args, **kwargs): ...
    def topLeft(*args, **kwargs): ...
    def topRight(*args, **kwargs): ...
    def translate(*args, **kwargs): ...
    def translated(*args, **kwargs): ...
    def united(*args, **kwargs): ...
    def width(*args, **kwargs): ...
    def x(*args, **kwargs): ...
    def y(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QBasicMutex(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def isRecursive(*args, **kwargs): ...
    def lock(*args, **kwargs): ...
    def tryLock(*args, **kwargs): ...
    def unlock(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QLocale(_Object):
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
    def amText(*args, **kwargs): ...
    def bcp47Name(*args, **kwargs): ...
    def country(*args, **kwargs): ...
    def createSeparatedList(*args, **kwargs): ...
    def currencySymbol(*args, **kwargs): ...
    def dateFormat(*args, **kwargs): ...
    def dateTimeFormat(*args, **kwargs): ...
    def dayName(*args, **kwargs): ...
    def decimalPoint(*args, **kwargs): ...
    def exponential(*args, **kwargs): ...
    def firstDayOfWeek(*args, **kwargs): ...
    def groupSeparator(*args, **kwargs): ...
    def language(*args, **kwargs): ...
    def measurementSystem(*args, **kwargs): ...
    def monthName(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def nativeCountryName(*args, **kwargs): ...
    def nativeLanguageName(*args, **kwargs): ...
    def negativeSign(*args, **kwargs): ...
    def numberOptions(*args, **kwargs): ...
    def percent(*args, **kwargs): ...
    def pmText(*args, **kwargs): ...
    def positiveSign(*args, **kwargs): ...
    def quoteString(*args, **kwargs): ...
    def script(*args, **kwargs): ...
    def setNumberOptions(*args, **kwargs): ...
    def standaloneDayName(*args, **kwargs): ...
    def standaloneMonthName(*args, **kwargs): ...
    def textDirection(*args, **kwargs): ...
    def timeFormat(*args, **kwargs): ...
    def toCurrencyString(*args, **kwargs): ...
    def toDate(*args, **kwargs): ...
    def toDateTime(*args, **kwargs): ...
    def toDouble(*args, **kwargs): ...
    def toFloat(*args, **kwargs): ...
    def toInt(*args, **kwargs): ...
    def toLongLong(*args, **kwargs): ...
    def toLower(*args, **kwargs): ...
    def toShort(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    def toTime(*args, **kwargs): ...
    def toUInt(*args, **kwargs): ...
    def toULongLong(*args, **kwargs): ...
    def toUShort(*args, **kwargs): ...
    def toUpper(*args, **kwargs): ...
    def uiLanguages(*args, **kwargs): ...
    def weekdays(*args, **kwargs): ...
    def zeroDigit(*args, **kwargs): ...
    @staticmethod
    def c(*args, **kwargs): ...
    @staticmethod
    def countriesForLanguage(*args, **kwargs): ...
    @staticmethod
    def countryToString(*args, **kwargs): ...
    @staticmethod
    def languageToString(*args, **kwargs): ...
    @staticmethod
    def matchingLocales(*args, **kwargs): ...
    @staticmethod
    def scriptToString(*args, **kwargs): ...
    @staticmethod
    def setDefault(*args, **kwargs): ...
    @staticmethod
    def system(*args, **kwargs): ...
    Abkhazian : Language
    
    Afan : Language
    
    Afar : Language
    
    Afghanistan : Country
    
    Afrikaans : Language
    
    Aghem : Language
    
    Akan : Language
    
    Akkadian : Language
    
    Akoose : Language
    
    AlandIslands : Country
    
    Albania : Country
    
    Albanian : Language
    
    Algeria : Country
    
    AlternateQuotation : QuotationStyle
    
    AmericanSamoa : Country
    
    Amharic : Language
    
    AncientEgyptian : Language
    
    AncientGreek : Language
    
    AncientNorthArabian : Language
    
    Andorra : Country
    
    Angola : Country
    
    Anguilla : Country
    
    Antarctica : Country
    
    AntiguaAndBarbuda : Country
    
    AnyCountry : Country
    
    AnyLanguage : Language
    
    AnyScript : Script
    
    Arabic : Language
    
    ArabicScript : Script
    
    Aragonese : Language
    
    Aramaic : Language
    
    Argentina : Country
    
    Armenia : Country
    
    Armenian : Language
    
    ArmenianScript : Script
    
    Aruba : Country
    
    AscensionIsland : Country
    
    Assamese : Language
    
    Asturian : Language
    
    Asu : Language
    
    Atsam : Language
    
    Australia : Country
    
    Austria : Country
    
    Avaric : Language
    
    Avestan : Language
    
    AvestanScript : Script
    
    Aymara : Language
    
    Azerbaijan : Country
    
    Azerbaijani : Language
    
    Bafia : Language
    
    Bahamas : Country
    
    Bahrain : Country
    
    Balinese : Language
    
    BalineseScript : Script
    
    Bambara : Language
    
    BamumScript : Script
    
    Bamun : Language
    
    Bangladesh : Country
    
    Barbados : Country
    
    Basaa : Language
    
    Bashkir : Language
    
    Basque : Language
    
    Bassa : Language
    
    BassaVahScript : Script
    
    BatakScript : Script
    
    BatakToba : Language
    
    Belarus : Country
    
    Belarusian : Language
    
    Belgium : Country
    
    Belize : Country
    
    Bemba : Language
    
    Bena : Language
    
    Bengali : Language
    
    BengaliScript : Script
    
    Benin : Country
    
    Bermuda : Country
    
    Bhutan : Country
    
    Bhutani : Language
    
    Bihari : Language
    
    Bislama : Language
    
    Blin : Language
    
    Bodo : Language
    
    Bolivia : Country
    
    Bonaire : Country
    
    BopomofoScript : Script
    
    BosniaAndHerzegowina : Country
    
    Bosnian : Language
    
    Botswana : Country
    
    BouvetIsland : Country
    
    BrahmiScript : Script
    
    BrailleScript : Script
    
    Brazil : Country
    
    Breton : Language
    
    BritishIndianOceanTerritory : Country
    
    BritishVirginIslands : Country
    
    Brunei : Country
    
    Buginese : Language
    
    BugineseScript : Script
    
    Buhid : Language
    
    BuhidScript : Script
    
    Bulgaria : Country
    
    Bulgarian : Language
    
    BurkinaFaso : Country
    
    Burmese : Language
    
    Burundi : Country
    
    Byelorussian : Language
    
    C : Language
    
    Cambodia : Country
    
    Cambodian : Language
    
    Cameroon : Country
    
    Canada : Country
    
    CanadianAboriginalScript : Script
    
    CanaryIslands : Country
    
    CapeVerde : Country
    
    Carian : Language
    
    CarianScript : Script
    
    Catalan : Language
    
    CaucasianAlbanianScript : Script
    
    CaymanIslands : Country
    
    CentralAfricanRepublic : Country
    
    CentralKurdish : Language
    
    CentralMoroccoTamazight : Language
    
    CeutaAndMelilla : Country
    
    Chad : Country
    
    Chakma : Language
    
    ChakmaScript : Script
    
    ChamScript : Script
    
    Chamorro : Language
    
    Chechen : Language
    
    Cherokee : Language
    
    CherokeeScript : Script
    
    Chewa : Language
    
    Chiga : Language
    
    Chile : Country
    
    China : Country
    
    Chinese : Language
    
    ChristmasIsland : Country
    
    Church : Language
    
    Chuvash : Language
    
    ClassicalMandaic : Language
    
    ClippertonIsland : Country
    
    CocosIslands : Country
    
    Colognian : Language
    
    Colombia : Country
    
    Comoros : Country
    
    CongoBrazzaville : Country
    
    CongoKinshasa : Country
    
    CongoSwahili : Language
    
    CookIslands : Country
    
    Coptic : Language
    
    CopticScript : Script
    
    Cornish : Language
    
    Corsican : Language
    
    CostaRica : Country
    
    Country : Type[Country]
    
    Cree : Language
    
    Croatia : Country
    
    Croatian : Language
    
    Cuba : Country
    
    CuneiformScript : Script
    
    CuraSao : Country
    
    CurrencyDisplayName : CurrencySymbolFormat
    
    CurrencyIsoCode : CurrencySymbolFormat
    
    CurrencySymbol : CurrencySymbolFormat
    
    CurrencySymbolFormat : Type[CurrencySymbolFormat]
    
    CypriotScript : Script
    
    Cyprus : Country
    
    CyrillicScript : Script
    
    Czech : Language
    
    CzechRepublic : Country
    
    Danish : Language
    
    DemocraticRepublicOfCongo : Country
    
    DemocraticRepublicOfKorea : Country
    
    Denmark : Country
    
    DeseretScript : Script
    
    DevanagariScript : Script
    
    DiegoGarcia : Country
    
    Divehi : Language
    
    Djibouti : Country
    
    Dogri : Language
    
    Dominica : Country
    
    DominicanRepublic : Country
    
    Duala : Language
    
    DuployanScript : Script
    
    Dutch : Language
    
    Dzongkha : Language
    
    EastTimor : Country
    
    EasternCham : Language
    
    EasternKayah : Language
    
    Ecuador : Country
    
    Egypt : Country
    
    EgyptianHieroglyphsScript : Script
    
    ElSalvador : Country
    
    ElbasanScript : Script
    
    Embu : Language
    
    English : Language
    
    EquatorialGuinea : Country
    
    Eritrea : Country
    
    Esperanto : Language
    
    Estonia : Country
    
    Estonian : Language
    
    Ethiopia : Country
    
    EthiopicScript : Script
    
    Etruscan : Language
    
    Ewe : Language
    
    Ewondo : Language
    
    FalklandIslands : Country
    
    FaroeIslands : Country
    
    Faroese : Language
    
    Fiji : Country
    
    Fijian : Language
    
    Filipino : Language
    
    Finland : Country
    
    Finnish : Language
    
    FormatType : Type[FormatType]
    
    France : Country
    
    FraserScript : Script
    
    French : Language
    
    FrenchGuiana : Country
    
    FrenchPolynesia : Country
    
    FrenchSouthernTerritories : Country
    
    Frisian : Language
    
    Friulian : Language
    
    Fulah : Language
    
    Ga : Language
    
    Gabon : Country
    
    Gaelic : Language
    
    Galician : Language
    
    Gambia : Country
    
    Ganda : Language
    
    Geez : Language
    
    Georgia : Country
    
    Georgian : Language
    
    GeorgianScript : Script
    
    German : Language
    
    Germany : Country
    
    Ghana : Country
    
    Gibraltar : Country
    
    GlagoliticScript : Script
    
    Gothic : Language
    
    GothicScript : Script
    
    GranthaScript : Script
    
    Greece : Country
    
    Greek : Language
    
    GreekScript : Script
    
    Greenland : Country
    
    Greenlandic : Language
    
    Grenada : Country
    
    Guadeloupe : Country
    
    Guam : Country
    
    Guarani : Language
    
    Guatemala : Country
    
    Guernsey : Country
    
    Guinea : Country
    
    GuineaBissau : Country
    
    Gujarati : Language
    
    GujaratiScript : Script
    
    GurmukhiScript : Script
    
    Gusii : Language
    
    Guyana : Country
    
    Haiti : Country
    
    Haitian : Language
    
    HanScript : Script
    
    HangulScript : Script
    
    Hanunoo : Language
    
    HanunooScript : Script
    
    Hausa : Language
    
    Hawaiian : Language
    
    HeardAndMcDonaldIslands : Country
    
    Hebrew : Language
    
    HebrewScript : Script
    
    Herero : Language
    
    Hindi : Language
    
    HiraganaScript : Script
    
    HiriMotu : Language
    
    HmongNjua : Language
    
    Ho : Language
    
    Honduras : Country
    
    HongKong : Country
    
    Hungarian : Language
    
    Hungary : Country
    
    Iceland : Country
    
    Icelandic : Language
    
    Igbo : Language
    
    ImperialAramaicScript : Script
    
    ImperialSystem : MeasurementSystem
    
    ImperialUKSystem : MeasurementSystem
    
    ImperialUSSystem : MeasurementSystem
    
    InariSami : Language
    
    India : Country
    
    Indonesia : Country
    
    Indonesian : Language
    
    Ingush : Language
    
    InscriptionalPahlaviScript : Script
    
    InscriptionalParthianScript : Script
    
    Interlingua : Language
    
    Interlingue : Language
    
    Inuktitut : Language
    
    Inupiak : Language
    
    Iran : Country
    
    Iraq : Country
    
    Ireland : Country
    
    Irish : Language
    
    IsleOfMan : Country
    
    Israel : Country
    
    Italian : Language
    
    Italy : Country
    
    IvoryCoast : Country
    
    Jamaica : Country
    
    Japan : Country
    
    Japanese : Language
    
    JapaneseScript : Script
    
    Javanese : Language
    
    JavaneseScript : Script
    
    Jersey : Country
    
    Jju : Language
    
    JolaFonyi : Language
    
    Jordan : Country
    
    Kabuverdianu : Language
    
    Kabyle : Language
    
    KaithiScript : Script
    
    Kako : Language
    
    Kalenjin : Language
    
    Kamba : Language
    
    Kannada : Language
    
    KannadaScript : Script
    
    Kanuri : Language
    
    Kashmiri : Language
    
    KatakanaScript : Script
    
    KayahLiScript : Script
    
    Kazakh : Language
    
    Kazakhstan : Country
    
    Kenya : Country
    
    Kenyang : Language
    
    KharoshthiScript : Script
    
    Khmer : Language
    
    KhmerScript : Script
    
    KhojkiScript : Script
    
    KhudawadiScript : Script
    
    Kiche : Language
    
    Kikuyu : Language
    
    Kinyarwanda : Language
    
    Kirghiz : Language
    
    Kiribati : Country
    
    Komi : Language
    
    Kongo : Language
    
    Konkani : Language
    
    Korean : Language
    
    KoreanScript : Script
    
    Koro : Language
    
    Kosovo : Country
    
    KoyraChiini : Language
    
    KoyraboroSenni : Language
    
    Kpelle : Language
    
    Kurdish : Language
    
    Kurundi : Language
    
    Kuwait : Country
    
    Kwanyama : Language
    
    Kwasio : Language
    
    Kyrgyzstan : Country
    
    Lakota : Language
    
    Langi : Language
    
    Language : Type[Language]
    
    LannaScript : Script
    
    Lao : Language
    
    LaoScript : Script
    
    Laos : Country
    
    LargeFloweryMiao : Language
    
    LastCountry : Country
    
    LastLanguage : Language
    
    LastScript : Script
    
    Latin : Language
    
    LatinAmericaAndTheCaribbean : Country
    
    LatinScript : Script
    
    Latvia : Country
    
    Latvian : Language
    
    Lebanon : Country
    
    Lepcha : Language
    
    LepchaScript : Script
    
    Lesotho : Country
    
    Lezghian : Language
    
    Liberia : Country
    
    Libya : Country
    
    Liechtenstein : Country
    
    Limbu : Language
    
    LimbuScript : Script
    
    Limburgish : Language
    
    LinearA : Language
    
    LinearAScript : Script
    
    LinearBScript : Script
    
    Lingala : Language
    
    Lisu : Language
    
    Lithuania : Country
    
    Lithuanian : Language
    
    LongFormat : FormatType
    
    LowGerman : Language
    
    LowerSorbian : Language
    
    Lu : Language
    
    LubaKatanga : Language
    
    LuleSami : Language
    
    Luo : Language
    
    Luxembourg : Country
    
    Luxembourgish : Language
    
    Luyia : Language
    
    Lycian : Language
    
    LycianScript : Script
    
    Lydian : Language
    
    LydianScript : Script
    
    Macau : Country
    
    Macedonia : Country
    
    Macedonian : Language
    
    Machame : Language
    
    Madagascar : Country
    
    MahajaniScript : Script
    
    Maithili : Language
    
    MakhuwaMeetto : Language
    
    Makonde : Language
    
    Malagasy : Language
    
    Malawi : Country
    
    Malay : Language
    
    Malayalam : Language
    
    MalayalamScript : Script
    
    Malaysia : Country
    
    Maldives : Country
    
    Mali : Country
    
    Malta : Country
    
    Maltese : Language
    
    MandaeanScript : Script
    
    Mandingo : Language
    
    ManichaeanMiddlePersian : Language
    
    ManichaeanScript : Script
    
    Manipuri : Language
    
    Manx : Language
    
    Maori : Language
    
    Mapuche : Language
    
    Marathi : Language
    
    MarshallIslands : Country
    
    Marshallese : Language
    
    Martinique : Country
    
    Masai : Language
    
    Mauritania : Country
    
    Mauritius : Country
    
    Mayotte : Country
    
    MeasurementSystem : Type[MeasurementSystem]
    
    MeiteiMayekScript : Script
    
    Mende : Language
    
    MendeKikakuiScript : Script
    
    Meroitic : Language
    
    MeroiticCursiveScript : Script
    
    MeroiticScript : Script
    
    Meru : Language
    
    Meta : Language
    
    MetricSystem : MeasurementSystem
    
    Mexico : Country
    
    Micronesia : Country
    
    ModiScript : Script
    
    Mohawk : Language
    
    Moldavian : Language
    
    Moldova : Country
    
    Monaco : Country
    
    Mongolia : Country
    
    Mongolian : Language
    
    MongolianScript : Script
    
    Mono : Language
    
    Montenegro : Country
    
    Montserrat : Country
    
    Morisyen : Language
    
    Morocco : Country
    
    Mozambique : Country
    
    MroScript : Script
    
    Mundang : Language
    
    Myanmar : Country
    
    MyanmarScript : Script
    
    NabataeanScript : Script
    
    Nama : Language
    
    Namibia : Country
    
    NarrowFormat : FormatType
    
    NauruCountry : Country
    
    NauruLanguage : Language
    
    Navaho : Language
    
    Ndonga : Language
    
    Nepal : Country
    
    Nepali : Language
    
    Netherlands : Country
    
    NewCaledonia : Country
    
    NewTaiLueScript : Script
    
    NewZealand : Country
    
    Ngiemboon : Language
    
    Ngomba : Language
    
    Nicaragua : Country
    
    Niger : Country
    
    Nigeria : Country
    
    Niue : Country
    
    Nko : Language
    
    NkoScript : Script
    
    NorfolkIsland : Country
    
    NorthKorea : Country
    
    NorthNdebele : Language
    
    NorthernMarianaIslands : Country
    
    NorthernSami : Language
    
    NorthernSotho : Language
    
    NorthernThai : Language
    
    Norway : Country
    
    Norwegian : Language
    
    NorwegianBokmal : Language
    
    NorwegianNynorsk : Language
    
    Nuer : Language
    
    NumberOption : Type[NumberOption]
    
    NumberOptions : Type[NumberOptions]
    
    Nyanja : Language
    
    Nyankole : Language
    
    Occitan : Language
    
    OghamScript : Script
    
    Ojibwa : Language
    
    OlChikiScript : Script
    
    OldIrish : Language
    
    OldItalicScript : Script
    
    OldNorse : Language
    
    OldNorthArabianScript : Script
    
    OldPermicScript : Script
    
    OldPersian : Language
    
    OldPersianScript : Script
    
    OldSouthArabianScript : Script
    
    OldTurkish : Language
    
    Oman : Country
    
    OmitGroupSeparator : NumberOption
    
    Oriya : Language
    
    OriyaScript : Script
    
    OrkhonScript : Script
    
    Oromo : Language
    
    OsmanyaScript : Script
    
    Ossetic : Language
    
    PahawhHmongScript : Script
    
    Pahlavi : Language
    
    Pakistan : Country
    
    Palau : Country
    
    PalestinianTerritories : Country
    
    Pali : Language
    
    PalmyreneScript : Script
    
    Panama : Country
    
    PapuaNewGuinea : Country
    
    Paraguay : Country
    
    Parthian : Language
    
    Pashto : Language
    
    PauCinHauScript : Script
    
    PeoplesRepublicOfCongo : Country
    
    Persian : Language
    
    Peru : Country
    
    PhagsPaScript : Script
    
    Philippines : Country
    
    Phoenician : Language
    
    PhoenicianScript : Script
    
    Pitcairn : Country
    
    Poland : Country
    
    Polish : Language
    
    PollardPhoneticScript : Script
    
    Portugal : Country
    
    Portuguese : Language
    
    PrakritLanguage : Language
    
    Prussian : Language
    
    PsalterPahlaviScript : Script
    
    PuertoRico : Country
    
    Punjabi : Language
    
    Qatar : Country
    
    Quechua : Language
    
    QuotationStyle : Type[QuotationStyle]
    
    Rejang : Language
    
    RejangScript : Script
    
    RejectGroupSeparator : NumberOption
    
    RepublicOfKorea : Country
    
    Reunion : Country
    
    RhaetoRomance : Language
    
    Romania : Country
    
    Romanian : Language
    
    Romansh : Language
    
    Rombo : Language
    
    Rundi : Language
    
    RunicScript : Script
    
    Russia : Country
    
    Russian : Language
    
    RussianFederation : Country
    
    Rwa : Language
    
    Rwanda : Country
    
    Sabaean : Language
    
    Saho : Language
    
    SaintBarthelemy : Country
    
    SaintHelena : Country
    
    SaintKittsAndNevis : Country
    
    SaintLucia : Country
    
    SaintMartin : Country
    
    SaintPierreAndMiquelon : Country
    
    SaintVincentAndTheGrenadines : Country
    
    Sakha : Language
    
    Samaritan : Language
    
    SamaritanScript : Script
    
    Samburu : Language
    
    Samoa : Country
    
    Samoan : Language
    
    SanMarino : Country
    
    Sango : Language
    
    Sangu : Language
    
    Sanskrit : Language
    
    Santali : Language
    
    SaoTomeAndPrincipe : Country
    
    Sardinian : Language
    
    SaudiArabia : Country
    
    Saurashtra : Language
    
    SaurashtraScript : Script
    
    Script : Type[Script]
    
    Sena : Language
    
    Senegal : Country
    
    Serbia : Country
    
    Serbian : Language
    
    SerboCroatian : Language
    
    Seychelles : Country
    
    Shambala : Language
    
    SharadaScript : Script
    
    ShavianScript : Script
    
    Shona : Language
    
    ShortFormat : FormatType
    
    SichuanYi : Language
    
    Sidamo : Language
    
    SiddhamScript : Script
    
    SierraLeone : Country
    
    SimplifiedChineseScript : Script
    
    SimplifiedHanScript : Script
    
    Sindhi : Language
    
    Singapore : Country
    
    Sinhala : Language
    
    SinhalaScript : Script
    
    SintMaarten : Country
    
    SkoltSami : Language
    
    Slovak : Language
    
    Slovakia : Country
    
    Slovenia : Country
    
    Slovenian : Language
    
    Soga : Language
    
    SolomonIslands : Country
    
    Somali : Language
    
    Somalia : Country
    
    Sora : Language
    
    SoraSompengScript : Script
    
    SouthAfrica : Country
    
    SouthGeorgiaAndTheSouthSandwichIslands : Country
    
    SouthKorea : Country
    
    SouthNdebele : Language
    
    SouthSudan : Country
    
    SouthernSami : Language
    
    SouthernSotho : Language
    
    Spain : Country
    
    Spanish : Language
    
    SriLanka : Country
    
    StandardMoroccanTamazight : Language
    
    StandardQuotation : QuotationStyle
    
    Sudan : Country
    
    Sundanese : Language
    
    SundaneseScript : Script
    
    Suriname : Country
    
    SvalbardAndJanMayenIslands : Country
    
    Swahili : Language
    
    Swati : Language
    
    Swaziland : Country
    
    Sweden : Country
    
    Swedish : Language
    
    SwissGerman : Language
    
    Switzerland : Country
    
    Sylheti : Language
    
    SylotiNagriScript : Script
    
    Syria : Country
    
    Syriac : Language
    
    SyriacScript : Script
    
    SyrianArabRepublic : Country
    
    Tachelhit : Language
    
    Tagalog : Language
    
    TagalogScript : Script
    
    Tagbanwa : Language
    
    TagbanwaScript : Script
    
    Tahitian : Language
    
    TaiDam : Language
    
    TaiLeScript : Script
    
    TaiNua : Language
    
    TaiVietScript : Script
    
    Taita : Language
    
    Taiwan : Country
    
    Tajik : Language
    
    Tajikistan : Country
    
    TakriScript : Script
    
    Tamil : Language
    
    TamilScript : Script
    
    Tanzania : Country
    
    Taroko : Language
    
    Tasawaq : Language
    
    Tatar : Language
    
    TedimChin : Language
    
    Telugu : Language
    
    TeluguScript : Script
    
    Teso : Language
    
    ThaanaScript : Script
    
    Thai : Language
    
    ThaiScript : Script
    
    Thailand : Country
    
    Tibetan : Language
    
    TibetanScript : Script
    
    TifinaghScript : Script
    
    Tigre : Language
    
    Tigrinya : Language
    
    TirhutaScript : Script
    
    Togo : Country
    
    Tokelau : Country
    
    Tonga : Country
    
    Tongan : Language
    
    TraditionalChineseScript : Script
    
    TraditionalHanScript : Script
    
    TrinidadAndTobago : Country
    
    TristanDaCunha : Country
    
    Tsonga : Language
    
    Tswana : Language
    
    Tunisia : Country
    
    Turkey : Country
    
    Turkish : Language
    
    Turkmen : Language
    
    Turkmenistan : Country
    
    TurksAndCaicosIslands : Country
    
    Tuvalu : Country
    
    Twi : Language
    
    Tyap : Language
    
    Uganda : Country
    
    Ugaritic : Language
    
    UgariticScript : Script
    
    Uighur : Language
    
    Uigur : Language
    
    Ukraine : Country
    
    Ukrainian : Language
    
    UnitedArabEmirates : Country
    
    UnitedKingdom : Country
    
    UnitedStates : Country
    
    UnitedStatesMinorOutlyingIslands : Country
    
    UnitedStatesVirginIslands : Country
    
    UpperSorbian : Language
    
    Urdu : Language
    
    Uruguay : Country
    
    Uzbek : Language
    
    Uzbekistan : Country
    
    Vai : Language
    
    VaiScript : Script
    
    Vanuatu : Country
    
    VarangKshitiScript : Script
    
    VaticanCityState : Country
    
    Venda : Language
    
    Venezuela : Country
    
    Vietnam : Country
    
    Vietnamese : Language
    
    Volapuk : Language
    
    Vunjo : Language
    
    Walamo : Language
    
    WallisAndFutunaIslands : Country
    
    Walloon : Language
    
    Walser : Language
    
    Warlpiri : Language
    
    Welsh : Language
    
    WesternFrisian : Language
    
    WesternSahara : Country
    
    Wolof : Language
    
    Xhosa : Language
    
    Yangben : Language
    
    Yemen : Country
    
    YiScript : Script
    
    Yiddish : Language
    
    Yoruba : Language
    
    Zambia : Country
    
    Zarma : Language
    
    Zhuang : Language
    
    Zimbabwe : Country
    
    Zulu : Language
    
    __new__ : builtin_function_or_method


class QXmlStreamNamespaceDeclaration(_Object):
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
    def namespaceUri(*args, **kwargs): ...
    def prefix(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QReadLocker(_Object):
    def __enter__(*args, **kwargs): ...
    def __exit__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def readWriteLock(*args, **kwargs): ...
    def relock(*args, **kwargs): ...
    def unlock(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QFactoryInterface(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def keys(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QWriteLocker(_Object):
    def __enter__(*args, **kwargs): ...
    def __exit__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def readWriteLock(*args, **kwargs): ...
    def relock(*args, **kwargs): ...
    def unlock(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QJsonValue(_Object):
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
    def isArray(*args, **kwargs): ...
    def isBool(*args, **kwargs): ...
    def isDouble(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isObject(*args, **kwargs): ...
    def isString(*args, **kwargs): ...
    def isUndefined(*args, **kwargs): ...
    def toArray(*args, **kwargs): ...
    def toBool(*args, **kwargs): ...
    def toDouble(*args, **kwargs): ...
    def toInt(*args, **kwargs): ...
    def toObject(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    def toVariant(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    @staticmethod
    def fromVariant(*args, **kwargs): ...
    Array : Type
    
    Bool : Type
    
    Double : Type
    
    Null : Type
    
    Object : Type
    
    String : Type
    
    Type : Type[Type]
    
    Undefined : Type
    
    __new__ : builtin_function_or_method


class QObject(_Object):
    def __delattr__(*args, **kwargs):
        """
        x.__delattr__('name') <==> del x.name
        """
        ...
    def __getattribute__(*args, **kwargs):
        """
        x.__getattribute__('name') <==> x.name
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __setattr__(*args, **kwargs):
        """
        x.__setattr__('name', value) <==> x.name = value
        """
        ...
    def blockSignals(*args, **kwargs): ...
    def childEvent(*args, **kwargs): ...
    def children(*args, **kwargs): ...
    def connectNotify(*args, **kwargs): ...
    def customEvent(*args, **kwargs): ...
    def deleteLater(*args, **kwargs): ...
    def disconnectNotify(*args, **kwargs): ...
    def dumpObjectInfo(*args, **kwargs): ...
    def dumpObjectTree(*args, **kwargs): ...
    def dynamicPropertyNames(*args, **kwargs): ...
    def emit(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def eventFilter(*args, **kwargs): ...
    def findChild(*args, **kwargs): ...
    def findChildren(*args, **kwargs): ...
    def inherits(*args, **kwargs): ...
    def installEventFilter(*args, **kwargs): ...
    def isSignalConnected(*args, **kwargs): ...
    def isWidgetType(*args, **kwargs): ...
    def isWindowType(*args, **kwargs): ...
    def killTimer(*args, **kwargs): ...
    def metaObject(*args, **kwargs): ...
    def moveToThread(*args, **kwargs): ...
    def objectName(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def property(*args, **kwargs): ...
    def receivers(*args, **kwargs): ...
    def removeEventFilter(*args, **kwargs): ...
    def sender(*args, **kwargs): ...
    def senderSignalIndex(*args, **kwargs): ...
    def setObjectName(*args, **kwargs): ...
    def setParent(*args, **kwargs): ...
    def setProperty(*args, **kwargs): ...
    def signalsBlocked(*args, **kwargs): ...
    def startTimer(*args, **kwargs): ...
    def thread(*args, **kwargs): ...
    def timerEvent(*args, **kwargs): ...
    def tr(*args, **kwargs): ...
    @staticmethod
    def connect(*args, **kwargs): ...
    @staticmethod
    def disconnect(*args, **kwargs): ...
    @staticmethod
    def registerUserData(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    destroyed : Signal
    
    objectNameChanged : Signal
    
    staticMetaObject : QMetaObject


class QDateTime(_Object):
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def addDays(*args, **kwargs): ...
    def addMSecs(*args, **kwargs): ...
    def addMonths(*args, **kwargs): ...
    def addSecs(*args, **kwargs): ...
    def addYears(*args, **kwargs): ...
    def date(*args, **kwargs): ...
    def daysTo(*args, **kwargs): ...
    def isDaylightTime(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def msecsTo(*args, **kwargs): ...
    def offsetFromUtc(*args, **kwargs): ...
    def secsTo(*args, **kwargs): ...
    def setDate(*args, **kwargs): ...
    def setMSecsSinceEpoch(*args, **kwargs): ...
    def setOffsetFromUtc(*args, **kwargs): ...
    def setTime(*args, **kwargs): ...
    def setTimeSpec(*args, **kwargs): ...
    def setTime_t(*args, **kwargs): ...
    def setUtcOffset(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def time(*args, **kwargs): ...
    def timeSpec(*args, **kwargs): ...
    def timeZoneAbbreviation(*args, **kwargs): ...
    def toLocalTime(*args, **kwargs): ...
    def toMSecsSinceEpoch(*args, **kwargs): ...
    def toOffsetFromUtc(*args, **kwargs): ...
    def toPython(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    def toTimeSpec(*args, **kwargs): ...
    def toTime_t(*args, **kwargs): ...
    def toUTC(*args, **kwargs): ...
    def utcOffset(*args, **kwargs): ...
    @staticmethod
    def currentDateTime(*args, **kwargs): ...
    @staticmethod
    def currentDateTimeUtc(*args, **kwargs): ...
    @staticmethod
    def currentMSecsSinceEpoch(*args, **kwargs): ...
    @staticmethod
    def fromMSecsSinceEpoch(*args, **kwargs): ...
    @staticmethod
    def fromString(*args, **kwargs): ...
    @staticmethod
    def fromTime_t(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class Connection(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    __new__ : builtin_function_or_method


class QFileInfo(_Object):
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
    def __getattribute__(*args, **kwargs):
        """
        x.__getattribute__('name') <==> x.name
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
    def __reduce__(*args, **kwargs): ...
    def absoluteDir(*args, **kwargs): ...
    def absoluteFilePath(*args, **kwargs): ...
    def absolutePath(*args, **kwargs): ...
    def baseName(*args, **kwargs): ...
    def bundleName(*args, **kwargs): ...
    def caching(*args, **kwargs): ...
    def canonicalFilePath(*args, **kwargs): ...
    def canonicalPath(*args, **kwargs): ...
    def completeBaseName(*args, **kwargs): ...
    def completeSuffix(*args, **kwargs): ...
    def created(*args, **kwargs): ...
    def dir(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def filePath(*args, **kwargs): ...
    def getAllMetadata(*args, **kwargs): ...
    def group(*args, **kwargs): ...
    def groupId(*args, **kwargs): ...
    def isAbsolute(*args, **kwargs): ...
    def isBundle(*args, **kwargs): ...
    def isDir(*args, **kwargs): ...
    def isExecutable(*args, **kwargs): ...
    def isFile(*args, **kwargs): ...
    def isHidden(*args, **kwargs): ...
    def isNativePath(*args, **kwargs): ...
    def isReadable(*args, **kwargs): ...
    def isRelative(*args, **kwargs): ...
    def isRoot(*args, **kwargs): ...
    def isSymLink(*args, **kwargs): ...
    def isWritable(*args, **kwargs): ...
    def lastModified(*args, **kwargs): ...
    def lastRead(*args, **kwargs): ...
    def makeAbsolute(*args, **kwargs): ...
    def owner(*args, **kwargs): ...
    def ownerId(*args, **kwargs): ...
    def path(*args, **kwargs): ...
    def readLink(*args, **kwargs): ...
    def refresh(*args, **kwargs): ...
    def setCaching(*args, **kwargs): ...
    def setFile(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def suffix(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def symLinkTarget(*args, **kwargs): ...
    @staticmethod
    def exists(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QTextEncoder(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def fromUnicode(*args, **kwargs): ...
    def hasFailure(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QLibraryInfo(_Object):
    @staticmethod
    def build(*args, **kwargs): ...
    @staticmethod
    def buildDate(*args, **kwargs): ...
    @staticmethod
    def isDebugBuild(*args, **kwargs): ...
    @staticmethod
    def licensedProducts(*args, **kwargs): ...
    @staticmethod
    def licensee(*args, **kwargs): ...
    @staticmethod
    def location(*args, **kwargs): ...
    @staticmethod
    def platformPluginArguments(*args, **kwargs): ...
    ArchDataPath : LibraryLocation
    
    BinariesPath : LibraryLocation
    
    DataPath : LibraryLocation
    
    DocumentationPath : LibraryLocation
    
    ExamplesPath : LibraryLocation
    
    HeadersPath : LibraryLocation
    
    ImportsPath : LibraryLocation
    
    LibrariesPath : LibraryLocation
    
    LibraryExecutablesPath : LibraryLocation
    
    LibraryLocation : Type[LibraryLocation]
    
    PluginsPath : LibraryLocation
    
    PrefixPath : LibraryLocation
    
    Qml2ImportsPath : LibraryLocation
    
    SettingsPath : LibraryLocation
    
    TestsPath : LibraryLocation
    
    TranslationsPath : LibraryLocation


class QItemSelectionRange(_Object):
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
    def bottom(*args, **kwargs): ...
    def bottomRight(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def height(*args, **kwargs): ...
    def indexes(*args, **kwargs): ...
    def intersected(*args, **kwargs): ...
    def intersects(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def left(*args, **kwargs): ...
    def model(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def right(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def top(*args, **kwargs): ...
    def topLeft(*args, **kwargs): ...
    def width(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QBasicTimer(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def isActive(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    def timerId(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QCryptographicHash(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addData(*args, **kwargs): ...
    def reset(*args, **kwargs): ...
    def result(*args, **kwargs): ...
    @staticmethod
    def hash(*args, **kwargs): ...
    Algorithm : Type[Algorithm]
    
    Md4 : Algorithm
    
    Md5 : Algorithm
    
    Sha1 : Algorithm
    
    Sha224 : Algorithm
    
    Sha256 : Algorithm
    
    Sha384 : Algorithm
    
    Sha3_224 : Algorithm
    
    Sha3_256 : Algorithm
    
    Sha3_384 : Algorithm
    
    Sha3_512 : Algorithm
    
    Sha512 : Algorithm
    
    __new__ : builtin_function_or_method


class QPointF(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __div__(*args, **kwargs):
        """
        x.__div__(y) <==> x/y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
        """
        ...
    def __idiv__(*args, **kwargs):
        """
        x.__idiv__(y) <==> x/=y
        """
        ...
    def __imul__(*args, **kwargs):
        """
        x.__imul__(y) <==> x*=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __isub__(*args, **kwargs):
        """
        x.__isub__(y) <==> x-=y
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
    def __mul__(*args, **kwargs):
        """
        x.__mul__(y) <==> x*y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __neg__(*args, **kwargs):
        """
        x.__neg__() <==> -x
        """
        ...
    def __nonzero__(self) -> bool:
        """
        x.__nonzero__() <==> x != 0
        """
        ...
    def __pos__(*args, **kwargs):
        """
        x.__pos__() <==> +x
        """
        ...
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rdiv__(*args, **kwargs):
        """
        x.__rdiv__(y) <==> y/x
        """
        ...
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rmul__(*args, **kwargs):
        """
        x.__rmul__(y) <==> y*x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __rtruediv__(*args, **kwargs):
        """
        x.__rtruediv__(y) <==> y/x
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def __truediv__(*args, **kwargs):
        """
        x.__truediv__(y) <==> x/y
        """
        ...
    def isNull(*args, **kwargs): ...
    def manhattanLength(*args, **kwargs): ...
    def setX(*args, **kwargs): ...
    def setY(*args, **kwargs): ...
    def toPoint(*args, **kwargs): ...
    def toTuple(*args, **kwargs): ...
    def x(*args, **kwargs): ...
    def y(*args, **kwargs): ...
    @staticmethod
    def dotProduct(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QEasingCurve(_Object):
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
    def addCubicBezierSegment(*args, **kwargs): ...
    def addTCBSegment(*args, **kwargs): ...
    def amplitude(*args, **kwargs): ...
    def customType(*args, **kwargs): ...
    def overshoot(*args, **kwargs): ...
    def period(*args, **kwargs): ...
    def setAmplitude(*args, **kwargs): ...
    def setCustomType(*args, **kwargs): ...
    def setOvershoot(*args, **kwargs): ...
    def setPeriod(*args, **kwargs): ...
    def setType(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    def toCubicSpline(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    def valueForProgress(*args, **kwargs): ...
    BezierSpline : Type
    
    CosineCurve : Type
    
    Custom : Type
    
    InBack : Type
    
    InBounce : Type
    
    InCirc : Type
    
    InCubic : Type
    
    InCurve : Type
    
    InElastic : Type
    
    InExpo : Type
    
    InOutBack : Type
    
    InOutBounce : Type
    
    InOutCirc : Type
    
    InOutCubic : Type
    
    InOutElastic : Type
    
    InOutExpo : Type
    
    InOutQuad : Type
    
    InOutQuart : Type
    
    InOutQuint : Type
    
    InOutSine : Type
    
    InQuad : Type
    
    InQuart : Type
    
    InQuint : Type
    
    InSine : Type
    
    Linear : Type
    
    NCurveTypes : Type
    
    OutBack : Type
    
    OutBounce : Type
    
    OutCirc : Type
    
    OutCubic : Type
    
    OutCurve : Type
    
    OutElastic : Type
    
    OutExpo : Type
    
    OutInBack : Type
    
    OutInBounce : Type
    
    OutInCirc : Type
    
    OutInCubic : Type
    
    OutInElastic : Type
    
    OutInExpo : Type
    
    OutInQuad : Type
    
    OutInQuart : Type
    
    OutInQuint : Type
    
    OutInSine : Type
    
    OutQuad : Type
    
    OutQuart : Type
    
    OutQuint : Type
    
    OutSine : Type
    
    SineCurve : Type
    
    TCBSpline : Type
    
    Type : Type[Type]
    
    __new__ : builtin_function_or_method


class QByteArrayMatcher(_Object):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def indexIn(*args, **kwargs): ...
    def pattern(*args, **kwargs): ...
    def setPattern(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QXmlStreamNotationDeclaration(_Object):
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
    def name(*args, **kwargs): ...
    def publicId(*args, **kwargs): ...
    def systemId(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QReadWriteLock(_Object):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def lockForRead(*args, **kwargs): ...
    def lockForWrite(*args, **kwargs): ...
    def tryLockForRead(*args, **kwargs): ...
    def tryLockForWrite(*args, **kwargs): ...
    def unlock(*args, **kwargs): ...
    NonRecursive : RecursionMode
    
    RecursionMode : Type[RecursionMode]
    
    Recursive : RecursionMode
    
    __new__ : builtin_function_or_method


class QMargins(_Object):
    def __add__(*args, **kwargs):
        """
        x.__add__(y) <==> x+y
        """
        ...
    def __copy__(*args, **kwargs): ...
    def __div__(*args, **kwargs):
        """
        x.__div__(y) <==> x/y
        """
        ...
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
    def __iadd__(*args, **kwargs):
        """
        x.__iadd__(y) <==> x+=y
        """
        ...
    def __idiv__(*args, **kwargs):
        """
        x.__idiv__(y) <==> x/=y
        """
        ...
    def __imul__(*args, **kwargs):
        """
        x.__imul__(y) <==> x*=y
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def __isub__(*args, **kwargs):
        """
        x.__isub__(y) <==> x-=y
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
    def __mul__(*args, **kwargs):
        """
        x.__mul__(y) <==> x*y
        """
        ...
    def __ne__(self, other: Any) -> bool:
        """
        x.__ne__(y) <==> x!=y
        """
        ...
    def __neg__(*args, **kwargs):
        """
        x.__neg__() <==> -x
        """
        ...
    def __nonzero__(self) -> bool:
        """
        x.__nonzero__() <==> x != 0
        """
        ...
    def __pos__(*args, **kwargs):
        """
        x.__pos__() <==> +x
        """
        ...
    def __radd__(*args, **kwargs):
        """
        x.__radd__(y) <==> y+x
        """
        ...
    def __rdiv__(*args, **kwargs):
        """
        x.__rdiv__(y) <==> y/x
        """
        ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def __rmul__(*args, **kwargs):
        """
        x.__rmul__(y) <==> y*x
        """
        ...
    def __rsub__(*args, **kwargs):
        """
        x.__rsub__(y) <==> y-x
        """
        ...
    def __rtruediv__(*args, **kwargs):
        """
        x.__rtruediv__(y) <==> y/x
        """
        ...
    def __sub__(*args, **kwargs):
        """
        x.__sub__(y) <==> x-y
        """
        ...
    def __truediv__(*args, **kwargs):
        """
        x.__truediv__(y) <==> x/y
        """
        ...
    def bottom(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def left(*args, **kwargs): ...
    def right(*args, **kwargs): ...
    def setBottom(*args, **kwargs): ...
    def setLeft(*args, **kwargs): ...
    def setRight(*args, **kwargs): ...
    def setTop(*args, **kwargs): ...
    def top(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QXmlStreamAttribute(_Object):
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
    def isDefault(*args, **kwargs): ...
    def name(*args, **kwargs): ...
    def namespaceUri(*args, **kwargs): ...
    def prefix(*args, **kwargs): ...
    def qualifiedName(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QRegExp(_Object):
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def cap(*args, **kwargs): ...
    def captureCount(*args, **kwargs): ...
    def capturedTexts(*args, **kwargs): ...
    def caseSensitivity(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def exactMatch(*args, **kwargs): ...
    def indexIn(*args, **kwargs): ...
    def isEmpty(*args, **kwargs): ...
    def isMinimal(*args, **kwargs): ...
    def isValid(*args, **kwargs): ...
    def lastIndexIn(*args, **kwargs): ...
    def matchedLength(*args, **kwargs): ...
    def pattern(*args, **kwargs): ...
    def patternSyntax(*args, **kwargs): ...
    def pos(*args, **kwargs): ...
    def replace(*args, **kwargs): ...
    def setCaseSensitivity(*args, **kwargs): ...
    def setMinimal(*args, **kwargs): ...
    def setPattern(*args, **kwargs): ...
    def setPatternSyntax(*args, **kwargs): ...
    def swap(*args, **kwargs): ...
    @staticmethod
    def escape(*args, **kwargs): ...
    CaretAtOffset : CaretMode
    
    CaretAtZero : CaretMode
    
    CaretMode : Type[CaretMode]
    
    CaretWontMatch : CaretMode
    
    FixedString : PatternSyntax
    
    PatternSyntax : Type[PatternSyntax]
    
    RegExp : PatternSyntax
    
    RegExp2 : PatternSyntax
    
    W3CXmlSchema11 : PatternSyntax
    
    Wildcard : PatternSyntax
    
    WildcardUnix : PatternSyntax
    
    __new__ : builtin_function_or_method


class QTime(_Object):
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
    def __getattribute__(*args, **kwargs):
        """
        x.__getattribute__('name') <==> x.name
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
    def __reduce__(*args, **kwargs): ...
    def __repr__(self) -> str:
        """
        x.__repr__() <==> repr(x)
        """
        ...
    def addMSecs(*args, **kwargs): ...
    def addSecs(*args, **kwargs): ...
    def elapsed(*args, **kwargs): ...
    def hour(*args, **kwargs): ...
    def isNull(*args, **kwargs): ...
    def minute(*args, **kwargs): ...
    def msec(*args, **kwargs): ...
    def msecsSinceStartOfDay(*args, **kwargs): ...
    def msecsTo(*args, **kwargs): ...
    def restart(*args, **kwargs): ...
    def second(*args, **kwargs): ...
    def secsTo(*args, **kwargs): ...
    def setHMS(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def toPython(*args, **kwargs): ...
    def toString(*args, **kwargs): ...
    @staticmethod
    def currentTime(*args, **kwargs): ...
    @staticmethod
    def fromMSecsSinceStartOfDay(*args, **kwargs): ...
    @staticmethod
    def fromString(*args, **kwargs): ...
    @staticmethod
    def isValid(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QChildEvent(QEvent):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def added(*args, **kwargs): ...
    def child(*args, **kwargs): ...
    def polished(*args, **kwargs): ...
    def removed(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QTimeLine(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def currentFrame(*args, **kwargs): ...
    def currentTime(*args, **kwargs): ...
    def currentValue(*args, **kwargs): ...
    def curveShape(*args, **kwargs): ...
    def direction(*args, **kwargs): ...
    def duration(*args, **kwargs): ...
    def easingCurve(*args, **kwargs): ...
    def endFrame(*args, **kwargs): ...
    def frameForTime(*args, **kwargs): ...
    def loopCount(*args, **kwargs): ...
    def resume(*args, **kwargs): ...
    def setCurrentTime(*args, **kwargs): ...
    def setCurveShape(*args, **kwargs): ...
    def setDirection(*args, **kwargs): ...
    def setDuration(*args, **kwargs): ...
    def setEasingCurve(*args, **kwargs): ...
    def setEndFrame(*args, **kwargs): ...
    def setFrameRange(*args, **kwargs): ...
    def setLoopCount(*args, **kwargs): ...
    def setPaused(*args, **kwargs): ...
    def setStartFrame(*args, **kwargs): ...
    def setUpdateInterval(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def startFrame(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    def timerEvent(*args, **kwargs): ...
    def toggleDirection(*args, **kwargs): ...
    def updateInterval(*args, **kwargs): ...
    def valueForTime(*args, **kwargs): ...
    Backward : Direction
    
    CosineCurve : CurveShape
    
    CurveShape : Type[CurveShape]
    
    Direction : Type[Direction]
    
    EaseInCurve : CurveShape
    
    EaseInOutCurve : CurveShape
    
    EaseOutCurve : CurveShape
    
    Forward : Direction
    
    LinearCurve : CurveShape
    
    NotRunning : State
    
    Paused : State
    
    Running : State
    
    SineCurve : CurveShape
    
    State : Type[State]
    
    __new__ : builtin_function_or_method
    
    finished : Signal
    
    frameChanged : Signal
    
    stateChanged : Signal
    
    staticMetaObject : QMetaObject
    
    valueChanged : Signal


class QAbstractItemModel(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def beginInsertColumns(*args, **kwargs): ...
    def beginInsertRows(*args, **kwargs): ...
    def beginMoveColumns(*args, **kwargs): ...
    def beginMoveRows(*args, **kwargs): ...
    def beginRemoveColumns(*args, **kwargs): ...
    def beginRemoveRows(*args, **kwargs): ...
    def beginResetModel(*args, **kwargs): ...
    def buddy(*args, **kwargs): ...
    def canDropMimeData(*args, **kwargs): ...
    def canFetchMore(*args, **kwargs): ...
    def changePersistentIndex(*args, **kwargs): ...
    def changePersistentIndexList(*args, **kwargs): ...
    def columnCount(*args, **kwargs): ...
    def createIndex(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def decodeData(*args, **kwargs): ...
    def dropMimeData(*args, **kwargs): ...
    def encodeData(*args, **kwargs): ...
    def endInsertColumns(*args, **kwargs): ...
    def endInsertRows(*args, **kwargs): ...
    def endMoveColumns(*args, **kwargs): ...
    def endMoveRows(*args, **kwargs): ...
    def endRemoveColumns(*args, **kwargs): ...
    def endRemoveRows(*args, **kwargs): ...
    def endResetModel(*args, **kwargs): ...
    def fetchMore(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def hasChildren(*args, **kwargs): ...
    def hasIndex(*args, **kwargs): ...
    def headerData(*args, **kwargs): ...
    def index(*args, **kwargs): ...
    def insertColumn(*args, **kwargs): ...
    def insertColumns(*args, **kwargs): ...
    def insertRow(*args, **kwargs): ...
    def insertRows(*args, **kwargs): ...
    def itemData(*args, **kwargs): ...
    def match(*args, **kwargs): ...
    def mimeData(*args, **kwargs): ...
    def mimeTypes(*args, **kwargs): ...
    def moveColumn(*args, **kwargs): ...
    def moveColumns(*args, **kwargs): ...
    def moveRow(*args, **kwargs): ...
    def moveRows(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def persistentIndexList(*args, **kwargs): ...
    def removeColumn(*args, **kwargs): ...
    def removeColumns(*args, **kwargs): ...
    def removeRow(*args, **kwargs): ...
    def removeRows(*args, **kwargs): ...
    def resetInternalData(*args, **kwargs): ...
    def revert(*args, **kwargs): ...
    def roleNames(*args, **kwargs): ...
    def rowCount(*args, **kwargs): ...
    def setData(*args, **kwargs): ...
    def setHeaderData(*args, **kwargs): ...
    def setItemData(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    def sort(*args, **kwargs): ...
    def span(*args, **kwargs): ...
    def submit(*args, **kwargs): ...
    def supportedDragActions(*args, **kwargs): ...
    def supportedDropActions(*args, **kwargs): ...
    HorizontalSortHint : LayoutChangeHint
    
    LayoutChangeHint : Type[LayoutChangeHint]
    
    NoLayoutChangeHint : LayoutChangeHint
    
    VerticalSortHint : LayoutChangeHint
    
    __new__ : builtin_function_or_method
    
    columnsAboutToBeInserted : Signal
    
    columnsAboutToBeMoved : Signal
    
    columnsAboutToBeRemoved : Signal
    
    columnsInserted : Signal
    
    columnsMoved : Signal
    
    columnsRemoved : Signal
    
    dataChanged : Signal
    
    headerDataChanged : Signal
    
    layoutAboutToBeChanged : Signal
    
    layoutChanged : Signal
    
    modelAboutToBeReset : Signal
    
    modelReset : Signal
    
    rowsAboutToBeInserted : Signal
    
    rowsAboutToBeMoved : Signal
    
    rowsAboutToBeRemoved : Signal
    
    rowsInserted : Signal
    
    rowsMoved : Signal
    
    rowsRemoved : Signal
    
    staticMetaObject : QMetaObject


class QSignalMapper(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def map(*args, **kwargs): ...
    def mapping(*args, **kwargs): ...
    def removeMappings(*args, **kwargs): ...
    def setMapping(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    mapped : Signal
    
    staticMetaObject : QMetaObject


class QFileSelector(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def allSelectors(*args, **kwargs): ...
    def extraSelectors(*args, **kwargs): ...
    def select(*args, **kwargs): ...
    def setExtraSelectors(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QMutex(QBasicMutex):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def lock(*args, **kwargs): ...
    def tryLock(*args, **kwargs): ...
    def unlock(*args, **kwargs): ...
    NonRecursive : RecursionMode
    
    RecursionMode : Type[RecursionMode]
    
    Recursive : RecursionMode
    
    __new__ : builtin_function_or_method


class QAbstractTransition(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addAnimation(*args, **kwargs): ...
    def animations(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def eventTest(*args, **kwargs): ...
    def machine(*args, **kwargs): ...
    def onTransition(*args, **kwargs): ...
    def removeAnimation(*args, **kwargs): ...
    def setTargetState(*args, **kwargs): ...
    def setTargetStates(*args, **kwargs): ...
    def setTransitionType(*args, **kwargs): ...
    def sourceState(*args, **kwargs): ...
    def targetState(*args, **kwargs): ...
    def targetStates(*args, **kwargs): ...
    def transitionType(*args, **kwargs): ...
    ExternalTransition : TransitionType
    
    InternalTransition : TransitionType
    
    TransitionType : Type[TransitionType]
    
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject
    
    targetStateChanged : Signal
    
    targetStatesChanged : Signal
    
    triggered : Signal


class QItemSelectionModel(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def clear(*args, **kwargs): ...
    def clearCurrentIndex(*args, **kwargs): ...
    def clearSelection(*args, **kwargs): ...
    def columnIntersectsSelection(*args, **kwargs): ...
    def currentIndex(*args, **kwargs): ...
    def emitSelectionChanged(*args, **kwargs): ...
    def hasSelection(*args, **kwargs): ...
    def isColumnSelected(*args, **kwargs): ...
    def isRowSelected(*args, **kwargs): ...
    def isSelected(*args, **kwargs): ...
    def model(*args, **kwargs): ...
    def reset(*args, **kwargs): ...
    def rowIntersectsSelection(*args, **kwargs): ...
    def select(*args, **kwargs): ...
    def selectedColumns(*args, **kwargs): ...
    def selectedIndexes(*args, **kwargs): ...
    def selectedRows(*args, **kwargs): ...
    def selection(*args, **kwargs): ...
    def setCurrentIndex(*args, **kwargs): ...
    def setModel(*args, **kwargs): ...
    Clear : SelectionFlag
    
    ClearAndSelect : SelectionFlag
    
    Columns : SelectionFlag
    
    Current : SelectionFlag
    
    Deselect : SelectionFlag
    
    NoUpdate : SelectionFlag
    
    Rows : SelectionFlag
    
    Select : SelectionFlag
    
    SelectCurrent : SelectionFlag
    
    SelectionFlag : Type[SelectionFlag]
    
    SelectionFlags : Type[SelectionFlags]
    
    Toggle : SelectionFlag
    
    ToggleCurrent : SelectionFlag
    
    __new__ : builtin_function_or_method
    
    currentChanged : Signal
    
    currentColumnChanged : Signal
    
    currentRowChanged : Signal
    
    modelChanged : Signal
    
    selectionChanged : Signal
    
    staticMetaObject : QMetaObject


class QGenericReturnArgument(QGenericArgument):
    def __copy__(*args, **kwargs): ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    __new__ : builtin_function_or_method


class QEventLoop(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def exec_(*args, **kwargs): ...
    def exit(*args, **kwargs): ...
    def isRunning(*args, **kwargs): ...
    def processEvents(*args, **kwargs): ...
    def quit(*args, **kwargs): ...
    def wakeUp(*args, **kwargs): ...
    AllEvents : ProcessEventsFlag
    
    DialogExec : ProcessEventsFlag
    
    EventLoopExec : ProcessEventsFlag
    
    ExcludeSocketNotifiers : ProcessEventsFlag
    
    ExcludeUserInputEvents : ProcessEventsFlag
    
    ProcessEventsFlag : Type[ProcessEventsFlag]
    
    ProcessEventsFlags : Type[ProcessEventsFlags]
    
    WaitForMoreEvents : ProcessEventsFlag
    
    X11ExcludeTimers : ProcessEventsFlag
    
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QTimer(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def interval(*args, **kwargs): ...
    def isActive(*args, **kwargs): ...
    def isSingleShot(*args, **kwargs): ...
    def killTimer(*args, **kwargs): ...
    def remainingTime(*args, **kwargs): ...
    def setInterval(*args, **kwargs): ...
    def setSingleShot(*args, **kwargs): ...
    def setTimerType(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    def timerEvent(*args, **kwargs): ...
    def timerId(*args, **kwargs): ...
    def timerType(*args, **kwargs): ...
    @staticmethod
    def singleShot(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject
    
    timeout : Signal


class QIODevice(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def atEnd(*args, **kwargs): ...
    def bytesAvailable(*args, **kwargs): ...
    def bytesToWrite(*args, **kwargs): ...
    def canReadLine(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def getChar(*args, **kwargs): ...
    def isOpen(*args, **kwargs): ...
    def isReadable(*args, **kwargs): ...
    def isSequential(*args, **kwargs): ...
    def isTextModeEnabled(*args, **kwargs): ...
    def isWritable(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def openMode(*args, **kwargs): ...
    def peek(*args, **kwargs): ...
    def pos(*args, **kwargs): ...
    def putChar(*args, **kwargs): ...
    def read(*args, **kwargs): ...
    def readAll(*args, **kwargs): ...
    def readData(*args, **kwargs): ...
    def readLine(*args, **kwargs): ...
    def readLineData(*args, **kwargs): ...
    def reset(*args, **kwargs): ...
    def seek(*args, **kwargs): ...
    def setErrorString(*args, **kwargs): ...
    def setOpenMode(*args, **kwargs): ...
    def setTextModeEnabled(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def ungetChar(*args, **kwargs): ...
    def waitForBytesWritten(*args, **kwargs): ...
    def waitForReadyRead(*args, **kwargs): ...
    def write(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    Append : OpenModeFlag
    
    NotOpen : OpenModeFlag
    
    OpenMode : Type[OpenMode]
    
    OpenModeFlag : Type[OpenModeFlag]
    
    ReadOnly : OpenModeFlag
    
    ReadWrite : OpenModeFlag
    
    Text : OpenModeFlag
    
    Truncate : OpenModeFlag
    
    Unbuffered : OpenModeFlag
    
    WriteOnly : OpenModeFlag
    
    __new__ : builtin_function_or_method
    
    aboutToClose : Signal
    
    bytesWritten : Signal
    
    readChannelFinished : Signal
    
    readyRead : Signal
    
    staticMetaObject : QMetaObject


class QPluginLoader(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def errorString(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def instance(*args, **kwargs): ...
    def isLoaded(*args, **kwargs): ...
    def load(*args, **kwargs): ...
    def metaData(*args, **kwargs): ...
    def setFileName(*args, **kwargs): ...
    def unload(*args, **kwargs): ...
    @staticmethod
    def staticInstances(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QThread(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def eventDispatcher(*args, **kwargs): ...
    def exec_(*args, **kwargs): ...
    def exit(*args, **kwargs): ...
    def isFinished(*args, **kwargs): ...
    def isInterruptionRequested(*args, **kwargs): ...
    def isRunning(*args, **kwargs): ...
    def loopLevel(*args, **kwargs): ...
    def priority(*args, **kwargs): ...
    def quit(*args, **kwargs): ...
    def requestInterruption(*args, **kwargs): ...
    def run(*args, **kwargs): ...
    def setEventDispatcher(*args, **kwargs): ...
    def setPriority(*args, **kwargs): ...
    def setStackSize(*args, **kwargs): ...
    def stackSize(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def terminate(*args, **kwargs): ...
    def wait(*args, **kwargs): ...
    @staticmethod
    def currentThread(*args, **kwargs): ...
    @staticmethod
    def currentThreadId(*args, **kwargs): ...
    @staticmethod
    def idealThreadCount(*args, **kwargs): ...
    @staticmethod
    def msleep(*args, **kwargs): ...
    @staticmethod
    def setTerminationEnabled(*args, **kwargs): ...
    @staticmethod
    def sleep(*args, **kwargs): ...
    @staticmethod
    def usleep(*args, **kwargs): ...
    @staticmethod
    def yieldCurrentThread(*args, **kwargs): ...
    HighPriority : Priority
    
    HighestPriority : Priority
    
    IdlePriority : Priority
    
    InheritPriority : Priority
    
    LowPriority : Priority
    
    LowestPriority : Priority
    
    NormalPriority : Priority
    
    Priority : Type[Priority]
    
    TimeCriticalPriority : Priority
    
    __new__ : builtin_function_or_method
    
    finished : Signal
    
    started : Signal
    
    staticMetaObject : QMetaObject


class QMimeData(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def clear(*args, **kwargs): ...
    def colorData(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def formats(*args, **kwargs): ...
    def hasColor(*args, **kwargs): ...
    def hasFormat(*args, **kwargs): ...
    def hasHtml(*args, **kwargs): ...
    def hasImage(*args, **kwargs): ...
    def hasText(*args, **kwargs): ...
    def hasUrls(*args, **kwargs): ...
    def html(*args, **kwargs): ...
    def imageData(*args, **kwargs): ...
    def removeFormat(*args, **kwargs): ...
    def retrieveData(*args, **kwargs): ...
    def setColorData(*args, **kwargs): ...
    def setData(*args, **kwargs): ...
    def setHtml(*args, **kwargs): ...
    def setImageData(*args, **kwargs): ...
    def setText(*args, **kwargs): ...
    def setUrls(*args, **kwargs): ...
    def text(*args, **kwargs): ...
    def urls(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QAbstractAnimation(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def currentLoop(*args, **kwargs): ...
    def currentLoopTime(*args, **kwargs): ...
    def currentTime(*args, **kwargs): ...
    def direction(*args, **kwargs): ...
    def duration(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def group(*args, **kwargs): ...
    def loopCount(*args, **kwargs): ...
    def pause(*args, **kwargs): ...
    def resume(*args, **kwargs): ...
    def setCurrentTime(*args, **kwargs): ...
    def setDirection(*args, **kwargs): ...
    def setLoopCount(*args, **kwargs): ...
    def setPaused(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    def totalDuration(*args, **kwargs): ...
    def updateCurrentTime(*args, **kwargs): ...
    def updateDirection(*args, **kwargs): ...
    def updateState(*args, **kwargs): ...
    Backward : Direction
    
    DeleteWhenStopped : DeletionPolicy
    
    DeletionPolicy : Type[DeletionPolicy]
    
    Direction : Type[Direction]
    
    Forward : Direction
    
    KeepWhenStopped : DeletionPolicy
    
    Paused : State
    
    Running : State
    
    State : Type[State]
    
    Stopped : State
    
    __new__ : builtin_function_or_method
    
    currentLoopChanged : Signal
    
    directionChanged : Signal
    
    finished : Signal
    
    stateChanged : Signal
    
    staticMetaObject : QMetaObject


class QTimerEvent(QEvent):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def timerId(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QDynamicPropertyChangeEvent(QEvent):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def propertyName(*args, **kwargs): ...
    __new__ : builtin_function_or_method


class QCoreApplication(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def notify(*args, **kwargs): ...
    @staticmethod
    def addLibraryPath(*args, **kwargs): ...
    @staticmethod
    def applicationDirPath(*args, **kwargs): ...
    @staticmethod
    def applicationFilePath(*args, **kwargs): ...
    @staticmethod
    def applicationName(*args, **kwargs): ...
    @staticmethod
    def applicationPid(*args, **kwargs): ...
    @staticmethod
    def applicationVersion(*args, **kwargs): ...
    @staticmethod
    def arguments(*args, **kwargs): ...
    @staticmethod
    def closingDown(*args, **kwargs): ...
    @staticmethod
    def eventDispatcher(*args, **kwargs): ...
    @staticmethod
    def exec_(*args, **kwargs): ...
    @staticmethod
    def exit(*args, **kwargs): ...
    @staticmethod
    def flush(*args, **kwargs): ...
    @staticmethod
    def hasPendingEvents(*args, **kwargs): ...
    @staticmethod
    def installTranslator(*args, **kwargs): ...
    @staticmethod
    def instance(*args, **kwargs): ...
    @staticmethod
    def isQuitLockEnabled(*args, **kwargs): ...
    @staticmethod
    def isSetuidAllowed(*args, **kwargs): ...
    @staticmethod
    def libraryPaths(*args, **kwargs): ...
    @staticmethod
    def organizationDomain(*args, **kwargs): ...
    @staticmethod
    def organizationName(*args, **kwargs): ...
    @staticmethod
    def postEvent(*args, **kwargs): ...
    @staticmethod
    def processEvents(*args, **kwargs): ...
    @staticmethod
    def quit(*args, **kwargs): ...
    @staticmethod
    def removeLibraryPath(*args, **kwargs): ...
    @staticmethod
    def removePostedEvents(*args, **kwargs): ...
    @staticmethod
    def removeTranslator(*args, **kwargs): ...
    @staticmethod
    def sendEvent(*args, **kwargs): ...
    @staticmethod
    def sendPostedEvents(*args, **kwargs): ...
    @staticmethod
    def setApplicationName(*args, **kwargs): ...
    @staticmethod
    def setApplicationVersion(*args, **kwargs): ...
    @staticmethod
    def setAttribute(*args, **kwargs): ...
    @staticmethod
    def setEventDispatcher(*args, **kwargs): ...
    @staticmethod
    def setLibraryPaths(*args, **kwargs): ...
    @staticmethod
    def setOrganizationDomain(*args, **kwargs): ...
    @staticmethod
    def setOrganizationName(*args, **kwargs): ...
    @staticmethod
    def setQuitLockEnabled(*args, **kwargs): ...
    @staticmethod
    def setSetuidAllowed(*args, **kwargs): ...
    @staticmethod
    def startingUp(*args, **kwargs): ...
    @staticmethod
    def testAttribute(*args, **kwargs): ...
    @staticmethod
    def translate(*args, **kwargs): ...
    ApplicationFlags : int
    
    __new__ : builtin_function_or_method
    
    aboutToQuit : Signal
    
    applicationNameChanged : Signal
    
    applicationVersionChanged : Signal
    
    organizationDomainChanged : Signal
    
    organizationNameChanged : Signal
    
    staticMetaObject : QMetaObject


class QWinEventNotifier(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def handle(*args, **kwargs): ...
    def isEnabled(*args, **kwargs): ...
    def setEnabled(*args, **kwargs): ...
    def setHandle(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    activated : Signal
    
    staticMetaObject : QMetaObject


class QThreadPool(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def activeThreadCount(*args, **kwargs): ...
    def cancel(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def expiryTimeout(*args, **kwargs): ...
    def maxThreadCount(*args, **kwargs): ...
    def releaseThread(*args, **kwargs): ...
    def reserveThread(*args, **kwargs): ...
    def setExpiryTimeout(*args, **kwargs): ...
    def setMaxThreadCount(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def tryStart(*args, **kwargs): ...
    def waitForDone(*args, **kwargs): ...
    @staticmethod
    def globalInstance(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QTranslator(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def isEmpty(*args, **kwargs): ...
    def load(*args, **kwargs): ...
    def translate(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QSocketNotifier(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def isEnabled(*args, **kwargs): ...
    def setEnabled(*args, **kwargs): ...
    def socket(*args, **kwargs): ...
    def type(*args, **kwargs): ...
    Exception : Type
    
    Read : Type
    
    Type : Type[Type]
    
    Write : Type
    
    __new__ : builtin_function_or_method
    
    activated : Signal
    
    staticMetaObject : QMetaObject


class QSettings(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def allKeys(*args, **kwargs): ...
    def applicationName(*args, **kwargs): ...
    def beginGroup(*args, **kwargs): ...
    def beginReadArray(*args, **kwargs): ...
    def beginWriteArray(*args, **kwargs): ...
    def childGroups(*args, **kwargs): ...
    def childKeys(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def contains(*args, **kwargs): ...
    def endArray(*args, **kwargs): ...
    def endGroup(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def fallbacksEnabled(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def format(*args, **kwargs): ...
    def group(*args, **kwargs): ...
    def iniCodec(*args, **kwargs): ...
    def isWritable(*args, **kwargs): ...
    def organizationName(*args, **kwargs): ...
    def remove(*args, **kwargs): ...
    def scope(*args, **kwargs): ...
    def setArrayIndex(*args, **kwargs): ...
    def setFallbacksEnabled(*args, **kwargs): ...
    def setIniCodec(*args, **kwargs): ...
    def setValue(*args, **kwargs): ...
    def status(*args, **kwargs): ...
    def sync(*args, **kwargs): ...
    def value(*args, **kwargs): ...
    @staticmethod
    def defaultFormat(*args, **kwargs): ...
    @staticmethod
    def setDefaultFormat(*args, **kwargs): ...
    @staticmethod
    def setPath(*args, **kwargs): ...
    AccessError : Status
    
    CustomFormat1 : Format
    
    CustomFormat10 : Format
    
    CustomFormat11 : Format
    
    CustomFormat12 : Format
    
    CustomFormat13 : Format
    
    CustomFormat14 : Format
    
    CustomFormat15 : Format
    
    CustomFormat16 : Format
    
    CustomFormat2 : Format
    
    CustomFormat3 : Format
    
    CustomFormat4 : Format
    
    CustomFormat5 : Format
    
    CustomFormat6 : Format
    
    CustomFormat7 : Format
    
    CustomFormat8 : Format
    
    CustomFormat9 : Format
    
    Format : Type[Format]
    
    FormatError : Status
    
    IniFormat : Format
    
    InvalidFormat : Format
    
    NativeFormat : Format
    
    NoError : Status
    
    Scope : Type[Scope]
    
    Status : Type[Status]
    
    SystemScope : Scope
    
    UserScope : Scope
    
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QAbstractEventDispatcher(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def closingDown(*args, **kwargs): ...
    def flush(*args, **kwargs): ...
    def hasPendingEvents(*args, **kwargs): ...
    def interrupt(*args, **kwargs): ...
    def processEvents(*args, **kwargs): ...
    def registerEventNotifier(*args, **kwargs): ...
    def registerSocketNotifier(*args, **kwargs): ...
    def registerTimer(*args, **kwargs): ...
    def registeredTimers(*args, **kwargs): ...
    def remainingTime(*args, **kwargs): ...
    def startingUp(*args, **kwargs): ...
    def unregisterEventNotifier(*args, **kwargs): ...
    def unregisterSocketNotifier(*args, **kwargs): ...
    def unregisterTimer(*args, **kwargs): ...
    def unregisterTimers(*args, **kwargs): ...
    def wakeUp(*args, **kwargs): ...
    @staticmethod
    def instance(*args, **kwargs): ...
    TimerInfo : Type[TimerInfo]
    
    __new__ : builtin_function_or_method
    
    aboutToBlock : Signal
    
    awake : Signal
    
    staticMetaObject : QMetaObject


class QAbstractState(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def active(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def machine(*args, **kwargs): ...
    def onEntry(*args, **kwargs): ...
    def onExit(*args, **kwargs): ...
    def parentState(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    activeChanged : Signal
    
    entered : Signal
    
    exited : Signal
    
    staticMetaObject : QMetaObject


class QFileSystemWatcher(QObject):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addPath(*args, **kwargs): ...
    def addPaths(*args, **kwargs): ...
    def directories(*args, **kwargs): ...
    def files(*args, **kwargs): ...
    def removePath(*args, **kwargs): ...
    def removePaths(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    directoryChanged : Signal
    
    fileChanged : Signal
    
    staticMetaObject : QMetaObject


class QSignalTransition(QAbstractTransition):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def eventTest(*args, **kwargs): ...
    def onTransition(*args, **kwargs): ...
    def senderObject(*args, **kwargs): ...
    def setSenderObject(*args, **kwargs): ...
    def setSignal(*args, **kwargs): ...
    def signal(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    senderObjectChanged : Signal
    
    signalChanged : Signal
    
    staticMetaObject : QMetaObject


class QAnimationGroup(QAbstractAnimation):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addAnimation(*args, **kwargs): ...
    def animationAt(*args, **kwargs): ...
    def animationCount(*args, **kwargs): ...
    def clear(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def indexOfAnimation(*args, **kwargs): ...
    def insertAnimation(*args, **kwargs): ...
    def removeAnimation(*args, **kwargs): ...
    def takeAnimation(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QAbstractListModel(QAbstractItemModel):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def columnCount(*args, **kwargs): ...
    def dropMimeData(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def hasChildren(*args, **kwargs): ...
    def index(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QAbstractProxyModel(QAbstractItemModel):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def buddy(*args, **kwargs): ...
    def canDropMimeData(*args, **kwargs): ...
    def canFetchMore(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def dropMimeData(*args, **kwargs): ...
    def fetchMore(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def hasChildren(*args, **kwargs): ...
    def headerData(*args, **kwargs): ...
    def itemData(*args, **kwargs): ...
    def mapFromSource(*args, **kwargs): ...
    def mapSelectionFromSource(*args, **kwargs): ...
    def mapSelectionToSource(*args, **kwargs): ...
    def mapToSource(*args, **kwargs): ...
    def mimeData(*args, **kwargs): ...
    def mimeTypes(*args, **kwargs): ...
    def resetInternalData(*args, **kwargs): ...
    def revert(*args, **kwargs): ...
    def setData(*args, **kwargs): ...
    def setHeaderData(*args, **kwargs): ...
    def setItemData(*args, **kwargs): ...
    def setSourceModel(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    def sort(*args, **kwargs): ...
    def sourceModel(*args, **kwargs): ...
    def span(*args, **kwargs): ...
    def submit(*args, **kwargs): ...
    def supportedDragActions(*args, **kwargs): ...
    def supportedDropActions(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    sourceModelChanged : Signal
    
    staticMetaObject : QMetaObject


class QHistoryState(QAbstractState):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def defaultState(*args, **kwargs): ...
    def defaultTransition(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def historyType(*args, **kwargs): ...
    def onEntry(*args, **kwargs): ...
    def onExit(*args, **kwargs): ...
    def setDefaultState(*args, **kwargs): ...
    def setDefaultTransition(*args, **kwargs): ...
    def setHistoryType(*args, **kwargs): ...
    DeepHistory : HistoryType
    
    HistoryType : Type[HistoryType]
    
    ShallowHistory : HistoryType
    
    __new__ : builtin_function_or_method
    
    defaultStateChanged : Signal
    
    defaultTransitionChanged : Signal
    
    historyTypeChanged : Signal
    
    staticMetaObject : QMetaObject


class QEventTransition(QAbstractTransition):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def eventSource(*args, **kwargs): ...
    def eventTest(*args, **kwargs): ...
    def eventType(*args, **kwargs): ...
    def onTransition(*args, **kwargs): ...
    def setEventSource(*args, **kwargs): ...
    def setEventType(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QPauseAnimation(QAbstractAnimation):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def duration(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def setDuration(*args, **kwargs): ...
    def updateCurrentTime(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QFinalState(QAbstractState):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def onEntry(*args, **kwargs): ...
    def onExit(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QAbstractTableModel(QAbstractItemModel):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def dropMimeData(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def hasChildren(*args, **kwargs): ...
    def index(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QState(QAbstractState):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addTransition(*args, **kwargs): ...
    def assignProperty(*args, **kwargs): ...
    def childMode(*args, **kwargs): ...
    def errorState(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def initialState(*args, **kwargs): ...
    def onEntry(*args, **kwargs): ...
    def onExit(*args, **kwargs): ...
    def removeTransition(*args, **kwargs): ...
    def setChildMode(*args, **kwargs): ...
    def setErrorState(*args, **kwargs): ...
    def setInitialState(*args, **kwargs): ...
    def transitions(*args, **kwargs): ...
    ChildMode : Type[ChildMode]
    
    DontRestoreProperties : RestorePolicy
    
    ExclusiveStates : ChildMode
    
    ParallelStates : ChildMode
    
    RestorePolicy : Type[RestorePolicy]
    
    RestoreProperties : RestorePolicy
    
    __new__ : builtin_function_or_method
    
    childModeChanged : Signal
    
    errorStateChanged : Signal
    
    finished : Signal
    
    initialStateChanged : Signal
    
    propertiesAssigned : Signal
    
    staticMetaObject : QMetaObject


class QVariantAnimation(QAbstractAnimation):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def currentValue(*args, **kwargs): ...
    def duration(*args, **kwargs): ...
    def easingCurve(*args, **kwargs): ...
    def endValue(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def interpolated(*args, **kwargs): ...
    def keyValueAt(*args, **kwargs): ...
    def keyValues(*args, **kwargs): ...
    def setDuration(*args, **kwargs): ...
    def setEasingCurve(*args, **kwargs): ...
    def setEndValue(*args, **kwargs): ...
    def setKeyValueAt(*args, **kwargs): ...
    def setKeyValues(*args, **kwargs): ...
    def setStartValue(*args, **kwargs): ...
    def startValue(*args, **kwargs): ...
    def updateCurrentTime(*args, **kwargs): ...
    def updateCurrentValue(*args, **kwargs): ...
    def updateState(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject
    
    valueChanged : Signal


class QFileDevice(QIODevice):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def atEnd(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def error(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def flush(*args, **kwargs): ...
    def handle(*args, **kwargs): ...
    def isSequential(*args, **kwargs): ...
    def map(*args, **kwargs): ...
    def permissions(*args, **kwargs): ...
    def pos(*args, **kwargs): ...
    def readData(*args, **kwargs): ...
    def readLineData(*args, **kwargs): ...
    def resize(*args, **kwargs): ...
    def seek(*args, **kwargs): ...
    def setPermissions(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def unmap(*args, **kwargs): ...
    def unsetError(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    AbortError : FileError
    
    AutoCloseHandle : FileHandleFlag
    
    CopyError : FileError
    
    DontCloseHandle : FileHandleFlag
    
    ExeGroup : Permission
    
    ExeOther : Permission
    
    ExeOwner : Permission
    
    ExeUser : Permission
    
    FatalError : FileError
    
    FileError : Type[FileError]
    
    FileHandleFlag : Type[FileHandleFlag]
    
    FileHandleFlags : Type[FileHandleFlags]
    
    MapPrivateOption : MemoryMapFlags
    
    MemoryMapFlags : Type[MemoryMapFlags]
    
    NoError : FileError
    
    NoOptions : MemoryMapFlags
    
    OpenError : FileError
    
    Permission : Type[Permission]
    
    Permissions : Type[Permissions]
    
    PermissionsError : FileError
    
    PositionError : FileError
    
    ReadError : FileError
    
    ReadGroup : Permission
    
    ReadOther : Permission
    
    ReadOwner : Permission
    
    ReadUser : Permission
    
    RemoveError : FileError
    
    RenameError : FileError
    
    ResizeError : FileError
    
    ResourceError : FileError
    
    TimeOutError : FileError
    
    UnspecifiedError : FileError
    
    WriteError : FileError
    
    WriteGroup : Permission
    
    WriteOther : Permission
    
    WriteOwner : Permission
    
    WriteUser : Permission
    
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QProcess(QIODevice):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def arguments(*args, **kwargs): ...
    def atEnd(*args, **kwargs): ...
    def bytesAvailable(*args, **kwargs): ...
    def bytesToWrite(*args, **kwargs): ...
    def canReadLine(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def closeReadChannel(*args, **kwargs): ...
    def closeWriteChannel(*args, **kwargs): ...
    def environment(*args, **kwargs): ...
    def exitCode(*args, **kwargs): ...
    def exitStatus(*args, **kwargs): ...
    def inputChannelMode(*args, **kwargs): ...
    def isSequential(*args, **kwargs): ...
    def kill(*args, **kwargs): ...
    def nativeArguments(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def pid(*args, **kwargs): ...
    def processChannelMode(*args, **kwargs): ...
    def processEnvironment(*args, **kwargs): ...
    def processId(*args, **kwargs): ...
    def program(*args, **kwargs): ...
    def readAllStandardError(*args, **kwargs): ...
    def readAllStandardOutput(*args, **kwargs): ...
    def readChannel(*args, **kwargs): ...
    def readData(*args, **kwargs): ...
    def setArguments(*args, **kwargs): ...
    def setEnvironment(*args, **kwargs): ...
    def setInputChannelMode(*args, **kwargs): ...
    def setNativeArguments(*args, **kwargs): ...
    def setProcessChannelMode(*args, **kwargs): ...
    def setProcessEnvironment(*args, **kwargs): ...
    def setProcessState(*args, **kwargs): ...
    def setProgram(*args, **kwargs): ...
    def setReadChannel(*args, **kwargs): ...
    def setStandardErrorFile(*args, **kwargs): ...
    def setStandardInputFile(*args, **kwargs): ...
    def setStandardOutputFile(*args, **kwargs): ...
    def setStandardOutputProcess(*args, **kwargs): ...
    def setWorkingDirectory(*args, **kwargs): ...
    def setupChildProcess(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def state(*args, **kwargs): ...
    def terminate(*args, **kwargs): ...
    def waitForBytesWritten(*args, **kwargs): ...
    def waitForFinished(*args, **kwargs): ...
    def waitForReadyRead(*args, **kwargs): ...
    def waitForStarted(*args, **kwargs): ...
    def workingDirectory(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    @staticmethod
    def execute(*args, **kwargs): ...
    @staticmethod
    def nullDevice(*args, **kwargs): ...
    @staticmethod
    def startDetached(*args, **kwargs): ...
    @staticmethod
    def systemEnvironment(*args, **kwargs): ...
    CrashExit : ExitStatus
    
    Crashed : ProcessError
    
    ExitStatus : Type[ExitStatus]
    
    FailedToStart : ProcessError
    
    ForwardedChannels : ProcessChannelMode
    
    ForwardedErrorChannel : ProcessChannelMode
    
    ForwardedInputChannel : InputChannelMode
    
    ForwardedOutputChannel : ProcessChannelMode
    
    InputChannelMode : Type[InputChannelMode]
    
    ManagedInputChannel : InputChannelMode
    
    MergedChannels : ProcessChannelMode
    
    NormalExit : ExitStatus
    
    NotRunning : ProcessState
    
    ProcessChannel : Type[ProcessChannel]
    
    ProcessChannelMode : Type[ProcessChannelMode]
    
    ProcessError : Type[ProcessError]
    
    ProcessState : Type[ProcessState]
    
    ReadError : ProcessError
    
    Running : ProcessState
    
    SeparateChannels : ProcessChannelMode
    
    StandardError : ProcessChannel
    
    StandardOutput : ProcessChannel
    
    Starting : ProcessState
    
    Timedout : ProcessError
    
    UnknownError : ProcessError
    
    WriteError : ProcessError
    
    __new__ : builtin_function_or_method
    
    error : Signal
    
    errorOccurred : Signal
    
    finished : Signal
    
    readyReadStandardError : Signal
    
    readyReadStandardOutput : Signal
    
    started : Signal
    
    stateChanged : Signal
    
    staticMetaObject : QMetaObject


class QBuffer(QIODevice):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def atEnd(*args, **kwargs): ...
    def buffer(*args, **kwargs): ...
    def canReadLine(*args, **kwargs): ...
    def close(*args, **kwargs): ...
    def connectNotify(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def disconnectNotify(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def pos(*args, **kwargs): ...
    def readData(*args, **kwargs): ...
    def seek(*args, **kwargs): ...
    def setBuffer(*args, **kwargs): ...
    def setData(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    def writeData(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QPropertyAnimation(QVariantAnimation):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def event(*args, **kwargs): ...
    def propertyName(*args, **kwargs): ...
    def setPropertyName(*args, **kwargs): ...
    def setTargetObject(*args, **kwargs): ...
    def targetObject(*args, **kwargs): ...
    def updateCurrentValue(*args, **kwargs): ...
    def updateState(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QSequentialAnimationGroup(QAnimationGroup):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addPause(*args, **kwargs): ...
    def currentAnimation(*args, **kwargs): ...
    def duration(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def insertPause(*args, **kwargs): ...
    def updateCurrentTime(*args, **kwargs): ...
    def updateDirection(*args, **kwargs): ...
    def updateState(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    currentAnimationChanged : Signal
    
    staticMetaObject : QMetaObject


class QSortFilterProxyModel(QAbstractProxyModel):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def buddy(*args, **kwargs): ...
    def canFetchMore(*args, **kwargs): ...
    def columnCount(*args, **kwargs): ...
    def data(*args, **kwargs): ...
    def dropMimeData(*args, **kwargs): ...
    def dynamicSortFilter(*args, **kwargs): ...
    def fetchMore(*args, **kwargs): ...
    def filterAcceptsColumn(*args, **kwargs): ...
    def filterAcceptsRow(*args, **kwargs): ...
    def filterCaseSensitivity(*args, **kwargs): ...
    def filterKeyColumn(*args, **kwargs): ...
    def filterRegExp(*args, **kwargs): ...
    def filterRole(*args, **kwargs): ...
    def flags(*args, **kwargs): ...
    def hasChildren(*args, **kwargs): ...
    def headerData(*args, **kwargs): ...
    def index(*args, **kwargs): ...
    def insertColumns(*args, **kwargs): ...
    def insertRows(*args, **kwargs): ...
    def invalidate(*args, **kwargs): ...
    def invalidateFilter(*args, **kwargs): ...
    def isSortLocaleAware(*args, **kwargs): ...
    def lessThan(*args, **kwargs): ...
    def mapFromSource(*args, **kwargs): ...
    def mapSelectionFromSource(*args, **kwargs): ...
    def mapSelectionToSource(*args, **kwargs): ...
    def mapToSource(*args, **kwargs): ...
    def match(*args, **kwargs): ...
    def mimeData(*args, **kwargs): ...
    def mimeTypes(*args, **kwargs): ...
    def parent(*args, **kwargs): ...
    def removeColumns(*args, **kwargs): ...
    def removeRows(*args, **kwargs): ...
    def rowCount(*args, **kwargs): ...
    def setData(*args, **kwargs): ...
    def setDynamicSortFilter(*args, **kwargs): ...
    def setFilterCaseSensitivity(*args, **kwargs): ...
    def setFilterFixedString(*args, **kwargs): ...
    def setFilterKeyColumn(*args, **kwargs): ...
    def setFilterRegExp(*args, **kwargs): ...
    def setFilterRole(*args, **kwargs): ...
    def setFilterWildcard(*args, **kwargs): ...
    def setHeaderData(*args, **kwargs): ...
    def setSortCaseSensitivity(*args, **kwargs): ...
    def setSortLocaleAware(*args, **kwargs): ...
    def setSortRole(*args, **kwargs): ...
    def setSourceModel(*args, **kwargs): ...
    def sibling(*args, **kwargs): ...
    def sort(*args, **kwargs): ...
    def sortCaseSensitivity(*args, **kwargs): ...
    def sortColumn(*args, **kwargs): ...
    def sortOrder(*args, **kwargs): ...
    def sortRole(*args, **kwargs): ...
    def span(*args, **kwargs): ...
    def supportedDropActions(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QStateMachine(QState):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def addDefaultAnimation(*args, **kwargs): ...
    def addState(*args, **kwargs): ...
    def beginMicrostep(*args, **kwargs): ...
    def beginSelectTransitions(*args, **kwargs): ...
    def cancelDelayedEvent(*args, **kwargs): ...
    def clearError(*args, **kwargs): ...
    def configuration(*args, **kwargs): ...
    def defaultAnimations(*args, **kwargs): ...
    def endMicrostep(*args, **kwargs): ...
    def endSelectTransitions(*args, **kwargs): ...
    def error(*args, **kwargs): ...
    def errorString(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def eventFilter(*args, **kwargs): ...
    def globalRestorePolicy(*args, **kwargs): ...
    def isAnimated(*args, **kwargs): ...
    def isRunning(*args, **kwargs): ...
    def onEntry(*args, **kwargs): ...
    def onExit(*args, **kwargs): ...
    def postDelayedEvent(*args, **kwargs): ...
    def postEvent(*args, **kwargs): ...
    def removeDefaultAnimation(*args, **kwargs): ...
    def removeState(*args, **kwargs): ...
    def setAnimated(*args, **kwargs): ...
    def setGlobalRestorePolicy(*args, **kwargs): ...
    def setRunning(*args, **kwargs): ...
    def start(*args, **kwargs): ...
    def stop(*args, **kwargs): ...
    Error : Type[Error]
    
    EventPriority : Type[EventPriority]
    
    HighPriority : EventPriority
    
    NoCommonAncestorForTransitionError : Error
    
    NoDefaultStateInHistoryStateError : Error
    
    NoError : Error
    
    NoInitialStateError : Error
    
    NormalPriority : EventPriority
    
    SignalEvent : Type[SignalEvent]
    
    WrappedEvent : Type[WrappedEvent]
    
    __new__ : builtin_function_or_method
    
    runningChanged : Signal
    
    started : Signal
    
    staticMetaObject : QMetaObject
    
    stopped : Signal


class QParallelAnimationGroup(QAnimationGroup):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def duration(*args, **kwargs): ...
    def event(*args, **kwargs): ...
    def updateCurrentTime(*args, **kwargs): ...
    def updateDirection(*args, **kwargs): ...
    def updateState(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QFile(QFileDevice):
    def __getattribute__(*args, **kwargs):
        """
        x.__getattribute__('name') <==> x.name
        """
        ...
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def fileName(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def setFileName(*args, **kwargs): ...
    def size(*args, **kwargs): ...
    @staticmethod
    def copy(*args, **kwargs): ...
    @staticmethod
    def decodeName(*args, **kwargs): ...
    @staticmethod
    def encodeName(*args, **kwargs): ...
    @staticmethod
    def exists(*args, **kwargs): ...
    @staticmethod
    def link(*args, **kwargs): ...
    @staticmethod
    def permissions(*args, **kwargs): ...
    @staticmethod
    def readLink(*args, **kwargs): ...
    @staticmethod
    def remove(*args, **kwargs): ...
    @staticmethod
    def rename(*args, **kwargs): ...
    @staticmethod
    def resize(*args, **kwargs): ...
    @staticmethod
    def setPermissions(*args, **kwargs): ...
    @staticmethod
    def symLinkTarget(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject


class QTemporaryFile(QFile):
    def __init__(*args, **kwargs):
        """
        x.__init__(...) initializes x; see help(type(x)) for signature
        """
        ...
    def autoRemove(*args, **kwargs): ...
    def fileName(*args, **kwargs): ...
    def fileTemplate(*args, **kwargs): ...
    def open(*args, **kwargs): ...
    def setAutoRemove(*args, **kwargs): ...
    def setFileTemplate(*args, **kwargs): ...
    @staticmethod
    def createLocalFile(*args, **kwargs): ...
    @staticmethod
    def createNativeFile(*args, **kwargs): ...
    __new__ : builtin_function_or_method
    
    staticMetaObject : QMetaObject




def QT_TRANSLATE_NOOP3(*args, **kwargs): ...
def qFatal(*args, **kwargs): ...
def qAbs(*args, **kwargs): ...
def qAtan(*args, **kwargs): ...
def qAcos(*args, **kwargs): ...
def QT_TRANSLATE_NOOP_UTF8(*args, **kwargs): ...
def qRegisterResourceData(*args, **kwargs): ...
def qDebug(*args, **kwargs): ...
def qWarning(*args, **kwargs): ...
def QT_TR_NOOP(*args, **kwargs): ...
def QT_TR_NOOP_UTF8(*args, **kwargs): ...
def qCritical(*args, **kwargs): ...
def qInstallMessageHandler(*args, **kwargs): ...
def qIsNull(*args, **kwargs): ...
def qtTrId(*args, **kwargs): ...
def qIsFinite(*args, **kwargs): ...
def SIGNAL(*args, **kwargs): ...
def qFastSin(*args, **kwargs): ...
def qAddPostRoutine(*args, **kwargs): ...
def qChecksum(*args, **kwargs): ...
def qAsin(*args, **kwargs): ...
def qFastCos(*args, **kwargs): ...
def SLOT(*args, **kwargs): ...
def qFuzzyIsNull(*args, **kwargs): ...
def qIsNaN(*args, **kwargs): ...
def qUnregisterResourceData(*args, **kwargs): ...
def __moduleShutdown(*args, **kwargs): ...
def qVersion(*args, **kwargs): ...
def qAtan2(*args, **kwargs): ...
def qFabs(*args, **kwargs): ...
def QT_TRANSLATE_NOOP(*args, **kwargs): ...
def qFuzzyCompare(*args, **kwargs): ...
def qIsInf(*args, **kwargs): ...
def qTan(*args, **kwargs): ...
def qrand(*args, **kwargs): ...
def qsrand(*args, **kwargs): ...
def qExp(*args, **kwargs): ...


QtCriticalMsg : QtMsgType
QtSystemMsg : QtMsgType
QtFatalMsg : QtMsgType
QtInfoMsg : QtMsgType
QtDebugMsg : QtMsgType
__version__ : str
QtWarningMsg : QtMsgType

