#Creation Date:  (July 16, 2007)
#Author: John Creson
import maya.cmds as cmds
import maya.utils as utils
import threading
import time
import sys
from PyQt4 import QtCore, QtGui


pumpedThread = None
app = None


def pumpQt():

	global app
	def processor():
		app.processEvents()
                        

	while 1:
		time.sleep(0.01)
		utils.executeDeferred( processor )

def initializePumpThread():
	global pumpedThread
	global app
	if pumpedThread == None:
		app = QtGui.QApplication(sys.argv)
		pumpedThread = threading.Thread( target = pumpQt, args = () )
		pumpedThread.start()
	
		

# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
