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

#
# On MACOS this plugin requires the cg.framework to be installed.
# This is available at https://developer.download.nvidia.com/cg/Cg.html
#

cmake_minimum_required(VERSION 3.13)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME cgfxShader)

# set SOURCE_FILES
set(SOURCE_FILES
	cgfxVector.cpp cgfxShaderCmd.cpp cgfxAttrDef.cpp cgfxEffectDef.cpp cgfxProfile.cpp
	pluginMain.cpp cgfxFindImage.cpp cgfxShaderNode.cpp cgfxPassStateSetter.cpp 
	cgfxTextureCache.cpp nv_dds.cpp
	cgfxRCPtr.h cgfxVector.h cgfxShaderCmd.h cgfxProfile.h
	cgfxEffectDef.h cgfxAttrDef.h cgfxShaderCommon.h cgfxPassStateSetter.h
	cgfxTextureCache.h cgfxFindImage.h cgfxShaderNode.h nv_dds.h
)

# set linking libraries
set(LIBRARIES OpenMaya Image OpenMayaUI OpenMayaRender Foundation)

if (WIN32)
	set(PACKAGE_LIBS winmm.lib)
endif()

# add Cg and CgGL libraries
find_cg()

# find OpenGL
find_opengl()

# find glew
find_glew()
if(GLEW_FOUND)

build_plugin()
else()
	message(STATUS "Glew library is not found. cgFxShader plugin will be skipped.")
endif()


