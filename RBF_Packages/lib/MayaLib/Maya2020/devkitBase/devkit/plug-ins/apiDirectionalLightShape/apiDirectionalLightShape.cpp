//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

///////////////////////////////////////////////////////////////////////////////
//
// DescriptioN:
// 
//	Creates plug-in shapes which is classified generally as a light
//	and specifically as a VP2 point, area, directional or spot lights.
//
//  The second classification allows for the usage of the internal
//	light and UI (geometry drawing) VP2 evluators to be used.
//	Note that there is no explicit VP2 override class used as
//	the internal VP2 geometry evaluator will handle drawing.
//
//	Attributes which control the light are created to match names on
//	a Maya lights so that they will be picked up when DG
//	evaluation occurs.
//
// Usage:	Create node and set it's object color to a one of the user defined 
//			preference colors:
//
//	string $node = `shadingNode -asLight apiDirectionalLightShape`;
//	color -ud 7 $node;
//
//  string $node2 = `shadingNode -asLight apiSpotLightShape`;
//  string $node3 = `shadingNode -asLight apiAreaLightShape`;
//  string $node4 = `shadingNode -asLight apiPointLightShape`;
//  string $node5 = `shadingNode -asLight apiImageLightShape`;
//
// to create nodes which is categorized one of the internal light types.
//
//  string $node5 = `shadingNode -asLight apiCusomtDirectionalLightShape`;
//
// to create nodes which have custom UI but use internal directional light
// evaluation.
//
// Caveats:
//	Not all attributes are currently supported fully.
//
////////////////////////////////////////////////////////////////////////////////

#include <math.h>           
#include <maya/MIOStream.h>

#include <maya/MFnPlugin.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MPxGeometryOverride.h>
#include <maya/MDrawRegistry.h>
#include <maya/MHWGeometryUtilities.h>
#include <maya/MEventMessage.h>
#include <maya/MFnDependencyNode.h>

#include "apiDirectionalLightShape.h"           

////////////////////////////////////////////////////////////////////////////////
//
// Light shape implementation
//
////////////////////////////////////////////////////////////////////////////////

MTypeId apiDirectionalLightShape::id( 0x00081103 );
MTypeId apiSpotLightShape::id( 0x00081104 );
MTypeId apiPointLightShape::id( 0x00081105 );
MTypeId apiAreaLightShape::id( 0x00081106 );
MTypeId apiCustomDirectionalLightShape::id( 0x00081107 );
MTypeId apiImageLightShape::id( 0x00081108 );

// Generic Input attributes
MObject  apiLightShape::aColor;
MObject  apiLightShape::aIntensity; 
MObject  apiLightShape::aExposure; 
MObject  apiLightShape::aEmitDiffuse;
MObject  apiLightShape::aEmitSpecular;
MObject  apiLightShape::aDecayRate;
MObject  apiLightShape::aLocatorScale;
// Attributes for lights which shadow
MObject  apiShadowingLightShape::aUseDepthMapShadows;
MObject  apiShadowingLightShape::aUseRayTraceShadows;
MObject  apiShadowingLightShape::aShadowColor;
MObject  apiShadowingLightShape::aDepthMapResolution;
// Input spotlight attributes
MObject  apiSpotLightShape::aConeAngle;
MObject  apiSpotLightShape::aPenumbraAngle;
MObject  apiSpotLightShape::aDropOff;

// Output attributes
MObject  apiLightShape::aOutColor;

#define MAKE_INPUT(attr)	\
    CHECK_MSTATUS(attr.setKeyable(true) );		\
    CHECK_MSTATUS(attr.setStorable(true) );		\
    CHECK_MSTATUS(attr.setReadable(true) );		\
    CHECK_MSTATUS(attr.setWritable(true) );		\
	CHECK_MSTATUS(attr.setAffectsAppearance(true) );


#define MAKE_OUTPUT(attr)	\
    CHECK_MSTATUS(attr.setKeyable(false) );		\
    CHECK_MSTATUS(attr.setStorable(false) );	\
    CHECK_MSTATUS(attr.setReadable(true) ); 	\
    CHECK_MSTATUS(attr.setWritable(false) );

