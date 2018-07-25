#pragma once
#include "runtime_private.h"
#include "component.h"
#include "math/vector.h"
#include "math/quaternion.h"

namespace Runtime
{
	struct Transform : public ComponentBase
	{
		Vector3f m_position;
		Quaternion m_rotation;
		Vector3f m_scale;
		bool m_needUpdate;
		Matrix4f m_cachedTransform;
	};
}

DECLEAR_TYPE(Runtime, Transform, Runtime::ComponentBase);