cmake_minimum_required(VERSION 3.8)

project(libtimer LANGUAGES CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")

add_library(${PROJECT_NAME}
	STATIC
		${${PROJECT_NAME}_SOURCES})

target_compile_options(${PROJECT_NAME}
	PUBLIC
		$<$<CXX_COMPILER_ID:MSVC>:
			/W3 /MP4
			$<$<CONFIG:Debug>:/MTd>
			$<$<CONFIG:Release>:/MT>>
)
target_include_directories(${PROJECT_NAME}
	PUBLIC
		include
)