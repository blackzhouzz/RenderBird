#include "component_typelist.h"
#include "transform.h"
#include "component.h"

namespace Runtime
{
	ComponentTypeList::ComponentTypeList()
	{
	}

	size_t ComponentTypeList::GetComponentTypeCount()const
	{
		return m_compTypeList.size();
	}

	void ComponentTypeList::RegisterAllComponentType()
	{
		for (auto itr = GlobalTypeList::Instance().begin(); itr != GlobalTypeList::Instance().end(); ++itr)
		{
			TypeInfo* typeInfo = itr->second;
			if (typeInfo->HasFlag(TypeInfoFlag::IsComponent))
			{
				m_compTypeList[typeInfo->GetTypeId()] = typeInfo;
			}
		}
	}

	TypeInfo* ComponentTypeList::GetComponentTypeInfo(int typeId)const
	{
		std::map<int, TypeInfo*>::const_iterator itr = m_compTypeList.find(typeId);
		if (itr != m_compTypeList.end())
			return itr->second;
		return nullptr;
	}
}