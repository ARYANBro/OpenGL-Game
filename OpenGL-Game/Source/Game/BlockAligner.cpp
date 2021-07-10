#include "BlockAligner.h"

#include <Engine/Application.h>
#include <Engine/Texture.h>

BlockAligner::BlockAligner(int numBlocksX, int numBlocksY, std::uint_fast32_t windowWidth) noexcept
	: m_NumBlocksX(numBlocksX), m_NumBlocksY(numBlocksY), m_UnitBlockSizeX(static_cast<float>(windowWidth) / m_NumBlocksX)
{
}

glm::vec2 BlockAligner::GetBlockPosition(float hToWRatio) noexcept
{
	glm::vec2 scale = GetBlockScale(hToWRatio);
	glm::vec2 pos = { static_cast<float>(m_XAligned) * scale.x, static_cast<float>(m_YAligned) * scale.y };

	m_XAligned++;

	if (m_XAligned == m_NumBlocksX)
	{
		m_XAligned = 0;
		m_YAligned++;
	}
	
	return pos;
}

glm::vec2 BlockAligner::GetBlockScale(float hToWRatio) noexcept
{
	glm::vec2 scale;

	scale.x = m_UnitBlockSizeX;
	scale.y = hToWRatio * m_UnitBlockSizeX;

	return scale;
}
