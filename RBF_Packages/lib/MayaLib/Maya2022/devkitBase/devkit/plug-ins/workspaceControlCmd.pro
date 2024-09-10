include(qtconfig)

QT += widgets
unix|macx {
  CONFIG += no_plugin_name_prefix
}
TARGET = workspaceControlCmd
HEADERS += workspaceControlCmd.h
SOURCES += workspaceControlCmd.cpp
LIBS += -lOpenMayaUI