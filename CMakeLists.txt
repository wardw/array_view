cmake_minimum_required(VERSION 3.5)
project(array_view)

option(BUILD_TESTS "Build unit tests. Requires GTest." ON)
option(BUILD_GTEST "Build GTest along with this project (ON) or use an existing installation (OFF)." ON)

# An interface target for array_view
add_library(array_view INTERFACE)
add_library(array_view::array_view ALIAS array_view)

target_include_directories(array_view INTERFACE .)
target_compile_features(array_view INTERFACE cxx_std_14)

# Export target, importable from build directory
export(TARGETS array_view
	NAMESPACE array_view::
	FILE array_viewConfig.cmake
)

if(BUILD_TESTS)

	add_executable(av_test "array_view/array_view_test.cpp")
	target_link_libraries(av_test array_view::array_view )

	if(BUILD_GTEST)
		set(GTEST_ROOT $ENV{GTEST_ROOT} CACHE PATH "Path to GTest directory")
		if("${GTEST_ROOT}" STREQUAL "")
    		message(FATAL_ERROR "GTEST_ROOT unspecified. Set GTEST_ROOT to point to your GTest directory.")
		endif()

		file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/gtest")
		add_subdirectory("${GTEST_ROOT}" "${CMAKE_BINARY_DIR}/gtest")
		include_directories("${GTEST_ROOT}/include")
		target_link_libraries(av_test gtest gtest_main pthread)
	else()
		find_package(GTest REQUIRED)
		target_link_libraries(av_test GTest::GTest GTest::Main)
	endif()

endif()
