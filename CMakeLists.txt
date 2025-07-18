# Base CMakeLists file - set up all executables

# Setup the project
cmake_minimum_required(VERSION 3.10)


set(CMAKE_BUILD_TYPE None)
message("cmake version ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}")
message("cmake install prefix: ${CMAKE_INSTALL_PREFIX}")
message("source directory: ${CMAKE_CURRENT_SOURCE_DIR}")

project(doublecap VERSION 1.0
        LANGUAGES CXX
        DESCRIPTION "Geant4 project to simulate double-detector neutron-capture-induced defect formation measurement")

# for IDEs
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 17 used for Geant 4-11.1-release
set(CMAKE_CXX_STANDARD 17) 

# required for newest Geant4 build apparently
set(CMAKE_OSX_DEPLOYMENT_TARGET 15.0)

option(IGNORE_WARNINGS "Ignore all warnings with -w flag" OFF)
if (IGNORE_WARNINGS)
	add_definitions(-w)
	message(WARNING "Ignoring all warnings")
endif()


# =============================================
# Geant4

list(APPEND CMAKE_PREFIX_PATH "$ENV{G4INSTALL}")
find_package(Geant4 REQUIRED ui_all vis_all)

if (Geant4_FOUND)
	message(STATUS "Libraries for Geant4 found")
        message(STATUS "Input directory: " ${Geant4_DIR})
	message(STATUS "Package Version: ${Geant4_VERSION}")
        message(STATUS "Include DIRS: ${Geant4_INCLUDE_DIRS}")
else()
	message(FATAL_ERROR "Failed to find Geant4.")
endif()

include("${Geant4_USE_FILE}")

# =============================================
# ROOT

list(APPEND CMAKE_PREFIX_PATH "$ENV{ROOTSYS}")
find_package(ROOT REQUIRED)

if (ROOT_FOUND)
	message(STATUS "Libraries for ROOT found")
	message(STATUS "Input directory: ${ROOT_DIR}")
	message(STATUS "Package Version: ${ROOT_VERSION}")
	message(STATUS "Include DIRS: ${ROOT_INCLUDE_DIRS}")
else()
	message(FATAL_ERROR "Failed to find ROOT")
endif()

include("${ROOT_USE_FILE}")

# =============================================


# configure executables to all be put in base build directory
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

option(COPY_MACROS "Copy files from macros directory to build directory" ON)
if (COPY_MACROS)
	message(STATUS "Copying macros to build directory")
	execute_process(
		COMMAND find ${CMAKE_SOURCE_DIR}/macros -type f -exec ln -sf "{}" ${CMAKE_BINARY_DIR} ";"
	)
endif()

add_subdirectory(src)