//////////////////////////////////////////////////////////////////
// Base light
//////////////////////////////////////////////////////////////////
MStatus apiLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya light.
//
{ 
	MFnNumericAttribute nAttr; 

    aColor = nAttr.createColor( "color", "cl" );
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.5f, 0.5f, 0.5f) );

    aEmitDiffuse = nAttr.create( "emitDiffuse", "ed", MFnNumericData::kBoolean);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(true) );

    aEmitSpecular = nAttr.create( "emitSpecular", "sn", MFnNumericData::kBoolean);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(true) );

    aIntensity = nAttr.create( "intensity", "i", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(1.0f) );

    aExposure = nAttr.create( "exposure", "exp", MFnNumericData::kFloat);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.0f) );
	
	aLocatorScale = nAttr.create( "locatorScale", "lls", MFnNumericData::kDouble);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(1.0) );
	    
    aDecayRate = nAttr.create( "decayRate", "de", MFnNumericData::kShort);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0) );    

    CHECK_MSTATUS ( addAttribute(aColor) ); 
    CHECK_MSTATUS ( addAttribute(aIntensity) );
    CHECK_MSTATUS ( addAttribute(aExposure) );
    CHECK_MSTATUS ( addAttribute(aEmitDiffuse) );
    CHECK_MSTATUS ( addAttribute(aEmitSpecular) );
    CHECK_MSTATUS ( addAttribute(aLocatorScale) );
    CHECK_MSTATUS ( addAttribute(aDecayRate) );	
	
	// Note that "oc" conflicts with objectColor so we use something else.
    aOutColor = nAttr.createColor( "outColor", "ocl" );
	MAKE_OUTPUT(nAttr);
    CHECK_MSTATUS ( addAttribute(aOutColor) );	

	CHECK_MSTATUS(attributeAffects(aColor, aOutColor));
	CHECK_MSTATUS(attributeAffects(aIntensity, aOutColor));
	CHECK_MSTATUS(attributeAffects(aExposure, aOutColor));
	CHECK_MSTATUS(attributeAffects(aEmitDiffuse, aOutColor));
	CHECK_MSTATUS(attributeAffects(aEmitSpecular, aOutColor));

    return MS::kSuccess;
}

//
// Description:
//		Compute output
//
MStatus apiLightShape::compute(const MPlug& plug,
											MDataBlock& block )
{
	if ((plug != aOutColor) && (plug.parent() != aOutColor))
	{
		return MS::kUnknownParameter;
	}

	// Set outColor be color
	//
	MFloatVector resultColor = block.inputValue( aColor ).asFloatVector();
	MDataHandle outColorHandle = block.outputValue( aOutColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

	return MS::kSuccess;
}


//////////////////////////////////////////////////////////////////
// Shadowing light
//////////////////////////////////////////////////////////////////
MStatus apiShadowingLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya shadowing light.
//
{ 
	apiLightShape::initialize();

	MFnNumericAttribute nAttr; 

    aUseDepthMapShadows = nAttr.create( "useDepthMapShadows", "dms", MFnNumericData::kBoolean);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(false) );

	aUseRayTraceShadows = nAttr.create( "useRayTraceShadows", "urs", MFnNumericData::kBoolean);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(true) );

	aShadowColor = nAttr.createColor( "shadowColor", "sc" );
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.0f, 0.0f, 0.0f) );

    aDepthMapResolution = nAttr.create( "dmapResolution", "dr", MFnNumericData::kShort);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(512) );    

    CHECK_MSTATUS ( addAttribute(aUseDepthMapShadows) );
    CHECK_MSTATUS ( addAttribute(aUseRayTraceShadows) );
    CHECK_MSTATUS ( addAttribute(aShadowColor) );
    CHECK_MSTATUS ( addAttribute(aDepthMapResolution) );	
	
	CHECK_MSTATUS(attributeAffects(aUseDepthMapShadows, apiLightShape::aOutColor));
	CHECK_MSTATUS(attributeAffects(aUseRayTraceShadows, apiLightShape::aOutColor));
	CHECK_MSTATUS(attributeAffects(aShadowColor, apiLightShape::aOutColor));
	CHECK_MSTATUS(attributeAffects(aDepthMapResolution, apiLightShape::aOutColor));

    return MS::kSuccess;
}

// 
// Description:
//
MSelectionMask apiLightShape::getShapeSelectionMask() const 
{
	return MSelectionMask::kSelectLights;
}

