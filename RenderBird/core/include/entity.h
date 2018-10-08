#pragma once
#include "CorePrivate.h"
#include "Component.h"
#include "event.h"

namespace Core
{
	struct EntityData
	{
		size_t m_chunkIndex;
		class ComponentChunk* m_chunk;
	};

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