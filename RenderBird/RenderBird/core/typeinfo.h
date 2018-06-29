#pragma once
#include "coreprivate.h"
#include <map>

namespace RenderBird
{
	class IAllocator
	{
	public:
		virtual void* New() = 0;
		virtual void PlacementNew(void* data) = 0;
		virtual void PlacementDelete(void* data) = 0;
	};

	template<typename T>
	class TAllocator : public IAllocator
	{
		virtual void* New()
		{
			T* ret = new T();
			return (void*)ret;
		}
		virtual void PlacementNew(void* data)
		{
			::new(data) T();
		}
		virtual void PlacementDelete(void* data)
		{
			static_cast<T*>(data)->~T();
		}
	};

	class TypeInfo
	{
	public:
		TypeInfo(std::string name, TypeInfo* superTypeInfo, IAllocator* alloc, int typeId, int typeSize)
			: m_name(name)
			, m_superTypeInfo(superTypeInfo)
			, m_alloc(alloc)
			, m_typeId(typeId)
			, m_typeSize(typeSize)
		{
		}
		~TypeInfo()
		{
			if (m_alloc != nullptr)
			{
				delete m_alloc;
			}
		}
		void* NewObject()const
		{
			if (m_alloc != nullptr)
				return m_alloc->New();
			return nullptr;
		}
		void PlacementNew(void* data)
		{
			m_alloc->PlacementNew(data);
		}
		void PlacementDelete(void* data)
		{
			m_alloc->PlacementDelete(data);
		}
		const std::string& GetName()const { return m_name; }
		TypeInfo* GetSuperTypeInfo()const { return m_superTypeInfo; }
		int GetTypeId()const { return m_typeId; }
		int GetTypeSize()const { return m_typeSize; }
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
		IAllocator* m_alloc;
		int m_typeId;
		int m_typeSize;
	};

#define DECLARE_TYPEINFO_COMMON(Type)\
private:\
	static TypeInfo* m_typeInfo;\
public:\
	static TypeInfo* ThisTypeInfo();

#define DECLARE_TYPEINFO_BASE(Type)\
	DECLARE_TYPEINFO_COMMON(Type)

#define DECLARE_TYPEINFO(Type, SuperType)\
	DECLARE_TYPEINFO_COMMON(Type)\
public:\
	typedef SuperType Super;

#define IMPLEMENT_TYPEINFO_COMMON(Type)\
	TypeInfo* Type::m_typeInfo = nullptr;\
	void RegisterTypeInfo##Type()\
	{\
		extern void GlobalRegisterTypeInfo(TypeInfo* typeInfo);\
		GlobalRegisterTypeInfo(Type::ThisTypeInfo());\
	}

#define IMPLEMENT_TYPEINFO_BASE(Type)\
	IMPLEMENT_TYPEINFO_COMMON(Type)\
	TypeInfo* Type::ThisTypeInfo()\
	{\
		if (m_typeInfo == nullptr)\
		{\
			m_typeInfo = new TypeInfo(#Type, nullptr, new TAllocator<Type>(), TYPE_ID(Type), sizeof(Type));\
		}\
		return m_typeInfo;\
	}

#define IMPLEMENT_TYPEINFO(Type, SuperType)\
	IMPLEMENT_TYPEINFO_COMMON(Type)\
	TypeInfo* Type::ThisTypeInfo()\
	{\
		if (m_typeInfo == nullptr)\
		{\
			m_typeInfo = new TypeInfo(#Type, SuperType::ThisTypeInfo(), new TAllocator<Type>(), TYPE_ID(Type), sizeof(Type));\
		}\
		return m_typeInfo;\
	}

#define IMPLEMENT_TYPEINFO_ABSTRACT(Type, SuperType)\
	IMPLEMENT_TYPEINFO_COMMON(Type)\
	TypeInfo* Type::ThisTypeInfo()\
	{\
		if (m_typeInfo == nullptr)\
		{\
			m_typeInfo = new TypeInfo(#Type, SuperType::ThisTypeInfo(), nullptr, TYPE_ID(Type), sizeof(Type));\
		}\
		return m_typeInfo;\
	}
}