#Creation Date:  (July 16, 2007)
#Author: John Creson

import sys
# Change this append so it points to the correct path to the example's dockwidgets folder.

sys.path.append("C:/Program Files (x86)/PyQt4/examples/mainwindows/dockwidgets")


from PyQt4 import QtCore, QtGui
import dockwidgets
import pumpThread as pt

dockwindow = None






def dockwidgetsCall():
    global app
    global dockwindow
    pt.initializePumpThread()
    app = QtGui.qApp
    dockwindow = dockwidgets.MainWindow()
    dockwindow.show()
# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
