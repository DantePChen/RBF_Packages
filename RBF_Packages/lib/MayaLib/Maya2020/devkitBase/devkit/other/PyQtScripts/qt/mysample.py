#Creation Date:  (July 16, 2007)
#Author: John Creson
import sys
from PyQt4 import QtCore, QtGui
from sample import Ui_Form
import pumpThread as pt


dialog=None

class MyDialog(QtGui.QDialog,Ui_Form):
	
	def __init__(self):
		QtGui.QDialog.__init__(self)
		self.setupUi( self )

def mysample():
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
