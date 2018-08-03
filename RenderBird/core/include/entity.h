#pragma once
#include "core_private.h"
#include "component.h"
#include "event.h"

namespace Core
{
	struct EntityId
	{
		EntityId() = default;
		bool operator == (const EntityId& entity)const 
		{
			return this->m_id == entity.m_id;
		}
		bool operator != (const EntityId& entity)const
		{
			return this->m_id != entity.m_id;
		}
		bool operator < (const EntityId& entity)const
		{
			return this->m_id < entity.m_id;
		}
		bool operator == (size_t id)const
		{
			return this->m_id == id;
		}
		size_t m_id;

		const static EntityId INVALID;
	};

	struct EntityCreatedEvent : public Event<EntityCreatedEvent>
	{
		explicit EntityCreatedEvent(EntityId entity)
			: m_entity(entity) {}

		EntityId m_entity;
	};

	struct EntityDestroyedEvent : public Event<EntityDestroyedEvent>
	{
		explicit EntityDestroyedEvent(EntityId entity)
			: m_entity(entity) {}

		EntityId m_entity;
	};

}