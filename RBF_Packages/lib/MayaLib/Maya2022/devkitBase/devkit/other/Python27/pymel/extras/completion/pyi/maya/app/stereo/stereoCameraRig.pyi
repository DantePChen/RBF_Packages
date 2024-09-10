from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import sys
import maya.mel as mel
from . import stereoCameraErrors
from . import stereoCameraUtil
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

def makeStereoCameraRig(rigRoot, rigTypeName, leftCam, rightCam):
    """
    Take the root of a hiearchy, a left and right camera under that
    root, and build the attributes and connections necessary for Maya
    to consider it as a stereo rig.
    """
    ...
def isRigRoot(dagObject):
    """
    Return true if this DAG object is the root of a stereo rig.
    """
    ...
def __isCamera(node):
    """
    Return true if the argument is a camera shape, or a transform above one
    """
    ...
def setStereoPair(rigRoot, leftCam, rightCam):
    """
    Take an existing rig, and change the left and right ca
    """
    ...
def __validateRig(rigRoot, leftCam, rightCam):
    """
    Make sure the 3 objects form a valid stereo rig. If the rig is
    valid, return the transforms, even if the shapes were passed in.
    If the rig is invalid, print an error message and return
    [None, None, None]
    """
    ...
def rigRoot(dagObject):
    """
    Return the root of the rig if this dagObject belongs to a stereo rig.
    Returns an empty string if the dagObject does not belong to any rig.
    """
    ...
def selectedCameras():
    """
    Return the current list of selected stereo cameras in the scene.
    """
    ...
def __makeTransform(dagNode): ...
def __findCam(viewCam, attribute):
    """
    Private method to find left, right, center cameras
    """
    ...
def setZeroParallaxPlane(viewCam, distance): ...
def __addAttrAndConnect(attr, node, otherNode):
    """
    Private method to create a dynamic message attribute if it does not
    exist yet. The attribute is then connected to the one passed as
    argument.
    """
    ...
def _followProxyConnection(dagObject): ...
def centerCam(viewCam):
    """
    Same as leftCam for the center camera.
    """
    ...
def leftCam(viewCam):
    """
    Given the main camera node, indicate which camera is the left camera.
    If the left camera coud not be found, viewCam is returned
    """
    ...
def rightCam(viewCam):
    """
    Same as leftCam for the right camera.
    """
    ...
def __followRootConnection(node, attr):
    """
    Return the node connected to the specified attribte on the root
    of the rig.
    Return None for all error cases.
    """
    ...
def listRigs(rigOnly='False'):
    """
    Lists the current stereo camera rigs in the scene. Return the list
    of root nodes.
    """
    ...
def rigType(rigRoot): ...
def createStereoCameraRig(rigName="''"):
    """
    Create a stereo camera rig.
    The rig creation call back is called, then specific dynamic
    attributes are created to track cameras belonging to stereo rigs.
    
    If no rigName is set, the default rig tool is used.
    
    Return an array [rig root, Left eye camera, right eye camera]
    """
    ...

