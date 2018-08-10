#pragma once
#include "core_private.h"
#include "entity.h"
#include "pool.h"
#include "component_typelist.h"
#include "archetype.h"
#include "component.h"
#include "component_chunk.h"
#include <list>
#include <algorithm>

namespace Core
{
	class EntityManager : public Singleton<EntityManager>
	{
	public:
		EntityManager();
		~EntityManager();
		Archetype* CreateArchetype(int count, ...);
		template<typename ... Types>
		Archetype* CreateArchetype()
		{
			std::list<ComponentTypeId> typeIdList;
			CreateArchetypeImpl<0, Types...>(typeIdList);
			typeIdList.sort();
			typeIdList.erase(std::unique(typeIdList.begin(), typeIdList.end()), typeIdList.end());
			return GetOrCreateArchetype(typeIdList);
		}
		Archetype* CreateEmptyArchetype();
		EntityData* GetEntityData(EntityId entity)const;
		EntityId CreateEntity(Archetype* archetype);
		void DestroyEntity(EntityId entity);
		void* AddComponent(EntityId entity, TypeInfo* typeinfo);
		template<typename T>
		T* AddComponent(EntityId entity);
		bool RemoveComponent(EntityId entity, TypeInfo* typeinfo);
		template<typename T>
		bool RemoveComponent(EntityId entity);
		void* GetComponent(EntityId entity, TypeInfo* typeinfo);
		template<typename T>
		T* GetComponent(EntityId entity);
		std::vector<Archetype*>::const_iterator ArchetypesBegin()const { return m_archetypes.begin(); }
		std::vector<Archetype*>::const_iterator ArchetypesEnd()const { return m_archetypes.end(); }
	private:
		Archetype* FindArchetype(const std::list<ComponentTypeId>& typeIdList);
		Archetype* GetOrCreateArchetype(const std::list<ComponentTypeId>& typeIdList);
		template <int N, typename C>
		void CreateArchetypeImpl(std::list<ComponentTypeId>& typeIdList)
		{
			ComponentTypeId compTypeId;
			compTypeId.m_typeInfo = TypeOf<C>::Value();
			typeIdList.push_back(compTypeId);
		}
		template <int N, typename C0, typename C1, typename ... Cn>
		void CreateArchetypeImpl(std::list<ComponentTypeId>& typeIdList)
		{
			ComponentTypeId compTypeId;
			compTypeId.m_typeInfo = TypeOf<C0>::Value();
			typeIdList.push_back(compTypeId);
			CreateArchetypeImpl<N + 1, C1, Cn...>(typeIdList);
		}
		ComponentChunk* SetNewArchetype(EntityId entityId, EntityData* entityData, Archetype* destArchetype);
	private:
		PODPool<EntityData> m_entityPool;
		std::vector<Archetype*> m_archetypes;
	};

	template<typename T>
	inline T* EntityManager::AddComponent(EntityId entity)
	{
		return static_cast<T*>(AddComponent(entity, TypeOf<T>::Value()));
	}

	template<typename T>
	inline bool EntityManager::RemoveComponent(EntityId entity)
	{
		return RemoveComponent(entity, TypeOf<T>::Value());
	}

	template<typename T>
	inline T* EntityManager::GetComponent(EntityId entity)
	{
		return static_cast<T*>(GetComponent(entity, TypeOf<T>::Value()));
	}
}