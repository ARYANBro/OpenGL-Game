#include "ComponentPool.h"

#include "Scene.h"

#include <cassert>
#include <cstring>

#define MAX_COMPONENTS 32

ComponentPool::ComponentPool(std::size_t componentSize)
	: m_ComponentSize(componentSize)
{
	assert(componentSize > 0);
	m_Components = operator new(componentSize * MAX_COMPONENTS);
}

ComponentPool::~ComponentPool() noexcept
{
	operator delete(m_Components, m_ComponentSize * MAX_COMPONENTS);
}

void* ComponentPool::Get(std::size_t index) noexcept
{
	assert(index < m_Length);
	return (reinterpret_cast<char*>(m_Components) + (index * m_ComponentSize));
}

bool ComponentPool::Contains(std::size_t id) noexcept
{
	char zero[m_ComponentSize] = { 0 };
	bool cmp = std::memcmp(Get(id), &zero, m_ComponentSize);
	return id < m_Length && cmp;
}

void ComponentPool::Remove(std::size_t index) noexcept
{
	std::memset(Get(index), 0, m_ComponentSize);
	m_Length--;
}