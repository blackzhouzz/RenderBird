#pragma once
#include "core_private.h"
#include <map>

enum TypeInfoFlag
{
	IsAbstract = 1 << 0,
	Default = 0,
};

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
	TypeInfo(std::string name, TypeInfo* superTypeInfo, IAllocator* alloc, int typeId, size_t typeSize, size_t flags)
		: m_name(name)
		, m_superTypeInfo(superTypeInfo)
		, m_alloc(alloc)
		, m_typeId(typeId)
		, m_typeSize(typeSize)
		, m_flags(flags)
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
	size_t GetTypeSize()const { return m_typeSize; }
	bool IsSubclassOf(const TypeInfo* baseClass)const
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
	bool IsSubclassOf(const std::string& className)const
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
	bool IsAbstract()const { return m_flags & TypeInfoFlag::IsAbstract; }
private:
	std::string m_name;
	TypeInfo* m_superTypeInfo;
	IAllocator* m_alloc;
	int m_typeId;
	size_t m_typeSize;
	size_t m_flags;
};

//#define DECLARE_TYPEINFO_COMMON(Type)\
//private:\
//	static TypeInfo* m_typeInfo;\
//public:\
//	static TypeInfo* ThisTypeInfo();
//
//#define DECLARE_TYPEINFO_BASE(Type)\
//	DECLARE_TYPEINFO_COMMON(Type)
//
//#define DECLARE_TYPEINFO(Type, SuperType)\
//	DECLARE_TYPEINFO_COMMON(Type)\
//public:\
//	typedef SuperType Super;
//
//#define IMPLEMENT_TYPEINFO_COMMON(Type)\
//	TypeInfo* Type::m_typeInfo = nullptr;\
//	void RegisterTypeInfo##Type()\
//	{\
//		GlobalTypeList::RegisterTypeInfo(Type::ThisTypeInfo());\
//	}
//
//#define IMPLEMENT_TYPEINFO_BASE(Type)\
//	IMPLEMENT_TYPEINFO_COMMON(Type)\
//	TypeInfo* Type::ThisTypeInfo()\
//	{\
//		if (m_typeInfo == nullptr)\
//		{\
//			m_typeInfo = new TypeInfo(#Type, nullptr, new TAllocator<Type>(), TYPE_ID(Type), sizeof(Type), TypeInfoFlag::Default);\
//		}\
//		return m_typeInfo;\
//	}
//
//#define IMPLEMENT_TYPEINFO(Type, SuperType)\
//	IMPLEMENT_TYPEINFO_COMMON(Type)\
//	TypeInfo* Type::ThisTypeInfo()\
//	{\
//		if (m_typeInfo == nullptr)\
//		{\
//			m_typeInfo = new TypeInfo(#Type, SuperType::ThisTypeInfo(), new TAllocator<Type>(), TYPE_ID(Type), sizeof(Type), TypeInfoFlag::Default);\
//		}\
//		return m_typeInfo;\
//	}
//
//#define IMPLEMENT_TYPEINFO_ABSTRACT(Type, SuperType)\
//	IMPLEMENT_TYPEINFO_COMMON(Type)\
//	TypeInfo* Type::ThisTypeInfo()\
//	{\
//		if (m_typeInfo == nullptr)\
//		{\
//			m_typeInfo = new TypeInfo(#Type, SuperType::ThisTypeInfo(), nullptr, TYPE_ID(Type), sizeof(Type), TypeInfoFlag::IsAbstract);\
//		}\
//		return m_typeInfo;\
//	}

#define REGISTER_TYPEINFO(cls) \
	{ \
		extern void RegisterTypeInfo##cls(); \
		RegisterTypeInfo##cls(); \
	}

using TypeMapIterator = std::map<int, TypeInfo*>::iterator;

class GlobalTypeList : public Core::Singleton<GlobalTypeList>
{
public:
	using TypeInfoMap = std::map<int, TypeInfo*>;

	void RegisterTypeInfo(TypeInfo* typeInfo);
	TypeInfo* GetTypeInfo(int typeId);
	TypeMapIterator begin();
	TypeMapIterator end();
private:
	TypeInfoMap m_typeMap;
};

#ifndef TYPE_ID_GROUP
#define TYPE_ID_GROUP(name, id) static const int TYPE_ID_GROUP_##name = id;
#endif

#ifndef TYPE_ID_CLASS
#define	TYPE_ID_CLASS(cls, group, id)\
	namespace group\
	{\
		class cls;\
	}\
	template <>\
	struct TypeId<group::cls>\
	{\
		static const int value = id + TYPE_ID_GROUP_##group;\
	};
#endif

#ifndef TYPE_ID_STRUCT
#define	TYPE_ID_STRUCT(cls, group, id)\
	namespace group\
	{\
		struct cls;\
	}\
	template <>\
	struct TypeId<group::cls>\
	{\
		static const int value = id + TYPE_ID_GROUP_##group;\
	};
#endif

template <typename T>
struct TypeId;

template<typename T>
struct TypeOf;

#define DECLEAR_TYPE_BASE(Module, Type)\
	template<>\
	struct TypeOf<Module::Type>\
	{\
		static TypeInfo* StaticTypeInfo()\
		{\
			static TypeInfo* typeInfo = nullptr;\
			if (typeInfo == nullptr)\
			{\
				typeInfo = new TypeInfo(#Type, nullptr, nullptr, TypeId<Module::Type>::value, sizeof(Module::Type), TypeInfoFlag::Default);\
			}\
			return typeInfo;\
		}\
	};

#define DECLEAR_TYPE(Module, Type, SuperType)\
	template<>\
	struct TypeOf<Module::Type>\
	{\
		using Super = SuperType;\
		static TypeInfo* StaticTypeInfo()\
		{\
			static TypeInfo* typeInfo = nullptr;\
			if (typeInfo == nullptr)\
			{\
				typeInfo = new TypeInfo(#Type, TypeOf<Super>::StaticTypeInfo(), new TAllocator<Module::Type>(), TypeId<Module::Type>::value, sizeof(Module::Type), TypeInfoFlag::Default);\
			}\
			return typeInfo;\
		}\
	};

#define DECLEAR_TYPE_ABSTRACT(Module, Type, SuperType)\
	template<>\
	struct TypeOf<Module::Type>\
	{\
		using Super = SuperType;\
		static TypeInfo* StaticTypeInfo()\
		{\
			static TypeInfo* typeInfo = nullptr;\
			if (typeInfo == nullptr)\
			{\
				typeInfo = new TypeInfo(#Type, TypeOf<Super>::StaticTypeInfo(), nullptr, TypeId<Module::Type>::value, sizeof(Module::Type), TypeInfoFlag::IsAbstract);\
			}\
			return typeInfo;\
		}\
	};

#define IMPLEMENT_TYPE(Module, Type)\
	void RegisterTypeInfo##Type()\
	{\
		GlobalTypeList::IntancePtr()->RegisterTypeInfo(TypeOf<Module::Type>::StaticTypeInfo());\
	}
