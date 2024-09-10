==========================================================================
// Copyright 2022 Autodesk, Inc. All rights reserved. 
//
// Use of this software is subject to the terms of the Autodesk 
// license agreement provided at the time of installation or download, 
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
==========================================================================


apiMeshShape
============

Introduction
------------

This plug-in produces the shape node "apiMesh" and "apiMeshSubscene"
dependency graph node "apiMeshCreator", and data type "apiMeshData". The
shape node "apiMesh" performs the draw with a MPxGeometryOverride and the
apiMeshSubscene with a MPxSubsceneOverride 

It demonstrates how to create a polygonal mesh shape with vertices that can
be selected, moved, animated, and deformed.  This shape also supports OpenGL
display of materials.

This plug-in also registers a new kind of geometry data called "apiMeshData",
and demonstrates how to pass this data between nodes.

This plug-in shows how non-editable geometry can be integrated into Maya. The
gpuCache node is a Shape node that holds baked animated geometry in memory. The
node can stream the animated geometry to the viewport without triggering any
DG evaluation of geometry attributes (e.g., mesh, nurbs or subd surface data).


Implementation notes
--------------------

This plugin also illustrates how to migrate from using the deprecated
glFunctionTable to using glew instead.

In Maya 2020, the function apiMeshSubSceneOverride::rebuildGeometryBuffers has
this section of code:
 

static MGLFunctionTable* gGLFT = NULL;
_OPENMAYA_DEPRECATION_PUSH_AND_DISABLE_WARNING
MHardwareRenderer* pRenderer = MHardwareRenderer::theRenderer();
if (!gGLFT && pRenderer)
{
    gGLFT = pRenderer->glFunctionTable();
}
_OPENMAYA_POP_WARNING
if (gGLFT)
{
    gGLFT->glGenBuffersARB(1, &fBoxPositionBufferId);
    if (fBoxPositionBufferId != 0)
    {
        gGLFT->glBindBufferARB(MGL_ARRAY_BUFFER_ARB, fBoxPositionBufferId);
        gGLFT->glBufferDataARB(MGL_ARRAY_BUFFER_ARB, 8*3*sizeof(float), boxPositions, MGL_STATIC_DRAW_ARB);
        gGLFT->glBindBufferARB(MGL_ARRAY_BUFFER_ARB, 0);
        fBoxPositionBuffer->resourceHandle(&fBoxPositionBufferId, 8*3);
    }

Staring with Maya 2023, the same function uses this section of code instead:

glGenBuffers(1, &fBoxPositionBufferId);
if (fBoxPositionBufferId != 0)
{
    glBindBuffer(GL_ARRAY_BUFFER, fBoxPositionBufferId);
    glBufferData(GL_ARRAY_BUFFER, 8*3*sizeof(float), boxPositions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    fBoxPositionBuffer->resourceHandle(&fBoxPositionBufferId, 8*3);
}

