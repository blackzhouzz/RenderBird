#include "transform.h"
#include "math/mathutils.h"

IMPLEMENT_TYPE(Core, Transform)

namespace Core
{
	Matrix4f TransformUtils::GetMatrix(Transform* trans)
	{
		return MathUtils::MakeTransform(trans->m_position, trans->m_rotation, trans->m_scale);
	}

	void TransformUtils::TryUpdateMatrix(Transform* trans)
	{
		if (trans->m_needUpdate)
		{
			trans->m_cached = MathUtils::MakeTransform(trans->m_position, trans->m_rotation, trans->m_scale);
			trans->m_needUpdate = false;
		}
	}

	void TransformUtils::LookAt(Transform* trans, const Vector3f& eyePos, const Vector3f& at, const Vector3f& up)
	{
		Vector3f axisZ = (at - eyePos).Normalize();
		Vector3f axisX = Cross(up, axisZ).Normalize();
		if (axisX.LengthSQ() < C_FLOAT_EPSILON)
		{
			axisX = Vector3f::UNIT_X;
		}
		Vector3f axisY = Cross(axisZ, axisX).Normalize();
		trans->m_position = Vector3f(-Dot(axisX, eyePos), -Dot(axisZ, eyePos), -Dot(axisY, eyePos));
		trans->m_rotation = MathUtils::AxesToQuaternion(axisX, axisY, axisZ);
		trans->m_cached = MathUtils::MakeTransform(trans->m_position, trans->m_rotation, trans->m_scale);
		trans->m_needUpdate = false;
	}
}

