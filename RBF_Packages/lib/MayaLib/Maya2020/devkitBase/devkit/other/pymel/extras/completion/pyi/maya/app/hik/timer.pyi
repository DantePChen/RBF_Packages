from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import string
import maya.mel as mel
import maya.utils as utils
import maya.cmds as cmds
import sys
import threading
import time


if False:
    from typing import Dict, List, Tuple, Union, Optional

class TimerObj(threading.Thread):
    def __init__(self, runTime, command): ...
    def run(self): ...




def startTimer(runTime, command): ...
def prepMelCommand(commandString):
    """
    ######################
    #       functions
    ######################
    """
    ...

