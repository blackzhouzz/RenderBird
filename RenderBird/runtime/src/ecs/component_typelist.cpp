#include "ecs/component_typelist.h"
#include "transform.h"
#include "ecs/component.h"

namespace Runtime
{
	ComponentTypeList::ComponentTypeList()
	{
	}

	void ComponentTypeList::RegisterAllComponentType()
	{
		TypeInfo* baseType = TypeOf<ComponentBase>::StaticTypeInfo();
		for (TypeMapIterator itr = GlobalTypeList::Instance().begin(); itr != GlobalTypeList::Instance().end(); ++itr)
		{
			TypeInfo* typeInfo = itr->second;
			if (typeInfo->IsSubclassOf(baseType) && typeInfo != baseType)
			{
				m_compTypeList[typeInfo->GetTypeId()] = typeInfo;
			}
		}
	}

}