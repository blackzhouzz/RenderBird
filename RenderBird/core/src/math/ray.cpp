#include "math/ray.h"

namespace Core
{
	const Float Ray::DefaultMaxT = 10000.0;

	Ray Ray::TransformRay(const Ray& ray, const Matrix4f& mat)
	{
		Ray ret = ray;
		ret.m_origin = mat.TransformPoint(ray.m_origin);
		ret.m_direction = mat.TransformDirection(ray.m_direction);
		return ret;
	}
}