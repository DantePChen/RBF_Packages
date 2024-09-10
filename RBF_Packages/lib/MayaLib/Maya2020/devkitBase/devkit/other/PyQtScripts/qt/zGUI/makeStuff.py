# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'C:\Documents and Settings\John Creson\My Documents\maya\QtUi\makeStuff.ui'
#
# Created: Fri Feb 23 23:26:21 2007
#      by: PyQt4 UI code generator 4.1.1
#
# WARNING! All changes made in this file will be lost!

import sys
from PyQt4 import QtCore, QtGui

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(QtCore.QSize(QtCore.QRect(0,0,200,137).size()).expandedTo(Dialog.minimumSizeHint()))

        self.clickMake = QtGui.QPushButton(Dialog)
        self.clickMake.setGeometry(QtCore.QRect(100,30,61,71))
        self.clickMake.setObjectName("clickMake")

        self.checkSphere = QtGui.QCheckBox(Dialog)
        self.checkSphere.setGeometry(QtCore.QRect(20,30,71,18))
        self.checkSphere.setObjectName("checkSphere")

        self.checkTorus = QtGui.QCheckBox(Dialog)
        self.checkTorus.setGeometry(QtCore.QRect(20,50,71,18))
        self.checkTorus.setObjectName("checkTorus")

        self.checkCube = QtGui.QCheckBox(Dialog)
        self.checkCube.setGeometry(QtCore.QRect(20,70,71,18))
        self.checkCube.setObjectName("checkCube")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QtGui.QApplication.translate("Dialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.clickMake.setToolTip(QtGui.QApplication.translate("Dialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Choose your objects and push the Make button.</p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.clickMake.setText(QtGui.QApplication.translate("Dialog", "Make", None, QtGui.QApplication.UnicodeUTF8))
        self.checkSphere.setToolTip(QtGui.QApplication.translate("Dialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Check to choose Sphere to Make.</p>\n"
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.checkSphere.setText(QtGui.QApplication.translate("Dialog", "sphere", None, QtGui.QApplication.UnicodeUTF8))
        self.checkTorus.setToolTip(QtGui.QApplication.translate("Dialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Check to choose Torus to Make.</p>\n"
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.checkTorus.setText(QtGui.QApplication.translate("Dialog", "torus", None, QtGui.QApplication.UnicodeUTF8))
        self.checkCube.setToolTip(QtGui.QApplication.translate("Dialog", "<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
        "p, li { white-space: pre-wrap; }\n"
        "</style></head><body style=\" font-family:\'MS Shell Dlg 2\'; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
        "<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Check to choose Cube to Make.</p>\n"
        "<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p></body></html>", None, QtGui.QApplication.UnicodeUTF8))
        self.checkCube.setText(QtGui.QApplication.translate("Dialog", "cube", None, QtGui.QApplication.UnicodeUTF8))

# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
