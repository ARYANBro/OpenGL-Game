#pragma once

#include <glm/glm.hpp>

class Texture;

class BlockAligner
{
public:
	BlockAligner(int numBlocksX, int numBlocksY, std::uint_fast32_t windowWidth) noexcept;

	glm::vec2 GetBlockPosition(float hToWRatio) noexcept;
	glm::vec2 GetBlockScale(float hToWRatio) noexcept;

private:
	int m_NumBlocksX;
	int m_NumBlocksY;

	float m_UnitBlockSizeX;

	int m_XAligned = 0;
	int m_YAligned = 0;
};
