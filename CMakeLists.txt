cmake_minimum_required(VERSION 3.5)
project(array_view)

option(BUILD_TESTS "Build unit tests. Requires GTest." ON)

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
	find_package(GTest REQUIRED)

	add_executable(av_test
		"array_view/array_view_test.cpp")

	target_link_libraries(av_test
		array_view::array_view GTest::GTest GTest::Main)
endif()
