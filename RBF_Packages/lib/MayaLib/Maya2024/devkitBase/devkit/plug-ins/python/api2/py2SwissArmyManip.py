##-
## ==========================================================================
## Copyright 2019 Autodesk, Inc.  All rights reserved.
##
## Use of this software is subject to the terms of the Autodesk
## license agreement provided at the time of installation or download,
## or which otherwise accompanies this software in either electronic
## or hard copy form.
## ==========================================================================
##+

#
# Autodesk Script File
# MODIFY THIS AT YOUR OWN RISK
#
# Creation Date:   11 October 2019
#

#########################################################################
# DESCRIPTION:
#
# A Python 2.0 version of swissArmyManip.py
# 
# This plug-in is an example of a user-defined manipulator,
# which is comprised of a variety of the base manipulators:
#	- MFnCircleSweepManip
#	- MFnDirectionManip
#	- MFnDiscManip
#	- MFnDistanceManip
#	- MFnFreePointTriadManip
#	- MFnStateManip
#	- MFnToggleManip
#	- MFnRotateManip
#	- MFnScaleManip
#
# It attaches one of every kind of user-defined manipulator to a node.
# It demonstrates the source code required to create each user-defined manipulator.
#
# To use this plug-in:
#
#	(1) Execute these commands:
# 
#		import maya.cmds as cmds
#		cmds.createNode("spSwissArmyLocator2")
#
#  	(2) Click the Show Manipulator Tool icon on the side toolbar.
#		The locator on the screen will be overlaid with one of every kind of user-defined manipulator.
# 
#########################################################################

import maya.api.OpenMaya as OpenMaya
import maya.api.OpenMayaUI as OpenMayaUI
import maya.OpenMayaRender as OpenMayaRender

import math,sys
import maya.cmds as cmds

glRenderer = OpenMayaRender.MHardwareRenderer.theRenderer()
glFT = glRenderer.glFunctionTable()

def maya_useNewAPI():
	pass

kSwissArmyLocator2Name = "spSwissArmyLocator2"
kSwissArmyLocator2Id = OpenMaya.MTypeId(0x87019)
kSwissArmyLocator2ManipName = "spSwissArmyLocator2Manip"
kSwissArmyLocator2ManipId = OpenMaya.MTypeId(0x8701A)

delta1 = 0.01
delta2 = 0.02
delta3 = 0.03
delta4 = 0.04

# Locator Data
centre = [	[  0.10, 0.0,  0.10 ],
			[  0.10, 0.0, -0.10 ],
			[ -0.10, 0.0, -0.10 ],
			[ -0.10, 0.0,  0.10 ], 
			[  0.10, 0.0,  0.10 ] ] 
state1 = [	[  1.00, 0.0,  1.00 ],
			[  1.00, 0.0,  0.50 ],
			[  0.50, 0.0,  0.50 ],
			[  0.50, 0.0,  1.00 ], 
			[  1.00, 0.0,  1.00 ] ] 
state2 = [	[  1.00, 0.0, -1.00 ],
			[  1.00, 0.0, -0.50 ],
			[  0.50, 0.0, -0.50 ],
			[  0.50, 0.0, -1.00 ], 
			[  1.00, 0.0, -1.00 ] ] 
state3 = [	[ -1.00, 0.0, -1.00 ],
			[ -1.00, 0.0, -0.50 ],
			[ -0.50, 0.0, -0.50 ],
			[ -0.50, 0.0, -1.00 ], 
			[ -1.00, 0.0, -1.00 ] ] 
state4 = [	[ -1.00, 0.0,  1.00 ],
			[ -1.00, 0.0,  0.50 ],
			[ -0.50, 0.0,  0.50 ],
			[ -0.50, 0.0,  1.00 ], 
			[ -1.00, 0.0,  1.00 ] ] 
arrow1 = [	[  0.00, 0.0,  1.00 ],
			[  0.10, 0.0,  0.20 ],
			[ -0.10, 0.0,  0.20 ],
			[  0.00, 0.0,  1.00 ] ] 
arrow2 = [	[  1.00, 0.0,  0.00 ],
			[  0.20, 0.0,  0.10 ],
			[  0.20, 0.0, -0.10 ],
			[  1.00, 0.0,  0.00 ] ] 
arrow3 = [	[  0.00, 0.0, -1.00 ],
			[  0.10, 0.0, -0.20 ],
			[ -0.10, 0.0, -0.20 ],
			[  0.00, 0.0, -1.00 ] ] 
arrow4 = [	[ -1.00, 0.0,  0.00 ],
			[ -0.20, 0.0,  0.10 ],
			[ -0.20, 0.0, -0.10 ],
			[ -1.00, 0.0,  0.00 ] ] 
perimeter=[	[  1.10, 0.0,  1.10 ],
			[  1.10, 0.0, -1.10 ],
			[ -1.10, 0.0, -1.10 ],
			[ -1.10, 0.0,  1.10 ], 
			[  1.10, 0.0,  1.10 ] ] 

kCentreCount = 5
kState1Count = 5
kState2Count = 5
kState3Count = 5
kState4Count = 5
kArrow1Count = 4
kArrow2Count = 4
kArrow3Count = 4
kArrow4Count = 4
kPerimeterCount = 5


########################################################################
########################################################################


