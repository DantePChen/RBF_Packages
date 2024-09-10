from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.override as override
import maya.app.renderSetup.model.renderSetup as renderSetup
import os
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds


if False:
    from typing import Dict, List, Tuple, Union, Optional

class ColorConstants(object):
    def __init__(self): ...
    BLACK : tuple
    
    BLUE : tuple
    
    GREEN : tuple
    
    PURPLE : tuple
    
    RED : tuple
    
    WHITE : tuple
    
    YELLOW : tuple
    
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor




def setColor(attr, color): ...
def getShader(shapeName): ...
def assignMaterial(shapeName, shadingGroupName): ...
def testShapeColor(shapeName, color): ...
def createShadingGroup(color):
    """
    Create a shading group connected to a surface shader of the given color.
    Return the names of both the shading group and its associated shader.
    """
    ...
def findShadingEngine(connections): ...
def findComplementShadingGroup(shapeName): ...
def findShadingGroup(shapeName): ...
def createShader(shaderType):
    """
    Create a shader of the given type
    """
    ...
def findSurfaceShader(sgName): ...
def tmpSubDirName(dir, subDir):
    """
    Create a unique sub directory
    """
    ...
def createBasicRenderSetup():
    """
    Create a basic render setup
    """
    ...
def getShape(transform):
    """
    Return the shape of the argument transform.
    """
    ...

