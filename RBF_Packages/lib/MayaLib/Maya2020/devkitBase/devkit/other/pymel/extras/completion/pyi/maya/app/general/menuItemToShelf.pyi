from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
import maya.cmds as cmds


from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

def menuItemToShelf(shelf, item):
    """
    Create a shelfButton on a shelf which acts like a menuItem.
    
    Note: If item has dragMenuCommand defined, then in will be executed,
    and the result will be used as the new button's command.
    Otherwise item's command will be used as the new button's command.
    
    If item has dragDoubleClickCommand defined, it will be used as the new
    button's doubleClickCommand.
    
    shelf - the shelf to put the new shelfButton on
    item - the menuItem to copy
    
    eturn the path of the created shelfButton
    """
    ...

