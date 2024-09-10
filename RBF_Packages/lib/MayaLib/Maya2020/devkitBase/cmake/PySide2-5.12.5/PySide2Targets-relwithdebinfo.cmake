#----------------------------------------------------------------
# Generated CMake target import file for configuration "RelWithDebInfo".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PySide2::pyside2" for configuration "RelWithDebInfo"
set_property(TARGET PySide2::pyside2 APPEND PROPERTY IMPORTED_CONFIGURATIONS RELWITHDEBINFO)
set_target_properties(PySide2::pyside2 PROPERTIES
  IMPORTED_IMPLIB_RELWITHDEBINFO "${_IMPORT_PREFIX}/lib/pyside2-python2.7.lib"
  IMPORTED_LOCATION_RELWITHDEBINFO "${_IMPORT_PREFIX}/bin/pyside2-python2.7.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS PySide2::pyside2 )
list(APPEND _IMPORT_CHECK_FILES_FOR_PySide2::pyside2 "${_IMPORT_PREFIX}/lib/pyside2-python2.7.lib" "${_IMPORT_PREFIX}/bin/pyside2-python2.7.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
