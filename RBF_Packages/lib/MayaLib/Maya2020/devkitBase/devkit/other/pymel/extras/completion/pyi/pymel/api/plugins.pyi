from typing import Any, Container, Dict, Generic, Iterable, Iterator, List, Optional, Set, Tuple, TypeVar, Union
import exceptions
import maya.OpenMaya as om
import maya
import inspect
import sys
import collections
import os
import maya.OpenMayaMPx as mpx


"""
A quick example::

    from pymel.api.plugins import Command
    class testCmd(Command):
        @classmethod
        def createSyntax(cls):
            syntax = om.MSyntax()
            # the node type name
            syntax.addArg(om.MSyntax.kString)
            return syntax

        def doIt(self, args):
            argParser =  om.MArgParser(self.syntax(), args)
            arg = argParser.commandArgumentString(0)
            print "doing it: {}".format(arg)

    testCmd.register()
    cmds.testCmd()
    testCmd.deregister()

An example of a plugin which creates a node::

    import math

    import pymel.api.plugins as plugins
    import maya.OpenMaya as om

    class PymelSineNode(plugins.DependNode):
        '''Example node adapted from maya's example sine node plugin

        Shows how much easier it is to create a plugin node using pymel.api.plugins
        '''
        # For quick testing, if _typeId is not defined, pymel will create one by
        # hashing the node name. For longer-term uses, you should explicitly set
        # own typeId like this
        #
        # (NOTE - if using the automatic typeId generation, the hashlib python
        # builtin library must be functional / working from within maya... due
        # to dynamic library linking issues (ie, libssl, libcrypto), this
        # may not always be the case out-of-the-box on some linux distros
        _typeId = om.MTypeId(0x900FF)

        # by default, the name of the node will be the name of the class - to
        # override and set your own maya node name, do this:
        #_name = 'PymelSineNode'

        @classmethod
        def initialize(cls):
            # input
            nAttr = om.MFnNumericAttribute()
            cls.input = nAttr.create( "input", "in", om.MFnNumericData.kFloat, 0.0 )
            nAttr.setStorable(1)
            cls.addAttribute( cls.input )

            # output
            cls.output = nAttr.create( "output", "out", om.MFnNumericData.kFloat, 0.0 )
            nAttr.setStorable(1)
            nAttr.setWritable(1)
            cls.addAttribute( cls.output )

            # set attributeAffects relationships
            cls.attributeAffects( cls.input, cls.output )

        def compute(self, plug, dataBlock):
            if ( plug == self.output ):
                dataHandle = dataBlock.inputValue( self.input )
                inputFloat = dataHandle.asFloat()
                result = math.sin( inputFloat )
                outputHandle = dataBlock.outputValue( self.output )
                outputHandle.setFloat( result )
                dataBlock.setClean( plug )
                return om.MStatus.kSuccess
            return om.MStatus.kUnknownParameter

    ## initialize the script plug-in
    def initializePlugin(mobject):
        PymelSineNode.register(mobject)

    # uninitialize the script plug-in
    def uninitializePlugin(mobject):
        PymelSineNode.deregister(mobject)
"""


from maya.OpenMayaMPx import MPxTransform as _mpx
from maya.OpenMayaMPx import MPxLocatorNode as _mpxCls


if False:
    from typing import Dict, List, Tuple, Union, Optional

