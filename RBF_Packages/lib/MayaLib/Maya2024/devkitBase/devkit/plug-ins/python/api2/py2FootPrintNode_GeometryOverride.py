#-
# ===========================================================================
# Copyright 2020 Autodesk, Inc.  All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
#+

########################################################################
# DESCRIPTION:
#
# This plug-in demonstrates how to draw a simple mesh like foot Print in an efficient way.
#
# This efficient way is supported in Viewport 2.0.
#
# For comparison, you can also reference a devkit sample named footPrintNode. (See: pyFootPrintNode.py)
# In that sample, we draw the footPrint using the MUIDrawManager primitives in footPrintDrawOverride::addUIDrawables()
#
# For comparison, you can also reference another devkit sample named rawfootPrintNode.
# In that sample, we draw the footPrint with OpenGL\DX in method rawFootPrintDrawOverride::draw().
#
# Note that the method: 
#   footPrint.draw() 
# is only called in legacy default viewport to draw the foot print.
# while the methods:
#   footPrintGeometryOverride.updateDG() 
#   footPrintGeometryOverride.updateRenderItems() 
#   footPrintGeometryOverride.populateGeometry()
# are only called in Viewport 2.0 to prepare and draw the foot print.
#
# Example usage is to load the plug-in and create the node using
# the createNode command:
# 
# import maya.cmds as cmds
# cmds.loadPlugin('footPrintNode_GeometryOverride')
# cmds.createNode('footPrint_GeometryOverride')
#
#########################################################################
from builtins import range
import sys
import ctypes
import maya.api.OpenMaya as om
import maya.api.OpenMayaUI as omui
import maya.api.OpenMayaAnim as oma
import maya.api.OpenMayaRender as omr
import maya.mel as mel

def maya_useNewAPI():
	"""
	The presence of this function tells Maya that the plugin produces, and
	expects to be passed, objects created using the Maya Python API 2.0.
	"""
	pass

def matrixAsArray(matrix):
	array = []
	for i in range(16):
		array.append(matrix[i])
	return array

## Foot Data
##
sole = [ [  0.00, 0.0, -0.70 ],
				 [  0.04, 0.0, -0.69 ],
				 [  0.09, 0.0, -0.65 ],
				 [  0.13, 0.0, -0.61 ],
				 [  0.16, 0.0, -0.54 ],
				 [  0.17, 0.0, -0.46 ],
				 [  0.17, 0.0, -0.35 ],
				 [  0.16, 0.0, -0.25 ],
				 [  0.15, 0.0, -0.14 ],
				 [  0.13, 0.0,  0.00 ],
				 [  0.00, 0.0,  0.00 ],
				 [ -0.13, 0.0,  0.00 ],
				 [ -0.15, 0.0, -0.14 ],
				 [ -0.16, 0.0, -0.25 ],
				 [ -0.17, 0.0, -0.35 ],
				 [ -0.17, 0.0, -0.46 ],
				 [ -0.16, 0.0, -0.54 ],
				 [ -0.13, 0.0, -0.61 ],
				 [ -0.09, 0.0, -0.65 ],
				 [ -0.04, 0.0, -0.69 ],
				 [ -0.00, 0.0, -0.70 ] ]
heel = [ [  0.00, 0.0,  0.06 ],
				 [  0.13, 0.0,  0.06 ],
				 [  0.14, 0.0,  0.15 ],
				 [  0.14, 0.0,  0.21 ],
				 [  0.13, 0.0,  0.25 ],
				 [  0.11, 0.0,  0.28 ],
				 [  0.09, 0.0,  0.29 ],
				 [  0.04, 0.0,  0.30 ],
				 [  0.00, 0.0,  0.30 ],
				 [ -0.04, 0.0,  0.30 ],
				 [ -0.09, 0.0,  0.29 ],
				 [ -0.11, 0.0,  0.28 ],
				 [ -0.13, 0.0,  0.25 ],
				 [ -0.14, 0.0,  0.21 ],
				 [ -0.14, 0.0,  0.15 ],
				 [ -0.13, 0.0,  0.06 ],
				 [ -0.00, 0.0,  0.06 ] ]
