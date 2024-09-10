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
set(PROJECT_NAME rockingTransformCheck)

option(CHECK_ROTATION_LIMITS_USING_ATTRIBUTES "On rockingTransformCheck.cpp, use attributes to check rotation limits." OFF)

# set SOURCE_FILES
set(SOURCE_FILES
   rockingTransformCheck.cpp
   rockingTransformCheckMain.cpp
   rockingTransform.cpp
   rockingTransformCheck.h
   rockingTransform.h
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     Foundation

)


# Build plugin
build_plugin()

