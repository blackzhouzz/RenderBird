#pragma once
#include "runtime_private.h"
#include "component.h"
#include "math/vector.h"
#include "math/quaternion.h"

namespace Runtime
{
	struct Transform
	{
		Vector3f m_position;
		Quaternion m_rotation;
		Vector3f m_scale;
		bool m_needUpdate;
		Matrix4f m_cachedTransform;
	};
}

DECLEAR_TYPE_COMPONENT(Runtime, Transform);

DEFAULT_BEGIN(Runtime, Transform)
DEFAULT_DATA(Vector3f::ZERO)
DEFAULT_DATA(Quaternion::IDENTITY)
DEFAULT_DATA(Vector3f::ONE)
DEFAULT_DATA(false)
DEFAULT_DATA(Matrix4f::IDENTITY)
DEFAULT_END()
