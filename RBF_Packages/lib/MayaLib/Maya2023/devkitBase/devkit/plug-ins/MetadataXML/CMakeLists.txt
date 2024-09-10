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
set(PROJECT_NAME MetadataXML)

#set resource file list
set(RESOURCES_FILES 
    structureSchema.xsd
    structureSample.xml
    metadataSchema.xsd
    metadataSample.xml)


# set SOURCE_FILES
set(SOURCE_FILES
   metadataXMLPlugin.cpp
   metadataXML.cpp
   associationsSerializerXML.cpp
   channelSerializerXML.cpp
   streamSerializerXML.cpp
   structureSerializerXML.cpp
   metadataXMLPluginStrings.h
   metadataXML.h
   associationsSerializerXML.h
   channelSerializerXML.h
   streamSerializerXML.h
   structureSerializerXML.h
   ${RESOURCES_FILES}
)

# set linking libraries
set(LIBRARIES
     OpenMaya
     Foundation
     MetaData
)

find_libxml2()




# Build plugin
build_plugin()

