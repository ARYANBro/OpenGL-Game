#pragma once

#include "ResourceLibrary.h"

#include <cstdint>
#include <string>
#include <forward_list>
#include <memory>

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture() noexcept;

	void Bind(unsigned int slot = 0) const noexcept;
	void Unbind() const noexcept;

	std::uint_fast32_t GetWidth() const noexcept { return m_Width; }
	std::uint_fast32_t GetHeight() const noexcept { return m_Height; }

	const std::string& GetTexturePath() const noexcept { return m_FilePath; }

private:
	std::uint_fast32_t m_Width, m_Height;
	std::uint_fast32_t m_RendererID;
	mutable unsigned int m_Slot;
	std::string m_FilePath;
};

using TextureLibrary = ResourceLibrary<Texture, std::string>;
