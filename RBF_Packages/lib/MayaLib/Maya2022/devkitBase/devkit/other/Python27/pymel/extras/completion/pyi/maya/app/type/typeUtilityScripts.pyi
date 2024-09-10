from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.mel as mel
import maya.OpenMaya as om
import maya.cmds as cmds


from sets import Set


if False:
    from typing import Dict, List, Tuple, Union, Optional

def resetAllManipulations(nodeName):
    """
    #resets all manipulations.
    """
    ...
def setCurvePreset(attributeName, valueString): ...
def getShaderFromArray(GrpMessageConections):
    """
    #given a list of nodes, find the shading engine, and it's material
    """
    ...
def flipTypeManipulator(): ...
def toggleTypeManipButton(typeNode):
    """
    #this function is used to toggle the colour of the manipulator button (dark background when the manip is selected)
    """
    ...
def resetTypeCurve(attributeName, curveName): ...
def getMObjectFromName(nodeName): ...
def setCurveAttr(myRamp, pos, val): ...
def getShaderFromObject(mesh):
    """
    #get the shader attribute attached to an object
    #this and the next function are VERY similar, but they go about their task in different ways - which only work in different situations.
    """
    ...
def getVectorShadingGroups(mesh, extrudeNode):
    """
    #given the group nodes, get the associated materials
    """
    ...
def particlesToTypePivotPoints(): ...
def getFalloffCurveAttr(thisNode, attr):
    """
    # curve utilities
    """
    ...
def getShadingGroupsFromObject(mesh):
    """
    #get the shaders attached to an object
    """
    ...
def getCurrentCtxName(): ...
def splitTypeMaterials(extrudeNode, meshShape, typeNode):
    """
    #assign materials to the type tool
    """
    ...
def joinTypeMaterials(meshShape, typeNode): ...
def setShellAnimateKeys(typeNode):
    """
    #sets keys for TRS on the animaiton attributes
    """
    ...
def checkTypeManipButton():
    """
    #check if the manipulator is loaded and make sure the manip button background is correct (run from AE replacement)
    """
    ...

