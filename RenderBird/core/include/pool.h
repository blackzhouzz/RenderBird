#pragma once
#include <vector>
#include <set>

namespace Core
{
	using DataHandle = std::pair<int, void*>;
	const int INVALID_DATA_ID = -1;
	template<typename T>
	struct DefaultFilter
	{
		bool operator()(const T& t)
		{
			return true;
		}
	};

	class Pool
	{
	public:
		explicit Pool(int dataSize, int chunkSize = 4096, int reserveSize = 4096)
			: m_dataSize(dataSize + sizeof(int))
			, m_chunkSize(chunkSize)
			, m_blockSize((dataSize + sizeof(int)) * chunkSize)
		{
			Reserve(reserveSize);
		}
		int GetCount() const { return m_count; }
		size_t GetCapacity() const { return m_capacity; }
		size_t GetChunkSize() const { return m_chunks.size(); }
		size_t GetMemorySize() const { return m_capacity * m_dataSize; }
		inline bool IsValidId(int id)const
		{
			if (id >= 0 && id < m_count)
			{
				return IsUsed(GetDataPtr(id));
			}
			return false;
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
			return GetDataPtr(id);
		}
		void PurgeMemory()
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
	protected:
		bool IsUsed(void* data)const
		{
			char* used = reinterpret_cast<char*>(data) + m_dataSize - sizeof(int);
			return *reinterpret_cast<int*>(used) == 1;
		}
		void SetUsed(void* data, int val)
		{
			char* used = reinterpret_cast<char*>(data) + m_dataSize - sizeof(int);
			*reinterpret_cast<int*>(used) = val;
		}
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
			void* data = GetDataPtr(id);
			SetUsed(data, 1);
			return data;
		}

		inline void* GetDataPtr(int id)const
		{
			return m_chunks[id / m_chunkSize] + (id % m_chunkSize) * m_dataSize;
		}

		inline bool Dealloc(int id)
		{
			if (m_freeList.find(id) == m_freeList.end())
			{
				void* data = GetDataPtr(id);
				SetUsed(data, 0);
				m_freeList.insert(id);
				return true;
			}
			return false;
		}
	public:
		template<typename T, typename Filter = DefaultFilter<T>>
		class PoolIterator
		{
		private:
			class Iterator
			{
			public:
				explicit Iterator(Pool* pool, int index)
					: m_pool(pool)
					, m_index(index)
				{
					MoveToValidIndex();
				}
				bool operator != (const Iterator& other) const
				{
					return (this->m_index) != (other.m_index);
				}
				const T& operator* () const
				{
					T* data = reinterpret_cast<T*>(m_pool->GetDataPtr(m_index));
					return *data;
				}
				T& operator* ()
				{
					T* data = reinterpret_cast<T*>(m_pool->GetDataPtr(m_index));
					return *data;
				}
				void MoveToValidIndex()
				{
					while (m_index < m_pool->m_count)
					{
						T* data = reinterpret_cast<T*>(m_pool->GetDataPtr(m_index));
						if (m_pool->IsUsed(data) && Filter()(*data))
						{
							return;
						}
						m_index++;
					}
				}
				const Iterator& operator++ ()
				{
					m_index++;
					MoveToValidIndex();
					return *this;
				}
			private:
				Pool * m_pool;
				int m_index;
			};
		public:
			PoolIterator(Pool* pool)
				: m_pool(pool)
			{
			}
			Iterator begin()
			{
				return Iterator(m_pool, 0);
			}
			Iterator end()
			{
				return Iterator(m_pool, m_pool->m_count);
			}
			const Iterator begin()const
			{
				return Iterator(m_pool, 0);
			}
			const Iterator end()const
			{
				return Iterator(m_pool, m_pool->m_count);
			}
		private:
			Pool* m_pool;
		};
		template<typename T, typename Filter = DefaultFilter<T>>
		PoolIterator<T, Filter> Each()
		{
			return PoolIterator<T, Filter>(this);
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
	public:
		template<typename Filter = DefaultFilter<T>>
		PoolIterator<T, Filter> EachObjects()
		{
			return PoolIterator<T, Filter>(this);
		}
	};
}
