# Form implementation generated from reading ui file 'frmconnect.ui'
#
# Created: Wed Feb 28 21:34:40 2001
#      by: The Python User Interface Compiler (pyuic)
#
# WARNING! All changes made in this file will be lost!

import sys
from PyQt4 import *

app=None
win=None

class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(QtCore.QSize(QtCore.QRect(0,0,683,70).size()).expandedTo(Dialog.minimumSizeHint()))

        self.label = QtGui.QLabel(Dialog)
        self.label.setGeometry(QtCore.QRect(20,20,79,16))
        self.label.setObjectName("label")

        self.lineEdit = QtGui.QLineEdit(Dialog)
        self.lineEdit.setGeometry(QtCore.QRect(280,40,113,22))
        self.lineEdit.setObjectName("lineEdit")

        self.lineEdit_2 = QtGui.QLineEdit(Dialog)
        self.lineEdit_2.setGeometry(QtCore.QRect(420,40,113,22))
        self.lineEdit_2.setObjectName("lineEdit_2")

        self.pushButton_3 = QtGui.QPushButton(Dialog)
        self.pushButton_3.setGeometry(QtCore.QRect(420,10,75,24))
        self.pushButton_3.setObjectName("pushButton_3")

        self.pushButton_2 = QtGui.QPushButton(Dialog)
        self.pushButton_2.setGeometry(QtCore.QRect(280,10,81,24))
        self.pushButton_2.setObjectName("pushButton_2")

        self.pushButton_4 = QtGui.QPushButton(Dialog)
        self.pushButton_4.setGeometry(QtCore.QRect(550,20,111,41))
        self.pushButton_4.setObjectName("pushButton_4")

        self.pushButton_5 = QtGui.QPushButton(Dialog)
        self.pushButton_5.setGeometry(QtCore.QRect(160,10,111,24))
        self.pushButton_5.setObjectName("pushButton_5")

        self.pushButton = QtGui.QPushButton(Dialog)
        self.pushButton.setGeometry(QtCore.QRect(160,40,111,31))
        self.pushButton.setObjectName("pushButton")

        self.comboBox = QtGui.QComboBox(Dialog)
        self.comboBox.setGeometry(QtCore.QRect(10,40,131,22))
        self.comboBox.setObjectName("comboBox")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        Dialog.setWindowTitle(QtGui.QApplication.translate("Dialog", "Dialog", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("Dialog", "Choose Level     ", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_3.setText(QtGui.QApplication.translate("Dialog", "Input Hi res", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_2.setText(QtGui.QApplication.translate("Dialog", "Input Low res", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_4.setText(QtGui.QApplication.translate("Dialog", "Create Displacement", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton_5.setText(QtGui.QApplication.translate("Dialog", "Freeze Duplicate", None, QtGui.QApplication.UnicodeUTF8))
        self.pushButton.setText(QtGui.QApplication.translate("Dialog", "Add a Level", None, QtGui.QApplication.UnicodeUTF8))

# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
