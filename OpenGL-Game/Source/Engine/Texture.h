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

	const std::string& GetTexturePath() const noexcept { return m_FilePath; }

private:
	std::uint_fast32_t m_RendererID;
	mutable unsigned int m_Slot;
	std::string m_FilePath;
};

class TextureLibrary : public ResourceLibrary<Texture, std::string>
{
private:
	virtual std::shared_ptr<Texture> LoadResource(const std::string& filePath) override
	{
		return std::make_shared<Texture>(filePath);
	}

	virtual bool Compare(const Texture& texture, const std::string& filePath) override
	{
		return texture.GetTexturePath() == filePath;
	}
};