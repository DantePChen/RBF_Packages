
# - Devkit module
# The cmake file to set up compilation options for devkit
# Variables that will be defined:
# DEVKIT<lib>_LIBRARY  Path to <lib> library
# DEVKIT_INCLUDE_DIR    Path to the devkit's include directories
# DEVKIT_LIBRARIES      All the devkit libraries
#


if (NOT DEFINED ENV{DEVKIT_LOCATION})
  message(STATUS "Please set DEVKIT_LOCATION environment variable to the folder of your devkit package.")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{DEVKIT_LOCATION}/cmake/modules)

# OS Specific environment setup
set(DEVKIT_CUSTOM_DEFINITIONS "_BOOL")
set(DEVKIT_INC_SUFFIX "include")
set(DEVKIT_LIB_SUFFIX "lib")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

if(WIN32)
    # Windows
    set(DEVKIT_CUSTOM_DEFINITIONS "${DEVKIT_CUSTOM_DEFINITIONS};NT_PLUGIN")
    set(OPENMAYA OpenMaya.lib)
    set(APPLICATIONSERVICES_LIB "")
elseif(APPLE)
    # Apple
    set(CMAKE_MACOSX_RPATH 1)
    set(DEVKIT_INC_SUFFIX "include")
    set(OPENMAYA libOpenMaya.dylib)
else()
    # Linux
    set(DEVKIT_CUSTOM_DEFINITIONS "${DEVKIT_CUSTOM_DEFINITIONS};LINUX")
    set(OPENMAYA libOpenMaya.so)
    set(APPLICATIONSERVICES_LIB "")
	set(LINUX TRUE)
endif()

# devkitBase  directory
set(DEVKIT_BASE_DIR $ENV{DEVKIT_LOCATION})
set(DEVKIT_DIR "${DEVKIT_BASE_DIR}/devkit")

# set dekit/cmake directory
set(DEVKIT_CMAKE_DIR "${DEVKIT_DIR}/cmake")


# Another possible library path 
if(APPLE)
	set(BUILD_LIB_RELATIVE_DIR ${DEVKIT_BASE_DIR}/../../runTime/Maya.app/Contents/MacOS)
else()
	# LINUX and Windows
	set(BUILD_LIB_RELATIVE_DIR ${DEVKIT_BASE_DIR}/../../runTime/lib)
endif()

# devkitBase library directory
find_path(DEVKIT_LIBRARY_DIR ${OPENMAYA}
    PATHS
      ${DEVKIT_BASE_DIR}/lib
	  ${BUILD_LIB_RELATIVE_DIR}
  DOC "Devkit library path"
)


# devkitBase include directory
find_path(DEVKIT_INCLUDE_DIR maya/MFn.h
  PATHS
    ${DEVKIT_BASE_DIR}
  PATH_SUFFIXES
    "${DEVKIT_INC_SUFFIX}/"
  DOC "Devkit include path"
)

set(INCLUDE_DIRS ${DEVKIT_INCLUDE_DIR})

# C++ standard (c++14 or c++17)
if (MAYA_WANT_CPP_17)
  set(CMAKE_CXX_STANDARD 17)
else()
  set(CMAKE_CXX_STANDARD 14)
endif()

#  platform defines
if( APPLE )
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
elseif( UNIX )
  set (DEVKIT_PLATFORM_FLAGS "-D_BOOL -D_GLIBCXX_USE_CXX11_ABI=0")
elseif ( WIN32 )
  set (DEVKIT_PLATFORM_FLAGS "-DNT_PLUGIN -DUSERDLL -DCRT_SECURE_NO_DEPRECATE -D_HAS_ITERATOR_DEBUGGING=0 -D_CRT_SECURE_NO_WARNINGS")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /wd4996")

  # Produce PDB in debug and release (used with /Zi compiler option )
  set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /IGNORE:4099" )
  set (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG /IGNORE:4099" )

  # Set debug mode as MD
  set(CMAKE_CXX_FLAGS_DEBUG "/MD")
else()
  message( FATAL_ERROR "Unknown platform.  Cannot set Maya platform specific C++ flags.")
endif()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

## Setup platform specific helper defines build variants
if(WIN32)
  add_definitions (-DPARTIO_WIN32)
else()
  add_definitions (-Wextra -Wno-unused-parameter -Wno-unused-variable)
endif()

if (APPLE)
  add_definitions (-Wno-unused-private-field)
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS} "-O0")
  set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS} "-Os")
endif()

# set build type to be Debug and Release
set(CMAKE_CONFIGURATION_TYPES Debug Release)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# Group the resource and mel files
source_group("Mels" FILES ${MEL_FILES})
source_group("Resources" FILES ${RESOURCES_FILES})

if (WIN32)
	set(SUFFIX_NAME ".mll")
elseif(APPLE)
	set(SUFFIX_NAME ".bundle")
else()
	 set(SUFFIX_NAME ".so")
endif()
	

