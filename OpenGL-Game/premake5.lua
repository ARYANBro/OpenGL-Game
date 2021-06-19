project "OpenGL-Game"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{wks.location}/Binaries/%{cfg.buildcfg}/%{prj.name}")
	objdir ("%{wks.location}/Binaries/%{cfg.buildcfg}/%{prj.name}/Objects")

	files
	{
		"Source/**.cpp",

		-- glm
		"Vendor/glm/**.hpp",
		"Vendor/glm/**.inl",

		-- stb_image
		"Vendor/stb_image/source/stb_image.cpp"
	}

	includedirs
	{
		"Source",
		includeDirs["glad"],
		includeDirs["glfw"],
		includeDirs["glm"],
		includeDirs["stb_image"]
	}

	links
	{
		"glfw",
		"glad",
		"opengl32",
		"gdi32"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		
