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

#
# Creation Date:   4 October 2006
#

########################################################################
# DESCRIPTION:
# 
# Registers the IK solver "simpleSolverNode".
# It is a single-bone, single-plane IK solver.
#
# This plug-in demonstrates how to create and register an IK solver.
# Due to the complex nature of IK solvers, this plug-in only 
# works with 2-joint skeletons (1 bone) in the x-y plane.
#
# To use the solver, create a single IK bone (joint tool) with two joints by
# selecting Skeleton > Joint Tool in the Animation menu set. Then, enter the
# following commands in the command window to create an IK handle, which uses
# the new solver:  
#
#	import maya.cmds as cmds
#	cmds.createNode("spSimpleSolverNode", name="spSimpleSolverNode1")
#
# This creates a handle that can be dragged around in the x-y plane to rotate the joint.
#
# The following command can be used to determine which solver a handle is using:
#
#	cmds.ikHandle(sol="spSimpleSolverNode1", sj="joint1", ee="joint2")
#
########################################################################

# imports
import maya.OpenMaya as OpenMaya
import maya.OpenMayaUI as OpenMayaUI
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaAnim as OpenMayaAnim
import math, sys

# consts
kSolverNodeName = "spSimpleSolverNode"
kSolverNodeId = OpenMaya.MTypeId(0x00080051)


class simpleSolverNode(OpenMayaMPx.MPxIkSolverNode):
	def __init__(self):
		OpenMayaMPx.MPxIkSolverNode.__init__(self)


	def solverTypeName(self):
		return kSolverNodeName

	
	def doSolve(self):
		self.doSimpleSolver()


	def doSimpleSolver(self):
		"""
		Solve single joint in the x-y plane
			- first it calculates the angle between the handle and the end-effector.
			- then it determines which way to rotate the joint.
		"""
		handle_group = self.handleGroup()
		handle = handle_group.handle(0)
		handlePath = OpenMaya.MDagPath.getAPathTo(handle)
		fnHandle = OpenMayaAnim.MFnIkHandle(handlePath)

		# Get the position of the end_effector
		end_effector = OpenMaya.MDagPath()
		fnHandle.getEffector(end_effector)
		tran = OpenMaya.MFnTransform(end_effector)
		effector_position = tran.rotatePivot(OpenMaya.MSpace.kWorld)

		# Get the position of the handle
		handle_position = fnHandle.rotatePivot(OpenMaya.MSpace.kWorld)

		# Get the start joint position
		start_joint = OpenMaya.MDagPath()
		fnHandle.getStartJoint(start_joint)
		start_transform = OpenMaya.MFnTransform(start_joint)
		start_position = start_transform.rotatePivot(OpenMaya.MSpace.kWorld)

		# Calculate the rotation angle
		v1 = start_position - effector_position
		v2 = start_position - handle_position
		angle = v1.angle(v2)

		# -------- Figure out which way to rotate --------
		#
		#  define two vectors U and V as follows
		#  U   =   EndEffector(E) - StartJoint(S)
		#  N   =   Normal to U passing through EndEffector
		#
		#  Clip handle_position to half-plane U to determine the region it
		#  lies in. Use the region to determine  the rotation direction.
		#
		#             U
		#             ^              Region      Rotation
		#             |  B           
		#            (E)---N            A          C-C-W
		#         A   |                 B           C-W
		#             |  B
		#             |
		#            (S)
		#

		rot = 0.0	# Rotation about Z-axis

		# U and N define a half-plane to clip the handle against
		U = effector_position - start_position
		U.normalize()

		# Get a normal to U
		zAxis = OpenMaya.MVector(0.0, 0.0, 1.0)
		N = U ^ zAxis # Cross product
		N.normalize()

		# P is the handle position vector
		P = handle_position - effector_position

		# Determine the rotation direction
		PdotN = P[0]*N[0] + P[1]*N[1]
		if PdotN < 0:
			rot = angle # counter-clockwise
		else:
			rot = -1.0 * angle	# clockwise

		# get and set the Joint Angles 
		jointAngles = OpenMaya.MDoubleArray()
		try:
			self._getJointAngles(jointAngles)
		except:
			# getting angles failed, do nothing
			pass
		else:
			jointAngles.set(jointAngles[0] + rot, 0)
			self._setJointAngles(jointAngles)


##############################################################################


def nodeCreator():
	return OpenMayaMPx.asMPxPtr(simpleSolverNode())


def nodeInitializer():
	# nothing to initialize
	pass


# initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject, "Autodesk", "1.0", "Any")

	try:
		mplugin.registerNode(kSolverNodeName, kSolverNodeId, nodeCreator, nodeInitializer, OpenMayaMPx.MPxNode.kIkSolverNode)
	except:
		sys.stderr.write("Failed to register node: %s" % kSolverNodeName)
		raise


# uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)
	try:
		mplugin.deregisterNode(kSolverNodeId)
	except:
		sys.stderr.write("Failed to unregister node: %s" % kSolverNodeName)
		raise
