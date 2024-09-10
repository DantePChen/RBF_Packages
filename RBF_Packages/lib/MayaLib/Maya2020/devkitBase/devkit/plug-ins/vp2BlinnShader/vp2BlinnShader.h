#ifndef _vp2BlinnShader
#define _vp2BlinnShader
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
#include <maya/MPxHardwareShader.h>
#include <maya/MRenderProfile.h>

class vp2BlinnShader : public MPxHardwareShader 
{
public:
                    vp2BlinnShader();
            ~vp2BlinnShader() override;

    MStatus compute( const MPlug&, MDataBlock& ) override;

	// VP1 profile. Just leave as unsupported hardware profile
	// as sample code.
	const MRenderProfile & profile() override
	{
		static MRenderProfile sProfile;
		if(sProfile.numberOfRenderers() == 0)
			sProfile.addRenderer(MRenderProfile::kMayaSoftware);
		return sProfile;	
	}

	// Swatch rendering. Called irregardless of VP1 or VP2.
	//
	MStatus renderSwatchImage( MImage & image ) override;

    static  void *  creator();
    static  MStatus initialize();

    static  MTypeId id;

protected:

private:
	// Attributes
	static MObject  aColor;
	static MObject	aTransparency;
	static MObject  aDiffuse;
	static MObject  aDiffuseColor;
	static MObject  aSpecularColor;
	static MObject  aSpecularRolloff;
	static MObject  aEccentricity;
	static MObject  aNonTexturedColor;
	static MObject  aNonTexturedTransparency;
};

#endif /* _vp2BlinnShader */
