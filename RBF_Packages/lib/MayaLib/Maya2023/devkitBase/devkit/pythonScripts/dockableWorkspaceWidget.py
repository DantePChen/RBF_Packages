# Copyright 2017 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.

from builtins import int
from maya.app.general.mayaMixin import MayaQWidgetDockableMixin
from maya import OpenMayaUI as omui
from PySide2.QtCore import * 
from PySide2.QtGui import * 
from PySide2.QtWidgets import *
customMixinWindow = None

''' MayaQWidgetDockableMixin is not a workspace control in itself, it is added 
    as a child to a created workspace control. See help(MayaQWidgetDockableMixin) 
    for more details. The following class is a simple widget with a layout and  a push button.
''' 
class DockableWidget(MayaQWidgetDockableMixin, QWidget):
  def __init__(self, parent=None):
    super(DockableWidget, self).__init__(parent=parent)
    self.button1 = QPushButton()
    self.button1.setText('PushMe')
    
    layout = QVBoxLayout()
    layout.addWidget(self.button1)
    self.setLayout(layout)
    self.setWindowTitle('Custom Maya Mixin Workspace Control')
   

''' A workspace control is created by calling show() on the DockableWidget class. 
    This control is only created once if the retain property is set to true, which 
    is the default. The uiScript argument passed to the show() method will be 
    invoked every time this control is opened/restored. It is recommended to add 
    the proper import statement in the uiScript argument.
    
    For example, in renderSetup.py file the UIScript for renderSetupWindow is
      "uiScript='import maya.app.renderSetup.views.renderSetup as renderSetup\nrenderSetup.createUI(restore=True)'"
        
    The following method needs to be invoked in order to create the workspace control for the example above.
    If the control is being restored, then Maya will call the method by passing restore=True
'''    
def DockableWidgetUIScript(restore=False):
  global customMixinWindow
  
  ''' When the control is restoring, the workspace control has already been created and
      all that needs to be done is restoring its UI.
  '''
  if restore == True:
	  # Grab the created workspace control with the following.
      restoredControl = omui.MQtUtil.getCurrentParent()
  
  if customMixinWindow is None:
	  # Create a custom mixin widget for the first time
      customMixinWindow = DockableWidget()     
      customMixinWindow.setObjectName('customMayaMixinWindow')
      
  if restore == True:
	  # Add custom mixin widget to the workspace control
      mixinPtr = omui.MQtUtil.findControl(customMixinWindow.objectName())
      omui.MQtUtil.addWidgetToMayaLayout(int(mixinPtr), int(restoredControl))
  else:
	  # Create a workspace control for the mixin widget by passing all the needed parameters. See workspaceControl command documentation for all available flags.
      customMixinWindow.show(dockable=True, height=600, width=480, uiScript='DockableWidgetUIScript(restore=True)')
      
  return customMixinWindow
	  
''' Using the workspaceControl Maya command to query/edit flags about the created 
    workspace control can be achieved this way:
        maya.cmds.workspaceControl('customMayaMixinWindowWorkspaceControl', q=True, visible=True)
        maya.cmds.workspaceControl('customMayaMixinWindowWorkspaceControl', e=True, visible=False)
        
    Note that Maya automatically appends the "WorkspaceControl" string to the 
    workspace control child object name. In this example it is the child widget name (customMayaMixinWindow)
'''

def main():
    ui = DockableWidgetUIScript()
    return ui


if __name__ == '__main__':
    main()
