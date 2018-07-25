#pragma once
#include "runtime_private.h"
#include "component.h"
#include "event.h"

namespace Runtime
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
		EntityId& operator = (size_t id)
		{
			this->m_id = id;
			return *this;
		}
		EntityId& operator = (const EntityId& entity)
		{
			this->m_id = entity.m_id;
			return *this;
		}
		size_t m_id;
	};

	struct EntityData
	{
		size_t m_chunkIndex;
		class Chunk* m_chunk;
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