cmake_minimum_required(VERSION 3.19.6)

project(OpenGL-Game)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(OutputDirectory ${CMAKE_BUILD_TYPE})
set(LibraryDirectory ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/Vendor)

add_link_options(
	-fuse-ld=lld
	-static
)

add_subdirectory(OpenGL-Game ${OutputDirectory}/OpenGL-Game)
add_subdirectory(OpenGL-Game/Vendor/glfw ${LibraryDirectory}/glfw)
add_subdirectory(OpenGL-Game/Vendor/glad ${LibraryDirectory}/glad)
