from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya
import sys
import traceback
import maya
import maya.cmds as cmds


"""
MayaDynamicsIntegration - integration with the user interface for Maya's 
                          built-in Dynamics such as supporting Maya's fields
"""


from collections import defaultdict


if False:
    from typing import Dict, List, Tuple, Union, Optional

def collectBulletObjects(objects):
    """
    Take the list of objects and find all Bullet shapes associated with them.
    
    In this case we are interested in shapes that can be affected by fields.
    """
    ...
def solverForShape(shape):
    """
    Get the solver for the given bullet shape
    """
    ...
def deleteConnections(bullet_shapes, fields):
    """
    Delete all of the connections between the given fields and bullet shapes
    """
    ...
def addDynamicConnectHook(): ...
def removeDynamicConnectHook(): ...
def disconnectFieldFromShape(field, shape):
    """
    Disconnect the given field from the given bullet shape
    """
    ...
def makeConnections(bullet_shapes, fields):
    """
    Make all of the required connections
    """
    ...
def connectDynamicCB(fields, emitters, collisionObjects, objects, delete):
    """
    This is the callback that gets called when the 'connectDynamic' command
    is called. This callback looks for bullet shapes in the objects and 
    takes over if it finds some.
    """
    ...
def connectFieldToShape(field, shape):
    """
    Connect the given field to the given bullet shape
    """
    ...
def findExisitingConnections(objects):
    """
    In the case where we connect a field to a solver, we need to check if that
    field is already connected to any objects being handled by that solver. So, if
    we need to break those direct connections to prevent the field from affecting
    the same object twice. So, here we iterate over any solver objects and 
    build up a list of the objects they are solving and the fields that they are
    connected to.
    
    This function returns a tuple of:
    - solvers->fields->shapes the fields are connected to
    - solvers->connected fields
    
    We build these lists because, for large operations, it is more efficient
    """
    ...


strVersion : unicode
aVersion : list
kBulletSoftBodyShapeType : str
kBulletSolverShapeType : str
connectDynamicCB_ID : NoneType
gMayaVersion : float
kBulletRigidBodyShapeType : str

