if (NOT PYTHON_CONFIG_SUFFIX)
    message(STATUS "PySide2Config: Using default python: .cp310-win_amd64")
    SET(PYTHON_CONFIG_SUFFIX .cp310-win_amd64)
endif()
include(${CMAKE_CURRENT_LIST_DIR}/PySide2Config${PYTHON_CONFIG_SUFFIX}.cmake)
