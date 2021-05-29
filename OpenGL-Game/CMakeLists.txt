cmake_minimum_required(VERSION 3.19.6)

project(OpenGL-Game)

file(GLOB_RECURSE SourceFiles
	Source/*.cpp
)

add_executable(${PROJECT_NAME} ${SourceFiles})

target_link_libraries(${PROJECT_NAME}
	glfw
	glad
)

target_include_directories(${PROJECT_NAME} 
	PUBLIC Source
)
