newaction
{
	trigger = "clean",
	description = "Cleans the whole workspace",
	onProject = function(prj)
		print("Cleaning " .. prj.name)
		os.remove("./Binaries/**")
	end
}

newaction
{
	trigger = "clean-Makefiles",
	description = "Cleans generated project files",
	execute = function()
		print("Cleaning Makefiles")
		os.remove("./**Makefile");
	end
}
