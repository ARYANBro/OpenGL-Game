#pragma once

#include <cstdint>

class Entity;

class ComponentPool
{
public:
	ComponentPool(std::size_t componentSize);
	~ComponentPool() noexcept;

	void Remove(std::size_t index) noexcept;
	
	template<typename ComponentType>
	ComponentType* Add(std::size_t index) noexcept;

	void* Get(std::size_t index) noexcept;

	std::size_t GetComponentSize() const noexcept { return m_ComponentSize; }
	std::size_t GetLength() const noexcept { return m_Length; }

private:
	char* m_Components;
	std::size_t m_ComponentSize;
	std::size_t m_Length = 0;
};

template<typename ComponentType>
ComponentType* ComponentPool::Add(std::size_t index) noexcept
{
	m_Length++;
	return new(Get(index)) ComponentType();
}