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

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

set(MAYA_L10N_WARNLEVEL 1) # Only L10N warnings for this library

# specify project name
set(PROJECT_NAME dx11Shader)

# set SOURCE_FILES
set(SOURCE_FILES
    dx11Shader.h
    dx11ShaderCmd.h
    dx11ShaderCompileHelper.h
    dx11ShaderOverride.h
    dx11ShaderSemantics.h
    dx11ShaderStrings.h
    dx11ShaderUniformParamBuilder.h
    crackFreePrimitiveGenerator.h
    dx11ConeAngleToHotspotConverter.h
    crackFreePrimitiveGenerator.cpp
    dx11ConeAngleToHotspotConverter.cpp
    dx11Shader.cpp
    dx11ShaderCmd.cpp
    dx11ShaderCompileHelper.cpp
    dx11ShaderOverride.cpp
    dx11ShaderPluginMain.cpp
    dx11ShaderSemantics.cpp
    dx11ShaderStrings.cpp
    dx11ShaderUniformParamBuilder.cpp
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     OpenMayaAnim
     Image
     OpenMayaUI
     OpenMayaRender
     Foundation
)

if (WIN32)
    find_D3DX11Effects()
    set(PACKAGE_LIBS ${PACKAGE_LIBS} d3d11.lib dxguid.lib D3DCompiler.lib )
endif()

find_opengl()

add_definitions(-DJAMBUILD)

set(PLUGIN_DESTINATION binPlugin)

# Build plugin
build_plugin()
