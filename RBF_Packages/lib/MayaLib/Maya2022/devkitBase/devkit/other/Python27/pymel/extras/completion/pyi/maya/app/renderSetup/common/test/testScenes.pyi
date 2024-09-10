from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.app.renderSetup.model.renderSetup as renderSetup
import maya.mel as mel
import maya.cmds as cmds
import maya.app.renderSetup.model.override as override
import maya.app.renderSetup.model.connectionOverride as connectionOverride
import maya.app.renderSetup.model.selector as selector


from maya.app.renderSetup.common.test.sceneUtils import assignMaterial
from maya.app.renderSetup.common.test.sceneUtils import testShapeColor
from maya.app.renderSetup.common.test.sceneUtils import setColor
from maya.app.renderSetup.common.test.sceneUtils import ColorConstants
from maya.app.renderSetup.common.test.sceneUtils import createShader
from collections import namedtuple


if False:
    from typing import Dict, List, Tuple, Union, Optional

Instance = namedtuple('Instance', ('transform', 'shape', 'generator', 'materials'))


Material = namedtuple('Material', ('shadingEngine', 'surfaceShader', 'displacementShader', 'volumeShader'))




def createRenderSetupScene():
    """
    Create a trivial scene with render setup.
    Return the render layer.
    """
    ...
def createSurfaceShader(color):
    """
    Creates a Surface Shader with the specified color.
    Returns a tuple (shaderName, shadingEngineName)
    """
    ...
def _createSceneWithMaterialShaderOverrides(typeId, setSource): ...
def createSceneWithMaterials():
    """
    Create a test scene composed of
    - 2 polySphere (instances of the same shape)
    - 2 nurbsSphere (instances of the same shape)
    First instance has whole shape material assignment.
    Second instance has per-face shape material assignment.
    - 1 polyCube without any per-face
    - 1 directionalLight
    
    Returns a set of Instance (named tuples) containing these 6 objects.
    
    To see it in maya, just run python script:
    import maya.app.renderSetup.common.test.testScenes as scenes; scenes.createSceneWithMaterials()
    """
    ...
def createSphereWithAllShaders():
    """
    Creates a poly sphere (pSphere1) with the following material.
     
    place2dTexture1 ----> noise1 ----> bump1 ----> lambert1 ----\ 
                                        displacementShader1 ----- initialShadingGroup
                                                 volumeFog1 ----/
    You can see it with: 
    import maya.app.renderSetup.common.test.testScenes as scenes; scenes.createSphereWithAllShaders()
    """
    ...
def createAllShaders():
    """
    place2dTexture# ----> noise# ----> bump# ----> blinn# ----\ 
                                      displacementShader# ----- shadingGroup#
                                               volumeFog# ----/
    """
    ...
def assignShadingEngine(shape, shadingEngine, components='None'):
    """
    Assign shading engine to shape.
    "components" is an optional list of mesh face indices or nurbs surface patch indices
    that can be used to specify per-face shading engine assignment.
    A mesh index is given by an integer >= 0.
    A surface patch is given by a tuple (span, section) where span and section are integers >= 0.
    """
    ...
def createSceneWithMaterialOverrides(): ...
def createSceneWithShaderOverrides(): ...


DefaultMaterial : Material

