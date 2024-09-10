from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import maya.OpenMaya as OpenMaya
import maya.mel as mel
import json
import maya
import maya.cmds as cmds


"""
This module contains the UI code for the QuickRig tool.

It also holds several utility methods that are used by the tool.  These are
used to automatically perform tasks such as guides creation, guides mirroring,
skeleton creation, joint orientation, rig creation, etc.
"""


from maya.common.utils import getSourceNodeFromPlug
from maya.common.ui import showConfirmationDialog
from maya.common.utils import getIndexAfterLastValidElement
from functools import partial
from functools import wraps
from maya.common.utils import getSourceNodes
from maya.common.utils import getSourceNode
from maya.common.ui import LayoutManager
from maya.common.ui import showMessageBox


if False:
    from typing import Dict, List, Tuple, Union, Optional

class QuickRigTool:
    """
    This is the main UI class for the QuickRig tool.
    
    It handles creation of the UI and provides various callbacks to handle
    user interactions.
    """
    
    
    
    def __init__(self, windowName="'quickRigWindowId'"):
        """
        Simple constructor.
        
        It does not create the UI.  UI creation is deferred until create() is
        called
        """
        ...
    def create(self):
        """
        This method completely builds the UI.  It shows the resulting window
        when it is fully created.
        """
        ...
    def updateUI(self):
        """
        This method performs a full UI refresh.
        
        It refreshes the character list and its associated buttons.  It also
        refreshes the HumanIK tool.
        """
        ...


