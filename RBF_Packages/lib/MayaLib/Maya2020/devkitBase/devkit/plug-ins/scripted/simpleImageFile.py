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
# Produces a simple image file plug-in.
#
# This plug-in registers a new image file format against file extension ".moo".
# Loading any ".moo" image file will produce a procedurally generated colour
# spectrum including values outside of 0 to 1.
#
# Usage:
#	(1) Run the script:
#
#	import maya.cmds
#	maya.cmds.loadPlugin("simpleImageFile.py")
#	
#	(2) Perform the following:
#		
#		- Create a poly plane.
#		- Assign a shader to it.
#		- Assign a file texture to the shader.
#		- Make a copy of a project/image file and name it test.moo.
#		- Assign the test.moo image to the file texture.
#		- Turn on hardware texturing.
#		- Render the scene.
#
########################################################################

import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx
import maya.OpenMayaRender as OpenMayaRender

import sys

kImagePluginName = "spSimpleImageFile"

# Use MGLFunctionTable to make openGL calls
glRenderer = OpenMayaRender.MHardwareRenderer.theRenderer()
glFT = glRenderer.glFunctionTable()

		
# Class definition
class SimpleImageFile(OpenMayaMPx.MPxImageFile):
	
	# init
	def __init__(self):
		OpenMayaMPx.MPxImageFile.__init__(self)
	
	#	
	# DESCRIPTION:
	#	Configure the image characteristics. A real image file
	#	format plugin would extract these values from the image
	#	file header.
	#
	#################################################################
	def open( self, pathname, info ):
		
		if info is not None:
			info.width( 512 )
			info.height( 512 )
			info.channels( 3 )
			info.pixelType( OpenMaya.MImage.kFloat )

			# Only necessary if your format defines a native
			# hardware texture loader
			info.hardwareType( OpenMaya.MImageFileInfo.kHwTexture2D)		
		
	#
	# DESCRIPTION:
	# Load the contents of this image file into an MImage. A real
	# file format plugin would extract the pixel data from the image
	# file here.
	#
	#################################################################
	def load( self, image, idx ):
		width = 512
		height = 512
	
		# Create a floating point image and fill it with
		# a pretty rainbow test image.
		#	
		image.create( width, height, 3, OpenMaya.MImage.kFloat )
		self.populateTestImage( image.floatPixels(), width, height )		
		
	#
	# DESCRIPTION:
	# Load the contents of this image file into an OpenGL texture. A 
	# real file format plugin would extract the pixel data from the 
	# image file here.
	#
	#################################################################
	def glLoad( self, info, imageNumber ):
		w = info.width()
		h = info.height()

		# Create a floating point image and fill it with
		# a pretty rainbow test image.
		#
		image = OpenMaya.MImage()
		image.create( w, h, 3, OpenMaya.MImage.kFloat )
		self.populateTestImage( image.floatPixels(), w, h )
	
		# Now load it into OpenGL as a floating point image
		glFT.glTexImage2D( OpenMayaRender.MGL_TEXTURE_2D, 0, \
			OpenMayaRender.MGL_RGB, w, h, 0, OpenMayaRender.MGL_RGB, \
			OpenMayaRender.MGL_FLOAT, pixelsPtr )


	#
	# DESCRIPTION:
	# Helper method to populate our procedural test image
	#
	#################################################################
	def populateTestImage( self, pixels, w, h ):
		#
		rainbowScale = 4.0
		index = 0
		for y in range( 0, h ):
			g = rainbowScale * y / float(h)			
			for x in range( 0, w ):
				r = rainbowScale * x / float(w)
				OpenMaya.MScriptUtil.setFloatArray( pixels, index, r )
				index+=1
				OpenMaya.MScriptUtil.setFloatArray( pixels, index, g )
				index+=1
				b = rainbowScale * 1.5 - r - g
				OpenMaya.MScriptUtil.setFloatArray( pixels, index, b )
				index+=1
				
# creator
def creator():
	return OpenMayaMPx.asMPxPtr( SimpleImageFile() )	


# initialize plugin
def initializePlugin( mObject ):
	mPlugin = OpenMayaMPx.MFnPlugin(mObject, "Autodesk", "1.0", "Any")	
	#
	extensions = ["moo"]	
	try:
		mPlugin.registerImageFile( kImagePluginName, creator, extensions )
	except:
		sys.stderr.write( "Failed to register image plugin: %s" % kImagePluginName )
		raise
		
# uninitialize plugin
def uninitializePlugin( mObject ):
	mPlugin = OpenMayaMPx.MFnPlugin( mObject )
	try:
		mPlugin.deregisterImageFile( kImagePluginName )
	except:
		sys.stderr.write( "Failed to deregister image: %s" % kImagePluginName )
		raise
