from functools import partial


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Dialog(object):
    def __init__(self, path="''", name="''", transformConnection="''"): pass
    def add(self):
        """
        Add the transform to the catalog.
        """
        pass
    def build(self): pass
    def onDismissButton(self, data, msg): pass
    def onPathBrowse(self, data): pass
    def remove(self):
        """
        Remove the transform from the catalog.
        """
        pass
    def show(self): pass
    __dict__ = None
    
    
    __weakref__ = None


class ViewTransformDialog(Dialog):
    def __init__(self, path="''", name="''", transformConnection="''"): pass
    def apply(self): pass
    def direction(self): pass
    def title(self): pass
    def type(self): pass


class RenderingSpaceDialog(Dialog):
    def __init__(self, path="''", name="''", transformConnection="''"): pass
    def apply(self): pass
    def direction(self): pass
    def title(self): pass
    def type(self): pass


class InputTransformDialog(Dialog):
    def __init__(self, path="''", name="''", transformConnection="''"): pass
    def apply(self): pass
    def direction(self): pass
    def title(self): pass
    def type(self): pass


class OutputTransformDialog(Dialog):
    def __init__(self, path="''", name="''", transformConnection="''", connectionType="''", outputTarget="''", dialogTitle="''"): pass
    def apply(self): pass
    def direction(self): pass
    def title(self): pass
    def type(self): pass




def addCustomTransformDialog(type):
    """
    Add and return a user transform.
    
    Return transform name, or empty string in case of error or cancel.
    """
    pass
def dialogFactory(type): pass

