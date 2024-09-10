#-
# ===========================================================================
# Copyright 2015 Autodesk, Inc.  All rights reserved.
#
# Use of this software is subject to the terms of the Autodesk license
# agreement provided at the time of installation or download, or which
# otherwise accompanies this software in either electronic or hard copy form.
# ===========================================================================
#+

from string import *
import maya.api.OpenMaya as om
import maya.api.OpenMayaRender as omr
import textwrap

# Produces dependency graph node DepthShader
# This node is an example of a surface shader that colors objects based on the distance from the camera.
# The inputs for this node are can be found in the Maya UI on the Attribute Editor for the node.
# The output attribute of the node is called "outColor". It is a 3 float value that represents the resulting color produced by the node. 
# To use this shader, create a DepthShader with Shading Group or connect its output to a Shading Group's "SurfaceShader" attribute.


def maya_useNewAPI():
	"""
	The presence of this function tells Maya that the plugin produces, and
	expects to be passed, objects created using the Maya Python API 2.0.
	"""
	pass

##################################################################
## Plugin Depth Shader Class Declaration
##################################################################
class depthShader(om.MPxNode):
	# Id tag for use with binary file format
	id = om.MTypeId( 0x81002 )

	# Input attributes
	aColorNear = None
	aColorFar = None
	aNear = None
	aFar = None
	aPointCamera = None

	# Output attributes
	aOutColor = None

	@staticmethod
	def creator():
		return depthShader()

	@staticmethod
	def initialize():
		nAttr = om.MFnNumericAttribute()

		# Create input attributes

		depthShader.aColorNear = nAttr.createColor("color", "c")
		nAttr.keyable = True
		nAttr.storable = True
		nAttr.readable = True
		nAttr.writable = True
		nAttr.default = (0.0, 1.0, 0.0)			# Green


		depthShader.aColorFar = nAttr.createColor("colorFar", "cf")
		nAttr.keyable = True
		nAttr.storable = True
		nAttr.readable = True
		nAttr.writable = True
		nAttr.default = (0.0, 0.0, 1.0)			# Blue

		depthShader.aNear = nAttr.create("near", "n", om.MFnNumericData.kFloat)
		nAttr.keyable = True
		nAttr.storable = True
		nAttr.readable = True
		nAttr.writable = True
		nAttr.setMin(0.0)
		nAttr.setSoftMax(1000.0)

		depthShader.aFar = nAttr.create("far", "f", om.MFnNumericData.kFloat)
		nAttr.keyable = True
		nAttr.storable = True
		nAttr.readable = True
		nAttr.writable = True
		nAttr.setMin(0.0)
		nAttr.setSoftMax(1000.0)
		nAttr.default = 2.0

		depthShader.aPointCamera = nAttr.createPoint("pointCamera", "p")
		nAttr.keyable = True
		nAttr.storable = True
		nAttr.readable = True
		nAttr.writable = True
		nAttr.hidden = True

		# Create output attributes
		depthShader.aOutColor = nAttr.createColor("outColor", "oc")
		nAttr.keyable = False
		nAttr.storable = False
		nAttr.readable = True
		nAttr.writable = False

		om.MPxNode.addAttribute(depthShader.aColorNear)
		om.MPxNode.addAttribute(depthShader.aColorFar)
		om.MPxNode.addAttribute(depthShader.aNear)
		om.MPxNode.addAttribute(depthShader.aFar)
		om.MPxNode.addAttribute(depthShader.aPointCamera)
		om.MPxNode.addAttribute(depthShader.aOutColor)

		om.MPxNode.attributeAffects(depthShader.aColorNear, depthShader.aOutColor)
		om.MPxNode.attributeAffects(depthShader.aColorFar, depthShader.aOutColor)
		om.MPxNode.attributeAffects(depthShader.aNear, depthShader.aOutColor)
		om.MPxNode.attributeAffects(depthShader.aFar, depthShader.aOutColor)
		om.MPxNode.attributeAffects(depthShader.aPointCamera, depthShader.aOutColor)

	def __init__(self):
		om.MPxNode.__init__(self)

	def compute(self, plug, block):
		# outColor or individial R, G, B channel
		if (plug != depthShader.aOutColor) and (plug.parent() != depthShader.aOutColor):
			return None # Let the Maya parent class compute the plug

		# get sample surface shading parameters
		pCamera   = block.inputValue(depthShader.aPointCamera).asFloatVector()
		cNear     = block.inputValue(depthShader.aColorNear).asFloatVector()
		cFar      = block.inputValue(depthShader.aColorFar).asFloatVector()
		nearClip  = block.inputValue(depthShader.aNear).asFloat()
		farClip   = block.inputValue(depthShader.aFar).asFloat()

		# pCamera.z is negative
		ratio = 1.0
		dist = farClip - nearClip
		if dist != 0:
			ratio = (farClip + pCamera.z) / dist
		resultColor = cNear * ratio + cFar*(1.0 - ratio)

		# set ouput color attribute
		outColorHandle = block.outputValue( depthShader.aOutColor )
		outColorHandle.setMFloatVector( resultColor )
		outColorHandle.setClean()

		# The plug has been computed successfully
		return self

	def postConstructor(self):
		self.setMPSafe(True)

