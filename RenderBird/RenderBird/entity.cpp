#include "entity.h"

namespace RenderBird
{
	IMPLEMENT_TYPEINFO(Entity, Object)

	Component* Entity::AddComponent(TypeInfo* type)
	{
		Component* comp = (Component*)type->Alloc();
		m_components.push_back(comp);
		return comp;
	}

	Component* Entity::GetComponent(TypeInfo* type)
	{
		for (int i = 0; i < m_components.size(); ++i)
		{
			if (m_components[i]->GetTypeInfo() == type)
			{
				return m_components[i];
			}
		}
		return nullptr;
	}

	void Entity::RemoveComponent(Component* comp)
	{

	}

	size_t Entity::GetComponentCount()const
	{
		return m_components.size();
	}

	Component* Entity::GetComponentAt(uint index)
	{
		assert(index < m_components.size());
		return m_components[index];
	}
}