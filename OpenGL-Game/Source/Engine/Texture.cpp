#include "Texture.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <stdexcept>

Texture::Texture(const std::string& filePath)
	: m_FilePath(filePath)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	int numChannels;

	int width, height;
	stbi_set_flip_vertically_on_load(true);
	const void* pixelData = stbi_load(m_FilePath.c_str(), &width, &height, &numChannels, 0);

	m_Width = width;
	m_Height = height;

	if (pixelData == nullptr)
		throw std::runtime_error("Could not open: " + m_FilePath);

	GLenum internalFormat;
	GLenum dataFormat;

	switch (numChannels)
	{
		case 1:
			internalFormat = GL_R8;
			dataFormat = GL_RED;
			break;

		case 2:
			internalFormat = GL_RG8;
			dataFormat = GL_RG;
			break;

		case 3:
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
			break;

		case 4:
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
			break;
	}

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTextureStorage2D(m_RendererID, 3, internalFormat, m_Width, m_Height);
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, pixelData);
	glGenerateTextureMipmap(m_RendererID);
}

Texture::~Texture() noexcept
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const noexcept
{
	m_Slot = slot;
	glBindTextureUnit(slot, m_RendererID);
}

void Texture::Unbind() const noexcept
{
	glBindTextureUnit(m_Slot, 0);
}

template<>
bool ResourceLibrary<Texture, std::string>::Compare(const Texture& texture, const std::string& param)
{
	return texture.GetTexturePath() == param;
}

template<>
std::shared_ptr<Texture> ResourceLibrary<Texture, std::string>::CreateResource(const std::string& param)
{
	return std::make_shared<Texture>(param);
}
