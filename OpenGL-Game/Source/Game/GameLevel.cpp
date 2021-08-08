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

	BlockAligner blockAligner(15, 8, Application::GetInstance().GetWindow().GetWidth());

	for (int brickID : m_LevelLayout)
	{
		Entity block = scene.CreateEntity();
		auto tag = block.GetComponent<TagComponent>();

		auto transform = block.GetComponent<TransformComponent>();
		glm::vec2 position = blockAligner.GetBlockPosition(m_TexRatio);
		glm::vec2 scale = blockAligner.GetBlockScale(m_TexRatio);
	
		transform->Translation = { position, 0.0f };
		transform->Scale = { scale, 0.0f };
		switch (brickID)
		{
			case 0:
			{
				tag->Tag = "AirBlock";
				break;
			}
			case 1:
			{
				tag->Tag = "SolidBlock";
				auto spriteRenderer = block.AddComponent<SpriteRendererComponent>();
				spriteRenderer->Texture = m_SolidBlockTexture;

				auto physicsComponent = block.AddComponent<PhysicsComponent>();
				physicsComponent->Colider = std::make_unique<AABBColider>();
				physicsComponent->PhysicsType = PhysicsType::Static;
				physicsComponent->GetColider<AABBColider>().SetPosition(static_cast<glm::vec2>(position));
				physicsComponent->GetColider<AABBColider>().SetScale(static_cast<glm::vec2>(scale));
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

				auto physicsComponent = block.AddComponent<PhysicsComponent>();
				physicsComponent->Colider = std::make_unique<AABBColider>();
				physicsComponent->PhysicsType = PhysicsType::Static;
				physicsComponent->GetColider<AABBColider>().SetPosition(static_cast<glm::vec2>(position));
				physicsComponent->GetColider<AABBColider>().SetScale(static_cast<glm::vec2>(scale));
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
}

void GameLevel::OnEnd(Scene& scene)
{
	scene.GetRegistry().EachEntity([&](EntityID e)
	{
		Entity entity = scene.GetEntity(e);

		auto tag = entity.GetComponent<TagComponent>()->Tag;

		if (tag == "Block" || tag == "SolidBlock" || tag == "AirBlock")
			scene.DestroyEntity(entity);
	});
}

void GameLevel::Load(Scene& scene) noexcept
{
	BlockAligner blockAligner(15, 8, Application::GetInstance().GetWindow().GetWidth());

	int i = 0;
	scene.GetRegistry().EachEntity([&](EntityID entity)
	{
		Entity block = scene.GetEntity(entity);
		auto tag = block.GetComponent<TagComponent>();
		if (tag->Tag == "Block" || tag->Tag == "AirBlock" || tag->Tag == "SolidBlock")
		{
			int brickID = m_LevelLayout[i++];

			auto transform = block.GetComponent<TransformComponent>();
			switch (brickID)
			{
				case 0:
				{
					if (block.HasComponent<SpriteRendererComponent>())
						block.RemoveComponent<SpriteRendererComponent>();

					if (block.HasComponent<PhysicsComponent>())
						block.RemoveComponent<PhysicsComponent>();

					tag->Tag = "AirBlock";
					break;
				}
				case 1:
				{
					SpriteRendererComponent* spriteRenderer = block.GetComponent<SpriteRendererComponent>();

					if (!spriteRenderer)
						spriteRenderer = block.AddComponent<SpriteRendererComponent>();

					PhysicsComponent* physicsComponent = block.GetComponent<PhysicsComponent>();

					if (!physicsComponent)
					{
						physicsComponent = block.AddComponent<PhysicsComponent>();
						physicsComponent->Colider = std::make_unique<AABBColider>();
					}

					spriteRenderer->Texture = m_SolidBlockTexture;
					tag->Tag = "SolidBlock";

					physicsComponent->GetColider<AABBColider>().SetPosition(transform->Translation);
					physicsComponent->GetColider<AABBColider>().SetScale(transform->Scale);
					physicsComponent->PhysicsType = PhysicsType::Static;
					break;
				}
				case 2:
				case 3:
				case 4:
				case 5:
				{
					tag->Tag = "Block";
					SpriteRendererComponent* spriteRenderer = block.GetComponent<SpriteRendererComponent>();

					if (!spriteRenderer)
						spriteRenderer = block.AddComponent<SpriteRendererComponent>();

					spriteRenderer->Texture = m_BlockTexture;
					spriteRenderer->Color = m_ColorScheme[brickID];

					PhysicsComponent* physicsComponent = block.GetComponent<PhysicsComponent>();

					if (!physicsComponent)
					{
						physicsComponent = block.AddComponent<PhysicsComponent>();
						physicsComponent->Colider = std::make_unique<AABBColider>();
					}

					physicsComponent->PhysicsType = PhysicsType::Static;
					physicsComponent->GetColider<AABBColider>().SetPosition(transform->Translation);
					physicsComponent->GetColider<AABBColider>().SetScale(transform->Scale);
					break;
				}
				default:
					assert(false);
					break;
			}
		}
	});
}