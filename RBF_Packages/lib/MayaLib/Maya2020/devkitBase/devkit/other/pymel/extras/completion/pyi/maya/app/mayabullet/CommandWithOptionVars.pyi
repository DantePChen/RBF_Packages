from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import logging
import maya


from maya.app.mayabullet.Trace import Trace


if False:
    from typing import Dict, List, Tuple, Union, Optional

class CommandWithOptionVars(object):
    """
    Base class that handles Maya Commands.  Tailored for MenuItems.
     * Executes command from menu
     * Displays option box dialog
     * Gets/Sets OptionVars
    
    This is the Base Class for handling all that is needed for menuItem, the
    menuItem OptionBox w/ Dialog, and commandline calls. One can derive a class
    for individual menuItems/actions with minimal code as shown below.
    
    :Examples:
    MenuItem using prefs: CommandWithOptionVars().executeCommandCB()
    MenuItemOptionBox:    CommandWithOptionVars().createOptionDialog()
    Commandline:          CommandWithOptionVars().command()
    Cmdline w/ override:  CommandWithOptionVars().command(myoption='AAA')
    """
    
    
    
    def __init__(*args, **kw): ...
    def addOptionDialogWidgets(*args, **kw): ...
    def createOptionDialog(*args, **kw): ...
    def executeCommandAndHideOptionBoxCB(*args, **kw): ...
    def executeCommandAndSaveCB(*args, **kw): ...
    def executeCommandCB(*args, **kw): ...
    def getOptionVars(*args, **kw): ...
    def getWidgetValues(*args, **kw): ...
    def hideOptionBoxCB(*args, **kw): ...
    def optionBoxClosing(*args, **kw): ...
    def resetOptionBoxToDefaultsCB(*args, **kw): ...
    def saveOptionBoxPreferencesCB(*args, **kw): ...
    def setOptionVars(*args, **kw): ...
    def setWidgetValues(*args, **kw): ...
    def updateOptionBox(*args, **kw): ...
    def visibilityChangedCB(*args, **kw): ...
    @staticmethod
    def command(*args, **kw): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def retrieveOptionVars(*args, **kw): ...


logger : logging.Logger

