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


cmake_minimum_required(VERSION 2.8)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME glslShader)

# set SOURCE_FILES
set(SOURCE_FILES
   GLSLShader.cpp 
   GLSLShaderOverride.cpp 
   GLSLShaderMainMaya.cpp 
   GLSLShaderStrings.cpp 
   GLSLShaderSemantics.cpp 
   crackFreePrimitiveGenerator.cpp 
   adjacentTrianglesPrimitiveGenerator.cpp 
   GLSLShaderCmd.cpp
   GLSLShader.h
   GLSLShaderOverride.h
   GLSLShaderStrings.h
   GLSLShaderSemantics.h
   crackFreePrimitiveGenerator.h
   adjacentTrianglesPrimitiveGenerator.h
   GLSLShaderCmd.h
)

# set linking libraries
set(LIBRARIES OpenMaya Image OpenMayaUI OpenMayaRender Foundation)

# set package libraries
if (WIN32)
	set(PACKAGE_LIBS winmm.lib)
elseif(LINUX)
	set(PACKAGE_LIBS "-lX11 -lXext -lGL -lGLU -DGLX_GLXEXT_PROTOTYPES")
endif()

# find OpenGL
find_opengl()

build_plugin()


