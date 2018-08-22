#pragma once
#include "core_private.h"
#include "pool.h"
#include "typeinfo.h"

namespace Core
{
#ifndef DECLEAR_TYPE_COMPONENT
#define DECLEAR_TYPE_COMPONENT(Module, Type)\
	template<>\
	struct TypeOf<Module::Type>\
	{\
		static TypeInfo* Value()\
		{\
			static_assert(std::is_trivially_copyable<Module::Type>::value, "Component type must be copyable!");\
			static TypeInfo* typeInfo = nullptr;\
			if (typeInfo == nullptr)\
			{\
				typeInfo = new TTypeInfo<Module::Type>(#Type, nullptr, TypeInfoFlag::IsComponent);\
			}\
			return typeInfo;\
		}\
	};
#endif

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
