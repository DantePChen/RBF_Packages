//-
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#ifndef CUSTOMTEXTURESHADEROVERRIDE_H
#define CUSTOMTEXTURESHADEROVERRIDE_H

#include <maya/MPxShaderOverride.h>
#include <maya/MShaderManager.h>


class customTextureShaderOverride : public MHWRender::MPxShaderOverride
{
public:
	customTextureShaderOverride(const MObject& obj);
	static MHWRender::MPxShaderOverride* Creator(const MObject& obj);

	~customTextureShaderOverride() override;

	MString initialize( const MInitContext& initContext, MSharedPtr<MUserData>& userData) override;

	MHWRender::MShaderInstance* shaderInstance(MHWRender::MDrawContext& context) const override;

	void updateDG(MObject object) override;
	void updateDevice() override;
	void endUpdate() override;

	bool handlesDraw(MHWRender::MDrawContext& context) override;
	void activateKey(MHWRender::MDrawContext& context, const MString& key) override;
	bool draw(MHWRender::MDrawContext& context,const MHWRender::MRenderItemList& renderItemList) const override;
	void terminateKey(MHWRender::MDrawContext& context, const MString& key) override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;
	bool isTransparent() override;
	bool overridesDrawState() override;
    bool supportsMultiDraw() const override;

private:
	void updateShaderInstance();
	void generateCustomTexture();
	void buildAndUpdateCustomDataTextureViaMaya();

	// beauty shader
	MHWRender::MShaderInstance *m_beautyShaderInstance;
	MHWRender::MShaderInstance *m_multiDrawBeautyShaderInstance;

	bool m_shaderBound;
	bool m_inBeautyPass;
    bool m_inMultiDrawMode;

	// custom texture data:
	unsigned char* m_customTextureData;
	MHWRender::MTextureDescription m_desc;
};

#endif