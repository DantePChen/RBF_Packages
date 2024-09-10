
#Creation Date:  (December 1, 2006)

#Author: John Creson

import maya.cmds as cmds
import maya, os, sys, fnmatch


def findFile(path):
	#Find the file named path in the sys.path.
    	#Returns the full path name if found, None if not found
    	for dirname in sys.path:
        	possible = os.path.join(dirname, path)
        	if os.path.isfile(possible):
            		# print dirname
			return dirname
    	# print ("None")
	return None





def gen_pythonScripts(mainDir, pMenu):


# this looks in the folder where this script is run from and generates the
# cascading menues and Python script buttons

    if (mainDir[-1] != "/"):
        mainDir = (mainDir + "/")

    contents = os.listdir(mainDir)
    gMainProgressBar = maya.mel.eval('$test2=$gMainProgressBar')

    for j in (contents):
        # print ("testing: "+j)
        if (os.path.isdir (mainDir + j)):

            if(cmds.progressBar (gMainProgressBar, query=True, isCancelled=True)):
                break
            menuName = ("empt_" + j + "_menu")
            menu = cmds.menuItem (menuName, subMenu=True, aob=1, tearOff=True, parent=pMenu, label=j)
            #This adds the folder into the PythonPath
            sys.path.append(os.path.abspath(mainDir+j+"/"))

            # print ("sending back: "+(mainDir + j))
	    gen_pythonScripts ((mainDir+j+"/"), menuName)

        else:
            # print ("contents of j " + j)
	    if ((fnmatch.fnmatch (j, "*.py")) and not (fnmatch.fnmatch (j, "pythonTools*.py")) and not (fnmatch.fnmatch (j, "*.*.py"))):

		if(cmds.progressBar (gMainProgressBar, query=True, isCancelled=True)):

                    break

		if (cmds.progressBar (gMainProgressBar, q=True, pr=True)  == 100):

		    cmds.progressBar (gMainProgressBar, e=True, pr=1)

		cmds.progressBar (gMainProgressBar, edit=True, step=1)
		cmds.progressBar (gMainProgressBar, e=True, status=("Adding: "+j) )


		parts, xt = os.path.splitext(j)
		# print parts
		# print mainDir + j


		pyMenuItem = cmds.menuItem (parent=pMenu, label=parts,
                                        command = 'pythonScripts.importAndRun ("%(parts)s")'%vars())







def importAndRun(scrpt):

# create the Python command that is invoked by the menu item
# This could be changed to scrpt.main if that is normal
# The reload command should have a way of being turned off and on from the UI

    exec 'import ' + scrpt

    exec  'reload (' + scrpt + ')'

    exec scrpt + '.' + scrpt + '()'



def pythonScripts():

    if (cmds.menu ('pythonScripts_menu', exists=True)):

        # print ("Removing old pythonScripts menu...")
	cmds.deleteUI ('pythonScripts_menu')

    gMainWindow = maya.mel.eval('$gMainWindow=$gMainWindow')
    gMainProgressBar = maya.mel.eval('$gMainProgressBar=$gMainProgressBar')

    mainDir = findFile('pythonScripts.py')

    timer = maya.mel.eval('timerX')
    cmds.waitCursor (state=True)
    # print ""

    cmds.progressBar (gMainProgressBar, edit=True,
    		beginProgress=True,
    		isInterruptable=True,
    		status="Creating pythonScripts...",
    		maxValue=100)

    pMenu = (cmds.menu ('pythonScripts_menu', parent=gMainWindow, tearOff=True, aob=True, label="pythonScripts"))

    gen_pythonScripts(mainDir, pMenu)

    cmds.waitCursor (state=False)

    endTime = maya.mel.eval('timerX -st %(timer)s' % vars())

    cmds.progressBar (gMainProgressBar, edit=True,
    		endProgress=True,)

    # print ("pythonScripts has now been updated in: " + str(endTime) + " seconds...!")




# ===========================================================================
# Copyright 2019 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
