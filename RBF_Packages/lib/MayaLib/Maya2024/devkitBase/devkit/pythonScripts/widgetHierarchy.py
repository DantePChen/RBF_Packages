# Copyright 2015 Autodesk, Inc. All rights reserved.
# 
# Use of this software is subject to the terms of the Autodesk
# license agreement provided at the time of installation or download,
# or which otherwise accompanies this software in either electronic
# or hard copy form.

"""
Widget Hierarchy
"""

# remember what version of PySide is being used
pysideVersion = '-1'

try:
    from PySide2.QtCore import * 
    from PySide2.QtGui import * 
    from PySide2.QtWidgets import *
    from PySide2 import __version__ as pysideVersion
    from shiboken2 import wrapInstance 
except:
    from PySide6.QtCore import Qt
    from PySide6.QtGui import QStandardItemModel, QStandardItem
    from PySide6.QtWidgets import QTreeView, QWidget
    from PySide6 import __version__ as pysideVersion
    from shiboken6 import wrapInstance

from maya import OpenMayaUI as omui 
from maya.app.general.mayaMixin import MayaQWidgetBaseMixin
from pprint import pprint


class WidgetHierarchyTree(MayaQWidgetBaseMixin, QTreeView):
    def __init__(self, rootWidget=None, *args, **kwargs):
        super(WidgetHierarchyTree, self).__init__(*args, **kwargs)

        # Destroy this widget when closed.  Otherwise it will stay around
        self.setAttribute(Qt.WA_DeleteOnClose, True)

        # Determine root widget to scan
        if rootWidget != None:
            self.rootWidget = rootWidget
        else:
            mayaMainWindowPtr = omui.MQtUtil.mainWindow() 
            self.rootWidget = wrapInstance(int(mayaMainWindowPtr), QWidget)

        self.populateModel()


    def populateModel(self):
        # Create the headers
        self.columnHeaders = ['Class', 'ObjectName', 'Children']
        myModel = QStandardItemModel(0,len(self.columnHeaders))
        for col,colName in enumerate(self.columnHeaders):
            myModel.setHeaderData(col, Qt.Horizontal, colName)

        # Recurse through child widgets
        parentItem = myModel.invisibleRootItem();
        self.populateModel_recurseChildren(parentItem, self.rootWidget)
        self.setModel( myModel )


    def populateModel_recurseChildren(self, parentItem, widget):
        # Construct the item data and append the row
        classNameStr = str(widget.__class__).split("'")[1]
        if pysideVersion == '1.2.0' :
          classNameStr = classNameStr.replace('PySide.','').replace('QtGui.', '').replace('QtCore.', '')
        else:
          classNameStr = classNameStr.replace('PySide2.','').replace('QtGui.', '').replace('QtCore.', '').replace('QtWidgets.', '')

        items = [QStandardItem(classNameStr), 
                 QStandardItem(widget.objectName()),
                 QStandardItem(str(len(widget.children()))),
                 ]
        parentItem.appendRow(items)

        # Recurse children and perform the same action
        for childWidget in widget.children():
            self.populateModel_recurseChildren(items[0], childWidget)


def main():
    ui = WidgetHierarchyTree()
    ui.show()
    return ui


if __name__ == '__main__':
    main()
