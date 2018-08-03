#include "component_chunk.h"

namespace Core
{
	ComponentChunk::ComponentChunk(Archetype* archetype, size_t capacity)
		: m_capacity(capacity)
		, m_archetype(archetype)
		, m_lastIndex(0)
	{
		size_t blockSize = (archetype->GetStrideSize()) * capacity;
		m_blockData = new uint8[blockSize];
	}

	ComponentChunk::~ComponentChunk()
	{
		PurgeMemory();
	}

	size_t ComponentChunk::Allocate()
	{
		if (m_lastIndex == m_capacity)
		{
			assert("allocate chunk failed");
		}
		size_t id = m_lastIndex++;
		m_archetype->MoveChunk(this);
		return id;
	}

	void ComponentChunk::CopyData(size_t srcChunkIndex, size_t destChunkIndex)
	{
		uint8* compData = m_blockData;
		for (auto type : *m_archetype)
		{
			size_t typeSize = type.m_typeInfo->GetTypeSize();
			uint8* srcData = compData + typeSize * srcChunkIndex;
			uint8* destData = compData + typeSize * destChunkIndex;
			memcpy(destData, srcData, typeSize);
			compData += typeSize * m_capacity;
		}
	}

	void ComponentChunk::Deallocate(size_t chunkIndex)
	{
		if (chunkIndex != m_lastIndex - 1)
		{
			CopyData(m_lastIndex - 1, chunkIndex);
			--m_lastIndex;
			m_archetype->MoveChunk(this);
		}
	}

	void* ComponentChunk::GetEntityComponentPtr(size_t entityIndex, TypeInfo* compTypeInfo)
	{
		size_t typeOffset = 0;
		if (!m_archetype->GetComponentTypeOffset(compTypeInfo, typeOffset))
		{
			return nullptr;
		}
		return m_blockData + (typeOffset)* m_capacity + compTypeInfo->GetTypeSize() * entityIndex;
	}

	void ComponentChunk::SetAllComponentDefault(size_t entityIndex)
	{
		uint8* compData = m_blockData;
		for (auto compTypeInfo : *m_archetype)
		{
			size_t typeSize = compTypeInfo.m_typeInfo->GetTypeSize();
			uint8* data = compData + typeSize * entityIndex;
			memcpy(data, compTypeInfo.m_typeInfo->GetDefaultValue(), typeSize);
			compData += typeSize * m_capacity;
		}
	}

	void* ComponentChunk::SetComponentDefault(size_t entityIndex, TypeInfo* compTypeInfo)
	{
		size_t typeOffset = 0;
		if (!m_archetype->GetComponentTypeOffset(compTypeInfo, typeOffset))
		{
			return nullptr;
		}
		void* data = m_blockData + (typeOffset)* m_capacity + compTypeInfo->GetTypeSize() * entityIndex;
		memcpy(data, compTypeInfo->GetDefaultValue(), compTypeInfo->GetTypeSize());
		return data;
	}

	void ComponentChunk::PurgeMemory()
	{
		delete[] m_blockData;
		m_blockData = nullptr;
		m_capacity = 0;
		m_lastIndex = 0;
	}
}