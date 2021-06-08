cmake_minimum_required(VERSION 3.19.6)

project(stb_image)

add_library(${PROJECT_NAME} INTERFACE)

target_sources(${PROJECT_NAME}
	INTERFACE source/stb_image.cpp
)

target_include_directories(${PROJECT_NAME}
	INTERFACE include
)