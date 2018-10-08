#pragma once
#include "CorePrivate.h"
#include <map>
#include "TypeInfo.h"

namespace Core
{
	class ComponentTypeList : public Singleton<ComponentTypeList>
	{
	public:
		using Iterator = std::map<size_t, TypeInfo*>::iterator;
		ComponentTypeList();
		void RegisterAllComponentType();
		size_t GetComponentTypeCount()const;
		TypeInfo* GetComponentTypeInfo(size_t typeId)const;
		Iterator begin() { return m_compTypeList.begin(); }
		Iterator end() { return m_compTypeList.end(); }
	private:
		std::map<size_t, TypeInfo*> m_compTypeList;
	};
}
