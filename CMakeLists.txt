	cmake_minimum_required(VERSION 2.8)
project(array_view)

# Tested only on OS X
# Let GTEST_ROOT point to your GTEST installation

option(BUILD_GTEST "Build GTest with this project" OFF)

if(DEFINED ENV{GTEST_ROOT})
	message("Gtest found at $ENV{GTEST_ROOT}")
	set(GTEST_ROOT $ENV{GTEST_ROOT})
else()
	message("Gtest not found. Set path in GTEST_ROOT")
	return()
endif()


# Single source file
set(gtest_src "array_view/array_view_test.cpp")

# Build options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

enable_testing()

include_directories(".")
include_directories("${GTEST_ROOT}/include")
add_executable(array_view ${gtest_src})


if(BUILD_GTEST)
	# todo: mkdir gtest
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
