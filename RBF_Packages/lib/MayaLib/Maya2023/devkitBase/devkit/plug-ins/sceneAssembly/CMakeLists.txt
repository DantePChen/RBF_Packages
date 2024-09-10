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
set(PROJECT_NAME sceneAssembly)

#set .mel file list
set(MEL_FILES 
    sceneAssemblyInitStrings.mel
)

# set SOURCE_FILES
set(SOURCE_FILES
    sceneAssemblyPluginMain.cpp
    assemblyReference.cpp
    assemblyDefinition.cpp
    assemblyDefinitionFileCache.cpp
    adskRepresentations.cpp
    adskRepresentationFactory.cpp
    adskRepresentationCmd.cpp
    assemblyReferenceInitialRep.cpp
    adskPrepareRenderGlobals.cpp
    adskSceneMetadataCmd.cpp
    sceneAssemblyStrings.h
    assemblyReference.h
    assemblyDefinition.h
    assemblyDefinitionFileCache.h
    adskRepresentations.h
    adskRepresentationFactory.h
    adskRepresentationCmd.h
    assemblyReferenceInitialRep.h
    adskPrepareRenderGlobals.h
    adskSceneMetadataCmd.h
)

# set linking libraries
set(LIBRARIES
    OpenMaya
    Foundation
    MetaData
)

set(PLUGIN_DESTINATION binPlugin)

# Build plugin
build_plugin()
