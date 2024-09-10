"""
This is example code demonstrating how to add and remove MCameraMessages using the 
Maya Python API 2.0 .

"""

import maya.api.OpenMaya as OpenMaya
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
    camFn = OpenMaya.MFnCamera()

    # For simplicity, select the persp camera's shape
    #
    maya.cmds.select('perspShape')
    
    # Create a selection list iterator
    #
    slist = OpenMaya.MGlobal.getActiveSelectionList()
    iter = OpenMaya.MItSelectionList(slist)
    
    print("List length is %d"%slist.length())    

    # Iterate over all selected dependency nodes
    #
    while not iter.isDone():
        # Get the selected dependency node
        #
        dependNode = iter.getDependNode()
        dagPath = iter.getDagPath()
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
import cameraMessageTest2 as cmt2

maya.cmds.file(f=1,new=1)
reload(cmt2)

cmt2.test()
cmt2.removeCallbacks()
"""