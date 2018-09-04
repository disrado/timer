cmake_minimum_required(VERSION 3.8)

project(timer LANGUAGES CXX)

add_subdirectory(modules/libtimer)
add_subdirectory(modules/app)

if (ENABLE_TESTING)
	enable_testing()
	add_subdirectory(tests)
endif ()