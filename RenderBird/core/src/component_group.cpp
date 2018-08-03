#include <cstdarg>
#include "component_group.h"
#include "entitymanager.h"

namespace Core
{
	ComponentGroup::ComponentGroup()
		: m_componentCount(0)
	{
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

		CreateFromTypeList(typeIdList);
	}

	ComponentGroup::ComponentGroup(Archetype* archetype)
	{
		const std::list<ComponentTypeId>& typeIdList = archetype->GetComponentTypeList();
		CreateFromTypeList(typeIdList);
	}

	ComponentGroup::~ComponentGroup()
	{
		for (auto comparr : m_componentArray)
		{
			delete comparr;
		}
		m_componentArray.clear();
	}

	void ComponentGroup::CreateFromTypeList(const std::list<ComponentTypeId>& typeIdList)
	{
		auto itrBegin = EntityManager::Instance().ArchetypesBegin();
		auto itrEnd = EntityManager::Instance().ArchetypesEnd();

		for (auto typeId : typeIdList)
		{
			ComponentArray* compArray = new ComponentArray();
			compArray->m_typeInfo = typeId.m_typeInfo;
			compArray->m_count = 0;
			for (auto itr = itrBegin; itr != itrEnd; ++itr)
			{
				Archetype* archetype = *itr;
				if (archetype->HasAllComponentTypeId(typeIdList))
				{
					std::vector<ComponentChunk*> chunks = archetype->GetAllChunks();
					for (auto chunk : chunks)
					{
						compArray->m_compVisitors.push_back(ComponentArray::ComponentVisitor(chunk, typeId.m_typeInfo));
						compArray->m_count += chunk->GetCount();
					}
				}
			}
			m_componentArray.push_back(compArray);
			if (m_componentCount == 0)
			{
				m_componentCount = compArray->m_count;
			}
			else if (m_componentCount != compArray->m_count)
			{
				assert(false);
			}
		}
	}

	ComponentArray* ComponentGroup::GetComponentArray(TypeInfo* typeInfo)const
	{
		for (auto comparr : m_componentArray)
		{
			if (comparr->GetTypeInfo()->GetTypeId() == typeInfo->GetTypeId())
			{
				return comparr;
			}
		}
		return nullptr;
	}

	ComponentArray::ComponentArray()
		: m_typeInfo(nullptr)
		, m_count(0)
	{
	}
}