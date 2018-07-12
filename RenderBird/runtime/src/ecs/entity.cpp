#include "ecs/entity.h"
#include "ecs/entitymanager.h"

namespace Runtime
{
	Entity::Entity()
		: m_id(INVALID_DATA_ID)
	{
	}

	int Entity::GetComponentId(int compTypeId)const
	{
		std::map<int, int>::const_iterator itr = m_compIds.find(compTypeId);
		if (itr != m_compIds.end())
			return itr->second;
		return INVALID_DATA_ID;
	}

	size_t Entity::GetComponentCount()const
	{
		return m_compIds.size();
	}

	Entity::ComponentIterator Entity::begin()
	{
		return m_compIds.begin();
	}

	Entity::ComponentIterator Entity::end()
	{
		return m_compIds.end();
	}

	int Entity::GetId()const
	{
		return m_id;
	}

	ComponentHandle Entity::AddComponent(int compTypeId)
	{
		EntityManager& inst = EntityManager::Instance();
		ComponentHandle ret = GetComponent(compTypeId);
		if (!ret.IsValid())
		{
			ret = inst.m_pools[compTypeId]->NewComponent();
			ret.Get<ComponentBase>()->m_ownerId = m_id;
			m_compIds[compTypeId] = ret.m_id;
		}
		return ret;
	}

	bool Entity::RemoveComponent(int compTypeId)
	{
		EntityManager& inst = EntityManager::Instance();
		Entity::ComponentIterator itr = m_compIds.find(compTypeId);
		if (itr != m_compIds.end())
		{
			if (inst.m_pools[compTypeId]->DeleteComponent(itr->second))
			{
				m_compIds.erase(itr);
				return true;
			}
		}
		return false;
	}

	void Entity::RemoveAllComponent()
	{
		while (m_compIds.size() > 0)
		{
			RemoveComponent(m_compIds.begin()->first);
		}
	}

	ComponentHandle Entity::GetComponent(int compTypeId)
	{
		EntityManager& inst = EntityManager::Instance();
		ComponentHandle ret;
		int compId = INVALID_DATA_ID;
		if (FindComponentId(compTypeId, compId))
		{
			ret = inst.m_pools[compTypeId]->GetComponent(compId);
		}
		return ret;
	}

	bool Entity::FindComponentId(int compTypeId, int& outCompId)
	{
		Entity::ComponentIterator itr = m_compIds.find(compTypeId);
		if (itr != m_compIds.end())
		{
			outCompId = itr->second;
			return true;
		}
		return false;
	}
}