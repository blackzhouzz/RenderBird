#pragma once
#include "coreprivate.h"

namespace RenderBird
{
	class Object;
	typedef Object* (*ObjectAllocator)();

	template<typename T>
	struct TAllocator
	{
		static Object* Alloc()
		{
			T* ret = new T();
			return (Object*)ret;
		}
	};

	class TypeInfo
	{
	public:
		TypeInfo(std::string name, TypeInfo* superTypeInfo, ObjectAllocator alloc)
			: m_name(name)
			, m_superTypeInfo(superTypeInfo)
			, m_alloc(alloc)
		{
		}

		Object* Alloc()const
		{
			if (m_alloc != nullptr)
				return m_alloc();
			return nullptr;
		}
		const std::string& GetName()const { return m_name; }
		TypeInfo* GetSuperTypeInfo()const { return m_superTypeInfo; }
		bool IsDrivenFrom(const TypeInfo* baseClass)const
		{
			const TypeInfo* typeInfo = this;
			while (typeInfo)
			{
				if (typeInfo == baseClass)
				{
					return true;
				}
				typeInfo = typeInfo->GetSuperTypeInfo();
			}
			return false;
		}
		bool IsDrivedFrom(const std::string& className)const
		{
			const TypeInfo* typeInfo = this;
			while (typeInfo)
			{
				if (typeInfo->GetName() == className)
				{
					return true;
				}
				typeInfo = typeInfo->GetSuperTypeInfo();
			}
			return false;
		}
	private:
		std::string m_name;
		TypeInfo* m_superTypeInfo;
		ObjectAllocator m_alloc;
	};

#define DECLARE_TYPEINFO_COMMON(ClassType)\
private:\
	static TypeInfo* m_typeInfo;\
public:\
	static TypeInfo* ThisTypeInfo();\
	virtual TypeInfo* GetTypeInfo()const { return ThisTypeInfo(); }\
	

#define DECLARE_TYPEINFO_BASE(ClassType)\
	DECLARE_TYPEINFO_COMMON(ClassType)

#define DECLARE_TYPEINFO(ClassType, SuperClassType)\
	DECLARE_TYPEINFO_COMMON(ClassType)\
public:\
	typedef SuperClassType SuperTypeInfo;

#define IMPLEMENT_TYPEINFO_BASE(ClassType)\
	TypeInfo* ClassType::m_typeInfo = nullptr;\
	TypeInfo* ClassType::ThisTypeInfo()\
	{\
		if (m_typeInfo == nullptr)\
		{\
			m_typeInfo = new TypeInfo(#ClassType, nullptr, nullptr);\
		}\
		return m_typeInfo;\
	}

#define IMPLEMENT_TYPEINFO(ClassType, SuperClassType)\
	TypeInfo* ClassType::m_typeInfo = nullptr;\
	TypeInfo* ClassType::ThisTypeInfo()\
	{\
		if (m_typeInfo == nullptr)\
		{\
			m_typeInfo = new TypeInfo(#ClassType, SuperClassType::ThisTypeInfo(), &TAllocator<ClassType>::Alloc);\
		}\
		return m_typeInfo;\
	}
}