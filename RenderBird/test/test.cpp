#include "stdafx.h"
#include <time.h>

#include "test_benchmark_entity_component.h"
#include <iostream>
#include<stdarg.h>
#include "test_raytracing.h"
#include "FBXImportUtils.h"
#include "RenderStatistic.h"

//
//using namespace RenderBird;
//


void RegisterAllTypeInfo()
{
	//RegisterTypeInfoCore();
	RegisterTypeInfoRenderBird();
}


//template<>
//struct TestType1<Core::Transform>
//{
//	static TypeInfo* Get()
//	{
//		static TypeInfo* typeInfo = nullptr;
//		if (m_typeInfo == nullptr)
//		{
//			m_typeInfo = new TypeInfo("Transform", nullptr, new TAllocator<Transform>(), Core::TYPE_ID(Transform), sizeof(Core::Transform));\
//		}
//		return m_typeInfo;
//	}
//	static TypeInfo* m_typeInfo;
//};
//#include <ctime>

static void TestRaytracing()
{
	RenderStatistic::m_timer.Begin();
	TestRayTracingApp app;
	app.Init();
	app.Run();

	std::cout << "Ray trace time is " << RenderStatistic::m_timer.End() * 0.001f << std::endl;
}

struct A
{
	char c1;
	short s1;
	char c2;
	float f1;
	double d1;
};
struct B
{
	char c1;
	char c2;
	short s1;
	float f1;
	double d1;
};


static void TestBenchmark()
{
	RegisterAllTypeInfo();
	Test_BenchmarkEntitySlow();
	Test_BenchmarkEntityFastest();
	//Test_IteratorComponent();
}
#include "DistantLight.h"

int main()
{
	//FBXImportUtils::LoadFBX("c:/b.fbx");
	//std::cout << TEST::ccc;
	//static_assert(std::is_pod<int*>::value,"a");
	TestRaytracing();
	//TestBenchmark();
	return 0;
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
	////PoolBase<int>* pool = new PoolBase<int>();
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