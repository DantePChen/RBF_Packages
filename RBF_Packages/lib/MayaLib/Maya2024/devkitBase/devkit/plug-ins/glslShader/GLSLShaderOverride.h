//-
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+

#ifndef GLSLSHADER_GLSLSHADEROVERRIDE_H
#define GLSLSHADER_GLSLSHADEROVERRIDE_H

#include <maya/MPxShaderOverride.h>
#include <maya/MShaderManager.h>
#include <maya/MUniformParameter.h>
#include <maya/MUniformParameterList.h>
#include <maya/MVaryingParameterList.h>

#include "GLSLShader.h"


class GLSLShaderOverride : public MHWRender::MPxShaderOverride
{
public:
	GLSLShaderOverride(const MObject& obj);
	static MHWRender::MPxShaderOverride* Creator(const MObject& obj);

	~GLSLShaderOverride() override;

	MString initialize( const MInitContext& initContext, MSharedPtr<MUserData>& userData) override;
	
	void updateDG(MObject object) override;
	void updateDevice() override;
	void endUpdate() override;

	void activateKey(MHWRender::MDrawContext& context, const MString& key) override;
	bool handlesDraw(MHWRender::MDrawContext& context) override;
	bool draw(MHWRender::MDrawContext& context,const MHWRender::MRenderItemList& renderItemList) const override;
	void terminateKey(MHWRender::MDrawContext& context, const MString& key) override;

	MHWRender::DrawAPI supportedDrawAPIs() const override;
	bool isTransparent() override;
	bool supportsAdvancedTransparency() const override;
	bool overridesDrawState() override;
	double boundingBoxExtraScale() const override;
	bool overridesNonMaterialItems() const override;
	bool handlesConsolidatedGeometry() const override;

	MHWRender::MShaderInstance* shaderInstance(MDrawContext&) const override;

	bool rebuildAlways() override;
	
private:
	double fBBoxExtraScale;
	bool fShaderBound;

	//GLSLShader associated with the shader override
	GLSLShaderNode* fShaderNode;
	
};

#endif
