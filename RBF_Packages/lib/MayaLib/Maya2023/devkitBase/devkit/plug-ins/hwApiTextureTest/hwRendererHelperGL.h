#ifndef __hwRendererHelperGL_h__
#define __hwRendererHelperGL_h__

#include "hwRendererHelper.h"

class hwRendererHelperGL : public hwRendererHelper
{
public:
	hwRendererHelperGL(MHWRender::MRenderer* renderer);
	~hwRendererHelperGL() override;

protected:
	bool renderTextureToTarget(MHWRender::MTexture* texture, MHWRender::MRenderTarget *target) override;
};

#endif

//-
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
