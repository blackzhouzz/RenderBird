#include "shape.h"

namespace RenderBird
{
	Float Shape::Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)
	{
		Float pdf = 1.0f / GetArea();

		Vector3f vecLight = hitInfo.m_pos - ss->m_pos;
		pdf *= vecLight.LengthSquared() / std::abs(Vector3f::DotProduct(hitInfo.m_n, -wi));
		return pdf;
	}

	Vector2f Shape::GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal)
	{
		Vector2f ret(C_FLOAT_MAX, -C_FLOAT_MAX);
		auto boudingBox = GetBoundingBox(localToWorld);
		for (uint32 i = 0; i < 8; ++i)
		{
			const Vector3f vert = boudingBox.GetVertex(i);
			Float d = Vector3f::DotProduct(normal, vert);
			ret[0] = d < ret[0] ? d : ret[0];
			ret[1] = d > ret[1] ? d : ret[1];
		}
		return ret;
	}
}