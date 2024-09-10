from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
if False:
    from typing import Dict, List, Tuple, Union, Optional

class _Object(object):
    __dict__ : dictproxy


class QTest(_Object):
    @staticmethod
    def addColumnInternal(*args, **kwargs): ...
    @staticmethod
    def asciiToKey(*args, **kwargs): ...
    @staticmethod
    def compare_ptr_helper(*args, **kwargs): ...
    @staticmethod
    def compare_string_helper(*args, **kwargs): ...
    @staticmethod
    def currentAppName(*args, **kwargs): ...
    @staticmethod
    def currentDataTag(*args, **kwargs): ...
    @staticmethod
    def currentTestFailed(*args, **kwargs): ...
    @staticmethod
    def currentTestFunction(*args, **kwargs): ...
    @staticmethod
    def ignoreMessage(*args, **kwargs): ...
    @staticmethod
    def keyClick(*args, **kwargs): ...
    @staticmethod
    def keyClicks(*args, **kwargs): ...
    @staticmethod
    def keyEvent(*args, **kwargs): ...
    @staticmethod
    def keyPress(*args, **kwargs): ...
    @staticmethod
    def keyRelease(*args, **kwargs): ...
    @staticmethod
    def keyToAscii(*args, **kwargs): ...
    @staticmethod
    def mouseClick(*args, **kwargs): ...
    @staticmethod
    def mouseDClick(*args, **kwargs): ...
    @staticmethod
    def mouseEvent(*args, **kwargs): ...
    @staticmethod
    def mouseMove(*args, **kwargs): ...
    @staticmethod
    def mousePress(*args, **kwargs): ...
    @staticmethod
    def mouseRelease(*args, **kwargs): ...
    @staticmethod
    def qElementData(*args, **kwargs): ...
    @staticmethod
    def qExpectFail(*args, **kwargs): ...
    @staticmethod
    def qFindTestData(*args, **kwargs): ...
    @staticmethod
    def qGlobalData(*args, **kwargs): ...
    @staticmethod
    def qSkip(*args, **kwargs): ...
    @staticmethod
    def qWaitForWindowActive(*args, **kwargs): ...
    @staticmethod
    def qWaitForWindowExposed(*args, **kwargs): ...
    @staticmethod
    def sendKeyEvent(*args, **kwargs): ...
    @staticmethod
    def setBenchmarkResult(*args, **kwargs): ...
    @staticmethod
    def setMainSourcePath(*args, **kwargs): ...
    @staticmethod
    def simulateEvent(*args, **kwargs): ...
    @staticmethod
    def testObject(*args, **kwargs): ...
    @staticmethod
    def toPrettyCString(*args, **kwargs): ...
    @staticmethod
    def touchEvent(*args, **kwargs): ...
    @staticmethod
    def waitForEvents(*args, **kwargs): ...
    Abort : TestFailMode
    
    AlignmentFaults : QBenchmarkMetric
    
    BitsPerSecond : QBenchmarkMetric
    
    BranchInstructions : QBenchmarkMetric
    
    BranchMisses : QBenchmarkMetric
    
    BusCycles : QBenchmarkMetric
    
    BytesAllocated : QBenchmarkMetric
    
    BytesPerSecond : QBenchmarkMetric
    
    CPUCycles : QBenchmarkMetric
    
    CPUMigrations : QBenchmarkMetric
    
    CPUTicks : QBenchmarkMetric
    
    CacheMisses : QBenchmarkMetric
    
    CachePrefetchMisses : QBenchmarkMetric
    
    CachePrefetches : QBenchmarkMetric
    
    CacheReadMisses : QBenchmarkMetric
    
    CacheReads : QBenchmarkMetric
    
    CacheReferences : QBenchmarkMetric
    
    CacheWriteMisses : QBenchmarkMetric
    
    CacheWrites : QBenchmarkMetric
    
    Click : KeyAction
    
    ContextSwitches : QBenchmarkMetric
    
    Continue : TestFailMode
    
    EmulationFaults : QBenchmarkMetric
    
    Events : QBenchmarkMetric
    
    FramesPerSecond : QBenchmarkMetric
    
    InstructionReads : QBenchmarkMetric
    
    Instructions : QBenchmarkMetric
    
    KeyAction : Type[KeyAction]
    
    MajorPageFaults : QBenchmarkMetric
    
    MinorPageFaults : QBenchmarkMetric
    
    MouseAction : Type[MouseAction]
    
    MouseClick : MouseAction
    
    MouseDClick : MouseAction
    
    MouseMove : MouseAction
    
    MousePress : MouseAction
    
    MouseRelease : MouseAction
    
    PageFaults : QBenchmarkMetric
    
    Press : KeyAction
    
    QBenchmarkMetric : Type[QBenchmarkMetric]
    
    QTouchEventSequence : Type[QTouchEventSequence]
    
    Release : KeyAction
    
    Shortcut : KeyAction
    
    StalledCycles : QBenchmarkMetric
    
    TestFailMode : Type[TestFailMode]
    
    WalltimeMilliseconds : QBenchmarkMetric
    
    WalltimeNanoseconds : QBenchmarkMetric



