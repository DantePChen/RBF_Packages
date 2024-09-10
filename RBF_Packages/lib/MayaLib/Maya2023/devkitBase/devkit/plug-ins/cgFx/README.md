==========================================================================
// Copyright 2020 Autodesk, Inc. All rights reserved. 
//
// Use of this software is subject to the terms of the Autodesk 
// license agreement provided at the time of installation or download, 
// or which otherwise accompanies this software in either electronic 
// or hard copy form.
==========================================================================


CgFxShader plugin
===================================================

Introduction
------------

The plug-in defines one node named "cgfxShader" and
one command, which is also named "cgfxShader". The command is used to manipulate the node.
This is very similar to the expression command and node.

The "cgfxShader" node is a hardware shader (derived from MPxHwShaderNode). More information can be found with the code.

Building the plug-in
-----------------------------

Before building the cgFxShader plug-in, you will need to install glew.
* See "Building a plug-in with glew" section of the main devkit README.

The OpenGL requirements are the same as other maya plugins.

Afterwards, you can use either the Makefile, the MSVC project or the
Xcode project to build this plug-in using the procedure normally used
for building any other devkit plug-in.
