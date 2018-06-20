#pragma once
#include "object.h"
#include "component.h"
#include <vector>

namespace RenderBird
{
	class Entity : Object
	{
		DECLARE_TYPEINFO(Entity, Object)
	public:
		Component* AddComponent(TypeInfo* type);
		Component* GetComponent(TypeInfo* type);
		void RemoveComponent(Component* comp);
		size_t GetComponentCount()const;
		Component* GetComponentAt(uint index);
	private:
		std::vector<Component*> m_components;
	};
}