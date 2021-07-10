#pragma once

#include <forward_list>
#include <memory>
#include <cassert>

template<typename ResourceType, typename ParamType>
class ResourceLibrary
{
public:
	ResourceType* Load(const ParamType& param);
	void Reset() noexcept;

	bool Compare(const ResourceType& resource, const ParamType& param);
	std::shared_ptr<ResourceType> CreateResource(const ParamType& param);

	static ResourceLibrary& Get() noexcept
	{
		static ResourceLibrary s_ResourceLib;
		return s_ResourceLib;
	}

private:
	std::forward_list<std::shared_ptr<ResourceType>> m_Resources;
};

template<typename ResourceType, typename ParamType>
ResourceType* ResourceLibrary<ResourceType, ParamType>::Load(const ParamType& param)
{
	for (auto& resource : m_Resources)
	{
		if (resource.use_count() != 0)
		{
			if (Compare(*resource, param))
				return resource.get();
		}
	}

	auto resource = CreateResource(param);
	m_Resources.push_front(resource);
	return resource.get();
}

template<typename ResourceType, typename ParamType>
void ResourceLibrary<ResourceType, ParamType>::Reset() noexcept
{
	for (auto& resource : m_Resources)
		resource.reset();
}