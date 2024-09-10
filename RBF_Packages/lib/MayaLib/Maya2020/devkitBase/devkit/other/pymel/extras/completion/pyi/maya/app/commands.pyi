from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import maya
import os


if False:
    from typing import Dict, List, Tuple, Union, Optional

def processCommandList():
    """
    Process the "commandList" file that contains the mappings between command names and the
    libraries in which they are found.  This function will install stub functions in maya.cmds
    for all commands that are not yet loaded.  The stub functions will load the required library
    and then execute the command.
    """
    ...
def __makeStubFunc(command, library): ...


commandListLocations : dict

