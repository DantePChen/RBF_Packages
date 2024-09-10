#-
# ===========================================================================
# Copyright 2021 Autodesk, Inc.  All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
#+

########################################################################
# DESCRIPTION:
#
# Produces the Python commands "spMoveToolCmd" and "spMoveToolContext".
#
# Interactive tool for moving objects and components. 
#
# This is an example of a selection-action tool. When nothing is selected, this
# tool behaves in exactly the same way as the selection tool in Maya. Once an object
# is selected, the tool turns into a translation tool.
#
# The plug-in can translate: 
#	- transforms 
#	- NURBS curve CVs 
#	- NURBS surface CVs 
#	- polygonal vertices 
#
# This plug-in can only perform translation in orthographic views. 
# Undo, redo, and journalling are supported by this tool.  
#
# To use this plug-in, execute the following:
#
#	import maya
#	maya.cmds.loadPlugin("moveTool.py")
#	maya.cmds.spMoveToolContext("spMoveToolContext1")
#	shelfTopLevel = maya.mel.eval("global string $gShelfTopLevel;$temp = $gShelfTopLevel")
#	maya.cmds.setParent("%s|General" % shelfTopLevel)
#	maya.cmds.toolButton("spMoveTool1", cl="toolCluster", t="spMoveToolContext1", i1="moveTool.xpm") 
#	# Remove UI objects with
#	maya.cmds.deleteUI("spMoveToolContext1")
#	maya.cmds.deleteUI("spMoveTool1")
#
# This creates a new entry in the "Shelf1" tab of the tool shelf called "moveTool". Click the
# new icon, then select an object and drag it around in an orthographic view. The left mouse
# button allows movement in two directions, while the middle mouse button constrains the movement
# to a single direction. 
#
# Note that you must have a Shelf1 tab before executing the commands. 
#
########################################################################

from builtins import next
import maya.api.OpenMaya as om
import maya.api.OpenMayaUI as omui
import sys, math

# tell Maya that we want to use Python API 2.0
maya_useNewAPI = True

kPluginCmdName="spMoveToolCmd"
kPluginCtxName="spMoveToolContext"
kVectorEpsilon = 1.0e-3
kZNegAxis = ( 0, 0,-1)

# command
class MoveToolCmd(omui.MPxToolCommand):
	kDoIt, kUndoIt, kRedoIt = 0, 1, 2

	def __init__(self):
		omui.MPxToolCommand.__init__(self)
		self.commandString = kPluginCmdName
		self.__delta = om.MVector()

	def doIt(self, args):
		argData = om.MArgDatabase(self.syntax, args)
		vector = om.MVector(1.0, 0.0, 0.0)
		if args.length() == 1:
			vector.x = args.asDouble(0)
		elif args.length == 2:
			vector.x = args.asDouble(0)
			vector.y = args.asDouble(1)
		elif args.length == 3:
			vector.x = args.asDouble(0)
			vector.y = args.asDouble(1)
			vector.z = args.asDouble(2)
		self.__delta = vector
		self.__action(MoveToolCmd.kDoIt)

	def redoIt(self):
		self.__action(MoveToolCmd.kRedoIt)

	def undoIt(self):
		self.__action(MoveToolCmd.kUndoIt)

	def isUndoable(self):
		return True

	def finalize(self):
		"""
		Command is finished, construct a string for the command
		for journalling.
		"""
		command = om.MArgList()
		command.addArg(self.commandString)
		command.addArg(self.__delta.x)
		command.addArg(self.__delta.y)
		command.addArg(self.__delta.z)

		# This call adds the command to the undo queue and sets
		# the journal string for the command.
		#
		try:
			omui.MPxToolCommand.doFinalize(self, command)
		except:
			pass

	def setVector(self, x, y, z):
		self.__delta.x = x
		self.__delta.y = y
		self.__delta.z = z

	def __action(self, flag):
		"""
		Do the actual work here to move the objects	by vector
		"""
		if flag == MoveToolCmd.kUndoIt:
			vector = -self.__delta
		else:
			vector = self.__delta

		# Create a selection list iterator
		#
		slist = om.MGlobal.getActiveSelectionList()
		sIter = om.MItSelectionList(slist)

		mdagPath = om.MDagPath()
		mComponent = om.MObject()
		spc = om.MSpace.kWorld

		# Translate all selected objects
		#
		while not sIter.isDone():
			# Get path and possibly a component
			#
			mdagPath = sIter.getDagPath()
			mComponent = sIter.getComponent()
			try:
				transFn = om.MFnTransform(mdagPath)
			except:
				pass
			else:
				try:
					transFn.translateBy(vector, spc)
				except:
					sys.stderr.write("Error doing translate on transform\n")
				next(sIter)
				continue

			try:
				cvFn = om.MItCurveCV(mdagPath, mComponent)
			except:
				pass
			else:
				while not cvFn.isDone():
					cvFn.translateBy(vector, spc)
					next(cvFn)
				cvFn.updateCurve()

			try:
				sCvFn = om.MItSurfaceCV(mdagPath, mComponent, True)
			except:
				pass

			else:
				while not sCvFn.isDone():
					while not CvFn.isRowDone():
						sCvFn.translateBy(vector, spc)
						next(sCvFn)
					sCvFn.nextRow()
				sCvFn.updateSurface()

			try:
				vtxFn = om.MItMeshVertex(mdagPath, mComponent)
			except:
				pass
			else:
				while not vtxFn.isDone():
					vtxFn.translateBy(vector, spc)
					next(vtxFn)
				vtxFn.updateSurface()

			next(sIter)


