include(../qtconfig)

QT += widgets
unix|macx {
  CONFIG += no_plugin_name_prefix
}
TARGET = qtAdskStyleProxy
win32 {
	TARGET_EXT	= .dll
}
HEADERS += qtAdskStyleProxy.h
SOURCES += qtAdskStyleProxy.cpp
LIBS += -lOpenMayaUI

# This is a Qt style plugin, not a Maya plugin, so we don't need these functions.
QMAKE_LFLAGS -= /export:initializePlugin /export:uninitializePlugin

# To use this plugin:
#
# - After it is compiled, copy the target file (qtAdskStyleProxy.dll/.so/.bundle)
#   to the '.../plugins/styles/' folder, where there should already be a
#   plugin 'AdskStyle'.
#
# - Start Maya with the style flag, like so:
#     maya -style qtadskstyleproxy
#
# - Or you can start Maya normally and use PySide to set the style:
#     from PySide2 import QtWidgets, QtGui, QtCore
#
#     app = QtWidgets.QApplication
#     st = app.setStyle('qtadskstyleproxy')
