#pragma once
#include "core_private.h"
#include "core_register.h"
#include "component.h"
#include "typeinfo.h"
#include "math_utils.h"

namespace Core
{
	struct Transform
	{
		Vector3f m_pos;
		Quaternion m_rotation;
		Vector3f m_scale;
		Matrix4f m_matrix;
	};

	struct TransformUtils
	{
		static Matrix4f GetMatrix(Transform* trans);
		static void LookAt(Transform* trans, const Vector3f& eyePos, const Vector3f& at, const Vector3f& up);
		//static void LookDir(Transform* trans, const Vector3f& eyePos, const Vector3f& dir, const Vector3f& up);
	};
}

DECLEAR_TYPE_COMPONENT(Core, Transform);

DEFAULT_BEGIN(Core, Transform)
DEFAULT_DATA(Core::C_Zero_v3f)
DEFAULT_DATA(Core::C_Identity_qf)
DEFAULT_DATA(Core::C_One_v3f)
DEFAULT_DATA(Core::C_Identity_m4f)
DEFAULT_END()
