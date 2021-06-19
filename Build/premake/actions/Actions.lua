newaction
{
	trigger = "clean",
	description = "Cleans the whole workspace",
	onProject = function(prj)
		print("Cleaning " .. prj.name)
		os.remove("./Binaries/**")
		os.remove("./**Makefile")
	end
}
