#pragma once
#include "core/typeinfo.h"
#include "ecsprivate.h"
#include "core/pool.h"

namespace RenderBird
{
	template<typename T>
	struct Component
	{
	private:
		static TypeInfo* m_typeInfo;
	public:
		static TypeInfo* ThisTypeInfo()
		{
			if (m_typeInfo == nullptr)
			{
				m_typeInfo = new TypeInfo(typeid(T).name(), nullptr, new TAllocator<T>(), type_id<T, CompTypeList>::value, sizeof(T));
			}
			return m_typeInfo;
		}
		typedef TAllocator<T> CompAllocator;
	};

	template<typename T> TypeInfo* Component<T>::m_typeInfo = nullptr;

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
		//template<typename T, typename ... Args>
		//T* New(Args && ... args)
		//{
		//	::new(m_data) T(std::forward<Args>(args) ...);
		//	return static_cast<T*>(m_data);
		//}
		//template<typename T>
		//void Delete()
		//{
		//	static_cast<T*>(m_data)->~T();
		//}
		int m_id;
		void* m_data;
	};
}