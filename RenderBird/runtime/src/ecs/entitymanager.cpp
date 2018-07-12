#include "ecs/entitymanager.h"
#include "event.h"

namespace Runtime
{
	EntityManager::EntityManager()
	{
		ComponentTypeList::IntancePtr()->RegisterAllComponentType();
		for (auto typeInfo: ComponentTypeList::Instance())
		{
			m_pools[typeInfo.first] = new ComponentPool(typeInfo.second);
		}
	}
	
	EntityManager::~EntityManager()
	{
		for (std::map<int, ComponentPool*>::iterator itr = m_pools.begin(); itr != m_pools.end(); ++itr)
		{
			delete itr->second;
		}
	}

	Entity* EntityManager::CreateEntity()
	{
		EntityHandle handle = m_entityPool.New();
		handle.second->m_id = handle.first;
		EventManager::IntancePtr()->emit<EntityCreatedEvent>(handle.second);
		return handle.second;
	}

	void EntityManager::DestroyEntity(int entityId)
	{
		if (m_entityPool.IsValidId(entityId))
		{
			Entity* entity = m_entityPool.GetObject(entityId);
			entity->RemoveAllComponent();
			EventManager::IntancePtr()->emit<EntityDestroyedEvent>(entity);
			m_entityPool.Delete(entityId);
		}
	}

	void EntityManager::DestroyEntity(Entity* entity)
	{
		if (m_entityPool.IsValidId(entity->m_id))
		{
			entity->RemoveAllComponent();
			EventManager::IntancePtr()->emit<EntityDestroyedEvent>(entity);
			m_entityPool.Delete(entity->m_id);
		}
	}

	bool EntityManager::HasEntity(int entityId)const
	{
		return m_entityPool.IsValidId(entityId);
	}

	Entity* EntityManager::GetEntity(int entityId)
	{
		if (m_entityPool.IsValidId(entityId))
		{
			return m_entityPool.GetObject(entityId);
		}
		return nullptr;
	}

}