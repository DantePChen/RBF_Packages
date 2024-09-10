// ==========================================================================
// Copyright 2019 Autodesk, Inc.  All rights reserved.
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
// ==========================================================================

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MColor.h>
#include <maya/MFnPlugin.h>
#include <maya/MDistance.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MGlobal.h>
#include <maya/MDagMessage.h>
#include <maya/MEvaluationManager.h>
#include <maya/MEvaluationNode.h>

// Viewport 2.0 includes
#include <maya/MDrawRegistry.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MHWGeometry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MUserData.h>
#include <maya/MSharedPtr.h>

#include <cassert>
#include <memory>
#include <unordered_map>
#include <atomic>

/*
TOPICS: Customized Rendering ; Animated Material ; Viewport Caching (VP2 Custom Caching)
DESCRIPTION:

This plug-in demonstrates how to : 
1. Draw simple Animated Material like animated color in an efficient and cache-friendly way.
2. Support Evaluation & Viewport Cache for nodes with customized rendering.

This plugin uses different techniques to optimize the performance of the foot print draw
in VP2.

Technique 1:
Forcing the renderer requested attributes to be evaluated and cached by EM.
Aka, define a 'rendering-required-attribute'.
This is usually a must-have for any custom rendering nodes.

Technique 1.1:
Geometry-change tracking with "always-return-true" attribute.

Technique 2:
Use VP2 shader's PreDrawCallback to do simple animated material.

Technique 3:
Use Evaluation-cache to store the constant buffer + Viewport-cache to store the vertex buffer.

Code structure:
    - class FootPrintNode
        The DAG node that representing the customize rendering geometry.
        Evaluation caching should be fully implemented here.

    - class FootPrintGeometryOverrideAnimatedMaterial
        The customized rendering logic for FootPrintNode.
        Viewport caching is handled in here.

    - class AnimatedColorShader
        Helper class encapsulated a shader with a proper PreDrawCallback
        Which implemented Animated Material along with FootPrintGeometryOverrideAnimatedMaterial

USAGE:
```MEL
    loadPlugin footPrintNode_GeometryOverride_AnimatedMaterial;
    createNode footPrint_GeometryOverride_AnimatedMaterial;
```
*/

// Plugin identifiers
static constexpr unsigned int gPluginNodeId                 = 0x00080039;
static constexpr const char   gPluginNodeName[]             = "footPrint_GeometryOverride_AnimatedMaterial";
static constexpr const char   gPluginDrawDbClassification[] = "drawdb/geometry/footPrint_GeometryOverride_AnimatedMaterial";
static constexpr const char   gPluginDrawRegistrantId[]     = "footPrintNode_GeometryOverride_AnimatedMaterialPlugin";
static constexpr const char   gPluginSelectionMask[]        = "footPrintSelection";
static constexpr const char   gPluginNodeMessagePrefix[]    = "FootPrintGeometryOverrideAnimatedMaterial: ";

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Node implementation with standard viewport draw
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// 
// Proxy node for drawing procedure foot print geometry
// This node contains two input attribute : "Size" and "Color"
// - Changing the "Size" attribute will cause the viewport to regenerate its geometry data (animated vertex buffer)
// - Changing the "Color" attribute reset the shader param without reseting the whole shader (animated material/constant-buffer)
// The rendering logic is specified in "class FootPrintGeometryOverrideAnimatedMaterial"
class FootPrintNode : public MPxLocatorNode // Or MPxSurfaceShape
{
public:
    // Node Attributes :
    // Attribute table 							//	'Alias'		I/O		Data-Type	Function body (in lambda format)

    // Controls the size of the footprint geometry
    static  MObject         inputSize;			// 'isz'		Input	Distance
    static  MObject         outputSize;			// 'sz'			Output	Distance	: outputSize		= [](inputSize) -> {return inputSize;}

    // Controls the color of the footprint
    static MObject          inputColor;			// 'ic'			Input	Color
    static MObject          outputColor;		// 'oc'			Output	Color		: outputColor		= [](inputColor) -> {return inputColor;}

    // Add your renderer-required-attributes here
    // static MObject          inputXX;
    // static MObject          outputXX;

    // Utility attribute for viewport
    // [[maya::storable(false)]] [[maya::connectable(false)]] [[maya::hidden]]
    static	MObject			geometryChanging;	// 'gcg'		Output	Bool		: geometryChanging	= [](inputSize) -> {return true;} [*] check notes

    // Attribute dependencies:
    //		inputColor	-> outputColor
    //		inputSize	-> outputSize
    //		inputSize	-> geometryChanging 
    // 
    //		* inputXX	-> outputXX
    //		* inputXX	-> geometryChanging (if XX affect the geometry)
    // 
    // "Logical" dependencies (Technique 1):
    //		outputColor, outputSize, geometryChanging -> [renderer]
    //		* outputXX -> [renderer]
    // 
    // Additional note:
    //
    // Q :	Why is there outputSize / outputColor ? (Technique 1)
    // A :	Input attributes, like inputSize/inputColor cannot be cached by Evaluation Cache
    //		Check FootPrintNode::setDependentsDirty() for more details about this work-around
    //
    // Q :	'outputXX' is not updating in EM mode? (Technique 1)
    // A :	The virtual connections to [renderer] are not understand by EM.
    //		Add this connection to FootPrintNode::setDependentsDirty().
    //
    // Q :	"geometryChanging" always returns 'true' ? (Technique 1.1)
    // A :	"geometryChanging" is a "dirty flag attribute" tracking if the node needs geometry-update
    //		If anything affecting it is changed, it will re-evaluate and return 'true' 
    //		This will notify the viewport that the geometry is changing, 
    //		Then, it will be reset to 'false' when geometry is updated (populateGeometry() is called)
    //		This allows us to track the dirty status without override setDependentsDirty() or postEvaluation()
    //		* Note, viewport will not reset its value in background evaluation (VP2 caching)
    //		  Check requiresGeometryUpdate(), populateGeometry() for detail
    //

public:
    // Override methods for depend node
    FootPrintNode();
    ~FootPrintNode() override;

