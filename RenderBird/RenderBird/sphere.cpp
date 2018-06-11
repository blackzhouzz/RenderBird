#include "LNCorePrivate.h"
#include "LNSphere.h"

namespace Luna
{
	const Sphere Sphere::ZERO(Vector3Float::ZERO, 0.0f);
	Sphere::Sphere(const Vector3Float& center, float radius)
		:mCenter(center), mRadius(radius)
	{
	}

	//Sphere Sphere::TransformBy(const Matrix4f& mat)const
	//{
	//	Sphere ret;
	//	ret.mCenter = mCenter * mat;
	//	ret.mRadius = mRadius;
	//	return ret;
	//}
}