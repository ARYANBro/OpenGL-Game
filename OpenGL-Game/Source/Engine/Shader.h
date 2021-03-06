#pragma once


#include "ResourceLibrary.h"

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <forward_list>
#include <memory>
#include <cstdint>
#include <string>
#include <unordered_map>

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader() noexcept;

	void Bind() const noexcept;
	void Unbind() const noexcept;

	const std::string& GetFilePath() const noexcept { return m_FilePath; }

	void SetUInt(const std::string& name, std::uint_fast32_t value) const;
	void SetInt(const std::string& name, std::int_fast32_t value) const;

	void SetFloat3(const std::string& name, glm::vec3 value) const;
	void SetFloat4(const std::string& name, glm::vec4 value) const;

	void SetMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	GLint GetUniformLocation(const std::string& name) const;
	std::string ReadFile(const std::string& filePath);
	std::unordered_map<GLenum, std::string> ParseShaders(const std::string& file) noexcept;

private:
	std::uint_fast32_t m_RendererID;
	const std::string& m_FilePath;
};

using ShaderLibrary = ResourceLibrary<Shader, std::string>;
