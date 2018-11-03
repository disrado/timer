cmake_minimum_required(VERSION 3.8)

project(libtimer LANGUAGES CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME}
	STATIC
		${${PROJECT_NAME}_SOURCES})

target_compile_options(${PROJECT_NAME}
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:
			/MP /W3 /Zf
			$<$<CONFIG:Debug>:/MTd>
			$<$<CONFIG:Release>:/MT>>
		$<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
			-Wall -Wextra -Werror -Wpedantic -pedantic-errors -pipe -pthread>
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
)