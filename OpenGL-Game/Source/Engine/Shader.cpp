#include "Shader.h"

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

Shader::Shader() noexcept
{
	m_RendererID = glCreateProgram();
}

void Shader::Load(const std::string& filePath)
{
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