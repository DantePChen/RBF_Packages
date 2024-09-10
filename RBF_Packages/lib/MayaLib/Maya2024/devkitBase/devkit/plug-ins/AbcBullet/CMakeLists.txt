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
set(PROJECT_NAME AbcBullet)


# set SOURCE_FILES
set(SOURCE_FILES
    AbcBullet.cpp AbcWriteJob.cpp AttributesWriter.cpp 
      MayaTransformWriter.cpp MayaUtility.cpp
      MayaTransformCollectionWriter.cpp 
    AbcBullet.h AbcWriteJob.h AttributesWriter.h
      Foundation.h 
      MayaTransformWriter.h MayaUtility.h
      MayaTransformCollectionWriter.h 
      AbcBulletStringTable.h 
)

# set linking libraries
set(LIBRARIES
     OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI
)

# find Alembic
find_alembic()

# Build plugin
build_plugin()

