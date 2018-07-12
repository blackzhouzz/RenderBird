#include "math/sphere.h"

namespace Core
{
	const Sphere Sphere::ZERO(Vector3f::ZERO, 0.0);
	Sphere::Sphere(const Vector3f& center, float radius)
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