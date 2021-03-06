#include "Entitymanager.h"
#include "event.h"
#include <cstdarg>
#include <algorithm>

namespace Core
{
	EntityManager::EntityManager()
	{
		ComponentTypeList::IntancePtr()->RegisterAllComponentType();
	}
	
	EntityManager::~EntityManager()
	{
	}

	Archetype* EntityManager::CreateArchetype(int count, ...)
	{
		std::list<ComponentTypeId> typeIdList;

		va_list args;
		va_start(args, count);
		while (count--)
		{
			auto arg = va_arg(args, TypeInfo*);
			ComponentTypeId compTypeId;
			compTypeId.m_typeInfo = arg;
			typeIdList.push_back(compTypeId);
		}
		va_end(args);

		typeIdList.sort();
		typeIdList.erase(std::unique(typeIdList.begin(), typeIdList.end()), typeIdList.end());

		return GetOrCreateArchetype(typeIdList);
	}

	Archetype* EntityManager::CreateEmptyArchetype()
	{
		std::list<ComponentTypeId> typeIdList;
		return GetOrCreateArchetype(typeIdList);
	}

	Archetype* EntityManager::GetOrCreateArchetype(const std::list<ComponentTypeId>& typeIdList)
	{
		Archetype* ret = FindArchetype(typeIdList);
		if (ret == nullptr)
		{
			ret = new Archetype(typeIdList);
			m_archetypes.push_back(ret);
		}
		return ret;
	}

	Archetype* EntityManager::FindArchetype(const std::list<ComponentTypeId>& typeIdList)
	{
		for (auto archetype : m_archetypes)
		{
			if (archetype->IsEqual(typeIdList))
			{
				return archetype;
			}
		}
		return nullptr;
	}

	EntityData* EntityManager::GetEntityData(EntityId entity)const
	{
		return m_entityPool.Get(entity.m_id);
	}

	EntityId EntityManager::CreateEntity(Archetype* archetype)
	{
		EntityId id;
		auto handle = m_entityPool.Malloc();
		id = EntityId{ handle.first };

		ComponentChunk* chunk = archetype->GetOrCreateChunk();
		size_t chunkIndex = chunk->Allocate(id);
		handle.second->m_chunk = chunk;
		handle.second->m_chunkIndex = chunkIndex;
		chunk->SetAllComponentDefault(chunkIndex);
		EventManager::IntancePtr()->emit<EntityCreatedEvent>(id);
		return id;
	}

	void EntityManager::DestroyEntity(EntityId entity)
	{
		EntityData* entityData = m_entityPool.Get(entity.m_id);
		entityData->m_chunk->Deallocate(entityData->m_chunkIndex);
		m_entityPool.Free(entity.m_id);
		EventManager::IntancePtr()->emit<EntityDestroyedEvent>(entity);
	}

	ComponentChunk* EntityManager::SetNewArchetype(EntityId entityId, EntityData* entityData, Archetype* destArchetype)
	{
		ComponentChunk* srcChunk = entityData->m_chunk;
		Archetype* srcArchetype = srcChunk->GetArchetype();
		if (srcArchetype->IsEqual(destArchetype))
		{
			return srcChunk;
		}
		size_t srcChunkIndex = entityData->m_chunkIndex;

		ComponentChunk* destChunk = destArchetype->GetOrCreateChunk();
		size_t destChunkIndex = destChunk->Allocate(entityId);
		entityData->m_chunk = destChunk;
		entityData->m_chunkIndex = destChunkIndex;

		uint8* srcData = srcChunk->GetComponentData();
		uint8* destData = destChunk->GetComponentData();

		size_t srcOffset = 0;
		for (auto srcTypeId : *srcArchetype)
		{
			size_t srcTypeSize = srcTypeId.m_typeInfo->GetTypeSize();
			size_t destOffset = 0;
			for (auto destTypeId : *destArchetype)
			{
				size_t destTypeSize = destTypeId.m_typeInfo->GetTypeSize();
				if (srcTypeId.m_typeInfo->GetTypeId() == destTypeId.m_typeInfo->GetTypeId())
				{
					uint8* srcCompData = srcData + srcOffset * srcChunk->GetCapacity() + srcTypeSize * srcChunkIndex;
					uint8* destCompData = destData + destOffset * destChunk->GetCapacity() + destTypeSize * destChunkIndex;
					memcpy(destCompData, srcCompData, destTypeSize);
					break;
				}
				destOffset += destTypeSize;
			}
			srcOffset += srcTypeSize;
		}

		srcChunk->Deallocate(srcChunkIndex);

		return destChunk;
	}

	void* EntityManager::AddComponent(EntityId entity, TypeInfo* typeinfo)
	{
		EntityData* entityData = m_entityPool.Get(entity.m_id);
		ComponentChunk* chunk = entityData->m_chunk;
		Archetype* srcArchetype = chunk->GetArchetype();
		if (srcArchetype->HasComponentTypeId(typeinfo))
		{
			return chunk->GetEntityComponentPtr(entityData->m_chunkIndex, typeinfo);
		}

		std::list<ComponentTypeId> typeIdList = srcArchetype->GetComponentTypeList();
		typeIdList.push_back(ComponentTypeId(typeinfo));
		typeIdList.sort();

		Archetype* destArchetype = GetOrCreateArchetype(typeIdList);
		chunk = SetNewArchetype(entity, entityData, destArchetype);

		return chunk->SetComponentDefault(entityData->m_chunkIndex, typeinfo);
	}

	bool EntityManager::RemoveComponent(EntityId entity, TypeInfo* typeinfo)
	{
		EntityData* entityData = m_entityPool.Get(entity.m_id);
		Archetype* srcArchetype = entityData->m_chunk->GetArchetype();
		if (!srcArchetype->HasComponentTypeId(typeinfo))
		{
			return false;
		}
		std::list<ComponentTypeId> typeIdList = srcArchetype->GetComponentTypeList();
		typeIdList.erase(std::remove(typeIdList.begin(), typeIdList.end(), ComponentTypeId(typeinfo)), typeIdList.end());

		Archetype* destArchetype = GetOrCreateArchetype(typeIdList);
		SetNewArchetype(entity, entityData, destArchetype);
		return true;
	}

	void* EntityManager::GetComponent(EntityId entity, TypeInfo* typeinfo)
	{
		EntityData* entityData = m_entityPool.Get(entity.m_id);
		ComponentChunk* chunk = entityData->m_chunk;
		return chunk->GetEntityComponentPtr(entityData->m_chunkIndex, typeinfo);
	}

}