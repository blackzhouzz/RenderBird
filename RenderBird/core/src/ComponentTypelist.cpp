#include "ComponentTypelist.h"
#include "Transform.h"
#include "Component.h"

namespace Core
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

	TypeInfo* ComponentTypeList::GetComponentTypeInfo(size_t typeId)const
	{
		std::map<size_t, TypeInfo*>::const_iterator itr = m_compTypeList.find(typeId);
		if (itr != m_compTypeList.end())
			return itr->second;
		return nullptr;
	}
}