class swissArmyLocator2Manip(OpenMayaUI.MPxManipContainer):
	def __init__(self):
		OpenMayaUI.MPxManipContainer.__init__(self)

		
		self.fCircleSweepManip = OpenMaya.MDagPath()
		self.fDirectionManip = OpenMaya.MDagPath()
		self.fDiscManip = OpenMaya.MDagPath()
		self.fDistanceManip = OpenMaya.MDagPath()
		self.fFreePointTriadManip = OpenMaya.MDagPath()
		self.fStateManip = OpenMaya.MDagPath()
		self.fToggleManip = OpenMaya.MDagPath()
		self.fRotateManip = OpenMaya.MDagPath()
		self.fScaleManip = OpenMaya.MDagPath()
		self.fNodePath = OpenMaya.MDagPath()


	def createChildren(self):
		# FreePointTriadManip
		self.fFreePointTriadManip = self.addFreePointTriadManip("freePointTriadManip", "point")
		freePointTriadManipFn = OpenMayaUI.MFnFreePointTriadManip(self.fFreePointTriadManip)

		# DirectionManip
		self.fDirectionManip = self.addDirectionManip("directionManip", "direction")
		directionManipFn = OpenMayaUI.MFnDirectionManip(self.fDirectionManip)
		self._DirectionDirectionIndex = directionManipFn.directionIndex()
		self._DirectionStartPointIndex = directionManipFn.startPointIndex()

		# ToggleManip
		self.fToggleManip = self.addToggleManip("toggleManip", "toggle")
		toggleManipFn = OpenMayaUI.MFnToggleManip(self.fToggleManip)
		self._ToggleLengthIndex = toggleManipFn.lengthIndex()
		self._ToggleStartPointIndex = toggleManipFn.startPointIndex()

		# StateManip
		self.fStateManip = self.addStateManip("stateManip", "state")
		stateManipFn = OpenMayaUI.MFnStateManip(self.fStateManip)
		self._StatePositionIndex = stateManipFn.positionIndex()

		# DiscManip
		self.fDiscManip = self.addDiscManip("discManip", "angle")
		discManipFn = OpenMayaUI.MFnDiscManip(self.fDiscManip)
		self._DiscCenterIndex = discManipFn.centerIndex()

		# CircleSweepManip
		self.fCircleSweepManip = self.addCircleSweepManip("circleSweepManip", "angle")
		circleSweepManipFn = OpenMayaUI.MFnCircleSweepManip(self.fCircleSweepManip)
		circleSweepManipFn.setCenterPoint(OpenMaya.MPoint(2, 7, 2))
		circleSweepManipFn.setNormal(OpenMaya.MVector(0, 1, 0))
		circleSweepManipFn.setRadius(2.0)
		circleSweepManipFn.setDrawAsArc(True)
		self._CircleSweepCenterIndex = circleSweepManipFn.centerIndex()

		# DistanceManip
		self.fDistanceManip = self.addDistanceManip("distanceManip", "distance")
		distanceManipFn = OpenMayaUI.MFnDistanceManip(self.fDistanceManip)
		distanceManipFn.setStartPoint(OpenMaya.MPoint(0, 0, 0))
		distanceManipFn.setDirection(OpenMaya.MVector(0, 1, 0))
		self._DistanceStartPointIndex = distanceManipFn.startPointIndex()

		# RotateManip
		self.fRotateManip = self.addRotateManip("RotateManip", "rotation")
		rotateManipFn = OpenMayaUI.MFnRotateManip(self.fRotateManip)

		# ScaleManip
		self.fScaleManip = self.addScaleManip("scaleManip", "scale")
		scaleManipFn = OpenMayaUI.MFnScaleManip(self.fScaleManip)

	def connectToDependNode(self, node):
		# Get the DAG path
		dagNodeFn = OpenMaya.MFnDagNode(node)
		self.fNodePath = dagNodeFn.getPath()
		parentNode = dagNodeFn.parent(0)
		parentNodeFn = OpenMaya.MFnDagNode(parentNode)
		parentPath = parentNodeFn.getPath()

		# Connect the plugs
		nodeFn = OpenMaya.MFnDependencyNode()
		nodeFn.setObject(node)   
		self._plugDoubleIndex = -1
		self._plugMPointIndex = -1
		self._plugMVectorIndex = -1

		# FreePointTriadManip
		freePointTriadManipFn = OpenMayaUI.MFnFreePointTriadManip(self.fFreePointTriadManip)
		try:
			translationPlug = parentNodeFn.findPlug("t",True)
			freePointTriadManipFn.connectToPointPlug(translationPlug)
		except:
			pass

		# DirectionManip
		directionManipFn = OpenMayaUI.MFnDirectionManip()
		directionManipFn.setObject(self.fDirectionManip)
		directionManipFn.setDirection(OpenMaya.MVector(3, 1, 4))
		try:
			directionPlug = nodeFn.findPlug("arrow2Direction",True)
			directionManipFn.connectToDirectionPlug(directionPlug)
			self.addPlugToManipConversion(self._DirectionStartPointIndex)
		except:
			pass

		try:
			self.addPlugToManipConversion(self._DirectionDirectionIndex)
		except:
			pass

		# DistanceManip
		distanceManipFn = OpenMayaUI.MFnDistanceManip()
		distanceManipFn.setObject(self.fDistanceManip)
		try:
			sizePlug = nodeFn.findPlug("size",True)
			distanceManipFn.connectToDistancePlug(sizePlug)
			self.addPlugToManipConversion(self._DistanceStartPointIndex)
		except:
			pass

		# CircleSweepManip
		circleSweepManipFn = OpenMayaUI.MFnCircleSweepManip(self.fCircleSweepManip)
		try:
			arrow1AnglePlug = nodeFn.findPlug("arrow1Angle",True)
			circleSweepManipFn.connectToAnglePlug(arrow1AnglePlug)
			self.addPlugToManipConversion(self._CircleSweepCenterIndex)
		except:
			pass

		# DiscManip
		discManipFn = OpenMayaUI.MFnDiscManip(self.fDiscManip)
		try:
			arrow3AnglePlug = nodeFn.findPlug("arrow3Angle",True)
			discManipFn.connectToAnglePlug(arrow3AnglePlug)
			self.addPlugToManipConversion(self._DiscCenterIndex)
		except:
			pass

		# StateManip
		stateManipFn = OpenMayaUI.MFnStateManip(self.fStateManip)
		try:
			statePlug = nodeFn.findPlug("state",True)
			stateManipFn.connectToStatePlug(statePlug)
			self.addPlugToManipConversion(self._StatePositionIndex)
		except:
			pass

		# ToggleManip
		toggleManipFn = OpenMayaUI.MFnToggleManip(self.fToggleManip)
		toggleManipFn.length = 3.14
		try:
			togglePlug = nodeFn.findPlug("toggle",True)
			toggleManipFn.connectToTogglePlug(togglePlug)
			self.addPlugToManipConversion(self._ToggleStartPointIndex)
		except:
			pass

		try:
			self.addPlugToManipConversion(self._ToggleLengthIndex)
		except:
			pass

		# Determine the transform node for the locator
		transformPath = OpenMaya.MDagPath(self.fNodePath)
		transformPath.pop()

		transformNode = OpenMaya.MFnTransform(transformPath)

		# RotateManip
		rotateManipFn = OpenMayaUI.MFnRotateManip(self.fRotateManip)
		try:
			rotatePlug = transformNode.findPlug("rotate",True)
			rotateManipFn.connectToRotationPlug(rotatePlug)
			rotateManipFn.displayWithNode(node)
		except:
			pass

		# ScaleManip
		scaleManipFn = OpenMayaUI.MFnScaleManip(self.fScaleManip)
		try:
			scalePlug = transformNode.findPlug("scale",True)
			scaleManipFn.connectToScalePlug(scalePlug)
			scaleManipFn.displayWithNode(node)
		except:
			pass

		try:
			mVectorPlug = nodeFn.findPlug("inPlugMVector",True)
			tmpIndex = self.addManipToPlugConversion(mVectorPlug)
			self._plugMVectorIndex = tmpIndex
		except:
			pass

		try:
			mPointPlug = nodeFn.findPlug("inPlugMPoint",True)
			tmpIndex = self.addManipToPlugConversion(mPointPlug)
			self._plugMPointIndex = tmpIndex
		except:
			pass

		try:
			doublePlug = nodeFn.findPlug("inPlugDouble",True)
			tmpIndex = self.addManipToPlugConversion(doublePlug)
			self._plugDoubleIndex = tmpIndex
		except:
			pass

		self.finishAddingManips()
		OpenMayaUI.MPxManipContainer.connectToDependNode(self, node)


	def draw(self, view, path, style, status):
		OpenMayaUI.MPxManipContainer.draw(self, view, path, style, status)
		view.beginGL()
		textPos = OpenMaya.MPoint(self.nodeTranslation())
		view.drawText("Swiss Army Manipulator 2", textPos, OpenMayaUI.M3dView.kLeft)
		view.endGL()


	def plugToManipConversion(self, theIndex):

		manipVec = self.getConverterManipMVectorValue(theIndex)
		numData = OpenMaya.MFnNumericData()
		numDataObj = numData.create(OpenMaya.MFnNumericData.k3Float)
		numData.setData(manipVec)
		manipData = OpenMayaUI.MManipData(numDataObj)

		# Set some out attributes for testing getConvertManip by a calling
		# script
		if theIndex == self._CircleSweepCenterIndex:
			# Only want to stuff the initial value into the out attribute
			self._CircleSweepCenterIndex = -1
			manipMPoint = self.getConverterManipMPointValue(theIndex)
			x = manipMPoint[0]
			y = manipMPoint[1]
			z = manipMPoint[2]
			a = manipMPoint[3]
			cmds.setAttr( "%s.outManipMPoint"%(self.fNodePath), x, y, z, a )

		if theIndex == self._DirectionDirectionIndex:
			# Only want to stuff the initial value into the out attribute
			self._DirectionDirectionIndex = -1
			manipMVector = self.getConverterManipMVectorValue(theIndex)
			x = manipMVector[0]
			y = manipMVector[1]
			z = manipMVector[2]
			cmds.setAttr( "%s.outManipMVector"%(self.fNodePath), x, y, z )

		if theIndex == self._ToggleLengthIndex:
			# Only want to stuff the initial value into the out attribute
			self._ToggleLengthIndex = -1
			manipDouble = self.getConverterManipDoubleValue(theIndex)
			cmds.setAttr( "%s.outManipDouble"%(self.fNodePath), manipDouble )
			result = OpenMayaUI.MManipData(manipDouble)
			return result

		return manipData

	def manipToPlugConversion(self, plugIndex):

		plugVec = self.getConverterPlugMVectorValue(plugIndex)
		numData = OpenMaya.MFnNumericData()
		numDataObj = numData.create(OpenMaya.MFnNumericData.k3Double)
		numData.setData(plugVec)

		manipData = OpenMayaUI.MManipData(numDataObj)

		# Set some out attributes for testing getConvertPlug by a calling script
		if plugIndex == self._plugMPointIndex:
			plugMPoint = self.getConverterPlugMPointValue(self._plugMPointIndex)
			x = plugMPoint[0]
			y = plugMPoint[1]
			z = plugMPoint[2]
			a = plugMPoint[3]
			cmds.setAttr( "%s.outPlugMPoint"%(self.fNodePath), x, y, z, a )

		if plugIndex == self._plugMVectorIndex:
			plugMVector = self.getConverterPlugMVectorValue(self._plugMVectorIndex)
			x = plugMVector[0]
			y = plugMVector[1]
			z = plugMVector[2]
			cmds.setAttr( "%s.outPlugMVector"%(self.fNodePath), x, y, z )

		if plugIndex == self._plugDoubleIndex:
			plugDouble = self.getConverterPlugDoubleValue(self._plugDoubleIndex)
			cmds.setAttr( "%s.outPlugDouble"%(self.fNodePath), plugDouble )
			result = OpenMayaUI.MManipData(plugDouble)
			return result

		return manipData

	
	def nodeTranslation(self):
		dagFn = OpenMaya.MFnDagNode(self.fNodePath)
		path = OpenMaya.MDagPath()
		path = dagFn.getPath()
		path.pop()  # pop from the shape to the transform
		transformFn = OpenMaya.MFnTransform(path)
		return transformFn.translation(OpenMaya.MSpace.kWorld)