//////////////////////////////////////////////////////////////////
// Point light shape
//////////////////////////////////////////////////////////////////
void* apiPointLightShape::creator()
//
// Description
//    Called to create new instance of this node type.
//
{
	return new apiPointLightShape();
}

MStatus apiPointLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya point light.
//
{ 
	apiShadowingLightShape::initialize();
    return MS::kSuccess;
}


MString & apiPointLightShape::drawdbIdentfier() 
{
	//
	// - We use "light" to give a general classification as a light
	// - We use "drawdb/light/<light type>" to indicate reuse
	// of the internal support for a given light in VP2.
	//
	static MString drawdb("light:drawdb/light/pointLight");
	return drawdb;
}

//////////////////////////////////////////////////////////////////
// Directional light shape
//////////////////////////////////////////////////////////////////
void* apiDirectionalLightShape::creator()
//
// Description
//    Called to create new instance of this node type.
//
{
	return new apiDirectionalLightShape();
}

MStatus apiDirectionalLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya directional light.
//
{ 
	apiShadowingLightShape::initialize();
    return MS::kSuccess;
}

MString & apiDirectionalLightShape::drawdbIdentfier() 
{
	//
	// - We use "light" to give a general classification as a light
	// - We use "drawdb/light/<light type>" to indicate reuse
	// of the internal support for a given light in VP2.
	//
	static MString drawdb("light:drawdb/light/directionalLight");
	return drawdb;
}

////////////////////////////////////////////////////////////////////////////////
// Spotlight shape
////////////////////////////////////////////////////////////////////////////////
void* apiSpotLightShape::creator()
//
// Description
//    Called to create new instance of this node type.
//
{
	return new apiSpotLightShape();
}

MStatus apiSpotLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya spot light.
//
{ 
	apiShadowingLightShape::initialize();

	MFnNumericAttribute nAttr; 
	
	aConeAngle = nAttr.create( "coneAngle", "ca", MFnNumericData::kDouble);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(40.0) );
    aPenumbraAngle = nAttr.create( "penumbraAngle", "pa", MFnNumericData::kDouble);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.0) );
    aDropOff = nAttr.create( "dropoff", "dro", MFnNumericData::kDouble);
	MAKE_INPUT(nAttr);
    CHECK_MSTATUS ( nAttr.setDefault(0.0) );

    CHECK_MSTATUS ( addAttribute(aConeAngle) );
    CHECK_MSTATUS ( addAttribute(aPenumbraAngle) );
    CHECK_MSTATUS ( addAttribute(aDropOff) );
  
	CHECK_MSTATUS(attributeAffects(aConeAngle, apiLightShape::aOutColor));
	CHECK_MSTATUS(attributeAffects(aPenumbraAngle, apiLightShape::aOutColor));
	CHECK_MSTATUS(attributeAffects(aDropOff, apiLightShape::aOutColor));

    return MS::kSuccess;
}

MString & apiSpotLightShape::drawdbIdentfier() 
{
	//
	// - We use "light" to give a general classification as a light
	// - We use "drawdb/light/<light type>" to indicate reuse
	// of the internal support for a given light in VP2.
	//
	static MString drawdb("light:drawdb/light/spotLight");
	return drawdb;
}

////////////////////////////////////////////////////////////////////////////////
// Area shape
////////////////////////////////////////////////////////////////////////////////
void* apiAreaLightShape::creator()
//
// Description
//    Called to create new instance of this node type.
//
{
	return new apiAreaLightShape();
}

MStatus apiAreaLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya area light.
//
{ 
	apiShadowingLightShape::initialize();
    return MS::kSuccess;
}

MString & apiAreaLightShape::drawdbIdentfier() 
{
	//
	// - We use "light" to give a general classification as a light
	// - We use "drawdb/light/<light type>" to indicate reuse
	// of the internal support for a given light in VP2.
	//
	static MString drawdb("light:drawdb/light/areaLight");
	return drawdb;
}


////////////////////////////////////////////////////////////////////////////////
// Directional light shape with custom drawing + associated geometry override
// See the drawdbIdentifier() method for more information.
////////////////////////////////////////////////////////////////////////////////
apiCustomDirectionalLightShape::apiCustomDirectionalLightShape()
{
	// Add callback so we can update drawing style on viewport changes
	mModelEditorChangedCbId = MEventMessage::addEventCallback(
				"modelEditorChanged", OnModelEditorChanged, this);
}

