#ifndef _lambertShaderOverride
#define _lambertShaderOverride

//-
// ===========================================================================
// Copyright 2012 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//+

// Example Plugin: lambertShader.cpp
//
// This is the MPxSurfaceShadingNodeOverride implementation to go along with
// the node defined in lambertShader.cpp. This provides draw support in
// Viewport 2.0.
//

#include <maya/MPxSurfaceShadingNodeOverride.h>

class lambertShaderOverride : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
	static MHWRender::MPxSurfaceShadingNodeOverride* creator(const MObject& obj);

	~lambertShaderOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	MString fragmentName() const override;
	void getCustomMappings(
		MHWRender::MAttributeParameterMappingList& mappings) override;

	MString primaryColorParameter() const override;
	MString transparencyParameter() const override;
	MString bumpAttribute() const override;

private:
	lambertShaderOverride(const MObject& obj);
};

#endif // _lambertShaderOverride