########################################################################
########################################################################

class swissArmyLocator2(OpenMayaUI.MPxLocatorNode):
	aSize = OpenMaya.MObject() # The size of the locator
	aPoint = OpenMaya.MObject()
	aPointX = OpenMaya.MObject()
	aPointY = OpenMaya.MObject()
	aPointZ = OpenMaya.MObject()
	aArrow1Angle = OpenMaya.MObject()
	aArrow2Direction = OpenMaya.MObject()
	aArrow2DirectionX = OpenMaya.MObject()
	aArrow2DirectionY = OpenMaya.MObject()
	aArrow2DirectionZ = OpenMaya.MObject()

	aArrow3Angle = OpenMaya.MObject()
	aArrow4Distance = OpenMaya.MObject()
	aState = OpenMaya.MObject()
	aToggle = OpenMaya.MObject()

	# The in/out Manip/Plug attributes allow testing of the getConverter
	# methods from a calling script
	# In Manips
	aInManipDouble = OpenMaya.MObject()
	aInManipMPoint = OpenMaya.MObject()
	aInManipMPointX = OpenMaya.MObject()
	aInManipMPointY = OpenMaya.MObject()
	aInManipMPointZ = OpenMaya.MObject()
	aInManipMVector = OpenMaya.MObject()
	aInManipMVectorX = OpenMaya.MObject()
	aInManipMVectorY = OpenMaya.MObject()
	aInManipMVectorZ = OpenMaya.MObject()

	# In Plugs
	aInPlugDouble = OpenMaya.MObject()
	aInPlugMPoint = OpenMaya.MObject()
	aInPlugMPointX = OpenMaya.MObject()
	aInPlugMPointY = OpenMaya.MObject()
	aInPlugMPointZ = OpenMaya.MObject()
	aInPlugMVector = OpenMaya.MObject()
	aInPlugMVectorX = OpenMaya.MObject()
	aInPlugMVectorY = OpenMaya.MObject()
	aInPlugMVectorZ = OpenMaya.MObject()

	# Out Manips
	aOutManipDouble = OpenMaya.MObject()
	aOutManipMPoint = OpenMaya.MObject()
	aOutManipMPointX = OpenMaya.MObject()
	aOutManipMPointY = OpenMaya.MObject()
	aOutManipMPointZ = OpenMaya.MObject()
	aOutManipMVector = OpenMaya.MObject()
	aOutManipMVectorX = OpenMaya.MObject()
	aOutManipMVectorY = OpenMaya.MObject()
	aOutManipMVectorZ = OpenMaya.MObject()

	# Out Plugs
	aOutPlugDouble = OpenMaya.MObject()
	aOutPlugMPoint = OpenMaya.MObject()
	aOutPlugMPointX = OpenMaya.MObject()
	aOutPlugMPointY = OpenMaya.MObject()
	aOutPlugMPointZ = OpenMaya.MObject()
	aOutPlugMVector = OpenMaya.MObject()
	aOutPlugMVectorX = OpenMaya.MObject()
	aOutPlugMVectorY = OpenMaya.MObject()
	aOutPlugMVectorZ = OpenMaya.MObject()

	def __init__(self):
		OpenMayaUI.MPxLocatorNode.__init__(self)


	def compute(self, plug, data):
		return None


	def draw(self, view, path, style, status):

		# Get the size
		thisNode = self.thisMObject()

		plug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aSize)
		sizeVal = plug.asMDistance()

		arrow1AnglePlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aArrow1Angle)
		arrow1Angle = arrow1AnglePlug.asMAngle()
		angle1 = -arrow1Angle.asRadians() - 3.1415927/2.0

		arrow3AnglePlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aArrow3Angle)
		arrow3Angle = arrow3AnglePlug.asMAngle()
		angle3 = arrow3Angle.asRadians()

		statePlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aState)
		state = statePlug.asInt()

		togglePlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aToggle)
		toggle = togglePlug.asBool()

		directionXPlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aArrow2DirectionX)
		directionYPlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aArrow2DirectionY)
		directionZPlug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aArrow2DirectionZ)
		dirX = directionXPlug.asDouble()
		dirY = directionYPlug.asDouble()
		dirZ = directionZPlug.asDouble()

		angle2 = math.atan2(dirZ, dirX)
		angle2 += 3.1415927

		multiplier = sizeVal.asCentimeters()

		view.beginGL() 

		if ((style == OpenMayaUI.M3dView.kFlatShaded) or
				(style == OpenMayaUI.M3dView.kGouraudShaded)):
			# Push the color settings
			glFT.glPushAttrib(OpenMayaRender.MGL_CURRENT_BIT)

			if (status == OpenMayaUI.M3dView.kActive):
				view.setDrawColor(13, OpenMayaUI.M3dView.kActiveColors)
			else:
				view.setDrawColor(13, OpenMayaUI.M3dView.kDormantColors)

			if (toggle):
				if (status == OpenMayaUI.M3dView.kActive):
					view.setDrawColor(15, OpenMayaUI.M3dView.kActiveColors)
				else:
					view.setDrawColor(15, OpenMayaUI.M3dView.kDormantColors)
				glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
				last = kCentreCount - 1
				for i in range(last):
					glFT.glVertex3f(centre[i][0] * multiplier,
							   centre[i][1] * multiplier,
							   centre[i][2] * multiplier)
				glFT.glEnd()

			if (state == 0):
				if (status == OpenMayaUI.M3dView.kActive):
					view.setDrawColor(19, OpenMayaUI.M3dView.kActiveColors)
				else:
					view.setDrawColor(19, OpenMayaUI.M3dView.kDormantColors)
				glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
				last = kState1Count - 1
				for i in range(last):
					glFT.glVertex3f(state1[i][0] * multiplier,
									state1[i][1] * multiplier,
									state1[i][2] * multiplier)
				glFT.glEnd()

			if (state == 1):
				if (status == OpenMayaUI.M3dView.kActive):
					view.setDrawColor(21, OpenMayaUI.M3dView.kActiveColors)
				else:
					view.setDrawColor(21, OpenMayaUI.M3dView.kDormantColors)
				glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
				last = kState2Count - 1
				for i in range(last):
					glFT.glVertex3f(state2[i][0] * multiplier,
									state2[i][1] * multiplier,
									state2[i][2] * multiplier)
				glFT.glEnd()

			if (state == 2):
				if (status == OpenMayaUI.M3dView.kActive):
					view.setDrawColor(18, OpenMayaUI.M3dView.kActiveColors)
				else:
					view.setDrawColor(18, OpenMayaUI.M3dView.kDormantColors)
					glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
					last = kState3Count - 1
					for i in range(last):
						glFT.glVertex3f(state3[i][0] * multiplier,
										state3[i][1] * multiplier,
										state3[i][2] * multiplier)
					glFT.glEnd()

			if (state == 3):
				if (status == OpenMayaUI.M3dView.kActive):
					view.setDrawColor(17, OpenMayaUI.M3dView.kActiveColors)
				else:
					view.setDrawColor(17, OpenMayaUI.M3dView.kDormantColors)
				glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
				last = kState4Count - 1
				for i in range(last):
					glFT.glVertex3f(state4[i][0] * multiplier,
									state4[i][1] * multiplier,
									state4[i][2] * multiplier)
				glFT.glEnd()

			if (status == OpenMayaUI.M3dView.kActive):
				view.setDrawColor(12, OpenMayaUI.M3dView.kActiveColors)
			else:
				view.setDrawColor(12, OpenMayaUI.M3dView.kDormantColors)
			glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
			last = kArrow1Count - 1
			for i in range(last):
				glFT.glVertex3f((-arrow1[i][0] * multiplier * math.cos(angle1) - arrow1[i][2] * multiplier * math.sin(angle1)),
								(arrow1[i][1] * multiplier + delta1),
								(arrow1[i][2] * multiplier * math.cos(angle1) - arrow1[i][0] * multiplier * math.sin(angle1)))
			glFT.glEnd()

			if (status == OpenMayaUI.M3dView.kActive):
				view.setDrawColor(16, OpenMayaUI.M3dView.kActiveColors)
			else:
				view.setDrawColor(16, OpenMayaUI.M3dView.kDormantColors)
			glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
			last = kArrow2Count - 1
			for i in range(last):
				glFT.glVertex3f((-arrow2[i][0] * multiplier * math.cos(angle2) - arrow2[i][2] * multiplier * math.sin(angle2)),
								(arrow2[i][1] * multiplier + delta2),
								(arrow2[i][2] * multiplier * math.cos(angle2) - arrow2[i][0] * multiplier * math.sin(angle2)))
			glFT.glEnd()

			if (status == OpenMayaUI.M3dView.kActive):
				view.setDrawColor(13, OpenMayaUI.M3dView.kActiveColors)
			else:
				view.setDrawColor(13, OpenMayaUI.M3dView.kDormantColors)
			glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
			last = kArrow3Count - 1
			for i in range(last):
				glFT.glVertex3f((-arrow3[i][0] * multiplier * math.cos(angle3) - arrow3[i][2] * multiplier * math.sin(angle3)),
								(arrow3[i][1] * multiplier + delta3),
								(arrow3[i][2] * multiplier * math.cos(angle3) - arrow3[i][0] * multiplier * math.sin(angle3)))
			glFT.glEnd()

			if (status == OpenMayaUI.M3dView.kActive):
				view.setDrawColor(5, OpenMayaUI.M3dView.kActiveColors)
			else:
				view.setDrawColor(5, OpenMayaUI.M3dView.kDormantColors)
			glFT.glBegin(OpenMayaRender.MGL_TRIANGLE_FAN)
			last = kArrow4Count - 1
			for i in range(last):
				glFT.glVertex3f((arrow4[i][0] * multiplier),
								(arrow4[i][1] * multiplier + delta4),
								(arrow4[i][2] * multiplier))
			glFT.glEnd()

			glFT.glPopAttrib()

		# Draw the outline of the locator
		glFT.glBegin(OpenMayaRender.MGL_LINES)

		if toggle:
			last = kCentreCount - 1
			for i in range(last): 
				glFT.glVertex3f(centre[i][0] * multiplier, 
								centre[i][1] * multiplier, 
								centre[i][2] * multiplier)
				glFT.glVertex3f(centre[i+1][0] * multiplier, 
								centre[i+1][1] * multiplier, 
								centre[i+1][2] * multiplier)

		if (state == 0):
			last = kState1Count - 1
			for i in range(last): 
				glFT.glVertex3f(state1[i][0] * multiplier, 
								state1[i][1] * multiplier, 
								state1[i][2] * multiplier)
				glFT.glVertex3f(state1[i+1][0] * multiplier, 
								state1[i+1][1] * multiplier, 
								state1[i+1][2] * multiplier)

		if (state == 1):
			last = kState2Count - 1
			for i in range(last): 
				glFT.glVertex3f(state2[i][0] * multiplier, 
								state2[i][1] * multiplier, 
								state2[i][2] * multiplier)
				glFT.glVertex3f(state2[i+1][0] * multiplier, 
								state2[i+1][1] * multiplier, 
								state2[i+1][2] * multiplier)

		if (state == 2):
			last = kState3Count - 1
			for i in range(last): 
				glFT.glVertex3f(state3[i][0] * multiplier, 
								state3[i][1] * multiplier, 
								state3[i][2] * multiplier)
				glFT.glVertex3f(state3[i+1][0] * multiplier, 
								state3[i+1][1] * multiplier, 
								state3[i+1][2] * multiplier)

		if (state == 3):
			last = kState4Count - 1
			for i in range(last): 
				glFT.glVertex3f(state4[i][0] * multiplier, 
								state4[i][1] * multiplier, 
								state4[i][2] * multiplier)
				glFT.glVertex3f(state4[i+1][0] * multiplier, 
								state4[i+1][1] * multiplier, 
								state4[i+1][2] * multiplier)

		last = kArrow1Count - 1
		for i in range(last): 
			glFT.glVertex3f((-arrow1[i][0] * multiplier * math.cos(angle1) - arrow1[i][2] * multiplier * math.sin(angle1)),
							(arrow1[i][1] * multiplier + delta1),
							(arrow1[i][2] * multiplier * math.cos(angle1) - arrow1[i][0] * multiplier * math.sin(angle1)))
			glFT.glVertex3f((-arrow1[i+1][0] * multiplier * math.cos(angle1) - arrow1[i+1][2] * multiplier * math.sin(angle1)),
							(arrow1[i+1][1] * multiplier + delta1),
							(arrow1[i+1][2] * multiplier * math.cos(angle1) - arrow1[i+1][0] * multiplier * math.sin(angle1)))

		last = kArrow2Count - 1
		for i in range(last): 
			glFT.glVertex3f((-arrow2[i][0] * multiplier * math.cos(angle2) - arrow2[i][2] * multiplier * math.sin(angle2)),
							(arrow2[i][1] * multiplier + delta2),
							(arrow2[i][2] * multiplier * math.cos(angle2) - arrow2[i][0] * multiplier * math.sin(angle2)))
			glFT.glVertex3f((-arrow2[i+1][0] * multiplier * math.cos(angle2) - arrow2[i+1][2] * multiplier * math.sin(angle2)),
							(arrow2[i+1][1] * multiplier + delta2),
							(arrow2[i+1][2] * multiplier * math.cos(angle2) - arrow2[i+1][0] * multiplier * math.sin(angle2)))

		last = kArrow3Count - 1
		for i in range(last): 
			glFT.glVertex3f((-arrow3[i][0] * multiplier * math.cos(angle3) - arrow3[i][2] * multiplier * math.sin(angle3)),
							(arrow3[i][1] * multiplier + delta3),
							(arrow3[i][2] * multiplier * math.cos(angle3) - arrow3[i][0] * multiplier * math.sin(angle3)))
			glFT.glVertex3f((-arrow3[i+1][0] * multiplier * math.cos(angle3) - arrow3[i+1][2] * multiplier * math.sin(angle3)),
							(arrow3[i+1][1] * multiplier + delta3),
							(arrow3[i+1][2] * multiplier * math.cos(angle3) - arrow3[i+1][0] * multiplier * math.sin(angle3)))

		last = kArrow4Count - 1
		for i in range(last): 
			glFT.glVertex3f((arrow4[i][0] * multiplier),
							(arrow4[i][1] * multiplier + delta4),
							(arrow4[i][2] * multiplier))
			glFT.glVertex3f((arrow4[i+1][0] * multiplier),
							(arrow4[i+1][1] * multiplier + delta4),
							(arrow4[i+1][2] * multiplier))

		last = kPerimeterCount - 1
		for i in range(last): 
			glFT.glVertex3f(perimeter[i][0] * multiplier,
							perimeter[i][1] * multiplier,
							perimeter[i][2] * multiplier)
			glFT.glVertex3f(perimeter[i+1][0] * multiplier,
							perimeter[i+1][1] * multiplier,
							perimeter[i+1][2] * multiplier)

		glFT.glEnd()

		view.endGL()


	def isBounded(self):
		return True


	def boundingBox(self):
		thisNode = self.thisMObject()
		plug = OpenMaya.MPlug(thisNode, swissArmyLocator2.aSize)
		sizeVal = plug.asMDistance()

		multiplier = sizeVal.asCentimeters()

		corner1 = OpenMaya.MPoint(-1.1, 0.0, -1.1)
		corner2 = OpenMaya.MPoint(1.1, 0.0, 1.1)

		corner1 = corner1 * multiplier
		corner2 = corner2 * multiplier

		return OpenMaya.MBoundingBox(corner1, corner2)