apiCustomDirectionalLightShape::~apiCustomDirectionalLightShape()
{
    if (mModelEditorChangedCbId != 0)
    {
		MMessage::removeCallback(mModelEditorChangedCbId);
		mModelEditorChangedCbId = 0;
    }
}

//
// Description
//    Called to create new instance of this node type.
//
void* apiCustomDirectionalLightShape::creator()
{
	return new apiCustomDirectionalLightShape();
}

//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya directional light.
//
MStatus apiCustomDirectionalLightShape::initialize()
{ 
	apiDirectionalLightShape::initialize();
	return MS::kSuccess;
}

void apiCustomDirectionalLightShape::OnModelEditorChanged(void *clientData)
{
	apiCustomDirectionalLightShape* shape = static_cast<apiCustomDirectionalLightShape*>(clientData);
	if (shape)
	{
		MHWRender::MRenderer::setGeometryDrawDirty(shape->thisMObject());
	}
}

// To allow for custom geometry drawing while still using an internal
// VP2 lighting computation:
//
// - We use "light" to give a general classification as a light
// - We use "drawdb/light/<light type>" to indicate reuse
//   of the internal support for a given light in VP2. In this example we choose <light type> tb be directionalLight.
// - We use "drawdb/geometry/light/<custom type> to indicate usage of a plug-in override. In this example
//   we use <custom type> to be "directionalLightCustom", which matches the classification of 
//   the geometry override class apiDirectionalLightShape_GeometryOverride.
//
MString & apiCustomDirectionalLightShape::drawdbIdentfier() 
{
	static MString drawdb("light:drawdb/light/directionalLight:drawdb/geometry/light/directionalLightCustom");
	return drawdb;
}

bool apiCustomDirectionalLightShape::isBounded() const
{
	return true;
}

MBoundingBox apiCustomDirectionalLightShape::boundingBox() const
{
	MPoint corner1( -2.0, -2.0, -2.0);
	MPoint corner2( 2.0, 2.0, 2.0 );
	return MBoundingBox( corner1, corner2 );
}

////////////////////////////////////////////////////////////////////////////////
// Geometry override for custom drawing of apiCustomDirectionalLightShape
////////////////////////////////////////////////////////////////////////////////
class apiDirectionalLightShape_GeometryOverride : public MHWRender::MPxGeometryOverride
{
public:
	static MHWRender::MPxGeometryOverride* Creator(const MObject& obj)
	{
		return new apiDirectionalLightShape_GeometryOverride(obj);
	}

	~apiDirectionalLightShape_GeometryOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	bool isIndexingDirty(
		const MHWRender::MRenderItem& item) override;
	bool isStreamDirty(
		const MHWRender::MVertexBufferDescriptor& desc) override;
	void updateDG() override;
	void updateRenderItems(const MDagPath& path, MHWRender::MRenderItemList& list) override;
	void populateGeometry(const MHWRender::MGeometryRequirements& requirements,
					const MHWRender::MRenderItemList& renderItems,
					MHWRender::MGeometry& data) override;

	bool hasUIDrawables() const override;
	void addUIDrawables(
		const MDagPath& path,
		MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& frameContext) override;

	void cleanUp() override;

protected:
	apiDirectionalLightShape_GeometryOverride(const MObject& obj);

	//apiCustomDirectionalLightShape* mNode;
};

apiDirectionalLightShape_GeometryOverride::apiDirectionalLightShape_GeometryOverride(const MObject& obj)
	: MHWRender::MPxGeometryOverride(obj)
{
	//MStatus status;
    //MFnDependencyNode node(obj, &status);
	//mNode = status ? dynamic_cast<apiCustomDirectionalLightShape*>(node.userNode()) : NULL;
}

apiDirectionalLightShape_GeometryOverride::~apiDirectionalLightShape_GeometryOverride()
{
	//mNode = NULL;
} 

MHWRender::DrawAPI apiDirectionalLightShape_GeometryOverride::supportedDrawAPIs() const
{
	return (MHWRender::kAllDevices);
}

bool apiDirectionalLightShape_GeometryOverride::isIndexingDirty(
		const MHWRender::MRenderItem& item)
{
	return false;
}
bool apiDirectionalLightShape_GeometryOverride::isStreamDirty(const MHWRender::MVertexBufferDescriptor& desc)
{
	return false;
}

