#pragma once
#include "runtime_private.h"
#include "component.h"
#include "event.h"

namespace Runtime
{
	class Entity
	{
		friend class EntityManager;
	public:
		using ComponentIterator = std::map<int, int>::iterator;
		Entity();
		size_t GetComponentCount()const;
		ComponentIterator begin();
		ComponentIterator end();
		int GetId()const;
		template<typename T>
		int GetComponentId()const;
		int GetComponentId(int compTypeId)const;
		template<typename T>
		ComponentHandle AddComponent();
		ComponentHandle AddComponent(int compTypeId);
		template<typename T>
		bool RemoveComponent();
		bool RemoveComponent(int compTypeId);
		void RemoveAllComponent();
		template<typename T>
		ComponentHandle GetComponent();
		ComponentHandle GetComponent(int compTypeId);
	private:
		bool FindComponentId(int compTypeId, int& outCompId);
	private:
		int m_id;
		std::map<int, int> m_compIds;
	};

	template<typename T>
	int Entity::GetComponentId()const
	{
		return GetComponentId(TypeId<T>::value);
	}

	template<typename T>
	ComponentHandle Entity::AddComponent()
	{
		return AddComponent(TypeId<T>::value);
	}

	template<typename T>
	bool Entity::RemoveComponent()
	{
		return RemoveComponent(TypeId<T>::value);
	}

	template<typename T>
	ComponentHandle Entity::GetComponent()
	{
		return GetComponent(TypeId<T>::value);
	}

	using EntityHandle = ObjectPool<Entity>::ObjectHandle;

	struct EntityCreatedEvent : public Event<EntityCreatedEvent> 
	{
		explicit EntityCreatedEvent(Entity* entity) 
			: m_entity(entity) {}
		virtual ~EntityCreatedEvent(){}

		Entity* m_entity;
	};

	struct EntityDestroyedEvent : public Event<EntityDestroyedEvent> 
	{
		explicit EntityDestroyedEvent(Entity* entity)
			: m_entity(entity) {}
		virtual ~EntityDestroyedEvent(){}

		Entity* m_entity;
	};

}