    MStatus   		compute( const MPlug& plug, MDataBlock& data ) override;
    MStatus			setDependentsDirty(const MPlug& plug, MPlugArray& plugArray) override;
    MStatus			postEvaluation(const  MDGContext& context, const MEvaluationNode& evaluationNode, PostEvaluationType evalType) override;
    void			getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const override;

    // Do not override postEvaluation() to force evaluating 'rendering-required-attribute'.
    // Check Technique 1 for how to do this.

    bool            isBounded() const override { return true; }
    MBoundingBox    boundingBox() const override;

    MSelectionMask	getShapeSelectionMask() const override { return MSelectionMask(gPluginSelectionMask); }

    static  void *  creator() { return new FootPrintNode(); }
    static  MStatus initialize();

public:
    // Data defines the custom shape, which decides the generated geometry's vertex buffer
    // This only contains geometry data, material and appearance data should not be contained here 
    // E.g. Outer Radius R and inner radius r for a torus shape
    struct GeometryParameters
    {
        double Size;
    };

    // Methods for the renderer to call
    MColor				renderColor() const;
    bool				isGeometryChanging() const;
    void				updateRenderAttributes();
    GeometryParameters	updatingGeometry();

public:
    static const	MTypeId		id;
    static const	MString		drawDbClassification;
    static const	MString		drawRegistrantId;
    static const	MString		drawDbLightClassification;
};

MObject FootPrintNode::outputSize       = {};
MObject FootPrintNode::inputSize        = {};
MObject FootPrintNode::inputColor       = {};
MObject FootPrintNode::outputColor      = {};
MObject FootPrintNode::geometryChanging = {};

const MTypeId FootPrintNode::id                   = gPluginNodeId;
const MString FootPrintNode::drawDbClassification = gPluginDrawDbClassification;
const MString FootPrintNode::drawRegistrantId     = gPluginDrawRegistrantId;

FootPrintNode::FootPrintNode() {}
FootPrintNode::~FootPrintNode() {}

MStatus FootPrintNode::compute( const MPlug& plug, MDataBlock& data )
{
    // Check documentation in "class FootPrintNode" for descriptions about the attributes here
    if (plug.partialName() == "sz")
    {
        MStatus status;
        MDataHandle inputSizeHandle = data.inputValue(inputSize, &status);
        if (status != MStatus::kSuccess) return status;

        MDataHandle sizeHandle = data.outputValue(outputSize, &status);
        if (status != MStatus::kSuccess) return status;

        sizeHandle.copy(inputSizeHandle);
    }
    else if (plug.partialName() == "ocl")
    {
        MStatus status;
        MDataHandle inputColorHandle = data.inputValue(inputColor, &status);
        if (status != MStatus::kSuccess) return status;

        MDataHandle colorHandle = data.outputValue(outputColor, &status);
        if (status != MStatus::kSuccess) return status;

        colorHandle.copy(inputColorHandle);
    }
    else if (plug.partialName() == "gcg")
    {
        MStatus status;
        MDataHandle boolHandle = data.outputValue(geometryChanging, &status);
        if (status != MStatus::kSuccess) return status;
        boolHandle.setBool(true);
    }
    else
    {
        return MS::kUnknownParameter;
    }

    return MStatus::kSuccess;
}

/*
    Technique 1: Hack the EM to force evaluate and cache attributes.

    To improve performance, Evaluation Manager aggressively skips evaluation of attributes which are not connected to other nodes.
    In cases where an external user of DG data (in this case the renderer) 
    needs to read unconnected values from a node during or after EM evaluation,
    we need to take extra steps to ensure the data is evaluated by the EM (and cached).

    The most notable rules used by the EM for skipping evaluation are:
        1. Output attributes without output-connections are not computed in EM (and are not eligible for caching).
        2. Input attributes are never cached in Evaluation Cache.

    In FootPrintNode, attributes "outputSize", "outputColor", "geometryChanging" are virtually connected to the renderer.
    But EM does not understand these "virtual connection", and skips evaluation and caching for them.
    The current workaround are:
    1. To bypass rule 2, we made them a passing-through output attributes : inputSize->outputSize, inputColor->outputColor
    2. To bypass rule 1, repeat the affect relationship in setDependentsDirty(). [*]
        [*]	Note, this is a trick that relies on some internal hack to EM.
            Maya may provide better API for this in future updates. 
            When proper force evaluation API is come, you won't need to override this method.
*/
MStatus FootPrintNode::setDependentsDirty(const MPlug& plug, MPlugArray& plugArray)
{
    // Repeating the affect relationship we have specified
    // This method just mean to trick EM
    // No need to do this outside of EM graph construction (for the sake of performance)
    if (MEvaluationManager::graphConstructionActive())
    {
        if (plug.partialName() == "isz")
        {
            MObject thisNode = thisMObject();
            MPlug osPlug(thisNode, outputSize);
            MPlug scPlug(thisNode, geometryChanging);
            plugArray.append(osPlug);
            plugArray.append(scPlug);
        }

        MStatus status;
        MPlug parentPlug = plug.parent(&status);
        if (plug.partialName() == "ic" || (!parentPlug.isNull() && parentPlug.partialName() == "ic"))
        {
            MObject thisNode = thisMObject();
            MPlug ocPlug(thisNode, outputColor);
            plugArray.append(ocPlug);
        }
    }

    // Try not set any data or attribute value in this method
    // Because EM's parallel evaluation will not call this method at all
    // A widely used *bad* approach is to write "mGeometryChanged=true" when some attribute changed.
    // Use Technique 1.1 to avoid this.

    return MStatus::kSuccess;
}

