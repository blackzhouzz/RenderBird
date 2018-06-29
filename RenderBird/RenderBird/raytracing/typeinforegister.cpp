#include "typeinforegister.h"
#include "runtime/object.h"

namespace RenderBird
{
	typedef std::map<int, TypeInfo*> TypeInfoMap;
	static TypeInfoMap* typeInfoMap = nullptr;

	void GlobalRegisterTypeInfo(TypeInfo* typeInfo)
	{
		if (typeInfoMap == nullptr)
		{
			typeInfoMap = new TypeInfoMap();
		}
		int typeId = typeInfo->GetTypeId();
		assert(typeInfoMap->find(typeId) == typeInfoMap->end());
		(*typeInfoMap)[typeId] = typeInfo;
	}

	TypeInfo* GetTypeInfoFromTypeMap(int typeId)
	{
		TypeInfoMap::iterator itr = typeInfoMap->find(typeId);
		if (itr != typeInfoMap->end())
		{
			return itr->second;
		}
		return nullptr;
	}

#define REGISTER_TYPEINFO(cls) \
	{ \
		extern void RegisterTypeInfo##cls(); \
		RegisterTypeInfo##cls(); \
	}

	void RegisterAllTypeInfo()
	{
		REGISTER_TYPEINFO(Object)
		REGISTER_TYPEINFO(TriangleMesh)
	}


}