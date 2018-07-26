#pragma once
#include "runtime_private.h"
#include "chunk.h"
#include "component.h"

namespace Runtime
{
	class ComponentVisitor
	{
	public:
		explicit ComponentVisitor(Chunk* chunk, TypeInfo* compTypeInfo);
	public:
		size_t GetCount()const;
		void* GetDataAt(size_t index)const;
		template<typename T>
		T* GetDataAt(size_t index)const
		{
			return ((T*)m_address) + index;
		}
	private:
		Chunk* m_chunk;
		uint8* m_address;
		uint8* m_iterator;
		size_t m_typeSize;
	};

	class ComponentArray
	{
	public:
		ComponentArray();
		~ComponentArray();

		template<typename T>
		class Visitor
		{
		public:
			Visitor(ComponentArray* compArray)
				: m_compArray(compArray)
			{
				assert(compArray->m_typeInfo->GetTypeSize() == sizeof(T));
			}
			class Iterator
			{
			public:
				explicit Iterator(const Visitor<T>* visitor, size_t index)
					: m_visitor(visitor)
					, m_index(index)
					, m_vecSubIndex(0)
					, m_vecIndex(0)
					, m_currentVisitor(m_visitor->m_compArray->m_compVisitors[m_vecIndex])
				{
				}
				bool operator != (const Iterator& other) const
				{
					return (this->m_index) != (other.m_index);
				}
				T* operator* () const
				{
					return m_currentVisitor->GetDataAt<T>(m_vecSubIndex);
				}
				T* operator* ()
				{
					return m_currentVisitor->GetDataAt<T>(m_vecSubIndex);
				}
				const Iterator& operator++ ()
				{
					m_index++;
					m_vecSubIndex++;
					if (m_vecSubIndex >= m_currentVisitor->GetCount())
					{
						m_vecSubIndex = 0;
						m_vecIndex++;
						m_currentVisitor = m_visitor->m_compArray->m_compVisitors[m_vecIndex];
					}
					return *this;
				}
			private:
				const Visitor<T>* m_visitor;
				size_t m_index;
				size_t m_vecSubIndex;
				size_t m_vecIndex;
				ComponentVisitor* m_currentVisitor;
			};
			Iterator begin()
			{
				return Iterator(this, 0);
			}
			Iterator end()
			{
				return Iterator(this, m_compArray->m_totalCount);
			}
			const Iterator begin()const
			{
				return Iterator(this, 0);
			}
			const Iterator end()const
			{
				return Iterator(this, m_compArray->m_totalCount);
			}
		private:
			ComponentArray* m_compArray;
		};
		std::vector<ComponentVisitor*> m_compVisitors;
		TypeInfo* m_typeInfo;
		size_t m_totalCount;
	};

	class ComponentGroup
	{
	public:
		ComponentGroup(int count, ...);
		~ComponentGroup();
		ComponentArray* GetComponentArray(TypeInfo* typeInfo)const;
		template<typename T>
		ComponentArray* GetComponentArray()const
		{
			return GetComponentArray(TypeOf<T>::Value());
		}
		std::vector<ComponentArray*> m_componentArray;
	};

}