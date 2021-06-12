#include "ComponentPool.h"

#include "Scene.h"

#include <cassert>

#define MAX_COMPONENTS 32

ComponentPool::ComponentPool(std::size_t componentSize)
	: m_ComponentSize(componentSize)
{
	assert(componentSize > 0);
	m_Components = new char[componentSize * MAX_COMPONENTS];
}

ComponentPool::~ComponentPool() noexcept
{
	delete[] m_Components;
}

void ComponentPool::Remove(std::size_t index) noexcept
{
	m_Length--;
	operator delete(Get(index), m_ComponentSize);
}

void* ComponentPool::Get(std::size_t index) noexcept
{
	return &m_Components[index * m_ComponentSize];
}