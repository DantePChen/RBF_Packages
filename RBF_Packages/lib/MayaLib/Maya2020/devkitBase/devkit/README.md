# Overview & Setup
This file describes how to build the Maya Dev Kit examples. To make sure developers are able to use the development tools of their choice, the Maya Dev Kit uses [CMake](https://cmake.org) to create platform-specific build project files.

Before getting started,  building examples, make sure you've downloaded and installed CMake. See https://cmake.org/download/ for details.

Once you’ve installed cmake, download and install the Maya dev kit to a directory of your choice. Make sure not to install the Dev Kit to your Maya installation directory (e.g.,  `C:\"Program Files"\Autodesk\Maya2020”`).

Lastly, create the environment variable `DEVKIT_LOCATION` and set it to the `devkitBase` subdirectory. To build devkit application examples in Linux, create the environment variable `MAYA_LOCATION` and set it to where Maya is installed (default location:  `/usr/autodesk/maya2020`)

# Generating and Building Projects
Each Maya Dev Kit example contains a `CMakeList.txt` file. These files are used to generate input files for the native build system of your choice, or other supported build systems, such as generating Makefiles in OSX with the following Linux commands. For a complete list of supported CMake generators consult the [CMake documentation](https://cmake.org/cmake/help/v3.0/manual/cmake-generators.7.html). 

Note: Linux compiler version should be [Red Hat Developer Toolset 6.1](https://www.softwarecollections.org/en/scls/rhscl/devtoolset-6/) (DTS-6.1) (gcc 6.3.1)

To generate application and plug-in specific projects:

	* Navigate to the Dev Kit sample directory you wish to build
	* To generate a platform-specific build solution and place all files in a new directory called `build`  type the following instructions in a command shell:
	
	**Windows**
	`cmake -H. -Bbuild -G "Visual Studio 15 2017 Win64"`
	
	**OSX**
	`cmake -H. -Bbuild -G Xcode`
	
	**Linux**
	`cmake -H. -Bbuild -G "Unix Makefiles"`

Alternatively you can select the generator you wish to use via the `cmake-gui` executable which comes with cmake. Please refer to [cmake-gui instructions](https://cmake.org/runningcmake/) for more details.

Once a platform specific build solution has been created, you can open the project in the corresponding tool (e.g., Visual Studio, Xcode, etc.) or you can trigger a build from the command line using the command:
 `cmake --build build`

# Creating your own CMakeLists.txt file
You can create your own `CMakeLists.txt` files for plug-ins and applications that use the Maya API. 

For reference, have a look at the example files in `$ENV{DEVKIT_LOCATION}/devkit/applications`

A typical `CMakeLists.txt` file is structured as  follows:
```
cmake_minimum_required(VERSION 2.8)
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
	find_zlib()
	find_alembic()
	
  # PART 6 - add the plugin building function name 
	build_plugin()
```

To adapt this project for your needs:

	* Change the `PROJECT_NAME` variable to something different
	* Add the source code files you need included in your executable by modifying `# PART 3`
	* Add additional libraries you need to link against in `# PART 4`.
	* Add new `Find***.cmake` files to include libraries your code may  depend on. For example if your application / plug-in relies on libxml2 you can use the prepared macro `find_z()`.  If you want to add new libraries, write new `Find***.cmake` files to put in ./cmake/modules.
	 

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
