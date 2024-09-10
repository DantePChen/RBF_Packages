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
set(PROJECT_NAME ik2Bsolver)

# set mel files
set(MEL_FILES ik2Bsolver.mel)

# set SOURCE_FILES
set(SOURCE_FILES
	ik2Bsolver.cpp AwVector.cpp AwQuaternion.cpp AwPoint.cpp AwMatrix.cpp
	AwVector.h AwQuaternion.h AwPoint.h AwMatrix.h AwMath.h
)

# set linking libraries
set(LIBRARIES OpenMayaAnim OpenMaya Foundation)

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR})

build_plugin()


