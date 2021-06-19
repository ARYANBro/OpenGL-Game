project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("%{wks.location}/Binaries/%{cfg.buildcfg}/Vendor/%{prj.name}")
	objdir ("%{wks.location}/Binaries/%{cfg.buildcfg}/Vendor/%{prj.name}/Objects")

	files
	{
		"src/glad.c"
	}

	includedirs
	{
		"include"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
