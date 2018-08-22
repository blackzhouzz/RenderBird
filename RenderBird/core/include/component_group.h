#pragma once
#include "core_private.h"
#include "component_chunk.h"
#include "component.h"

namespace Core
{
	class ComponentArray
	{
	private:
		friend class ComponentGroup;
		class ComponentVisitor
		{
		public:
			explicit ComponentVisitor(ComponentChunk* chunk, TypeInfo* compTypeInfo)
				: m_chunk(chunk)
			{
				size_t typeOffset = 0;
				m_chunk->GetArchetype()->GetComponentTypeOffset(compTypeInfo, typeOffset);
				m_address = m_chunk->GetComponentData() + (typeOffset)* m_chunk->GetCapacity();
			}
		public:
			template<typename T>
			T* GetDataAt(size_t index)const
			{
				return ((T*)m_address) + index;
			}
			EntityId GetEntityId(size_t index)const
			{
				return m_chunk->GetEntityId(index);
			}
			size_t GetCount()const { return m_chunk->GetCount(); }
		private:
			ComponentChunk* m_chunk;
			uint8* m_address;
		};
	private:
		ComponentArray();
	public:
		TypeInfo* GetTypeInfo()const { return m_typeInfo; }
		template<typename T>
		class Visitor
		{
		public:
			Visitor() {}
			Visitor(ComponentArray* componentArray)
				: m_componentArray(componentArray)
			{
			}
			T* Get(size_t firstIndex, size_t secondIndex)const
			{
				return m_componentArray->m_compVisitors[firstIndex].GetDataAt<T>(secondIndex);
			}
			EntityId GetEntityId(size_t firstIndex, size_t secondIndex)const
			{
				return m_componentArray->m_compVisitors[firstIndex].GetEntityId(secondIndex);
			}
			void MoveNext(size_t& firstIndex, size_t& secondIndex)
			{
				secondIndex++;
				if (secondIndex >= m_componentArray->m_compVisitors[firstIndex].GetCount())
				{
					secondIndex = 0;
					firstIndex++;
				}
			}
			ComponentArray* m_componentArray;
		};
	private:
		std::vector<ComponentVisitor> m_compVisitors;
		TypeInfo* m_typeInfo;
		size_t m_count;
	};

	class ComponentGroup
	{
		template<typename ... Types> friend class ComponentGroupVisitor;
	public:
		ComponentGroup();
		ComponentGroup(int count, ...);
		ComponentGroup(Archetype* archetype);
		~ComponentGroup();
		template<typename ... Types>
		static void Create(ComponentGroup* group)
		{
			std::list<ComponentTypeId> typeIdList;
			CreateImpl<0, Types...>(typeIdList);
			typeIdList.sort();
			typeIdList.erase(std::unique(typeIdList.begin(), typeIdList.end()), typeIdList.end());
			group->CreateFromTypeList(typeIdList);
		}
		ComponentArray* GetComponentArray(TypeInfo* typeInfo)const;
		template<typename T>
		ComponentArray* GetComponentArray()const
		{
			return GetComponentArray(TypeOf<T>::Value());
		}
	private:
		template <int N, typename C>
		static void CreateImpl(std::list<ComponentTypeId>& typeIdList)
		{
			ComponentTypeId compTypeId;
			compTypeId.m_typeInfo = TypeOf<C>::Value();
			typeIdList.push_back(compTypeId);
		}
		template <int N, typename C0, typename C1, typename ... Cn>
		static void CreateImpl(std::list<ComponentTypeId>& typeIdList)
		{
			ComponentTypeId compTypeId;
			compTypeId.m_typeInfo = TypeOf<C0>::Value();
			typeIdList.push_back(compTypeId);
			CreateImpl<N + 1, C1, Cn...>(typeIdList);
		}
		template<typename T>
		ComponentArray::Visitor<T> GetComponentArrayVisitor()const
		{
			return ComponentArray::Visitor<T>(GetComponentArray<T>());
		}
		size_t GetComponentCount()const { return m_componentCount; }
		void CreateFromTypeList(const std::list<ComponentTypeId>& typeIdList);
	private:
		std::vector<ComponentArray*> m_componentArray;
		size_t m_componentCount;
	};

	template<typename ... Types>
	class ComponentGroupVisitor
	{
	public:
		explicit ComponentGroupVisitor(ComponentGroup* group)
			: m_group(group)
		{
			Unpack();
		}
		void Reset()
		{
			m_currentIndex = 0;
			m_firstIndex = 0;
			m_secondIndex = 0;
		}
		bool HasNext()const
		{
			if (m_group == nullptr)
				return false;
			return m_currentIndex < m_group->GetComponentCount();
		}
		template<typename T>
		T* Get()const
		{
			return std::get<type_index<T>::value>(m_visitors).Get(m_firstIndex, m_secondIndex);
		}
		EntityId GetEntityId()const
		{
			return std::get<0>(m_visitors).GetEntityId(m_firstIndex, m_secondIndex);
		}
		void MoveNext()
		{
			m_currentIndex++;
			std::get<0>(m_visitors).MoveNext(m_firstIndex, m_secondIndex);
		}
	private:
		void Unpack()
		{
			if (m_group != nullptr)
			{
				UnpackImpl<0, Types...>();
			}
		}
		template <int N, typename C>
		void UnpackImpl()
		{
			std::get<N>(m_visitors) = m_group->GetComponentArrayVisitor<C>();
		}
		template <int N, typename C0, typename C1, typename ... Cn>
		void UnpackImpl()
		{
			std::get<N>(m_visitors) = m_group->GetComponentArrayVisitor<C0>();
			UnpackImpl<N + 1, C1, Cn...>();
		}
		template<typename, typename>
		struct index_of {};

		// index_of base case: found the type we're looking for.
		template <typename T, typename... List>
		struct index_of<T, ComponentGroupVisitor<T, List...>>
			: std::integral_constant<std::size_t, 0>
		{
		};

		// index_of recursive case: 1 + index_of the rest of the types.
		template <typename T, typename TOther, typename... List>
		struct index_of<T, ComponentGroupVisitor<TOther, List...>>
			: std::integral_constant<std::size_t,
			1 + index_of<T, ComponentGroupVisitor<List...>>::value>
		{
		};

		template<typename T>
		using type_index = typename index_of<T, ComponentGroupVisitor<Types...>>;
	private:
		std::tuple<ComponentArray::Visitor<Types> ...> m_visitors;
		ComponentGroup* m_group;
		size_t m_firstIndex;
		size_t m_secondIndex;
		size_t m_currentIndex;
	};
}