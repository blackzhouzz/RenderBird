#include "stdafx.h"
#include <time.h>

#include "test_benchmark_entity_component.h"
#include <iostream>
#include<stdarg.h>
#include "geometry.h"
#include "test_raytracing.h"
//
//using namespace RenderBird;
//
//#include "runtime_register.h"
//#include "renderbird_register.h"

//
//void RegisterAllTypeInfo()
//{
//	RegisterTypeInfoRuntime();
//	RegisterTypeInfoRenderBird();
//}


//template<>
//struct TestType1<Runtime::Transform>
//{
//	static TypeInfo* Get()
//	{
//		static TypeInfo* typeInfo = nullptr;
//		if (m_typeInfo == nullptr)
//		{
//			m_typeInfo = new TypeInfo("Transform", nullptr, new TAllocator<Transform>(), Runtime::TYPE_ID(Transform), sizeof(Runtime::Transform));\
//		}
//		return m_typeInfo;
//	}
//	static TypeInfo* m_typeInfo;
//};
//#include <ctime>
int main()
{
	TestRayTracingApp app;
	app.Init();
	app.Run();

	return 0;
	//RegisterAllTypeInfo();
	////Test_IteratorComponent();
	//Test_BenchmarkEntitySlow();
	//Test_BenchmarkEntityFast();
	////Test_BenchmarkEntity();
	//return 0;
	//static_assert(std::is_pod<Quaternion>::value, "component type must be pod");
	//Transform t1;
	//TestFilter<Transform, MyFilter> test;
	//test.print(t1);
	//ITestAllocator* test = new TestAllocator<int>();
	//A a1;
	//a1.m_i = 2;
	//a1.m_str = "3131";
	//A* pA = new A(a1);
	//TestObjectPool();
	//Object* pobj = new TriangleMesh();
	////Pool<int>* pool = new Pool<int>();
	//GetTypeInfoFromTypeMap(TYPE_ID(Object));
	//auto entity1 = EntityManager::IntancePtr()->CreateEntity();
	//auto comp1 = EntityManager::IntancePtr()->AddComponent<Transform>(entity1);
	//auto entity2 = EntityManager::IntancePtr()->CreateEntity();
	//auto comp2 = EntityManager::IntancePtr()->AddComponent<Transform>(entity2);
	//comp2 = EntityManager::IntancePtr()->AddComponent<Transform>(entity2);
	//EntityManager::IntancePtr()->DestroyEntity(entity1);
	//void* data = EntityManager::IntancePtr()->m_entityPool.Begin();
	//while (data != nullptr)
	//{
	//	Entity* entity = (Entity*)(data);
	//	data = EntityManager::IntancePtr()->m_entityPool.Next();
	//}
	//
	//for (auto x : EntityManager::IntancePtr()->EachEntities())
	//{
	//	printf("a");
	//}
	//for (auto x : EntityManager::IntancePtr()->EachComponents<Transform, MyFilter>())
	//{
	//	printf("a");
	//}

	//auto entity = EntityManager::IntancePtr()->GetEntity(entity1);

	//for (auto x : *entity)
	//{
	//	auto comp = x.Get<Transform>();
	//	printf("a");
	//}

	//auto trans = EntityManager::IntancePtr()->AddComponent<TestComponent>(entity);
	//TestComponent* pTrans = trans.Get<TestComponent>();
	//pTrans->m_str = "789";
	//EntityManager::IntancePtr()->RemoveComponent<TestComponent>(entity);

	//ComponentGroup<int, float>::Test();

	//ComponentTypeList::IntancePtr()->RegisterAllComponentType();
	//auto a1 = Transform::ThisTypeInfo();
	//auto a2 = TestComponent::ThisTypeInfo();
	system("pause");
	return 0;
}