void apiDirectionalLightShape_GeometryOverride::updateDG()
{
}

void apiDirectionalLightShape_GeometryOverride::updateRenderItems(
	const MDagPath& path,
	MHWRender::MRenderItemList& list)
{
}

void apiDirectionalLightShape_GeometryOverride::populateGeometry(
	const MHWRender::MGeometryRequirements& requirements,
	const MHWRender::MRenderItemList& renderItems,
	MHWRender::MGeometry& data)
{
}

void apiDirectionalLightShape_GeometryOverride::cleanUp()
{
}

bool apiDirectionalLightShape_GeometryOverride::hasUIDrawables() const
{
	return true;
}

//
// Some simple drawing to be used to display the UI for apiCustomDirectionalLightShape
// instances.
//
void apiDirectionalLightShape_GeometryOverride::addUIDrawables(
	const MDagPath& path,
	MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& frameContext)
{
	// See if the camera path transform is the same object's. If so we are
	// looking through the child camera.
	MDagPath objectTransformPath(path);
	objectTransformPath.pop();

	MDagPath cameraPath = frameContext.getCurrentCameraPath();
	cameraPath.pop();
	if (objectTransformPath == cameraPath)
	{
		return;
	}


	MVector coneDirection(0, 0, -1);
	double	coneHeight = 1;
	double	coneRadius = 0.5;

	// Draw a basic cone and line to show direction of the light
	drawManager.beginDrawable(MHWRender::MUIDrawManager::kSelectable, 20);

	MHWRender::DisplayStatus displayStatus = MHWRender::MGeometryUtilities::displayStatus(path);
	if (displayStatus == MHWRender::kDormant)
	{
		drawManager.setColor( MColor(0.375f, 0.525f, 0.7f) );
	}
	else
	{
		MColor color = MHWRender::MGeometryUtilities::wireframeColor(path);
		drawManager.setColor( color );
	}

	unsigned int displayStyle = frameContext.getDisplayStyle();
	bool filled = !(displayStyle & MHWRender::MFrameContext::kWireFrame);
	if (filled)
	{
		drawManager.setPaintStyle(MHWRender::MUIDrawManager::kShaded);
	}
	else
	{
		drawManager.setPaintStyle(MHWRender::MUIDrawManager::kFlat);		
	}
	drawManager.cone( MPoint(0,0,0),
					coneDirection,
					coneRadius,
					coneHeight,
					filled);
	drawManager.setLineWidth(2);
	drawManager.line( MPoint(0,0,2), MPoint(0,0,0) );

	drawManager.endDrawable();
}


//////////////////////////////////////////////////////////////////
// Image light shape
//////////////////////////////////////////////////////////////////
void* apiImageLightShape::creator()
//
// Description
//    Called to create new instance of this node type.
//
{
	return new apiImageLightShape();
}

MStatus apiImageLightShape::initialize()
//
// Description:
//		Attribute initialization.
//		Note that the long and short names are made to match a
//		Maya image light.
//
{ 
	apiLightShape::initialize();
    return MS::kSuccess;
}


MString & apiImageLightShape::drawdbIdentfier() 
{
	//
	// - We use "light" to give a general classification as a light
	// - We use "drawdb/light/<light type>" to indicate reuse
	// of the internal support for a given light in VP2.
	//
	static MString drawdb("light:drawdb/light/image");
	return drawdb;
}


////////////////////////////////////////////////////////////////////////////////
//
// Node registristration
// - apiPointLightShape, apiDirectionalLightShape, apiSpotLightShape, 
// apiAreaLightShape, and apiImageLightShape all just use a "drawdb/light/<light type>" 
// classification to allow for internally specified UI drawing and VP2 lighting support.
// - apiCustomDirectionalLightShape additionally specifies a geometry override
// classification to allow apiDirectionalLightShape_GeometryOverride to perform
// the UI drawing (instead of the internally specified implementation)
// 
////////////////////////////////////////////////////////////////////////////////

