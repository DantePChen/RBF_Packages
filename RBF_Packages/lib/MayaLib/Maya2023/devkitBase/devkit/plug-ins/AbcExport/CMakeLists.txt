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
set(PROJECT_NAME AbcExport)


# set SOURCE_FILES
set(SOURCE_FILES
   AbcExport.cpp AbcWriteJob.cpp AttributesWriter.cpp 
      MayaMeshWriter.cpp MayaPointPrimitiveWriter.cpp 
      MayaTransformWriter.cpp MayaUtility.cpp
      MayaCameraWriter.cpp MayaNurbsCurveWriter.cpp
	  MayaLocatorWriter.cpp MayaNurbsSurfaceWriter.cpp
    AbcExport.h AbcWriteJob.h AttributesWriter.h
      Foundation.h MayaMeshWriter.h MayaPointPrimitiveWriter.h
      MayaTransformWriter.h MayaUtility.h
      MayaCameraWriter.h MayaNurbsCurveWriter.h
	  MayaLocatorWriter.h MayaNurbsSurfaceWriter.h
)

# set linking libraries
set(LIBRARIES
     OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI
)

# find z
find_zlib()

# find Alembic
find_alembic()

# Build plugin
build_plugin()

