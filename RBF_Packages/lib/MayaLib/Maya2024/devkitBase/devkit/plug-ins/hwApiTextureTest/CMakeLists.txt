#-
# ==========================================================================
# Copyright (c) 2018 Autodesk, Inc.
# All rights reserved.
# 
# These coded instructions, statements, and computer programs contain
# unpublished proprietary information written by Autodesk, Inc., and are
# protected by Federal copyright law. They may not be disclosed to third
# parties or copied or duplicated in any form, in whole or in part, without
# the prior written consent of Autodesk, Inc.
# ==========================================================================
#+


cmake_minimum_required(VERSION 3.13)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME hwApiTextureTest)

# set SOURCE_FILES
set(SOURCE_FILES
	hwApiTextureTest.cpp hwApiTextureTestStrings.cpp hwRendererHelper.cpp hwRendererHelperDX.cpp hwRendererHelperGL.cpp
	hwApiTextureTestStrings.h hwRendererHelper.h hwRendererHelperDX.h hwRendererHelperGL.h
)

# set linking libraries
set(LIBRARIES OpenMaya OpenMayaUI OpenMayaRender Foundation)

if(WIN32)
    find_D3DX11Effects()
    # set package libraries in MSVC
    set(PACKAGE_LIBS ${PACKAGE_LIBS} d3d11.lib dxguid.lib D3DCompiler.lib)
endif()

find_opengl()
find_glew()

if(GLEW_FOUND)
    build_plugin()
else()
	message(STATUS "Glew library is not found. hwApiTextureTest plugin will be skipped.")
endif()