class HIKContext(object):
    """
    Simple Context Manager for restoring HIK Animation settings and managing HIK callbacks
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_value, traceback): ...
    def __init__(self, NodeList): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class UserException(exceptions.Exception):
    """
    This class is the exception class for errors caused by improper user
    interaction.
    """
    
    
    
    def __init__(self, message): ...
    __weakref__ : getset_descriptor


class HIKManipulationScope:
    """
    This class is a simple manager that sets a manipulation mode when entering
    and resets the previous manipulation mode when exiting.
    """
    
    
    
    def __enter__(self): ...
    def __exit__(self, exc_type, exc_value, traceback): ...
    def __init__(self, manipulationMode): ...
    def setManipulationMode(self, mode): ...


class PieceWiseLinearFunction:
    """
    This class handles interpolation between an array of points.
    """
    
    
    
    def __init__(self, points): ...
    def evaluate(self, value): ...


class Vector3:
    """
    This class is a minimalist vector class.
    
    It only does the strict minimum needed by this tool and is not meant to be
    a generic vector class.
    """
    
    
    
    def __add__(self, other): ...
    def __getitem__(self, key): ...
    def __init__(self, *args): ...
    def __mul__(self, other): ...
    def __setitem__(self, key, value): ...
    def __sub__(self, other): ...
    def cross(self, other): ...
    def dot(self, other): ...
    def length(self): ...
    def project(self, target): ...




def removeOutput_step2(input): ...
def callback_step0_UpdateCharacterButtons(tool):
    """
    ###############################################################################
    #                                                                             #
    #  UI callbacks                                                               #
    #                                                                             #
    ###############################################################################
    """
    ...
def removeOutput_step4(input): ...
def getOutput_step2(input): ...
def getInput_step1(input): ...
def removeOutput_step3(input): ...
def hikGetControlRig(character):
    """
    This method returns the control rig of the given HumanIK character if it
    exists, or the empty string otherwise.
    """
    ...
def getOutput_step1(input): ...
def qrSetSkeletonRootNode(character, skeletonRootNode):
    """
    This method associates the skeleton root node to the given character.
    """
    ...
def createReferential(xVector, zTargetVector, zBackupTargetVector):
    """
    This method create a rotation matrix representing a referential
    described by the given vectors.
    
    The matrix will take a point in the local referential and convert it to
    "world" referential.
    
    The X axis of the referential will be oriented towards the given xVector.
    The Z axis will be oriented towards the given zTargetVector, but it will
    be made orthogonal to the X axis.  The Y axis will be chosen as a
    complement of the two others to create a right-handed system.
    """
    ...
def isCloseEnough(a, b, epsilon='1e-06'):
    """
    This method checks whether two values are close to one another within a
    given tolerance.
    """
    ...
def callback_step3_SelectAllGuides(*args, **kwargs): ...
def convertFoot(tweakParameters, kneePosition, anklePosition, footPosition):
    """
    This method takes the position of the foot guides coming out of the
    embedding algorithm (knee, ankle and foot) and maps them to foot joints
    that fits what HumanIK expects.
    """
    ...
def callback_scriptJob_uiDeleted(tool): ...
def createHikSkeleton(character, skeletonParameters):
    """
    This method creates a HumanIK skeleton from a set of parameters
    corresponding to what can be set in the HumanIK skeleton generation tool.
    
    It uses the skeleton generator node, but deletes it when its done.
    """
    ...
def qrGetSkeletonRootNode(character):
    """
    This method returns the root node of the skeleton associated with the given
    character, if any.
    """
    ...
def callback_step2_ShowSegmentationHelp(tool):
    """
    # This callback can work without any pre-condition.
    """
    ...
def callback_step4_CreateSkeleton(*args, **kwargs): ...
def callback_step0_RenameCharacter(tool): ...
def qrGetSceneCharacters():
    """
    This method returns a list of names for all HumanIK characters in the
    current scene that have been created using the Quick Rig tool.
    """
    ...
def getOutput_step5(input): ...
def computeJointOrients(hikInfos, positions, useOrientation, orientTowardsChild):
    """
    This method computes the joint orientations for a given character, given
    the position of the guides.
    
    It returns a map of HumanIK / guide names to the desired referential.
    """
    ...
def hikSetCurrentCharacter(character):
    """
    This method sets the given HumanIK character as the global current HumanIK
    character.
    """
    ...
def createContainer():
    """
    This method creates a container object which fields can be assigned
    dynamically.
    
    For instance, the object returned by this method will allow:
    
    obj = createContainer()
    obj.newAttribute = 'my new attribute value'
    """
    ...
def hikUpdateTool():
    """
    This method refreshes the HumanIK tool UI so that it fits the current
    HumanIK character.
    """
    ...
def callback_step0_CreateCharacter(tool): ...
def qrDeleteGuidesNode(character):
    """
    This method deletes the guides from the given character, if any.
    
    It returns True if the guides were deleted, False otherwise.
    """
    ...
def checkIfSkinExists(meshes):
    """
    This method returns True if any of the mesh in the given list has an
    associated skin cluster, False otherwise.
    """
    ...
def qrClearMeshes(character):
    """
    This method removes all of the meshes associated with the given character.
    """
    ...
def getInput_step4(input): ...
def qruiRefreshGuidesColor(tool): ...
def computeNeededJointOrient(targetMatrix, currentMatrix):
    """
    This method compute the value needed for the joint orient attribute so that
    the given joint orientation is the one given by the target, knowing that
    its current rotation is given by the current matrix.
    """
    ...
def getInputs(tool, level, deleteOutput): ...
def callback_step1_SelectAllMeshes(tool):
    """
    # This callback can work without any pre-condition.
    """
    ...
def qrIsCharacter(character):
    """
    This method checks whether the character was created using the Quick Rig
    tool and therefore can be edited by the tool.
    """
    ...
def callback_step0_DeleteCharacter(*args, **kwargs): ...
def qruiRefreshMeshes(tool): ...
def getSelectedMeshes():
    """
    This method returns the list of all currently selected meshes.
    
    It returns the actual meshes, i.e. the name of the shape nodes, not the
    transform nodes.
    """
    ...
def hikGetSceneCharacters():
    """
    This method returns a list of names for all HumanIK characters in the
    current scene.
    """
    ...
def qrSetGuidesVisibility(guidesNode, visible):
    """
    This method sets the visibility attributes on all the guide nodes.
    """
    ...
def getOutput_step0(input): ...
def qrGetGuidesPositions(character):
    """
    This method returns position of each guide node associated with the given
    character, if any.
    """
    ...
def qrGetBoundingBoxFromGuidesNode(guidesNode):
    """
    This method extracts from the guides node an object representing the
    bounding box of the mesh(es) used for embedding.
    """
    ...
def mirrorJoints(center, axis, joints):
    """
    This method applies mirroring to a list of joints.
    """
    ...
def qrGetGuidesColorFromGuidesNode(guidesNode):
    """
    This method sets the color from the guides node.
    
    It returns the color of the root of all the guides.
    """
    ...
def hikGetSkeletonNodesMap(character):
    """
    This method returns the scene joints for the skeleton associated with the
    given character, if any.
    """
    ...
def callback_step3_MirrorGuidesLeftToRight(*args, **kwargs): ...
def qrGetGuidesNode(character):
    """
    This method returns the name of the guides node associated with the given
    character, if any.
    """
    ...
def convertSpine(tweakParameters, spineCount, wantHipsTranslation, hipsPosition, backPosition, shouldersPosition):
    """
    This method takes the position of the spine guides coming out of the
    embedding algorithm (hips, back and shoulders) and maps them to spine
    joints that fits what HumanIK expects.
    """
    ...
def qrAddInfoAttribute(character):
    """
    This method adds the Quick Rig info attribute to the character node.
    
    This attribute is a compound that holds information about meshes and guides
    associated with this character.
    """
    ...
def hikCreateCharacter(nameHint):
    """
    This method creates a new HumanIK character trying to use the given name
    hint to name the new character.
    """
    ...
def qrGetMeshes(character):
    """
    This method returns the list of meshes associated with the given character,
    if any.
    """
    ...
def getCharacterDefiniton(character):
    """
    This method gets the HumanIK information from the given character.
    
    It uses the character and HumanIK commands to do so, even though all of
    this information would be available from the skeleton settings.
    """
    ...
def callback_step5_BindOptions(*args, **kwargs): ...
def getOutput_step4(input): ...
def callback_step0_RefreshCharacterList(tool): ...
def removeOutput_step1(input): ...
def fabs(*args, **kwargs):
    """
    fabs(x)
    
    Return the absolute value of the float x.
    """
    ...
def getInput_step0(input): ...
def listAveragedJoints(mapGuideToScene):
    """
    This methods creates and returns two lists of joints from the list given as
    'items'.  The first list contains unique tuples where the second element is
    the mirror counterpart of the first element.  The second list contains the
    name of joints that do not have a mirror counterpart, i.e. center joints.
    
    This method is used when all of the joints need to be averaged with their
    mirror counterpart, relative to their position with regards to the mirror
    plane.
    """
    ...
def callback_step3_ChooseGuidesColor(tool):
    """
    # This callback can work without any pre-condition.
    """
    ...
def getOutput_step3(input): ...
def hikGetSkeletonGeneratorNode(character):
    """
    This method returns the name of the skeleton generator node associated with
    the given HumanIK character if it exists, or the empty string otherwise.
    """
    ...
def qrDeleteSkeleton(character):
    """
    This method deletes the skeleton from the given character, if any.
    
    It returns True if the skeleton was deleted, False otherwise.
    """
    ...
def removeOutput_step0(input): ...
def hikDeleteControlRig(character):
    """
    This method deletes the control rig (if any) for the given HumanIK
    character.
    """
    ...
def qrGetGuidesNodesFromGuidesNode(guidesNode):
    """
    This method extracts from the guides node a dictionary associating a guide
    name to the name of the node representing it in the current embedding.
    """
    ...
def qruiGetCharacter(tool):
    """
    ###############################################################################
    #                                                                             #
    #  Quick Rig UI utility tools (methods and classes)                           #
    #                                                                             #
    #  These are very specific to the way the QuickRig UI is built.               #
    #                                                                             #
    ###############################################################################
    """
    ...
def qrSetGuidesNode(character, guidesNode):
    """
    This method associates the guides node to the given character.
    """
    ...
def sqrt(*args, **kwargs):
    """
    sqrt(x)
    
    Return the square root of x.
    """
    ...
def hikGetCurrentCharacter():
    """
    This method returns the name of the current HumanIK character.
    """
    ...
def detachSkinFromMesh(meshes):
    """
    This method unbinds the skin for all the meshes in the given list, if any.
    """
    ...
def qruiGetGuidesColor(tool): ...
def callback_step5_BindSkin(*args, **kwargs): ...
def callback_step4_DeleteSkeleton(*args, **kwargs): ...
def getInput_step5(input): ...
def hikGetJointNodeName(character, jointName):
    """
    This method returns the joint node name in the given HumanIK character for
    the given generic HumanIK joint name.
    
    It does so by following the connection to the character node from the the
    required joint node.
    """
    ...
def qrCreateGuidesNode(embedding):
    """
    This method creates a node in the scene that will store the embedding
    information returned by the skeletonEmbed command.
    
    It will create a root joint to which will be parented one joint for each
    joint in the embedding.  It will also store bounding box information
    as an attribute on that root joint.
    """
    ...
def listMirroredJoints(mapGuideToScene, guidesNode, items, leftToRight):
    """
    This methods creates and returns two lists of joints from the list given as
    'items'.  The first list contains tuples where the first element is the
    source joint and the second element is its mirror counterpart.  The second
    list contains the name of joints that do not have a mirror counterpart,
    i.e. center joints.
    
    The direction of source to destination is determined by the truth value of
    the 'leftToRight' parameter.
    
    This method is used when a set of selected joints must be mirrored to be
    applied to their mirror counterpart.
    """
    ...
def averageJoints(center, axis, joints):
    """
    This method averages a list of joints so that they become perfectly
    symmetric with regards to the mirror plane.
    """
    ...
def positionHikControlRig(definition, positions):
    """
    This method sets the positions of the HumanIK control rig effectors.
    """
    ...
def hikInitialize():
    """
    This method makes sure the HumanIK tool is loaded and visible.
    """
    ...
def qrMirrorGuides(guidesNode, center, axis, leftToRight, guides):
    """
    This method mirrors the given guides with regards to the center.
    
    Each guide in the given guides can either be:
    - A center guide (in which case the guide is brought to the center plane)
    - A symmetry guide (in which case it serves at a source that is applied to
      its corresponding symmetric guide)
    - Not a guide (in which case nothing is done)
    """
    ...
def qrGetGuidesFromEmbedding(skeletonParameters, tweakParameters, embedding):
    """
    This method takes the embedding returned by the skeletonEmbed command and
    converts it to the skeleton guides.
    
    It can add spine, neck and shoulder (clavicle) joints if requested.
    """
    ...
def convertHand(tweakParameters, elbowPosition, handPosition):
    """
    This method takes the position of the hand guides coming out of the
    embedding algorithm (elbow and hand) and maps them to hand joints that
    fits what HumanIK expects.
    """
    ...
def convertShoulder(tweakParameters, shoulderCount, shouldersPosition, leftShoulderPosition, rightShoulderPosition):
    """
    This method takes the position of the shoulder guides coming out of the
    embedding algorithm (shoulders and left/right shoulder) and maps them to
    clavicle joints that fits what HumanIK expects.
    """
    ...
def callback_step3_EnableXRayJoints(tool):
    """
    # This callback can work without any pre-condition.
    """
    ...
def degrees(*args, **kwargs):
    """
    degrees(x)
    
    Convert angle x from radians to degrees.
    """
    ...
def qruiEnableGuidesColor(tool, enabled): ...
def callback_scriptJob_deleteAll(tool): ...
def callback_step0_ChangeMode(tool): ...
def removeOutput_step5(input): ...
def callback_step2_DeleteGuides(*args, **kwargs): ...
def computeTStance(definition, positions, useTStanceCorrection):
    """
    This method computes the T-stance position for a given character, given
    the position of the guides.
    
    It leaves the joints that can be corrected using effectors in their current
    direction, but adjusts the bone length to match the guides.
    
    It returns a map of HumanIK / guide names to the T-stance position.
    """
    ...
def convertReference(minCorner, maxCorner):
    """
    This method takes the position of bounding box corners and maps them to
    a reference joint that HumanIK expects.
    """
    ...
def hikRenameDefinition(character):
    """
    This method opens the dialog allowing the user to rename the given HumanIK
    character.
    """
    ...
def getSelectedShapes(shapeTypes='None'):
    """
    This method returns the list of all currently selected shapes.
    
    It returns the actual shapes, i.e. the name of the shape nodes, not the
    transform nodes.
    """
    ...
def hikNoneString():
    """
    This method returns the string to display when no character is selected.
    """
    ...
def qrGetRequiredGuides(skeletonParameters):
    """
    This method returns a list of the guides corresponding to the given skeleton parameters.
    
    It should be in sync with the output of:
    - qrGetGuidesFromEmbedding( )
    """
    ...
def positionHikSkeleton(orderedJoints, skeletonNodes, positions, jointOrients):
    """
    This method sets the positions and joint orientations for a HumanIK skeleton.
    """
    ...
def hikCreateControlRig(character):
    """
    This method creates a control rig for the given HumanIK character.
    """
    ...
def qruiSetGuidesColor(tool, color): ...
def qrSetGuidesColor(guidesNode, color):
    """
    This method sets the color on all the guides.
    """
    ...
def qrRefreshMeshes(character):
    """
    This method makes sure meshes associated with the given character are
    stored properly in the character.
    
    It makes sure everything connected is actually a mesh, no mesh is
    duplicated and the mesh array is tightly packed.
    """
    ...
def qrInitialize():
    """
    This method makes sure all of the needed dependencies are loaded and
    initialized.
    
    This makes sure skinning shapes scripts are loaded, as well as HumanIK
    scripts.
    """
    ...
def qrAddMeshes(character, meshesToAdd):
    """
    This method adds the given meshes to the current character.
    """
    ...
def callback_AutoRig(tool): ...
def callbackUtil_step3_MirrorGuides(input, leftToRight): ...
def setDefaultOptionsGVB(resolution):
    """
    This method sets the necessary variables to have reasonable default
    parameters to perform geodesic voxel binding that gives good results on a
    wide majority of cases.
    """
    ...
def callback_step1_ClearMeshes(*args, **kwargs): ...
def tool_to_input(level, deleteOutput='True'):
    """
    This decorator takes a function which takes "callback input" and wraps it
    into a function which takes the tool as an input.
    
    Callback input is extracted from the character and the tool based on the
    given level required, each level corresponding to a step of the rigging
    process.  The decorator handles making sure that output of further steps is
    deleted if allowed by the user.
    """
    ...
def callback_step3_ShowAllGuides(*args, **kwargs): ...
def qrAverageGuides(guidesNode, center, axis):
    """
    This method averages guides with regards to the center.
    """
    ...
def callback_step3_HideAllGuides(*args, **kwargs): ...
def convertNeck(tweakParameters, neckCount, shouldersPosition, headPosition, boundingBox):
    """
    This method takes the position of the neck guides coming out of the
    embedding algorithm (shoulders and neck) and maps them to neck joints that
    fits what HumanIK expects.
    """
    ...
def hikDeleteWholeCharacter(character):
    """
    This method deletes the given HumanIK character.
    
    It deletes its control rig (if any), its skeleton (if any) and its
    character definition.
    """
    ...
def enableXRayJoints(enabled):
    """
    This method enables "X-Ray Joints" option on all viewports.
    """
    ...
def getInput_step3(input): ...
def qrGetDefaultTweakParameters():
    """
    This method returns the default tweak parameters for modifying the result
    of the skeletonEmbed command.
    
    At the moment, these are hard-coded, but eventually they should be
    configurable.
    """
    ...
def centerJoints(center, axis, joints):
    """
    This method centers a list of joints so they lie directly on the mirror plane.
    """
    ...
def OpenQuickRigUI():
    """
    This method is the entry point of the Quick Rig tool.
    
    It creates the Quick Rig tool window and brings it up.
    """
    ...
def hikGetEffectorNodeName(character, effectorName):
    """
    This method returns the effector node name in the given HumanIK character
    for the given generic HumanIK effector name.
    """
    ...
def callback_step3_MirrorGuidesRightToLeft(*args, **kwargs): ...
def callback_step2_CreateGuides(*args, **kwargs): ...
def callback_step1_AddSelectedMeshes(*args, **kwargs): ...
def getInput_step2(input): ...
def callback_step5_DeleteSkin(*args, **kwargs): ...


kOrientJointsOptions : list
kOptionsButtonWidth : int
kTweakFootToeRatio : float
kMeshesAttributeName : str
kDisabledGuidesColor : list
kTweakFootAnkleRatio : float
kColorSwatchHeight : int
kGuidesAttributeName : str
kFrameMarginWidth : int
kSkeletonAttributeName : str
kFrameParam : dict
kColorSwatchColorHeight : int
kFrameMarginHeight : int
kTweakShoulderRatio : float
kOptionsTextWidth : int
kTweakFoot : bool
kQuickRigInfoAttributeName : str
kCharacterUpAxis : int
kTweakFootUseCorrectedAnkleForToe : bool
kCharacterDownDirection : int
kTweakNeckFirstRatio : float
kSkinBindingOptions : list
kResolutions : list
kGuidesColor : list
kSymmetryOptions : list
kSkeletonFieldWidth : int
kTweakNeckLastRatio : float
kRowLayoutHeight : int

