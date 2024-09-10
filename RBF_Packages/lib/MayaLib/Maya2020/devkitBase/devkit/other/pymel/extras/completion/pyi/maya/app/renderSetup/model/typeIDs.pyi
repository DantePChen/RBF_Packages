from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import maya.api.OpenMaya as OpenMaya


"""
This module centralizes type IDs for all render setup nodes.  The
range of reserved node type IDs for render setup is 0x58000370 to
0x580003FF, inclusive.  See file Maya/src/Plugins/NodeIdList.txt.
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

def isRenderSetupType(typeID):
    """
    Args:
        typeID: the MTypeId to test
    
    Returns: True if it is in the range of reserved RenderSetup class types otherwise False
    
    Note: Don't include the light editor nodes as RenderSetup node types. 
          We must be able to add overrides on the light editor nodes.
    """
    ...


selector : OpenMaya.MTypeId
renderSettingsChildCollection : OpenMaya.MTypeId
valueOverride : OpenMaya.MTypeId
renderSetup : OpenMaya.MTypeId
applyRel3FloatsOverride : OpenMaya.MTypeId
childNode : OpenMaya.MTypeId
applyAbsFloatOverride : OpenMaya.MTypeId
lightItemBase : OpenMaya.MTypeId
applyAbsOverride : OpenMaya.MTypeId
materialOverride : OpenMaya.MTypeId
basicSelector : OpenMaya.MTypeId
connectionOverride : OpenMaya.MTypeId
listItem : OpenMaya.MTypeId
renderLayer : OpenMaya.MTypeId
applyRelFloatOverride : OpenMaya.MTypeId
applyAbsBoolOverride : OpenMaya.MTypeId
aovCollection : OpenMaya.MTypeId
applyAbsIntOverride : OpenMaya.MTypeId
lightsCollection : OpenMaya.MTypeId
applyAbs3FloatsOverride : OpenMaya.MTypeId
simpleSelector : OpenMaya.MTypeId
lightItem : OpenMaya.MTypeId
collection : OpenMaya.MTypeId
absUniqueOverride : OpenMaya.MTypeId
applyRelIntOverride : OpenMaya.MTypeId
lightsCollectionSelector : OpenMaya.MTypeId
absOverride : OpenMaya.MTypeId
applyAbsStringOverride : OpenMaya.MTypeId
applyAbsEnumOverride : OpenMaya.MTypeId
applyRel2FloatsOverride : OpenMaya.MTypeId
lightGroup : OpenMaya.MTypeId
upperBound : long
applyOverride : OpenMaya.MTypeId
connectionUniqueOverride : OpenMaya.MTypeId
relOverride : OpenMaya.MTypeId
lowerBound : long
applyRelOverride : OpenMaya.MTypeId
override : OpenMaya.MTypeId
lightEditor : OpenMaya.MTypeId
applyAbs2FloatsOverride : OpenMaya.MTypeId
renderSettingsCollection : OpenMaya.MTypeId
arnoldAOVChildSelector : OpenMaya.MTypeId
relUniqueOverride : OpenMaya.MTypeId
applyConnectionOverride : OpenMaya.MTypeId
lightsChildCollection : OpenMaya.MTypeId
shaderOverride : OpenMaya.MTypeId
aovChildCollection : OpenMaya.MTypeId

