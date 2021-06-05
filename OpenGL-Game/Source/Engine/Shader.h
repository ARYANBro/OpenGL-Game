#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <glad/glad.h>

class Shader
{
public:
	Shader() noexcept;

	void Load(const std::string& filePath);
	void Bind() const noexcept;
	void Unbind() const noexcept;

private:
	std::string ReadFile(const std::string& filePath);
	std::unordered_map<GLenum, std::string> ParseShaders(const std::string& file) noexcept;

private:
	std::uint_fast32_t m_RendererID;
};
