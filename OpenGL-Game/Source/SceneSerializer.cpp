#include "SceneSerializer.h"

#include "Scene.h"
#include "Entity.h"
#include "Game.h"

#include <yaml-cpp/yaml.h>

#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

static YAML::Emitter& operator<<(YAML::Emitter& emitter, glm::vec3 color) noexcept
{
	emitter << YAML::Flow
		<< YAML::BeginSeq << color.x << color.y << color.z << YAML::EndSeq;
	
	return emitter;
}

static void DeserializeEntity(const YAML::Node& entityNode, Entity entity, Scene& scene)
{
	if (auto spriteRendererNode = entityNode["SpriteRendererComponent"]; spriteRendererNode)
	{
		auto spriteComponent = entity.AddComponent<SpriteRendererComponent>();

		spriteComponent->Color = spriteRendererNode["Color"].as<glm::vec3>();

		std::string texturepath = spriteRendererNode["Texturepath"].as<std::string>();
		spriteComponent->Texture = scene.GetGame()->GetTextureLibrary().Load(texturepath);
	}

	if (auto transformNode = entityNode["TransformComponent"]; transformNode)
	{
		auto transformComponent = entity.AddComponent<TransformComponent>();

		transformComponent->Translation = transformNode["Translation"].as<glm::vec3>();
		transformComponent->Scale = transformNode["Scale"].as<glm::vec3>();
		transformComponent->Rotation = transformNode["Rotation"].as<glm::vec3>();
	}
}

static void SerializeEntityData(YAML::Emitter& emitter, EntityID entity, const SceneRegistry& registry)
{
	emitter << YAML::Key << "EntityID" << YAML::Value << entity;

	if (registry.HasComponent<SpriteRendererComponent>(entity))
	{
		emitter << YAML::Key << "SpriteRendererComponent"
			<< YAML::Value << YAML::BeginMap;

		auto component = registry.GetComponent<SpriteRendererComponent>(entity);

		emitter << YAML::Key << "Texturepath" << YAML::Value << component->Texture->GetTexturePath();
		emitter << YAML::Key << "Color" << YAML::Value << component->Color;

		emitter << YAML::EndMap;
	}
	
	if (registry.HasComponent<TransformComponent>(entity))
	{
		emitter << YAML::Key << "TransformComponent"
			<< YAML::Value << YAML::BeginMap;

		auto component = registry.GetComponent<TransformComponent>(entity);

		emitter << YAML::Key << "Translation" << YAML::Value << component->Translation;
		emitter << YAML::Key << "Scale" << YAML::Value << component->Scale;
		emitter << YAML::Key << "Rotation" << YAML::Value << component->Rotation;

		emitter << YAML::EndMap;
	}
}

void SceneSerializer::Serialize(const Scene& scene, const std::string& filepath)
{
	std::ofstream ofile;
	ofile.exceptions(std::ios::failbit | std::ios::badbit);
	ofile.open(filepath);

	YAML::Emitter emitter(ofile);

	emitter << YAML::BeginSeq;

	for (auto entityData : scene.m_Registry.GetEntityData())
	{
		emitter << YAML::BeginMap;

		SerializeEntityData(emitter, entityData.EntityID, scene.m_Registry);

		emitter << YAML::EndMap;
	}

	emitter << YAML::EndSeq;
}

void SceneSerializer::Deserialize(Scene& scene, const std::string& filepath)
{
	YAML::Node node = YAML::LoadFile(filepath);

	if (!node.IsSequence())
		throw std::runtime_error("Root node not a sequence");

	for (const auto& entityNode : node)
	{
		DeserializeEntity(entityNode, scene.CreateEntity(), scene);
	}
}
