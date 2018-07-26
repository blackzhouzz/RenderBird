#pragma once
#include "core_private.h"
#include <map>

enum TypeInfoFlag
{
	Default = 0,
	IsComponent = 1 << 0,
};

template<typename T, bool is_abstract>
struct TAllocator
{
	static void* New()
	{
		return nullptr;
	}
	static void Delete(void* data)
	{
	}
	static void PlacementNew(void* data)
	{
	}
	static void PlacementDelete(void* data)
	{
	}
};

template<typename T>
struct TAllocator<T, false>
{
	static void* New()
	{
		T* ret = new T();
		return (void*)ret;
	}
	static void Delete(void* data)
	{
		delete static_cast<T*>(data);
	}
	static void PlacementNew(void* data)
	{
		::new(data) T();
	}
	static void PlacementDelete(void* data)
	{
		static_cast<T*>(data)->~T();
	}
};

class TypeInfo
{
public:
	virtual void* New()const = 0;
	virtual void Delete(void* data)const = 0;
	virtual void PlacementNew(void* data)const = 0;
	virtual void PlacementDelete(void* data)const = 0;
	virtual const std::string& GetName()const = 0;
	virtual TypeInfo* GetSuperTypeInfo()const = 0;
	virtual size_t GetTypeId()const = 0;
	virtual size_t GetTypeSize()const = 0;
	virtual const void* GetDefaultValue()const = 0;
	virtual bool IsSubclassOf(const TypeInfo* baseClass)const = 0;
	virtual bool IsSubclassOf(const std::string& className)const = 0;
	virtual bool HasFlag(TypeInfoFlag flag)const = 0;
	virtual bool IsAbtract()const = 0;
};

template<typename T>
class TTypeInfo : public TypeInfo
{
public:
	using Allocator = TAllocator<T, std::is_abstract<T>::value>;
	explicit TTypeInfo(std::string name, TypeInfo* superTypeInfo, size_t flags)
		: m_name(name)
		, m_superTypeInfo(superTypeInfo)
		, m_flags(flags)
	{
	}
	virtual void* New()const { return Allocator::New();}
	virtual void Delete(void* data)const { return Allocator::Delete(data); }
	virtual void PlacementNew(void* data)const { return Allocator::PlacementNew(data); }
	virtual void PlacementDelete(void* data)const { return Allocator::PlacementDelete(data); }
	virtual const std::string& GetName()const { return m_name; }
	virtual TypeInfo* GetSuperTypeInfo()const { return m_superTypeInfo; }
	virtual size_t GetTypeId()const { return TypeId<T>::value; }
	virtual size_t GetTypeSize()const { return sizeof(T); }
	virtual const void* GetDefaultValue()const { return DefaultOf<T>::Value(); }
	virtual bool IsSubclassOf(const TypeInfo* baseClass)const
	{
		const TypeInfo* typeInfo = this;
		while (typeInfo)
		{
			if (typeInfo->GetTypeId() == baseClass->GetTypeId())
			{
				return true;
			}
			typeInfo = typeInfo->GetSuperTypeInfo();
		}
		return false;
	}
	virtual bool IsSubclassOf(const std::string& className)const
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
	virtual bool HasFlag(TypeInfoFlag flag)const { return (m_flags & flag) != 0; }
	virtual bool IsAbtract()const { return std::is_abstract<T>::value; }
private:
	std::string m_name;
	TypeInfo* m_superTypeInfo;
	size_t m_flags;
};

using TypeMapIterator = std::map<size_t, TypeInfo*>::iterator;

class GlobalTypeList : public Core::Singleton<GlobalTypeList>
{
public:
	using TypeInfoMap = std::map<size_t, TypeInfo*>;

	void RegisterTypeInfo(TypeInfo* typeInfo);
	TypeInfo* GetTypeInfo(size_t typeId);
	TypeMapIterator begin();
	TypeMapIterator end();
private:
	TypeInfoMap m_typeMap;
};

#define REGISTER_TYPEINFO(cls) \
	{ \
		extern void RegisterTypeInfo##cls(); \
		RegisterTypeInfo##cls(); \
	}

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
struct TypeId
{
	static const int value = 0;
};

template<typename T>
struct TypeOf
{
	static TypeInfo* Value() { return nullptr; }
};

template<typename T>
struct DefaultOf
{
	static const T* Value() { return nullptr; }
};

#define DEFAULT_BEGIN(Module, Type)\
template<>\
inline const Module::Type* DefaultOf<Module::Type>::Value()\
{\
	static Module::Type value = {

#define DEFAULT_DATA(Data) Data,

#define DEFAULT_END()\
	};\
	return &value;\
}

#define DECLEAR_TYPE_BASE(Module, Type)\
	template<>\
	struct TypeOf<Module::Type>\
	{\
		static TypeInfo* Value()\
		{\
			static TypeInfo* typeInfo = nullptr;\
			if (typeInfo == nullptr)\
			{\
				typeInfo = new TTypeInfo<Module::Type>(#Type, nullptr, TypeInfoFlag::Default);\
			}\
			return typeInfo;\
		}\
	};

#define DECLEAR_TYPE(Module, Type, SuperType)\
	template<>\
	struct TypeOf<Module::Type>\
	{\
		using Super = SuperType;\
		static TypeInfo* Value()\
		{\
			static TypeInfo* typeInfo = nullptr;\
			if (typeInfo == nullptr)\
			{\
				typeInfo = new TTypeInfo<Module::Type>(#Type, TypeOf<Super>::Value(), TypeInfoFlag::Default);\
			}\
			return typeInfo;\
		}\
	};

#define IMPLEMENT_TYPE(Module, Type)\
	void RegisterTypeInfo##Type()\
	{\
		GlobalTypeList::IntancePtr()->RegisterTypeInfo(TypeOf<Module::Type>::Value());\
	}
