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
set(PROJECT_NAME AbcImport)

# set mel files
set(MEL_FILES 
	AlembicCreateUI.mel AEalembicNodeTemplate.mel
    performAlembicExport.mel performAlembicImport.mel
    doAlembicExportArgList.mel doAlembicImportArgList.mel
    insertPipelineSubMenu.mel AbcImportInitStrings.mel
	)

# set SOURCE_FILES
set(SOURCE_FILES
   AbcImport.cpp AbcImportStrings.cpp AlembicNode.cpp CreateSceneHelper.cpp 
      main.cpp MeshHelper.cpp NodeIteratorVisitorHelper.cpp 
      PointHelper.cpp util.cpp XformHelper.cpp
      CameraHelper.cpp NurbsCurveHelper.cpp
	  LocatorHelper.cpp NurbsSurfaceHelper.cpp
      AlembicImportFileTranslator.cpp
    AbcImport.h AbcImportStrings.h AlembicNode.h CreateSceneHelper.h
      MeshHelper.h NodeIteratorVisitorHelper.h
      PointHelper.h util.h XformHelper.h 
      CameraHelper.h NurbsCurveHelper.h
	  LocatorHelper.h NurbsSurfaceHelper.h
      AlembicImportFileTranslator.h
)

# set linking libraries
set(LIBRARIES
     OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI
)

# find Alembic
find_alembic()

# Build plugin
build_plugin()

