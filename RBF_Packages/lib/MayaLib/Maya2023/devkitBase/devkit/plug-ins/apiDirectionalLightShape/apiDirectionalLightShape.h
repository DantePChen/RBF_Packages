#ifndef _apiDirectionalLightShape
#define _apiDirectionalLightShape

//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
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
//	Creates a plug-in shape which is classified generally as a light
//	and specifically as a VP2 directional light.
//
//  The second classification allows for the usage of the internal
//	light and UI (geometry drawing) VP2 evluators to be used.
//	Note that there is no explicit VP2 override class used as
//	the internal VP2 geometry evaluator will handle drawing.
//
//	Attributes which control the light are created to match names on
//	a Maya directional light so that they will be picked up when DG
//	evaluation occurs.
//
// Usage:	Create node and set it's object color to a one of the user defined 
//			preference colors:
//
//	string $node = `shadingNode -asLight apiDirectionalLightShape`;
//	color -ud 7 $node;
//
// to create a node which is categorized as a light.
//
// Caveats:
//	Not all attributes are currently supported fully.
//
////////////////////////////////////////////////////////////////////////////////

#include <maya/MTypeId.h> 
#include <maya/MPxComponentShape.h> 


class apiLightShape : public MPxSurfaceShape
{
public:
	apiLightShape() {};
	~apiLightShape() override {}; 
    MStatus compute( const MPlug&, MDataBlock& ) override;

	static  MStatus initialize();

	// Required to categorized the shape for selection
	MSelectionMask	getShapeSelectionMask() const override;

	// Input attributes to mimic an internal light
	static MObject  aColor;
    static MObject  aIntensity;
    static MObject  aExposure;
    static MObject  aEmitDiffuse;
    static MObject  aEmitSpecular;
	static MObject	aLocatorScale;
	static MObject	aDecayRate;

	// General output color attribute
	static MObject  aOutColor;
};

class apiShadowingLightShape : public apiLightShape
{
public:
	apiShadowingLightShape() {};
	~apiShadowingLightShape() override {}; 

	static  MStatus initialize();

	// Input attributes to mimic an internal light which supports shadow maps
	static MObject  aUseDepthMapShadows;
	static MObject  aUseRayTraceShadows;
	static MObject  aShadowColor;
	static MObject  aDepthMapResolution;
};

class apiPointLightShape : public apiShadowingLightShape
{
public:
	apiPointLightShape() {};
	~apiPointLightShape() override {}; 

	static MString & drawdbIdentfier();

	static  void*	creator();
	static  MStatus initialize();

	static	MTypeId		id;
};


class apiDirectionalLightShape : public apiShadowingLightShape
{
public:
	apiDirectionalLightShape() {};
	~apiDirectionalLightShape() override {}; 

	static MString & drawdbIdentfier();

	static  void*	creator();
	static  MStatus initialize();

	static	MTypeId		id;
};

class apiSpotLightShape : public apiShadowingLightShape
{
public:
	apiSpotLightShape() {};
	~apiSpotLightShape() override {}; 

	static MString & drawdbIdentfier();

	static  void*	creator();
	static  MStatus initialize();

	static	MTypeId		id;

	// Spotlight specific attributes
	static MObject	aConeAngle;
	static MObject	aPenumbraAngle;
	static MObject	aDropOff;
};

class apiAreaLightShape : public apiShadowingLightShape
{
public:
	apiAreaLightShape () {};
	~apiAreaLightShape () override {}; 

	static MString & drawdbIdentfier();

	static  void*	creator();
	static  MStatus initialize();

	static	MTypeId		id;
};

class apiCustomDirectionalLightShape : public apiDirectionalLightShape
{
public:
   apiCustomDirectionalLightShape();
   ~apiCustomDirectionalLightShape() override;

   	bool		    isBounded() const override;
	MBoundingBox    boundingBox() const override;

	static MString & drawdbIdentfier();

	static  void*	creator();
	static  MStatus initialize();

	static	MTypeId		id;

	static void OnModelEditorChanged(void *clientData);
	MCallbackId mModelEditorChangedCbId;
};

class apiImageLightShape : public apiLightShape
{
public:
	apiImageLightShape() {};
	~apiImageLightShape() override {}; 

	static MString & drawdbIdentfier();

	static  void*	creator();
	static  MStatus initialize();

	static	MTypeId		id;
};

#endif /* _apiDirectionalLightShape */
