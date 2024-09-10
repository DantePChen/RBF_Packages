include(qtconfig)

QT += widgets
unix|macx {
  CONFIG += no_plugin_name_prefix
}
TARGET = helixQtCmd
HEADERS += helixQtCmd.h
SOURCES += helixQtCmd.cpp
