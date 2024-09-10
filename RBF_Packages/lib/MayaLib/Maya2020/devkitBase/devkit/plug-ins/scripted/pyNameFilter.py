# ===========================================================================
# Copyright 2018 Autodesk, Inc. All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
import maya.OpenMaya as om
import maya.OpenMayaMPx as omMPx

# The pyNameFilter class is a cache configuration filter that looks at the 
# dependency graph (DG) nodes relating to caching in order to match their 
# name with a pattern.
#
# This filter requires a param string 
# (newFilterParam from the cacheEvaluator command) formatted like so:
#      "pattern,option"
# where
#      pattern is the substring to search for in all the dependency nodes 
#              relating to the cache configuration
#      option is optional. Can be one of the following string values:
#          reverse: reverse logic match behavior: the filter will match on 
#                   node names NOT containing the pattern
#
# It can be used to include or exclude nodes from caching modes based on 
# their names when used with the cacheEvaluator command.
#
# Example:
#  Use default evaluation cache configuration mode but exclude cones from 
#  the cache (Python):
#
#      import maya.cmds as cmds
#      cmds.loadPlugin( 'pyNameFilter.py' )
#      
#      # Reset Rules
#      cmds.cacheEvaluator(resetRules = True);
#      
#      # Add Eval Cache action/filter
#      cmds.cacheEvaluator(newFilter = 'evaluationCacheNodes', newAction = 'enableEvaluationCache' )
#      
#      # Add Custom Filter
#      cmds.cacheEvaluator(newFilter = 'pyNameFilter', newFilterParam = 'Cone', newAction = 'disableAllCache' )
#
# See also the OpenMaya API documentation for the MPxCacheConfigRuleFilter 
# proxy class

def pyNameFilterCreator( param ):
    # Parse the creation parameters here 
    if 0 == len( param ):
        om.MGlobal.displayError( 'Cannot construct NameFilter object: empty parameter string' )
        return None

    tokens = param.split( ',' )
    if len( tokens ) > 2:
        om.MGlobal.displayError( 'Cannot construct NameFilter object: the parameter string has too many tokens' )
        return None

    # Get the pattern to match against
    pattern = tokens[0]

    # Get the option
    reverseLogic = False
    if len(tokens) > 1:
        if tokens[1] == "reverse":
            reverseLogic = True
        else:
            displayString = 'Cannot construct NameFilter object: expected "reverse" for the second token in the parameter string, got "' + tokens[1] + '"'
            om.MGlobal.displayError( displayString )
            return None

    return omMPx.asMPxPtr( pyNameFilter(pattern = pattern, reverseLogic = reverseLogic) )


def initializePlugin( mObject ):
    om.MCacheConfigRuleRegistry.registerFilter( "pyNameFilter", pyNameFilterCreator );

def uninitializePlugin( mObject ):
    om.MCacheConfigRuleRegistry.unregisterFilter( "pyNameFilter" );

class pyNameFilter( omMPx.MPxCacheConfigRuleFilter ):
    def __init__( self, pattern = None, reverseLogic = False ):
        super(pyNameFilter, self).__init__()
        self._pattern       = pattern
        self._reverseLogic  = reverseLogic

    def isMatch( self, evalNode ):
        fnNode = om.MFnDependencyNode( evalNode.dependencyNode() )
        nodeAbsName = fnNode.absoluteName( );
            
        # Simple name-matching, using the reverse logic if specified
        nodeNameMatchesPattern = True if self._pattern in nodeAbsName else False
        return nodeNameMatchesPattern if False == self._reverseLogic else not nodeNameMatchesPattern
    