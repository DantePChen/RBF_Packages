if False:
    from typing import Dict, List, Tuple, Union, Optional

from . import item as itemModel

class LightItem(itemModel.LightItemBase):
    """
    Light editor item for light sources
    """
    
    
    
    def __init__(self): pass
    def compute(self, mplug, dataBlock): pass
    def dispose(self, deleteLight): pass
    def getAttrName(self, index): pass
    def getAttrPlug(self, index): pass
    def getAttrValue(self, index): pass
    def getLightName(self, fullPath='False'): pass
    def getLightShape(self): pass
    def getLightTransform(self): pass
    def getLightTransformName(self, fullPath='False'): pass
    def getLightType(self): pass
    def isAbstractClass(self): pass
    def isAcceptableChild(self, model):
        """
        Check if the model could be a child
        """
        pass
    def isConnected(self, index): pass
    def registerCallbacks(self, lightShapeObj): pass
    def rename(self, newName): pass
    def setLightShape(self, lightShapeObj): pass
    def unregisterCallbacks(self): pass
    @classmethod
    def creator(cls): pass
    @staticmethod
    def initializer(): pass
    kTypeId = None
    
    
    kTypeName = 'lightItem'
    
    
    light = None