soleCount = 21
heelCount = 17

## Maintain a mini cache for 3d solid shaders in order to reuse the shader
## instance whenever possible. This can allow Viewport 2.0 optimization e.g.
## the GPU instancing system and the consolidation system to be leveraged.
##
the3dSolidShaders = {}

def get3dSolidShader(color):
	# Return the shader instance if exists.
	key = str(color)
	if key in the3dSolidShaders:
		return the3dSolidShaders[key]

	shader = None

	shaderMgr = omr.MRenderer.getShaderManager()
	if shaderMgr:
		shader = shaderMgr.getStockShader(omr.MShaderManager.k3dSolidShader)

	if shader:
		shader.setParameter(footPrintGeometryOverride.colorParameterName_, color)
		the3dSolidShaders[key] = shader

	return shader

def releaseShaders():
	shaderMgr = omr.MRenderer.getShaderManager()
	if shaderMgr:
		for key, shader in list(the3dSolidShaders.items()):
			shaderMgr.releaseShader(shader)

		the3dSolidShaders.clear()


#############################################################################
##
## Node implementation with standard viewport draw
##
#############################################################################
class footPrint(omui.MPxLocatorNode):
	id = om.MTypeId( 0x00080066 )
	drawDbClassification = "drawdb/geometry/footPrint_GeometryOverride_py"
	drawRegistrantId = "FootprintNode_GeometryOverridePlugin_py"

	size = None	## The size of the foot

	@staticmethod
	def creator():
		return footPrint()

	@staticmethod
	def initialize():
		unitFn = om.MFnUnitAttribute()

		footPrint.size = unitFn.create( "size", "sz", om.MFnUnitAttribute.kDistance )
		unitFn.default = om.MDistance(1.0)

		om.MPxNode.addAttribute( footPrint.size )

	def __init__(self):
		omui.MPxLocatorNode.__init__(self)

	def compute(self, plug, data):
		return None

	def draw(self, view, path, style, status):
		## Get the size
		##
		thisNode = self.thisMObject()
		plug = om.MPlug( thisNode, footPrint.size )
		sizeVal = plug.asMDistance()
		multiplier = sizeVal.asCentimeters()

		global sole, soleCount
		global heel, heelCount

		view.beginGL()

		## drawing in VP1 views will be done using V1 Python APIs:
		import maya.OpenMayaRender as v1omr
		glRenderer = v1omr.MHardwareRenderer.theRenderer()
		glFT = glRenderer.glFunctionTable()

		if ( style == omui.M3dView.kFlatShaded ) or ( style == omui.M3dView.kGouraudShaded ):
			## Push the color settings
			##
			glFT.glPushAttrib( v1omr.MGL_CURRENT_BIT )
			
			# Show both faces
			glFT.glDisable( v1omr.MGL_CULL_FACE )

			if status == omui.M3dView.kActive:
				view.setDrawColor( 13, omui.M3dView.kActiveColors )
			else:
				view.setDrawColor( 13, omui.M3dView.kDormantColors )

			glFT.glBegin( v1omr.MGL_TRIANGLE_FAN )
			for i in range(soleCount-1):
				glFT.glVertex3f( sole[i][0] * multiplier, sole[i][1] * multiplier, sole[i][2] * multiplier )
			glFT.glEnd()

			glFT.glBegin( v1omr.MGL_TRIANGLE_FAN )
			for i in range(heelCount-1):
				glFT.glVertex3f( heel[i][0] * multiplier, heel[i][1] * multiplier, heel[i][2] * multiplier )
			glFT.glEnd()

			glFT.glPopAttrib()

		## Draw the outline of the foot
		##
		glFT.glBegin( v1omr.MGL_LINES )
		for i in range(soleCount-1):
			glFT.glVertex3f( sole[i][0] * multiplier, sole[i][1] * multiplier, sole[i][2] * multiplier )
			glFT.glVertex3f( sole[i+1][0] * multiplier, sole[i+1][1] * multiplier, sole[i+1][2] * multiplier )

		for i in range(heelCount-1):
			glFT.glVertex3f( heel[i][0] * multiplier, heel[i][1] * multiplier, heel[i][2] * multiplier )
			glFT.glVertex3f( heel[i+1][0] * multiplier, heel[i+1][1] * multiplier, heel[i+1][2] * multiplier )
		glFT.glEnd()

		view.endGL()

	def isBounded(self):
		return True

	def boundingBox(self):
		## Get the size
		##
		thisNode = self.thisMObject()
		plug = om.MPlug( thisNode, footPrint.size )
		sizeVal = plug.asMDistance()
		multiplier = sizeVal.asCentimeters()

		corner1 = om.MPoint( -0.17, 0.0, -0.7 )
		corner2 = om.MPoint( 0.17, 0.0, 0.3 )

		corner1 *= multiplier
		corner2 *= multiplier

		return om.MBoundingBox( corner1, corner2 )

	def getShapeSelectionMask(self):
		selType = om.MSelectionMask("footPrintSelection_py")
		return om.MSelectionMask( selType )

