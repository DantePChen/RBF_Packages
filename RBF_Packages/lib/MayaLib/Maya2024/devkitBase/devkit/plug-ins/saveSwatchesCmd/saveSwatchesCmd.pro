include(../qtconfig)

QT += widgets
unix|macx {
  CONFIG += no_plugin_name_prefix
}
TARGET = saveSwatchesCmd
HEADERS += saveSwatchesCmd.h
SOURCES += saveSwatchesCmd.cpp
LIBS += -lOpenMayaUI
