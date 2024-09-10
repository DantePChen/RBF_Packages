from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import xml
import maya.OpenMaya as OpenMaya
import maya
import string
import maya.cmds as cmds
import sys
import xml.etree.ElementTree as ET
import os
import math


if False:
    from typing import Dict, List, Tuple, Union, Optional

class RetargUtils:
    @staticmethod
    def addAttrConnection(attr1, attr2):
        """
        Create a utility node that adds the two attributes and return the output attribute
        """
        ...
    @staticmethod
    def addAttrFunc(attr1, val):
        """
        Create a utility node that adds an attribute with the specified values and returns the output attribute
        """
        ...
    @staticmethod
    def addVecConnection(attr1, attr2):
        """
        Create a utility node that adds the two vector attributes and returns the output vector attribute
        """
        ...
    @staticmethod
    def addVecFunc(attr1, val):
        """
        Create a utility node that adds two vector attributes and returns the output vector attribute
        """
        ...
    @staticmethod
    def addVecFuncGetNode(attr1, val):
        """
        Create a utility node that adds the two vector attributes and returns the output node attribute
        """
        ...
    @staticmethod
    def connectPairBlendCompound(src, dest, attr):
        """
        Disconnect individual x,y,z attribute channels between a src pairBlend node 
        and a user specified destination node, hooking up the attribute's compound instead.
        """
        ...
    @staticmethod
    def convertRad2Deg(rot): ...
    @staticmethod
    def decomposeRelative(sourceMat, sourceRootMat, newRootMat):
        """
        Create a set of utility nodes that returns the source mat translation relative to the the new root
        """
        ...
    @staticmethod
    def disconnectIfConnected(src, dest): ...
    @staticmethod
    def disconnectPairBlendCompound(src, dest, attr):
        """
        Disconnect the translation or rotation attribute compound instead between 
        a src pairBlend node and a user specified destination node, hooking up 
        individual x,y,z attribute channels instead.
        """
        ...
    @staticmethod
    def eulerToQuat(euler, rotateOrder='None'):
        """
        Create a eulerToQuat conversion node, and connect an euler triple plug to its
        inputRotate attribute. If a rotateOrder plug name is specified, hook this 
        up to the inputRotateOrder attr.
        On exit, return the name of the newly created node.
        """
        ...
    @staticmethod
    def eulerValToComposeMat(euler, rotateOrder): ...
    @staticmethod
    def getAttr(plug): ...
    @staticmethod
    def getJointOrient(nodeName):
        """
        Return the joint orient value for the specified node
        """
        ...
    @staticmethod
    def getMatrix(node, matrixString):
        """
        Get the maya matrix from a specified node and the name of a string
        """
        ...
    @staticmethod
    def getWorldPositionUsingRP(nodeName): ...
    @staticmethod
    def hasAnimCurves(node):
        """
        Determine if the named node has translation or rotation attributes driven by animation curves
        """
        ...
    @staticmethod
    def hasPairBlend(node):
        """
        Determine if the named node has a pairBlend node connected to the translation or rotation attributes
        """
        ...
    @staticmethod
    def hookUpDecomposeToMatrix(matrixSource):
        """
        Create a decompose utility node that hooks up to the source and return it.
        """
        ...
    @staticmethod
    def inverseQuat(quat): ...
    @staticmethod
    def isAnimCurve(object): ...
    @staticmethod
    def isLocked(node, attr): ...
    @staticmethod
    def listAnimCurves(node, attrs):
        """
        Determine which of the named attributes in attrs has an animation curve
        """
        ...
    @staticmethod
    def loadPlugin(name):
        """
        Load the named plugin if it isn't already loaded.
        """
        ...
    @staticmethod
    def matrixToList(matrixVal): ...
    @staticmethod
    def matrixToQuat(matrixAttr):
        """
        Create a utility node that converts a matrix rot to a quat
        """
        ...
    @staticmethod
    def multConstMat(mVal, mVal2):
        """
        Create a utiliyt node that multiplis the two matrix values and returns the output attribute
        """
        ...
    @staticmethod
    def multMatrix(matrixAttr, matrixAttr1):
        """
        Create a matrix that multiplies the two attributes and returns the output attribute
        """
        ...
    @staticmethod
    def multMatrixByConstMat(matrixVal, pre, matrixAttr):
        """
        Create a utility node that multiplies the specified matrix attribute by the matrix val.
        if pre is true that tne value is first, if false it's multiplied after the attribute
        """
        ...
    @staticmethod
    def multMatrixByConstMatNode(matrixVal, pre, matrixAttr):
        """
        Create a utility node that multiplies the specified matrix attribute by the matrix val.
        if pre is true that tne value is first, if false it's multiplied after the attribute
        """
        ...
    @staticmethod
    def multPointByMatrix(matrixAttr, pointAttr):
        """
        Create a utility node that multiplies the point by the matrix
        """
        ...
    @staticmethod
    def multQuatConnection(quat1, quat2): ...
    @staticmethod
    def nameToNode(name):
        """
        Convert the string name to a maya node
        """
        ...
    @staticmethod
    def parkAnimCurves(node):
        """
        Place animation curves connected to a node on a pairblend. Mute the channels
        corresponding to the animation curves.
        """
        ...
    @staticmethod
    def quatToEuler(quat, rotateOrder): ...
    @staticmethod
    def quatToMatrix(quatAttr):
        """
        Create a utility node that converts a quat to a matrix
        """
        ...
    @staticmethod
    def removeAllConnections(nodes): ...
    @staticmethod
    def setAttr(plug, val): ...
    @staticmethod
    def setAttrIfNotNone(attr, val): ...
    @staticmethod
    def subVecConnection(attr1, attr2):
        """
        Create a utility node that subtracts the two vector attributes and returns the output vector attribute
        """
        ...
    @staticmethod
    def subVecFunc(attr1, val):
        """
        Create a utility node that subtracts the value from the atttribute and returns the output vector attribute
        """
        ...
    @staticmethod
    def unparkAnimCurves(node):
        """
        Move parked animation curves off a (muted) pairBlend node and back onto
        appropriate node attributes.
        """
        ...
    @staticmethod
    def walkGraph(startAttr, endAttr):
        """
        Build a list of nodes between node and the first HIKState2GlobalSK found in the graph
        """
        ...