#############################################################################
##
## Viewport 2.0 override implementation
##
#############################################################################

class footPrintGeometryOverride(omr.MPxGeometryOverride):
	colorParameterName_ = "solidColor"
	wireframeItemName_ = "footPrintLocatorWires"
	shadedItemName_ = "footPrintLocatorTriangles"
	
	@staticmethod
	def creator(obj):
		return footPrintGeometryOverride(obj)

	def __init__(self, obj):
		omr.MPxGeometryOverride.__init__(self, obj)
		self.mLocatorNode = obj
		self.mMultiplier = 0.0
		self.mMultiplierChanged = True

	def __del__(self):
		pass

	def supportedDrawAPIs(self):
		# this plugin supports all modes
		return omr.MRenderer.kOpenGL | omr.MRenderer.kOpenGLCoreProfile | omr.MRenderer.kDirectX11

	def hasUIDrawables(self):
		return False

	def updateDG(self):
		plug = om.MPlug(self.mLocatorNode, footPrint.size)
		newScale = 1.0
		if not plug.isNull:
			sizeVal = plug.asMDistance()
			newScale = sizeVal.asCentimeters()
		
		if newScale != self.mMultiplier:
			self.mMultiplier = newScale
			self.mMultiplierChanged = True
	
	def cleanUp(self):
		pass

	def isIndexingDirty(self, item):
		return False
		
	def isStreamDirty(self, desc):
		return self.mMultiplierChanged
		
	def updateRenderItems(self, dagPath, renderList ):
		shader = get3dSolidShader(omr.MGeometryUtilities.wireframeColor(dagPath))
		if not shader:
			return

		fullItemList = ( 
			(footPrintGeometryOverride.wireframeItemName_, omr.MGeometry.kLines, omr.MGeometry.kWireframe),
			(footPrintGeometryOverride.shadedItemName_, omr.MGeometry.kTriangles, omr.MGeometry.kShaded)
		)

		for itemName, geometryType, drawMode in fullItemList:
			renderItem = None
			index = renderList.indexOf(itemName)
			if index < 0:
				renderItem = omr.MRenderItem.create(
					itemName,
					omr.MRenderItem.DecorationItem,
					geometryType)
				renderItem.setDrawMode(drawMode)
				renderItem.setDepthPriority(5)

				renderList.append(renderItem)
			else:
				renderItem = renderList[index]

			if renderItem:
				renderItem.setShader(shader)
				renderItem.enable(True)

	def populateGeometry(self, requirements, renderItems, data):
		vertexBufferDescriptorList = requirements.vertexRequirements()
		
		for vertexBufferDescriptor in vertexBufferDescriptorList:
			if vertexBufferDescriptor.semantic == omr.MGeometry.kPosition:
				verticesCount = soleCount+heelCount
				verticesBuffer = data.createVertexBuffer(vertexBufferDescriptor)
				verticesPositionDataAddress = verticesBuffer.acquire(verticesCount, True)
				verticesPositionData = ((ctypes.c_float * 3)*verticesCount).from_address(verticesPositionDataAddress)	
				
				verticesPointerOffset = 0
				
				# We concatenate the heel and sole positions into a single vertex buffer.
				# The index buffers will decide which positions will be selected for each render items.
				for vtxList in (heel, sole):
					for vtx in vtxList:
						verticesPositionData[verticesPointerOffset][0] = vtx[0] * self.mMultiplier
						verticesPositionData[verticesPointerOffset][1] = vtx[1] * self.mMultiplier
						verticesPositionData[verticesPointerOffset][2] = vtx[2] * self.mMultiplier
						verticesPointerOffset += 1
						
				verticesBuffer.commit(verticesPositionDataAddress)
				
				break
					
		for item in renderItems:
			if not item:
				continue

			indexBuffer = data.createIndexBuffer(omr.MGeometry.kUnsignedInt32)

			if item.name() == footPrintGeometryOverride.wireframeItemName_:

				primitiveIndex = 0
				startIndex = 0
				numPrimitive = heelCount + soleCount - 2
				numIndex = numPrimitive * 2

				indicesAddress = indexBuffer.acquire(numIndex, True)
				indices = (ctypes.c_uint * numIndex).from_address(indicesAddress)

				i = 0

				while i < numIndex:

					if i < (heelCount - 1) * 2:
						startIndex = 0
						primitiveIndex = i // 2
					else:
						startIndex = heelCount
						primitiveIndex = i // 2 - heelCount + 1

					indices[i] = startIndex + primitiveIndex
					indices[i+1] = startIndex + primitiveIndex + 1

					i += 2

				indexBuffer.commit(indicesAddress)

			elif item.name() == footPrintGeometryOverride.shadedItemName_:
				primitiveIndex = 0
				startIndex = 0
				numPrimitive = heelCount + soleCount - 4
				numIndex = numPrimitive * 3

				indicesAddress = indexBuffer.acquire(numIndex, True)
				indices = (ctypes.c_uint * numIndex).from_address(indicesAddress)

				i = 0

				while i < numIndex:

					if i < (heelCount - 2) * 3:
						startIndex = 0
						primitiveIndex = i // 3
					else:
						startIndex = heelCount
						primitiveIndex = i // 3 - heelCount + 2

					indices[i] = startIndex
					indices[i+1] = startIndex + primitiveIndex + 1
					indices[i+2] = startIndex + primitiveIndex + 2

					i += 3

				indexBuffer.commit(indicesAddress)

			item.associateWithIndexBuffer(indexBuffer)

		mMultiplierChanged = False

