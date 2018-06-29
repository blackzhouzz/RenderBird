#include "entitymanager.h"

namespace RenderBird
{
	EntityManager::EntityManager()
	{
		ComponentTypeList::GetInstance()->RegisterAllComponentType();
		for (int i = 0; i < CompTypeListCount; ++i)
		{
			TypeInfo* typeInfo = ComponentTypeList::GetInstance()->GetComponentTypeAt(i);
			m_compPools[i] = new Pool(typeInfo->GetTypeSize());
			m_compPools[i]->Reserve(4096);
		}
	}
	
	EntityManager::~EntityManager()
	{
	}

	EntityHandle EntityManager::CreateEntity()
	{
		EntityHandle handle = m_entities.New();
		handle.second->m_id = handle.first;
		return handle;
	}

	void EntityManager::DestroyEntity(EntityHandle& handle)
	{
		m_entities.Delete(handle);
	}

	ComponentHandle EntityManager::AddComponent(const EntityHandle& handle, int compTypeId)
	{
		ComponentHandle ret;
		if (m_entities.IsValidId(handle.first))
		{
			EntityData* entityData = m_entities.GetObject(handle.first);
			Pool* compPool = m_compPools[compTypeId];
			int compId = entityData->m_compIds[compTypeId];
			TypeInfo* typeInfo = ComponentTypeList::GetInstance()->GetComponentTypeAt(compTypeId);
			
			if (compPool->IsValidId(compId))
			{
				ret.m_id = compId;
				ret.m_data = compPool->Get(compId);
			}
			else
			{
				auto compDataHandle = compPool->Malloc();
				typeInfo->PlacementNew(compDataHandle.second);
				entityData->m_compIds[compTypeId] = compDataHandle.first;
				ret.m_id = compDataHandle.first;
				ret.m_data = compDataHandle.second;
				
			}
		}
		return ret;
	}

	void EntityManager::RemoveComponent(const EntityHandle& handle, int compTypeId)
	{
		if (m_entities.IsValidId(handle.first))
		{
			EntityData* entityData = m_entities.GetObject(handle.first);
			Pool* compPool = m_compPools[compTypeId];
			int compId = entityData->m_compIds[compTypeId];
			TypeInfo* typeInfo = ComponentTypeList::GetInstance()->GetComponentTypeAt(compTypeId);
			if (compPool->IsValidId(compId))
			{
				void* data = compPool->Get(compId);
				typeInfo->PlacementDelete(data);
				compPool->Free(compId);
			}
		}
	}
}