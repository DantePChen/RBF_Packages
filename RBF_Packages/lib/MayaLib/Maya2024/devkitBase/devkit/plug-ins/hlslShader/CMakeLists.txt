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
set(PROJECT_NAME hlslShader)

option(VALIDATE_TECHNIQUES     "On hlslShader.cpp, turn on to enable validation." OFF)
option(VALIDATE_TECHNIQUE_LIST "On hlslShader.cpp, turn on to enable validation." OFF)

# set SOURCE_FILES
set(SOURCE_FILES
	hlslPluginMain.cpp hlslShader.cpp hlslShader.h
)

# set linking libraries
set(LIBRARIES OpenMaya Image OpenMayaUI OpenMayaRender Foundation)

# set package libraries in MSVC
set(PACKAGE_LIBS winmm.lib glu32.lib OpenGL32.lib)

# Find d3d9 d3dx9 dxguid libraries in DirectXSDK 
set(libs d3d9 d3dx9 dxguid)
find_directX("${libs}")

build_plugin()


