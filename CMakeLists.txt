cmake_minimum_required(VERSION 2.8.11)
set(PROJECT "OpenBurn")
project(${PROJECT})

#set C++14
set(CMAKE_CXX_STANDARD 14)

# Instruct CMake to run moc, rcc, and uic automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#if(NOT CMAKE_BUILD_TYPE)
#  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
#      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
#      FORCE)
#endif(NOT CMAKE_BUILD_TYPE)

# Set some platform specific GUI bundle things
if (MSVC)
    set(GUI_TYPE WIN32)
endif()

if (APPLE)
	set(GUI_TYPE MACOSX_BUNDLE)
endif()

find_package(Qt5 COMPONENTS PrintSupport Widgets Qml Quick REQUIRED)
# Directory with source code
add_subdirectory(src)