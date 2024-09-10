from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import plug
from . import typeIDs
from . import renderSetup
from . import utils
import maya.api.OpenMaya as OpenMaya
from . import applyOverride
from . import autoKey


"""
The overridden attribute manager is a singleton that observes the scene to
react to attribute changes.  If the attribute change is on an attribute
that is overridden by render setup, the attribute manager will attempt to
take the value change and reproduce it on the override itself.

This allows for convenient workflows like using direct manipulation on an
object with a value override, where the value is actually written back to
the override.

Apply value override nodes conditionally implement the passive output plug
behavior, through a chain of responsibility.  A passive output allows
setting its destination input.  If this destination input is connected to
an apply override node, the overridden attribute manager asks the
highest-priority apply override node to write the value to its
corresponding override, if it's enabled, else pass the request to the next
lower-priority apply override node.  The chain ends by writing into the
original.  If the highest-priority apply override node returns true from
isPassiveOutput(), this means that the overridden attribute write must
succeed, as one of the apply override nodes in the chain will accept the
write.

Autokey functionality is supported in this framework: in autokey mode, we
query the auto keyer to ask if an overridden attribute would be auto-keyed.
If so, we add the override attribute to the list of attributes the auto
keyer will add keys to.  See the autoKey render setup module and the
autoKeyframe command for more information.

Note that it is understood that changing the override value will cause all
overridden attributes to change.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class OverriddenAttributeManager(object):
    """
    Observe and react to overridden attribute changes.
    
    The overridden attribute manager attempts to convert changes to
    overridden attributes to changes to overrides.  See the module
    documentation for more details.
    
    The overridden attribute manager is only active when a render layer
    other than the default (or master) layer is visible.
    """
    
    
    
    def __init__(self): ...
    def aboutToDelete(self):
        """
        Final clean up before the manager is destroyed.
        """
        ...
    def addAttributeChangeObservation(self):
        """
        Start observing DG attribute changes.
        """
        ...
    def isObserving(self):
        """
        Returns state of DG attribute change observation.
        """
        ...
    def onAttributeChanged(self, msg, plg, otherPlug, clientData): ...
    def onRenderLayerChanged(self):
        """
        Called after the visible render layer has been changed.
        """
        ...
    def removeAttributeChangeObservation(self):
        """
        End observation of DG attribute changes.
        """
        ...
    def renderSetupAdded(self):
        """
        Called just after the render setup node has been added.
        """
        ...
    def renderSetupPreDelete(self):
        """
        Called just before the render setup node is deleted.
        
        Unregisters from visible render layer and attribute change
        observation.
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def isDefaultRenderLayerVisible(): ...
def instance(): ...
def initialize(): ...
def finalize(): ...


_instance : OverriddenAttributeManager

