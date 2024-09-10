# Overview & Setup
This file describes how to build Maya Dev Kit examples. 

Since the Maya Dev Kit uses [CMake](https://cmake.org) to create platform-specific build files, before you can start creating Dev Kit examples, you'll first need to [download and install CMake|https://cmake.org/download]. 

Once cmake is installed, download and install the Maya Dev Kit to a directory of your choice. 

> Note: Make sure not to install the Dev Kit to your Maya installation directory (e.g.,  `C:\"Program Files"\Autodesk\Maya<version>`.)

Next, you'll want to make sure you have a compatible C++ compiler on your system. The Dev Kit requires the following compilers:

| OS         | Compiler  |
|------------|------------|
| Linux      | [RHEL 8.6 or higher, using DTS-11 with gcc 11.2.1] |
| Windows    | Visual Studio 2022 (17.3.X) |
| macOS      | XCode 13.4.1 or greater |

Lastly, create the environment variable `DEVKIT_LOCATION` and set it to the `devkitBase` subdirectory. To build Dev Kit application examples on Linux, set the `MAYA_LOCATION` environment variable to where Maya is installed on your machine (e.g., default location: `/usr/autodesk/maya<version>`.)

# Generating and Building Projects
Each Maya Dev Kit example contains a `CMakeList.txt` file. These files are used to generate input files for the build system of your choice. For a complete list of supported CMake generators consult the [CMake documentation](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html). 

To generate application and plug-in specific projects:

	* Navigate to the Dev Kit sample directory you wish to build
	* To generate a platform-specific build solution, and place all files in a new directory called `build`, type the following instructions in a command shell:
	
	**Windows**
	`cmake . -Bbuild -G "Visual Studio 17 2022"`
	
	**OSX**
	`cmake . -Bbuild -G Xcode`
	
	**Linux**
	`cmake . -Bbuild -G "Unix Makefiles"`

Alternatively you can select the generator you wish to use via the [cmake-gui](https://cmake.org/runningcmake/) executable, which comes with cmake.

Once a platform specific build solution has been created, you can open the project in the corresponding tool (e.g., Visual Studio, Xcode, etc.) or you can trigger a build from the command line using the command:
 `cmake --build build`

# Creating your own CMakeLists.txt file
You can create your own `CMakeLists.txt` files for plug-ins and applications that use the Maya API.

For reference, have a look at the example files in `$ENV{DEVKIT_LOCATION}/devkit/applications`

A typical `CMakeLists.txt` file is structured as follows:
```
cmake_minimum_required(VERSION 3.13)
  # PART 1 - Specify the project name
  set(PROJECT_NAME asciiToBinary)
  project(${PROJECT_NAME})

  # PART 2 - Include the project setting file / include path
  include($ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake)

  # PART 3 - Add the source files for your executable
  set(SOURCE_FILES
    mcp.cpp
  )
  
  # PART 4 - Specify the libraries in the devkit 
  set(LIBRARIES
    OpenMaya Foundation OpenMayaAnim OpenMayaFX OpenMayaRender OpenMayaUI
  )

  # PART 5 - specify the 3rd party package used in the project, please refer to $ENV{DEVKIT_LOCATION}/cmake/pluginEntry.cmake for the prepared packages.
  find_alembic()

  # PART 6 - add the plugin building function name 
  build_plugin()
```

To adapt this project for your needs:

* Change the `PROJECT_NAME` variable to something different
* Add the source code files you need included in your executable by modifying `# PART 3`
* Add additional libraries you need to link against in `# PART 4`.
* Add new `Find***.cmake` files to include libraries your code may  depend on. For example if your application / plug-in relies on libxml2 you can use the prepared macro `find_z()`. If you want to add new libraries, write new `Find***.cmake` files to put in ./cmake/modules.

If you’d like to include MEL files in your project, set the variable `${MEL_FILES}` _before_ including Devkit.cmake and include `${MEL_FILES}` as an argument to the `add_executable` command.

```
  set(MEL_FILES blah.mel foo.mel)

  # PART 1
  ...

  # PART 3
  set(SOURCE_FILES 
    file1.cpp ... fileN.cpp
    ${MEL_FILES}
  )
```

If you’d like to include resource files in your project, set the variable `RESOURCE_FILES` _before_ including Devkit.cmake and include `${RESOURCE_FILES}` as an argument to the `add_executable` command.

```
  set(MEL_FILES blah.mel foo.mel)
  set(RESOURCES_FILES myResource.xpm)

  # PART 1
  ...

  # PART 3
  set(SOURCE_FILES 
    file1.cpp ... fileN.cpp
    ${MEL_FILES}
    ${RESOURCE_FILES}
  )
```

If you are building an application instead of a plugin, replace `build_plugin` with `build_application` function in part 6 as follows:

```
build_application()
```

# Building a plug-in with Glew and CMake

Before building the cgFxShader plug-in, you will need to install glew. 

  ### PART 1
  Download the latest distribution of glew
  * On Windows you can download glew here http://glew.sourceforge.net/index.html
  * On Linux you can install glew with `apt-get update`and `apt-get install glew-utils libglew-dev`
  ### PART 2
  Edit your environment variable and add `GLEW_ROOT=D:\glew-2.1.0` or wherever you unzipped glew.
  ### PART 3
Execute the cmake command to build your plugin. The built-in `FindGlew.cmake` script also known as `find_package(GLEW)` uses the specified root to resolve the library and include paths.
### NOTES
  * The glew distribution on Windows contains both a x32, x64 folder. If CMake is unable to find glew please remove the one which does not correspond to your architecture.
