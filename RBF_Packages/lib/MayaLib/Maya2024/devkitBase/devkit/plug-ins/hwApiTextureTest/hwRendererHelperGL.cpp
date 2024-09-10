#include "hwRendererHelperGL.h"
#include "hwApiTextureTestStrings.h"

#include <maya/MTextureManager.h>
#include <maya/MRenderTargetManager.h>
#include <maya/MHardwareRenderer.h>

#ifdef MAYA_PLUGIN_USES_GLEW

#if defined(__APPLE__)
#include <GL/glew.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#elif defined(__linux__) || defined(__linux__)
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#elif defined(_WIN32)
#include <GL/glew.h>
#include <gl/Gl.h>
#include <gl/Glu.h>
#else
#error Unknown OS
#endif


hwRendererHelperGL::hwRendererHelperGL(MHWRender::MRenderer* renderer)
: hwRendererHelper(renderer)
{
}

hwRendererHelperGL::~hwRendererHelperGL()
{
}

bool hwRendererHelperGL::renderTextureToTarget(MHWRender::MTexture* texture, MHWRender::MRenderTarget *target)
{
	if (texture == NULL || target == NULL)
		return false;

	GLuint *pTextureId = (GLuint *)texture->resourceHandle();
	GLuint *pTargetId  = (GLuint *)target->resourceHandle();
	if (pTextureId == NULL || pTargetId == NULL)
		return false;

	MHWRender::MTextureDescription textureDesc;
	texture->textureDescription(textureDesc);

	MHWRender::MRenderTargetDescription targetDesc;
	target->targetDescription(targetDesc);

	if (textureDesc.fFormat != targetDesc.rasterFormat())
		return false;

	GLint srcX0 = 0;
	GLint srcY0 = textureDesc.fHeight;
	GLint srcX1 = textureDesc.fWidth;
	GLint srcY1 = 0;

	GLint dstX0 = 0;
	GLint dstY0 = 0;
	GLint dstX1 = targetDesc.width();
	GLint dstY1 = targetDesc.height();

    // Generates 2 framebuffers, one as READ_FRAMEBUFFER and the other as DRAW_FRAMEBFFER.
    GLuint framebuffers[2] = {0, 0};
	glGenFramebuffersEXT(2, framebuffers);
	if (framebuffers[0] == 0 || framebuffers[1] == 0)
		return false;

    // Bind the first FBO as the read framebuffer, and attach texture
    glBindFramebufferEXT(GL_READ_FRAMEBUFFER, framebuffers[0]);
	glFramebufferTexture2DEXT(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pTextureId, 0);

    // Bind the 2nd FBO as the draw frambuffer, and attach the target
    glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, framebuffers[1]);
	glFramebufferTexture2DEXT(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *pTargetId, 0);

	// Copy data from read buffer (the texture) to draw buffer (the target)
	glBlitFramebufferEXT(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	// Restore the framebuffer binding.
	glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

	// Deletes the 2 framebuffers.
	glDeleteFramebuffersEXT(2, framebuffers);

	return true;
}

#endif


//-
// Copyright 2012 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license agreement
// provided at the time of installation or download, or which otherwise
// accompanies this software in either electronic or hard copy form.
//+
