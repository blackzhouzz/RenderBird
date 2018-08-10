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
		Vector3f forward = (at - eyePos).Normalize();
		Vector3f right = Cross(up, forward).Normalize();
		if (right.LengthSQ() < C_FLOAT_EPSILON)
		{
			right = Vector3f::UNIT_X;
		}
		Vector3f newup = Cross(forward, right).Normalize();
		trans->m_position = eyePos;
		trans->m_rotation = MathUtils::AxesToQuaternion(right, forward, newup);
		trans->m_cached = MathUtils::MakeTransform(trans->m_position, trans->m_rotation, trans->m_scale);
		trans->m_needUpdate = false;
	}

	void TransformUtils::LookDir(Transform* trans, const Vector3f& eyePos, const Vector3f& dir, const Vector3f& up)
	{
		Vector3f forward = (dir).GetNormalized();
		Vector3f right = Cross(up, forward).Normalize();
		if (right.LengthSQ() < C_FLOAT_EPSILON)
		{
			right = Vector3f::UNIT_X;
		}
		Vector3f newup = Cross(forward, right).Normalize();
		//Matrix3f.FromAxes()
		Matrix3f rot;
		rot.FromAxes(right, forward, newup);
		auto vec = rot * Vector3f::UNIT_Y;
		trans->m_position = eyePos;
		trans->m_rotation = MathUtils::AxesToQuaternion(right, forward, newup);
		trans->m_cached = MathUtils::MakeTransform(trans->m_position, trans->m_rotation, trans->m_scale);
		trans->m_needUpdate = false;
	}
}

