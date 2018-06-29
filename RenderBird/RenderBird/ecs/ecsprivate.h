#pragma once
#include "core/coreprivate.h"
#include "core/typeinfo.h"


//#define DECLARE_COMPONENT_TYPEINFO(Type)\
//private:\
//	static TypeInfo* m_typeInfo;\
//public:\
//	static TypeInfo* ThisTypeInfo()\
//	{\
//		if (m_typeInfo == nullptr)\
//		{\
//			m_typeInfo = new TypeInfo(typeid(Type).name(), nullptr, &TAllocator<Type>::Alloc, 0, sizeof(Type));\
//		}\
//		return m_typeInfo;\
//	}
//
//#define IMPLEMENT_COMPONENT_TYPEINFO(Type)\
