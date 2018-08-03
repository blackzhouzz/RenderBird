#include "math/sphere.h"

namespace Core
{
	const Sphere Sphere::ZERO(Vector3f::ZERO, 0.0f);
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