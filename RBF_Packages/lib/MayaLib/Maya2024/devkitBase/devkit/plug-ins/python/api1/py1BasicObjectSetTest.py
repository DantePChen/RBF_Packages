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
# Creation Date:   12 October 2006
#
# Description:
#	Tests out MPxObjectSet from python by creating a BasicMPxObjectSet
#	and adding and removing objects from it
#

import maya.mel
import maya.cmds as cmds

# Main entry point
#
def testBasicObjectSet():
	failures = testBasicObjectSetMain()
	if failures > 0:
		print("basicObjectSetTest: %d FAILED tests\n" % failures)
	return failures


def testBasicObjectSetMain():
	failures = 0

	# Test #1
	#
	# - Create a sphere and a cube.
	# - Create a custom MPxObjectSet and add the sphere and cube to the set.
	# - Delete the sphere.
	# - Ensure the set is still present.
	# - Delete the cube.
	# - Ensure the set is deleted.
	#
	cmds.file(f=True, new=True)
	sphere = cmds.polySphere(r=1, sx=20, sy=20, ax=(0, 1, 0), tx=1, ch=1)
	cube = cmds.polyCube(w=1, h=1, d=1, sx=1, sy=1, sz=1, ax=(0, 1, 0), tx=1, ch=1)
	cmds.select(sphere[0], cube[0])
	objSet = maya.mel.eval("spBasicObjectSetTest")

	cmds.delete(sphere[0])
	if not cmds.objExists(objSet):
		failures += 1

	cmds.delete(cube[0])
	if cmds.objExists(objSet):
		failures += 1

	if failures > 0:
		print("testBasicObjectSetMain (Test #1): FAILED\n")
		failures = 0

	# Test #2
	#
	# - Create a sphere and a cube.
	# - Create a custom MPxObjectSet and add the sphere to the set.
	# - Connect the cube.message -> set.usedBy.
	# - Delete the sphere.
	# - Ensure the set is still present.
	#
	cmds.file(f=True, new=True)
	sphere = cmds.polySphere(r=1, sx=20, sy=20, ax=(0, 1, 0), tx=1, ch=1)
	cube = cmds.polyCube(w=1, h=1, d=1, sx=1, sy=1, sz=1, ax=(0, 1, 0), tx=1, ch=1)
	cmds.select(sphere[0])
	objSet = maya.mel.eval("spBasicObjectSetTest")
	cmds.connectAttr("%s.message" % cube[0], "%s.usedBy[0]" % objSet)
	cmds.delete(sphere[0])
	if not cmds.objExists(objSet):
		failures += 1

	if failures > 0:
		print("testBasicObjectSetMain (Test #1): FAILED\n")

	# Clamp failures to 1.
	#
	if failures > 1:
		failures = 1

	return failures