#pragma once
#include "runtime_private.h"
#include "typelist.h"
#include "typeinfo.h"

namespace Runtime
{
	class ComponentTypeList : public Singleton<ComponentTypeList>
	{
	public:
		using Iterator = std::map<int, TypeInfo*>::iterator;
		ComponentTypeList();
		void RegisterAllComponentType();
		size_t GetComponentTypeCount()const
		{
			return m_compTypeList.size();
		}
		TypeInfo* GetComponentTypeInfo(int typeId)const
		{
			std::map<int, TypeInfo*>::const_iterator itr = m_compTypeList.find(typeId);
			if (itr != m_compTypeList.end())
				return itr->second;
			return nullptr;
		}
		Iterator begin() { return m_compTypeList.begin(); }
		Iterator end() { return m_compTypeList.end(); }
	private:
		std::map<int, TypeInfo*> m_compTypeList;
	};

}