class HIKRetargeter:
    """
    TODO: Add python doc description of HIKRetargeter
    """
    
    
    
    def __del__(self): ...
    def __init__(self, HIKDestination='None'):
        """
        # Public interface
        """
        ...
    def connect(self): ...
    def destinationAttrs(self): ...
    def disconnect(self): ...
    def fromGraph(self, node):
        """
        Deserialize object from scene graph
        """
        ...
    def fromXML(self, root, dest, file='None'):
        """
        Initialize the object from an XML description.
        """
        ...
    def fromstring(self, text, dest='None'):
        """
        Deserialize the object from a XML string
        """
        ...
    def getDestination(self): ...
    def getMapping(self, bodyPart, type, linkNum='0'): ...
    def getMappingIds(self): ...
    def getMappingKey(self, bodyPart, type, linkNum='0'): ...
    def getSource(self): ...
    def isConnected(self): ...
    def read(self, file, dest):
        """
        Deserialize the object from a user specified XML file
        """
        ...
    def removeMapping(self, bodyPart, type, linkNum='0'): ...
    def setDestination(self, destination): ...
    def setMapping(self, bodyPart, destRig, type, retargeter, id, linkNum='0'): ...
    def toGraph(self):
        """
        Serialize object to scene graph
        """
        ...
    def toXML(self):
        """
        Build an XML description of the object
        """
        ...
    def tostring(self, indent='False'):
        """
        Serialize the object to a XML string
        """
        ...
    def write(self, file):
        """
        Serialize the object to a user specified XML file
        """
        ...
    @staticmethod
    def createDefaultMapping(source, dest, bodyPart, destRig, type, id, linkNum='0'): ...


class DefaultRetargeter:
    """
    The DefaultRetargeter creates a network of utility node to retarget motion from a source matrix to a control rig.
    
    Keyword Arguments:
    
    Sample Usage:
    
    Create a retargeter object and call one of the retargetting functions on it. For example,
    
      retargeter = DefaultRetargeter(), 
      retargeter.SetUpRot(...)
    """
    
    
    
    def __init__(self, matSource='None', destRig='None', destSkel='None', type='None', id='-1', body='None'): ...
    def calculateOffsets(self, destination, destinationSkel, type):
        """
        Get the offset matrix or translate between the source and the destination skeletons
        """
        ...
    def delRetargeter(self, src):
        """
        Delete the network of retargetting nodes between the source and destination nodes.
        """
        ...
    def destinationAttrs(self): ...
    def doRetarget(self, src): ...
    def fromDictionary(self, vals):
        """
        Deserialize object from python dictionary
        """
        ...
    def fromGraph(self, node):
        """
        Deserialize the object from a scene graph node
        """
        ...
    def getDestinationRig(self): ...
    def getId(self): ...
    def getOffset(self): ...
    def getType(self): ...
    def setOffset(self, x, y, z): ...
    def setUpRot(self, matrixSource, destination, destinationSkel='None', offset='None', destinationSkelParent='None'):
        """
        Set up a utility node network that drives the destination rotation by the matrixSource attribute.
        
        Keyword arguments:
        
        matrixSource          - worldMatrix of some node.
        destination           - control item that we want to rotate that will drive the destinationSkel item
        destinationSkel       - bone or joint that the the destination will drive on the rig
        destinationSkelParent - currently experimental, it is used to (not tested for a while) specify
                                a different parent that the skeleton should rotate against, for example 
                                for use with a neck joint that rotates relative to COM and not the spine
        """
        ...
    def setUpTrans(self, matrixSource, destination, destSkel='None', offset='None', sourceRoot='None', destRoot='None'):
        """
        setUpTrans creates a set of utility nodes that drive the translation of the destination by the matrixSource attribute.
        
        Keyword arguments:
        
        matrixSource - The worldMatrix of some node (?)
        destination  - The control item we want to move to the translation of the specified matrixSource
        destSkel     - The skeleton of the destination element default = None.
                       If destSkel is not None, this argument is used to compute the translation offset between
                       the destination and destSkel. This offset is then used to determine how to position the 
                       destination.
        sourceRoot   - ? default = None
        destRoot     - ? default = None
        
        # TODO: This stuff should make its way into the comments
        #if sourceRoot and destRoot are specified, the sourceRoot should be the root of the matrixSource
        # and the destRoot should be the root of the destination.
        # This let's us match up translations with different origins.
        #Current the sourceRoot and destRoot should have the same orientation but this may change.
        
        TODO: Do we need to use the rotatepivot also here for the source dest root stuff?
        """
        ...
    def toDictionary(self):
        """
        Serialize object to python dictionary
        """
        ...
    def toGraph(self, src):
        """
        Serialize object to a scene graph node
        """
        ...


class MappedRetargeter:
    def __init__(self): ...
    def fromDictionary(self, vals):
        """
        Deserialize the class from a python dictionary
        """
        ...
    def getRetargeter(self): ...
    def setRetargeter(self, retargeter): ...
    def toDictionary(self):
        """
        Serialize the class to a python dictionary
        """
        ...


class PivotRetargeter(DefaultRetargeter):
    pass



