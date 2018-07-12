#pragma once
#include "entity.h"
#include "pool.h"
#include "ecs/componentregister.h"

namespace Runtime
{
	class EntityManager : public Singleton<EntityManager>
	{
		friend class Entity;
	public:
		EntityManager();
		~EntityManager();
		Entity* CreateEntity();
		void DestroyEntity(int entityId);
		void DestroyEntity(Entity* entity);
		bool HasEntity(int entityId)const;
		Entity* GetEntity(int entityId);
		template<typename Filter = DefaultFilter<Entity>>
		Pool::PoolIterator<Entity> EachEntities();
		template<typename T, typename Filter = DefaultFilter<T>>
		Pool::PoolIterator<T, Filter> EachComponents();
	private:
		std::map<int, ComponentPool*> m_pools;
		ObjectPool<Entity> m_entityPool;
	};

	template<typename Filter>
	Pool::PoolIterator<Entity> EntityManager::EachEntities()
	{
		return m_entityPool.EachObjects<Filter>();
	}

	template<typename T, typename Filter>
	Pool::PoolIterator<T, Filter> EntityManager::EachComponents()
	{
		return m_pools[TypeId<T>::value]->EachComponents<T, Filter>();
	}
}