def initializePlugin(obj):
	plugin = om.MFnPlugin(obj, "Autodesk", "3.0", "Any")

	try:
		plugin.registerNode("footPrint_GeometryOverride_py", footPrint.id, footPrint.creator, footPrint.initialize, om.MPxNode.kLocatorNode, footPrint.drawDbClassification)
	except:
		sys.stderr.write("Failed to register node\n")
		raise

	try:
		omr.MDrawRegistry.registerGeometryOverrideCreator(footPrint.drawDbClassification, footPrint.drawRegistrantId, footPrintGeometryOverride.creator)
	except:
		sys.stderr.write("Failed to register override\n")
		raise

	try:
		om.MSelectionMask.registerSelectionType("footPrintSelection_py")
		mel.eval("selectType -byName \"footPrintSelection_py\" 1")
	except:
		sys.stderr.write("Failed to register selection mask\n")
		raise

def uninitializePlugin(obj):
	plugin = om.MFnPlugin(obj)

	try:
		plugin.deregisterNode(footPrint.id)
	except:
		sys.stderr.write("Failed to deregister node\n")
		pass

	try:
		omr.MDrawRegistry.deregisterGeometryOverrideCreator(footPrint.drawDbClassification, footPrint.drawRegistrantId)
	except:
		sys.stderr.write("Failed to deregister override\n")
		pass

	try:
		om.MSelectionMask.deregisterSelectionType("footPrintSelection_py") 
	except:
		sys.stderr.write("Failed to deregister selection mask\n")
		pass

	try:
		releaseShaders()
	except:
		sys.stderr.write("Failed to release shaders\n")
		pass
