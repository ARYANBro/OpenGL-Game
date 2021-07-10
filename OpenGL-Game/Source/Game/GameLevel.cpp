#include "GameLevel.h"

#include "Scripts/BackGround.h"
#include "BlockAligner.h"

#include <Scene/Scene.h>
#include <Engine/Application.h>
#include <Scene/Entity.h>

void GameLevel::OnBegin(Scene& scene)
{
	Entity backGround = scene.CreateEntity();

	auto spriteRenderer = backGround.AddComponent<SpriteRendererComponent>();
	spriteRenderer->Texture = TextureLibrary::Get().Load("Assets\\Textures\\Background\\pexels-adrien-olichon-2387793.jpg");

	auto script = backGround.AddComponent<ScriptComponent>();
	script->Script = std::make_unique<BackGround>();

	for (int brickID : m_LevelLayout)
	{
		Entity block = scene.CreateEntity();
		auto tag = block.GetComponent<TagComponent>();

		switch (brickID)
		{
			case 0:
				tag->Tag = "AirBlock";
				break;
			case 1:
			{
				tag->Tag = "Block";
				auto spriteRenderer = block.AddComponent<SpriteRendererComponent>();
				spriteRenderer->Texture = m_SolidBlockTexture;
				break;
			}
			case 2:
			case 3:
			case 4:
			case 5:
			{
				tag->Tag = "Block";
				auto spriteRenderer = block.AddComponent<SpriteRendererComponent>();
				spriteRenderer->Texture = m_BlockTexture;
				spriteRenderer->Color = m_ColorScheme[brickID];
				break;
			}
			default:
				assert(false);
				break;
		}		
	}
}

void GameLevel::OnUpdate(Scene& scene, const DeltaTime& dt)
{
	BlockAligner blockAligner(15, 8, Application::GetInstance().GetWindow().GetWidth());

	scene.GetRegistry().EachComponent<TagComponent>([&, this](EntityID entity, TagComponent& tag)
	{
		auto transform = scene.GetRegistry().GetComponent<TransformComponent>(entity);

		if (tag.Tag == "Block")
		{
			auto spriteRenderer = scene.GetRegistry().GetComponent<SpriteRendererComponent>(entity);
			
			glm::vec2 position = blockAligner.GetBlockPosition(m_TexRatio);
			glm::vec2 scale = blockAligner.GetBlockScale(m_TexRatio);
			
			transform->Translation = { position, 0.0f };
			transform->Scale = { scale, 0.0f };
		}
		else if (tag.Tag == "AirBlock")
		{
			glm::vec2 position = blockAligner.GetBlockPosition(m_TexRatio);
			glm::vec2 scale = blockAligner.GetBlockScale(m_TexRatio);

			transform->Translation = { position, 0.0f };
			transform->Scale = { scale, 0.0f };
		}
	});
}

void GameLevel::OnEnd(Scene& scene)
{
}