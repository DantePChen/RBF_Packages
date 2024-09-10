#ifndef _phongShaderOverride
#define _phongShaderOverride

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

class phongShaderOverride : public MHWRender::MPxSurfaceShadingNodeOverride
{
public:
	static MHWRender::MPxSurfaceShadingNodeOverride* creator(const MObject& obj);

	~phongShaderOverride() override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;

	MString fragmentName() const override;
	void getCustomMappings(
		MHWRender::MAttributeParameterMappingList& mappings) override;

	MString primaryColorParameter() const override;
	MString bumpAttribute() const override;

	void updateDG() override;
	void updateShader(
		MHWRender::MShaderInstance& shader,
		const MHWRender::MAttributeParameterMappingList& mappings) override;

private:
	phongShaderOverride(const MObject& obj);

	MObject fObject;
	float fSpecularColor[3];
	mutable MString fResolvedSpecularColorName;
};

#endif // _phongShaderOverride
