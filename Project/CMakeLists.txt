cmake_minimum_required(VERSION 3.19.6)

project(Project)

file(GLOB_RECURSE SourceFiles
	Source/*.cpp
)

add_executable(${PROJECT_NAME} ${SourceFiles})

target_include_directories(${PROJECT_NAME} 
	PUBLIC Source
)
