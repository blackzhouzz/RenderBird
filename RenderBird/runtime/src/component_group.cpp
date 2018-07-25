#include <cstdarg>
#include "component_group.h"
#include "entitymanager.h"

namespace Runtime
{
	ComponentVisitor::ComponentVisitor(Chunk* chunk, TypeInfo* compTypeInfo)
		: m_chunk(chunk)
		, m_typeSize(compTypeInfo->GetTypeSize())
	{
		size_t typeOffset = 0;
		m_chunk->GetArchetype()->GetComponentTypeOffset(compTypeInfo, typeOffset);
		m_address = m_chunk->GetBlockData() + (typeOffset)* m_chunk->GetCapacity();
	}
	
	size_t ComponentVisitor::GetCount()const
	{
		return m_chunk->GetCount();
	}

	void* ComponentVisitor::GetDataAt(size_t index)const
	{
		return m_address + m_typeSize * index;
	}

	ComponentGroup::ComponentGroup(int count, ...)
	{
		std::list<ComponentTypeId> typeIdList;

		va_list args;
		va_start(args, count);
		while (count--)
		{
			auto arg = va_arg(args, TypeInfo*);
			ComponentTypeId compTypeId;
			compTypeId.m_typeInfo = arg;
			typeIdList.push_back(compTypeId);
		}
		va_end(args);
		auto itrBegin = EntityManager::Instance().ArchetypesBegin();
		auto itrEnd = EntityManager::Instance().ArchetypesEnd();

		for (auto typeId : typeIdList)
		{
			ComponentArray* compArray = new ComponentArray();
			compArray->m_typeInfo = typeId.m_typeInfo;
			compArray->m_totalCount = 0;
			for (auto itr = itrBegin; itr != itrEnd; ++itr)
			{
				Archetype* archetype = *itr;
				if (archetype->HasAllComponentTypeId(typeIdList))
				{
					std::vector<Chunk*> chunks = archetype->GetAllChunks();
					for (auto chunk : chunks)
					{
						ComponentVisitor* visitor = new ComponentVisitor(chunk, typeId.m_typeInfo);
						compArray->m_compVisitors.push_back(visitor);
						compArray->m_totalCount += chunk->GetCount();
					}
				}
			}
			m_componentArray.push_back(compArray);
		}
	}

	ComponentGroup::~ComponentGroup()
	{
		for (auto comparr : m_componentArray)
		{
			delete comparr;
		}
		m_componentArray.clear();
	}

	ComponentArray* ComponentGroup::GetComponentArray(TypeInfo* typeInfo)const
	{
		for (auto comparr : m_componentArray)
		{
			if (comparr->m_typeInfo->GetTypeId() == typeInfo->GetTypeId())
			{
				return comparr;
			}
		}
		return nullptr;
	}

	ComponentArray::ComponentArray()
		: m_typeInfo(nullptr)
		, m_totalCount(0)
	{
	}

	ComponentArray::~ComponentArray()
	{
		for (auto visistor : m_compVisitors)
		{
			delete visistor;
		}
		m_compVisitors.clear();
	}
}