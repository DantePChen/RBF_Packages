#-
# ==========================================================================
# Copyright (c) 2018 Autodesk, Inc.
# All rights reserved.
# 
# These coded instructions, statements, and computer programs contain
# unpublished proprietary information written by Autodesk, Inc., and are
# protected by Federal copyright law. They may not be disclosed to third
# parties or copied or duplicated in any form, in whole or in pgit art, without
# the prior written consent of Autodesk, Inc.
# ==========================================================================
#+


cmake_minimum_required(VERSION 3.13)

# include the project setting file
include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

# specify project name
set(PROJECT_NAME cleanPerFaceAssignment)

#set .mel file list
set(MEL_FILES 
    cleanPerFace.mel)


# set SOURCE_FILES
set(SOURCE_FILES
   cleanPerFaceAssignmentCmd.cpp
   cleanPerFaceAssignmentPluginMain.cpp
   cleanPerFaceAssignmentCmd.h
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     Foundation

)

# Build plugin
build_plugin()

