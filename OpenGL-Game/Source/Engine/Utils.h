#pragma once

#include <string>

namespace Utils
{
	std::string GetFileDirectory(const std::string& filepath) noexcept;
	std::string GetFileName(const std::string& filepath) noexcept;
	size_t GetFileExtensionPos(const std::string& filepath) noexcept;
	std::string GetFileExtension(const std::string& filepath) noexcept;
	std::string GetFileBaseName(const std::string& filepath) noexcept;
}

