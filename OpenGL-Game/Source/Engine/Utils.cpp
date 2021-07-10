#include "Utils.h"

#include <cassert>

std::string Utils::GetFileDirectory(const std::string& filepath) noexcept
{
	std::size_t lastDash = filepath.find_last_of("/\\");
	return lastDash == std::string::npos ? "." : filepath.substr(0, lastDash);
}

std::string Utils::GetFileName(const std::string& filepath) noexcept
{
	std::size_t lastDash = filepath.find_last_of("/\\");
	return lastDash == std::string::npos ? filepath : filepath.substr(lastDash + 1);
}

size_t Utils::GetFileExtensionPos(const std::string& filepath) noexcept
{
	return GetFileName(filepath).find_last_of('.');
}

std::string Utils::GetFileExtension(const std::string& filepath) noexcept
{
	std::size_t ext = GetFileExtensionPos(filepath);

	std::string errorMessage = filepath + " does not contain an extension";
	assert(ext != std::string::npos && errorMessage.c_str());
	return GetFileName(filepath).substr(ext + 1);
}

std::string Utils::GetFileBaseName(const std::string& filepath) noexcept
{
	std::string filename = GetFileName(filepath);
	return filename.substr(0, GetFileExtensionPos(filename));
}