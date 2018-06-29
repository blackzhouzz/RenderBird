#pragma once

#include "ecsprivate.h"
#include "core/typelist.h"
#include "tests/testcomponent.h"

namespace RenderBird
{
	typedef TypeList<
		struct Transform, 
		struct TestComponent
	> CompTypeList;

	const int CompTypeListCount = length<CompTypeList>::value;

	template<int I>
	struct CompTypeAt
	{
		typedef type_at<I, CompTypeList> type;
	};

	template<typename T>
	struct CompTypeId
	{
		enum 
		{
			value = type_id<T, CompTypeList>::value
		};
	};

	class ComponentTypeList : public TSingleton<ComponentTypeList>
	{
	public:
		void RegisterAllComponentType()
		{
			RegisterComponentType<Transform>();
			RegisterComponentType<TestComponent>();
		}
		int GetComponentTypeCount()const
		{
			return CompTypeListCount;
		}
		TypeInfo* GetComponentTypeAt(int typeId)const
		{
			return m_compTypeList[typeId];
		}
	private:
		template<typename T>
		void RegisterComponentType()
		{
			int typeId = type_id<T, CompTypeList>::value;
			m_compTypeList[typeId] = type_at<type_id<T, CompTypeList>::value, CompTypeList>::type::ThisTypeInfo();
		}
		TypeInfo* m_compTypeList[CompTypeListCount];
	};
}
