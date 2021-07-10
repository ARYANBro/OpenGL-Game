#include "Entity.h"


Entity::Entity(std::uint_fast32_t entityID, Scene* scene) noexcept
	: m_EntityID(entityID), m_Scene(scene)
{
}