MStatus FootPrintNode::postEvaluation(const MDGContext & context, const MEvaluationNode & evaluationNode, PostEvaluationType evalType)
{
    // For cache restoration only
    // This method is responsible for fixing the 'geometryChanging' flag in cache restore frames
    // Because in cache store phase,
    // PopulateGeometry & Viewport-Caching happens before Evaluation-Cache store
    // The value of 'geometryChanging' will always be set to 'false' (it is already used by render)
    // Thus, we have to fix the geometryChanging attribute to the correct value 
    MStatus status;
    if (evalType == PostEvaluationEnum::kEvaluatedDirectly && // kEvaluatedDirectly indicates we are restoring from cache
        evaluationNode.dirtyPlugExists(geometryChanging,&status) && status)
    {
        MDataBlock data = forceCache();
        MDataHandle boolHandle = data.outputValue(geometryChanging, &status);
        if (status != MStatus::kSuccess) return status;
        boolHandle.setBool(true);
    }
    return status;
}

void FootPrintNode::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo& disablingInfo, MNodeCacheSetupInfo& cacheSetupInfo, MObjectArray& monitoredAttributes) const
{
    MPxLocatorNode::getCacheSetup(evalNode, disablingInfo, cacheSetupInfo, monitoredAttributes);
    assert(!disablingInfo.getCacheDisabled());
    cacheSetupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
}

MBoundingBox FootPrintNode::boundingBox() const
{
    // Get the outputSize
    //
    MObject thisNode = thisMObject();
    MPlug plug( thisNode, outputSize );
    MDistance sizeVal;
    plug.getValue( sizeVal );

    double multiplier = sizeVal.asCentimeters();

    MPoint corner1( -0.17, 0.0, -0.7 );
    MPoint corner2( 0.17, 0.0, 0.3 );

    corner1 = corner1 * multiplier;
    corner2 = corner2 * multiplier;

    return MBoundingBox( corner1, corner2 );
}

// Get the footprint instance's color for rendering
// This code should only be called by a shader's PreDrawCallback
// See also, AnimatedColorShader::ShaderPreDrawCallback()
MColor FootPrintNode::renderColor() const
{
    // The data block should always be ready in render time
    MDataBlock block = const_cast<FootPrintNode*>(this)->forceCache();
    // Technique: Use outputValue() to read the attribute's value as-is (no evaluation)
    //		Never use inputValue() in renderer callbacks (except UpdateDG)
    MFloatVector color = block.outputValue(outputColor).asFloatVector();

    return MColor(color[0], color[1], color[2]);
}

// Must be called after MPxGeometryOverride::updateDG()
// Typically used by MPxGeometryOverride::requiresGeometryUpdate()
bool FootPrintNode::isGeometryChanging() const
{
    MDataBlock block = const_cast<FootPrintNode*>(this)->forceCache();
    // Use inputValue() to trigger evaluation here
    // Because MPxGeometryOverride::requiresGeometryUpdate() can be called outside of
    // MPxGeometryOverride::initialize()/updateDG()
    // This evaluation is safe because this attribute cannot be connected
    // And thus cannot reach other nodes
    return block.inputValue(FootPrintNode::geometryChanging).asBool();
}

// Workload for MPxGeometryOverride::updateDG()
// Updating all the attributes needed by the renderer
// Ensure these attributes can be accessed by outputValue() safely later
void FootPrintNode::updateRenderAttributes()
{
    MDataBlock datablock = forceCache();
    datablock.inputValue(FootPrintNode::outputColor);
    datablock.inputValue(FootPrintNode::geometryChanging);
    datablock.inputValue(FootPrintNode::outputSize);
}

// Should only be called from MPxGeometryOverride::populateGeometry()
// Returns the parameters required to update geometry
// Set "FootPrintNode::geometryChanging" to "false" to avoid duplicate update
// [[ensure : isGeometryChanging() == false]]
FootPrintNode::GeometryParameters FootPrintNode::updatingGeometry()
{
    MDataBlock block = const_cast<FootPrintNode*>(this)->forceCache();

    // Reset the geometryChanging attribute to false so that we do not update the geometry multiple times
    block.outputValue(FootPrintNode::geometryChanging).set(false);

    GeometryParameters param;
    param.Size = block.outputValue(FootPrintNode::outputSize).asDistance().asCentimeters();
    return param;
}

using namespace MHWRender;

struct ShaderDeleter {
    void operator()(MShaderInstance* shader) {
        auto* rednerer = MRenderer::theRenderer();
        auto* mgr = rednerer ? rednerer->getShaderManager() : nullptr;
        if (mgr)
            mgr->releaseShader(shader);
    }
};

// Animated material shader (color)
// 
// This class encapsulated a MShaderInstance with a predefined PreDrawCallback
// So that we can update the shader's simple-parameters when time change
// In this case, the shader's PreDrawCallback will always update the shader's "solidColor" parameter
class AnimatedColorShader {
public:
    // Demand a solid color instance with the proper PreDrawCallback setup
    MShaderInstance* get();

    // Release the shader
    void reset() { mAnimateColorShader.reset(); }

private:
    // Shader Pre-draw callback for update the animated color
    static void ShaderPreDrawCallback(MDrawContext& context, const MRenderItemList& renderItemList, MShaderInstance *shader);

    std::unique_ptr<MShaderInstance, ShaderDeleter> mAnimateColorShader;
};

// Custom data attached to the MRenderItem created for FootPrintNode
// Contains simply a pointer back to the node
// Check AnimatedColorShader::ShaderPreDrawCallback() and FootPrintGeometryOverrideAnimatedMaterial::updateRenderItems() for detail
class FootprintNodeRenderItemData : public MUserData
{
public:
    FootprintNodeRenderItemData(FootPrintNode* node)
        : Node(node)
    {}

    FootPrintNode* Node;
};

