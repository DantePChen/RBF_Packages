from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
from . import rendererCallbacks
import maya
from . import utils
import maya.app.renderSetup.common.utils as commonUtils
import maya.cmds as cmds
import maya.api.OpenMaya as OpenMaya
from . import renderSetup


"""
The import/export mechanism for Render Setup relies on the json library from Python 
for the encode/decode of any kind of data types.

Render settings information is comprised of multiple pieces:
- Generic default nodes
- Renderer specific default nodes
- Data exported by a registered 3rdParty renderer

3rdParty renderers can register to export render settings information by either 
deriving from the RenderSettingsCallbacks class defined in the rendererCallbacks
module, or by implementing an identical interface. This class is then registered by 
calling:

rendererCallbacks.registerCallbacks(rendererName, 
                                    rendererCallbacks.CALLBACKS_TYPE_RENDER_SETTINGS
                                    callbacksClassImplementation)
                                    
A sample implementation of the RenderSettingsCallbacks is as follows:

# This works because the BasicNodeExporter has the same interface as the RenderSettingsCallbacks
class RendererRenderSettingsCallbacks(rendererCallbacks.BasicNodeExporter):
    def __init__(self):
        self.setNodes(['defaultRendererNode'])
        self.setPlugsToIgnore(['defaultRendererNode.attrToIgnore'])
        
The above example exports the attribute values of the defaultRendererNode with the exception of the
attribute defaultRendererNode.attrToIgnore which is being passed to the setPlugsToIgnore function call.
Users can use a BasicNodeExporter in a similar way to specify an array of nodes to export attribute
information from with setNodes, as well as a list of attributes to avoid exporting with the 
setPlugsToIgnore call.

Note: In order to ease the understanding of the json file resulting from an 'Export All' and to improve
      the long-term maintainability, the encode/decode mechanism creates a Python structure with a specific
      decomposition for the Render Settings attributes. The goal is to highlight the three pieces of data 
      that is exported so that issues can be tracked down more easily.

      The structure is:

        {
            "sceneSettings": {
                "mayaSoftware": {
                    "userData": {},              # User defined information is here (renderer specific data)
                    "defaultNodes": {},          # Generic default nodes information is here
                    "defaultRendererNodes": {}   # Renderer specific default nodes information is here
                }
            },
            "renderSetup": {
            }
        }
"""


if False:
    from typing import Dict, List, Tuple, Union, Optional

class DefaultNodeExporter(rendererCallbacks.BasicNodeExporter):
    """
    Exporter to only manage the default nodes independant of the selected renderer
    """
    
    
    
    def __init__(self): ...


class VoidUserDataExporter(rendererCallbacks.RenderSettingsCallbacks):
    """
    Placeholder exporter class
    """
    
    
    
    pass


class DefaultRendererNodeExporter(rendererCallbacks.BasicNodeExporter):
    """
    Exporter to only manage renderer specific default nodes
    """
    
    
    
    def __init__(self, renderer, defaultNodeExporter): ...




def encodeMasterValues():
    """
    Encode the master values of the attributes related to the Render
    Settings of a specific renderer
    """
    ...
def getAllRenderSettingsNodesInScene():
    """
    Return a set of all Render Settings nodes in the scene using their names
    """
    ...
def sceneHasRenderSettingsApplyOverrides(): ...
def _registerSelectedRender(currentRenderer): ...
def decode(*args, **kwargs): ...
def getDefaultNodes():
    """
    Return the list of default nodes
    """
    ...
def encode():
    """
    Encode all the attribute values related to the Render Settings of a specific renderer
    """
    ...
def _registerDefaultMechanisms(renderer): ...


voidUserDataExporter : VoidUserDataExporter
kRendererMismatch : unicode
renderSettingsExporters : dict
kCreateDefaultNodesFailed : unicode

