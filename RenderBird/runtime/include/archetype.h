#pragma once
#include "runtime_private.h"
#include <algorithm>
#include <list>
#include "component.h"

namespace Runtime
{
	class Chunk;
	class Archetype
	{
		friend class EntityManager;
	private:
		explicit Archetype(const std::list<ComponentTypeId>& typeIdList);
		void OnDestroy();
	public:
		~Archetype();
		bool HasComponentTypeId(TypeInfo* typeInfo)const;
		bool HasComponentTypeId(const ComponentTypeId& info)const;
		bool IsEqual(Archetype* archetype)const;
		bool IsEqual(const std::list<ComponentTypeId>& typeList)const;
		bool HasAllComponentTypeId(const std::list<ComponentTypeId>& typeList)const;
		std::list<ComponentTypeId>::const_iterator begin()const;
		std::list<ComponentTypeId>::const_iterator end()const;
		bool GetComponentTypeOffset(TypeInfo* type, size_t& offset)const;
		size_t GetStrideSize()const { return m_strideSize; }
		const std::list<ComponentTypeId>& GetComponentTypeList()const { return m_typeIdList; }
		Chunk* GetOrCreateChunk();
		void MoveChunk(Chunk* chunk);
		std::vector<Chunk*> GetAllChunks()const;
	private:
		std::list<ComponentTypeId> m_typeIdList;
		size_t m_strideSize;
		std::set<Chunk*> m_fullChunk;
		std::set<Chunk*> m_nonFullChunk;
	};

}