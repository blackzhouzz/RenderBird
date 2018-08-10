#pragma once
#include "core_private.h"
#include "component.h"
#include "math/vector.h"
#include "math/quaternion.h"
#include "math/matrix.h"
#include "typeinfo.h"

namespace Core
{
	struct Transform
	{
		Vector3f m_position;
		Quaternion m_rotation;
		Vector3f m_scale;
		bool m_needUpdate;
		Matrix4f m_cached;
	};

	struct TransformUtils
	{
		static Matrix4f GetMatrix(Transform* trans);
		static void TryUpdateMatrix(Transform* trans);
		static void LookAt(Transform* trans, const Vector3f& eyePos, const Vector3f& at, const Vector3f& up);
		static void LookDir(Transform* trans, const Vector3f& eyePos, const Vector3f& dir, const Vector3f& up);
	};
}

DECLEAR_TYPE_COMPONENT(Core, Transform);

DEFAULT_BEGIN(Core, Transform)
DEFAULT_DATA(Core::Vector3f::ZERO)
DEFAULT_DATA(Core::Quaternion::IDENTITY)
DEFAULT_DATA(Core::Vector3f::ONE)
DEFAULT_DATA(false)
DEFAULT_DATA(Core::Matrix4f::IDENTITY)
DEFAULT_END()
