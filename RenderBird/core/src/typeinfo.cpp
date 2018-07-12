#include "typeinfo.h"

void GlobalTypeList::RegisterTypeInfo(TypeInfo* typeInfo)
{
	int typeId = typeInfo->GetTypeId();
	assert(m_typeMap.find(typeId) == m_typeMap.end());
	m_typeMap[typeId] = typeInfo;
}

TypeInfo* GlobalTypeList::GetTypeInfo(int typeId)
{
	TypeInfoMap::iterator itr = m_typeMap.find(typeId);
	if (itr != m_typeMap.end())
	{
		return itr->second;
	}
	return nullptr;
}

TypeMapIterator GlobalTypeList::begin()
{
	return m_typeMap.begin();
}

TypeMapIterator GlobalTypeList::end()
{
	return m_typeMap.end();
}