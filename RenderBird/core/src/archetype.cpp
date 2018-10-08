#include "Archetype.h"
#include "ComponentChunk.h"

namespace Core
{
	Archetype::Archetype(const std::list<ComponentTypeId>& typeIdList)
		: m_typeIdList(typeIdList)
	{
		m_typeIdList.sort();
		m_strideSize = 0;
		for (auto id : *this)
		{
			TypeInfo* typeInfo = id.m_typeInfo;
			m_strideSize += typeInfo->GetTypeSize();
		}
	}

	void Archetype::OnDestroy()
	{
		for (auto chunk : m_fullChunk)
		{
			delete chunk;
		}
		m_fullChunk.clear();
		for (auto chunk : m_nonFullChunk)
		{
			delete chunk;
		}
		m_nonFullChunk.clear();
	}

	Archetype::~Archetype()
	{
		OnDestroy();
	}

	bool Archetype::HasComponentTypeId(TypeInfo* typeInfo)const
	{
		ComponentTypeId id(typeInfo);
		return std::find(m_typeIdList.begin(), m_typeIdList.end(), id) != m_typeIdList.end();
	}
	bool Archetype::HasComponentTypeId(const ComponentTypeId& info)const
	{
		return std::find(m_typeIdList.begin(), m_typeIdList.end(), info) != m_typeIdList.end();
	}
	bool Archetype::IsEqual(Archetype* archetype)const
	{
		return IsEqual(archetype->m_typeIdList);
	}
	bool Archetype::IsEqual(const std::list<ComponentTypeId>& typeList)const
	{
		if (typeList.size() != m_typeIdList.size())
			return false;
		for (std::list<ComponentTypeId>::const_iterator itrLeft = m_typeIdList.begin()
			, itrRight = typeList.begin();
			itrLeft != m_typeIdList.end();
			++itrLeft, ++itrRight
			)
		{
			if (itrLeft->m_typeInfo->GetTypeId() != itrRight->m_typeInfo->GetTypeId())
			{
				return false;
			}
		}
		return true;
	}
	bool Archetype::HasAllComponentTypeId(const std::list<ComponentTypeId>& typeList)const
	{
		for (auto itrLeft = typeList.begin(); itrLeft != typeList.end(); ++itrLeft)
		{
			bool isFind = false;
			for (auto itrRight = m_typeIdList.begin(); itrRight != m_typeIdList.end(); ++itrRight)
			{
				if (itrRight->m_typeInfo->GetTypeId() == itrLeft->m_typeInfo->GetTypeId())
				{
					isFind = true;
					break;
				}
			}
			if (!isFind)
			{
				return false;
			}
		}
		return true;
	}

	std::list<ComponentTypeId>::const_iterator Archetype::begin()const
	{
		return m_typeIdList.begin();
	}

	std::list<ComponentTypeId>::const_iterator Archetype::end()const
	{
		return m_typeIdList.end();
	}

	bool Archetype::GetComponentTypeOffset(TypeInfo* type, size_t& offset)const
	{
		offset = 0;
		for (auto id : m_typeIdList)
		{
			if (id.m_typeInfo->GetTypeId() == type->GetTypeId())
			{
				return true;
			}
			offset += id.m_typeInfo->GetTypeSize();
		}
		return false;
	}

	ComponentChunk* Archetype::GetOrCreateChunk()
	{
		ComponentChunk* ret = nullptr;
		if (m_nonFullChunk.size() > 0)
		{
			ret = *m_nonFullChunk.begin();
		}
		else
		{
			ret = new ComponentChunk(this);
			m_nonFullChunk.insert(ret);
		}
		return ret;
	}

	void Archetype::UpdateChunk(ComponentChunk* chunk)
	{
		if (chunk->IsFull())
		{
			if (m_nonFullChunk.find(chunk) != m_nonFullChunk.end())
			{
				m_nonFullChunk.erase(chunk);
				m_fullChunk.insert(chunk);
			}
		}
		else
		{
			if (m_fullChunk.find(chunk) != m_fullChunk.end())
			{
				m_fullChunk.erase(chunk);
				m_nonFullChunk.insert(chunk);
			}
		}
	}

	std::vector<ComponentChunk*> Archetype::GetAllChunks()const
	{
		std::vector<ComponentChunk*> ret;
		for (auto chunk : m_fullChunk)
		{
			ret.push_back(chunk);
		}
		for (auto chunk : m_nonFullChunk)
		{
			ret.push_back(chunk);
		}
		return ret;
	}
}