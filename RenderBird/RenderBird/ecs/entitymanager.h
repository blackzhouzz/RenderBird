#pragma once
#include "entity.h"
#include "core/pool.h"
#include "ecs/componentregister.h"

namespace RenderBird
{
	class EntityManager : public TSingleton<EntityManager>
	{
	public:
		EntityManager();
		~EntityManager();
		EntityHandle CreateEntity();
		void DestroyEntity(EntityHandle& handle);
		ComponentHandle AddComponent(const EntityHandle& handle, int compTypeId);
		template<typename T>
		ComponentHandle AddComponent(const EntityHandle& handle);
		void RemoveComponent(const EntityHandle& handle, int compTypeId);
		template<typename T>
		void RemoveComponent(const EntityHandle& handle);
	private:
		Pool* m_compPools[CompTypeListCount];
		ObjectPool<EntityData> m_entities;
	};

	template<typename T>
	ComponentHandle EntityManager::AddComponent(const EntityHandle& handle)
	{
		int compTypeId = CompTypeId<T>::value;
		return AddComponent(handle, compTypeId);
	}

	template<typename T>
	void EntityManager::RemoveComponent(const EntityHandle& handle)
	{
		int compTypeId = CompTypeId<T>::value;
		RemoveComponent(handle, compTypeId);
	}
}