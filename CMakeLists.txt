cmake_minimum_required(VERSION 2.8)
project(array_view)

# Only tested on OS X
# Set GTEST_ROOT to point to your GTEST installation

option(BUILD_GTEST "Build GTest with this project" ON)

if(DEFINED ENV{GTEST_ROOT})
	message("array_view: Using Gtest from GTEST_ROOT at $ENV{GTEST_ROOT}")
	set(GTEST_ROOT $ENV{GTEST_ROOT})
else()
	message(FATAL_ERROR "array_view: Gtest not found. Set path to GTest installation in GTEST_ROOT")
endif()


# Single source file
set(gtest_src "array_view/array_view_test.cpp")

# Build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

enable_testing()

include_directories("." "${GTEST_ROOT}/include")
add_executable(array_view ${gtest_src})


if(BUILD_GTEST)
	# Tested with GTest 1.7.0
	file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/gtest")
	add_subdirectory($ENV{GTEST_ROOT} "${CMAKE_BINARY_DIR}/gtest")
	target_link_libraries(array_view gtest gtest_main pthread)
else()
	# Implies gtest libs have been previously built
	find_package(GTest REQUIRED)
	target_link_libraries(array_view ${GTEST_BOTH_LIBRARIES})
endif()


# CTest. Doing it this way ensures 'make check' depends on the array_view build target
add_test(NAME all COMMAND array_view)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS array_view)
