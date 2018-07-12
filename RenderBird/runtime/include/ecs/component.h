#pragma once
#include "typeinfo.h"
#include "pool.h"
#include "typelist.h"
#include "runtime_register.h"
using namespace Core;

namespace Runtime
{
	class ComponentBase
	{
	public:
		ComponentBase()
			: m_ownerId(INVALID_DATA_ID)
		{
		}
		int m_ownerId;
	//protected:
	//	static size_t m_familyCounter;
	};

	template<typename T>
	class Component : public ComponentBase
	{
	public:
		static TypeInfo* StaticTypeInfo()
		{
			static TypeInfo* typeInfo = nullptr;
			if (typeInfo == nullptr)
			{
				typeInfo = new TypeInfo(typeid(T).name(), TypeOf<ComponentBase>::StaticTypeInfo(), new TAllocator<T>(), TypeId<T>::value, sizeof(T), TypeInfoFlag::Default);
			}
			return typeInfo;
		}

		static void RegisterTypeInfo()
		{
			GlobalTypeList::IntancePtr()->RegisterTypeInfo(StaticTypeInfo());
		}

		//static size_t FamilyId()
		//{
		//	static size_t index = m_familyCounter++;
		//	return index;
		//}
	};


#ifndef REGISTER_TYPEINFO_COMPONENT
#define REGISTER_TYPEINFO_COMPONENT(group, cls) group::cls::RegisterTypeInfo();
#endif

	struct ComponentHandle
	{
		ComponentHandle()
			: m_id(INVALID_DATA_ID)
			, m_data(nullptr)
		{
		}
		template<typename T>
		T* Get()
		{
			return static_cast<T*>(m_data);
		}
		bool IsValid()const
		{
			return m_id != INVALID_DATA_ID;
		}
		int m_id;
		void* m_data;
	};

	class ComponentPool
	{
	public:
		explicit ComponentPool(TypeInfo* typeInfo)
			: m_typeInfo(typeInfo)
		{
			m_pool = new Pool(typeInfo->GetTypeSize());
		}
		~ComponentPool()
		{
			delete m_pool;
		}
		ComponentHandle NewComponent()
		{
			ComponentHandle ret;
			auto compDataHandle = m_pool->Malloc();
			m_typeInfo->PlacementNew(compDataHandle.second);
			ret.m_id = compDataHandle.first;
			ret.m_data = compDataHandle.second;
			return ret;
		}
		bool DeleteComponent(int compId)
		{
			if (m_pool->IsValidId(compId))
			{
				void* data = m_pool->Get(compId);
				m_typeInfo->PlacementDelete(data);
				m_pool->Free(compId);
				return true;
			}
			return false;
		}
		ComponentHandle GetComponent(int compId)
		{
			ComponentHandle ret;
			ret.m_id = compId;
			ret.m_data = m_pool->Get(compId);
			return ret;
		}
		bool IsValidId(int compId)const
		{
			return m_pool->IsValidId(compId);
		}
		template<typename T, typename Filter = DefaultFilter<T>>
		Pool::PoolIterator<T, Filter> EachComponents()
		{
			return m_pool->Each<T, Filter>();
		}
	private:
		Pool* m_pool;
		TypeInfo* m_typeInfo;
	};

	template<typename T>
	class ComponentGroup
	{

	};

	template<typename T>
	class ComponentArray
	{

	};
}

DECLEAR_TYPE_BASE(Runtime, ComponentBase)