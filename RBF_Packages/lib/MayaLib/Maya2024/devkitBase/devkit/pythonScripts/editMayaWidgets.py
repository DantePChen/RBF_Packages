# Copyright 2015 Autodesk, Inc. All rights reserved.
# 
# Use of this software is subject to the terms of the Autodesk
# license agreement provided at the time of installation or download,
# or which otherwise accompanies this software in either electronic
# or hard copy form.

from maya import cmds
from maya import mel
from maya import OpenMayaUI as omui 

try:
    from PySide2.QtWidgets import QWidget
    from shiboken2 import wrapInstance 
except:
    from PySide6.QtWidgets import QWidget
    from shiboken6 import wrapInstance

def changeMayaBackgroundColor(background='black', color='yellow'):
    # Get the mainWindow widget
    omui.MQtUtil.mainWindow()
    ptr = omui.MQtUtil.mainWindow()
    widget = wrapInstance(int(ptr), QWidget)

    # Change the cascading stylesheet
    style = ''
    if background: style += 'background-color: %s;'%background
    if color:      style += 'color: %s;'%color
    widget.setStyleSheet(style)


def changeMayaMenuColors(fontStyle='italic', fontWeight='bold', fontColor='cyan'):
    # Get the widget
    widgetStr = mel.eval( 'proc string getMCM() { global string $gMainCreateMenu; return $gMainCreateMenu; }; getMCM();' )
    ptr = omui.MQtUtil.findControl( widgetStr )
    widget = wrapInstance(int(ptr), QWidget)

    # Change the cascading stylesheet
    style = ''
    if fontStyle:  style += 'font-style: %s;'%fontStyle
    if fontWeight: style += 'font-weight: %s;'%fontWeight
    if fontColor:  style += 'color: %s;'%fontColor
    widget.setStyleSheet(style)


def main():
    changeMayaBackgroundColor()
    changeMayaMenuColors()
    
if __name__ == '__main__':
    main()