##################################################################
## Plugin Depth Shader Override Class Declaration
##################################################################
class depthShaderOverride(omr.MPxSurfaceShadingNodeOverride):
	@staticmethod
	def creator(obj):
		return depthShaderOverride(obj)

	def __init__(self, obj):
		omr.MPxSurfaceShadingNodeOverride.__init__(self, obj)

		# Register fragments with the manager if needed
		fragmentMgr = omr.MRenderer.getFragmentManager()
		if fragmentMgr != None:
			if not fragmentMgr.hasFragment("depthShaderPluginFragment"):
				fragmentBody  = textwrap.dedent("""
					<fragment uiName=\"depthShaderPluginFragment\" name=\"depthShaderPluginFragment\" type=\"plumbing\" class=\"ShadeFragment\" version=\"1.0\">
						<description><![CDATA[Depth shader fragment]]></description>
						<properties>
							<float name=\"depthValue\" />
							<float3 name=\"color\" />
							<float3 name=\"colorFar\" />
							<float name=\"near\" />
							<float name=\"far\" />
						</properties>
						<values>
							<float name=\"depthValue\" value=\"0.0\" />
							<float3 name=\"color\" value=\"0.0,1.0,0.0\" />
							<float3 name=\"colorFar\" value=\"0.0,0.0,1.0\" />
							<float name=\"near\" value=\"0.0\" />
							<float name=\"far\" value=\"2.0\" />
						</values>
						<outputs>
							<float3 name=\"outColor\" />
						</outputs>
						<implementation>
							<implementation render=\"OGSRenderer\" language=\"Cg\" lang_version=\"2.1\">
								<function_name val=\"depthShaderPluginFragment\" />
								<source><![CDATA[
									float3 depthShaderPluginFragment(float depthValue, float3 cNear, float3 cFar, float nearClip, float farClip) \n
									{ \n
										float ratio = (farClip + depthValue)/(farClip - nearClip); \n
										return cNear*ratio + cFar*(1.0f - ratio); \n
									} \n]]>
								</source>
							</implementation>
							<implementation render=\"OGSRenderer\" language=\"HLSL\" lang_version=\"11.0\">
								<function_name val=\"depthShaderPluginFragment\" />
								<source><![CDATA[
									float3 depthShaderPluginFragment(float depthValue, float3 cNear, float3 cFar, float nearClip, float farClip) \n
									{ \n
										float ratio = (farClip + depthValue)/(farClip - nearClip); \n
										return cNear*ratio + cFar*(1.0f - ratio); \n
									} \n]]>
								</source>
							</implementation>
							<implementation render=\"OGSRenderer\" language=\"GLSL\" lang_version=\"3.0\">
								<function_name val=\"depthShaderPluginFragment\" />
								<source><![CDATA[
									vec3 depthShaderPluginFragment(float depthValue, vec3 cNear, vec3 cFar, float nearClip, float farClip) \n
									{ \n
										float ratio = (farClip + depthValue)/(farClip - nearClip); \n
										return cNear*ratio + cFar*(1.0f - ratio); \n
									} \n]]>
								</source>
							</implementation>
						</implementation>
					</fragment>""")

				fragmentMgr.addShadeFragmentFromBuffer(fragmentBody, False)

			if not fragmentMgr.hasFragment("depthShaderPluginInterpolantFragment"):
				vertexFragmentBody  = textwrap.dedent("""
					<fragment uiName=\"depthShaderPluginInterpolantFragment\" name=\"depthShaderPluginInterpolantFragment\" type=\"interpolant\" class=\"ShadeFragment\" version=\"1.0\">
						<description><![CDATA[Depth shader vertex fragment]]></description>
						<properties>
							<float3 name=\"Pm\" semantic=\"Pm\" flags=\"varyingInputParam\" />
							<float4x4 name=\"worldViewProj\" semantic=\"worldviewprojection\" />
						</properties>
						<values>
						</values>
						<outputs>
							<float name=\"outDepthValue\" ^1s/>
						</outputs>
						<implementation>
							<implementation render=\"OGSRenderer\" language=\"Cg\" lang_version=\"2.1\">
								<function_name val=\"depthShaderPluginInterpolantFragment\" />
								<source><![CDATA[
									float depthShaderPluginInterpolantFragment(float depthValue) \n
									{ \n
										return depthValue; \n
									} \n]]>
								</source>
								<vertex_source><![CDATA[
									float idepthShaderPluginInterpolantFragment(float3 Pm, float4x4 worldViewProj) \n
									{ \n
										float4 pCamera = mul(worldViewProj, float4(Pm, 1.0f)); \n
										return (pCamera.z - pCamera.w*2.0f); \n
									} \n]]>
								</vertex_source>
							</implementation>
							<implementation render=\"OGSRenderer\" language=\"HLSL\" lang_version=\"11.0\">
								<function_name val=\"depthShaderPluginInterpolantFragment\" />
								<source><![CDATA[
									float depthShaderPluginInterpolantFragment(float depthValue) \n
									{ \n
										return depthValue; \n
									} \n]]>
								</source>
								<vertex_source><![CDATA[
									float idepthShaderPluginInterpolantFragment(float3 Pm, float4x4 worldViewProj) \n
									{ \n
										float4 pCamera = mul(float4(Pm, 1.0f), worldViewProj); \n
										return (pCamera.z - pCamera.w*2.0f); \n
									} \n]]>
								</vertex_source>
							</implementation>
							<implementation render=\"OGSRenderer\" language=\"GLSL\" lang_version=\"3.0\">
								<function_name val=\"depthShaderPluginInterpolantFragment\" />
								<source><![CDATA[
									float depthShaderPluginInterpolantFragment(float depthValue) \n
									{ \n
										return depthValue; \n
									} \n]]>
								</source>
								<vertex_source><![CDATA[
									float idepthShaderPluginInterpolantFragment(vec3 Pm, mat4 worldViewProj) \n
									{ \n
										vec4 pCamera = worldViewProj * vec4(Pm, 1.0f); \n
										return (pCamera.z - pCamera.w*2.0f); \n
									} \n]]>
								</vertex_source>
							</implementation>
						</implementation>
					</fragment>""")

				# In DirectX, need to specify a semantic for the output of the vertex shader
				if omr.MRenderer.drawAPI() == omr.MRenderer.kDirectX11:
					vertexFragmentBody = replace(vertexFragmentBody, "^1s", "semantic=\"extraDepth\" ")
				else:
					vertexFragmentBody = replace(vertexFragmentBody, "^1s", " ")

				fragmentMgr.addShadeFragmentFromBuffer(vertexFragmentBody, False)

			if not fragmentMgr.hasFragment("depthShaderPluginGraph"):
				fragmentGraphBody  = textwrap.dedent("""
					<fragment_graph name=\"depthShaderPluginGraph\" ref=\"depthShaderPluginGraph\" class=\"FragmentGraph\" version=\"1.0\">
						<fragments>
								<fragment_ref name=\"depthShaderPluginFragment\" ref=\"depthShaderPluginFragment\" />
								<fragment_ref name=\"depthShaderPluginInterpolantFragment\" ref=\"depthShaderPluginInterpolantFragment\" />
						</fragments>
						<connections>
							<connect from=\"depthShaderPluginInterpolantFragment.outDepthValue\" to=\"depthShaderPluginFragment.depthValue\" />
						</connections>
						<properties>
							<float3 name=\"Pm\" ref=\"depthShaderPluginInterpolantFragment.Pm\" semantic=\"Pm\" flags=\"varyingInputParam\" />
							<float4x4 name=\"worldViewProj\" ref=\"depthShaderPluginInterpolantFragment.worldViewProj\" semantic=\"worldviewprojection\" />
							<float3 name=\"color\" ref=\"depthShaderPluginFragment.color\" />
							<float3 name=\"colorFar\" ref=\"depthShaderPluginFragment.colorFar\" />
							<float name=\"near\" ref=\"depthShaderPluginFragment.near\" />
							<float name=\"far\" ref=\"depthShaderPluginFragment.far\" />
						</properties>
						<values>
							<float3 name=\"color\" value=\"0.0,1.0,0.0\" />
							<float3 name=\"colorFar\" value=\"0.0,0.0,1.0\" />
							<float name=\"near\" value=\"0.0\" />
							<float name=\"far\" value=\"2.0\" />
						</values>
						<outputs>
							<float3 name=\"outColor\" ref=\"depthShaderPluginFragment.outColor\" />
						</outputs>
					</fragment_graph>""")

				fragmentMgr.addFragmentGraphFromBuffer(fragmentGraphBody)

	def supportedDrawAPIs(self):
		return omr.MRenderer.kOpenGL | omr.MRenderer.kOpenGLCoreProfile | omr.MRenderer.kDirectX11

	def fragmentName(self):
		return "depthShaderPluginGraph"

##
## Plugin setup
#######################################################
sRegistrantId = "depthShaderPlugin"

def initializePlugin(obj):
	plugin = om.MFnPlugin(obj, "Autodesk", "4.5", "Any")
	try:
		userClassify = "shader/surface:drawdb/shader/surface/depthShader"
		plugin.registerNode("depthShader", depthShader.id, depthShader.creator, depthShader.initialize, om.MPxNode.kDependNode, userClassify)
	except:
		sys.stderr.write("Failed to register node\n")
		raise

	try:
		global sRegistrantId
		omr.MDrawRegistry.registerSurfaceShadingNodeOverrideCreator("drawdb/shader/surface/depthShader", sRegistrantId, depthShaderOverride.creator)
	except:
		sys.stderr.write("Failed to register override\n")
		raise

def uninitializePlugin(obj):
	plugin = om.MFnPlugin(obj)
	try:
		plugin.deregisterNode(depthShader.id)
	except:
		sys.stderr.write("Failed to deregister node\n")
		raise

	try:
		global sRegistrantId
		omr.MDrawRegistry.deregisterSurfaceShadingNodeOverrideCreator("drawdb/shader/surface/depthShader", sRegistrantId)
	except:
		sys.stderr.write("Failed to deregister override\n")
		raise