########################################################################
########################################################################


def locatorCreator():
	return swissArmyLocator2()


def locatorInit():
	unitFn = OpenMaya.MFnUnitAttribute()
	numericFn = OpenMaya.MFnNumericAttribute()

	# aSize
	swissArmyLocator2.aSize = unitFn.create("size", "sz", OpenMaya.MFnUnitAttribute.kDistance, 10.0)

	# aPoint
	swissArmyLocator2.aPointX = numericFn.create("pointX", "ptx", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aPointY = numericFn.create("pointY", "pty", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aPointZ = numericFn.create("pointZ", "ptz", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aPoint = numericFn.create("point", "pt", swissArmyLocator2.aPointX, swissArmyLocator2.aPointY, swissArmyLocator2.aPointZ)

	# aArrow1Angle
	swissArmyLocator2.aArrow1Angle = unitFn.create("arrow1Angle", "a1a", OpenMaya.MFnUnitAttribute.kAngle, 0.0)

	# aArrow2Direction
	swissArmyLocator2.aArrow2DirectionX = numericFn.create("arrow2DirectionX", "a2x", OpenMaya.MFnNumericData.kDouble, 3.0)
	swissArmyLocator2.aArrow2DirectionY = numericFn.create("arrow2DirectionY", "a2y", OpenMaya.MFnNumericData.kDouble, 1.0)
	swissArmyLocator2.aArrow2DirectionZ = numericFn.create("arrow2DirectionZ", "a2z", OpenMaya.MFnNumericData.kDouble, 4.0)
	swissArmyLocator2.aArrow2Direction = numericFn.create("arrow2Direction", "dir", swissArmyLocator2.aArrow2DirectionX, swissArmyLocator2.aArrow2DirectionY, swissArmyLocator2.aArrow2DirectionZ)

	# aArrow3Angle
	# aArrow3Angle
	swissArmyLocator2.aArrow3Angle = unitFn.create("arrow3Angle", "a3a", OpenMaya.MFnUnitAttribute.kAngle, 0.0)
	# aArrow4Distance
	swissArmyLocator2.aArrow4Distance = unitFn.create("arrow2Distance", "dis", OpenMaya.MFnUnitAttribute.kDistance, 0.0)

	# aState
	swissArmyLocator2.aState = numericFn.create("state", "s", OpenMaya.MFnNumericData.kLong, 0)

	# aToggle
	swissArmyLocator2.aToggle = numericFn.create("toggle", "t", OpenMaya.MFnNumericData.kBoolean, False)

	# The in/out Manip/Plug attributes allow testing of the getConverter
	# methods from a calling script

	# aInManipDouble
	swissArmyLocator2.aInManipDouble = numericFn.create("inManipDouble", "imd", OpenMaya.MFnNumericData.kDouble, 3.14)

	# aInManipMPoint
	swissArmyLocator2.aInManipMPointX = numericFn.create("inManipMPointX", "ima", OpenMaya.MFnNumericData.kDouble, 2.0)
	swissArmyLocator2.aInManipMPointY = numericFn.create("inManipMPointY", "imb", OpenMaya.MFnNumericData.kDouble, 7.0)
	swissArmyLocator2.aInManipMPointZ = numericFn.create("inManipMPointZ", "imc", OpenMaya.MFnNumericData.kDouble, 2.0)
	swissArmyLocator2.aInManipMPoint = numericFn.create("inManipMPoint", "imp", swissArmyLocator2.aInManipMPointX, swissArmyLocator2.aInManipMPointY, swissArmyLocator2.aInManipMPointZ)

	# aInManipMVector
	swissArmyLocator2.aInManipMVectorX = numericFn.create("inManipMVectorX", "imx", OpenMaya.MFnNumericData.kDouble, 3.0)
	swissArmyLocator2.aInManipMVectorY = numericFn.create("inManipMVectorY", "imy", OpenMaya.MFnNumericData.kDouble, 1.0)
	swissArmyLocator2.aInManipMVectorZ = numericFn.create("inManipMVectorZ", "imz", OpenMaya.MFnNumericData.kDouble, 4.0)
	swissArmyLocator2.aInManipMVector = numericFn.create("inManipMVector", "imv", swissArmyLocator2.aInManipMVectorX, swissArmyLocator2.aInManipMVectorY, swissArmyLocator2.aInManipMVectorZ)

	# aInPlugDouble
	swissArmyLocator2.aInPlugDouble = numericFn.create("inPlugDouble", "ipd", OpenMaya.MFnNumericData.kDouble, 2.718)

	# aInPlugMPoint
	swissArmyLocator2.aInPlugMPointX = numericFn.create("inPlugMPointX", "ipa", OpenMaya.MFnNumericData.kDouble, 1.0)
	swissArmyLocator2.aInPlugMPointY = numericFn.create("inPlugMPointY", "ipb", OpenMaya.MFnNumericData.kDouble, 4.0)
	swissArmyLocator2.aInPlugMPointZ = numericFn.create("inPlugMPointZ", "ipc", OpenMaya.MFnNumericData.kDouble, 1.0)
	swissArmyLocator2.aInPlugMPoint = numericFn.create("inPlugMPoint", "ipp", swissArmyLocator2.aInPlugMPointX, swissArmyLocator2.aInPlugMPointY, swissArmyLocator2.aInPlugMPointZ)

	# aInPlugMVector
	swissArmyLocator2.aInPlugMVectorX = numericFn.create("inPlugMVectorX", "ipx", OpenMaya.MFnNumericData.kDouble, 1.0)
	swissArmyLocator2.aInPlugMVectorY = numericFn.create("inPlugMVectorY", "ipy", OpenMaya.MFnNumericData.kDouble, 5.0)
	swissArmyLocator2.aInPlugMVectorZ = numericFn.create("inPlugMVectorZ", "ipz", OpenMaya.MFnNumericData.kDouble, 9.0)
	swissArmyLocator2.aInPlugMVector = numericFn.create("inPlugMVector", "ipv", swissArmyLocator2.aInPlugMVectorX, swissArmyLocator2.aInPlugMVectorY, swissArmyLocator2.aInPlugMVectorZ)

	# aOutManipDouble
	swissArmyLocator2.aOutManipDouble = numericFn.create("outManipDouble", "omd", OpenMaya.MFnNumericData.kDouble, 0.0)

	# aOutManipMPoint
	swissArmyLocator2.aOutManipMPointX = numericFn.create("outManipMPointX", "oma", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutManipMPointY = numericFn.create("outManipMPointY", "omb", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutManipMPointZ = numericFn.create("outManipMPointZ", "omc", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutManipMPoint = numericFn.create("outManipMPoint", "omp", swissArmyLocator2.aOutManipMPointX, swissArmyLocator2.aOutManipMPointY, swissArmyLocator2.aOutManipMPointZ)

	# aOutManipMVector
	swissArmyLocator2.aOutManipMVectorX = numericFn.create("outManipMVectorX", "omx", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutManipMVectorY = numericFn.create("outManipMVectorY", "omy", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutManipMVectorZ = numericFn.create("outManipMVectorZ", "omz", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutManipMVector = numericFn.create("outManipMVector", "omv", swissArmyLocator2.aOutManipMVectorX, swissArmyLocator2.aOutManipMVectorY, swissArmyLocator2.aOutManipMVectorZ)

	# aOutPlugDouble
	swissArmyLocator2.aOutPlugDouble = numericFn.create("outPlugDouble", "opd", OpenMaya.MFnNumericData.kDouble, 0.0)

	# aOutPlugMPoint
	swissArmyLocator2.aOutPlugMPointX = numericFn.create("outPlugMPointX", "opa", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutPlugMPointY = numericFn.create("outPlugMPointY", "opb", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutPlugMPointZ = numericFn.create("outPlugMPointZ", "opc", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutPlugMPoint = numericFn.create("outPlugMPoint", "opp", swissArmyLocator2.aOutPlugMPointX, swissArmyLocator2.aOutPlugMPointY, swissArmyLocator2.aOutPlugMPointZ)

	# aOutPlugMVector
	swissArmyLocator2.aOutPlugMVectorX = numericFn.create("outPlugMVectorX", "opx", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutPlugMVectorY = numericFn.create("outPlugMVectorY", "opy", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutPlugMVectorZ = numericFn.create("outPlugMVectorZ", "opz", OpenMaya.MFnNumericData.kDouble, 0.0)
	swissArmyLocator2.aOutPlugMVector = numericFn.create("outPlugMVector", "opv", swissArmyLocator2.aOutPlugMVectorX, swissArmyLocator2.aOutPlugMVectorY, swissArmyLocator2.aOutPlugMVectorZ)

	swissArmyLocator2.addAttribute(swissArmyLocator2.aPoint)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aArrow1Angle)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aArrow2Direction)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aArrow3Angle)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aArrow4Distance)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aState)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aToggle)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aSize)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aInManipDouble)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aInManipMPoint)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aInManipMVector)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aInPlugDouble)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aInPlugMPoint)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aInPlugMVector)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aOutManipDouble)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aOutManipMPoint)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aOutManipMVector)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aOutPlugDouble)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aOutPlugMPoint)
	swissArmyLocator2.addAttribute(swissArmyLocator2.aOutPlugMVector)

	OpenMayaUI.MPxManipContainer.addToManipConnectTable(kSwissArmyLocator2Id)


