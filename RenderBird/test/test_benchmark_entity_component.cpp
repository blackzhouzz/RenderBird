#include "stdafx.h"
#include "test_benchmark_entity_component.h"
#include <list>
#include <unordered_set>
#include <set>
#include "component_group.h"
#include "timer.h"
using namespace std;


struct EntityCreatedEventReceiver
	: public Receiver<EntityCreatedEventReceiver> {
	void receive(const EntityCreatedEvent &event) {
		//printf("created\n");
	}
};

struct EntityDestroyedEventReceiver
	: public Receiver<EntityDestroyedEventReceiver> {
	void receive(const EntityDestroyedEvent &event) {
		//printf("destroyed\n");
	}
};
static const int MAX_ENTITY_COUNT = 4096*1000;

void Test_BenchmarkEntitySlow()
{
	static int entities[MAX_ENTITY_COUNT];

	IntervalTime time;

	std::list<Transform> trans;
	time.Begin();
	std::cout << "slow insert entity benchmark ms " << time.End() << std::endl;
	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		trans.push_back(Transform());
	}
	std::cout << "slow create component benchmark ms: " << time.End() << std::endl;

	time.Begin();
	for (std::list<Transform>::iterator itr = trans.begin(); itr != trans.end(); ++itr)
	{
		itr->m_position = Vector3f::ZERO;
		//itr->m_rotation = Quaternion(0, 1, 0, 1);
		//itr->m_needUpdate = false;
		//itr->m_scale = Vector3f(0, 1, 0);
	}
	std::cout << "slow iterator component benchmark ms: " << time.End() << std::endl;
}

void Test_BenchmarkEntityFastest()
{
	//EntityCreatedEventReceiver createdReceiver;
	//EventManager::IntancePtr()->subscribe<EntityCreatedEvent>(createdReceiver);

	//EntityDestroyedEventReceiver destroyedReceiver;
	//EventManager::IntancePtr()->subscribe<EntityDestroyedEvent>(destroyedReceiver);

	static EntityId entities[MAX_ENTITY_COUNT];
	static Transform* components[MAX_ENTITY_COUNT];
	Archetype* archtype = EntityManager::IntancePtr()->CreateArchetype(1, TypeOf<Transform>::StaticTypeInfo());

	IntervalTime time;

	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		entities[i] = EntityManager::IntancePtr()->CreateEntity(archtype);
	}
	std::cout << "fastest create entity benchmark ms: " << time.End() << std::endl;

	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		components[i] = EntityManager::IntancePtr()->AddComponent<Transform>(entities[i]);
	}
	std::cout << "fastest create component benchmark ms: " << time.End() << std::endl;

	ComponentGroup group(1, TypeOf<Transform>::StaticTypeInfo());
	ComponentArray* arr = group.GetComponentArray<Transform>();
	time.Begin();

	//ComponentVisitor* vis = arr->m_visitors[0];

	//int count = vis->GetCount();
	//for (int i = 0; i < count; ++i)
	//{
	//	Transform* t = vis->GetDataAt<Transform>(i);
	//	t->m_position = Vector3f::ZERO;
	//}

	ComponentArray::Visitor<Transform> visitor(arr);
	for (auto x : visitor)
	{
		Transform* t = x;
		t->m_position = Vector3f::ZERO;
		//t->m_rotation = Quaternion(0, 1, 0, 1);
		//t->m_needUpdate = false;
		//t->m_scale = Vector3f(0, 1, 0);
	}
	std::cout << "fastest iterator component benchmark ms: " << time.End() << std::endl;
}

void Test_IteratorComponent()
{
	Archetype* archtype = EntityManager::IntancePtr()->CreateArchetype(1, TypeOf<Transform>::StaticTypeInfo());
	static const int testEntityCount = 10;

	static EntityId test_entities[testEntityCount];
	static Transform* test_components[testEntityCount];

	for (int i = 0; i < testEntityCount; ++i)
	{
		test_entities[i] = EntityManager::IntancePtr()->CreateEntity(archtype);
	}

	for (int i = 0; i < testEntityCount; ++i)
	{
		test_components[i] = EntityManager::IntancePtr()->AddComponent<Transform>(test_entities[i]);

		auto comp = EntityManager::IntancePtr()->GetComponent<Transform>(test_entities[i]);
		comp->m_position = Vector3f(0, i, 0);
		Vector3f temp = EntityManager::IntancePtr()->GetComponent<Transform>(test_entities[i])->m_position;
		temp = temp;
	}
	EntityManager::IntancePtr()->DestroyEntity(test_entities[0]);
	for (int i = 0; i < testEntityCount; ++i)
	{
		auto tran = EntityManager::IntancePtr()->GetComponent<Transform>(test_entities[i]);
		if (tran != nullptr)
		{
			Vector3f temp = tran->m_position;
			temp = temp;
		}
		else
		{
			break;
		}

	}

	ComponentGroup group(1, TypeOf<Transform>::StaticTypeInfo());
	ComponentArray* arr = group.GetComponentArray<Transform>();
	ComponentArray::Visitor<Transform> visitor(arr);
	for (auto x : visitor)
	{
		Transform* t = (Transform*)x;
		t->m_position = t->m_position;
	}
}

