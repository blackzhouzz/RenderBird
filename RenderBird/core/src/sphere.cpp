#include "Sphere.h"

namespace Core
{
	const Sphere Sphere::ZERO(C_Zero_v3f, 0.0f);
	Sphere::Sphere(const Vector3f& center, Float radius)
		:center(center), radius(radius)
	{
	}

	//Sphere Sphere::TransformBy(const Matrix4f& mat)const
	//{
	//	Sphere ret;
	//	ret.center = center * mat;
	//	ret.radius = radius;
	//	return ret;
	//}
}