def locatorManipCreator():
	return swissArmyLocator2Manip()


def locatorManipInit():
	OpenMayaUI.MPxManipContainer.initialize()


# initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMaya.MFnPlugin(mobject)

	try:
		mplugin.registerNode(kSwissArmyLocator2Name, 
								kSwissArmyLocator2Id,
								locatorCreator,
								locatorInit,
								OpenMaya.MPxNode.kLocatorNode)
	except:
		print ("Failed to register context command:", kSwissArmyLocator2Name)
		raise

	try:
		mplugin.registerNode(kSwissArmyLocator2ManipName, 
								kSwissArmyLocator2ManipId, 
								locatorManipCreator, 
								locatorManipInit,
								OpenMaya.MPxNode.kManipContainer)
	except:
		print ("Failed to register node:", kSwissArmyLocator2ManipName)
		raise


# uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = OpenMaya.MFnPlugin(mobject)
	OpenMayaUI.MPxManipContainer.removeFromManipConnectTable(kSwissArmyLocator2Id)

	try:
		mplugin.deregisterNode(kSwissArmyLocator2Id)
	except:
		print ("Failed to deregister context command:", kSwissArmyLocator2Name)
		raise

	try:
		mplugin.deregisterNode(kSwissArmyLocator2ManipId)
	except:
		print ("Failed to deregister node:", kSwissArmyLocator2ManipName)
		raise
