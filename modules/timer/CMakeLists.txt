cmake_minimum_required(VERSION 3.8)

project(app LANGUAGES CXX)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME}
	${${PROJECT_NAME}_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
	PUBLIC
		libtimer
)