MStatus initializePlugin( MObject obj )
{ 
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "1.0", "Any");

	// - Note that we do not specify a UI shape creator (for VP1 drawing)
	MStatus stat = plugin.registerShape( "apiPointLightShape", apiPointLightShape::id,
		&apiPointLightShape::creator,
		&apiPointLightShape::initialize,
		NULL,
		&apiPointLightShape::drawdbIdentfier());
	if ( ! stat ) {
		cerr << "Failed to register apiPointLightShape\n";
	}

	stat = plugin.registerShape( "apiDirectionalLightShape", apiDirectionalLightShape::id,
		&apiDirectionalLightShape::creator,
		&apiDirectionalLightShape::initialize,
		NULL,
		&apiDirectionalLightShape::drawdbIdentfier());
	if ( ! stat ) {
		cerr << "Failed to register apiDirectionalLightShape\n";
	}

	stat = plugin.registerShape( "apiSpotLightShape", apiSpotLightShape::id,
		&apiSpotLightShape::creator,
		&apiSpotLightShape::initialize,
		NULL,
		&apiSpotLightShape::drawdbIdentfier());
	if ( ! stat ) {
		cerr << "Failed to register apiSpotLightShape\n";
	}

	stat = plugin.registerShape( "apiAreaLightShape", apiAreaLightShape::id,
		&apiAreaLightShape::creator,
		&apiAreaLightShape::initialize,
		NULL,
		&apiAreaLightShape::drawdbIdentfier());
	if ( ! stat ) {
		cerr << "Failed to register apiAreaLightShape\n";
	}

	stat = plugin.registerShape( "apiImageLightShape", apiImageLightShape::id,
		&apiImageLightShape::creator,
		&apiImageLightShape::initialize,
		NULL,
		&apiImageLightShape::drawdbIdentfier());
	if ( ! stat ) {
		cerr << "Failed to register apiImageLightShape\n";
	}

	//
	// Note: We add use "drawdb/geometry/light/directionalLightCustom" to match
	// one of the classifications used for apiCustomDirectionalLightShape.
	// 
	stat = MHWRender::MDrawRegistry::registerGeometryOverrideCreator("drawdb/geometry/light/directionalLightCustom",
														  "directionalLightRegistrantId",
															apiDirectionalLightShape_GeometryOverride::Creator);
	if (!stat) {
		cerr << "Failed to register apiDirectionalLightShape_GeometryOverride\n";
		return stat;
	}

	//
	// Note: apiCustomDirectionalLightShape drawdbIdentifier() returns both the lighting and
	// the geometry override classification: "drawdb/geometry/light/directionalLightCustom"
	// allowing for the custom geometry override to replace the default light drawing.
	//
	stat = plugin.registerShape( "apiCustomDirectionalLightShape", apiCustomDirectionalLightShape::id,
		&apiCustomDirectionalLightShape::creator,
		&apiCustomDirectionalLightShape::initialize,
		NULL,
		&apiCustomDirectionalLightShape::drawdbIdentfier());
	if ( ! stat ) {
		cerr << "Failed to register apiCustomDirectionalLightShape\n";
	}

	return stat;
}

MStatus uninitializePlugin( MObject obj)
{
	MFnPlugin plugin( obj );
	MStatus stat;

	stat = plugin.deregisterNode( apiPointLightShape::id );
	if ( ! stat ) {
		cerr << "Failed to deregister apiPointLightShape\n";
	}
	stat = plugin.deregisterNode( apiDirectionalLightShape::id );
	if ( ! stat ) {
		cerr << "Failed to deregister apiDirectionalLightShape\n";
	}
	stat = plugin.deregisterNode( apiSpotLightShape::id );
	if ( ! stat ) {
		cerr << "Failed to deregister apiSpotLightShape\n";
	}
	stat = plugin.deregisterNode( apiAreaLightShape::id );
	if ( ! stat ) {
		cerr << "Failed to deregister apiAreaLightShape\n";
	}
	stat = plugin.deregisterNode( apiImageLightShape::id );
	if ( ! stat ) {
		cerr << "Failed to deregister apiImageLightShape\n";
	}
	stat = MHWRender::MDrawRegistry::deregisterGeometryOverrideCreator("drawdb/geometry/light/directionalLightCustom",
														  "directionalLightRegistrantId");
	if (! stat ) {
		cerr << "Failed to deregister apiDirectionalLightShape_GeometryOverride\n";
	}
	stat = plugin.deregisterNode( apiCustomDirectionalLightShape::id );
	if ( ! stat ) {
		cerr << "Failed to deregister apiCustomDirectionalLightShape\n";
	}
	return stat;
}
