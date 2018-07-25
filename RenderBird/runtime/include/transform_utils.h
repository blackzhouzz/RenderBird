#pragma once
#include "transform.h"

namespace Runtime
{
	class TransformUtils
	{
	public:
		static const Matrix4f& GetMatrix(Transform* trans);
		static void TryUpdateMatrix(Transform* trans);
		static void LookAt(Transform* trans, const Vector3f& eyePos, const Vector3f& at, const Vector3f& up);
	};
}