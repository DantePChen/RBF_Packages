//-
// ==========================================================================
// Copyright 2018 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+
#ifndef blindDataShaderOverride_h
#define blindDataShaderOverride_h

#include <maya/MPxShaderOverride.h>


//////////////////////////////////////////////////////////////////////////////////////////////
// Viewport 2.0 implementation for the shader
//
// Use to display meshs in the viewport 2.0 with a basic color per vertex shader.
//
//////////////////////////////////////////////////////////////////////////////////////////////
class blindDataShaderOverride : public MHWRender::MPxShaderOverride
{
public:
    
    static MHWRender::MPxShaderOverride* Creator(const MObject& obj)
    {
        return new blindDataShaderOverride(obj);
    }

    ~blindDataShaderOverride();

	MString initialize(const MInitContext& initContext, MInitFeedback& initFeedback) override;
	MHWRender::DrawAPI supportedDrawAPIs() const override;
	MHWRender::MShaderInstance* shaderInstance(MDrawContext& context) const override;

	void activateKey(MDrawContext& context, const MString& key)override;
	void terminateKey(MDrawContext& context, const MString& key)override;

	bool handlesDraw(MHWRender::MDrawContext& context) override;
	bool draw(MDrawContext& context, const MRenderItemList& renderItemList)const override;

private:
    blindDataShaderOverride(const MObject& obj);

	MHWRender::MShaderInstance *fCPVShaderInstance = nullptr;
};

#endif // blindDataShaderOverride_h
