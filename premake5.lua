workspace "OpenGL-Game"
	architecture "x86_64"

	configurations
	{
		"Debug",
		"Release"
	}

includeDirs = {}

includeDirs["glad"] = "%{wks.location}/OpenGL-Game/Vendor/glad/include"
includeDirs["glfw"] = "%{wks.location}/OpenGl-Game/Vendor/glfw/include"
includeDirs["glm"] = "%{wks.location}/OpenGL-Game/Vendor/glm"
includeDirs["stb_image"] = "%{wks.location}/OpenGL-Game/Vendor/stb_image/include"
includeDirs["yaml-cpp"] = "%{wks.location}/OpenGL-Game/Vendor/yaml-cpp/include"

include "OpenGL-Game"

include "OpenGL-Game/Vendor/glad"
include "OpenGL-Game/Vendor/glfw"
include "OpenGL-Game/Vendor/yaml-cpp"

include "Build/premake/actions/actions.lua"