#pragma once
#include "component.h"
#include "ecs/componentregister.h"

namespace RenderBird
{
	struct EntityData
	{
		EntityData()
			: m_id(INVALID_DATA_ID)
		{
			for (int i = 0; i < CompTypeListCount; ++i)
			{
				m_compIds[i] = INVALID_DATA_ID;
			}
		}
		int m_id;
		int m_compIds[CompTypeListCount];
	};
	using EntityHandle = ObjectPool<EntityData>::ObjectHandle;
}