// A simple store for sharing solid-color-shader with the given color
// This class is not memory-efficient
// One should add recycling mechanism for production usage
class SolidColorShaderStore
{
public:
    MShaderInstance* get(MColor color);
    void clear() { mShaders.clear(); }

private:
    static std::uint32_t encode(MColor color);
    std::unordered_map<std::uint32_t, std::unique_ptr<MShaderInstance, ShaderDeleter>> mShaders; // Map a R8B8G8A8 color to a shader instance
};

//---------------------------------------------------------------------------
// Global variables shared by all Footprint nodes
//---------------------------------------------------------------------------
// 
// Do not use C++ class as global variables, because .dll/.so's actual unload time is managed by the OS
// Having a non-trivial DLL exit function called at a bad time may CRASH Maya
// And you should put all the startup / cleanup work in the initializePlugin() / uninitializePlugin() function
//
// Details:
// Case 1 : User is unloading this plugin without closing Maya
//			Everything goes well, uninitializePlugin() is called before the global variable destructor
//			We need to ensure all memories and shaders are being released
//			uninitializePlugin() will call Globals->clear() to clear everything
// Case 2 : User is closing Maya with this plugin loaded
//			In this case, uninitializePlugin() will NOT be called
//			And calling MShaderManager::ReleaseShader will CRASH Maya at exit time
//			Thus, we just leave whatever memories we allocated as-is
//			Since the application is closing anyway
struct GlobalVariables {
    // A helper that shares all static solid color shader with same color (for drawing highlighted footprints)
    SolidColorShaderStore solidColorShaderStore;
};

// Note, do not use std::unique_ptr<GlobalVariables> here
// Because we don't release memory / shaders at Maya exit
static GlobalVariables* Globals = nullptr;

// Internal constants
// Primitive type constants are OK to be declared globally
static const /*constexpr*/ MString ColorParameterName = "solidColor";
static const /*constexpr*/ MString WireframeItemName  = "footPrintLocatorWires";
static const /*constexpr*/ MString ShadedItemName     = "footPrintLocatorTriangles";
static const /*constexpr*/ MColor  ErrorColor         = { 1.0F, 0.0F,0.0F,1.0F };

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Viewport 2 GeometryOverride class declaration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// Customized rendering logic of FootPrintNode
// - This class is responsible for generating the geometry (vertex buffer) from the procedure geometry (defined by FootPrintNode::Size)
// - Also handles the proper shader for rendering it ( animated color shader )
// Beside rendering, the functions for generating geometry is also a crucial part in Viewport Caching (in contrast with Evaluation Caching)
class FootPrintGeometryOverrideAnimatedMaterial : public MPxGeometryOverride
{
public:
    static MPxGeometryOverride* Creator(const MObject& obj) { return new FootPrintGeometryOverrideAnimatedMaterial(obj); }
    ~FootPrintGeometryOverrideAnimatedMaterial() override;

    // Support configuration functions :
    DrawAPI supportedDrawAPIs() const override			{ return (DrawAPI::kOpenGL | DrawAPI::kDirectX11 | DrawAPI::kOpenGLCoreProfile); }
    bool hasUIDrawables() const override							{ return false; }
    bool supportsEvaluationManagerParallelUpdate() const override	{ return true; }
    // Supporting Viewport Caching (VP2 Custom Caching)
    //		Viewport Cache for customize rendering nodes is caching the geometry data 
    //		generated by MPxGeometryOverride::populateGeometry()
    //		Like the graphics API, the geometry data is usually expressed in three different objects:
    //			- Vertex Buffer		:	Viewport Cache
    //			- Index	Buffer		:	No cache, must be time-independent
    //			- Uniform Buffer	:	Evaluation Cache
    //		Currently, *only* Vertex Buffer can be cached in Viewport Cache 
    //		And all the data in Uniform Buffer (shader parameters such as Color), must be stored in Evaluation Cache
    //		Viewport Caching is helpful if you are generating the geometry data in CPU
    //		For GPU generated geometry, the benefit is limited
    //		To make the Viewport caching works properly the following constraints must be meet
    //			- requiresUpdateRenderItems(path) always return 'false' in background-dg-context.
    //			- requiresGeometryUpdate() always return 'true' when restored from cache (if geometry is animated).
    //			- populateGeometry() should be context-safe
    //		Check these methods for more detail
    bool supportsVP2CustomCaching() const override					{ return true; }
    void configCache(const MEvaluationNode& evalNode, MCacheSchema& schema) const override;


    // Interaction with FootPrintNode
    //	- This is the only method you can call Mplug::getValue() or Mdatablock::inputValue()
    //  - This method can be empty (in EM modes), if you have setup the node correctly with Technique 1.
    void updateDG() override;
    // For the best practice, this method should contains no status data
    void cleanUp() override {};

    // Render item functions, only involves in foreground rendering, should not affect VP2 Caching at all
    bool requiresUpdateRenderItems(const MDagPath& path) const override;
    void updateRenderItems(const MDagPath &path, MRenderItemList& list) override;

    // Geometry update functions, major entry for support VP2 Custom Caching
    bool requiresGeometryUpdate() const override;
    bool isIndexingDirty(const MRenderItem &item) override			{ return false; } // Viewport Caching does not support index buffer change (animated topology)
    void populateGeometry(const MGeometryRequirements &requirements, const MRenderItemList &renderItems, MGeometry &data) override;

