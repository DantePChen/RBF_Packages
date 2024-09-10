include(../qtconfig)

QT += widgets uitools
unix|macx {
  CONFIG += no_plugin_name_prefix
}
TARGET = qtForms
HEADERS += qtForms.h
SOURCES += qtForms.cpp
FORMS += cubeForm.ui
RESOURCES += qtForms.qrc