class BasePluginMixin(object):
    @classmethod
    def create(cls): ...
    @classmethod
    def deregister(cls, plugin='None'):
        """
        If using from within a plugin module's ``initializePlugin`` or
        ``uninitializePlugin`` callback, pass along the MObject given to these
        functions.
        """
        ...
    @classmethod
    def getTypeId(cls, nodeName='None'):
        """
        # Defined here just so it can be shared between MPxTransformationMatrix
        # and DependNode
        """
        ...
    @classmethod
    def isRegistered(cls): ...
    @classmethod
    def mayaName(cls): ...
    @classmethod
    def register(cls, plugin='None'):
        """
        Used to register this MPx object wrapper with the maya plugin.
        
        By default the command will be registered to a dummy plugin provided by pymel.
        
        If using from within a plugin module's ``initializePlugin`` or
        ``uninitializePlugin`` callback, pass along the MObject given to these
        functions.
        
        When implementing the derived MPx wrappers, do not override this -
        instead, override _registerOverride
        """
        ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class _DummyPluginNodesMaker(object):
    def __enter__(self): ...
    def __exit__(self, type, value, traceback): ...
    def __init__(self, dummyClasses='None', alreadyCreated='None'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class PluginError(exceptions.Exception):
    __weakref__ : getset_descriptor


class PyNodeMethod(object):
    """
    Used as a decorator, placed on methods on a plugin node class, to signal
    that these methods should be placed on to PyNode objects constructed for
    the resulting depend nodes.
    
    >>> class FriendlyNode(DependNode):
    ...     _typeId = om.MTypeId(654748)
    ...     @PyNodeMethod
    ...     def introduce(self):
    ...         print "Hi, I'm an instance of a MyNode PyNode - my name is %s!" % self.name()
    >>> FriendlyNode.register()
    >>> import pymel.core as pm
    >>> frank = pm.createNode('FriendlyNode', name='Frank')
    >>> frank.introduce()
    Hi, I'm an instance of a MyNode PyNode - my name is Frank!
    """
    
    
    
    def __init__(self, func, name='None'): ...
    __dict__ : dictproxy
    
    __weakref__ : getset_descriptor


class DependNode(BasePluginMixin, mpx.MPxNode):
    @classmethod
    def getMpxType(cls): ...
    @classmethod
    def getTypeEnum(cls): ...
    @classmethod
    def initialize(cls): ...
    @classmethod
    def isAbstractClass(cls): ...


class FileTranslator(BasePluginMixin, mpx.MPxFileTranslator):
    def defaultExtension(self): ...
    def filter(self): ...
    def identifyFile(self, mfile, buffer, size): ...


class PluginRegistryError(PluginError):
    pass


class Command(BasePluginMixin, mpx.MPxCommand):
    """
    create a subclass of this with a doIt method
    """
    
    
    
    @classmethod
    def createSyntax(cls): ...


class TransformationMatrix(BasePluginMixin, mpx.MPxTransformationMatrix):
    @classmethod
    def deregister(cls, plugin='None'): ...
    @classmethod
    def register(cls, plugin='None'): ...


class EmitterNode(DependNode, mpx.MPxEmitterNode):
    pass


class BlendShape(DependNode, mpx.MPxBlendShape):
    pass


class SurfaceShape(DependNode, mpx.MPxSurfaceShape):
    pass


class ImagePlane(DependNode, mpx.MPxImagePlane):
    pass


class SpringNode(DependNode, mpx.MPxSpringNode):
    pass


class ManipulatorNode(DependNode, mpx.MPxManipulatorNode):
    pass


class Transform(DependNode, _mpx):
    pass


class ParticleAttributeMapperNode(DependNode, mpx.MPxParticleAttributeMapperNode):
    pass


class IkSolverNode(DependNode, mpx.MPxIkSolverNode):
    pass


class LocatorNode(DependNode, _mpxCls):
    pass


class GeometryFilter(DependNode, mpx.MPxGeometryFilter):
    pass


class AlreadyRegisteredError(PluginRegistryError):
    pass


class DeformerNode(DependNode, mpx.MPxDeformerNode):
    pass


class CameraSet(DependNode, mpx.MPxCameraSet):
    pass


class NotRegisteredError(PluginRegistryError):
    pass


class MotionPathNode(DependNode, mpx.MPxMotionPathNode):
    pass


class SkinCluster(DependNode, mpx.MPxSkinCluster):
    pass


class ObjectSet(DependNode, mpx.MPxObjectSet):
    pass


class Constraint(DependNode, mpx.MPxConstraint):
    pass


class ManipContainer(DependNode, mpx.MPxManipContainer):
    pass


class FieldNode(DependNode, mpx.MPxFieldNode):
    pass


class HwShaderNode(DependNode, mpx.MPxHwShaderNode):
    pass


class HardwareShader(DependNode, mpx.MPxHardwareShader):
    pass


class Assembly(DependNode, mpx.MPxAssembly):
    pass


class PolyTrg(DependNode, mpx.MPxPolyTrg):
    pass


class FluidEmitterNode(EmitterNode, mpx.MPxFluidEmitterNode):
    pass


class ComponentShape(SurfaceShape, mpx.MPxComponentShape):
    pass




def unloadAllPlugins(skipErrors='False', exclude="('DirectConnect',)"): ...
def enumToStr():
    """
    Returns a dictionary mapping from an MPxNode node type enum to it's
    string name.
    Useful for debugging purposes.
    """
    ...
def _pluginName(): ...
def _buildMpxNamesToMayaNodes(hierarchy='None'): ...
def _pluginModule(): ...
def _buildMpxNamesToApiEnumNames(dummyClasses='None', dummyNodes='None'): ...
def _unloadPlugin(): ...
def allMPx():
    """
    Returns a list of all MPx classes
    """
    ...
def pluginCommands(pluginName, reportedOnly='False'):
    """
    Returns the list of all commands that the plugin provides, to the best
    of our knowledge.
    
    Note that depending on your version of maya, this may not actually be the
    list of all commands provided.
    """
    ...
def _guessEnumStrFromMpxClass(className): ...
def _createDummyPluginNodeClasses():
    """
    Registers with the dummy pymel plugin a dummy node type for each MPxNode
    subclass
    
    returns a dictionary mapping from MPx class to a pymel dummy class of that
    type
    """
    ...
def uninitializePlugin(mobject):
    """
    do not call directly
    """
    ...
def _pluginFile(): ...
def _buildPluginHierarchy(dummyClasses='None'):
    """
    Dynamically query the mel node hierarchy for all plugin node types
    
    This command must be run from within a running maya session - ie, where
    maya.cmds, etc are accessible.
    """
    ...
def initializePlugin(mobject):
    """
    do not call directly
    """
    ...
def _suggestNewMPxValues(classes='None'): ...
def _getPlugin(object='None'): ...
def mayaPlugins():
    """
    all maya plugins in the maya install directory
    """
    ...
def _buildAll(): ...
def loadAllMayaPlugins():
    """
    will load all maya-installed plugins
    
    WARNING: tthe act of loading all the plugins may crash maya, especially if
    done from a non-GUI session
    """
    ...
def _loadPlugin(): ...


NON_CREATABLE : set
registered : set
_new : list
UNREPORTED_COMMANDS : dict
mpxNamesToMayaNodes : dict
_enumToStr : NoneType
mpxClassesToMpxEnums : dict
pyNodeMethods : dict
mpxNamesToEnumNames : dict
mpxNamesToApiEnumNames : dict
missingMPx : list
_allMPx : list
pluginMayaTypes : set

