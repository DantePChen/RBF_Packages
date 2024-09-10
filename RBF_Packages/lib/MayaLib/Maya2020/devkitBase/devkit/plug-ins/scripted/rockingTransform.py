########################################################################
# DESCRIPTION:
#
# Produces the custom transform node "spRockingTransformNode". 
#
# This plug-in implements an example custom transform that
# can be used to perform a rocking motion around the X axis.
# Geometry of any rotation can be made a child of this transform
# to demonstrate the effect.
# 
# The plug-in contains two pieces:
#
# 	1. The custom transform node -- rockingTransformNode
#	2. The custom transformation matrix -- rockingTransformMatrix
#
# These classes are used together in order to implement the
# rocking motion. Note that the rock attribute is stored outside
# of the regular transform attributes.
#
# To use, execute the following commands: 
#
#	import maya.cmds
#	maya.cmds.loadPlugin("rockingTransform.py")
#	maya.cmds.file(f=True,new=True)
#	maya.cmds.polyPlane()
#	maya.cmds.select("pPlane1",r=True)
#	maya.cmds.rotate(-15,-15,-15,r=True,ws=True)
#	maya.cmds.createNode("spRockingTransform")
#	maya.cmds.parent("pPlane1","spRockingTransform1")
#	maya.cmds.setAttr("spRockingTransform1.rockx",55)
#
########################################################################

import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import math
import sys

kRockingTransformPluginName = "spRockingTransform"
kRockingTransformNodeName = "spRockingTransformNode"
kRockingTransformNodeID = OpenMaya.MTypeId(0x87014)
kRockingTransformMatrixID = OpenMaya.MTypeId(0x87015)

# keep track of instances of rockingTransformMatrix to get around script limitation
# with proxy classes of base pointers that actually point to derived
# classes
kTrackingDictionary = {}


class rockingTransformMatrix(OpenMayaMPx.MPxTransformationMatrix):

    def __init__(self):
        OpenMayaMPx.MPxTransformationMatrix.__init__(self)
        kTrackingDictionary[OpenMayaMPx.asHashable(self)] = self
        self.rockXValue = 0.0

    def __del__(self):
        del kTrackingDictionary[OpenMayaMPx.asHashable(self)]

    def setRockInX(self,rockingValue):
        self.rockXValue = rockingValue

    def getRockInX(self):
        return self.rockXValue

    def asMatrix(self,percent=None):
        """
        Find the new matrix and return it
        """
        if percent == None:
            matrix = OpenMayaMPx.MPxTransformationMatrix.asMatrix(self)
            tm = OpenMaya.MTransformationMatrix(matrix)
            quat = self.rotation()
            rockingValue = self.getRockInX()
            newTheta = math.radians( rockingValue )
            quat.setToXAxis( newTheta )
            tm.addRotationQuaternion( quat.x, quat.y, quat.z, quat.w, OpenMaya.MSpace.kTransform )
            return tm.asMatrix()
        else:
            m = OpenMayaMPx.MPxTransformationMatrix(self)
            #
            trans = m.translation()
            rotatePivotTrans = m.rotatePivot()
            scalePivotTrans = m.scalePivotTranslation()
            trans = trans * percent
            rotatePivotTrans = rotatePivotTrans * percent
            scalePivotTrans = scalePivotTrans * percent
            m.translateTo(trans)
            m.setRotatePivot( rotatePivotTrans )
            m.setScalePivotTranslation( scalePivotTrans )
            #
            quat = self.rotation()
            rockingValue = self.getRockInX()
            newTheta = math.radians( rockingValue )

            quat.setToXAxis( newTheta )
            m.rotateBy( quat )
            eulerRotate = m.eulerRotation()
            m.rotateTo( eulerRotate * percent, OpenMaya.MSpace.kTransform )
            #
            s = self.scale( OpenMaya.MSpace.kTransform )
            s.x = 1.0 + ( s.x - 1.0 ) * percent
            s.y = 1.0 + ( s.y - 1.0 ) * percent
            s.z = 1.0 + ( s.z - 1.0 ) * percent
            m.scaleTo( s, OpenMaya.MSpace.kTransform )
            #
            return m.asMatrix()


