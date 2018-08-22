#include "ray.h"
#include "math_utils.h"

namespace Core
{
	const Float Ray::DefaultMaxT = 10000.0;

	Ray Ray::TransformBy(const Ray& ray, const Matrix4f& mat)
	{
		Ray ret = ray;
		ret.m_origin = mat * ray.m_origin;
		ret.m_direction = MathUtils::TransformDirection(mat, ray.m_direction);
		return ret;
	}
}