#include "stdafx.h"
#include "test_benchmark_entity_component.h"
#include <list>
#include <unordered_set>
#include <set>
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

void Test_BenchmarkEntity()
{
	static const int MAX_ENTITY_COUNT = 10000000;
	static int entities[MAX_ENTITY_COUNT];
	static ComponentHandle components[MAX_ENTITY_COUNT];
	static Entity* entitydata[MAX_ENTITY_COUNT];
	static set<int> entityids;
	IntervalTime time;

	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		entities[i] = EntityManager::IntancePtr()->CreateEntity()->GetId();
	}
	std::cout << "fast create entity benchmark ms: " << time.End() << std::endl;

	//for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	//{
	//	if (i != MAX_ENTITY_COUNT - 1)
	//	{
	//		EntityManager::IntancePtr()->DestroyEntity(entities[i]);
	//	}
	//}

	int index = 0;
	for (auto x : EntityManager::IntancePtr()->EachEntities())
	{
		entityids.insert(x.GetId());
	}
	time.Begin();
	for (set<int>::iterator itr = entityids.begin(); itr != entityids.end(); ++itr)
	{
		entities[index++] = *itr;
	}
	std::cout << "set iterator entity benchmark ms: " << time.End() << std::endl;

	std::vector<int> vecData(entityids.begin(), entityids.end());
	time.Begin();
	for (int i = 0; i < vecData.size(); ++i)
	{
		entities[i] = vecData[i];
	}
	std::cout << "vector iterator entity benchmark ms: " << time.End() << std::endl;

	time.Begin();
	index = 0;
	for (auto x : EntityManager::IntancePtr()->EachEntities())
	{
		entities[index++] = x.GetId();
	}
	std::cout << "common iterator entity benchmark ms: " << time.End() << std::endl;

}


void Test_BenchmarkEntityFast()
{
	//EntityCreatedEventReceiver createdReceiver;
	//EventManager::IntancePtr()->subscribe<EntityCreatedEvent>(createdReceiver);

	//EntityDestroyedEventReceiver destroyedReceiver;
	//EventManager::IntancePtr()->subscribe<EntityDestroyedEvent>(destroyedReceiver);

	static const int MAX_ENTITY_COUNT = 10000000;
	static Entity* entities[MAX_ENTITY_COUNT];
	static ComponentHandle components[MAX_ENTITY_COUNT];

	IntervalTime time;

	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		entities[i] = EntityManager::IntancePtr()->CreateEntity();
	}
	std::cout << "fast create entity benchmark ms: " << time.End() << std::endl;
	
	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		components[i] = entities[i]->AddComponent<Transform>();
	}
	std::cout << "fast create component benchmark ms: " << time.End() << std::endl;

	time.Begin();
	for (auto& x : EntityManager::IntancePtr()->EachComponents<Transform>())
	{
		x.m_position = Vector3f::ZERO;
		x.m_rotation = Quaternion(0, 1, 0, 1);
		x.m_needUpdate = false;
		x.m_scale = Vector3f(0, 1, 0);
	}
	std::cout << "fast iterator component benchmark ms: " << time.End() << std::endl;

}

void Test_BenchmarkEntitySlow()
{
	static const int MAX_ENTITY_COUNT = 10000000;
	static int entities[MAX_ENTITY_COUNT];
	static ComponentHandle components[MAX_ENTITY_COUNT];

	IntervalTime time;

	std::list<Entity> entityList;
	std::list<Transform> trans;
	time.Begin();
	for (int i = 0; i < MAX_ENTITY_COUNT; ++i)
	{
		entityList.push_back(Entity());
	}
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
		itr->m_rotation = Quaternion(0, 1, 0, 1);
		itr->m_needUpdate = false;
		itr->m_scale = Vector3f(0, 1, 0);
	}
	std::cout << "slow iterator component benchmark ms: " << time.End() << std::endl;
}

void Test_IteratorComponent()
{
	static const int testEntityCount = 10;

	static Entity* test_entities[testEntityCount];
	static ComponentHandle test_components[testEntityCount];

	for (int i = 0; i < testEntityCount; ++i)
	{
		test_entities[i] = EntityManager::IntancePtr()->CreateEntity();
	}

	for (int i = 0; i < testEntityCount; ++i)
	{
		test_components[i] = test_entities[i]->AddComponent<Transform>();
		auto comp = test_entities[i]->GetComponent<Transform>();
		comp.Get<Transform>()->m_position = Vector3f(0, 1, 0);
		Vector3f temp = test_components[i].Get<Transform>()->m_position;
		temp = temp;
	}

	for (auto& x : EntityManager::IntancePtr()->EachComponents<Transform>())
	{
		x.m_position = Vector3f(10, 0, 1);
	}

	for (auto& x : EntityManager::IntancePtr()->EachComponents<Transform>())
	{
		x.m_position = x.m_position;
	}

	for (int i = 0; i < testEntityCount; ++i)
	{
		EntityManager::IntancePtr()->DestroyEntity(test_entities[i]);
	}
}

