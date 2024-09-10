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
set(PROJECT_NAME loopfile)

# set SOURCE_FILES
set(SOURCE_FILES
	loopfile.c
)

find_package(Mocap REQUIRED)

# set Package library
set(PACKAGE_LIBS ${MAYA_MOCAP_LIBRARY})

if (WIN32)
	set(PACKAGE_LIBS ${PACKAGE_LIBS} ws2_32.lib winmm.lib)
endif()


build_plugin()


