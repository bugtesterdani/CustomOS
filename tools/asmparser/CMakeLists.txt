cmake_minimum_required(VERSION 3.22.1)

###################################
# Project configuration
project(asmparser
		LANGUAGES				CXX
		VERSION					0.1)

set(CMAKE_INCLUDE_CURRENT_DIR	ON)
set(CMAKE_CXX_STANDARD			20)

###################################
# Options
option(DEBUG "Enter debug" OFF)

###################################
# Build info
#add_definitions("-Wall")
#add_definitions("-Wextra")
#add_definitions("-Werror")

# cmake -DDEBUG=ON
if (DEBUG)
  add_definitions(-DDEBUG)
endif()

###################################
# Sources
add_subdirectory(src)

###################################
# App
set(MAIN_APP ${PROJECT_NAME})