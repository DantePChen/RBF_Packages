from __future__ import division
#-
# ==========================================================================
# Copyright (C) 1995 - 2006 Autodesk, Inc. and/or its licensors.  All 
# rights reserved.
#
# The coded instructions, statements, computer programs, and/or related 
# material (collectively the "Data") in these files contain unpublished 
# information proprietary to Autodesk, Inc. ("Autodesk") and/or its 
# licensors, which is protected by U.S. and Canadian federal copyright 
# law and by international treaties.
#
# The Data is provided for use exclusively by You. You have the right 
# to use, modify, and incorporate this Data into other products for 
# purposes authorized by the Autodesk software license agreement, 
# without fee.
#
# The copyright notices in the Software and this entire statement, 
# including the above license grant, this restriction and the 
# following disclaimer, must be included in all copies of the 
# Software, in whole or in part, and all derivative works of 
# the Software, unless such copies or derivative works are solely 
# in the form of machine-executable object code generated by a 
# source language processor.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND. 
# AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED 
# WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF 
# NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
# PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE, OR 
# TRADE PRACTICE. IN NO EVENT WILL AUTODESK AND/OR ITS LICENSORS 
# BE LIABLE FOR ANY LOST REVENUES, DATA, OR PROFITS, OR SPECIAL, 
# DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK 
# AND/OR ITS LICENSORS HAS BEEN ADVISED OF THE POSSIBILITY 
# OR PROBABILITY OF SUCH DAMAGES.
#
# ==========================================================================
#+

########################################################################
# DESCRIPTION:
#
# Produces the dependency graph node "spCustomImagePlane".  
#
# Demonstrates how to create your own custom image plane based on
# Maya's internal image plane classes. This allows API users to
# override the default Maya image plane behavior. This class works
# like typical API nodes in that it can have a compute method and
# can contain static attributes added by the API user. This
# example class overrides the default image plane behavior and
# allows users to add transparency to an image plane using the
# transparency attribute on the node. Note that this code also 
# illustrates how to use MImage to control the floating point
# depth buffer. When useDepthMap is set to true, depth is added
# to the image such that half of the image is at the near 
# clip plane and the remaining half is at the far clip plane. 
#
# Once the image plane node has been created, you must
# attach it to the camera shape that is displaying the node.
# Image planes are attached to cameras using the imagePlane message
# attribute on the camera shape. To attach this example image
# plane, you must connect the image plane's message attribute to
# the cameraShapes imagePlane attribute.  Note that the imagePlane
# attribute is a multi attribute and can hold references to
# multiple image planes.
#
# This example works only with renderers that use node evaluation
# as a part of the rendering process like Maya. It does 
# not work with renderers that rely on a scene translation mechanism like
# mental ray.
#
# To use, run the following:
#
#	import maya
#	maya.cmds.loadPlugin("customImagePlane.py")
#	imageP = maya.cmds.createNode("spCustomImagePlane")
#	maya.cmds.connectAttr( imageP + ".message", "perspShape.imagePlane[0]", force = True )
#
# Then, assign an image to the customImagePlane node.
#
########################################################################

from builtins import range
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import sys

kPluginNodeTypeName = "spCustomImagePlane"
kPluginNodeId = OpenMaya.MTypeId(0x0008005c)

# Node definition
class customImagePlane(OpenMayaMPx.MPxImagePlane):
	# class variables
	aTransparency = OpenMaya.MObject()
	
	# internal variables
	fTransparency = 0.0
			
	def __init__(self):
		OpenMayaMPx.MPxImagePlane.__init__(self)
		
	def getInternalValue(self, plug, handle ):
		if plug == self.aTransparency:
			handle.setDouble( self.fTransparency )
			return True
		
		return OpenMayaMPx.MPxImagePlane.getInternalValue(self, plug, handle )
	
	def setInternalValue(self, plug, handle ):
		if plug == self.aTransparency:
			self.fTransparency = handle.asDouble()
			self.setImageDirty()
			return True
		
		return OpenMayaMPx.MPxImagePlane.setInternalValue(self, plug, handle )	
		
	def loadImageMap(self, fileName, frame, image):
		try:	
			image.readFromFile(fileName)
			
			# get the width and height of the image an MScriptUtil is needed
			# to pass in a pointer to the MImage::getSize() method
			widthUtil = OpenMaya.MScriptUtil(0)
			widthPtr = widthUtil.asUintPtr()
			heightUtil = OpenMaya.MScriptUtil(0)
			heightPtr = heightUtil.asUintPtr()
			image.getSize( widthPtr, heightPtr )
			
			width = widthUtil.getUint(widthPtr)
			height = heightUtil.getUint(heightPtr)		
			size = width * height
			
			# Implement transparency
			charPixelPtr = image.pixels()
			for i in range( 0, size, 4 ):
				alphaIndex = (i*4)+3
				alpha = OpenMayaScript.getCharArrayItem(charPixelPtr,alphaIndex)
				OpenMayaScript.setCharArray( alpha * (1.0 - self.fTransparency), alphaIndex )
			
			# Implement use depth map
			thisNode = self.thisMObject()
			fnThisNode = OpenMaya.MFnDependencyNode(thisNode)
			useDepthMap = fnThisNode.attribute("useDepthMap")
			depthMap = OpenMaya.MPlug( thisNode, useDepthMap )
			value = depthMap.asBool()
			
			if value:				
				buffer = []			
				c = 0
				for i in range( 0, height ):
					for j in range( 0, width ):
						if i > height/2.0:
							buffer.insert(c, -1.0)
						else:
							buffer.insert(c, 0.0)
						c+=1
						
				depthMapArray = OpenMaya.MScriptUtil()
				depthMapArray.createFromList( buffer )
				depthMapArrayFloatPtr = depthMapArray.asFloatPtr()
				image.setDepthMap( depthMapArrayFloatPtr, width, height )
				
		except:
			pass
			
# creator
def nodeCreator():
	return OpenMayaMPx.asMPxPtr( customImagePlane() )
	
# initializer
def nodeInitializer():
	nAttr = OpenMaya.MFnNumericAttribute()

	# Setup the input attributes
	customImagePlane.aTransparency = nAttr.create("transparency", "tp", OpenMaya.MFnNumericData.kDouble, 0.0)
	nAttr.setStorable(True)
	nAttr.setInternal(True)
	nAttr.setMin(0.0)
	nAttr.setMax(1.0)
	nAttr.setKeyable(True)
	
	customImagePlane.addAttribute(customImagePlane.aTransparency)
	
# initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject, "Autodesk", "1.0", "Any")
	try:
		mplugin.registerNode( kPluginNodeTypeName, kPluginNodeId, nodeCreator, nodeInitializer, OpenMayaMPx.MPxNode.kImagePlaneNode )
	except:
		sys.stderr.write( "Failed to register node: %s" % kPluginNodeTypeName )
		raise


# uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.deregisterNode( kPluginNodeId )
	except:
		sys.stderr.write( "Failed to register node: %s" % kPluginNodeTypeName )
		raise
