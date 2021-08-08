#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>

#include <Engine/Texture.h>

class Scene;
class DeltaTime;

class GameLevel
{
public:
	GameLevel(const std::vector<int>& levelLayout, glm::vec2 levelExtents) noexcept
		: m_LevelLayout(levelLayout), m_LevelExtents(levelExtents) {}

	void OnBegin(Scene& scene);
	void OnUpdate(Scene& scene, const DeltaTime& dt);
	void OnEnd(Scene& scene);

	void Load(Scene& scene) noexcept;

	void SetColorScheme(const std::unordered_map<int, glm::vec3>& colorScheme) noexcept { m_ColorScheme = colorScheme; }
	void SetBlockTexture(Texture* texture) noexcept { m_BlockTexture = texture; }
	void SetSolidBlockTexture(Texture* texture) noexcept { m_SolidBlockTexture = texture; }
	constexpr void SetBlockTexRatio(float ratio) noexcept { m_TexRatio = ratio; }

	const std::vector<int>& GetLevelLayout() const noexcept { return m_LevelLayout; }
	glm::vec2 GetLevelExtents() const noexcept { return m_LevelExtents; }
	const std::unordered_map<int, glm::vec3>& GetColorScheme() const noexcept { return m_ColorScheme; }
	Texture* GetBlockTexture() const noexcept { return m_BlockTexture; }
	Texture* GetSolidBlockTexture() const noexcept { return m_SolidBlockTexture; }
	constexpr float GetTexRatio() const noexcept { return m_TexRatio; }

private:
	std::vector<int> m_LevelLayout;
	glm::vec2 m_LevelExtents;
	std::unordered_map<int,  glm::vec3> m_ColorScheme;
	Texture* m_BlockTexture;
	Texture* m_SolidBlockTexture;
	float m_TexRatio;
};