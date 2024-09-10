#-
# ==========================================================================
# Copyright (c) 2020 Autodesk, Inc.
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
set(PROJECT_NAME geometrySurfaceConstraint)

# set SOURCE_FILES
set(SOURCE_FILES
   geometrySurfaceConstraint.cpp
   geometrySurfaceConstraint.h
)

# set linking libraries
set(LIBRARIES
   OpenMaya
   Foundation
   OpenMayaAnim
)

# Build plugin
build_plugin()

