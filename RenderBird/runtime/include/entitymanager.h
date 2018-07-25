#pragma once
#include "runtime_private.h"
#include "entity.h"
#include "pool.h"
#include "component_typelist.h"
#include "archetype.h"
#include "chunk.h"

namespace Runtime
{
	class EntityManager : public Singleton<EntityManager>
	{
	public:
		EntityManager();
		~EntityManager();
		Archetype* CreateArchetype(int count, ...);
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
	private:
		PODPool<EntityData> m_entityPool;
		std::vector<Archetype*> m_archetypes;
	};

	template<typename T>
	inline T* EntityManager::AddComponent(EntityId entity)
	{
		return static_cast<T*>(AddComponent(entity, TypeOf<T>::StaticTypeInfo()));
	}

	template<typename T>
	inline bool EntityManager::RemoveComponent(EntityId entity)
	{
		return RemoveComponent(entity, TypeOf<T>::StaticTypeInfo());
	}

	template<typename T>
	inline T* EntityManager::GetComponent(EntityId entity)
	{
		return static_cast<T*>(GetComponent(entity, TypeOf<T>::StaticTypeInfo()));
	}
}