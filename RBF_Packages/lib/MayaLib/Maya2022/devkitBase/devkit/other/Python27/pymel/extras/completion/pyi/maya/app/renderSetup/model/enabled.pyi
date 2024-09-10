from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya


"""
Render setup overrides and collections can be enabled and disabled.

Disabling an override removes its effect, but keeps the override itself.

Disabling a collection disables all the overrides in its list, as well
as disabling any child (nested) collection it may have.

To implement this behavior, overrides and collections have three
attributes:

1) An enabled attribute.  This attribute is readable only (output), and
   is (trivially) computed from the two following attributes.
2) A self enabled attribute.  This writable attribute determines whether
   the override or collection itself is enabled.
3) A parent enabled attribute.  This writable attribute is connected to
   its parent's enabled output attribute, unless it is a collection
   immediately under a render layer.

The enabled output boolean value is the logical and of the self enabled
attribute and the parent enabled attribute.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def computeEnabled(node, dataBlock):
    """
    Returns the enabled state based on the basic conditions (selfEnabled and parentEnabled).
    """
    ...
def setEnabledOutput(node, dataBlock, value): ...
def createBoolOutputAttribute(longName, shortName, defaultValue):
    """
    Helper method to create an output (readable) boolean attribute
    """
    ...
def createBoolAttribute(longName, shortName, defaultValue):
    """
    Helper method to create an input (writable) boolean attribute
    """
    ...
def createIntAttribute(longName, shortName, defaultValue):
    """
    Helper method to create an input (writable) int attribute
    """
    ...
def createHiddenIntAttribute(longName, shortName):
    """
    Helper method to create a hidden, readable, non-keyable, and
    writable integer attribute.
    """
    ...
def compute(node, plug, dataBlock):
    """
    Computes the enabled plug with the basic conditions (selfEnabled and parentEnabled).
    Do not use if 'enabled' depends on other attributes.
    """
    ...
def createNumIsolatedChildrenAttribute():
    """
    Helper method to create the number of isolated children attribute.
    
    This renderLayer and collection attribute is a count of the number
    of isolate selected children in the subtree of the render layer
    or collection.
    """
    ...
def initializeAttributes(cls): ...

