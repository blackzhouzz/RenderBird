#include "math/vector.h"
#include "core/coreprivate.h"
#include "core/typeinfo.h"
#include "runtime/object.h"
#include "runtime/component.h"
#include "raytracing/typeinforegister.h"
#include "raytracing/trianglemesh.h"
#include "core/pool.h"
#include "tests/test_benchmark_entity_component.h"
#include "runtime/entitymanager.h"
#include "runtime/transform.h"
#include "math/vector.h"
#include "core/fastdelegate.hpp"
#include <iostream>
#include<stdarg.h>
using namespace MathLib;
using namespace RenderBird;
using namespace fastdelegate;

class A
{
};

class B
{
	virtual void Print() {}
	int m_i;
};

template<typename T>
struct Base
{
	static int i;
	static TypeInfo* m_typeInfo;
public:
	static TypeInfo* ThisTypeInfo()
	{
		//static_assert(std::is_pod<T>::value, "component type must be pod");
		if (m_typeInfo == nullptr)
		{
			m_typeInfo = new TypeInfo(typeid(T).name(), nullptr, new TAllocator<T>(), CompTypeList::type_index<T>::value, sizeof(T));
		}
		return m_typeInfo;
	}
};

template<typename T> TypeInfo* Base<T>::m_typeInfo = nullptr;

template< typename T > int Base< T >::i = 1;

class D1 : public Base<D1>
{

};

class D2 : public Base<D2>
{
};


class TestComp
{
public:
	TestComp(float floatData, int intData)
		: m_floatData(floatData)
		, m_intData(intData)
	{

	}
	~TestComp()
	{
		std::cout << m_floatData;
	}
	float m_floatData;
	int m_intData;
};

void TestObjectPool()
{
	ObjectPool<TestComp> objectPool;
	ObjectPool<TestComp>::ObjectHandle objHandle = objectPool.New(1.0f, 2);
	objHandle = objHandle;
	//objectPool.Delete(objHandle);
}

struct ITestAllocator
{
public:
	virtual void PlacementNew(void* data) = 0;
};

template<typename T>
struct TestAllocator : public ITestAllocator
{
	virtual void PlacementNew(void* data)
	{
		::new(data) T();
	}
};

struct MyFilter
{
	bool operator()(const Transform& t)
	{
		return t.m_position == Vector3f(0, 10, 0);
	}
};
typedef FastDelegate< void() > MyDelegate;


int main()
{
	//Test_IteratorComponent();
	//Test_BenchmarkEntitySlow();
	//Test_BenchmarkEntityFast();
	Test_BenchmarkEntity();
	system("pause");
	return 0;
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
	//RegisterAllTypeInfo();
	//GetTypeInfoFromTypeMap(TYPE_ID(Object));
	auto entity1 = EntityManager::IntancePtr()->CreateEntity();
	auto comp1 =EntityManager::IntancePtr()->AddComponent<Transform>(entity1);
	auto entity2 = EntityManager::IntancePtr()->CreateEntity();
	auto comp2 = EntityManager::IntancePtr()->AddComponent<Transform>(entity2);
	comp2 = EntityManager::IntancePtr()->AddComponent<Transform>(entity2);
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

	Transform* trans1 = comp1.Get<Transform>();
	trans1->m_position = Vector3f(10, 10, 0);

	Transform* trans2 = comp2.Get<Transform>();
	trans2->m_position = Vector3f(0, 10, 0);
	//for (auto x : EntityManager::IntancePtr()->EachComponents<Transform, MyFilter>())
	//{
	//	printf("a");
	//}

	auto entity = EntityManager::IntancePtr()->GetEntity(entity1);

	for (auto x : *entity)
	{
		auto comp = x.Get<Transform>();
		printf("a");
	}

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