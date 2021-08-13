#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <cassert>
#include <forward_list>
#include <memory>
#include <sstream>
#include <optional>

static GLenum ShaderNameToType(const std::string& shaderTypeString)
{
	if (shaderTypeString == "vertex")
		return GL_VERTEX_SHADER;
	else if (shaderTypeString == "fragment")
		return GL_FRAGMENT_SHADER;

	throw std::runtime_error(shaderTypeString + " Shader type not found");
}

static std::string ShaderTypeToName(GLenum shaderType)
{
	switch (shaderType)
	{
		case GL_VERTEX_SHADER:
			return "vertex";
		case GL_FRAGMENT_SHADER:
			return "fragment";
		default:
			throw std::runtime_error("shader type given as an argument is not implemented");
	}
}

static std::optional<std::string> ShaderCheckErrors(GLenum shaderType, GLuint shader) noexcept
{
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

		auto buffer = std::make_unique<char[]>(length);

		glGetShaderInfoLog(shader, length, nullptr, buffer.get());
		return buffer.get();
	}

	return std::nullopt;
}

static GLuint CompileShader(GLenum shaderType, std::string shaderSource)
{
	GLuint shader = glCreateShader(shaderType);
	const char* source = shaderSource.c_str();

	glShaderSource(shader, 1,  &source, nullptr);
	glCompileShader(shader);

	if (auto error = ShaderCheckErrors(shaderType, shader))
	{
		std::string shaderName;
		try
		{
			shaderName = ShaderTypeToName(shaderType);
		}
		catch (const std::exception& e)
		{
			shaderName = "null";
		}

		std::string exceptionMessage = ShaderTypeToName(shaderType) + " shader failed to compile:\n" + error.value();
		throw std::runtime_error(exceptionMessage);
	}

	return shader;
}

static std::optional<std::string> ShaderProgramCheckError(GLuint shaderProgram) noexcept
{
	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);

	if (!linkStatus)
	{
		GLint length;
		glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);

		auto buffer = std::make_unique<char[]>(length);

		glGetProgramInfoLog(shaderProgram, length, nullptr, buffer.get());
		return buffer.get();
	}

	return std::nullopt;
}	

static void LinkgShaderProgram(GLuint shaderProgram)
{
	glLinkProgram(shaderProgram);

	if (auto error = ShaderProgramCheckError(shaderProgram))
	{
		using namespace std::string_literals;

		std::string exceptionMessage = "Shader program faild to link:\n"s + error.value();
		throw std::runtime_error(exceptionMessage);
	}
}

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath)
{
	m_RendererID = glCreateProgram();

	// Read the file path
	std::string file = ReadFile(filePath);

	// Differentiate the shaders
	auto shaders = ParseShaders(file);

	std::forward_list<GLuint> compiledShaders;

	for (auto& shader : shaders)
	{
		GLenum shaderType = shader.first;
		std::string shaderSource = shader.second;
		GLuint glShader = CompileShader(shaderType, shaderSource);

		glAttachShader(m_RendererID, glShader);

		compiledShaders.push_front(glShader);
	}

	for (GLuint shader : compiledShaders)
		glDeleteShader(shader);

	LinkgShaderProgram(m_RendererID);
}

Shader::~Shader() noexcept
{
	glDeleteProgram(m_RendererID);
}

std::string Shader::ReadFile(const std::string& filePath)
{
	std::ifstream file;
	file.exceptions(std::ios::badbit);
	file.open(filePath);

	std::size_t fileSize;

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	std::string fileString;
	fileString.resize(fileSize);

	file.read(fileString.data(), fileString.size());

	return fileString;
}

std::unordered_map<GLenum, std::string> Shader::ParseShaders(const std::string& file) noexcept
{

	std::string searchToken = "#shader ";
	std::size_t shaderTokenPos = file.find(searchToken);

	std::unordered_map<GLenum, std::string> shaders;

	while (shaderTokenPos != std::string::npos)
	{
		std::size_t newLine = file.find('\n', shaderTokenPos);

		std::size_t numCharacters = newLine - (shaderTokenPos + searchToken.size());
		std::string shaderTypeStr = file.substr(shaderTokenPos + searchToken.size(), numCharacters);

		std::size_t nextSearchTokenPos = file.find(searchToken, shaderTokenPos + searchToken.size());

		std::size_t charsToAssign = nextSearchTokenPos != std::string::npos ? nextSearchTokenPos - shaderTokenPos - searchToken.size() - numCharacters : std::string::npos;

		GLenum shaderType;
		
		try
		{
			shaderType = ShaderNameToType(shaderTypeStr);
		}
		catch (...)
		{
			std::string message = shaderTypeStr + " shader type is not valid or not implemented yet";
			assert(false && message.c_str());
		}

		shaders[shaderType] = file.substr(shaderTokenPos + searchToken.size() + shaderTypeStr.size(), charsToAssign);

		shaderTokenPos = nextSearchTokenPos;
	}

	return shaders;
}

void Shader::Bind() const noexcept
{
	glUseProgram(m_RendererID);	
}

void Shader::Unbind() const noexcept
{
	glUseProgram(0);
}

void Shader::SetUInt(const std::string& name, std::uint_fast32_t value) const
{
	glProgramUniform1ui(m_RendererID, GetUniformLocation(name), value);
}

void Shader::SetInt(const std::string& name, std::int_fast32_t value) const 
{
	glProgramUniform1i(m_RendererID, GetUniformLocation(name), value);
}

void Shader::SetFloat3(const std::string& name, glm::vec3 value) const
{
	glProgramUniform3f(m_RendererID, GetUniformLocation(name), value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, glm::vec4 value) const 
{
	glProgramUniform4f(m_RendererID, GetUniformLocation(name), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& matrix) const
{
	glProgramUniformMatrix4fv(m_RendererID, GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	GLint location = glGetUniformLocation(m_RendererID, name.c_str());

	if (location == -1)
		throw std::runtime_error("Could not find uniform: " + name);

	return location;
}

template<>
bool ResourceLibrary<Shader, std::string>::Compare(const Shader& shader, const std::string& param)
{
	return shader.GetFilePath() == param;
}

template<>
std::shared_ptr<Shader> ResourceLibrary<Shader, std::string>::CreateResource(const std::string& param)
{
	return std::make_shared<Shader>(param);
}