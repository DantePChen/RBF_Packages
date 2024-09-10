#Creation Date:  (July 16, 2007)
#Author: John Creson
import maya.cmds as cmds
import maya.utils as utils
import sys
from PyQt4 import QtCore, QtGui
from frmConnect import Ui_Dialog
import pumpThread as pt

newDialog = None

class MyDialog(QtGui.QDialog,Ui_Dialog):
	
	def __init__(self):
		QtGui.QDialog.__init__(self)
		self.setupUi( self )
		self.connect(self.pushButton, QtCore.SIGNAL("clicked()"), self.HI)
	def HI(self):
                cmds.sphere()
		


def mysampleStripped2():
    global app
    global newDialog
    pt.initializePumpThread()
    app=QtGui.qApp
    newDialog = MyDialog()
    newDialog.show()
    app.connect(app, QtCore.SIGNAL("lastWindowClosed()"),
                            app, QtCore.SLOT("quit()"))


# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
