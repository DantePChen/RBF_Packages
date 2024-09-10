from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya
import maya.cmds as cmds


from maya.analytics.decorators import makeAnalytic
from maya.analytics.decorators import addMethodDocs
from maya.analytics.decorators import addHelp
from maya.analytics.BaseAnalytic import BaseAnalytic


if False:
    from typing import Dict, List, Tuple, Union, Optional

class analyticAnimation(BaseAnalytic):
    """
    Examine the animation in the system and gather some basic statistics
    about it. There are two types of animation to find:
    
        1) Anim curves, which animate in the usual manner
           Care is taken to make sure either time is either an explicit or
           implicit input since anim curves could be used for reasons
           other than animation (e.g. setDrivenKey)
        2) Any other node which has the time node as input
           Since these are pretty generic we can only take note of how
           many of these there are, and how many output connections they
           have.
    
    The summary data consists of a count of the static and non-static
    param curves. Any curve with an input to the time parameter is
    considered non-static since the time may warp and it's more difficult
    than it's worth to figure out if this is the case.
    
    Example of a normal dump for a simple scene:
    
    "output" : {
        "static"      : { "animCurveTL" : 4, "animCurveTA" : 1 },
        "nonStatic"   : { "animCurveTL" : 126, "animCurveTA" : 7 },
        "maybeStatic" : { "expression" : 1 },
        "keys"        : { "animCurveTL" : 7200, "animCurveTA" : 43 }
        "driven"      : { "animCurveTL" : { 1 : 7200 },
                          "animCurveTA" : { 1 : 42, 2 : 1 }
                          "expression"  : { 1 : 1 } }
    }
    
        "static"      : Count of animation nodes with the same value at all times
        "nonStatic"   : Count of animation nodes with differing values at some times
        "maybeStatic" : Count of animation nodes whose values could not be ascertained
        "keys"        : Count of keyframes, where appropriate.
        "driven"      : Count of number of nodes driving various numbers of outputs
                        e.g. { 1 : 7, 2 : 1 } means 7 nodes driving a single output and
                             1 node driving 2 outputs
    
    and the same scene with the 'summary' option enabled:
    
    "output" : {
        "summary" :
        {
            "static"       : 5,
            "nonStatic"    : 133,
            "maybeStatic"  : 1,
            "keys"         : 7243,
            "animCurveTL"  : 130,
            "animCurveTA"  : 8,
            "multiDriven"  : 1,
            "noDriven"     : 0,
            "expression"   : 1
        },
        "static"      : { "animCurveTL" : 4, "animCurveTA" : 1 },
        "nonStatic"   : { "animCurveTL" : 126, "animCurveTA" : 7 }
        "maybeStatic" : { "expression" : 1 }
        "keyframes"   : { "animCurveTL" : 7200, "animCurveTA" : 43 }
        "driven"      : { "animCurveTL" : { 1 : 7200 },
                          "animCurveTA" : { 1 : 42, 2 : 1 }
                          "expression"  : { 1 : 1 } }
    }
    
    For the summary the "multiDriven" value means "the number of
    animation nodes driving more than one outputs", and "noDriven" means
    "the number of animation nodes not driving any outputs".
    
    The additional NODE_TYPE counts indicate the number of nodes of each
    animation node type in the scene. The other summary values are a count
    of the data of that type. All of the summary information is available
    within the normal data, this is just a convenient method of accessing.
    
    When the 'details' option is on then the fully detailed information about
    all animation curves is added. Here is a sample for one curve:
    
        "static" :
        {
            "animCurveTL" :
            {
                "nurbsCone1_translateX" :
                {
                    "keyframes" : [ [1.0,1.0], [10.0,10.0] ],
                    "driven"    : {"group1.tx" : "transform"}
                }
            }
        },
        "nonStatic" :
        {
            ...
        },
        "maybeStatic" :
        {
            ...
        }
    
        The data is nested as "type of animation" over "type of animation
        node" over "animation node name". Inside each node are these
        fields:
    
        "driven"    : Keyed on plugs on the destination end of the animation,
                      values are the type of said node
        "keyframes" : [Key,Value] pairs for the animation keyframes.
                      an animCurve. For expressions et. al. the member
                      will be omitted.
    """
    
    
    
    def run(self):
        """
        Run the analytic on the currently loaded file.
        :return: Animation data as described in the class doc
        """
        ...
    @staticmethod
    def help():
        """
        Call this method to print the class documentation, including all methods.
        """
        ...
    ANALYTIC_DESCRIPTION_DETAILED : str
    
    ANALYTIC_DESCRIPTION_SHORT : unicode
    
    ANALYTIC_LABEL : unicode
    
    ANALYTIC_NAME : str
    
    KEY_DRIVEN : str
    
    KEY_KEYFRAMES : str
    
    KEY_MAYBE_STATIC : str
    
    KEY_MULTI_DRIVEN : str
    
    KEY_NON_STATIC : str
    
    KEY_NO_DRIVEN : str
    
    KEY_STATIC : str
    
    ZERO_TOLERANCE : float
    
    __fulldocs__ : str
    
    is_static : bool




kAnalyticLabel : unicode
OPTION_SUMMARY : str
OPTION_DETAILS : str
kAnalyticDescriptionShort : unicode

