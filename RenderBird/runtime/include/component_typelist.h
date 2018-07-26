#pragma once
#include "runtime_private.h"

namespace Runtime
{
	class ComponentTypeList : public Singleton<ComponentTypeList>
	{
	public:
		using Iterator = std::map<int, TypeInfo*>::iterator;
		ComponentTypeList();
		void RegisterAllComponentType();
		size_t GetComponentTypeCount()const;
		TypeInfo* GetComponentTypeInfo(int typeId)const;
		Iterator begin() { return m_compTypeList.begin(); }
		Iterator end() { return m_compTypeList.end(); }
	private:
		std::map<int, TypeInfo*> m_compTypeList;
	};
}
