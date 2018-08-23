#pragma once
#include "core_private.h"
#include "core_register.h"
#include "entity.h"

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
