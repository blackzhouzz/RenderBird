#pragma once
#include <vector>
#include <set>

namespace RenderBird
{
	using DataHandle = std::pair<int, void*>;
	const int INVALID_DATA_ID = -1;

	class Pool
	{
	public:
		explicit Pool(int dataSize, int chunkSize = 4096)
			: m_dataSize(dataSize)
			, m_chunkSize(chunkSize)
			, m_blockSize(dataSize * chunkSize)
		{
		}
		int GetCount() const { return m_count; }
		size_t GetCapacity() const { return m_capacity; }
		size_t GetChunkSize() const { return m_chunks.size(); }
		size_t GetMemorySize() const { return m_capacity * m_dataSize; }
		inline bool IsValidId(int id)const
		{
			if (id >= m_count 
				|| id == INVALID_DATA_ID
				|| m_freeList.find(id) != m_freeList.end()
				)
			{
				return false;
			}
			return true;
		}
		~Pool()
		{
			PurgeMemory();
		}
		inline void Reserve(std::size_t n)
		{
			while (m_capacity < n)
			{
				char *chunk = new char[m_blockSize];
				m_chunks.push_back(chunk);
				m_capacity += m_chunkSize;
			}
		}
		inline DataHandle Malloc()
		{
			DataHandle ret;
			ret.second = Alloc(ret.first);
			return ret;
		}
		inline void Free(DataHandle& handle)
		{
			Free(handle.first);
			handle.first = INVALID_DATA_ID;
		}
		inline void Free(int id)
		{
			Dealloc(id);
		}
		inline void* Get(int id)const
		{
			return m_chunks[id / m_chunkSize] + (id % m_chunkSize) * m_dataSize;
		}
		void PurgeMemory()
		{
			for (int i = 0; i < m_chunks.size(); ++i)
			{
				delete[] m_chunks[i];
			}
			m_chunks.clear();
			m_freeList.clear();
			m_capacity = 0;
			m_count = 0;
		}
	protected:
		inline void* Alloc(int& id)
		{
			id = INVALID_DATA_ID;
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
			void* data = m_chunks[id / m_chunkSize] + (id % m_chunkSize) * m_dataSize;
			return data;
		}

		inline bool Dealloc(int id)
		{
			if (m_freeList.find(id) == m_freeList.end())
			{
				m_freeList.insert(id);
				return true;
			}
			return false;
		}

	protected:
		std::vector<char*> m_chunks;
		std::set<int> m_freeList;
		int m_dataSize;
		int m_chunkSize;
		int m_blockSize;
		size_t m_capacity = 0;
		int m_count = 0;
	};

	template<typename T>
	class ObjectPool : public Pool
	{
	public:
		using Super = Pool;
		using ObjectHandle = std::pair<int, T*>;
		explicit ObjectPool(int chunkSize = 4096)
			: Super(sizeof(T), chunkSize)
		{
		}

		template <typename ... Args>
		inline ObjectHandle New(Args && ... args)
		{
			ObjectHandle ret;
			void* data = Super::Alloc(ret.first);
			::new(data) T(std::forward<Args>(args) ...);
			ret.second = static_cast<T*>(data);
			return ret;
		}

		inline void Delete(ObjectHandle& handle)
		{
			Delete(handle.first);
			handle.first = INVALID_DATA_ID;
		}

		inline void Delete(int id)
		{
			if (Super::Dealloc(id))
			{
				T *ptr = static_cast<T*>(Super::Get(id));
				ptr->~T();
			}
		}

		inline T* GetObject(int id)const
		{
			void* data = Super::Get(id);
			return static_cast<T*>(data);
		}

	};
}
