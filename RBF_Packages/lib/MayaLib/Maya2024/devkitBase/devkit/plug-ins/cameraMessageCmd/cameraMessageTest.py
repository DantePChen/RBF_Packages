"""
This is example code demonstrating how to add and remove MCameraMessages using the 
Maya Python API 1.0 .

"""

import maya.OpenMaya as OpenMaya
import maya.cmds

def beginManipCB(node, clientData):
    print("Inside beginManipCB, clientData is %s"%clientData)
def endManipCB(node, clientData):
    print("Inside endManipCB, clientData is %s"%clientData)

callbackIDs = []

def test():
    addCallbacksToPerspCamera()
    
def addCallbacksToPerspCamera():
    global callbackIDs
    dependNode = OpenMaya.MObject() 
    dagPath = OpenMaya.MDagPath()
    camFn = OpenMaya.MFnCamera()

    # For simplicity, select the persp camera's shape
    #
    maya.cmds.select('perspShape')
    
    # Create a selection list iterator
    #
    slist = OpenMaya.MSelectionList()
    OpenMaya.MGlobal.getActiveSelectionList(slist)
    iter = OpenMaya.MItSelectionList(slist)
    
    print("List length is %d"%slist.length())    

    # Iterate over all selected dependency nodes
    #
    while not iter.isDone():
        # Get the selected dependency node
        #
        iter.getDependNode(dependNode)
        iter.getDagPath(dagPath)
        print("Node: %s"%dagPath.partialPathName())
        if (dependNode.hasFn(OpenMaya.MFn.kCamera)):
            print("This is a camera, adding manipulation callbacks with payloads")
            payloadBegin = "12345.5"
            payloadEnd = "54321.5"
            
            # Add the begin manipulation and end manipulation callbacks to the camera shape
            #
            callbackIDs.append(
                OpenMaya.MCameraMessage.addBeginManipulationCallback(dependNode, beginManipCB, payloadBegin)
            )
            callbackIDs.append(
                OpenMaya.MCameraMessage.addEndManipulationCallback(dependNode, endManipCB, payloadEnd)
            )
        else:
            print("This node is not a camera...")
            
        next(iter)

def removeCallbacks():
    global callbackIDs
    for id in callbackIDs:
        OpenMaya.MMessage.removeCallback(id)
    callbackIDs = []

"""
import cameraMessageTest as cmt

maya.cmds.file(f=1,new=1)
reload(cmt)

cmt.test()
cmt.removeCallbacks()
"""