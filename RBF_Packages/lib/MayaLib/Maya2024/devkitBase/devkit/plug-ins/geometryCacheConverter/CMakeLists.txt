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
set(PROJECT_NAME geometryCacheConverter)



# set SOURCE_FILES
set(SOURCE_FILES
   geometryCacheBlockBase.cpp
   geometryCacheBlockDVAData.cpp
   geometryCacheBlockFVAData.cpp
   geometryCacheBlockIntData.cpp
   geometryCacheBlockStringData.cpp
   geometryCacheConverter.cpp
   geometryCacheFile.cpp
   geometryCacheBlockBase.h
   geometryCacheBlockDVAData.h
   geometryCacheBlockFVAData.h
   geometryCacheBlockIntData.h
   geometryCacheBlockStringData.h
   geometryCacheFile.h
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     OpenMayaAnim
     Foundation

)





# Build plugin
build_plugin()

