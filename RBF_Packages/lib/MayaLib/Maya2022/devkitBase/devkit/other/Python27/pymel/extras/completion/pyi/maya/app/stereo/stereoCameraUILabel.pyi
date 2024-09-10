from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import os
import string
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class UILabel:
    """
    Wrapper class around UI labels and buttons. It allows us to define
    a piece of UI once and use it across multiple UI elements.
    """
    
    
    
    def __init__(self, label='None', annotation='None', image='None', command='None', command_pack="'<function pack_cmd>'", command_args='[]', command_keywords='{}', option_box='False', check_box="''", disabled='False', radio_cb='[]', enable_cb='[]', divider_label='None'): ...
    def annotation(self): ...
    def check_box(self): ...
    def command(self): ...
    def command_option(self): ...
    def divider_label(self): ...
    def enabled(self):
        """
        Returns true if the UI should be created enabled. If a enable_cb
        is defined then execute the callback to determine if we should
        enable or disable the UI.
        """
        ...
    def image(self): ...
    def label(self): ...
    def option_box(self): ...
    def radioGrpItem(self):
        """
        Returns true if this ui label support check box menu item.
        """
        ...
    def radioItemOn(self):
        """
        If a check box callback has been added to this UI entry. Call that
        callback to determine if the item should be checked.
        """
        ...
    def setArgs(self, args): ...
    def setCommand(self, command): ...
    def type(self): ...


class UILabelGroup(UILabel):
    def __init__(self, label, items, radioGroup='False'): ...
    def items(self): ...
    def postMenuCommand(self): ...
    def radioGroup(self): ...
    def setArgs(self, args): ...
    def type(self): ...




def pack_load(command, command_args, command_keywords):
    """
    Identical to pack_cmd but it insures the plug-in is loaded before
    calling the command.
    """
    ...
def pack_cmd(command, command_args, command_keywords):
    """
    Packs the python command into a string command that can be
    executed by Maya.  Keywords argument is currently not implemented
    and is a placeholder for future code.
    """
    ...


EmptyLabel : UILabel

