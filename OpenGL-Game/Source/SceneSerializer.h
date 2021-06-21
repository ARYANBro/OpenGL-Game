#pragma once

#include <string>

class Scene;

class SceneSerializer
{
public:
	static void Serialize(const Scene& scene, const std::string& filepath);
	static void Deserialize(Scene& scene, const std::string& filepath);
};
