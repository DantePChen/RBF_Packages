include(../qtconfig)

unix|macx {
  CONFIG += no_plugin_name_prefix
}
TARGET = grabUV
SOURCES += grabUVMain.cpp
LIBS += -lOpenMayaUI -lOpenMayaRender
