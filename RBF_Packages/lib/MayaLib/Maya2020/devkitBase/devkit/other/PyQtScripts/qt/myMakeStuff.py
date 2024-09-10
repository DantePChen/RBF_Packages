#Creation Date:  (July 16, 2007)
#Author: John Creson
import sys
from PyQt4 import QtCore, QtGui
from makeStuff import Ui_Dialog
import maya.cmds as cmds
import pumpThread as pt

dialog=None

class MyDialog(QtGui.QDialog,Ui_Dialog):
	
	def __init__(self):
		QtGui.QDialog.__init__(self)
		self.setupUi( self )
		self.connect(self.clickMake, QtCore.SIGNAL("clicked()"),
                            self.HI)

	def HI(self):
            if self.checkSphere.isChecked():
                cmds.polySphere()
            if self.checkTorus.isChecked():
                cmds.polyTorus()
            if self.checkCube.isChecked():
                cmds.polyCube()
            pass
            
def myMakeStuff():
    global app
    global dialog
    pt.initializePumpThread()
    app=QtGui.qApp
    dialog = MyDialog()
    dialog.show()
    
    app.connect(app, QtCore.SIGNAL("lastWindowClosed()"),
                            app, QtCore.SLOT("quit()"))
	
# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