    // Debugging functions
    bool traceCallSequence() const override;
    void handleTraceMessage(const MString &message) const override;

private:
    FootPrintGeometryOverrideAnimatedMaterial(const MObject& obj);

protected:
    // Helpers for generating the footprint geometry
    void fillFootprintSolidIndices(int numIndex, unsigned int * indices);
    void fillFootprintWireframeIndices(int numIndex, unsigned int * indices);
    void fillFootPrintVertices(float * vertices, float outputSize);

private:
    // To ensure the plugin works well with Viewport and Evaluation Caching
    // We recommend to use a state-less MpxGeometryOverride:
    // 
    //	- Store all time-dependent data on the attributes of corresponding Maya node, Instead of a data member in MpxGeometryOverride.
    //	- updateDG() pulls (evaluates) all the time-dependent data on node by "MDataBlock::inputValue()",
    //	  but *not* store them in a data member of MPxGeometryOverride
    //	- Always access the time-dependent data by "MDataBlock::outputValue()" in the renderer
    //
    // Backgrounds of the recommendation : 
    // 
    // In this plugin example, we have 3 different pipeline stages involved:
    //	1. Node Evaluation
    //	2. Geometry Update (MPxGeometryOverride invocation)
    //	3. Rendering (Shader-pre-draw-callback invocation)
    // 
    // With (Evaluation or Viewport) Caching, some surprising mixture of stages happen concurrently :
    // 
    //	- Foreground thread : -- MPxGeometryOverride ------>------ Shader Callback -------->	(Rendering on frame 10)
    //	- Background thread : -------- Node Evaluation ----->-- MPxGeometryOverride ------->	(Viewport cache fill on frame 20)
    //
    // Accessing the same data directly cross stage boundaries can cause incorrect result or even crash maya.
    // Thus we must use different storage for each data in each DG context (frame).
    // And, a depend node's attribute are always isolated for different context.
    //
    // Beside, within the MPxGeometryOverride invocation stage, it is not always safe to access its data member :
    // During Viewport Cache Restoration, requiresGeometryUpdate() is the only method get called. (No updateDG() call)
    // Thus the data must be stored in the node.
    // 
    FootPrintNode*				mFootPrintNode;				// The node we are rendering
    AnimatedColorShader			mAnimateColorShader;		// Shader for render the shaded item with animated color
};

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// VP2 GeometryOverride implementation
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

FootPrintGeometryOverrideAnimatedMaterial::FootPrintGeometryOverrideAnimatedMaterial(const MObject& obj)
: MPxGeometryOverride(obj)
, mFootPrintNode(nullptr)
{
    MStatus returnStatus;
    MFnDependencyNode dependNode(obj, &returnStatus);
    if (returnStatus != MStatus::kSuccess) return;
    MPxNode* footPrintNode = dependNode.userNode(&returnStatus);
    if (returnStatus != MStatus::kSuccess) footPrintNode = nullptr;
    mFootPrintNode = dynamic_cast<FootPrintNode*>(footPrintNode);
}

FootPrintGeometryOverrideAnimatedMaterial::~FootPrintGeometryOverrideAnimatedMaterial()
{}

void FootPrintGeometryOverrideAnimatedMaterial::configCache(const MEvaluationNode& evalNode, MCacheSchema& schema) const
{
    // Rendering uses the output color, so we have to make sure it is cached.
    // If Evaluation Cache is not enabled on this node, we have to add it
    // to the list of attributes to cache.
    if (evalNode.dirtyPlugExists(FootPrintNode::outputColor)) {
        MStatus status = schema.add(FootPrintNode::outputColor);
        assert(status);
    }
}

void FootPrintGeometryOverrideAnimatedMaterial::updateDG()
{
    // Pull (evaluate) all attributes in FootPrintNode node we will be using
    // Here is the list of attributes we are pulling : 
    // - geometryChanging	: Needed by requiresGeometryUpdate()				To check if we needs to update the vertex and index buffer
    // - outputSize			: Needed by populateGeometry()						To generate the geometry
    // - outputColor		: Needed by mAnimateColorShader.PreDrawCallback()	To draw the geometry
    //
    // It is very important that all the attributes pulled from this method are cached (with Technique 1)
    // Otherwise Evaluation Cache will not work
    // In fact, this method is not needed by EM Evaluation modes
    mFootPrintNode->updateRenderAttributes();
}

bool FootPrintGeometryOverrideAnimatedMaterial::requiresUpdateRenderItems(const MDagPath & path) const 
{
    // Override this function if you need a more complicated animated-material behavior
    // For example, you will need to change this to `return true` if object's associated shader
    // is changing at every frame. (This could be very expensive)
    // Note, this method have to return 'false' in most case, otherwise VP2 caching may not work
    return false;
}

