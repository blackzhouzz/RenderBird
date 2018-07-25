#pragma once
#include <vector>
#include <set>

namespace Core
{
	const size_t INVALID_POOL_ID = (size_t)(-1);

	class PoolBase
	{
	public:
		static const size_t DEFAULT_SIZE = 4096;
		using Handle = std::pair<size_t, void*>;
		explicit PoolBase(size_t dataSize, size_t chunkSize = DEFAULT_SIZE, size_t reserveSize = DEFAULT_SIZE);
		~PoolBase();
		size_t GetCount() const { return m_count; }
		size_t GetCapacity() const { return m_capacity; }
		size_t GetChunkSize() const { return m_chunks.size(); }
		size_t GetMemorySize() const { return m_capacity * m_dataSize; }
		void Reserve(std::size_t n);
		void PurgeMemory();
	protected:
		void* Alloc(size_t& id);
		bool Dealloc(size_t id);
		void* GetDataPtr(size_t id)const;
	protected:
		std::vector<char*> m_chunks;
		std::set<size_t> m_freeList;
		size_t m_dataSize;
		size_t m_chunkSize;
		size_t m_blockSize;
		size_t m_capacity;
		size_t m_count;
	};

	template<typename T>
	class PODPool : public PoolBase
	{
	public:
		static const size_t DEFAULT_SIZE = 4096;
		using Super = PoolBase;
		using Handle = std::pair<size_t, T*>;
		explicit PODPool(size_t chunkSize = DEFAULT_SIZE)
			: Super(sizeof(T), chunkSize)
		{
			static_assert(std::is_pod<T>::value, "T is must be pod");
		}
		inline Handle Malloc()
		{
			Handle ret;
			void* data = Super::Alloc(ret.first);
			ret.second = static_cast<T*>(data);
			return ret;
		}

		inline void Free(const Handle& handle)
		{
			Dealloc(handle.first);
		}

		inline void Free(size_t handleId)
		{
			Dealloc(handleId);
		}

		inline T* Get(size_t id)const
		{
			return static_cast<T*>(Super::GetDataPtr(id));
		}
	};

	template<typename T>
	class ObjectPool : public PoolBase
	{
	public:
		static const size_t DEFAULT_SIZE = 4096;
		using Super = PoolBase;
		using Handle = std::pair<size_t, T*>;
		explicit ObjectPool(size_t chunkSize = DEFAULT_SIZE)
			: Super(sizeof(T), chunkSize)
		{
		}

		template <typename ... Args>
		inline Handle New(Args && ... args)
		{
			Handle ret;
			void* data = Super::Alloc(ret.first);
			::new(data) T(std::forward<Args>(args) ...);
			ret.second = static_cast<T*>(data);
			return ret;
		}

		inline void Delete(const Handle& handle)
		{
			if (Super::Dealloc(handle.first))
			{
				T *ptr = static_cast<T*>(Super::GetDataPtr(handle.first));
				ptr->~T();
			}
		}

		inline T* Get(size_t id)const
		{
			return static_cast<T*>(Super::GetDataPtr(id));
		}
	};
}
