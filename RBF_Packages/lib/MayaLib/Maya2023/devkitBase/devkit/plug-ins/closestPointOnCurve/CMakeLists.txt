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
set(PROJECT_NAME closestPointOnCurve)

#set .mel file list
set(MEL_FILES 
    AEclosestPointOnCurveTemplate.mel
    closestPointOnCurveInitStrings.mel)


# set SOURCE_FILES
set(SOURCE_FILES
   closestPointOnCurveCmd.cpp
   closestPointOnCurveNode.cpp
   closestPointOnCurvePlugin.cpp
   closestTangentUAndDistance.cpp
   closestPointOnCurveCmd.h
   closestPointOnCurveNode.h
   closestTangentUAndDistance.h
   closestPointOnCurveStrings.h
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     Foundation
)

find_opengl()

# only for plugins in Maya
set(PLUGIN_DESTINATION BonusTools)


# Build plugin
build_plugin()

