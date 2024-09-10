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
set(PROJECT_NAME viewDX11DeviceAccess)



# set SOURCE_FILES
set(SOURCE_FILES
   viewDX11DeviceAccess.cpp

)

# set linking libraries
set(LIBRARIES
     OpenMaya
     OpenMayaUI
     OpenMayaRender
     Foundation
)

find_opengl()

if(WIN32)
	set(dx11_libs d3d11 d3dx11)
	find_directX("${dx11_libs}")
endif()


# Build plugin
build_plugin()

