#pragma once
#include "CorePrivate.h"
#include "CoreRegister.h"
#include "Entity.h"

namespace Core
{
	struct TransformParent
	{
		EntityId m_parent;
	};
}


DECLEAR_TYPE_COMPONENT(Core, TransformParent);

DEFAULT_BEGIN(Core, TransformParent)
DEFAULT_DATA(Core::EntityId::INVALID)
DEFAULT_END()