class rockingTransformNode(OpenMayaMPx.MPxTransform):
    aRockInX = OpenMaya.MObject()
    aCachedRock = OpenMaya.MObject()

    def __init__(self, transform=None):
        if transform is None:
            OpenMayaMPx.MPxTransform.__init__(self)
        else:
            OpenMayaMPx.MPxTransform.__init__(self, transform)
        self.rockXValue = 0.0

    def createTransformationMatrix(self):
        return OpenMayaMPx.asMPxPtr( rockingTransformMatrix() )

    def className(self):
        return kRockingTransformNodeName

    #----------------------------------------------------------------------
    def computeLocalTransformation(self, mtx, data):

        rockInXData = data.inputValue( self.aRockInX )
        rockInX = rockInXData.asDouble()

        try:
            # mtx is of type 'MPxTransformationMatrix'. Use kTrackingDictionary
            # to retrieve the actual assocaited type 'rockingTransformMatrix'
            ltm = kTrackingDictionary[OpenMayaMPx.asHashable(mtx)]

            # Update the custom transformation matrix to the right rock value.
            ltm.setRockInX(rockInX)

        except Exception, ex:
            print 'rockingTransform computeLocalTransformation error: %s' % str(ex)

        return OpenMayaMPx.MPxTransform.computeLocalTransformation(self, mtx, data)

    #----------------------------------------------------------------------
    def compute(self, plug, data):

        if plug.isNull():
            return OpenMayaMPx.MPxTransform.compute(self, plug, data)

        if ( (plug == self.matrix)
        or   (plug == self.inverseMatrix)
        or   (plug == self.worldMatrix)
        or   (plug == self.worldInverseMatrix)
        or   (plug == self.parentMatrix)
        or   (plug == self.parentInverseMatrix) ):

            try:
                ltm = self.getRockingTransformationMatrix()
                if ltm is not None:
                    self.computeLocalTransformation(ltm, data)

            except Exception, ex:
                print 'rockingTransform Compute error: %s' % str(ex)

        return OpenMayaMPx.MPxTransform.compute(self, plug, data)

    #----------------------------------------------------------------------
    def validateAndSetValue(self, plug, handle):
        if not plug.isNull():
            if plug == self.aRockInX:
                block = self._forceCache()
                rock_handle = block.outputValue(plug)

                # Update our new rock in x value
                rockInX = handle.asDouble()
                rock_handle.setDouble(rockInX)
           
                # Update the custom transformation matrix to the
                # right value.
                ltm = self.getRockingTransformationMatrix()
                if ltm is not None:
                    ltm.setRockInX(rockInX)

                rock_handle.setClean()

                # Mark the matrix as dirty so that DG information
                # will update.
                self._dirtyMatrix()

        OpenMayaMPx.MPxTransform.validateAndSetValue(self, plug, handle)

    #----------------------------------------------------------------------
    def getRockingTransformationMatrix(self):
        baseXform = self.transformationMatrixPtr()
        return kTrackingDictionary[OpenMayaMPx.asHashable(baseXform)]


# create/initialize node and matrix
def matrixCreator():
    return OpenMayaMPx.asMPxPtr( rockingTransformMatrix() )

def nodeCreator():
    return OpenMayaMPx.asMPxPtr( rockingTransformNode() )

def nodeInitializer():
    numFn = OpenMaya.MFnNumericAttribute()

    rockingTransformNode.aRockInX = numFn.create("RockInX", "rockx", OpenMaya.MFnNumericData.kDouble, 0.0)

    numFn.setKeyable(True)
    numFn.setAffectsWorldSpace(True)

    rockingTransformNode.addAttribute(rockingTransformNode.aRockInX)
    rockingTransformNode.mustCallValidateAndSet(rockingTransformNode.aRockInX)
    return

# initialize the script plug-in
def initializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)

    try:
        mplugin.registerTransform( kRockingTransformPluginName, kRockingTransformNodeID, \
                                   nodeCreator, nodeInitializer, matrixCreator, kRockingTransformMatrixID )
    except:
        sys.stderr.write( "Failed to register transform: %s\n" % kRockingTransformPluginName )
        raise

# uninitialize the script plug-in
def uninitializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)

    try:
        mplugin.deregisterNode( kRockingTransformNodeID )
    except:
        sys.stderr.write( "Failed to unregister node: %s\n" % kRockingTransformPluginName )
        raise

# =====================================================================
# Copyright 2017 Autodesk, Inc.  All rights reserved.
#
# This computer source code  and related  instructions and comments are
# the unpublished confidential and proprietary information of Autodesk,
# Inc. and are  protected  under applicable  copyright and trade secret
# law. They may not  be disclosed to, copied or used by any third party
# without the prior written consent of Autodesk, Inc.
# =====================================================================
