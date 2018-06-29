#pragma once
#include "core/typeinfo.h"

namespace RenderBird
{
	enum ETypeID
	{
		TYPE_ID_ITEM(Entity, 1)
		TYPE_ID_ITEM(Component, 2)
		TYPE_ID_ITEM(Geometry, 3)
		TYPE_ID_ITEM(TriangleMesh, 4)
	};
	extern void RegisterAllTypeInfo();
	extern TypeInfo* GetTypeInfoFromTypeMap(int typeId);
}