class MoveContext(omui.MPxSelectionContext):
	kTop, kFront, kSide, kPersp = 0, 1, 2, 3
	
	def __init__(self):
		omui.MPxSelectionContext.__init__(self)
		self.setTitleString("moveTool")
		self.setImage("moveTool.xpm", omui.MPxContext.kImage1)
		self.__currWin = 0
		self.__view = omui.M3dView()
		self.__startPos_x = 0
		self.__endPos_x = 0
		self.__startPos_y = 0
		self.__endPos_y = 0
		self.__cmd = None

	def toolOnSetup(self, event):
		self.setHelpString("drag to move selected object")

	def doPress(self, event, drawMgr, context):
		omui.MPxSelectionContext.doPress(self, event, drawMgr, context)
		spc = om.MSpace.kWorld
		
		# If we are not in selecting mode (i.e. an object has been selected)
		# then set up for the translation.
		#
		if not self.isSelecting():
			self.__startPos_x, self.__startPos_y = event.position
			self.__view = omui.M3dView.active3dView()

			camera = self.__view.getCamera()
			fnCamera = om.MFnCamera(camera)
			upDir = fnCamera.upDirection(spc)
			rightDir = fnCamera.rightDirection(spc)

			# Determine the camera used in the current view
			#
			if fnCamera.isOrtho():
				if upDir.isEquivalent(om.MVector.kZnegAxisVector, kVectorEpsilon):
					self.__currWin = MoveContext.kTop
				elif rightDir.isEquivalent(om.MVector.kXaxisVector, kVectorEpsilon):
					self.__currWin = MoveContext.kFront
				else:
					self.__currWin = MoveContext.kSide
			else:
				om.MGlobal.displayWarning('moveTool only works in top, front and side views')
				self.__currWin = MoveContext.kPersp

			# Create an instance of the move tool command.
			#		
			self.__cmd = MoveToolCmd()
			self.__cmd.setVector(0.0, 0.0, 0.0)

	def doDrag(self, event, drawMgr, context):
		omui.MPxSelectionContext.doDrag(self, event, drawMgr, context)

		# If we are not in selecting mode (i.e. an object has been selected)
		# then do the translation.
		#

		if not self.isSelecting():
			self.__endPos_x, self.__endPos_y = event.position

			startW = om.MPoint()
			endW = om.MPoint()
			vec = om.MVector()
			self.__view.viewToWorld(self.__startPos_x, self.__startPos_y, startW, vec)
			self.__view.viewToWorld(self.__endPos_x, self.__endPos_y, endW, vec)
			downButton = event.mouseButton()

			# We reset the the move vector each time a drag event occurs
			# and then recalculate it based on the start position.
			#
			self.__cmd.undoIt()
			if self.__currWin == MoveContext.kTop:
				if downButton == omui.MEvent.kMiddleMouse:
					self.__cmd.setVector(endW.x - startW.x, 0.0, 0.0)
				else:
					self.__cmd.setVector(endW.x - startW.x, 0.0, endW.z - startW.z)

			elif self.__currWin == MoveContext.kFront:
				if downButton == omui.MEvent.kMiddleMouse:

					self.__cmd.setVector(endW.x - startW.x, 0.0, 0.0)

				else:

					self.__cmd.setVector(endW.x - startW.x, endW.y - startW.y, 0.0)

			elif self.__currWin == MoveContext.kSide:
				if downButton == omui.MEvent.kMiddleMouse:
					self.__cmd.setVector(0.0, 0.0, endW.z - startW.z)
				else:
					self.__cmd.setVector(0.0, endW.y - startW.y, endW.z - startW.z)

			self.__cmd.redoIt()
			self.__view.refresh(True)

	def doRelease(self, event, drawMgr, context):
		omui.MPxSelectionContext.doRelease(self, event, drawMgr, context) 
		if not self.isSelecting():
			self.__endPos_x, self.__endPos_y = event.position

			# Delete the move command if we have moved less then 2 pixels
			# otherwise call finalize to set up the journal and add the
			# command to the undo queue.

			#
			if (math.fabs(self.__startPos_x - self.__endPos_x) < 2 and 
					math.fabs(self.__startPos_y - self.__endPos_y) < 2):
				self.__cmd = None
				self.__view.refresh(True)
			else:
				self.__cmd.finalize()
				self.__view.refresh(True)

	def doEnterRegion(self, event):
		"""
		Print the tool description in the help line.
		"""
		self.setHelpString("click on object and drag to move it")


#############################################################################


class MoveContextCommand(omui.MPxContextCommand):
	def __init__(self):
		omui.MPxContextCommand.__init__(self)

	def makeObj(self):
		return MoveContext()

def cmdCreator():
	return MoveToolCmd()

def ctxCmdCreator():
	return MoveContextCommand()

def syntaxCreator():
	syntax = om.MSyntax()
	syntax.addArg(om.MSyntax.kDouble)
	syntax.addArg(om.MSyntax.kDouble)
	syntax.addArg(om.MSyntax.kDouble)
	return syntax

# Initialize the script plug-in

def initializePlugin(mobject):
	mplugin = om.MFnPlugin(mobject, "Autodesk", "1.0", "Any")
	try:
		mplugin.registerCommand(kPluginCmdName, cmdCreator, syntaxCreator)
		mplugin.registerContextCommand(kPluginCtxName, ctxCmdCreator)
	except:
		sys.stderr.write("Failed to register context command: %s\n" % kPluginCtxName)
		raise

# Uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = om.MFnPlugin(mobject)
	try:
		mplugin.deregisterCommand(kPluginCmdName)
		mplugin.deregisterContextCommand(kPluginCtxName)
	except:
		sys.stderr.write("Failed to deregister context command: %s\n" % kPluginCtxName)
		raise

