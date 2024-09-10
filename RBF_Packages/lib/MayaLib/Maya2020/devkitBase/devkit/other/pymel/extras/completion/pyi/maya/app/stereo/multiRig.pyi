from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
from . import stereoCameraRig
import maya.cmds as cmds
from . import stereoCameraErrors
from . import stereoCameraUtil
from . import stereoCameraSets
import copy
from . import stereoCameraDefaultRig as defaultRigType


"""
Multi-rigs are the collection of stereo camera rigs contained by a camera set.
This class provides a generic way of defining a multi-rig and creating them.
A multi-rig has 2 parts:

    - Naming information
    - Layer information

Each layer has information on how to populate that layer. It includes

    - Prefix the name of the layer
        - The type of stereo camera
    - Whether an object set should be created for that layer.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class NamingTemplateManager:
    """
    This manages the list of naming templates used by the editor.  Users
    can create customized templates.  Any operation that changes a template:
       - New
       - Delete
       - Modify
    
    Will force the storage of that changed value back into the optionVar
    Thus the optionVar is always in-sync with the manager.  It is possible
    to create more than one template manager; however, multiple instances
    may stomp over each other and provide un-predictable results.
    """
    
    
    
    def __init__(self):
        """
        Class constructor.  Retreive the optionVar for the manager and read 
        the contents.  If the option var does not exist, we create it and
        populate the default values.
        """
        ...
    def addNew(self):
        """
        Create a new template from the default form.
        """
        ...
    def defaultTemplate(self):
        """
        Return the default template.
        """
        ...
    def deleteTemplate(self, template):
        """
        Remove the name template from the template manager.
        """
        ...
    def resetToDefault(self):
        """
        Reset to the default settings.
        """
        ...
    def retreive(self):
        """
        Retreives the template information from the option var that
        contains this data.
        """
        ...
    def store(self):
        """
        Store the template information back onto the option var.
        """
        ...
    def templates(self):
        """
        Return the list of templates.
        """
        ...


class NamingTemplate:
    """
    This class encapsulates the naming convension when creating new camera set
    multi rigs.
    """
    
    
    
    def __init__(self, mgr, template='None'):
        """
        Class initializer.
        """
        ...
    def addLayer(self):
        """
        Add a new layer to this object.
        """
        ...
    def autoCreateSet(self, layer):
        """
        Query the value for the auto create object set option by layer id.
        """
        ...
    def camSetPrefix(self):
        """
        Returns the naming prefix to append to new cameras & sets.
        """
        ...
    def cameraSetNodeType(self):
        """
        Return the node type for the camera set.  This information is used
        to create a new camera set.
        """
        ...
    def checkMultibyte(self): ...
    def create(self, *args):
        """
        Create the Maya nodes per the specification of this template.
        """
        ...
    def deleteLayer(self, layer='0'):
        """
        Remove the specified layer from this template.
        """
        ...
    def gatherFromString(self, sval):
        """
        Takes a string previously packed using 'stringify' and restores
        the state to variables on this class.
        """
        ...
    def layerPrefixForLayer(self, layer):
        """
        Query the value for the layer prefix using layer id.
        """
        ...
    def layers(self):
        """
        Returns the number of layers this template currently holds.
        """
        ...
    def rigName(self):
        """
        Returns the name of this multi rig.
        """
        ...
    def rigTypeForLayer(self, layer):
        """
        Query the name for the rigType for the specified layer.
        """
        ...
    def setAutoCreateSet(self, create='1', layer='0'):
        """
        Set the auto create flag.
        """
        ...
    def setCamSetPrefix(self, camSetPre):
        """
        Sets the prefix name.
        """
        ...
    def setCameraSetNodeType(self, nodeType): ...
    def setLayerCamera(self, camera, layer='0'):
        """
        Change the camera value for the specified layer.
        """
        ...
    def setLayerPrefix(self, prefix, layer):
        """
        Change the prefix value for the layer.
        """
        ...
    def setRigName(self, name):
        """
        Sets the name for this multi-rig.
        """
        ...
    def store(self):
        """
        Forces the storage of the changes the option var back into the
        optionVar.
        """
        ...
    def stringify(self):
        """
        Converts the data members of this class into a string format.
        This is so we can pack the data into a Maya optionVar
        """
        ...




def getDefaultRig():
    """
    # Makes sure the plug-in is loaded.
    """
    ...
def swapDefault(oldDefault, newDefault): ...
def clearDefaultSwapOV(): ...


SwapRigsOptionVar : str
g3RigSetup : tuple
gDefaultTemplates : list

