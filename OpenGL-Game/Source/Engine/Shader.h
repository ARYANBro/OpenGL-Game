#pragma once

#include "ResourceLibrary.h"

#include <glad/glad.h>

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

private:
	GLint GetUniformLocation(const std::string& name) const;
	std::string ReadFile(const std::string& filePath);
	std::unordered_map<GLenum, std::string> ParseShaders(const std::string& file) noexcept;

private:
	std::uint_fast32_t m_RendererID;
	const std::string& m_FilePath;
};

class ShaderLibrary : public ResourceLibrary<Shader, std::string>
{
private:
	virtual std::shared_ptr<Shader> LoadResource(const std::string& filePath) override
	{
		return std::make_shared<Shader>(filePath);
	}

	virtual bool Compare(const Shader& shader, const std::string& filePath) override
	{
		return shader.GetFilePath() == filePath;
	}
};
