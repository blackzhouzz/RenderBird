#include "chunk.h"

namespace Runtime
{
	Chunk::Chunk(Archetype* archetype, size_t capacity)
		: m_capacity(capacity)
		, m_archetype(archetype)
		, m_lastIndex(0)
	{
		size_t blockSize = (archetype->GetStrideSize()) * capacity;
		m_blockData = new uint8[blockSize];
	}

	Chunk::~Chunk()
	{
		PurgeMemory();
	}

	size_t Chunk::Allocate()
	{
		if (m_lastIndex == m_capacity)
		{
			assert("allocate chunk failed");
		}
		size_t id = m_lastIndex++;
		m_archetype->MoveChunk(this);
		return id;
	}

	void Chunk::CopyData(size_t srcBlockIndex, size_t destBlockIndex)
	{
		uint8* compData = m_blockData;
		for (auto type : *m_archetype)
		{
			size_t typeSize = type.m_typeInfo->GetTypeSize();
			uint8* srcData = compData + typeSize * srcBlockIndex;
			uint8* destData = compData + typeSize * destBlockIndex;
			memcpy(destData, srcData, typeSize);
			compData += typeSize * m_capacity;
		}
	}

	void Chunk::Deallocate(size_t chunkIndex)
	{
		if (chunkIndex != m_lastIndex - 1)
		{
			CopyData(m_lastIndex - 1, chunkIndex);
			--m_lastIndex;
			m_archetype->MoveChunk(this);
		}
	}

	void* Chunk::GetEntityComponentPtr(size_t entityIndex, TypeInfo* compTypeInfo)
	{
		size_t typeOffset = 0;
		if (!m_archetype->GetComponentTypeOffset(compTypeInfo, typeOffset))
		{
			return nullptr;
		}
		return m_blockData + (typeOffset)* m_capacity + compTypeInfo->GetTypeSize() * entityIndex;
	}

	void Chunk::PurgeMemory()
	{
		delete[] m_blockData;
		m_blockData = nullptr;
		m_capacity = 0;
		m_lastIndex = 0;
	}
}