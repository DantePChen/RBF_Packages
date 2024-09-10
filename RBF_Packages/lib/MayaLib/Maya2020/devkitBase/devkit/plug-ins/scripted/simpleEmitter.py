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
# Produces the dependency graph node "simpleEmitter".
#
# This node is an example of a particle emitter that emits in a direction
# from a single position.
#
########################################################################

import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaRender as OpenMayaRender
import math
import random
import sys

kSimpleEmitterNodeName = "spSimpleEmitter"
kSimpleEmitterNodeID = OpenMaya.MTypeId(0x8700F)

SEGMENTS = 20
EDGES = 30
TORUS_PI = 3.14159265
TORUS_2PI = 2.0 * TORUS_PI

glRenderer = OpenMayaRender.MHardwareRenderer.theRenderer()
glFT = glRenderer.glFunctionTable()

class simpleEmitter(OpenMayaMPx.MPxEmitterNode):
	def __init__(self):
		OpenMayaMPx.MPxEmitterNode.__init__(self)

	def isFullValue( self, plugIndex, block ):
		value = 1
		mIsFull = OpenMayaMPx.cvar.MPxEmitterNode_mIsFull

		try:
			mhValue = block.inputArrayValue( mIsFull )
			mhValue.jumpToElement( plugIndex )
			hValue = mhValue.inputValue( )
			value = hValue.asBool()
		except:
			sys.stderr.write("Error getting the input array value\n")
			raise
		return value
					
	
	def getWorldPosition( self, point ):
		try:
			thisNode = simpleEmitter.thisMObject( self )
			fnThisNode = OpenMaya.MFnDependencyNode(thisNode)

			worldMatrixAttr = fnThisNode.attribute( "worldMatrix" )

			matrixPlug = OpenMaya.MPlug( thisNode, worldMatrixAttr )
			matrixPlug = matrixPlug.elementByLogicalIndex( 0 )

			matrixObject = OpenMaya.MObject()
			matrixObject = matrixPlug.asMObject(  )

			worldMatrixData = OpenMaya.MFnMatrixData( matrixObject )
			worldMatrix = worldMatrixData.matrix( )

			point.x = worldMatrix( 3, 0 )
			point.y = worldMatrix( 3, 1 )
			point.z = worldMatrix( 3, 2 )
		except:
			sys.stderr.write( "Error in getWorldPosition\n" )
			raise

	def currentTimeValue( self, block ):
		try:
			mCurrentTime = OpenMayaMPx.cvar.MPxEmitterNode_mCurrentTime
			hValue = block.inputValue( mCurrentTime )
			value = hValue.asTime()
		except:
			sys.stderr.write("Error getting current time value, returning 0")
			value = OpenMaya.MTime(0.0)
		return value

	def startTimeValue( self, plugIndex, block ):
		try:
			mStartTime = OpenMayaMPx.cvar.MPxEmitterNode_mStartTime
			mhValue = block.inputArrayValue( mStartTime )
			mhValue.jumpToElement( plugIndex )
			hValue = mhValue.inputValue( )
			value = hValue.asTime( )		
		except:
			sys.stderr.write("Error getting start time value, setting to 0")
			value = OpenMaya.MTime(0.0)
		return value

	def deltaTimeValue( self, plugIndex, block ):
		try:
			mDeltaTime = OpenMayaMPx.cvar.MPxEmitterNode_mDeltaTime

			mhValue = block.inputArrayValue( mDeltaTime )
			mhValue.jumpToElement( plugIndex )

			hValue = mhValue.inputValue()
			value = hValue.asTime()
		except:
			sys.stderr.write("Error getting delta time value, setting to 0\n")
			value = OpenMaya.MTime(0.0)
		return value

	def rateValue( self, block ):
		try:
			mRate = OpenMayaMPx.cvar.MPxEmitterNode_mRate
			hValue = block.inputValue( mRate )
			value = hValue.asDouble()		
		except:
			sys.stderr.write("Error getting rate value, setting to 0\n")
			value = 0.0
		return value

	def directionValue( self, block ):
		try:
			mDirection = OpenMayaMPx.cvar.MPxEmitterNode_mDirection
			hValue = block.inputValue( mDirection )
			value = hValue.asDouble3()
			dirV = OpenMaya.MVector(value[0], value[1], value[2])
		except:
			sys.stderr.write("Error getting direction value, setting to 0,0,0\n")
			dirV = OpenMaya.MVector(0.0, 0.0, 0.0)
		return dirV

	def speedValue( self, block ):
		try:
			mSpeed = OpenMayaMPx.cvar.MPxEmitterNode_mSpeed
			hValue = block.inputValue( mSpeed )
			value = hValue.asDouble()		
		except:
			sys.stderr.write("Error getting speed value, setting to 0\n")
			value = 0.0
		return value

	def inheritFactorValue( self, plugIndex, block ):
		try:
			mInheritFactor = OpenMayaMPx.cvar.MPxEmitterNode_mInheritFactor
			mhValue = block.inputArrayValue( mInheritFactor )
			mhValue.jumpToElement( plugIndex )
			hValue = mhValue.inputValue( )
			value = hValue.asDouble()		
		except:
			sys.stderr.write("Error getting inherit factor value, setting to 0\n")
			value = 0.0
		return value

	def useRotation( self, direction ):
		try:
			thisNode = simpleEmitter.thisMObject(self)
			fnThisNode = OpenMaya.MFnDependencyNode( thisNode )

			worldMatrixAttr = fnThisNode.attribute( "worldMatrix" )

			matrixPlug = OpenMaya.MPlug( thisNode, worldMatrixAttr )
			matrixPlug = matrixPlug.elementByLogicalIndex( 0 )

			matrixObject = OpenMaya.MObject()
			matrixObject = matrixPlug.asMObject( )

			worldMatrixData = OpenMaya.MFnMatrixData( matrixObject )
			worldMatrix = worldMatrixData.matrix( )

			rotatedVector = OpenMaya.MVector()
			rotatedVector = direction * worldMatrix
		except:
			sys.stderr.write("Error getting rotation value, setting to 0,0,0\n")
			rotatedVector = OpenMaya.MVector(0.0, 0.0, 0.0)
		return rotatedVector


	def compute(self, plug, block):
		mOutput = OpenMayaMPx.cvar.MPxEmitterNode_mOutput

		# Determine if we are requesting the output plug for this emitter node.
		#
		if plug == mOutput:
			# Get the logical index of the element this plug refers to,
			# because the node can be emitting particles into more than
			# one particle shape.
			#
			try:
				multiIndex = plug.logicalIndex( )

				# Get output data arrays (position, velocity, or parentId)
				# that the particle shape is holding from the previous frame.
				#
				hOutArray = block.outputArrayValue ( mOutput )

				# Create a builder to aid in the array construction efficiently.
				#
				bOutArray = hOutArray.builder( )

				# Get the appropriate data array that is being currently evaluated.
				#
				hOut = bOutArray.addElement( multiIndex )

				# Create the data and apply the function set,
				# particle array initialized to length zero, 
				# fnOutput.clear()
				#
				fnOutput = OpenMaya.MFnArrayAttrsData()
				dOutput = fnOutput.create( )

				# Check if the particle object has reached it's maximum,
				# hence is full. If it is full then just return with zero particles.
				#
				beenFull = simpleEmitter.isFullValue( self, multiIndex, block )
				if beenFull == 1:
					return

				# Get input position and velocity arrays where new particles are from,
				# also known as the owner. An owner is determined if connections exist
				# to the emitter node from a shape such as nurbs, polymesh, curve, 
				# or a lattice shape.
				#
				
				# Get a single position from world transform
				#
				inPosAry = OpenMaya.MVectorArray()
				inPosAry.clear()
			
				worldPos = OpenMaya.MPoint(0.0, 0.0, 0.0)
				simpleEmitter.getWorldPosition( self, worldPos )

				worldV = OpenMaya.MVector(worldPos[0], worldPos[1], worldPos[2])
				inPosAry.append( worldV )

				# Create a single velocity			
				inVelAry = OpenMaya.MVectorArray()
				inVelAry.clear()
				velocity = OpenMaya.MVector(0, 0, 0)
				inVelAry.append( velocity )

				# Get deltaTime, currentTime and startTime.
				# If deltaTime <= 0.0, or currentTime <= startTime,
				# do not emit new pariticles and return.
				#
				cT = simpleEmitter.currentTimeValue( self, block )
				sT = simpleEmitter.startTimeValue( self, multiIndex, block )
				dT = simpleEmitter.deltaTimeValue( self, multiIndex, block )

				dTValue = dT.value()
			
				if cT <= sT or dTValue <= 0.0:
					# We do not emit particles before the start time, 
					# and do not emit particles when moving backwards in time.
					# 

					# This code is necessary primarily the first time to 
					# establish the new data arrays allocated, and since we have 
					# already set the data array to length zero it does 
					# not generate any new particles.
					# 
					hOut.setMObject( dOutput )
					block.setClean( plug )
					return

				# Compute and store an emission rate
				#
				emitCountPP = OpenMaya.MIntArray()
				emitCountPP.clear()

				plugIndex = plug.logicalIndex( )

				# Get rate and delta time.
				#
				rate = simpleEmitter.rateValue( self, block )
				dtRate = simpleEmitter.deltaTimeValue( self, plugIndex, block )
				dtRateDbl = dtRate.asUnits( OpenMaya.MTime.kSeconds )
				dblCount = rate * dtRateDbl
				intCount = int(dblCount)
				emitCountPP.append( intCount )

				# Get speed, direction vector, and inheritFactor attributes.
				#			
				speed = simpleEmitter.speedValue( self, block )
				dirV = simpleEmitter.directionValue( self, block )
				inheritFactor = simpleEmitter.inheritFactorValue( self, multiIndex, block )

				# Get the position, velocity, and normalized time arrays to append new particle data.
				#
				fnOutPos = fnOutput.vectorArray( "position" )
				fnOutVel = fnOutput.vectorArray( "velocity" )
				fnOutTime = fnOutput.doubleArray( "timeInStep" )

				# Convert deltaTime into seconds.
				#			
				dt = dT.asUnits( OpenMaya.MTime.kSeconds )

				# Rotate the direction attribute by world transform
				rotatedV = simpleEmitter.useRotation( self, dirV )

				# Start emitting particles.
				#
				simpleEmitter.emit( self, inPosAry, inVelAry, emitCountPP, dt, speed, inheritFactor,\
						rotatedV, fnOutPos, fnOutVel, fnOutTime)

				# Update the data block with new dOutput and set plug clean.
				#	
				# sys.__stdout__.write( " handle: " + str(hOut) + "\n" )		
				hOut.setMObject( dOutput )
				block.setClean( plug )
			except:
				sys.stderr.write("simpleEmitter compute error\n")
				raise
		else:
			return OpenMaya.kUnknownParameter

	def emit( self, inPosAry, inVelAry, emitCountPP, dt, speed, inheritFactor, dirV, outPosAry, outVelAry, outTimeAry):
		try:
			posLength = inPosAry.length()
			velLength = inVelAry.length()
			countLength = emitCountPP.length()

			if not posLength == velLength or not posLength == countLength:
				return

			totalCount = 0
			for index in range(countLength):
				totalCount += emitCountPP[index]
			if totalCount <= 0:
				return

			dirV.normalize()
			
			for index in range(posLength):
				emitCount = emitCountPP[index]
				if emitCount <= 0:
					continue
			
				sPos = inPosAry[index]
				sVel = inVelAry[index]
				prePos = sPos - sVel * dt

				for i in range(emitCount):
					alpha = ( float(i) + random.random() ) / float(emitCount)
					newPos = prePos * (1.0 - alpha) + sPos * alpha
					newVel = dirV * speed

					newPos += newVel * ( dt * (1.0 - alpha) )
					newVel += sVel * inheritFactor

					# Add new data into output arrays.
					#
					outPosAry.append( newPos )
					outVelAry.append( newVel )
					outTimeAry.append( alpha )
		except Exception, e:
			sys.stderr.write( "Error in simpleEmitter.emit\n" )
			raise

	def draw( self, view, path, style, status):
		view.beginGL()

		for j in range(0, SEGMENTS):
			glFT.glPushMatrix()
			glFT.glRotatef(float(360.0 * j/SEGMENTS), 0.0, 1.0, 0.0)
			glFT.glTranslatef( 1.5, 0.0, 0.0)
			
			for i in range(0, EDGES):
				glFT.glBegin(OpenMayaRender.MGL_LINE_STRIP)

				p0 = float(TORUS_2PI * i/EDGES)
				p1 = float(TORUS_2PI * (i+1)/EDGES)
				glFT.glVertex2f(math.cos(p0), math.sin(p0))
				glFT.glVertex2f(math.cos(p1), math.sin(p1))
				
				glFT.glEnd()

			glFT.glPopMatrix()

		view.endGL()


def nodeCreator():
	return OpenMayaMPx.asMPxPtr( simpleEmitter() )

def nodeInitializer():
	return

# initialize the script plug-in
def initializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)

	try:
		mplugin.registerNode( kSimpleEmitterNodeName, kSimpleEmitterNodeID, \
								nodeCreator, nodeInitializer, OpenMayaMPx.MPxNode.kEmitterNode )
	except:
		sys.stderr.write( "Failed to register node: %s\n" % kSimpleEmitterNodeName )
		raise

# uninitialize the script plug-in
def uninitializePlugin(mobject):
	mplugin = OpenMayaMPx.MFnPlugin(mobject)

	try:
		mplugin.deregisterNode( kSimpleEmitterNodeID )
	except:
		sys.stderr.write( "Failed to unregister node: %s\n" % kSimpleEmitterNodeName )
		raise	
