#pragma once
#include "runtime_private.h"
#include "pool.h"
using namespace Core;

namespace Runtime
{
	struct ComponentBase
	{
		size_t m_ownerId;
	};

//	template<typename T>
//	class Component : public ComponentBase
//	{
//	public:
//		static TypeInfo* StaticTypeInfo()
//		{
//			static TypeInfo* typeInfo = nullptr;
//			if (typeInfo == nullptr)
//			{
//				typeInfo = new TypeInfo(typeid(T).name(), TypeOf<ComponentBase>::StaticTypeInfo(), new TAllocator<T>(), TypeId<T>::value, sizeof(T), TypeInfoFlag::Default);
//			}
//			return typeInfo;
//		}
//
//		static void RegisterTypeInfo()
//		{
//			GlobalTypeList::IntancePtr()->RegisterTypeInfo(StaticTypeInfo());
//		}
//	};
//
//
//#ifndef REGISTER_TYPEINFO_COMPONENT
//#define REGISTER_TYPEINFO_COMPONENT(group, cls) group::cls::RegisterTypeInfo();
//#endif

	struct ComponentTypeId
	{
		ComponentTypeId()
			: m_typeInfo(nullptr)
		{
		}
		ComponentTypeId(TypeInfo* typeInfo)
			: m_typeInfo(typeInfo)
		{
		}
		bool operator == (const ComponentTypeId& val)const
		{
			return m_typeInfo->GetTypeId() == val.m_typeInfo->GetTypeId();
		}
		bool operator != (const ComponentTypeId& val)const
		{
			return m_typeInfo->GetTypeId() != val.m_typeInfo->GetTypeId();
		}
		bool operator < (const ComponentTypeId& val)const
		{
			return m_typeInfo->GetTypeId() < val.m_typeInfo->GetTypeId();
		}
		bool operator > (const ComponentTypeId& val)const
		{
			return m_typeInfo->GetTypeId() > val.m_typeInfo->GetTypeId();
		}
		TypeInfo* m_typeInfo;
	};
}

DECLEAR_TYPE_BASE(Runtime, ComponentBase)