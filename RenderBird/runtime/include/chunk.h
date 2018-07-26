#pragma once
#include "runtime_private.h"
#include "archetype.h"

namespace Runtime
{
	class Chunk
	{
	public:
		static const size_t DEFAULT_SIZE = 16 * 1024;
		explicit Chunk(Archetype* archetype, size_t capacity = DEFAULT_SIZE);
		~Chunk();
		size_t GetCount()const { return m_lastIndex; }
		size_t GetCapacity()const { return m_capacity; }
		bool IsFull()const { return m_lastIndex >= m_capacity; }
		Archetype* GetArchetype()const { return m_archetype; }
		uint8* GetBlockData()const { return m_blockData; }
		size_t Allocate();
		void Deallocate(size_t chunkIndex);
		void CopyData(size_t srcChunkIndex, size_t destChunkIndex);
		void* GetEntityComponentPtr(size_t entityIndex, TypeInfo* compTypeInfo);
		void SetAllComponentDefault(size_t entityIndex);
		void* SetComponentDefault(size_t entityIndex, TypeInfo* compTypeInfo);
	private:
		void PurgeMemory();
	private:
		uint8* m_blockData;
		size_t m_capacity;
		size_t m_lastIndex;
		Archetype* m_archetype;
	};

}