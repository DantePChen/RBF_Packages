from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.cmds as cmds


"""
These are utilities to interact with Maya.  They give basic building blocks to
wrap simple operations in easier-to-use tools.

These can be used inside Maya and MayaLT.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class Singleton(type):
    """
    Helper for defining a singleton class in Python. You define it by starting
    your class as follows:
    
        from maya.common.utils import Singleton
        class MyClass(object):
            __metaclass__ = Singleton
            ... rest of implementation ...
    
    Then you can do things like this:
    
        a = MyClass()
        a.var = 12
        b = MyClass()
        assert b.var == 12
    """
    
    
    
    def __call__(cls, *args, **kwargs):
        """
        Construct the unique instance of the class if necessary, otherwise return the existing one
        """
        ...
    def singleton_exists(cls):
        """
        :return: True if the named class exists
        """
        ...




def getIndexAfterLastValidElement(attribute):
    """
    This method returns the index right after the last valid element in a multi
    attribute.
    """
    ...
def getSourceNodes(node, attribute, shapes='False'):
    """
    This method returns the name of the nodes connected as sources for the
    given attribute.
    """
    ...
def getSourceNodeFromPlug(plug, shapes='False'):
    """
    This method returns the name of the node connected as a source for the
    given plug.
    """
    ...
def getSourceNodesFromPlug(plug, shapes='False'):
    """
    This method returns the name of the nodes connected as sources for the
    given plug.
    """
    ...
def getSourceNode(node, attribute, shapes='False'):
    """
    This method returns the name of the node connected as a source for the
    given attribute.
    """
    ...

