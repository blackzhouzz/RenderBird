#include "Pool.h"

namespace Core
{
	PoolBase::PoolBase(size_t dataSize, size_t chunkSize, size_t reserveSize)
		: m_dataSize(dataSize)
		, m_chunkSize(chunkSize)
		, m_blockSize(dataSize * chunkSize)
	{
		Reserve(reserveSize);
	}

	PoolBase::~PoolBase()
	{
		PurgeMemory();
	}

	void PoolBase::Reserve(std::size_t n)
	{
		while (m_capacity < n)
		{
			char *chunk = new char[m_blockSize];
			m_chunks.push_back(chunk);
			m_capacity += m_chunkSize;
		}
	}

	void PoolBase::PurgeMemory()
	{
		for (size_t i = 0; i < m_chunks.size(); ++i)
		{
			delete[] m_chunks[i];
		}
		m_chunks.clear();
		m_freeList.clear();
		m_capacity = 0;
		m_count = 0;
	}

	void* PoolBase::Alloc(size_t& id)
	{
		id = INVALID_POOL_ID;
		if (m_freeList.size() > 0)
		{
			id = *m_freeList.begin();
			m_freeList.erase(m_freeList.begin());
		}
		else
		{
			id = m_count;
			m_count++;
		}
		Reserve(m_count);
		return GetDataPtr(id);
	}

	bool PoolBase::Dealloc(size_t id)
	{
		if (m_freeList.find(id) == m_freeList.end())
		{
			void* data = GetDataPtr(id);
			m_freeList.insert(id);
			return true;
		}
		return false;
	}

	void* PoolBase::GetDataPtr(size_t id)const
	{
		return m_chunks[id / m_chunkSize] + (id % m_chunkSize) * m_dataSize;
	}
}