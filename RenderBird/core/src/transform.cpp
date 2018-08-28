#include "transform.h"

IMPLEMENT_TYPE(Core, Transform)

namespace Core
{
	Matrix4f TransformUtils::GetMatrix(Transform* trans)
	{
		return MathUtils::TRS(trans->m_pos, trans->m_rotation, trans->m_scale);
	}

	void TransformUtils::LookAt(Transform* trans, const Vector3f& eyePos, const Vector3f& at, const Vector3f& up)
	{
		Vector3f forward = (at - eyePos).Normalized();
		trans->m_matrix = Matrix4f::LookAt(at, eyePos, up);
		trans->m_rotation = Quaternion::FromMatrix(trans->m_matrix);
		trans->m_pos = trans->m_matrix.TranslationVector3D();
	}
}

