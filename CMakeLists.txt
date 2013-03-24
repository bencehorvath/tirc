cmake_minimum_required(VERSION 2.8)

set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project (tirc)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}")

include (CheckFunctionExists)
include (cmake/CheckCXX11Features.cmake)
include (cmake/TwlMacros.cmake)

option(WITH_TESTER "Build the tester app." ON)

set (tirc_VERSION_MAJOR 1)
set (tirc_VERSION_MINOR 1)

set (SOURCES_DIR "${PROJECT_SOURCE_DIR}/src")

LIST_LENGTH(cxx11_length ${CXX11_FEATURE_LIST})

if(${cxx11_length} GREATER 12 OR ${cxx11_length} EQUAL 12)
	message(STATUS "Sufficient C++11 support found.")
else()
	message(FATAL_ERROR "Insufficient C++11 support on your machine. Install GCC >= 4.7 or Clang >= 3.2")
endif()

set(Boost_USE_STATIC_LIBS 		OFF)
set(Boost_USE_MULTITHREADED 	ON)
set(Boost_USE_STATIC_RUNTIME  	OFF)

find_package (Boost 1.50.0 COMPONENTS system regex)

if (Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	set(tirclib_REQUIRED_LIBRARIES ${Boost_LIBRARIES})
elseif()
	message(FATAL_ERROR "You cannot build tirclib without Boost >= 1.50.0.")
endif()



configure_file ("${SOURCES_DIR}/tirc_config.h.in" "${PROJECT_BINARY_DIR}/tirc_config.h")



add_definitions ("-DHAVE_CONFIG")
add_definitions ("-O3 -march=native -std=gnu++11 -fabi-version=6 -Wall")


include_directories ("${PROJECT_SOURCE_DIR}/src")
include_directories("${PROJECT_BINARY_DIR}")

add_subdirectory ("src")

