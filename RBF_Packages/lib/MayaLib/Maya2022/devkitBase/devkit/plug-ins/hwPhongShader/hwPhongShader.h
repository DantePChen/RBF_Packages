#ifndef _hwPhongShader
#define _hwPhongShader

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


///////////////////////////////////////////////////////////////////
//
// NOTE: PLEASE READ THE README.TXT FILE FOR INSTRUCTIONS ON
// COMPILING AND USAGE REQUIREMENTS.
//
// DESCRIPTION: 
//
// Simple node that implements Phong shading for the special
// case when the light is at the eye. We use a simple
// spherical reflection environment map to compute the Phong
// highlight
//
///////////////////////////////////////////////////////////////////

#include <maya/MPxHwShaderNode.h>
#include <maya/MPoint.h>

class hwPhongShader : public MPxHwShaderNode
{
public:
	hwPhongShader();
	~hwPhongShader() override;

    MStatus compute( const MPlug&, MDataBlock& ) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }

	// Internally cached attribute handling routines
	bool getInternalValue( const MPlug&, MDataHandle& )			override;
    bool setInternalValue( const MPlug&, const MDataHandle& )	override;

	// Overridden to draw an image for swatch rendering.
	///
	MStatus renderSwatchImage( MImage & image ) override;

	int		normalsPerVertex() override;
	int		texCoordsPerVertex() override;
	int		getTexCoordSetNames(MStringArray& names) override;

	bool	hasTransparency() override;

	bool	provideVertexIDs() override;
	bool	provideFaceIDs() override;
	bool    provideLocalUVCoord() override;

    static  void *  creator();
    static  MStatus initialize();

	float			Transparency() const { return mTransparency; }
	const float3 *	Ambient() const { return &mAmbientColor; }
	const float3 *	Diffuse() const { return &mDiffuseColor; }
	const float3 *	Specular() const { return &mSpecularColor; }
	const float3 * 	Shininess() const { return &mShininess; }

	void setTransparency(const float fTransparency);
	void setAmbient(const float3 &fAmbient);
	void setDiffuse(const float3 &fDiffuse);
	void setSpecular(const float3 &fSpecular);
	void setShininess(const float3 &fShininess);

	void			printGlError( const char *call );

	bool			attributesChangedVP2() { return mAttributesChangedVP2; }
	void			markAttributesChangedVP2() { mAttributesChangedVP2 = true; }
	void			markAttributesCleanVP2() { mAttributesChangedVP2 = false; }

    static  MTypeId id;

private:
	// Attributes
	static MObject  aColor;
	static MObject  aDiffuseColor;
	static MObject  aSpecularColor;
	static MObject  aShininessX;
	static MObject  aShininessY;
	static MObject  aShininessZ;
	static MObject  aShininess;
	static MObject	aTransparency;
	static MObject	aGeometryShape;
	
	MPoint			cameraPosWS;

	float3			mAmbientColor;
	float			mTransparency;
	float3			mDiffuseColor;
	float3			mSpecularColor;
	float3			mShininess;
	unsigned int	mGeometryShape;
	bool			mAttributesChanged; // Keep track if any attributes changed
	bool			mAttributesChangedVP2; // Keep track if any attributes changed for VP2
};

#endif /* _hwPhongShader */
