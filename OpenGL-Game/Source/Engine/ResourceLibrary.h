#pragma once

#include <forward_list>
#include <memory>

template<typename ResourceType, typename ParamType>
class ResourceLibrary
{
public:
	ResourceType* Load(const ParamType& filePath)
	{
		for (auto& resource : m_Resources)
		{
			if (resource.use_count() != 0)
			{
				if (Compare(*resource, filePath))
					return resource.get();
			}
		}

		auto resource = LoadResource(filePath);
		m_Resources.push_front(resource);
		return resource.get();
	}

private:
	std::forward_list<std::shared_ptr<ResourceType>> m_Resources;	

private:
	virtual std::shared_ptr<ResourceType> LoadResource(const std::string& filePath) = 0;
	virtual bool Compare(const ResourceType& resource, const std::string& filePath) = 0;
};