void FootPrintGeometryOverrideAnimatedMaterial::updateRenderItems( const MDagPath& path, MRenderItemList& list )
{
    // This method will only happen for rendering, but not VP2 caching
    // And it is usually call when the selection status of the object is changed

    DisplayStatus displayStatus = MGeometryUtilities::displayStatus(path);

    // Simplify the display status to [active, normal]
    bool active =	displayStatus == DisplayStatus::kLead || 
                    displayStatus == DisplayStatus::kActive || 
                    displayStatus == DisplayStatus::kHilite || 
                    displayStatus == DisplayStatus::kActiveComponent;

    MShaderInstance* shader;
    if (active)
    {
        // Get an static solid color shader for selection highlight
        // Note, by sharing the solid color shader, we allow VP2 to consolidate multiple geometries together during the drawing process
        MColor color = MGeometryUtilities::wireframeColor(path);
        shader = Globals->solidColorShaderStore.get(color);
    } else {
        // Get the animated color shader
        // Traditionally, we will need to setup the shader's "solidColor" parameter here to implement animated material
        // This is very inefficient because it will cause a full set of 'render-item-change'->'geometry-change'
        // And reseting all the assets loaded into VP2
        // Thus, we will use the ShaderPreDrawCallback here to reduce the cost
        // 
        // The shader's PreDrawCallback is configured to set its "solidColor" parameter with "FootPrintNode::renderColor()" before drawing
        // Thus, we do not need to change the shader's color parameter here
        // Note, FootPrintNode::renderColor() will not evaluate the "FootPrintNode::outputColor" attribute
        // Thus, it is important to update the attribute in updateDG()
        shader = mAnimateColorShader.get();
    }

    // Create the wireframe render item
    int index = list.indexOf(WireframeItemName);
    MRenderItem* wireframeItem = nullptr;
    if (index < 0)
    {
        wireframeItem = MRenderItem::Create(
            WireframeItemName,
            MRenderItem::DecorationItem,
            MGeometry::kLines);

        wireframeItem->setDrawMode(MGeometry::kWireframe);
        wireframeItem->enable(true);
        wireframeItem->depthPriority(MRenderItem::sDormantWireDepthPriority);

        // Attach a pointer back to the node, to retrieve the color at shader-pre-draw time
        auto shared_ptr = MSharedPtr<FootprintNodeRenderItemData>::make(mFootPrintNode);
        wireframeItem->setCustomData(shared_ptr);
        list.append(wireframeItem);
    } else {
        wireframeItem = list.itemAt(index);
    }

    if (wireframeItem)
    {
        wireframeItem->setShader(shader);
    }

    MRenderItem* shadedItem = nullptr;
    index = list.indexOf(ShadedItemName);
    if (index < 0)
    {
        shadedItem = MRenderItem::Create(
            ShadedItemName,
            MRenderItem::DecorationItem,
            MGeometry::kTriangles);
        shadedItem->setDrawMode((MGeometry::DrawMode)(MGeometry::kShaded | MGeometry::kTextured));
        shadedItem->depthPriority(MRenderItem::sDormantFilledDepthPriority);
        shadedItem->enable(true);
        auto shared_ptr = MSharedPtr<FootprintNodeRenderItemData>::make(mFootPrintNode);
        shadedItem->setCustomData(shared_ptr);
        list.append(shadedItem);
    } else{
        shadedItem = list.itemAt(index);
    }

    // Update the shader based on if it is selected
    if (shadedItem)
    {
        shadedItem->setShader(shader);
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Animated material implementations
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void AnimatedColorShader::ShaderPreDrawCallback(MDrawContext& context, const MRenderItemList& renderItemList, MShaderInstance *shader)
{
    MColor color = ErrorColor;

    // Get the color from the render-item
    // Just check the first render item is sufficient in this case
    const MRenderItem* renderItem = renderItemList.itemAt(0);
    if (renderItemList.length() > 0)
    {
        // You may want to use dynamic_cast for extra safty
        auto shared_ptr = renderItem->getCustomData();
        auto customData = static_cast<FootprintNodeRenderItemData*>(shared_ptr.get());
        auto footprint = customData ? customData->Node : nullptr;

        // Note, the call `footprint->renderColor()` cannot invoke DG evaluation
        color = footprint ?
            footprint->renderColor() : 
            ErrorColor;
    }

    // Set the color parameter for the shader
    shader->setParameter(ColorParameterName, &color.r);
}

MShaderInstance* AnimatedColorShader::get() {
    if (mAnimateColorShader)
        return mAnimateColorShader.get();

    MShaderInstance* shader = nullptr;
    MRenderer* renderer = MRenderer::theRenderer();
    if (renderer)
    {
        const MShaderManager* shaderMgr = renderer->getShaderManager();
        if (shaderMgr)
        {
            shader = shaderMgr->getStockShader(
                MShaderManager::k3dSolidShader,
                &AnimatedColorShader::ShaderPreDrawCallback);
            mAnimateColorShader.reset(shader);
        }
    }

    return shader;
}

// Encode a MColor with R8B8G8A8 encoding
std::uint32_t SolidColorShaderStore::encode(MColor color)
{
    constexpr int bitsPerChannel = 8;
    constexpr std::uint32_t mask = (1 << bitsPerChannel) - 1;
    return
        (static_cast<std::uint32_t>(color.r * mask) << (bitsPerChannel * 0)) |
        (static_cast<std::uint32_t>(color.g * mask) << (bitsPerChannel * 1)) |
        (static_cast<std::uint32_t>(color.b * mask) << (bitsPerChannel * 2)) |
        (static_cast<std::uint32_t>(color.a * mask) << (bitsPerChannel * 3));
}

// Get or create a solid color shader of the given color
MShaderInstance * SolidColorShaderStore::get(MColor color) {
    auto& stored = mShaders[encode(color)];
    if (!stored)
    {
        MShaderInstance* shader = nullptr;
        MRenderer* renderer = MRenderer::theRenderer();
        if (renderer)
        {
            const MShaderManager* shaderMgr = renderer->getShaderManager();
            if (shaderMgr)
            {
                shader = shaderMgr->getStockShader(MShaderManager::k3dSolidShader);
                shader->setParameter(ColorParameterName, &color.r);
                stored.reset(shader);
            }
        }
    }
    return stored.get();
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Geometry update and VP2 cache implementations
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// Return true when 'inputSize' is changed in the FootPrintNode
// Which requires us to re-generate the geometry
// Note, this method must return the exact same value in a cache-restoration frame
// as it was in the corresponding cache-store frame
bool FootPrintGeometryOverrideAnimatedMaterial::requiresGeometryUpdate() const
{
    // Checking the "FootPrintNode::geometryChanging" attribute
    // If any attribute affecting the geometry is changing, "FootPrintNode::geometryChanging" should be affected and dirtied
    // Also check FootPrintNode::updatingGeometry() 
    // Warning, this method may be called outside of regular { update() : cleanUp() } scope.
    // Thus, we must invoke node-local evaluation to ensure the correctness
    return mFootPrintNode->isGeometryChanging();
}

namespace
{
    static constexpr int soleCount = 21;
    static constexpr int heelCount = 17;
}

// Generate the geometry(vertex / index) from FootPrintNode's parameter data
// [[ensure : requiresGeometryUpdate() == false]]
void FootPrintGeometryOverrideAnimatedMaterial::populateGeometry(
    const MGeometryRequirements& requirements,
    const MRenderItemList& renderItems,
    MGeometry& data)
{
    // This call will ensure the post-condition that requiresGeometryUpdate() is false
    FootPrintNode::GeometryParameters footprintParam = mFootPrintNode->updatingGeometry();

    // Generating the vertex and index buffer from the parameters
    MVertexBuffer* verticesBuffer       = nullptr;
    float* vertices                     = nullptr;

    const MVertexBufferDescriptorList& vertexBufferDescriptorList = requirements.vertexRequirements();
    const int numberOfVertexRequirments = vertexBufferDescriptorList.length();

    MVertexBufferDescriptor vertexBufferDescriptor;
    for (int requirmentNumber = 0; requirmentNumber < numberOfVertexRequirments; ++requirmentNumber)
    {
        if (!vertexBufferDescriptorList.getDescriptor(requirmentNumber, vertexBufferDescriptor))
        {
            continue;
        }

        switch (vertexBufferDescriptor.semantic())
        {
        case MGeometry::kPosition:
            {
                if (!verticesBuffer)
                {
                    verticesBuffer = data.createVertexBuffer(vertexBufferDescriptor);
                    if (verticesBuffer)
                    {
                        vertices = (float*)verticesBuffer->acquire(soleCount+heelCount, false);
                    }
                }
            }
            break;
        default:
            // do nothing for stuff we don't understand
            break;
        }
    }

    fillFootPrintVertices(vertices, footprintParam.Size);

    if(verticesBuffer && vertices)
    {
        verticesBuffer->commit(vertices);
    }

    // Now fill the index data
    // Index data should not change with size
    for (int i=0; i < renderItems.length(); ++i)
    {
        const MRenderItem* item = renderItems.itemAt(i);

        if (!item)
        {
            continue;
        }

        MIndexBuffer* indexBuffer = data.createIndexBuffer(MGeometry::kUnsignedInt32);

        if (item->name() == WireframeItemName )
        {
            int numPrimitive = heelCount + soleCount - 2;
            int numIndex = numPrimitive * 2;

            unsigned int* indices = (unsigned int*)indexBuffer->acquire(numIndex, false);

            fillFootprintWireframeIndices(numIndex, indices);

            indexBuffer->commit(indices);
        }
        else if (item->name() == ShadedItemName )
        {
            int numPrimitive = heelCount + soleCount - 4;
            int numIndex = numPrimitive * 3;

            unsigned int* indices = (unsigned int*)indexBuffer->acquire(numIndex, false);
            fillFootprintSolidIndices(numIndex, indices);

            indexBuffer->commit(indices);
        }

        item->associateWithIndexBuffer(indexBuffer);
    }
}

namespace {
    // Foot Data
//
    float sole[][3] = { {  0.00f, 0.0f, -0.70f },
                        {  0.04f, 0.0f, -0.69f },
                        {  0.09f, 0.0f, -0.65f },
                        {  0.13f, 0.0f, -0.61f },
                        {  0.16f, 0.0f, -0.54f },
                        {  0.17f, 0.0f, -0.46f },
                        {  0.17f, 0.0f, -0.35f },
                        {  0.16f, 0.0f, -0.25f },
                        {  0.15f, 0.0f, -0.14f },
                        {  0.13f, 0.0f,  0.00f },
                        {  0.00f, 0.0f,  0.00f },
                        { -0.13f, 0.0f,  0.00f },
                        { -0.15f, 0.0f, -0.14f },
                        { -0.16f, 0.0f, -0.25f },
                        { -0.17f, 0.0f, -0.35f },
                        { -0.17f, 0.0f, -0.46f },
                        { -0.16f, 0.0f, -0.54f },
                        { -0.13f, 0.0f, -0.61f },
                        { -0.09f, 0.0f, -0.65f },
                        { -0.04f, 0.0f, -0.69f },
                        { -0.00f, 0.0f, -0.70f } };
    float heel[][3] = { {  0.00f, 0.0f,  0.06f },
                        {  0.13f, 0.0f,  0.06f },
                        {  0.14f, 0.0f,  0.15f },
                        {  0.14f, 0.0f,  0.21f },
                        {  0.13f, 0.0f,  0.25f },
                        {  0.11f, 0.0f,  0.28f },
                        {  0.09f, 0.0f,  0.29f },
                        {  0.04f, 0.0f,  0.30f },
                        {  0.00f, 0.0f,  0.30f },
                        { -0.04f, 0.0f,  0.30f },
                        { -0.09f, 0.0f,  0.29f },
                        { -0.11f, 0.0f,  0.28f },
                        { -0.13f, 0.0f,  0.25f },
                        { -0.14f, 0.0f,  0.21f },
                        { -0.14f, 0.0f,  0.15f },
                        { -0.13f, 0.0f,  0.06f },
                        { -0.00f, 0.0f,  0.06f } };
}

void FootPrintGeometryOverrideAnimatedMaterial::fillFootprintSolidIndices(int numIndex, unsigned int * indices)
{
    int primitiveIndex = 0;
    int startIndex = 0;

    for (int i = 0; i < numIndex; )
    {
        if (i < (heelCount - 2) * 3)
        {
            startIndex = 0;
            primitiveIndex = i / 3;
        }
        else
        {
            startIndex = heelCount;
            primitiveIndex = i / 3 - heelCount + 2;
        }
        indices[i++] = startIndex;
        indices[i++] = startIndex + primitiveIndex + 1;
        indices[i++] = startIndex + primitiveIndex + 2;
    }
}

void FootPrintGeometryOverrideAnimatedMaterial::fillFootprintWireframeIndices(int numIndex, unsigned int * indices)
{
    int primitiveIndex = 0;
    int startIndex = 0;
    for (int i = 0; i < numIndex; )
    {
        if (i < (heelCount - 1) * 2)
        {
            startIndex = 0;
            primitiveIndex = i / 2;
        }
        else
        {
            startIndex = heelCount;
            primitiveIndex = i / 2 - heelCount + 1;
        }
        indices[i++] = startIndex + primitiveIndex;
        indices[i++] = startIndex + primitiveIndex + 1;
    }
}

void FootPrintGeometryOverrideAnimatedMaterial::fillFootPrintVertices(float * vertices, float outputSize)
{
    int verticesPointerOffset = 0;

    // We concatenate the heel and sole positions into a single vertex buffer.
    // The index buffers will decide which positions will be selected for each render items.
    for (int currentVertex = 0; currentVertex < soleCount + heelCount; ++currentVertex)
    {
        if (vertices)
        {
            if (currentVertex < heelCount)
            {
                int heelVtx = currentVertex;
                vertices[verticesPointerOffset++] = heel[heelVtx][0] * outputSize;
                vertices[verticesPointerOffset++] = heel[heelVtx][1] * outputSize;
                vertices[verticesPointerOffset++] = heel[heelVtx][2] * outputSize;
            }
            else
            {
                int soleVtx = currentVertex - heelCount;
                vertices[verticesPointerOffset++] = sole[soleVtx][0] * outputSize;
                vertices[verticesPointerOffset++] = sole[soleVtx][1] * outputSize;
                vertices[verticesPointerOffset++] = sole[soleVtx][2] * outputSize;
            }
        }
    }
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Debug functions
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

// Return true if internal tracing is desired.
bool FootPrintGeometryOverrideAnimatedMaterial::traceCallSequence() const
{
    /*
    Tracing will look something like the following when in shaded mode (on selection change):

        FootPrintGeometryOverrideAnimatedMaterial: Geometry override DG update: footPrint1
        FootPrintGeometryOverrideAnimatedMaterial: Start geometry override render item update: |transform1|footPrint1
        FootPrintGeometryOverrideAnimatedMaterial: - Call API to update render items
        FootPrintGeometryOverrideAnimatedMaterial: End geometry override render item update: |transform1|footPrint1
        FootPrintGeometryOverrideAnimatedMaterial: End geometry override clean up: footPrint1

    This is based on the existing stream and indexing dirty flags being used
    which attempts to minimize the amount of render item, vertex buffer and indexing update.
    */
    return false;
}

inline void FootPrintGeometryOverrideAnimatedMaterial::handleTraceMessage(const MString & message) const
{
    MGlobal::displayInfo(gPluginNodeMessagePrefix + message);

    // Some simple custom message formatting.
    fputs(gPluginNodeMessagePrefix, stderr);
    fputs(message.asChar(), stderr);
    fputs("\n", stderr);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Plugin Registration
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

MStatus FootPrintNode::initialize()
{
    // Setup the FootPrintNode's attributes
    // Check the documentation in "class FootPrintNode" for detail

    MFnUnitAttribute unitFn;
    MStatus			 stat;

    outputSize = unitFn.create("outputSize", "sz", MFnUnitAttribute::kDistance);
    unitFn.setDefault(1.0);
    unitFn.setWritable(false);
    stat = addAttribute(outputSize);

    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }

    inputSize = unitFn.create("inputSize", "isz", MFnUnitAttribute::kDistance);
    unitFn.setDefault(1.0);
    stat = addAttribute(inputSize);

    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }


    MFnNumericAttribute attrFn;
    geometryChanging = attrFn.create("geometryChanging", "gcg", MFnNumericData::kBoolean, true);
    attrFn.setStorable(false);
    attrFn.setHidden(true);
    attrFn.setConnectable(false);
    stat = addAttribute(geometryChanging);
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }


    MFnNumericAttribute nAttr;
    inputColor = nAttr.createColor("inputColor", "ic");
    nAttr.setStorable(true);
    nAttr.setKeyable(true);
    nAttr.setDefault(0.6f, 0.6f, 0.6f);
    stat = addAttribute(inputColor);
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }

    outputColor = nAttr.createColor("outputColor", "ocl", &stat);
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }
    nAttr.setStorable(false);
    nAttr.setKeyable(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    stat = addAttribute(outputColor);
    if (!stat) {
        stat.perror("addAttribute");
        return stat;
    }

    attributeAffects(inputSize, outputSize);
    attributeAffects(inputSize, geometryChanging);
    attributeAffects(inputColor, outputColor);

    return MS::kSuccess;
}

MStatus initializePlugin(MObject obj)
{
    MStatus   status;
    MFnPlugin plugin(obj, PLUGIN_COMPANY, "3.0", "Any");

    // Initialize the global variables
    Globals = new GlobalVariables();

    status = plugin.registerNode(
        gPluginNodeName,
        FootPrintNode::id,
        &FootPrintNode::creator,
        &FootPrintNode::initialize,
        MPxNode::kLocatorNode,
        &FootPrintNode::drawDbClassification);
    if (!status) {
        status.perror("registerNode");
        return status;
    }

    status = MDrawRegistry::registerGeometryOverrideCreator(
        FootPrintNode::drawDbClassification,
        FootPrintNode::drawRegistrantId,
        FootPrintGeometryOverrideAnimatedMaterial::Creator);
    if (!status) {
        status.perror("registerDrawOverrideCreator");
        return status;
    }

    // Register a custom selection mask with priority 2 (same as locators
    // by default).
    MSelectionMask::registerSelectionType(gPluginSelectionMask, 2);

    char cmd[256];
    snprintf(cmd, sizeof(cmd), "selectType -byName \"%s\" 1", gPluginSelectionMask);
    status = MGlobal::executeCommand(cmd);

    return status;
}

MStatus uninitializePlugin(MObject obj)
{
    delete Globals;

    // Do not check the return code and return here
    // Plugin uninitialization should never fail
    MDrawRegistry::deregisterGeometryOverrideCreator(
        FootPrintNode::drawDbClassification,
        FootPrintNode::drawRegistrantId);

    MFnPlugin plugin(obj);
    plugin.deregisterNode(FootPrintNode::id);

    MSelectionMask::deregisterSelectionType(gPluginSelectionMask);

    // There is nothing we can do if any of the deregister process failed
    return MStatus::kSuccess;
}
