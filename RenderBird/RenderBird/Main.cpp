#include "math/vector.h"
#include "core/coreprivate.h"
#include "core/typeinfo.h"
#include "runtime/object.h"
#include "ecs/component.h"
#include "raytracing/typeinforegister.h"
#include "raytracing/trianglemesh.h"
#include "core/pool.h"
#include "tests/testcomponent.h"
#include "ecs/entitymanager.h"
#include <iostream>
#include<stdarg.h>
using namespace MathLib;
using namespace RenderBird;


class A
{
};

class B
{
	virtual void Print() {}
	int m_i;
};

#define TEST_TYPE(Type)\
	void Print()\
	{\
		std::cout << typeid(Type).name();\
	}


template<typename T>
struct Base
{
	static int i;
};

template< typename T > int Base< T >::i = 1;

struct D1 : public Base<D1>
{

};

struct D2 : public Base<D2>
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

int main()
{
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
	auto entity = EntityManager::GetInstance()->CreateEntity();
	auto trans = EntityManager::GetInstance()->AddComponent<TestComponent>(entity);
	TestComponent* pTrans = trans.Get<TestComponent>();
	pTrans->m_str = "789";
	EntityManager::GetInstance()->RemoveComponent<TestComponent>(entity);
	auto a1 = Transform::ThisTypeInfo();
	auto a2 = TestComponent::ThisTypeInfo();
	system("pause");
	return 0;
}